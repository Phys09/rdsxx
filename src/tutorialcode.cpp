//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <asio.hpp>
#include <functional>
#include <iostream>
#include <system_error>

using namespace std::chrono_literals;

void print(const std::error_code & /*e*/, asio::steady_timer *t, int *count) {
  if (*count < 5) {
    std::cout << *count << std::endl;
    ++(*count);

    t->expires_at(t->expiry() + 1s);
    t->async_wait([t, count](const auto &ec) { print(ec, t, count); });
  }
}

int main() {
  asio::io_context io;

  int count = 0;
  asio::steady_timer t(io, 1s);
  // t.async_wait([ptTimer = &t, ptCount = &count] (const std::error_code & ec)
  // { print(ec, ptTimer, ptCount); });
  t.async_wait([&](const std::error_code &ec) { print(ec, &t, &count); });

  io.run();

  std::cout << "Final count is " << count << std::endl;

  return 0;
}
