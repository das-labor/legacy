#include "muxd.h"

#pragma once
void mmmux_sigh_cleanup (int in_s);
int mmmux_server_sock_task (mmmux_sctx_t *c);
void mmmux_server_drop_privs (mmmux_sctx_t *in_c);
int mmmux_server_connect (mmmux_sctx_t *in_c);
void mmmux_server_disconnect (mmmux_sctx_t *in_c, int in_fd);
int mmmux_server_handle_data (mmmux_sctx_t *in_c);
int mmmux_server_create_socket (mmmux_sctx_t *in_c);
