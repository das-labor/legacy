/* Copyright 2003-2004 Roger Dingledine.
 * Copyright 2004-2005 Roger Dingledine, Nick Mathewson. */
/* See LICENSE for licensing information */
/* $Id: dns.c,v 1.179 2005/12/26 22:42:22 arma Exp $ */
const char dns_c_id[] =
  "$Id: dns.c,v 1.179 2005/12/26 22:42:22 arma Exp $";

/**
 * \file dns.c
 * \brief Implements a farm of 'DNS worker' threads or processes to
 * perform DNS lookups for onion routers and cache the results.
 * [This needs to be done in the background because of the lack of a
 * good, ubiquitous asynchronous DNS implementation.]
 **/

/* See
 * http://elvin.dstc.com/ListArchive/elvin-dev/archive/2001/09/msg00027.html
 * for some approaches to asynchronous dns. We will want to switch once one of
 * them becomes more commonly available.
 */

#include "or.h"
#include "../common/ht.h"

/** Longest hostname we're willing to resolve. */
#define MAX_ADDRESSLEN 256

/** Maximum DNS processes to spawn. */
#define MAX_DNSWORKERS 100
/** Minimum DNS processes to spawn. */
#define MIN_DNSWORKERS 3

/** If more than this many processes are idle, shut down the extras. */
#define MAX_IDLE_DNSWORKERS 10

/** Possible outcomes from hostname lookup: permanent failure,
 * transient (retryable) failure, and success. */
#define DNS_RESOLVE_FAILED_TRANSIENT 1
#define DNS_RESOLVE_FAILED_PERMANENT 2
#define DNS_RESOLVE_SUCCEEDED 3

/** How many dnsworkers we have running right now. */
static int num_dnsworkers=0;
/** How many of the running dnsworkers have an assigned task right now. */
static int num_dnsworkers_busy=0;
/** When did we last rotate the dnsworkers? */
static time_t last_rotation_time=0;

/** Linked list of connections waiting for a DNS answer. */
typedef struct pending_connection_t {
  connection_t *conn;
  struct pending_connection_t *next;
} pending_connection_t;

/** A DNS request: possibly completed, possibly pending; cached_resolve
 * structs are stored at the OR side in a hash table, and as a linked
 * list from oldest to newest.
 */
typedef struct cached_resolve_t {
  HT_ENTRY(cached_resolve_t) node;
  char address[MAX_ADDRESSLEN]; /**< The hostname to be resolved. */
  uint32_t addr; /**< IPv4 addr for <b>address</b>. */
  char state; /**< 0 is pending; 1 means answer is valid; 2 means resolve
               * failed. */
#define CACHE_STATE_PENDING 0
#define CACHE_STATE_VALID 1
#define CACHE_STATE_FAILED 2
  uint32_t expire; /**< Remove items from cache after this time. */
  pending_connection_t *pending_connections;
  struct cached_resolve_t *next;
} cached_resolve_t;

static void purge_expired_resolves(uint32_t now);
static int assign_to_dnsworker(connection_t *exitconn);
static void dns_purge_resolve(cached_resolve_t *resolve);
static void dns_found_answer(char *address, uint32_t addr, char outcome);
static int dnsworker_main(void *data);
static int spawn_dnsworker(void);
static int spawn_enough_dnsworkers(void);
static void send_resolved_cell(connection_t *conn, uint8_t answer_type);

/** Hash table of cached_resolve objects. */
static HT_HEAD(cache_map, cached_resolve_t) cache_root;

/** Function to compare hashed resolves on their addresses; used to
 * implement hash tables. */
static INLINE int
cached_resolves_eq(cached_resolve_t *a, cached_resolve_t *b)
{
  /* make this smarter one day? */
  return !strncmp(a->address, b->address, MAX_ADDRESSLEN);
}

static INLINE unsigned int
cached_resolve_hash(cached_resolve_t *a)
{
  return ht_string_hash(a->address);
}

HT_PROTOTYPE(cache_map, cached_resolve_t, node, cached_resolve_hash,
             cached_resolves_eq);
HT_GENERATE(cache_map, cached_resolve_t, node, cached_resolve_hash,
            cached_resolves_eq, 0.6, malloc, realloc, free);

/** Initialize the DNS cache. */
static void
init_cache_map(void)
{
  HT_INIT(&cache_root);
}

/** Initialize the DNS subsystem; called by the OR process. */
void
dns_init(void)
{
  init_cache_map();
  dnsworkers_rotate();
}

/** Helper: free storage held by an entry in the DNS cache. */
static void
_free_cached_resolve(cached_resolve_t *r)
{
  while (r->pending_connections) {
    pending_connection_t *victim = r->pending_connections;
    r->pending_connections = victim->next;
    tor_free(victim);
  }
  tor_free(r);
}

/** Free all storage held in the DNS cache */
void
dns_free_all(void)
{
  cached_resolve_t **ptr, **next, *item;
  for (ptr = HT_START(cache_map, &cache_root); ptr != NULL; ptr = next) {
    item = *ptr;
    next = HT_NEXT_RMV(cache_map, &cache_root, ptr);
    _free_cached_resolve(item);
  }
  HT_CLEAR(cache_map, &cache_root);
}

/** Linked list of resolved addresses, oldest to newest. */
static cached_resolve_t *oldest_cached_resolve = NULL;
static cached_resolve_t *newest_cached_resolve = NULL;

/** Remove every cached_resolve whose <b>expire</b> time is before <b>now</b>
 * from the cache. */
static void
purge_expired_resolves(uint32_t now)
{
  cached_resolve_t *resolve;
  pending_connection_t *pend;
  connection_t *pendconn;

  /* this is fast because the linked list
   * oldest_cached_resolve is ordered by when they came in.
   */
  while (oldest_cached_resolve && (oldest_cached_resolve->expire < now)) {
    resolve = oldest_cached_resolve;
    debug(LD_EXIT,"Forgetting old cached resolve (address %s, expires %lu)",
          safe_str(resolve->address), (unsigned long)resolve->expire);
    if (resolve->state == CACHE_STATE_PENDING) {
      debug(LD_EXIT,"Bug: Expiring a dns resolve ('%s') that's still pending."
            " Forgot to cull it?", safe_str(resolve->address));
      tor_fragile_assert();
    }
    if (resolve->pending_connections) {
      debug(LD_EXIT, "Closing pending connections on expiring DNS resolve!");
      tor_fragile_assert();
      while (resolve->pending_connections) {
        pend = resolve->pending_connections;
        resolve->pending_connections = pend->next;
        /* Connections should only be pending if they have no socket. */
        tor_assert(pend->conn->s == -1);
        pendconn = pend->conn;
        connection_edge_end(pendconn, END_STREAM_REASON_TIMEOUT,
                            pendconn->cpath_layer);
        circuit_detach_stream(circuit_get_by_edge_conn(pendconn), pendconn);
        connection_free(pendconn);
        tor_free(pend);
      }
    }
    oldest_cached_resolve = resolve->next;
    if (!oldest_cached_resolve) /* if there are no more, */
      newest_cached_resolve = NULL; /* then make sure the list's tail knows
                                     * that too */
    HT_REMOVE(cache_map, &cache_root, resolve);
    tor_free(resolve);
  }
}

/** Send a response to the RESOVLE request of a connection. answer_type must
 *  be one of RESOLVED_TYPE_(IPV4|ERROR|ERROR_TRANSIENT) */
static void
send_resolved_cell(connection_t *conn, uint8_t answer_type)
{
  char buf[RELAY_PAYLOAD_SIZE];
  size_t buflen;

  buf[0] = answer_type;

  switch (answer_type)
    {
    case RESOLVED_TYPE_IPV4:
      buf[1] = 4;
      set_uint32(buf+2, htonl(conn->addr));
      set_uint32(buf+6, htonl(MAX_DNS_ENTRY_AGE)); /*XXXX send a real TTL*/
      buflen = 10;
      break;
    case RESOLVED_TYPE_ERROR_TRANSIENT:
    case RESOLVED_TYPE_ERROR:
      {
        const char *errmsg = "Error resolving hostname";
        int msglen = strlen(errmsg);
        int ttl = (answer_type == RESOLVED_TYPE_ERROR ? MAX_DNS_ENTRY_AGE : 0);
        buf[1] = msglen;
        strlcpy(buf+2, errmsg, sizeof(buf)-2);
        set_uint32(buf+2+msglen, htonl((uint32_t)ttl));
        buflen = 6+msglen;
        break;
      }
    default:
      tor_assert(0);
    }
  connection_edge_send_command(conn, circuit_get_by_edge_conn(conn),
                               RELAY_COMMAND_RESOLVED, buf, buflen,
                               conn->cpath_layer);
}

/** Link <b>r</b> into the hash table of address-to-result mappings, and add it
 * to the linked list of resolves-by-age. */
static void
insert_resolve(cached_resolve_t *r)
{
  /* add us to the linked list of resolves */
  if (!oldest_cached_resolve) {
    oldest_cached_resolve = r;
  } else {
    newest_cached_resolve->next = r;
  }
  newest_cached_resolve = r;

  HT_INSERT(cache_map, &cache_root, r);
}

/** See if we have a cache entry for <b>exitconn</b>-\>address. if so,
 * if resolve valid, put it into <b>exitconn</b>-\>addr and return 1.
 * If resolve failed, unlink exitconn if needed, free it, and return -1.
 *
 * Else, if seen before and pending, add conn to the pending list,
 * and return 0.
 *
 * Else, if not seen before, add conn to pending list, hand to
 * dns farm, and return 0.
 */
int
dns_resolve(connection_t *exitconn)
{
  cached_resolve_t *resolve;
  cached_resolve_t search;
  pending_connection_t *pending_connection;
  struct in_addr in;
  circuit_t *circ;
  uint32_t now = time(NULL);
  assert_connection_ok(exitconn, 0);
  tor_assert(exitconn->s == -1);

  /* first check if exitconn->address is an IP. If so, we already
   * know the answer. */
  if (tor_inet_aton(exitconn->address, &in) != 0) {
    exitconn->addr = ntohl(in.s_addr);
    if (exitconn->purpose == EXIT_PURPOSE_RESOLVE)
      send_resolved_cell(exitconn, RESOLVED_TYPE_IPV4);
    return 1;
  }

  /* then take this opportunity to see if there are any expired
   * resolves in the hash table. */
  purge_expired_resolves(now);

  /* lower-case exitconn->address, so it's in canonical form */
  tor_strlower(exitconn->address);

  /* now check the hash table to see if 'address' is already there. */
  strlcpy(search.address, exitconn->address, sizeof(search.address));
  resolve = HT_FIND(cache_map, &cache_root, &search);
  if (resolve) { /* already there */
    switch (resolve->state) {
      case CACHE_STATE_PENDING:
        /* add us to the pending list */
        pending_connection = tor_malloc_zero(
                                      sizeof(pending_connection_t));
        pending_connection->conn = exitconn;
        pending_connection->next = resolve->pending_connections;
        resolve->pending_connections = pending_connection;
        debug(LD_EXIT,"Connection (fd %d) waiting for pending DNS "
              "resolve of '%s'",
              exitconn->s, safe_str(exitconn->address));
        exitconn->state = EXIT_CONN_STATE_RESOLVING;
        return 0;
      case CACHE_STATE_VALID:
        exitconn->addr = resolve->addr;
        debug(LD_EXIT,"Connection (fd %d) found cached answer for '%s'",
              exitconn->s, safe_str(exitconn->address));
        if (exitconn->purpose == EXIT_PURPOSE_RESOLVE)
          send_resolved_cell(exitconn, RESOLVED_TYPE_IPV4);
        return 1;
      case CACHE_STATE_FAILED:
        debug(LD_EXIT,"Connection (fd %d) found cached error for '%s'",
              exitconn->s, safe_str(exitconn->address));
        if (exitconn->purpose == EXIT_PURPOSE_RESOLVE)
          send_resolved_cell(exitconn, RESOLVED_TYPE_ERROR);
        circ = circuit_get_by_edge_conn(exitconn);
        if (circ)
          circuit_detach_stream(circ, exitconn);
        if (!exitconn->marked_for_close)
          connection_free(exitconn);
        return -1;
    }
    tor_assert(0);
  }
  /* not there, need to add it */
  resolve = tor_malloc_zero(sizeof(cached_resolve_t));
  resolve->state = CACHE_STATE_PENDING;
  resolve->expire = now + MAX_DNS_ENTRY_AGE;
  strlcpy(resolve->address, exitconn->address, sizeof(resolve->address));

  /* add us to the pending list */
  pending_connection = tor_malloc_zero(sizeof(pending_connection_t));
  pending_connection->conn = exitconn;
  resolve->pending_connections = pending_connection;
  exitconn->state = EXIT_CONN_STATE_RESOLVING;

  insert_resolve(resolve);
  return assign_to_dnsworker(exitconn);
}

/** Find or spawn a dns worker process to handle resolving
 * <b>exitconn</b>-\>address; tell that dns worker to begin resolving.
 */
static int
assign_to_dnsworker(connection_t *exitconn)
{
  connection_t *dnsconn;
  unsigned char len;

  tor_assert(exitconn->state == EXIT_CONN_STATE_RESOLVING);
  tor_assert(exitconn->s == -1);

  /* respawn here, to be sure there are enough */
  if (spawn_enough_dnsworkers() < 0) {
    goto err;
  }

  dnsconn = connection_get_by_type_state(CONN_TYPE_DNSWORKER,
                                         DNSWORKER_STATE_IDLE);

  if (!dnsconn) {
    warn(LD_EXIT,"no idle dns workers. Failing.");
    if (exitconn->purpose == EXIT_PURPOSE_RESOLVE)
      send_resolved_cell(exitconn, RESOLVED_TYPE_ERROR_TRANSIENT);
    goto err;
  }

  debug(LD_EXIT,
        "Connection (fd %d) needs to resolve '%s'; assigning "
        "to DNSWorker (fd %d)",
        exitconn->s, safe_str(exitconn->address), dnsconn->s);

  tor_free(dnsconn->address);
  dnsconn->address = tor_strdup(exitconn->address);
  dnsconn->state = DNSWORKER_STATE_BUSY;
  num_dnsworkers_busy++;

  len = strlen(dnsconn->address);
  connection_write_to_buf((char*)&len, 1, dnsconn);
  connection_write_to_buf(dnsconn->address, len, dnsconn);

  return 0;
err:
  dns_cancel_pending_resolve(exitconn->address); /* also sends end and frees */
  return -1;
}

/** Remove <b>conn</b> from the list of connections waiting for conn-\>address.
 */
void
connection_dns_remove(connection_t *conn)
{
  pending_connection_t *pend, *victim;
  cached_resolve_t search;
  cached_resolve_t *resolve;

  tor_assert(conn->type == CONN_TYPE_EXIT);
  tor_assert(conn->state == EXIT_CONN_STATE_RESOLVING);

  strlcpy(search.address, conn->address, sizeof(search.address));

  resolve = HT_FIND(cache_map, &cache_root, &search);
  if (!resolve) {
    /* XXXX RD This *is* a bug, right? -NM */
    notice(LD_BUG, "Address '%s' is not pending. Dropping.",
           safe_str(conn->address));
    return;
  }

  tor_assert(resolve->pending_connections);
  assert_connection_ok(conn,0);

  pend = resolve->pending_connections;

  if (pend->conn == conn) {
    resolve->pending_connections = pend->next;
    tor_free(pend);
    debug(LD_EXIT, "First connection (fd %d) no longer waiting for resolve "
          "of '%s'",
          conn->s, safe_str(conn->address));
    return;
  } else {
    for ( ; pend->next; pend = pend->next) {
      if (pend->next->conn == conn) {
        victim = pend->next;
        pend->next = victim->next;
        tor_free(victim);
        debug(LD_EXIT, "Connection (fd %d) no longer waiting for resolve "
              "of '%s'",
              conn->s, safe_str(conn->address));
        return; /* more are pending */
      }
    }
    tor_assert(0); /* not reachable unless onlyconn not in pending list */
  }
}

/** Log an error and abort if conn is waiting for a DNS resolve.
 */
void
assert_connection_edge_not_dns_pending(connection_t *conn)
{
  pending_connection_t *pend;
  cached_resolve_t **resolve;

  HT_FOREACH(resolve, cache_map, &cache_root) {
    for (pend = (*resolve)->pending_connections;
         pend;
         pend = pend->next) {
      tor_assert(pend->conn != conn);
    }
  }
}

/** Log an error and abort if any connection waiting for a DNS resolve is
 * corrupted. */
void
assert_all_pending_dns_resolves_ok(void)
{
  pending_connection_t *pend;
  cached_resolve_t **resolve;

  HT_FOREACH(resolve, cache_map, &cache_root) {
    for (pend = (*resolve)->pending_connections;
         pend;
         pend = pend->next) {
      assert_connection_ok(pend->conn, 0);
      tor_assert(pend->conn->s == -1);
      tor_assert(!connection_in_array(pend->conn));
    }
  }
}

/** Mark all connections waiting for <b>address</b> for close.  Then cancel
 * the resolve for <b>address</b> itself, and remove any cached results for
 * <b>address</b> from the cache.
 */
void
dns_cancel_pending_resolve(char *address)
{
  pending_connection_t *pend;
  cached_resolve_t search;
  cached_resolve_t *resolve;
  connection_t *pendconn;
  circuit_t *circ;

  strlcpy(search.address, address, sizeof(search.address));

  resolve = HT_FIND(cache_map, &cache_root, &search);
  if (!resolve) {
    /* XXXX RD This *is* a bug, right? -NM */
    notice(LD_BUG,"Address '%s' is not pending. Dropping.", safe_str(address));
    return;
  }

  if (!resolve->pending_connections) {
    /* XXX this should never trigger, but sometimes it does */
    warn(LD_BUG,"Bug: Address '%s' is pending but has no pending connections!",
         safe_str(address));
    tor_fragile_assert();
    return;
  }
  tor_assert(resolve->pending_connections);

  /* mark all pending connections to fail */
  debug(LD_EXIT, "Failing all connections waiting on DNS resolve of '%s'",
        safe_str(address));
  while (resolve->pending_connections) {
    pend = resolve->pending_connections;
    pend->conn->state = EXIT_CONN_STATE_RESOLVEFAILED;
    pendconn = pend->conn;
    tor_assert(pendconn->s == -1);
    if (!pendconn->marked_for_close) {
      connection_edge_end(pendconn, END_STREAM_REASON_RESOURCELIMIT,
                          pendconn->cpath_layer);
    }
    circ = circuit_get_by_edge_conn(pendconn);
    if (circ)
      circuit_detach_stream(circ, pendconn);
    connection_free(pendconn);
    resolve->pending_connections = pend->next;
    tor_free(pend);
  }

  dns_purge_resolve(resolve);
}

/** Remove <b>resolve</b> from the cache.
 */
static void
dns_purge_resolve(cached_resolve_t *resolve)
{
  cached_resolve_t *tmp;

  /* remove resolve from the linked list */
  if (resolve == oldest_cached_resolve) {
    oldest_cached_resolve = resolve->next;
    if (oldest_cached_resolve == NULL)
      newest_cached_resolve = NULL;
  } else {
    /* FFFF make it a doubly linked list if this becomes too slow */
    for (tmp=oldest_cached_resolve; tmp && tmp->next != resolve; tmp=tmp->next)
      ;
    tor_assert(tmp); /* it's got to be in the list, or we screwed up somewhere
                      * else */
    tmp->next = resolve->next; /* unlink it */

    if (newest_cached_resolve == resolve)
      newest_cached_resolve = tmp;
  }

  /* remove resolve from the map */
  HT_REMOVE(cache_map, &cache_root, resolve);

  tor_free(resolve);
}

/** Called on the OR side when a DNS worker tells us the outcome of a DNS
 * resolve: tell all pending connections about the result of the lookup, and
 * cache the value.  (<b>address</b> is a NUL-terminated string containing the
 * address to look up; <b>addr</b> is an IPv4 address in host order;
 * <b>outcome</b> is one of
 * DNS_RESOLVE_{FAILED_TRANSIENT|FAILED_PERMANENT|SUCCEEDED}.
 */
static void
dns_found_answer(char *address, uint32_t addr, char outcome)
{
  pending_connection_t *pend;
  cached_resolve_t search;
  cached_resolve_t *resolve;
  connection_t *pendconn;
  circuit_t *circ;

  strlcpy(search.address, address, sizeof(search.address));

  resolve = HT_FIND(cache_map, &cache_root, &search);
  if (!resolve) {
    info(LD_EXIT,"Resolved unasked address '%s'; caching anyway.",
         safe_str(address));
    resolve = tor_malloc_zero(sizeof(cached_resolve_t));
    resolve->state = (outcome == DNS_RESOLVE_SUCCEEDED) ?
      CACHE_STATE_VALID : CACHE_STATE_FAILED;
    resolve->addr = addr;
    resolve->expire = time(NULL) + MAX_DNS_ENTRY_AGE;
    insert_resolve(resolve);
    return;
  }

  if (resolve->state != CACHE_STATE_PENDING) {
    /* XXXX Maybe update addr? or check addr for consistency? Or let
     * VALID replace FAILED? */
    notice(LD_EXIT, "Resolved '%s' which was already resolved; ignoring",
           safe_str(address));
    tor_assert(resolve->pending_connections == NULL);
    return;
  }
  /* Removed this assertion: in fact, we'll sometimes get a double answer
   * to the same question.  This can happen when we ask one worker to resolve
   * X.Y.Z., then we cancel the request, and then we ask another worker to
   * resolve X.Y.Z. */
  /* tor_assert(resolve->state == CACHE_STATE_PENDING); */

  resolve->addr = addr;
  if (outcome == DNS_RESOLVE_SUCCEEDED)
    resolve->state = CACHE_STATE_VALID;
  else
    resolve->state = CACHE_STATE_FAILED;

  while (resolve->pending_connections) {
    pend = resolve->pending_connections;
    assert_connection_ok(pend->conn,time(NULL));
    pend->conn->addr = resolve->addr;
    pendconn = pend->conn; /* don't pass complex things to the
                              connection_mark_for_close macro */

    if (resolve->state == CACHE_STATE_FAILED) {
      /* prevent double-remove. */
      pendconn->state = EXIT_CONN_STATE_RESOLVEFAILED;
      if (pendconn->purpose == EXIT_PURPOSE_CONNECT) {
        connection_edge_end(pendconn, END_STREAM_REASON_RESOLVEFAILED,
                            pendconn->cpath_layer);
        /* This detach must happen after we send the end cell. */
        circuit_detach_stream(circuit_get_by_edge_conn(pendconn), pendconn);
      } else {
        send_resolved_cell(pendconn, RESOLVED_TYPE_ERROR);
        /* This detach must happen after we send the resolved cell. */
        circuit_detach_stream(circuit_get_by_edge_conn(pendconn), pendconn);
      }
      connection_free(pendconn);
    } else {
      if (pendconn->purpose == EXIT_PURPOSE_CONNECT) {
        /* prevent double-remove. */
        pend->conn->state = EXIT_CONN_STATE_CONNECTING;

        circ = circuit_get_by_edge_conn(pend->conn);
        tor_assert(circ);
        /* unlink pend->conn from resolving_streams, */
        circuit_detach_stream(circ, pend->conn);
        /* and link it to n_streams */
        pend->conn->next_stream = circ->n_streams;
        pend->conn->on_circuit = circ;
        circ->n_streams = pend->conn;

        connection_exit_connect(pend->conn);
      } else {
        /* prevent double-remove.  This isn't really an accurate state,
         * but it does the right thing. */
        pendconn->state = EXIT_CONN_STATE_RESOLVEFAILED;
        send_resolved_cell(pendconn, RESOLVED_TYPE_IPV4);
        circ = circuit_get_by_edge_conn(pendconn);
        tor_assert(circ);
        circuit_detach_stream(circ, pendconn);
        connection_free(pendconn);
      }
    }
    resolve->pending_connections = pend->next;
    tor_free(pend);
  }

  if (outcome == DNS_RESOLVE_FAILED_TRANSIENT) { /* remove from cache */
    dns_purge_resolve(resolve);
  }
}

/******************************************************************/

/*
 * Connection between OR and dnsworker
 */

/** Write handler: called when we've pushed a request to a dnsworker. */
int
connection_dns_finished_flushing(connection_t *conn)
{
  tor_assert(conn);
  tor_assert(conn->type == CONN_TYPE_DNSWORKER);
  connection_stop_writing(conn);
  return 0;
}

int
connection_dns_reached_eof(connection_t *conn)
{
  warn(LD_EXIT,"Read eof. Worker died unexpectedly.");
  if (conn->state == DNSWORKER_STATE_BUSY) {
    /* don't cancel the resolve here -- it would be cancelled in
     * connection_about_to_close_connection(), since conn is still
     * in state BUSY
     */
    num_dnsworkers_busy--;
  }
  num_dnsworkers--;
  connection_mark_for_close(conn);
  return 0;
}

/** Read handler: called when we get data from a dnsworker. See
 * if we have a complete answer.  If so, call dns_found_answer on the
 * result.  If not, wait.  Returns 0. */
int
connection_dns_process_inbuf(connection_t *conn)
{
  char success;
  uint32_t addr;

  tor_assert(conn);
  tor_assert(conn->type == CONN_TYPE_DNSWORKER);

  if (conn->state != DNSWORKER_STATE_BUSY && buf_datalen(conn->inbuf)) {
    warn(LD_BUG,"Bug: read data (%d bytes) from an idle dns worker (fd %d, "
         "address '%s'). Please report.",
         (int)buf_datalen(conn->inbuf), conn->s, safe_str(conn->address));
    tor_fragile_assert();

    /* Pull it off the buffer anyway, or it will just stay there.
     * Keep pulling things off because sometimes we get several
     * answers at once (!). */
    while (buf_datalen(conn->inbuf)) {
      connection_fetch_from_buf(&success,1,conn);
      connection_fetch_from_buf((char *)&addr,sizeof(uint32_t),conn);
      warn(LD_EXIT,"Discarding idle dns answer (success %d, addr %d.)",
           success, addr); // XXX safe_str
    }
    return 0;
  }
  if (buf_datalen(conn->inbuf) < 5) /* entire answer available? */
    return 0; /* not yet */
  tor_assert(conn->state == DNSWORKER_STATE_BUSY);
  tor_assert(buf_datalen(conn->inbuf) == 5);

  connection_fetch_from_buf(&success,1,conn);
  connection_fetch_from_buf((char *)&addr,sizeof(uint32_t),conn);

  debug(LD_EXIT, "DNSWorker (fd %d) returned answer for '%s'",
        conn->s, safe_str(conn->address));

  tor_assert(success >= DNS_RESOLVE_FAILED_TRANSIENT);
  tor_assert(success <= DNS_RESOLVE_SUCCEEDED);
  dns_found_answer(conn->address, ntohl(addr), success);

  tor_free(conn->address);
  conn->address = tor_strdup("<idle>");
  conn->state = DNSWORKER_STATE_IDLE;
  num_dnsworkers_busy--;
  if (conn->timestamp_created < last_rotation_time) {
    connection_mark_for_close(conn);
    num_dnsworkers--;
    spawn_enough_dnsworkers();
  }
  return 0;
}

/** Close and re-open all idle dnsworkers; schedule busy ones to be closed
 * and re-opened once they're no longer busy.
 **/
void
dnsworkers_rotate(void)
{
  connection_t *dnsconn;
  while ((dnsconn = connection_get_by_type_state(CONN_TYPE_DNSWORKER,
                                                 DNSWORKER_STATE_IDLE))) {
    connection_mark_for_close(dnsconn);
    num_dnsworkers--;
  }
  last_rotation_time = time(NULL);
  if (server_mode(get_options()))
    spawn_enough_dnsworkers();
}

/** Implementation for DNS workers; this code runs in a separate
 * execution context.  It takes as its argument an fdarray as returned
 * by socketpair(), and communicates via fdarray[1].  The protocol is
 * as follows:
 *    - The OR says:
 *         - ADDRESSLEN [1 byte]
 *         - ADDRESS    [ADDRESSLEN bytes]
 *    - The DNS worker does the lookup, and replies:
 *         - OUTCOME    [1 byte]
 *         - IP         [4 bytes]
 *
 * OUTCOME is one of DNS_RESOLVE_{FAILED_TRANSIENT|FAILED_PERMANENT|SUCCEEDED}.
 * IP is in host order.
 *
 * The dnsworker runs indefinitely, until its connection is closed or an error
 * occurs.
 */
static int
dnsworker_main(void *data)
{
  char address[MAX_ADDRESSLEN];
  unsigned char address_len;
  char answer[5];
  uint32_t ip;
  int *fdarray = data;
  int fd;
  int result;

  /* log_fn(LOG_NOTICE,"After spawn: fdarray @%d has %d:%d", (int)fdarray,
   * fdarray[0],fdarray[1]); */

  fd = fdarray[1]; /* this side is ours */
#ifndef TOR_IS_MULTITHREADED
  tor_close_socket(fdarray[0]); /* this is the side of the socketpair the
                                 * parent uses */
  tor_free_all(1); /* so the child doesn't hold the parent's fd's open */
  handle_signals(0); /* ignore interrupts from the keyboard, etc */
#endif
  tor_free(data);

  for (;;) {
    int r;

    if ((r = recv(fd, &address_len, 1, 0)) != 1) {
      if (r == 0) {
        info(LD_EXIT,"DNS worker exiting because Tor process closed "
             "connection (either pruned idle dnsworker or died).");
      } else {
        info(LD_EXIT,"DNS worker exiting because of error on connection "
             "to Tor process.");
        info(LD_EXIT,"(Error on %d was %s)", fd,
             tor_socket_strerror(tor_socket_errno(fd)));
      }
      tor_close_socket(fd);
      crypto_thread_cleanup();
      spawn_exit();
    }

    if (address_len && read_all(fd, address, address_len, 1) != address_len) {
      err(LD_BUG,"read hostname failed. Child exiting.");
      tor_close_socket(fd);
      crypto_thread_cleanup();
      spawn_exit();
    }
    address[address_len] = 0; /* null terminate it */

    result = tor_lookup_hostname(address, &ip);
    /* Make 0.0.0.0 an error, so that we can use "0" to mean "no addr") */
    if (!ip)
      result = -1;
    switch (result) {
      case 1:
        /* XXX result can never be 1, because we set it to -1 above on error */
        info(LD_NET,"Could not resolve dest addr %s (transient).",
             safe_str(address));
        answer[0] = DNS_RESOLVE_FAILED_TRANSIENT;
        break;
      case -1:
        info(LD_NET,"Could not resolve dest addr %s (permanent).",
             safe_str(address));
        answer[0] = DNS_RESOLVE_FAILED_PERMANENT;
        break;
      case 0:
        info(LD_NET,"Resolved address '%s'.",safe_str(address));
        answer[0] = DNS_RESOLVE_SUCCEEDED;
        break;
    }
    set_uint32(answer+1, ip);
    if (write_all(fd, answer, 5, 1) != 5) {
      err(LD_NET,"writing answer failed. Child exiting.");
      tor_close_socket(fd);
      crypto_thread_cleanup();
      spawn_exit();
    }
  }
  return 0; /* windows wants this function to return an int */
}

/** Launch a new DNS worker; return 0 on success, -1 on failure.
 */
static int
spawn_dnsworker(void)
{
  int *fdarray;
  int fd;
  connection_t *conn;
  int err;

  fdarray = tor_malloc(sizeof(int)*2);
  if ((err = tor_socketpair(AF_UNIX, SOCK_STREAM, 0, fdarray)) < 0) {
    warn(LD_NET, "Couldn't construct socketpair: %s",
         tor_socket_strerror(-err));
    tor_free(fdarray);
    return -1;
  }

  /* log_fn(LOG_NOTICE,"Before spawn: fdarray @%d has %d:%d",
            (int)fdarray, fdarray[0],fdarray[1]); */

  fd = fdarray[0]; /* We copy this out here, since dnsworker_main may free
                    * fdarray */
  spawn_func(dnsworker_main, (void*)fdarray);
  debug(LD_EXIT,"just spawned a dns worker.");
#ifndef TOR_IS_MULTITHREADED
  tor_close_socket(fdarray[1]); /* don't need the worker's side of the pipe */
  tor_free(fdarray);
#endif

  conn = connection_new(CONN_TYPE_DNSWORKER);

  set_socket_nonblocking(fd);

  /* set up conn so it's got all the data we need to remember */
  conn->s = fd;
  conn->address = tor_strdup("<unused>");

  if (connection_add(conn) < 0) { /* no space, forget it */
    warn(LD_NET,"connection_add failed. Giving up.");
    connection_free(conn); /* this closes fd */
    return -1;
  }

  conn->state = DNSWORKER_STATE_IDLE;
  connection_start_reading(conn);

  return 0; /* success */
}

/** If we have too many or too few DNS workers, spawn or kill some.
 * Return 0 if we are happy, return -1 if we tried to spawn more but
 * we couldn't.
 */
static int
spawn_enough_dnsworkers(void)
{
  int num_dnsworkers_needed; /* aim to have 1 more than needed,
                           * but no less than min and no more than max */
  connection_t *dnsconn;

  /* XXX This may not be the best strategy. Maybe we should queue pending
   *     requests until the old ones finish or time out: otherwise, if the
   *     connection requests come fast enough, we never get any DNS done. -NM
   *
   * XXX But if we queue them, then the adversary can pile even more
   *     queries onto us, blocking legitimate requests for even longer.  Maybe
   *     we should compromise and only kill if it's been at it for more than,
   *     e.g., 2 seconds. -RD
   */
  if (num_dnsworkers_busy == MAX_DNSWORKERS) {
    /* We always want at least one worker idle.
     * So find the oldest busy worker and kill it.
     */
    dnsconn = connection_get_by_type_state_lastwritten(CONN_TYPE_DNSWORKER,
                                                       DNSWORKER_STATE_BUSY);
    tor_assert(dnsconn);

    warn(LD_EXIT, "%d DNS workers are spawned; all are busy. Killing one.",
         MAX_DNSWORKERS);

    connection_mark_for_close(dnsconn);
    num_dnsworkers_busy--;
    num_dnsworkers--;
  }

  if (num_dnsworkers_busy >= MIN_DNSWORKERS)
    num_dnsworkers_needed = num_dnsworkers_busy+1;
  else
    num_dnsworkers_needed = MIN_DNSWORKERS;

  while (num_dnsworkers < num_dnsworkers_needed) {
    if (spawn_dnsworker() < 0) {
      warn(LD_EXIT,"Spawn failed. Will try again later.");
      return -1;
    }
    num_dnsworkers++;
  }

  while (num_dnsworkers > num_dnsworkers_busy+MAX_IDLE_DNSWORKERS) {
    /* too many idle? */
    /* cull excess workers */
    info(LD_EXIT,"%d of %d dnsworkers are idle. Killing one.",
         num_dnsworkers-num_dnsworkers_busy, num_dnsworkers);
    dnsconn = connection_get_by_type_state(CONN_TYPE_DNSWORKER,
                                           DNSWORKER_STATE_IDLE);
    tor_assert(dnsconn);
    connection_mark_for_close(dnsconn);
    num_dnsworkers--;
  }

  return 0;
}

