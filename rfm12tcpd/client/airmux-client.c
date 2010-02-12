#include "airmux-client.h"
#include <errno.h>

uint16_t strtoui (char *in_c)
{
	uint8_t i; 
	uint16_t out = 0;

	for (i=0;i<=MIN(strlen(in_c), 5) && in_c[i] != '\0';i++)
	{
		out += (10^i) * (in_c[i] - '0');
	}
}

airmux_context_t *airmux_init()
{
	airmux_context_t *mycontext;
	mycontext = malloc(sizeof(airmux_context_t));

	memset (&mycontext->hints, 0, sizeof(struct addrinfo));
	mycontext->hints.ai_family = AF_UNSPEC;
	mycontext->hints.ai_socktype = SOCK_STREAM;
	FD_CLR(&mycontext->wfds);
	FD_CLR(&mycontext->rfds);
	
	return mycontext;
}

int airmux_connect (airmux_context_t *in_ctx, char *in_dhost, char* in_dport)
{
	int i;
	struct addrinfo *tmpinfo;

	if (in_dhost == NULL)
	{
		in_dhost = malloc (strlen(DEFAULT_HOST)+1);
		strncpy (in_dhost, DEFAULT_HOST, strlen(DEFAULT_HOST));
	}

	if (in_dport == NULL)
	{
		in_dport = malloc(6);
		sprintf(in_dport, "%u", DEFAULT_PORT);
	}
	
	getaddrinfo (in_dhost, in_dport, &(in_ctx->hints), &(in_ctx->res));
	tmpinfo = in_ctx->res;
	for (;tmpinfo != NULL;tmpinfo = tmpinfo->ai_next)
	{
		if (in_ctx->sockfd =
			socket(tmpinfo->ai_family, tmpinfo->ai_socktype, tmpinfo->ai_protocol) == -1)
			continue;

		if (connect(in_ctx->sockfd, tmpinfo->ai_addr, tmpinfo->ai_addrlen) == -1)
		{
			close (in_ctx->sockfd);
			continue;
		}
		break;
	}

	if (tmpinfo == NULL)
	{
		return -1 * __LINE__;
	}

	FD_SET (in_ctx->sockfd, &in_ctx->wfds);
	FD_SET (in_ctx->sockfd, &in_ctx->rfds);
	return 1;
}

size_t airmux_rx (airmux_context_t *in_ctx, void* out_buf, size_t in_maxlen)
{
	int rv;
	const struct timeval t = {0,10};

	rv = select (1, &in_ctx->rfds, NULL, NULL, &t);
	if (rv == 0)
		return;


	if (rv < 0 || !FD_ISSET(in_ctx->sockfd, &in_ctx->rfds))
	{
		lastfd = 0;
		FD_CLR (in_ctx->sockfd, &in_ctx->rfds);
		FD_CLR (in_ctx->sockfd, &in_ctx->wfds);
		close (in_ctx->sockfd);
		in_ctx->sockfd = -1;
		return E_SOCKNOTREADY;
	}
	
	rv = recv (in_ctx->sockfd, out_buf, in_maxlen, 0);

	return rv;
}

size_t airmux_tx (airmux_context_t *in_ctx, void* in_buf, size_t in_len)
{
	if (in_ctx->sockfd < 0)
		return E_SOCKNOTREADY;
	
	return send (in_ctx->sockfd, in_buf, in_len, 0);
}
