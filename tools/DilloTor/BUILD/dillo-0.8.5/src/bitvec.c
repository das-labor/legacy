/*
 * File: bitvec.c
 *
 * Copyright 2001 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * A simple ADT for bit arrays
 */

#include "bitvec.h"


/*
 * Create a new bitvec with 'num_bits' size
 */
bitvec_t *a_Bitvec_new(gint num_bits)
{
   bitvec_t *bvec = g_new(bitvec_t, 1);

   bvec->vec = g_new0(guchar, num_bits/BVEC_SIZE + 1);
   bvec->len = num_bits;
   return bvec;
}

/*
 * Free a bitvec
 */
void a_Bitvec_free(bitvec_t *bvec)
{
   if (bvec) {
      g_free(bvec->vec);
      g_free(bvec);
   }
}

/*
 * Get a bit
 */
gint a_Bitvec_get_bit(bitvec_t *bvec, gint pos)
{
   g_return_val_if_fail (pos < bvec->len, 0);
   return (bvec->vec[pos/BVEC_SIZE] & 1 << pos % BVEC_SIZE);
}

/*
 * Set a bit
 */
void a_Bitvec_set_bit(bitvec_t *bvec, gint pos)
{
   g_return_if_fail (pos < bvec->len);
   bvec->vec[pos/BVEC_SIZE] |= 1 << (pos % BVEC_SIZE);
}
