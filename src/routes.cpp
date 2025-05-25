#include "../inc/server.hpp"
#include <cstdlib>
#include <thread>
#include <chrono>

void routes(crow::SimpleApp app)
{
    CROW_ROUTE(app, "/csv/")([](){
        auto page = crow::mustache::load("./csv.html");
        return page.render();
    });

CROW_ROUTE(app, "/dl").methods("POST"_method)([](const crow::request& req) {
    auto body = crow::json::load(req.body);
    
    if (!body) {
        return crow::response(400, "Invalid JSON payload");
    }

    std::string artist = body["artist"].s();
    std::string url = body["url"].s();

    std::string command = "yt-dlp -x --download-archive ./download-archive/downloaded.txt --add-metadata --no-overwrites -o \"./dl/"+artist+"/%(title)s.%(ext)s\" \"" + url + "\"";

    int result = std::system(command.c_str());

    if (result == 0)
        return crow::response(200, "downloaded successfully.");
    else
        return crow::response(500, "Download failed");
});


    CROW_ROUTE(app, "/upload").methods("POST"_method)([](const crow::request& req) {

        std::string content_type = req.get_header_value("Content-Type");
        std::string boundary = "--" + content_type.substr(content_type.find("boundary=") + 9);

        std::string file_content = extract_file_content(req.body, boundary);

        std::cout << get_current_datetime() << std::endl;
        std::ofstream out("./csv/uploaded_" + get_current_datetime() + ".csv");
        out << file_content;
        out.close();

        return crow::response(200, "File uploaded successfully.");
    });

    app.port(4242).multithreaded().run();
}