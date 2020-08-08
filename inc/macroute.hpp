
#ifndef MAC_MACROUTE_H_
#define MAC_MACROUTE_H_

#include "macrequesttype.hpp"

class MacRoute {

  struct mg_connection *connection;

  MacRequestType requestType;
  const char* uri;

protected:
  void responseNotFound();
  void responseNotImplemented();

public:

  MacRoute(MacRequestType requestType, const char* uri);
  void handle(struct mg_connection *nc);
  
  virtual void treat();

};

#endif