
#ifndef MCP_ROUTE_H_
#define MCP_ROUTE_H_

#include <map>
#include <string>
#include <vector>

#include "mcprequesttype.hpp"
#include "mcprouteinfo.hpp"

class McpRouteX;

class McpRoute {
    McpRouteInfo route_info;

    McpRequestType requestType;
    std::string path;
    std::string name;

   public:
    void responseTextError(int statusCode, std::string string);
    void responseText(int statusCode, std::string string);
    void responseJson(int statusCode, std::string string);

    void log(std::string string);

    std::string getHeaderValue(std::string var_name);
    std::string getPathValue(std::string var_name);
    std::vector<std::string> getQueryValues(std::string var_name);

    std::string getNow();

    McpRequestType getRequestType();
    std::string getPath();
    std::string getName();

    void handle(McpRouteInfo route_info);

    McpRoute(std::string name, McpRequestType request_type, std::string path);
    static McpRouteX *create(std::string name, McpRequestType request_type, std::string path,
                             const std::function<void(McpRoute *)> &function);
    virtual void treat();
};

class McpRouteX : public McpRoute {
    std::function<void(McpRoute *)> function;

   public:
    McpRouteX(std::string name, McpRequestType request_type, std::string path,
              const std::function<void(McpRoute *)> &function)
        : McpRoute(name, request_type, path), function(function) {
        this->function = function;
    }

    void treat() {
        this->function(this);
    }
};

#endif