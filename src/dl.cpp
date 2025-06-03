#include "../inc/dl.hpp"
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
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        if (curl_easy_perform(curl))
            readBuffer = "NULL";
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

//yt-dlp is to incredible to recode in here
//Run yt-dlp with system, return value to understand if succes or not and perm to everyone on music files
int ytdl(crow::json::rvalue body){

    std::string artist = body["artist"].s();
    std::string url = body["url"].s();
    std::string song = body["song"].s();
    if (url == "undefined") {
        std::replace(artist.begin(), artist.end(), ' ', '+');
        std::replace(song.begin(), song.end(), ' ', '+');
        std::string searchUrl = "https://www.youtube.com/results?search_query=" + artist + "+" + song;
        std::cout << searchUrl << std::endl;
        url = extractFirstVideoLink(fetchHTML(searchUrl));
    }
    std::string command = "yt-dlp -x --audio-format flac --download-archive ./download-archive/downloaded.txt --add-metadata --no-overwrites -o \"./dl/"+artist+"/%(title)s.%(ext)s\" \"" + url + "\""; 
    return(!(std::system(command.c_str()) && std::system("chmod 777 -R ./dl/*.flac"))); //I hate this surely there is a better way to allow acces for everyone to the file, docker config ?
}