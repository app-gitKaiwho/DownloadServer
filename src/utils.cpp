#include "../inc/utils.hpp"

//extract body, fin the body after removing the usual header and remove foot and make a sub-string with the actual juice 
std::string extract_file_content(const std::string& body, const std::string& boundary) {
    size_t file_start = body.find("\r\n\r\n") + 4;
    size_t file_end = body.find(boundary, file_start) - 4;
    return body.substr(file_start, file_end - file_start);
}

//just return current time to name files... impressing I know
std::string get_current_datetime() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    return std::string(buffer);
}

//Just raw html boundary from the Content-Type header 
std::string parseheader(const crow::request& req) {
    std::string content_type = req.get_header_value("Content-Type");
    std::string boundary = "--" + content_type.substr(content_type.find("boundary=") + 9);
    return (extract_file_content(req.body, boundary));
}