#include <stdlib.h>

extern "C" {
    #include "mongoose.h"
}

#include "utils.hpp"
#include "macroute.hpp"


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
}

char* MacRoute::getPath(){
    return this->path;
}

void MacRoute::handle(struct mg_connection *nc, struct http_message *hm) {
  this->connection = nc;
  this->http_message = hm;
  this->treat();
  this->connection = NULL;
  this->http_message = NULL;
}

char* MacRoute::getHeaderValue(const char* header_key) {
   struct mg_str *header_value = mg_get_http_header(this->http_message, header_key);
   return strdup(header_value->p);
}

void MacRoute::treat() {
    responseTextError(400, "nothing found!");
}

void MacRoute::responseTextError(int statusCode, char* message) {
   mg_http_send_error(this->connection, statusCode, message);
}

void MacRoute::responseText(int statusCode, char *text) {
   mg_send_head(this->connection, statusCode, strlen(text), "Content-Type: text/plain");
   mg_printf(this->connection, "%.*s", (int)strlen(text), text);
}