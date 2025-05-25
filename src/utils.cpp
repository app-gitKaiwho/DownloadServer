#include <ctime>
#include <string>

std::string extract_file_content(const std::string& body, const std::string& boundary) {
    size_t file_start = body.find("\r\n\r\n") + 4;
    size_t file_end = body.find(boundary, file_start) - 4;
    return body.substr(file_start, file_end - file_start);
}

std::string get_current_datetime() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);

    return std::string(buffer);
}
