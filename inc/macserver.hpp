

#ifndef MCP_SERVER_H_
#define MCP_SERVER_H_

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "macrequesttype.hpp"
#include "macroute.hpp"
#include "macrouteinfo.hpp"

class MacServer {
   private:
    std::list<MacRoute *> routes;

    int http_port;
    std::string path_prefix;

    MacRouteInfo findRoute(const MacRequestType requestType, std::string uri);

    static MacServer *instance;
    MacServer();

   protected:
   public:
    bool handleEvent(struct mg_connection *nc, struct http_message *hm, const MacRequestType requestType, std::string uri);

    int getHttpPort();
    void setHttpPort(int port);

    std::string getPathPrefix();
    void setPathPrefix(std::string path_prefix);

    void log(std::string string);

    void start();
    void addRoute(MacRoute *route);

    static MacServer *getInstance();
};

#endif