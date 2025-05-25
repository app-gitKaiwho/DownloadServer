#include "../inc/server.hpp"

int main()
{
    crow::SimpleApp app;
    routes(app);
    app.port(4242).multithreaded().run();
}