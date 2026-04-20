#include "asio.hpp"
#include "asio/detail/chrono.hpp"
#include <print>

void test_asio() {
  std::println("Testing asio function now!");
  asio::io_context io;
  auto wait_time = asio::chrono::seconds(5);
  asio::steady_timer t(io, wait_time);
  t.wait();
  std::println("Finished waiting for time");
}

int main(int argc, char *argv[]) {
  test_asio();
  return 0;
}
