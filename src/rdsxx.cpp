#include "asio/buffer.hpp"
#include "asio/connect.hpp"
#include "asio/error.hpp"
#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include <array>
#include <cstddef>
#include <exception>
#include <ios>
#include <iostream>
#include <print>
#include <ranges>
#include <system_error>

void print_buffer(const std::array<unsigned char, 128> &buffer,
                  const size_t n) {
  for (int i = 0; i < n; ++i) {
    std::print("{}", buffer[i]);
  }
}

int main(int argc, char *argv[]) {
  using asio::ip::tcp;
  try {
    if (argc != 2) {
      std::println(std::cerr, "Usage: Client <host>");
      return 1;
    }

    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
        resolver.resolve(argv[1], "daytime");
    static_assert(std::ranges::range<tcp::resolver::results_type>,
                  "results_type should be a range according to documentation");

    tcp::socket socket(io_context);
    asio::connect(socket, endpoints);

    while (true) {
      std::array<unsigned char, 128> buf;
      std::error_code error;

      size_t len = socket.read_some(asio::buffer(buf), error);

      if (error == asio::error::eof) {
        break; // Connection closed cleanly by peer
      } else if (error) {
        throw std::system_error(error); // some other error
        print_buffer(buf, len);
      }
    }
  } catch (const std::exception &e) {
    std::println("[Exception thrown] {}", e.what());
  }
  return 0;
}
