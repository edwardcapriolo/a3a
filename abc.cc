#include <iostream>
#include <seastar/core/app-template.hh>
#include <set>
#include <vector>
#include <seastar/core/distributed.hh>
#include <seastar/net/api.hh>
#include <seastar/core/sstring.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/future-util.hh>
#include <seastar/net/api.hh>
#include <iostream>
#include <seastar/core/sharded.hh>
#include <unordered_set>
#include <seastar/core/distributed.hh>
#include <seastar/core/coroutine.hh>
#include <iterator>                                         //for std::ostream_iterator
#include <algorithm>                                        //for std::copy
#include <iostream>                                         //for std::cout
#include <vector>
#include <set>
#include <sstream>
#include <bits/stdc++.h>


using namespace std;
using namespace seastar;

vector<string> split_sentence(string sen) {
    stringstream ss(sen);
    string word;
    vector<string> words;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

class my_datum {
public:
    std::string something;
    bool operator==(const my_datum& other) const  {
        return this->something == other.something;
    }
    bool operator< (const my_datum& other) const{
        return something < other.something;
    }
private:
};

namespace std {
    template <>
    struct hash<my_datum> {
        size_t operator()(const my_datum& p) const {
            return std::hash<string>{} (p.something);
        }
    };
}



struct local_origin_tag {
    template <typename T>
    static inline
    T move_if_local(T& ref) {
        return std::move(ref);
    }
};

struct remote_origin_tag {
    template <typename T>
    static inline
    T move_if_local(T& ref) {
        return ref;
    }
};

class cache {
private:
    std::set<my_datum> data;
public:

    template <typename Origin = local_origin_tag>
    bool insert(my_datum& my_datum){
        data.insert(my_datum);
        print_me();
        return true;
    }
    template <typename Origin = local_origin_tag>
    void put(my_datum& my_datum){
        data.insert(my_datum);
        print_me();
    }
    void print_me(){
        std::cout << "size: " << data.size() << "\n";
        for (my_datum person : data){
            std::cout << person.something << " ";
        }
        std::cout << "\n";
    }
    template <typename Origin = local_origin_tag>
    bool exists(my_datum & my_datum){
        return data.contains(my_datum);
    }
};



class sharded_cache {
private:
    seastar::distributed<cache>& _peers;
    inline
    unsigned get_shard(const my_datum& data){
        std::hash<my_datum> hasher;
        auto x = hasher(data);
        std::cout << "shard " << x << "\n";
        std::cout << "smp " << seastar::smp::count << "\n";
        return x % seastar::smp::count;
    }

public:
    sharded_cache(seastar::distributed<cache>& peers) : _peers(peers) {

    }
    seastar::future<bool> insert(my_datum& my_datum){
        auto cpu = get_shard(my_datum);
        if (seastar::this_shard_id() == cpu){
            return seastar::make_ready_future<bool>( _peers.local().insert(my_datum));
        } else {
            return _peers.invoke_on(cpu, &cache::insert<local_origin_tag>, std::ref(my_datum)  );
        }
    }
    seastar::future<> put(my_datum& my_datum){
        auto cpu = get_shard(my_datum);
        if (seastar::this_shard_id() == cpu){
            _peers.local().insert(my_datum);
            return seastar::make_ready_future<>();
        } else {
            return _peers.invoke_on(cpu, &cache::put<local_origin_tag>, std::ref(my_datum)  );
        }
    }

    seastar::future<bool> exists(my_datum& my_datum){
        auto cpu = get_shard(my_datum);
        if (seastar::this_shard_id() == cpu){
            return seastar::make_ready_future<bool>( _peers.local().exists(my_datum));
        } else {
            //return _peers.invoke_on(cpu, &cache::put<local_origin_tag>, std::ref(my_datum)  );
            seastar::future<bool> x =  _peers.invoke_on(cpu, &cache::exists<local_origin_tag>, std::ref(my_datum));
            return x;
        }
    }

};

class tcp_server {
public:
    tcp_server(sharded_cache& cache) : _cache(cache){

    }
    seastar::future<> init() {
        seastar::listen_options lo;
        lo.reuse_address = true;
        return seastar::do_with(
            seastar::listen(seastar::make_ipv4_address({1234}), lo),
                                this,
                                [] (auto& listener, auto& me) {
                                    return seastar::keep_doing([&listener, &me] () {
                                        return listener.accept().then(
                                            [&] (seastar::accept_result res) {
                                                (void) me->handle_connection(std::move(res.connection)).handle_exception(
                                                    [] (std::exception_ptr ep) {
                                                        fmt::print(stderr, "Could not handle connection: {}\n", ep);
                                                    });
                                            });
                                    });
                                });
    }
private:

    seastar::future<> handle_connection(seastar::connected_socket s) {
        seastar::output_stream<char> out = s.output();
        seastar::input_stream<char> in = s.input();
        return do_with(std::move(s), std::move(out), std::move(in), this,
                       [] (auto& s, auto& out, auto& in, auto& me) {
                           return seastar::repeat([&out, &in, &me] {
                               return in.read().then([&out, &me] (seastar::temporary_buffer<char> buf) {
                                   std::string* tmp = new std::string(buf.get(), buf.size() - 2);
                                   std:cout << "size " << buf.size() << std::endl;
                                   vector<string> words = split_sentence(*tmp);
                                   if (buf) {
                                        if (words[0] == "put"){
                                            my_datum* d = new my_datum();
                                            d->something = words[1];
                                            return me->_cache.put(*d).then([&]{
                                                return out.write("ok\n").then([&]{
                                                    return out.flush();
                                                });
                                            }).then([] { return seastar::stop_iteration::no; });
                                        } if (words[0] == "exists"){
                                            my_datum* d = new my_datum();
                                            d->something = words[1];
                                            return me->_cache.exists(*d).then([&] (auto found){
                                                if (found){
                                                    return out.write("found\n").then([&]{ return out.flush(); });
                                                } else {
                                                    return out.write("not found\n").then([&] {return out.flush(); });
                                                }
                                            }).then([] { return seastar::stop_iteration::no; });
                                        } else {
                                            return out.write("nomatch\n").then([&] {
                                                return out.flush();
                                            }).then([] { return seastar::stop_iteration::no; });
                                        }
                                    } else {
                                       return seastar::make_ready_future<seastar::stop_iteration>(seastar::stop_iteration::yes);
                                   }
                               });
                           }).then([&out] {
                               return out.close();
                           });
                       });
    }

    sharded_cache& _cache;
};


int main(int argc, char** argv) {
    seastar::app_template app;
    seastar::distributed<cache> cache_peers;
    sharded_cache cache(cache_peers);
    seastar::distributed<tcp_server> tcp_server;
    try {
        app.run(argc, argv, [&]{
            return cache_peers.start()
            .then([&] { return tcp_server.start(std::ref(cache)); })
            .then([&] { return tcp_server.invoke_on_all(&tcp_server::init); })
            ;
        });
    } catch(...) {
        std::cerr << "Couldn't start application: "
        << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}





