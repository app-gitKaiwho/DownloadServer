#include "crow.h"

void routes(crow::SimpleApp app);

std::string extract_file_content(const std::string& body, const std::string& boundary);

std::string get_current_datetime();