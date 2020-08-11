
#ifndef MAC_MACROUTE_H_
#define MAC_MACROUTE_H_

#include <iostream>

#include "macrequesttype.hpp"

class MacRoute {

  struct mg_connection *connection;
  struct http_message *http_message;
  std::string request_id;

  MacRequestType requestType;
  char* path;
  std::string name;

protected:
  void responseTextError(int statusCode, char *text);
  void responseText(int statusCode, char *text);
  void responseJson(int statusCode, char *text);

  void log(std::string string);

  char* getQueryString();
  char* getHeaderValue(const char* paramName);

  std::string getNow();


public:

  MacRequestType getRequestType();
  char* getPath();
  std::string getName();
  void setName(std::string name);

  void handle(struct mg_connection *nc, struct http_message *hm);
  
  MacRoute(MacRequestType requestType, char* path);
  virtual void treat();

};

#endif