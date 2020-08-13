
#ifndef MCP_ROUTEINFO_H_
#define MCP_ROUTEINFO_H_

#include <map>
#include <string>

class McpRoute;

struct McpRouteInfo {
    McpRoute *route;
    std::map<std::string, std::string> path_map;
    std::map<std::string, std::vector<std::string> > query_map;
    struct mg_connection *nc;
    struct http_message *hm;
    std::string request_id;
};

#endif