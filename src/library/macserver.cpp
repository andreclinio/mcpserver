

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
    std::string path_prefix = server->getPathPrefix();
    struct http_message *hm = (struct http_message *)ev_data;
    std::string uri = utils_mg_str_to_string(hm->uri);
    server->log("URI: " + uri);
    if (uri.rfind(path_prefix, 0) != 0) {
        server->log("Bad URL path prefix: " + uri + " for server starting at: " + path_prefix);
        return;
    }

    const MacRequestType requestType = utils_get_request_type(hm);
    const bool handled = server->handleEvent(nc, hm, requestType, hm->uri.p);
    if (handled) {
        server->log("Request handled!");
    }
    else {
        server->log("Request not handled!");
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

void MacServer::log(std::string text) {
    std::cout << "[" << utils_now() << "] - " << text << std::endl;
}


// ====================================================================================================================

MacServer::MacServer() {
    this->http_port = 80;
    this->path_prefix = "/";
}

void MacServer::setPathPrefix(std::string prefix) {
    this->path_prefix = prefix;
}

void MacServer::setHttpPort(int port) {
    this->http_port = port;
}

int MacServer::getHttpPort() {
    return this->http_port;
}

std::string MacServer::getPathPrefix() {
    return this->path_prefix;
}

MacRoute* MacServer::findRoute(MacRequestType requestType, const char* uri) {
    size_t found_spc = std::string(uri).find(' ');
    std::string path = (found_spc < 0) ? std::string(uri) : std::string(uri).substr(0, found_spc);
    size_t found_qst = std::string(path).find('?');
    std::string prefix = (found_qst < 0) ? std::string(path) : std::string(path).substr(0, found_qst);

    std::list <MacRoute*> ::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it) {
        MacRoute *r = *it;
        std::string rPath = std::string(getPathPrefix()) + "/" + std::string(r->getPath());
        const MacRequestType rReqType = r->getRequestType();
        if (rReqType == requestType && rPath.compare(prefix) == 0) {
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
    if (route == NULL) {
        return false;
    }
    log("route name: " + route->getName());
    try {
        route->handle(nc, hm);
    }
    catch (const char* msg) {
        log(std::string(msg));
        mg_http_send_error(nc, 500, msg);
    }
    return true;
}

void MacServer::start() {
    std::stringstream intro;
    intro << "Starting web server on port " << http_port << " path: " << path_prefix;
    log(intro.str());


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
