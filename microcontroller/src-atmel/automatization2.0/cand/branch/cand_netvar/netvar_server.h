
extern tcp_server_t *netvar_server;

void init_netvar_server();
void netvar_can_handler(rs232can_msg *msg);
