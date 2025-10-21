#include "http_client.hpp"
#include <curl/curl.h>
#include <sstream>
#include <iostream>

// Callback function for writing received data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

// Callback function for writing header data
static size_t HeaderCallback(void* contents, size_t size, size_t nmemb, std::string* headers) {
    size_t totalSize = size * nmemb;
    headers->append((char*)contents, totalSize);
    return totalSize;
}

HttpClient::HttpClient() : curl_handle(nullptr), timeout(30) {
    initializeCurl();
}

HttpClient::~HttpClient() {
    cleanupCurl();
}

bool HttpClient::initializeCurl() {
    // Initialize curl globally (should be done once)
    static bool curl_global_initialized = false;
    if (!curl_global_initialized) {
        if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
            return false;
        }
        curl_global_initialized = true;
    }

    // Create a curl handle
    curl_handle = curl_easy_init();
    return curl_handle != nullptr;
}

void HttpClient::cleanupCurl() {
    if (curl_handle) {
        curl_easy_cleanup((CURL*)curl_handle);
        curl_handle = nullptr;
    }
}

HttpClient::HttpResponse HttpClient::get(const std::string& url) {
    HttpResponse response = {0, "", "", false};
    
    if (!curl_handle) {
        return response;
    }

    CURL* curl = (CURL*)curl_handle;
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set callback for response body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    
    // Set callback for response headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);
    
    // Follow redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    
    // Set User-Agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "HttpClient/1.0");
    
    // Add custom headers if any
    struct curl_slist* headers_list = nullptr;
    for (const auto& header : custom_headers) {
        std::string header_line = header.first + ": " + header.second;
        headers_list = curl_slist_append(headers_list, header_line.c_str());
    }
    if (headers_list) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_list);
    }
    
    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    
    // Clean up headers list
    if (headers_list) {
        curl_slist_free_all(headers_list);
    }
    
    if (res == CURLE_OK) {
        // Get response code
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        response.status_code = (int)response_code;
        response.success = true;
    } else {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        response.success = false;
    }
    
    return response;
}

HttpClient::HttpResponse HttpClient::post(const std::string& url, const std::string& data) {
    HttpResponse response = {0, "", "", false};
    
    if (!curl_handle) {
        return response;
    }

    CURL* curl = (CURL*)curl_handle;
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set POST data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    
    // Set callback for response body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    
    // Set callback for response headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);
    
    // Follow redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    
    // Set User-Agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "HttpClient/1.0");
    
    // Add custom headers if any
    struct curl_slist* headers_list = nullptr;
    for (const auto& header : custom_headers) {
        std::string header_line = header.first + ": " + header.second;
        headers_list = curl_slist_append(headers_list, header_line.c_str());
    }
    if (headers_list) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_list);
    }
    
    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    
    // Clean up headers list
    if (headers_list) {
        curl_slist_free_all(headers_list);
    }
    
    if (res == CURLE_OK) {
        // Get response code
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        response.status_code = (int)response_code;
        response.success = true;
    } else {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        response.success = false;
    }
    
    return response;
}

void HttpClient::setHeaders(const std::map<std::string, std::string>& headers) {
    custom_headers = headers;
}

void HttpClient::setTimeout(int timeout_seconds) {
    timeout = timeout_seconds;
}