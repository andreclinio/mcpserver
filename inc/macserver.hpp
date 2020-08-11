

#ifndef MAC_MACSERVER_H_
#define MAC_MACSERVER_H_

#include <string>
#include <list> 
#include <iostream>
#include <sstream>

#include "macrequesttype.hpp"
#include "macroute.hpp"


class MacServer {

  private:

    std::list<MacRoute*> routes;

    int http_port;
    std::string path_prefix;

    MacRoute* findRoute(const MacRequestType requestType, char const *uri);

    static MacServer* instance;
    MacServer();

    
  protected:


  public:
    
    bool handleEvent(struct mg_connection *nc, struct http_message *hm, const MacRequestType requestType, char const *uri);


    int getHttpPort();
    void setHttpPort(int port);

    std::string getPathPrefix();
    void setPathPrefix(std::string path_prefix);

    void log(std::string string);

    void start();
    void addRoute(MacRoute *route);

    static MacServer* getInstance();

};


#endif