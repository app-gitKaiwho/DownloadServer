#include "../inc/dl.hpp"

//this is digusting... I NEED to change it later
//Open pipe to capture result print of the external python script scrap 'cause I don't know how to return anything else then boolean with it AND I don't know how to use BS4 in c++...
std::string executeCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

//yt-dlp is to incredible to recode in here
//Run yt-dlp with system, return value to understand if succes or not and perm to everyone on music files
int ytdl(crow::json::rvalue body){

    std::string artist = body["artist"].s();
    std::string url = body["url"].s();
    std::string song = body["song"].s();
    if (url == "undefined") {
        std::string command = "./scrap \"" + artist+ " " + song + "\"";
        url = executeCommand(command);
    }
    std::string command = "yt-dlp -x --audio-format flac --download-archive ./download-archive/downloaded.txt --add-metadata --no-overwrites -o \"./dl/"+artist+"/%(title)s.%(ext)s\" \"" + url + "\""; 
    return(!(std::system(command.c_str()) && std::system("chmod 777 -R ./dl/*.flac"))); //I hate this surely there is a better way to allow acces for everyone to the file, docker config ?
}