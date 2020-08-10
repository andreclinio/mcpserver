
#include <iostream>
using namespace std;


extern "C" {
    #include "mongoose.h"
}
#include "utils.hpp"
#include "macrequesttype.hpp"
#include "macroute.hpp"
#include "macserver.hpp"


// ============================================================================

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev != MG_EV_HTTP_REQUEST) {
        return;
    }

    MacServer* server = MacServer::getInstance();
    const char* path_prefix = server->getPathPrefix();

    struct http_message *hm = (struct http_message *)ev_data;
    static const struct mg_str api_prefix = MG_MK_STR(path_prefix);
    cout << ">> URI: " << hm->uri.p << endl;
    cout << ">> Path Prefix: " << path_prefix << endl;

    if (!has_prefix(&hm->uri, &api_prefix)) {
        cout << ">> Bad Path Prefix: " << endl;
        return;
    }

    struct mg_str key;
    key.p = hm->uri.p + api_prefix.len;
    key.len = hm->uri.len - api_prefix.len;
    const MacRequestType requestType = get_request_type(hm);
    cout << ">> Request type: " << requestType << endl;

    const bool handled = server->handleEvent(nc, hm, requestType, hm->uri.p);
    if (handled) {
        cout << ">> Request handled!" << endl;
    }
    else {
        printf(">> Request not handled!");
        mg_http_send_error(nc, 404, "No route found in MacServer!\r\n");
    }
}

// ====================================================================================================================

MacServer* MacServer::instance = 0;

MacServer* MacServer::getInstance() {
    if (instance == 0) {
        instance = new MacServer();
    }
    return instance;
}

// ====================================================================================================================

MacServer::MacServer() {
    this->http_port = 80;
    this->path_prefix = "/";
}

void MacServer::setPathPrefix(const char* prefix) {
    this->path_prefix = prefix;
}

void MacServer::setHttpPort(int port) {
    this->http_port = port;
}

int MacServer::getHttpPort() {
    return this->http_port;
}

const char* MacServer::getPathPrefix() {
    return this->path_prefix;
}

MacRoute* MacServer::findRoute(MacRequestType requestType, const char* uri) {
    size_t found = string(uri).find(' ');
    string path;
    if (found < 0) path = string(uri);
    path = string(uri).substr(0, found);

    // cout << "rt: " << requestType << " find: " << path << endl;
    list <MacRoute*> ::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it) {
        MacRoute *r = *it;
        string rPath = string(getPathPrefix()) + "/" + string(r->getPath());
        const MacRequestType routeRequestType = r->getRequestType();
        // cout << "rt: " << routeRequestType << " rota: " << rPath << endl;
        if (routeRequestType == requestType && rPath.compare(path) == 0) {
            // cout << "atendida!" << endl;
            return r;
        }
    }
    return NULL;
}

void MacServer::addRoute(MacRoute *route) {
    this->routes.push_front(route);
}


bool MacServer::handleEvent(struct mg_connection *nc, struct http_message *hm, MacRequestType requestType, const char* uri) {
    MacRoute* route = findRoute(requestType, hm->uri.p);
    if (route == NULL) return false;
    try {
        route->handle(nc, hm);
    }
    catch (const char* msg) {
        mg_http_send_error(nc, 500, msg);
    }
    return true;
}

void MacServer::start() {
    printf("Starting web server on port %d :: %s\n", http_port, path_prefix);

    static char* s_http_port = (char*)malloc(20);
    sprintf(s_http_port, "%d", http_port);

    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);

    struct mg_connection *nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        throw ("Server binding failure!");
    }

    mg_set_protocol_http_websocket(nc);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}
