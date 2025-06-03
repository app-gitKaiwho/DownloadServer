#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string fetchHTML(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Set User-Agent to mimic a browser
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::string extractFirstVideoLink(const std::string& html) {
    std::regex ytRegex("/watch\\?v=[\\w-]+");
    std::smatch match;
    if (std::regex_search(html, match, ytRegex)) {
        return "https://www.youtube.com" + match.str(0);
    }
    return "No link found.";
}

int main(int ac, char** av) {
    std::string query = av[1];
    std::string searchUrl = "https://www.youtube.com/results?search_query=" + std::regex_replace(query, std::regex(" "), "+");

    std::string html = fetchHTML(searchUrl);
    std::string videoLink = extractFirstVideoLink(html);

    std::cout << "First video link: " << videoLink << std::endl;
    return 0;
}
