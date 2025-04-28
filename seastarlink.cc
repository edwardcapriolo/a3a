#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <iostream>

int main(int argc, char** argv) {
  seastar::app_template app;
  app.run(argc, argv, [] {
    std::cout << "Jello world\n";
    std::cout << seastar::smp::count << "\n";
    return seastar::make_ready_future<>();
  });
}

