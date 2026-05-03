#include "asio.hpp"
#include "asio/detail/chrono.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include <functional>
#include <print>
#include <system_error>

struct Printer {
public:
  // Constructor
  Printer(asio::io_context &io)
      : timer_(io, asio::chrono::seconds(1)), count_(0) {
    timer_.async_wait([this](const std::error_code &ec) { this->print(); });
  }

  void print() {
    using namespace std::chrono_literals;
    if (count_ < 5) {
      std::println("{}", count_);
      ++count_;

      timer_.expires_at(timer_.expiry() + 1s);
      timer_.async_wait([this](const std::error_code &ec) { print(); });
    }
  }

  ~Printer() { std::println("Final count is: {}", count_); }

private:
  asio::steady_timer timer_;
  int count_;
};

void my_print(const std::error_code &ec, asio::steady_timer *t, int *count) {
  if (*count < 5) {
    std::println("count: {}", *count);
    ++(*count);

    t->expires_at(t->expiry() + asio::chrono::seconds(1));

    // Replace std::bind call with lambda
    auto on_timer_finish = [t, count](const std::error_code &ec_) -> auto {
      my_print(ec_, t, count);
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
