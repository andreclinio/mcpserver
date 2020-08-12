
#ifndef MAC_ROUTEINFO_H_
#define MAC_ROUTEINFO_H_

#include <string>
#include <map>

class MacRoute;

struct MacRouteInfo
{
    MacRoute *route;
    std::map<std::string, std::string> path_map;
    struct mg_connection *nc;
    struct http_message *hm;
    std::string request_id;
};

#endif