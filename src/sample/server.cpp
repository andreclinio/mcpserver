
#include "macserver.hpp"

class MyServer : public MacServer{

};

int main(void) {
  MacServer* server = MacServer::getInstance();
  server->setHttpPort(8090);
  server->setPathPrefix("/myserver/");
  server->start();
  return 0;
}
