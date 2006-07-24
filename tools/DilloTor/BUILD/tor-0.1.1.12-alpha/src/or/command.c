/* Copyright 2001 Matej Pfajfar.
 * Copyright 2001-2004 Roger Dingledine.
 * Copyright 2004-2005 Roger Dingledine, Nick Mathewson. */
/* See LICENSE for licensing information */
/* $Id: command.c,v 1.107 2006/01/05 21:23:03 nickm Exp $ */
const char command_c_id[] =
  "$Id: command.c,v 1.107 2006/01/05 21:23:03 nickm Exp $";

/**
 * \file command.c
 * \brief Functions for processing incoming cells.
 **/

/* In-points to command.c:
 *
 * - command_process_cell(), called from
 *   connection_or_process_cells_from_inbuf() in connection_or.c
 */

#include "or.h"

/** Keep statistics about how many of each type of cell we've received. */
uint64_t stats_n_padding_cells_processed = 0;
uint64_t stats_n_create_cells_processed = 0;
uint64_t stats_n_created_cells_processed = 0;
uint64_t stats_n_relay_cells_processed = 0;
uint64_t stats_n_destroy_cells_processed = 0;

/* These are the main four functions for processing cells */
static void command_process_create_cell(cell_t *cell, connection_t *conn);
static void command_process_created_cell(cell_t *cell, connection_t *conn);
static void command_process_relay_cell(cell_t *cell, connection_t *conn);
static void command_process_destroy_cell(cell_t *cell, connection_t *conn);

#ifdef KEEP_TIMING_STATS
/** This is a wrapper function around the actual function that processes the
 * <b>cell</b> that just arrived on <b>conn</b>. Increment <b>*time</b>
 * by the number of microseconds used by the call to <b>*func(cell, conn)</b>.
 */
static void
command_time_process_cell(cell_t *cell, connection_t *conn, int *time,
                               void (*func)(cell_t *, connection_t *))
{
  struct timeval start, end;
  long time_passed;

  tor_gettimeofday(&start);

  (*func)(cell, conn);

  tor_gettimeofday(&end);
  time_passed = tv_udiff(&start, &end) ;

  if (time_passed > 10000) { /* more than 10ms */
    debug(LD_OR,"That call just took %ld ms.",time_passed/1000);
  }
  if (time_passed < 0) {
    info(LD_GENERAL,"That call took us back in time!");
    time_passed = 0;
  }
  *time += time_passed;
}
#endif

/** Process a <b>cell</b> that was just received on <b>conn</b>. Keep internal
 * statistics about how many of each cell we've processed so far
 * this second, and the total number of microseconds it took to
 * process each type of cell.
 */
void
command_process_cell(cell_t *cell, connection_t *conn)
{
#ifdef KEEP_TIMING_STATS
  /* how many of each cell have we seen so far this second? needs better
   * name. */
  static int num_create=0, num_created=0, num_relay=0, num_destroy=0;
  /* how long has it taken to process each type of cell? */
  static int create_time=0, created_time=0, relay_time=0, destroy_time=0;
  static time_t current_second = 0; /* from previous calls to time */

  time_t now = time(NULL);

  if (now > current_second) { /* the second has rolled over */
    /* print stats */
    info(LD_OR,"At end of second: %d creates (%d ms), %d createds (%d ms), "
         "%d relays (%d ms), %d destroys (%d ms)",
         num_create, create_time/1000,
         num_created, created_time/1000,
         num_relay, relay_time/1000,
         num_destroy, destroy_time/1000);

    /* zero out stats */
    num_create = num_created = num_relay = num_destroy = 0;
    create_time = created_time = relay_time = destroy_time = 0;

    /* remember which second it is, for next time */
    current_second = now;
  }
#endif

  switch (cell->command) {
    case CELL_PADDING:
      ++stats_n_padding_cells_processed;
      /* do nothing */
      break;
    case CELL_CREATE:
    case CELL_CREATE_FAST:
      ++stats_n_create_cells_processed;
#ifdef KEEP_TIMING_STATS
      ++num_create;
      command_time_process_cell(cell, conn, &create_time,
                                command_process_create_cell);
#else
      command_process_create_cell(cell, conn);
#endif
      break;
    case CELL_CREATED:
    case CELL_CREATED_FAST:
      ++stats_n_created_cells_processed;
#ifdef KEEP_TIMING_STATS
      ++num_created;
      command_time_process_cell(cell, conn, &created_time,
                                command_process_created_cell);
#else
      command_process_created_cell(cell, conn);
#endif
      break;
    case CELL_RELAY:
      ++stats_n_relay_cells_processed;
#ifdef KEEP_TIMING_STATS
      ++num_relay;
      command_time_process_cell(cell, conn, &relay_time,
                                command_process_relay_cell);
#else
      command_process_relay_cell(cell, conn);
#endif
      break;
    case CELL_DESTROY:
      ++stats_n_destroy_cells_processed;
#ifdef KEEP_TIMING_STATS
      ++num_destroy;
      command_time_process_cell(cell, conn, &destroy_time,
                                command_process_destroy_cell);
#else
      command_process_destroy_cell(cell, conn);
#endif
      break;
    default:
      log_fn(LOG_PROTOCOL_WARN, LD_PROTOCOL,
             "Cell of unknown type (%d) received. Dropping.", cell->command);
      break;
  }
}

/** Process a 'create' <b>cell</b> that just arrived from <b>conn</b>. Make a
 * new circuit with the p_circ_id specified in cell. Put the circuit in state
 * onionskin_pending, and pass the onionskin to the cpuworker. Circ will get
 * picked up again when the cpuworker finishes decrypting it.
 */
static void
command_process_create_cell(cell_t *cell, connection_t *conn)
{
  circuit_t *circ;
  int id_is_high;

  if (we_are_hibernating()) {
    info(LD_OR,"Received create cell but we're shutting down. Sending back "
         "destroy.");
    connection_or_send_destroy(cell->circ_id, conn,
                               END_CIRC_REASON_HIBERNATING);
    return;
  }

  /* If the high bit of the circuit ID is not as expected, then switch
   * which half of the space we'll use for our own CREATE cells.
   *
   * This can happen because Tor 0.0.9pre5 and earlier decide which
   * half to use based on nickname, and we now use identity keys.
   */
  id_is_high = cell->circ_id & (1<<15);
  if (id_is_high && conn->circ_id_type == CIRC_ID_TYPE_HIGHER) {
    info(LD_OR, "Got a high circuit ID from %s (%d); switching to "
         "low circuit IDs.",
         conn->nickname ? conn->nickname : "client", conn->s);
    conn->circ_id_type = CIRC_ID_TYPE_LOWER;
  } else if (!id_is_high && conn->circ_id_type == CIRC_ID_TYPE_LOWER) {
    info(LD_OR, "Got a low circuit ID from %s (%d); switching to "
         "high circuit IDs.",
         conn->nickname ? conn->nickname : "client", conn->s);
    conn->circ_id_type = CIRC_ID_TYPE_HIGHER;
  }

  circ = circuit_get_by_circid_orconn(cell->circ_id, conn);

  if (circ) {
    routerinfo_t *router = router_get_by_digest(conn->identity_digest);
    log_fn(LOG_PROTOCOL_WARN, LD_PROTOCOL,
           "received CREATE cell (circID %d) for known circ. "
           "Dropping (age %d).",
           cell->circ_id, (int)(time(NULL) - conn->timestamp_created));
    if (router)
      log_fn(LOG_PROTOCOL_WARN, LD_PROTOCOL,
             "Details: nickname '%s', platform '%s'.",
             router->nickname, router->platform);
    return;
  }

  circ = circuit_new(cell->circ_id, conn);
  circ->purpose = CIRCUIT_PURPOSE_OR;
  circuit_set_state(circ, CIRCUIT_STATE_ONIONSKIN_PENDING);
  if (cell->command == CELL_CREATE) {
    circ->onionskin = tor_malloc(ONIONSKIN_CHALLENGE_LEN);
    memcpy(circ->onionskin, cell->payload, ONIONSKIN_CHALLENGE_LEN);

    /* hand it off to the cpuworkers, and then return */
    if (assign_to_cpuworker(NULL, CPUWORKER_TASK_ONION, circ) < 0) {
      warn(LD_GENERAL,"Failed to hand off onionskin. Closing.");
      circuit_mark_for_close(circ, END_CIRC_REASON_INTERNAL);
      return;
    }
    debug(LD_OR,"success: handed off onionskin.");
  } else {
    /* This is a CREATE_FAST cell; we can handle it immediately without using
     * a CPU worker.*/
    char keys[CPATH_KEY_MATERIAL_LEN];
    char reply[DIGEST_LEN*2];
    tor_assert(cell->command == CELL_CREATE_FAST);
    if (fast_server_handshake(cell->payload, reply, keys, sizeof(keys))<0) {
      warn(LD_OR,"Failed to generate key material. Closing.");
      circuit_mark_for_close(circ, END_CIRC_REASON_INTERNAL);
      return;
    }
    if (onionskin_answer(circ, CELL_CREATED_FAST, reply, keys)<0) {
      warn(LD_OR,"Failed to reply to CREATE_FAST cell. Closing.");
      circuit_mark_for_close(circ, END_CIRC_REASON_INTERNAL);
      return;
    }
  }
}

/** Process a 'created' <b>cell</b> that just arrived from <b>conn</b>.
 * Find the circuit
 * that it's intended for. If we're not the origin of the circuit, package
 * the 'created' cell in an 'extended' relay cell and pass it back. If we
 * are the origin of the circuit, send it to circuit_finish_handshake() to
 * finish processing keys, and then call circuit_send_next_onion_skin() to
 * extend to the next hop in the circuit if necessary.
 */
static void
command_process_created_cell(cell_t *cell, connection_t *conn)
{
  circuit_t *circ;

  circ = circuit_get_by_circid_orconn(cell->circ_id, conn);

  if (!circ) {
    info(LD_OR,"(circID %d) unknown circ (probably got a destroy earlier). "
         "Dropping.", cell->circ_id);
    return;
  }

  if (circ->n_circ_id != cell->circ_id) {
    log_fn(LOG_PROTOCOL_WARN,LD_PROTOCOL,
           "got created cell from OPward? Closing.");
    circuit_mark_for_close(circ, END_CIRC_REASON_TORPROTOCOL);
    return;
  }

  if (CIRCUIT_IS_ORIGIN(circ)) { /* we're the OP. Handshake this. */
    debug(LD_OR,"at OP. Finishing handshake.");
    if (circuit_finish_handshake(circ, cell->command, cell->payload) < 0) {
      warn(LD_OR,"circuit_finish_handshake failed.");
      circuit_mark_for_close(circ, END_CIRC_AT_ORIGIN);
      return;
    }
    debug(LD_OR,"Moving to next skin.");
    if (circuit_send_next_onion_skin(circ) < 0) {
      info(LD_OR,"circuit_send_next_onion_skin failed.");
      /* XXX push this circuit_close lower */
      circuit_mark_for_close(circ, END_CIRC_AT_ORIGIN);
      return;
    }
  } else { /* pack it into an extended relay cell, and send it. */
    debug(LD_OR,"Converting created cell to extended relay cell, sending.");
    connection_edge_send_command(NULL, circ, RELAY_COMMAND_EXTENDED,
                                 cell->payload, ONIONSKIN_REPLY_LEN, NULL);
  }
}

/** Process a 'relay' <b>cell</b> that just arrived from <b>conn</b>. Make sure
 * it came in with a recognized circ_id. Pass it on to
 * circuit_receive_relay_cell() for actual processing.
 */
static void
command_process_relay_cell(cell_t *cell, connection_t *conn)
{
  circuit_t *circ;
  int reason;

  circ = circuit_get_by_circid_orconn(cell->circ_id, conn);

  if (!circ) {
    debug(LD_OR,"unknown circuit %d on connection from %s:%d. Dropping.",
          cell->circ_id, conn->address, conn->port);
    return;
  }

  if (circ->state == CIRCUIT_STATE_ONIONSKIN_PENDING) {
    log_fn(LOG_PROTOCOL_WARN,LD_PROTOCOL,"circuit in create_wait. Closing.");
    circuit_mark_for_close(circ, END_CIRC_REASON_TORPROTOCOL);
    return;
  }

  if (cell->circ_id == circ->p_circ_id) { /* it's an outgoing cell */
    if ((reason = circuit_receive_relay_cell(cell, circ,
                                             CELL_DIRECTION_OUT)) < 0) {
      log_fn(LOG_PROTOCOL_WARN,LD_PROTOCOL,"circuit_receive_relay_cell "
             "(forward) failed. Closing.");
      circuit_mark_for_close(circ, -reason);
      return;
    }
  } else { /* it's an ingoing cell */
    if ((reason = circuit_receive_relay_cell(cell, circ,
                                             CELL_DIRECTION_IN)) < 0) {
      log_fn(LOG_PROTOCOL_WARN,LD_PROTOCOL,"circuit_receive_relay_cell "
             "(backward) failed. Closing.");
      circuit_mark_for_close(circ, -reason);
      return;
    }
  }
}

/** Process a 'destroy' <b>cell</b> that just arrived from
 * <b>conn</b>. Find the circ that it refers to (if any).
 *
 * If the circ is in state
 * onionskin_pending, then call onion_pending_remove() to remove it
 * from the pending onion list (note that if it's already being
 * processed by the cpuworker, it won't be in the list anymore; but
 * when the cpuworker returns it, the circuit will be gone, and the
 * cpuworker response will be dropped).
 *
 * Then mark the circuit for close (which marks all edges for close,
 * and passes the destroy cell onward if necessary).
 */
static void
command_process_destroy_cell(cell_t *cell, connection_t *conn)
{
  circuit_t *circ;
  uint8_t reason;

  circ = circuit_get_by_circid_orconn(cell->circ_id, conn);
  reason = (uint8_t)cell->payload[0];
  if (!circ) {
    info(LD_OR,"unknown circuit %d on connection from %s:%d. Dropping.",
         cell->circ_id, conn->address, conn->port);
    return;
  }
  debug(LD_OR,"Received for circID %d.",cell->circ_id);

  if (cell->circ_id == circ->p_circ_id) {
    /* the destroy came from behind */
    circuit_set_circid_orconn(circ, 0, NULL, P_CONN_CHANGED);
    circuit_mark_for_close(circ, reason);
  } else { /* the destroy came from ahead */
    circuit_set_circid_orconn(circ, 0, NULL, N_CONN_CHANGED);
    if (CIRCUIT_IS_ORIGIN(circ)) {
      circuit_mark_for_close(circ, reason);
    } else {
      char payload[1];
      debug(LD_OR, "Delivering 'truncated' back.");
      payload[0] = (char)reason;
      connection_edge_send_command(NULL, circ, RELAY_COMMAND_TRUNCATED,
                                   payload, sizeof(payload), NULL);
    }
  }
}

