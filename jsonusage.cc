#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;
using namespace std;

namespace a3a {
class server {
private:
    string name;
    string version;
};

template <class Item>
class response_error{
public:
    typedef Item value_type;
    int64_t code;
    string message;
    value_type data; // unknown
};
template <class Item>
void to_json(json& j, response_error<Item>& r){
    j = json{
        {"code", r.code},
        {"data", r.data}
    };
}
template <class Item>
void from_json(const json& j, response_error<Item>& r){
    j.at("code").get_to(r.code);
    j.at("data").get_to(r.data);
}

class response {
private:

public:
    string jsonrpc;
    string id;
    string result;
    response_error<string> error;// ?object
};

using json = nlohmann::json;

void to_json(json& j, const response& r){

    j = json{   {"jsonrpc", r.jsonrpc},
                {"id", r.id},
    };

    //json x = r.error;
    j["error"] = json::object();
    j["error"] = {
        {"code", r.error.code}
    };

}

void from_json(const json& j, response& r){
    j.at("jsonrpc").get_to(r.jsonrpc);
}

}//end namespace

int main(int argc, char** argv) {
    a3a::response_error<string> err {404, "failed", "really failed"};
    a3a::response r  {"rpc", "id", "result", err};
    json j = r;
    std::cout << j << std::endl;
    return 0;
}
