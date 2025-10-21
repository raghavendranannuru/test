#include "simple_http_client.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

SimpleHttpClient::SimpleHttpClient() : timeout(30) {
}

SimpleHttpClient::~SimpleHttpClient() {
}

bool SimpleHttpClient::parseUrl(const std::string& url, std::string& host, std::string& path, int& port, bool& use_ssl) {
    use_ssl = false;
    port = 80;
    
    // Check protocol
    size_t protocol_end = url.find("://");
    if (protocol_end == std::string::npos) {
        return false;
    }
    
    std::string protocol = url.substr(0, protocol_end);
    if (protocol == "https") {
        use_ssl = true;
        port = 443;
    } else if (protocol != "http") {
        return false;
    }
    
    // Extract host and path
    size_t start = protocol_end + 3;
    size_t path_start = url.find('/', start);
    
    if (path_start == std::string::npos) {
        host = url.substr(start);
        path = "/";
    } else {
        host = url.substr(start, path_start - start);
        path = url.substr(path_start);
    }
    
    // Check for port in host
    size_t port_pos = host.find(':');
    if (port_pos != std::string::npos) {
        port = std::stoi(host.substr(port_pos + 1));
        host = host.substr(0, port_pos);
    }
    
    return true;
}

int SimpleHttpClient::connectToHost(const std::string& host, int port) {
    struct hostent* server = gethostbyname(host.c_str());
    if (server == nullptr) {
        return -1;
    }
    
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(socket_fd);
        return -1;
    }
    
    return socket_fd;
}

bool SimpleHttpClient::sendHttpRequest(int socket_fd, const std::string& method, const std::string& path, const std::string& host) {
    std::ostringstream request;
    request << method << " " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    request << "User-Agent: SimpleHttpClient/1.0\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    
    std::string request_str = request.str();
    ssize_t bytes_sent = send(socket_fd, request_str.c_str(), request_str.length(), 0);
    
    return bytes_sent == (ssize_t)request_str.length();
}

SimpleHttpClient::SimpleHttpResponse SimpleHttpClient::receiveHttpResponse(int socket_fd) {
    SimpleHttpResponse response = {0, "", "", false, ""};
    
    std::string raw_response;
    char buffer[4096];
    ssize_t bytes_received;
    
    // Read the entire response
    while ((bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        raw_response += buffer;
    }
    
    if (raw_response.empty()) {
        response.error_message = "No response received";
        return response;
    }
    
    // Split headers and body
    size_t header_end = raw_response.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        response.error_message = "Invalid HTTP response format";
        return response;
    }
    
    std::string headers = raw_response.substr(0, header_end);
    response.body = raw_response.substr(header_end + 4);
    response.headers = headers;
    
    // Extract status code
    size_t status_start = headers.find(" ") + 1;
    size_t status_end = headers.find(" ", status_start);
    if (status_start != std::string::npos && status_end != std::string::npos) {
        std::string status_str = headers.substr(status_start, status_end - status_start);
        response.status_code = std::stoi(status_str);
        response.success = true;
    } else {
        response.error_message = "Could not parse status code";
    }
    
    return response;
}

void SimpleHttpClient::closeSocket(int socket_fd) {
    if (socket_fd >= 0) {
        close(socket_fd);
    }
}

SimpleHttpClient::SimpleHttpResponse SimpleHttpClient::get(const std::string& url) {
    SimpleHttpResponse response = {0, "", "", false, ""};
    
    std::string host, path;
    int port;
    bool use_ssl;
    
    if (!parseUrl(url, host, path, port, use_ssl)) {
        response.error_message = "Invalid URL format";
        return response;
    }
    
    if (use_ssl) {
        response.error_message = "HTTPS not supported in simple implementation (use libcurl version)";
        return response;
    }
    
    int socket_fd = connectToHost(host, port);
    if (socket_fd < 0) {
        response.error_message = "Failed to connect to host: " + host;
        return response;
    }
    
    if (!sendHttpRequest(socket_fd, "GET", path, host)) {
        response.error_message = "Failed to send HTTP request";
        closeSocket(socket_fd);
        return response;
    }
    
    response = receiveHttpResponse(socket_fd);
    closeSocket(socket_fd);
    
    return response;
}

void SimpleHttpClient::setTimeout(int timeout_seconds) {
    timeout = timeout_seconds;
}

std::string SimpleHttpClient::urlEncode(const std::string& str) {
    std::ostringstream encoded;
    for (char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::hex << (unsigned char)c;
        }
    }
    return encoded.str();
}