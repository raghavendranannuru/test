#ifndef SIMPLE_HTTP_CLIENT_HPP
#define SIMPLE_HTTP_CLIENT_HPP

#include <string>
#include <map>

/**
 * Simple HTTP Client using raw sockets (no external dependencies)
 * This implementation works on Unix-like systems (Linux, macOS)
 */
class SimpleHttpClient {
public:
    struct SimpleHttpResponse {
        int status_code;
        std::string headers;
        std::string body;
        bool success;
        std::string error_message;
    };

    SimpleHttpClient();
    ~SimpleHttpClient();

    /**
     * Perform a GET request to the specified URL
     * @param url The URL to connect to (e.g., "http://example.com/path")
     * @return SimpleHttpResponse containing the server response
     */
    SimpleHttpResponse get(const std::string& url);

    /**
     * Set connection timeout in seconds
     * @param timeout_seconds Timeout value in seconds
     */
    void setTimeout(int timeout_seconds);

private:
    int timeout;

    // Helper functions
    bool parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& use_ssl);
    int connectToHost(const std::string& host, int port);
    bool sendHttpRequest(int socket_fd, const std::string& method, const std::string& path, const std::string& host);
    SimpleHttpResponse receiveHttpResponse(int socket_fd);
    void closeSocket(int socket_fd);
    std::string urlEncode(const std::string& str);
};

#endif // SIMPLE_HTTP_CLIENT_HPP