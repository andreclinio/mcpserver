
#ifndef MAC_MACROUTE_H_
#define MAC_MACROUTE_H_

#include "macrequesttype.hpp"

class MacRoute {

  struct mg_connection *connection;
  struct http_message *http_message;

  MacRequestType requestType;
  char* path;

protected:
  void responseTextError(int statusCode, char *text);
  void responseText(int statusCode, char *text);
  
  char* getQueryString();
  char* getHeaderValue(const char* paramName);


public:

  MacRequestType getRequestType();
  char* getPath();

  MacRoute(MacRequestType requestType, char* path);
  void handle(struct mg_connection *nc, struct http_message *hm);
  
  virtual void treat();

};

#endif