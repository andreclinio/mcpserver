
extern "C" {
    #include "mongoose.h"
}

#include "utils.hpp"
#include "macroute.hpp"
#include "macserver.hpp"


// ============================================================================

MacRoute::MacRoute(MacRequestType requestType, char* path) {
    this->requestType = requestType;
    this->path = path;
}

MacRequestType MacRoute::getRequestType() {
    return this->requestType;
}

char* MacRoute::getQueryString() {       
      const char* query_string = route_info.hm->query_string.p;
      return (char*)query_string;
}

char* MacRoute::getPath(){
    return this->path;
}

void MacRoute::handle(MacRouteInfo route_info) {
  this->route_info = route_info;
  this->log(">> Treatment start");
  this->treat();
  this->log("<< Treatment end");
}

char* MacRoute::getHeaderValue(const char* header_key) {
   struct mg_str *header_value = mg_get_http_header(this->route_info.hm, header_key);
   return strdup(header_value->p);
}

std::string MacRoute::getPathValue(std::string var_name) {
    auto map = this->route_info.path_map;
    return map[var_name];
}

void MacRoute::treat() {
    responseTextError(400, (char*)"nothing found!");
}

void MacRoute::responseTextError(int statusCode, char* message) {
   mg_http_send_error(this->route_info.nc, statusCode, message);
}

void MacRoute::responseText(int statusCode, std::string string) {
   const char* text = string.c_str();
   mg_send_head(this->route_info.nc, statusCode, strlen(text), "Content-Type: text/plain");
   mg_printf(this->route_info.nc, "%.*s", (int)strlen(text), text);
}

void MacRoute::responseJson(int statusCode, std::string string) {
   const char* text = string.c_str();
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
      if (name.empty()) return "unamed";
      return this->name;
  }

  void MacRoute::setName(std::string name) {
      this->name = name;
  }