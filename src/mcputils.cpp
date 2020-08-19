
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <sstream>

extern "C" {
#include <string.h>

#include "mongoose.h"
}

#include "mcprequesttype.hpp"
#include "mcputils.hpp"

// ====================================================================================================================

static std::string S_GET = "GET";
static std::string S_PUT = "PUT";
static std::string S_POST = "POST";
static std::string S_DELETE = "DELETE";
static std::string S_LIST = "LIST";

// ====================================================================================================================

std::string utils_now() {
    auto start = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(start);
    char *ctime = std::ctime(&time);
    std::string stime = std::string((const char *)ctime).substr(0, strlen(ctime) - 1);
    return stime;
}

std::string utils_mg_str_to_string(const struct mg_str mgstr) {
    size_t length = mgstr.len;
    char *buffer = (char *)malloc(length + 1);
    strncpy(buffer, mgstr.p, length);
    buffer[length] = '\0';
    std::string string = std::string(buffer);
    free(buffer);
    buffer = NULL;
    return string;
}

McpRequestType utils_get_request_type(struct http_message *hm) {
    std::string rtype = utils_mg_str_to_string(hm->method);
    if (rtype.compare(S_GET) == 0) {
        return GET;
    } else if (rtype.compare(S_PUT) == 0) {
        return PUT;
    } else if (rtype.compare(S_POST) == 0) {
        return POST;
    } else if (rtype.compare(S_DELETE) == 0) {
        return DELETE;
    } else if (rtype.compare(S_LIST) == 0) {
        return LIST;
    } else {
        std::cerr << "Erro interno em utils_get_request_type!" << std::endl;
        throw("Invalid request type detected!");
    }
}

std::string utils_generate_uuid_v4() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

std::vector<std::string> utils_split_string(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool utils_is_wildcarded_path(std::string route_path) {
    std::vector<std::string> route_vector = utils_split_string(route_path, '/');
    for (std::string const &route_elem : route_vector) {
        if (route_elem[0] == ':')
            return true;
    }
    return false;
}

std::map<std::string, std::string> utils_match_paths(std::string route_path, std::string uri_path) {
    std::vector<std::string> route_vector = utils_split_string(route_path, '/');
    std::vector<std::string> uri_vector = utils_split_string(uri_path, '/');
    std::map<std::string, std::string> result;
    if (route_vector.size() != uri_vector.size()) {
        return result;
    }
    for (std::vector<std::string>::size_type u = 0; u != uri_vector.size(); u++) {
        std::string uri_elem = uri_vector[u];
        std::string route_elem = route_vector[u];
        if (route_elem[0] == ':') {
            std::string var_name = route_elem.substr(1);
            result[var_name] = uri_elem;
        } else if (uri_elem.compare(route_elem) == 0) {
            continue;
        } else {
            return result;
        }
    }
    return result;
}

std::map<std::string, std::vector<std::string> > utils_parse_query(std::string query_string) {
    std::map<std::string, std::vector<std::string> > result;
    size_t found_spc = query_string.find(' ');
    std::string cut_query_string = (found_spc < 0) ? query_string : query_string.substr(0, found_spc);
    std::vector<std::string> query_vector = utils_split_string(cut_query_string, '&');
    for (std::string const &query_elem : query_vector) {
        std::vector<std::string> elem_vector = utils_split_string(query_elem, '=');
        if (elem_vector.size() == 2) {
            std::string var_name = elem_vector[0];
            std::string var_value = elem_vector[1];
            if (result.find(var_name) == result.end()) {
                result[var_name] = std::vector<std::string>();
            }
            result[var_name].push_back(var_value);
        }
    }
    return result;
}
