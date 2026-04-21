cmake_minimum_required(VERSION 4.3.0)
project(rdsxx VERSION 0.0.1 DESCRIPTION "Redis in C++" LANGUAGES CXX)

# Set c++ standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)

# Export compile commands for clangd/lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(tutorialcode "./src/tutorialcode.cpp" "./vendored/asio/include/asio.hpp")
add_executable(rdsxxmain "./src/rdsxx.cpp" "./vendored/asio/include/asio.hpp")
target_include_directories(tutorialcode PUBLIC "./vendored/asio/include")
target_include_directories(rdsxxmain PUBLIC "./vendored/asio/include/")

