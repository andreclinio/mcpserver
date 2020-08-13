

#ifndef MCP_UTILS_H_
#define MCP_UTILS_H_

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "mcprequesttype.hpp"

McpRequestType utils_get_request_type(struct http_message *hm);
std::string utils_mg_str_to_string(const struct mg_str mgstr);
std::string utils_now();
std::string utils_generate_uuid_v4();

bool utils_is_wildcarded_path(std::string route_path);
std::map<std::string, std::string> utils_match_paths(std::string route_path, std::string uri_path);
std::map<std::string, std::vector<std::string> > utils_parse_query(std::string query_string);

#endif
