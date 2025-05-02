#include <vector>
#include "request.h"
#include <seastar/core/seastar.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/app-template.hh>
#include <seastar/http/routes.hh>
#include <seastar/http/request.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/httpd.hh>
#include <seastar/core/sleep.hh>
#include <seastar/coroutine/all.hh>
#include <seastar/util/log.hh>
#include <iostream>
#include <chrono>

using namespace seastar;
using namespace httpd;
using namespace std::chrono_literals;

using namespace seastar;
using namespace seastar::httpd;
using namespace std;
using namespace a3a;

vector<a3a::resource> list_resources(){
    a3a::resource r1 = { "file:///hobbies.txt", "my-hobbies", "a list of my hobbies one per line."};
    vector<a3a::resource> result;
    result.push_back(r1);
    return result;
}

void set_routes(routes& r) {
    function_handler* h3 = new function_handler([](const_req req) {
        using json = nlohmann::json;
        json j = list_resources();
        return j.dump();
    });
    r.add(operation_type::GET, url("/resources/list"), h3);
}

int main(int argc, char** argv) {
    app_template app;
    return app.run(argc, argv, [] () -> future<int> {
        auto server = new http_server("mcpplusplus");
        set_routes(server->_routes);
        co_await server->listen(seastar::make_ipv4_address({1235}));
        co_await seastar::sleep(10000s);
        co_await server->stop();
        delete server;
        co_return 0;
    });
}

void set_routs2(routes& r) {
    function_handler* h2 = new function_handler([](std::unique_ptr<http::request> req) -> future<seastar::json::json_return_type> {
        co_return seastar::json::json_return_type("json-future");
    });
    r.add(operation_type::GET, url("/fast"), h2);
}

