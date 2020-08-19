

extern "C" {
#include <execinfo.h>

#include "mongoose.h"
}

#include "version.hpp"
#include "mcputils.hpp"
#include "mcprequesttype.hpp"
#include "mcproute.hpp"
#include "mcpserver.hpp"

// ============================================================================

void signal_handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    if (ev != MG_EV_HTTP_REQUEST) {
        return;
    }
    McpServer *server = McpServer::getInstance();
    std::string path_prefix = server->getPathPrefix();
    struct http_message *hm = (struct http_message *)ev_data;
    std::string uri = utils_mg_str_to_string(hm->uri);
    server->log("Received request for '" + uri + "' for server path prefix " + path_prefix);
    if (uri.rfind(path_prefix, 0) != 0) {
        std::string err = "Bad URL path prefix: " + uri + " for server starting at: " + path_prefix;
        server->log(err);
        mg_http_send_error(nc, 404, (err+"\r\n").c_str());
        return;
    }

    const McpRequestType request_type = utils_get_request_type(hm);
    server->log("Path inside request: " + utils_mg_str_to_string(hm->method));

    const bool handled = server->handleEvent(nc, hm, request_type, uri);
    if (!handled) {
        std::string err = "Request not handled! No route found in server.";
        server->log(err);
        mg_http_send_error(nc, 404, (err + "\r\n").c_str());
    }
}

// ====================================================================================================================

McpServer *McpServer::instance = 0;

McpServer *McpServer::getInstance() {
    if (instance == 0) {
        instance = new McpServer();
    }
    return instance;
}

void McpServer::log(std::string text) {
    std::cout << "[" << utils_now() << "] - " << text << std::endl;
}

// ====================================================================================================================

McpServer::McpServer() {
    this->http_port = 80;
    this->path_prefix = "";
    log("Creating web server version: " + getVersion());

}

void McpServer::setPathPrefix(std::string prefix) {
    this->path_prefix = "/" + prefix;
}

void McpServer::setHttpPort(int port) {
    this->http_port = port;
}

int McpServer::getHttpPort() {
    return this->http_port;
}

std::string McpServer::getPathPrefix() {
    return this->path_prefix;
}

McpRouteInfo McpServer::findRoute(McpRequestType requestType, std::string uri) {
    size_t found_spc = uri.find(' ');
    std::string path = (found_spc < 0) ? uri : uri.substr(0, found_spc);
    size_t found_qst = path.find('?');
    std::string uri_path = (found_qst < 0) ? path : path.substr(0, found_qst);

    McpRouteInfo result;
    result.path_map = std::map<std::string, std::string>();
    result.route = NULL;

    for (McpRoute *r : routes) {
        const McpRequestType rReqType = r->getRequestType();
        if (rReqType != requestType) {
            continue;
        }
        std::string route_path = getPathPrefix() + "/" + r->getPath();
        if (route_path.compare(uri_path) == 0) {
            result.route = r;
            break;
        }

        if (utils_is_wildcarded_path(route_path)) {
            std::map<std::string, std::string> map = utils_match_paths(route_path, uri_path);
            if (!map.empty()) {
                result.route = r;
                result.path_map = map;
                break;
            }
        }
    }
    return result;
}

void McpServer::addRoute(McpRoute *route) {
    log("New route '" + route->getName() + "' for path '" + route->getPath() + "'");
    this->routes.push_front(route);
}

std::string getVersion() {
    return std::string(MCP_LIBRARY_VERSION);
}

bool McpServer::handleEvent(struct mg_connection *nc, struct http_message *hm, McpRequestType requestType, std::string uri) {
    McpRouteInfo route_info;
    try {
        route_info = findRoute(requestType, uri);
    } catch (const char *msg) {
        log("Internal server error - " + std::string(msg));
        mg_http_send_error(nc, 500, msg);
    }

    McpRoute *route = route_info.route;
    if (route == NULL) {
        return false;
    }
    std::string request_id = utils_generate_uuid_v4();

    log("Route selected: '" + route->getName() + "' - [" + request_id + "]");
    try {
        std::string query_string = utils_mg_str_to_string(hm->query_string);
        route_info.query_map = utils_parse_query(query_string);
        route_info.request_id = request_id;
        route_info.hm = hm;
        route_info.nc = nc;
        route->handle(route_info);
    } catch (const char *msg) {
        log(std::string(msg));
        mg_http_send_error(nc, 500, msg);
    }
    return true;
}

void McpServer::start() {
    signal(SIGSEGV, signal_handler);

    std::stringstream intro;
    intro << "Starting web server on port " << http_port << " path: " << path_prefix;
    log(intro.str());

    static char *s_http_port = (char *)malloc(20);
    sprintf(s_http_port, "%d", http_port);

    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);

    struct mg_connection *nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        throw("Server binding failure!");
    }

    mg_set_protocol_http_websocket(nc);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}
