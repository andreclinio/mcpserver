
#ifndef MAC_ROUTE_H_
#define MAC_ROUTE_H_

#include <string>

#include "macrequesttype.hpp"
#include "macrouteinfo.hpp"

class MacRoute {

  MacRouteInfo route_info;

  MacRequestType requestType;
  char* path;
  std::string name;

protected:
  void responseTextError(int statusCode, char *text);
  void responseText(int statusCode, std::string string);
  void responseJson(int statusCode, std::string string);

  void log(std::string string);

  char* getQueryString();
  char* getHeaderValue(const char* paramName);
  std::string getPathValue(std::string var_name);

  std::string getNow();


public:

  MacRequestType getRequestType();
  char* getPath();
  std::string getName();
  void setName(std::string name);

  void handle(MacRouteInfo route_info);
  
  MacRoute(MacRequestType request_type, char* path);
  virtual void treat();

};

#endif