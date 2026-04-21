#include "asio.hpp"
#include <print>
#include <system_error>

void my_print(const std::error_code & /*e*/, asio::steady_timer *t,
              int *count) {
  if (*count < 5) {
    std::println("count: {}", *count);
    ++(*count);

    t->expires_at(t->expiry() + asio::chrono::seconds(1));

    // Replace std::bind call with lambda
    auto on_timer_finish = [t, count](const std::error_code &ec) -> auto {
      my_print(ec, t, count);
    };
    t->async_wait(on_timer_finish);
  }
}

void test_asio() {
  std::println("Testing asio function now!");
  asio::io_context io;
  int count = 0;
  asio::steady_timer t(io, asio::chrono::seconds(1));

  // Replace std::bind call with lambda
  auto on_timer_finish = [&](const std::error_code &ec) -> auto {
    my_print(ec, &t, &count);
  };
  t.async_wait(on_timer_finish); // asynchronous wait
  io.run();

  std::println("Final count is: {}", count);
}

int main(int argc, char *argv[]) {
  test_asio();
  return 0;
}
