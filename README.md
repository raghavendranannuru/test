# C++ Website Connection Implementation

This repository contains C++ code to connect to websites using HTTP/HTTPS protocols. Two implementations are provided:

## 1. Simple HTTP Client (No Dependencies)
- **Files**: `simple_http_client.hpp`, `simple_http_client.cpp`, `simple_main.cpp`
- **Features**: Basic HTTP GET requests using raw sockets
- **Limitations**: HTTP only (no HTTPS), Unix-like systems only
- **Dependencies**: None (uses standard C++ and POSIX libraries)

## 2. Full HTTP Client (libcurl-based)
- **Files**: `http_client.hpp`, `http_client.cpp`, `main.cpp`
- **Features**: Full HTTP/HTTPS support, GET/POST requests, custom headers, timeouts
- **Dependencies**: libcurl library
- **Platforms**: Cross-platform (Linux, Windows, macOS)

## Quick Start

### Building and Running

#### Option 1: Simple Client (No External Dependencies)
```bash
# Build the simple client
make simple_client

# Run the demo
./simple_client
```

#### Option 2: Full Client (Requires libcurl)
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libcurl4-openssl-dev

# Build using Make
make full_client
./full_client

# OR build using CMake
mkdir build && cd build
cmake ..
make
./website_connection
```

### Usage Examples

#### Simple HTTP Client
```cpp
#include "simple_http_client.hpp"

SimpleHttpClient client;
auto response = client.get("http://example.com");

if (response.success) {
    std::cout << "Status: " << response.status_code << std::endl;
    std::cout << "Body: " << response.body << std::endl;
}
```

#### Full HTTP Client
```cpp
#include "http_client.hpp"

HttpClient client;
client.setTimeout(30);

// GET request
auto response = client.get("https://api.github.com");

// POST request with custom headers
std::map<std::string, std::string> headers = {
    {"Content-Type", "application/json"},
    {"Authorization", "Bearer token"}
};
client.setHeaders(headers);
auto postResponse = client.post("https://api.example.com/data", "{\"key\":\"value\"}");
```

## Features Comparison

| Feature | Simple Client | Full Client |
|---------|---------------|-------------|
| HTTP Support | ✅ | ✅ |
| HTTPS Support | ❌ | ✅ |
| GET Requests | ✅ | ✅ |
| POST Requests | ❌ | ✅ |
| Custom Headers | ❌ | ✅ |
| Timeouts | ⚠️ Basic | ✅ |
| Cross-platform | ❌ (Unix only) | ✅ |
| Dependencies | None | libcurl |

## Installation Requirements

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential libcurl4-openssl-dev cmake
```

### CentOS/RHEL
```bash
sudo yum install gcc-c++ libcurl-devel cmake
```

### macOS
```bash
brew install curl cmake
```

## File Structure
```
├── http_client.hpp          # Full client header
├── http_client.cpp          # Full client implementation
├── simple_http_client.hpp   # Simple client header
├── simple_http_client.cpp   # Simple client implementation
├── main.cpp                 # Full client demo
├── simple_main.cpp          # Simple client demo
├── CMakeLists.txt           # CMake build configuration
├── Makefile                 # Make build configuration
└── README.md                # This file
```

## Testing

The demo applications will test connections to:
- httpbin.org (HTTP testing service)
- example.com (Simple website)
- HTTPS endpoints (full client only)

Both implementations include error handling and provide detailed feedback about connection attempts.

## License

This is a test repository demonstrating C++ website connection capabilities.
