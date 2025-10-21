#include <iostream>
#include "simple_http_client.hpp"

void demonstrateSimpleWebsiteConnection() {
    std::cout << "=== Simple C++ Website Connection Demo ===" << std::endl;
    std::cout << "(Using raw sockets - HTTP only, no external dependencies)" << std::endl;
    
    SimpleHttpClient client;
    
    // Set a reasonable timeout
    client.setTimeout(10);
    
    // Test connecting to a simple HTTP website
    std::cout << "\n1. Connecting to httpbin.org (HTTP GET request)..." << std::endl;
    auto response1 = client.get("http://httpbin.org/get");
    
    if (response1.success) {
        std::cout << "✓ Connection successful!" << std::endl;
        std::cout << "Status Code: " << response1.status_code << std::endl;
        std::cout << "Response length: " << response1.body.length() << " bytes" << std::endl;
        std::cout << "First 200 characters of response:" << std::endl;
        std::cout << response1.body.substr(0, 200) << "..." << std::endl;
    } else {
        std::cout << "✗ Connection failed: " << response1.error_message << std::endl;
    }
    
    // Test another simple website
    std::cout << "\n2. Connecting to example.com..." << std::endl;
    auto response2 = client.get("http://example.com");
    
    if (response2.success) {
        std::cout << "✓ Connection successful!" << std::endl;
        std::cout << "Status Code: " << response2.status_code << std::endl;
        std::cout << "Response contains: " << response2.body.length() << " bytes" << std::endl;
        // Show a snippet of the HTML
        size_t title_pos = response2.body.find("<title>");
        if (title_pos != std::string::npos) {
            size_t title_end = response2.body.find("</title>", title_pos);
            if (title_end != std::string::npos) {
                std::string title = response2.body.substr(title_pos + 7, title_end - title_pos - 7);
                std::cout << "Page title: " << title << std::endl;
            }
        }
    } else {
        std::cout << "✗ Connection failed: " << response2.error_message << std::endl;
    }
    
    // Demonstrate HTTPS limitation
    std::cout << "\n3. Testing HTTPS limitation..." << std::endl;
    auto response3 = client.get("https://httpbin.org/get");
    
    if (response3.success) {
        std::cout << "✓ HTTPS connection successful!" << std::endl;
    } else {
        std::cout << "ℹ Expected limitation: " << response3.error_message << std::endl;
        std::cout << "  (Use the libcurl implementation for HTTPS support)" << std::endl;
    }
}

void showSimpleUsageExample() {
    std::cout << "\n=== Simple Usage Example ===" << std::endl;
    std::cout << R"(
// Basic usage (HTTP only):
SimpleHttpClient client;
auto response = client.get("http://api.example.com/data");

if (response.success) {
    std::cout << "Status: " << response.status_code << std::endl;
    std::cout << "Headers: " << response.headers << std::endl;
    std::cout << "Body: " << response.body << std::endl;
} else {
    std::cout << "Error: " << response.error_message << std::endl;
}

// With custom timeout:
client.setTimeout(30);
auto response2 = client.get("http://slow-website.com");
)" << std::endl;
}

int main() {
    std::cout << "Simple C++ Website Connection Implementation" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    demonstrateSimpleWebsiteConnection();
    showSimpleUsageExample();
    
    std::cout << "\nDemo completed!" << std::endl;
    std::cout << "\nNote: For HTTPS support and advanced features, use the libcurl implementation:" << std::endl;
    std::cout << "Build with: cmake . && make" << std::endl;
    std::cout << "Run with: ./website_connection" << std::endl;
    
    return 0;
}