#include <stdlib.h>

extern "C" {
    #include "mongoose.h"
}

#include "utils.hpp"
#include "macroute.hpp"


// ====================================================================================================================


MacRoute::MacRoute(MacRequestType requestType, const char* uri) {
    this->requestType = requestType;
    this->uri = uri;
}

void MacRoute::handle(struct mg_connection *nc) {
  this->connection = nc;
  this->treat();
  this->connection = NULL;
}

void MacRoute::treat() {
    responseNotImplemented();
}

void MacRoute::responseNotFound() {
   mg_printf(this->connection, "%s", "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n");
   mg_send_http_chunk(this->connection, "", 0); 
}

void MacRoute::responseNotImplemented() {
   mg_printf(this->connection, "%s", "HTTP/1.0 501 Not Implemented\r\nContent-Length: 0\r\n\r\n");
   mg_send_http_chunk(this->connection, "", 0); 
}