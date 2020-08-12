
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <sstream>
#include <iostream>

extern "C" {
    #include "mongoose.h"
    #include <string.h>
}

#include "utils.hpp"
#include "macrequesttype.hpp"

// ====================================================================================================================

static std::string S_GET = "GET";
static std::string  S_PUT = "PUT";
static std::string  S_POST = "POST";
static std::string  S_DELETE = "DELETE";

// ====================================================================================================================

std::string utils_now() {
    auto start = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(start);
    char* ctime = std::ctime(&time);
    std::string stime = std::string((const char*)ctime).substr(0, strlen(ctime)-1);
    return stime;
}

std::string utils_mg_str_to_string(const struct mg_str mgstr) {
    auto length = mgstr.len;
    char *buffer = (char*)malloc(length+1);
    strncpy(buffer, mgstr.p, length);
    buffer[length] = '\0';
    std::string string = std::string(buffer);
    free(buffer);
    buffer = NULL;
    return string;
}

MacRequestType utils_get_request_type(struct http_message *hm) {
    std::string rtype = utils_mg_str_to_string(hm->method);
    if (rtype.compare(S_GET) == 0) return GET;
    else if (rtype.compare(S_PUT) == 0) return PUT;
    else if (rtype.compare(S_POST) == 0) return POST;
    else {
        std::cerr << "Erro interno em utils_get_request_type!" << std::endl;
        throw  ("Invalid request type detected!");
    }
}

std::string utils_generate_uuid_v4() {
    std::random_device      rd;
    std::mt19937          gen(rd());
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

std::vector<std::string> utils_parse_path(std::string path) {
    std::string delimiter = "/";
    std::string part = "";
    std::size_t firstPos = 0;
    std::size_t secondPos = 0;
    std::vector<std::string> parts;
    while (firstPos != std::string::npos)  {
        firstPos = path.find(delimiter, firstPos);
        secondPos = path.find(delimiter, firstPos + 1);
        part = path.substr(firstPos + 1, (secondPos - 1) - firstPos);
        if (!part.empty()) parts.push_back(part);
        firstPos = secondPos;
    }
    return parts;
}


bool utils_is_wildcarded_path(std::string route_path) {
    auto route_vector = utils_parse_path(route_path);
    for(auto const& route_elem: route_vector) {
        if (route_elem[0] == ':') return true;
    }
    return false;
}

std::map<std::string,std::string> utils_match_paths(std::string route_path, std::string uri_path) {
    auto route_vector = utils_parse_path(route_path);
    auto uri_vector = utils_parse_path(uri_path);
    std::map<std::string,std::string> result;
    if (route_vector.size() != uri_vector.size()) return result;
    for(std::vector<std::string>::size_type u = 0; u != uri_vector.size(); u++) {
        auto uri_elem = uri_vector[u];
        auto route_elem = route_vector[u];
        if (route_elem[0] == ':') {
            auto var_name = route_elem.substr(1);
            result[var_name] = uri_elem;
        }
        else if (uri_elem.compare(route_elem) == 0) continue;
        else return result;
    }
    return result;
}