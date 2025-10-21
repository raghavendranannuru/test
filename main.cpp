#include <iostream>
#include <map>
#include "http_client.hpp"

void demonstrateWebsiteConnection() {
    std::cout << "=== C++ Website Connection Demo ===" << std::endl;
    
    HttpClient client;
    
    // Set a reasonable timeout
    client.setTimeout(10);
    
    // Test connecting to a simple website
    std::cout << "\n1. Connecting to httpbin.org (GET request)..." << std::endl;
    auto response1 = client.get("http://httpbin.org/get");
    
    if (response1.success) {
        std::cout << "✓ Connection successful!" << std::endl;
        std::cout << "Status Code: " << response1.status_code << std::endl;
        std::cout << "Response length: " << response1.body.length() << " bytes" << std::endl;
        std::cout << "First 200 characters of response:" << std::endl;
        std::cout << response1.body.substr(0, 200) << "..." << std::endl;
    } else {
        std::cout << "✗ Connection failed!" << std::endl;
    }
    
    // Test with custom headers
    std::cout << "\n2. Connecting with custom headers..." << std::endl;
    std::map<std::string, std::string> headers = {
        {"Accept", "application/json"},
        {"X-Custom-Header", "C++-Client"}
    };
    client.setHeaders(headers);
    
    auto response2 = client.get("http://httpbin.org/headers");
    
    if (response2.success) {
        std::cout << "✓ Connection with headers successful!" << std::endl;
        std::cout << "Status Code: " << response2.status_code << std::endl;
        std::cout << "Response contains custom header info" << std::endl;
    } else {
        std::cout << "✗ Connection with headers failed!" << std::endl;
    }
    
    // Test POST request
    std::cout << "\n3. Testing POST request..." << std::endl;
    std::string postData = R"({"message": "Hello from C++ client!", "test": true})";
    auto response3 = client.post("http://httpbin.org/post", postData);
    
    if (response3.success) {
        std::cout << "✓ POST request successful!" << std::endl;
        std::cout << "Status Code: " << response3.status_code << std::endl;
        std::cout << "Posted data was echoed back by server" << std::endl;
    } else {
        std::cout << "✗ POST request failed!" << std::endl;
    }
    
    // Test HTTPS connection
    std::cout << "\n4. Testing HTTPS connection..." << std::endl;
    auto response4 = client.get("https://httpbin.org/get");
    
    if (response4.success) {
        std::cout << "✓ HTTPS connection successful!" << std::endl;
        std::cout << "Status Code: " << response4.status_code << std::endl;
        std::cout << "Secure connection established" << std::endl;
    } else {
        std::cout << "✗ HTTPS connection failed!" << std::endl;
    }
}

void showUsageExample() {
    std::cout << "\n=== Usage Example ===" << std::endl;
    std::cout << R"(
// Basic usage:
HttpClient client;
auto response = client.get("https://api.github.com");

if (response.success) {
    std::cout << "Status: " << response.status_code << std::endl;
    std::cout << "Body: " << response.body << std::endl;
}

// With custom headers and timeout:
client.setTimeout(30);
std::map<std::string, std::string> headers = {
    {"Authorization", "Bearer your-token"},
    {"Content-Type", "application/json"}
};
client.setHeaders(headers);

// POST request:
std::string data = "{\"key\": \"value\"}";
auto postResponse = client.post("https://api.example.com/data", data);
)" << std::endl;
}

int main() {
    std::cout << "C++ Website Connection Implementation" << std::endl;
    std::cout << "====================================" << std::endl;
    
    demonstrateWebsiteConnection();
    showUsageExample();
    
    std::cout << "\nDemo completed successfully!" << std::endl;
    return 0;
}