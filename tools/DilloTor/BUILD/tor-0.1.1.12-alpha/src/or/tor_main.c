/* Copyright 2001-2004 Roger Dingledine.
 * Copyright 2004-2005 Roger Dingledine, Nick Mathewson. */
/* See LICENSE for licensing information */
/* $Id: tor_main.c,v 1.13 2005/12/14 20:40:40 nickm Exp $ */
const char tor_main_c_id[] =
  "$Id: tor_main.c,v 1.13 2005/12/14 20:40:40 nickm Exp $";

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

