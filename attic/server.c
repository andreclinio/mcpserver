// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_post_method = MG_MK_STR("POST");
static const struct mg_str s_delele_method = MG_MK_STR("DELETE");

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
  return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

static int has_prefix(const struct mg_str *uri, const struct mg_str *prefix) {
  return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}

static int is_get(struct http_message *hm) {
   return (is_equal(&hm->method, &s_get_method));
}

static void handle_get_ping(struct mg_connection *nc, struct http_message *hm) {
  char n1[100], n2[100];
  double result;

  /* Get form variables */
  mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
  mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

  /* Send headers */
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nTransfer-Encoding: chunked\r\n\r\n");

  /* Compute the result and send it back as a JSON object */
  result = strtod(n1, NULL) + strtod(n2, NULL);
  mg_printf_http_chunk(nc, "{ \"ping\": true, \"result\": %lf }", result);
  mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  if (ev != MG_EV_HTTP_REQUEST) {
    return;
  }
  struct http_message *hm = (struct http_message *) ev_data;
  static const struct mg_str api_prefix = MG_MK_STR("/myserver/");
  if (!has_prefix(&hm->uri, &api_prefix)) {
     printf("no-prefix: %s!\n", hm->uri.p); fflush(stdout);
     return;
  }
  printf("treating\n=======\n%s\n=======\n", hm->uri.p); fflush(stdout);
  struct mg_str key;
  key.p = hm->uri.p + api_prefix.len;
  key.len = hm->uri.len - api_prefix.len;
  if (is_get(hm) && mg_vcmp(&hm->uri, "/myserver/ping") == 0) {
      printf("ping!\n"); fflush(stdout);
      handle_get_ping(nc, hm);
  }
  else if (is_get(hm) && mg_vcmp(&hm->uri, "/pong") == 0) {
      char buf[100] = {0};
      memcpy(buf, "this is the pong!", sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
      printf("%s\n", buf);
  }
  else {
      mg_printf(nc, "%s", "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n");
        }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
