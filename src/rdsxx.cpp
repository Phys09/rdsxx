#include "asio.hpp"
#include "asio/bind_executor.hpp"
#include "asio/detail/chrono.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include "asio/strand.hpp"
#include <functional>
#include <print>
#include <system_error>
#include <thread>

struct Printer {
public:
  Printer(asio::io_context &io) noexcept
      : strand_(asio::make_strand(io)), timer_1(io, asio::chrono::seconds(1)),
        timer_2(io, asio::chrono::seconds(1)), count_(0) {
    timer_1.async_wait([this](const std::error_code &ec) { this->print1(); });
    timer_2.async_wait([this](const std::error_code &ec) { this->print2(); });
  }
  void print1() {
    using namespace std::chrono_literals;
    if (count_ < 10) {
      std::println("Timer 1: {}", count_);
      ++count_;

      timer_1.expires_at(timer_1.expiry() + 1s);
      timer_1.async_wait(asio::bind_executor(
          strand_, [this](const std::error_code &ec) { print1(); }));
    }
  }
  void print2() {
    using namespace std::chrono_literals;
    if (count_ < 10) {
      std::println("Timer 2: {}", count_);
      ++count_;

      timer_2.expires_at(timer_2.expiry() + 1s);
      timer_2.async_wait(asio::bind_executor(
          strand_, [this](const std::error_code &ec) { print2(); }));
    }
  }

  ~Printer() { std::println("Final count is: {}", count_); }

private:
  asio::strand<asio::io_context::executor_type> strand_;
  asio::steady_timer timer_1;
  asio::steady_timer timer_2;
  int count_;
};

void test_asio() {
  std::println("Testing asio function now!");
  asio::io_context io;
  auto p = Printer(io);

  std::thread t([&] { io.run(); });

  auto num_handlers_executed = io.run();
  std::println("Number of handlers executed by io_context: {}",
               num_handlers_executed);
  t.join();
}

int main(int argc, char *argv[]) {
  test_asio();
  return 0;
}
