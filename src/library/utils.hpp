

#include "macserver.hpp"

MacRequestType get_request_type(struct http_message *hm);

int has_prefix(const struct mg_str *uri, const struct mg_str *prefix);
int is_equal(const struct mg_str *s1, const struct mg_str *s2);
int has_prefix(const struct mg_str *uri, const struct mg_str *prefix);
int is_put(struct http_message *hm);
int is_post(struct http_message *hm);
int is_get(struct http_message *hm);
