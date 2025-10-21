#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <string>
#include <map>

/**
 * Simple HTTP Client for connecting to websites
 * This class provides basic HTTP functionality to connect to and communicate with web servers
 */
class HttpClient {
public:
    struct HttpResponse {
        int status_code;
        std::string headers;
        std::string body;
        bool success;
    };

    HttpClient();
    ~HttpClient();

    /**
     * Perform a GET request to the specified URL
     * @param url The URL to connect to (e.g., "http://example.com" or "https://google.com")
     * @return HttpResponse containing the server response
     */
    HttpResponse get(const std::string& url);

    /**
     * Perform a POST request to the specified URL
     * @param url The URL to connect to
     * @param data The data to send in the request body
     * @return HttpResponse containing the server response
     */
    HttpResponse post(const std::string& url, const std::string& data);

    /**
     * Set custom headers for requests
     * @param headers Map of header name to header value
     */
    void setHeaders(const std::map<std::string, std::string>& headers);

    /**
     * Set connection timeout in seconds
     * @param timeout_seconds Timeout value in seconds
     */
    void setTimeout(int timeout_seconds);

private:
    void* curl_handle; // CURL handle (void* to avoid including curl headers)
    std::map<std::string, std::string> custom_headers;
    int timeout;

    // Helper function to initialize curl
    bool initializeCurl();
    
    // Helper function to cleanup curl
    void cleanupCurl();
};

#endif // HTTP_CLIENT_HPP