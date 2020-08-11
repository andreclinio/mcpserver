
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
      const char* query_string = http_message->query_string.p;
      return (char*)query_string;
}

char* MacRoute::getPath(){
    return this->path;
}

void MacRoute::handle(struct mg_connection *nc, struct http_message *hm) {
  this->request_id = utils_generate_uuid_v4();
  this->connection = nc;
  this->http_message = hm;
  this->log(">> Treatment start");
  this->treat();
  this->log("<< Treatment end");
  this->connection = NULL;
  this->http_message = NULL;
  this->request_id = "---";
}

char* MacRoute::getHeaderValue(const char* header_key) {
   struct mg_str *header_value = mg_get_http_header(this->http_message, header_key);
   return strdup(header_value->p);
}

void MacRoute::treat() {
    responseTextError(400, (char*)"nothing found!");
}

void MacRoute::responseTextError(int statusCode, char* message) {
   mg_http_send_error(this->connection, statusCode, message);
}

void MacRoute::responseText(int statusCode, char *text) {
   mg_send_head(this->connection, statusCode, strlen(text), "Content-Type: text/plain");
   mg_printf(this->connection, "%.*s", (int)strlen(text), text);
}

void MacRoute::responseJson(int statusCode, char *text) {
   mg_send_head(this->connection, statusCode, strlen(text), "Content-Type: application/json");
   mg_printf(this->connection, "%.*s", (int)strlen(text), text);
}

void MacRoute::log(std::string string) {
    MacServer *server = MacServer::getInstance();
    server->log("[" + getName() + "] [" + this->request_id + "] " + string);
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