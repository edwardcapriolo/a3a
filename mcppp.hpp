#ifndef a3a_mcppp_hpp
#define a3a_mcppp_hpp

#include <seastar/core/seastar.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/app-template.hh>
#include <seastar/http/routes.hh>
#include <seastar/http/request.hh>
#include <seastar/http/function_handlers.hh>
#include <seastar/http/httpd.hh>
#include <seastar/core/sleep.hh>
#include <seastar/coroutine/all.hh>

namespace a3a {

using namespace std;
using namespace seastar;

class mcppp_session{
public:
  string id;
};
class mcppp {
private:
    std::map<string, mcppp_session> data;
};

class sharded_mcppp {
public:
    sharded_mcppp(seastar::distributed<mcppp>& peers) :
    _peers(peers) {

    }
private:
  seastar::distributed<mcppp>& _peers;
};

}
#endif
