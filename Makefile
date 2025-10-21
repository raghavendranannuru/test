# Makefile for C++ Website Connection implementations

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Default target
all: simple_client full_client

# Simple client (no external dependencies)
simple_client: simple_main.cpp simple_http_client.cpp
	$(CXX) $(CXXFLAGS) -o simple_client simple_main.cpp simple_http_client.cpp

# Full client with libcurl (requires libcurl-dev)
full_client: main.cpp http_client.cpp
	$(CXX) $(CXXFLAGS) -o full_client main.cpp http_client.cpp -lcurl

# Test if curl is available
test_curl:
	@echo "Testing if libcurl is available..."
	@pkg-config --exists libcurl && echo "✓ libcurl found" || echo "✗ libcurl not found - install with: sudo apt-get install libcurl4-openssl-dev"

# Clean build artifacts
clean:
	rm -f simple_client full_client

# Install dependencies on Ubuntu/Debian
install_deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libcurl4-openssl-dev cmake

# Run the simple client
run_simple: simple_client
	./simple_client

# Run the full client (requires libcurl)
run_full: full_client
	./full_client

.PHONY: all clean test_curl install_deps run_simple run_full