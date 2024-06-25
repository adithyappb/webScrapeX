cmake_minimum_required(VERSION 3.10)
project(ConcurrentWebCrawler)

set(CMAKE_CXX_STANDARD 17)
find_package(CURL REQUIRED)

add_executable(ConcurrentWebCrawler main.cpp WebCrawler.cpp)
target_link_libraries(ConcurrentWebCrawler CURL::libcurl)

