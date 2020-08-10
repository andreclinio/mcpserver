
#include "macserver.hpp"

class MyServer : public MacServer{

};

class PingRoute : public MacRoute {

  public:

  PingRoute() : MacRoute(GET, "ping") {
  }

  void treat() {
    responseText(200, "Pinged!");
  }
};

int main(void) {
  MacServer* server = MacServer::getInstance();
  server->setHttpPort(8090);
  server->setPathPrefix("/myserver");
  server->addRoute(new PingRoute());
  server->start();
  return 0;
}
