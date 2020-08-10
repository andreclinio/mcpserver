

#ifndef MAC_MACSERVER_H_
#define MAC_MACSERVER_H_

#include <list> 
using namespace std; 

#include "macrequesttype.hpp"
#include "macroute.hpp"


class MacServer {

  private:

    list<MacRoute*> routes;

    int http_port;
    const char* path_prefix;

    MacRoute* findRoute(const MacRequestType requestType, char const *uri);

    static MacServer* instance;
    MacServer();

    
  protected:


  public:
    
    bool handleEvent(struct mg_connection *nc, struct http_message *hm, const MacRequestType requestType, char const *uri);


    int getHttpPort();
    void setHttpPort(int port);
    const char* getPathPrefix();
    void setPathPrefix(const char *path_prefix);

    void start();
    void addRoute(MacRoute *route);

    static MacServer* getInstance();

};


#endif