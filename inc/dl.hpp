#include <string>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include "crow.h"

std::string executeCommand(const std::string& command);
int ytdl(crow::json::rvalue body);