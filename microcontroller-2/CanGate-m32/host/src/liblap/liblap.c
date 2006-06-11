#include "liblap.h"
#include "cann.h"

/*****************************************************************************
 * Nix zu tun
 */

static cann_conn_t *conn = NULL;


int can_connect(char *host, int port)
{
	conn = cann_connect(host, port);

	if (conn)
		return 0;

	return -1;
}

void can_close()
{
	cann_close(conn);
}

int can_get_nb(can_message_t *msg)
{
	return cann_get_nb(conn, msg);
}

int can_get(can_message_t *msg)
{
	return cann_get(conn, msg);
}

void can_send(can_message_t *msg)
{
	cann_send(conn, msg);
}

