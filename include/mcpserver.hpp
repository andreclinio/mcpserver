

#ifndef MCP_SERVER_H_
#define MCP_SERVER_H_

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <functional>

#include "mcprequesttype.hpp"
#include "mcproute.hpp"
#include "mcprouteinfo.hpp"

class McpServer {
   private:
    std::list<McpRoute *> routes;

    int http_port;
    std::string path_prefix;

    McpRouteInfo findRoute(const McpRequestType requestType, std::string uri);

    static McpServer *instance;
    McpServer();

   protected:
   public:
    bool handleEvent(struct mg_connection *nc, struct http_message *hm, const McpRequestType requestType, std::string uri);

    int getHttpPort();
    void setHttpPort(int port);

    std::string getPathPrefix();
    void setPathPrefix(std::string path_prefix);

    void log(std::string string);
    std::string getVersion();

    void start();
    void addRoute(McpRoute *route);

  
    static McpServer *getInstance();
};

#endif