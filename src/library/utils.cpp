

#include <iostream>
using namespace std;

extern "C" {
    #include "mongoose.h"
}
#include "utils.hpp"
#include "macrequesttype.hpp"

// ====================================================================================================================

static const struct mg_str S_GET = MG_MK_STR("GET");
static const struct mg_str S_PUT = MG_MK_STR("PUT");
static const struct mg_str S_POST = MG_MK_STR("POST");
static const struct mg_str S_DELETE = MG_MK_STR("DELETE");

// ====================================================================================================================

int has_prefix(const struct mg_str *uri, const struct mg_str *prefix) {
    return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}
 
int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

 int is_put(struct http_message *hm) {
    return (is_equal(&hm->method, &S_PUT));
}

 int is_post(struct http_message *hm) {
    return (is_equal(&hm->method, &S_POST));
}

 int is_get(struct http_message *hm) {
    return (is_equal(&hm->method, &S_GET));
}

 MacRequestType get_request_type(struct http_message *hm) {
    if (is_get(hm)) return GET;
    else if (is_put(hm)) return PUT;
    else if (is_post(hm)) return POST;
    throw  ("Invalid request type detected!");
}

