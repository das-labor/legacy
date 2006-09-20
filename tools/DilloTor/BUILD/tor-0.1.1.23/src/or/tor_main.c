/* Copyright 2001-2004 Roger Dingledine.
 * Copyright 2004-2006 Roger Dingledine, Nick Mathewson. */
/* See LICENSE for licensing information */
/* $Id: tor_main.c 5949 2006-02-09 05:46:49Z arma $ */
const char tor_main_c_id[] =
  "$Id: tor_main.c 5949 2006-02-09 05:46:49Z arma $";

/**
 * \file tor_main.c
 * \brief Stub module containing a main() function. Allows unit
 * test binary to link against main.c.
 **/

int tor_main(int argc, char *argv[]);

/** We keep main() in a separate file so that our unit tests can use
 * functions from main.c)
 */
int
main(int argc, char *argv[])
{
  return tor_main(argc, argv);
}

