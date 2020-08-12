

#include <chrono>
#include <iostream>

#include "macserver.hpp"

class MyServer : public MacServer
{
};

class PingRoute : public MacRoute
{
public:
    PingRoute() : MacRoute(GET, "ping")
    {
        setName("ping");
    }
    void treat()
    {
        responseText(200, "Pinged!");
    }
};

class TimeRoute : public MacRoute
{
    char buffer[64];

public:
    TimeRoute() : MacRoute(GET, "time")
    {
        setName("time");
    }

    void treat()
    {
        sprintf(buffer, "{ \"time\" : \"%s\" }\n", getNow().c_str());
        responseJson(200, buffer);
    }
};

class EchoQueryRoute : public MacRoute
{
    char buffer[64];

public:
    EchoQueryRoute() : MacRoute(GET, "echo/query")
    {
        setName("echo:query");
    }

    void treat()
    {
        std::string query_string = getQueryString();
        log("search for query string: " + query_string);
        responseText(200, getQueryString());
    }
};

class EchoPathRoute : public MacRoute
{
    char buffer[64];

public:
    EchoPathRoute() : MacRoute(GET, "echo/path/:var/dummy")
    {
        setName("echo:path");
    }

    void treat()
    {
        auto value = getPathValue(std::string("var"));
        log("Echoing " + value);
        responseText(200, value);
    }
};

int main(void)
{
    MacServer *server = MacServer::getInstance();
    server->setHttpPort(8090);
    server->setPathPrefix("/myserver");
    server->addRoute(new PingRoute());
    server->addRoute(new TimeRoute());
    server->addRoute(new EchoQueryRoute());
    server->addRoute(new EchoPathRoute());
    server->start();
    return 0;
}
