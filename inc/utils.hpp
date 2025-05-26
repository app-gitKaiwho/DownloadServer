#include "crow.h"
#include <cstdlib>

std::string extract_file_content(const std::string& body, const std::string& boundary);
std::string get_current_datetime();
std::string parseheader(const crow::request& req);