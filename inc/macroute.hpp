
#ifndef MCP_ROUTE_H_
#define MCP_ROUTE_H_

#include <map>
#include <string>
#include <vector>

#include "macrequesttype.hpp"
#include "macrouteinfo.hpp"

class MacRoute {
    MacRouteInfo route_info;

    MacRequestType requestType;
    std::string path;
    std::string name;

   protected:
    void responseTextError(int statusCode, std::string string);
    void responseText(int statusCode, std::string string);
    void responseJson(int statusCode, std::string string);

    void log(std::string string);

    std::string getHeaderValue(std::string var_name);
    std::string getPathValue(std::string var_name);
    std::vector<std::string> getQueryValues(std::string var_name);

    std::string getNow();

   public:
    MacRequestType getRequestType();
    std::string getPath();
    std::string getName();
    void setName(std::string name);

    void handle(MacRouteInfo route_info);

    MacRoute(MacRequestType request_type, std::string path);
    virtual void treat();
};

#endif