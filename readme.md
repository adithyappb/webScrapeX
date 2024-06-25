## WebScrapeX: A Concurrent Web Crawler

WebScrapeX is a concurrent web crawler implemented in C++ using modern C++ threading features. This project demonstrates efficient use of a thread pool, synchronization primitives, and network operations using libcurl. The crawler fetches web pages, extracts links, and saves the crawled data to a file, all within a specified timeout period.

# Key Features

- **Concurrency**: Utilizes a thread pool to manage worker threads efficiently.
- **Synchronization**: Uses mutexes and condition variables for thread-safe operations.
- **Network Operations**: Handles HTTP requests using libcurl.
- **Timeout Mechanism**: Ensures the crawler terminates after gathering data for a maximum of 30 seconds.

# Dependencies

- `libcurl` for HTTP requests.
- CMake for building the project.

# Installation and Prerequisites
- Ensure `libcurl` is installed on your system.

# Build and Run:

# Create a build directory and navigate to it.
- mkdir build
- cd build

# Configure the project with CMake.
- cmake ..

# Build the project.
- make

# Run the executable.
- ./ConcurrentWebCrawler
