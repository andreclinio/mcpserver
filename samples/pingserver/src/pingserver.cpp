

#include <chrono>
#include <functional>
#include <iostream>

#include "mcpserver.hpp"

class PingServer : public McpServer {};

int main(void) {
    McpServer* server = McpServer::getInstance();
    server->setHttpPort(8090);
    server->setPathPrefix("pingserver");

    server->addRoute(McpRoute::create("Ping", GET, "ping", [] (McpRoute* route) {
        route->responseText(200, "Pinged at: " + route->getNow());
    }));

    server->start();
    return 0;
}
