#include <iostream>
#include <seastar/core/app-template.hh>


using namespace seastar;
using namespace std;

int main(int argc, char** argv) {
    seastar::app_template app;
    try {
        app.run(argc, argv, [&]{
            return seastar::make_ready_future<>();
        });
    } catch(...) {
        std::cerr << "Couldn't start application: "
        << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}






