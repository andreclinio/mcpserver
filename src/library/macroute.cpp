
extern "C" {
#include "mongoose.h"
}

#include "macroute.hpp"
#include "macserver.hpp"
#include "utils.hpp"

// ============================================================================

MacRoute::MacRoute(MacRequestType requestType, std::string path) {
    this->requestType = requestType;
    this->path = path;
}

MacRequestType MacRoute::getRequestType() {
    return this->requestType;
}

std::string MacRoute::getPath() {
    return this->path;
}

void MacRoute::handle(MacRouteInfo route_info) {
    this->route_info = route_info;
    this->log(">> Treatment start");
    this->treat();
    this->log("<< Treatment end");
}

std::string MacRoute::getHeaderValue(std::string var_name) {
    struct mg_str *header_value = mg_get_http_header(this->route_info.hm, var_name.c_str());
    return utils_mg_str_to_string(*header_value);
}

std::vector<std::string> MacRoute::getQueryValues(std::string var_name) {
    if (this->route_info.query_map.find(var_name) == this->route_info.query_map.end()) {
        return std::vector<std::string>();
    }
    return this->route_info.query_map[var_name];
}

std::string MacRoute::getPathValue(std::string var_name) {
    std::map<std::string, std::string> map = this->route_info.path_map;
    return map[var_name];
}

void MacRoute::treat() {
    responseTextError(400, "nothing found!");
}

void MacRoute::responseTextError(int statusCode, std::string string) {
    const char *text = string.c_str();
    mg_http_send_error(this->route_info.nc, statusCode, text);
}

void MacRoute::responseText(int statusCode, std::string string) {
    string = string + "\r\n";
    const char *text = string.c_str();
    mg_send_head(this->route_info.nc, statusCode, strlen(text), "Content-Type: text/plain");
    mg_printf(this->route_info.nc, "%.*s", (int)strlen(text), text);
}

void MacRoute::responseJson(int statusCode, std::string string) {
    string = string + "\r\n";
    const char *text = string.c_str();
    mg_send_head(this->route_info.nc, statusCode, strlen(text), "Content-Type: application/json");
    mg_printf(this->route_info.nc, "%.*s", (int)strlen(text), text);
}

void MacRoute::log(std::string string) {
    MacServer *server = MacServer::getInstance();
    server->log("[" + getName() + "] [" + this->route_info.request_id + "] " + string);
}

std::string MacRoute::getNow() {
    return utils_now();
}

std::string MacRoute::getName() {
    if (name.empty()) {
        return "unamed";
    }
    return this->name;
}

void MacRoute::setName(std::string name) {
    this->name = name;
}