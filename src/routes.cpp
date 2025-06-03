#include "../inc/utils.hpp"
#include "../inc/dl.hpp"


//simple routes, run the app on threads
void routes(crow::SimpleApp app)
{
    //Main page, allow csv to be loaded
    CROW_ROUTE(app, "/csv/")([](){
        crow::mustache::template_t page = crow::mustache::load("./csv.html");
        return page.render();
    });


    CROW_ROUTE(app, "/static/<string>")([](const std::string& filename) {
    std::ifstream file("./static/" + filename, std::ios::binary);
    if (!file.is_open()) {
        return crow::response(404);
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

    crow::response res;
    res.code = 200;
    res.write(contents.str());

if (filename.substr(filename.size() - 4) == ".css") {
        res.add_header("Content-Type", "text/css");
    } else if (filename.substr(filename.size() - 3) == ".js") {
        res.add_header("Content-Type", "application/javascript");
    }
    return res;
});


    //get json on format {artist, url, song} can lack url if named as "undefined"
    CROW_ROUTE(app, "/dl").methods("POST"_method)([](const crow::request& req) {

        crow::json::rvalue body = crow::json::load(req.body);

        if (!body)
            return crow::response(400, "Invalid JSON payload");
        if (ytdl(body))
            return crow::response(200, "downloaded successfully.");
        else
            return crow::response(500, "Download failed");
    });


    //upload the csv to the srv, pretty useless, it was a try to see if I could do it. I can
    CROW_ROUTE(app, "/upload").methods("POST"_method)([](const crow::request& req) {

        std::ofstream out("./csv/uploaded_" + get_current_datetime() + ".csv");
        out << parseheader(req);
        out.close();

        return crow::response(200, "File uploaded successfully.");
    });
    app.port(4242).multithreaded().run();
}