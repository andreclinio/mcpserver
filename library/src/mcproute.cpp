
extern "C" {
#include "mongoose.h"
}

#include "mcputils.hpp"
#include "mcproute.hpp"
#include "mcpserver.hpp"

McpRouteX *McpRoute::create(std::string name, McpRequestType request_type, std::string path,
                            const std::function<void(McpRoute *)> &function) {
    return new McpRouteX(name, request_type, path, function);
}

// ============================================================================

McpRoute::McpRoute(std::string name, McpRequestType requestType, std::string path) {
    this->name = name;
    this->requestType = requestType;
    this->path = path;
}

McpRequestType McpRoute::getRequestType() {
    return this->requestType;
}

std::string McpRoute::getPath() {
    return this->path;
}

std::string McpRoute::getName() {
    return this->name;
}

void McpRoute::handle(McpRouteInfo route_info) {
    this->route_info = route_info;
    try {
        this->log(">> Treatment start");
        this->treat();
        this->log("<< Treatment end");
    } catch (const char *err) {
        this->log("<< Treatment exception: " + std::string(err));
    }
}

std::string McpRoute::getHeaderValue(std::string var_name) {
    struct mg_str *header_value = mg_get_http_header(this->route_info.hm, var_name.c_str());
    return utils_mg_str_to_string(*header_value);
}

std::vector<std::string> McpRoute::getQueryValues(std::string var_name) {
    if (this->route_info.query_map.find(var_name) == this->route_info.query_map.end()) {
        return std::vector<std::string>();
    }
    return this->route_info.query_map[var_name];
}

std::string McpRoute::getPathValue(std::string var_name) {
    std::map<std::string, std::string> map = this->route_info.path_map;
    return map[var_name];
}

void McpRoute::treat() {
    responseTextError(404, "No treatment implemented for assigned route!");
}

void McpRoute::responseTextError(int status_code, std::string string) {
    const char *text = string.c_str();
    mg_http_send_error(this->route_info.nc, status_code, text);
}

void McpRoute::responseText(int status_code, std::string string) {
    string = string + "\r\n";
    const char *text = string.c_str();
    mg_send_head(this->route_info.nc, status_code, strlen(text), "Content-Type: text/plain");
    mg_printf(this->route_info.nc, "%.*s", (int)strlen(text), text);
}

void McpRoute::responseJson(int status_code, std::string string) {
    string = string + "\r\n";
    const char *text = string.c_str();
    mg_send_head(this->route_info.nc, status_code, strlen(text), "Content-Type: application/json");
    mg_printf(this->route_info.nc, "%.*s", (int)strlen(text), text);
}

void McpRoute::log(std::string string) {
    McpServer *server = McpServer::getInstance();
    server->log("[" + getName() + "] [" + this->route_info.request_id + "] " + string);
}

std::string McpRoute::getNow() {
    return utils_now();
}