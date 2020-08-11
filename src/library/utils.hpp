
#include <iostream>

#include "macserver.hpp"

MacRequestType utils_get_request_type(struct http_message *hm);
std::string utils_mg_str_to_string(const struct mg_str mgstr);
std::string utils_now();
std::string utils_generate_uuid_v4();
