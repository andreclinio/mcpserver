

#ifndef MAC_MACSERVER_H_
#define MAC_MACSERVER_H_

#include "macrequesttype.hpp"
#include "macroute.hpp"


class MacServer {

  private:
    static MacServer* instance;

    MacServer();

    int http_port;
    const char* path_prefix;

  protected:


  public:
    MacRoute* findRoute(const MacRequestType requestType, char const *uri);

    bool handleEvent(struct mg_connection *nc, const MacRequestType requestType, char const *uri);

    int getHttpPort();
    void setHttpPort(int port);
    const char* getPathPrefix();
    void setPathPrefix(const char *path_prefix);
    void start();

    static MacServer* getInstance();

};


#endif