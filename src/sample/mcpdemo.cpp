

#include <chrono>
#include <functional>
#include <iostream>

#include "mcpserver.hpp"

class MyServer : public McpServer {};

class TimeRoute : public McpRoute {
    char buffer[64];

   public:
    TimeRoute() : McpRoute("Time", GET, "time") {}

    void treat() {
        sprintf(buffer, "{ \"time\" : \"%s\" }\n", getNow().c_str());
        responseJson(200, buffer);
    }
};

class EchoQueryRoute : public McpRoute {
    char buffer[64];

   public:
    EchoQueryRoute() : McpRoute("Echo Query", GET, "echo/query") {}

    void treat() {
        std::vector<std::string> var_values = getQueryValues("var");
        std::string result = "init ";
        for (const std::string v : var_values) {
            log("search for query string: " + v);
            result = result + v + " ";
        }
        responseText(200, result);
    }
};

class EchoPathRoute : public McpRoute {
    char buffer[64];

   public:
    EchoPathRoute() : McpRoute("Echo Path", GET, "echo/path/:var/dummy") {}

    void treat() {
        auto value = getPathValue(std::string("var"));
        log("Echoing path parameter 'var' " + value);
        responseText(200, value);
    }
};

int main(void) {
    McpServer* server = McpServer::getInstance();
    server->setHttpPort(8090);
    server->setPathPrefix("myserver");
    server->addRoute(new TimeRoute());
    server->addRoute(new EchoQueryRoute());
    server->addRoute(new EchoPathRoute());

    server->addRoute(McpRoute::create("Ping", GET, "ping", [] (McpRoute* route) {
        route->responseText(200, "Pinged at: " + route->getNow());
    }));

    server->start();
    return 0;
}
