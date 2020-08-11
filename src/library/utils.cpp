
#include <iostream>
#include <algorithm>
#include <random>

extern "C" {
    #include "mongoose.h"
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
    char *buffer = (char*)malloc(mgstr.len);
    strlcpy(buffer, mgstr.p, mgstr.len+1);
    std::string string = std::string(buffer);
    free(buffer);
    buffer = NULL;
    return string;
}

MacRequestType utils_get_request_type(struct http_message *hm) {
    std::string rtype = utils_mg_str_to_string(hm->method);
    if (rtype == S_GET) return GET;
    else if (rtype == S_PUT) return PUT;
    else if (rtype == S_POST) return POST;

    throw  ("Invalid request type detected!");
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
