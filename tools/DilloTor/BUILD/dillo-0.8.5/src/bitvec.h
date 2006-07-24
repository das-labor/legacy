#ifndef __BITVEC_H__
#define __BITVEC_H__

#include <glib.h>

#define BVEC_TYPE guchar
#define BVEC_SIZE sizeof(BVEC_TYPE)

typedef struct _bitvec bitvec_t;

struct _bitvec {
   BVEC_TYPE *vec;
   gint len;       /* number of bits [1 based] */
};


/*
 * Function prototypes
 */
bitvec_t *a_Bitvec_new(gint bits);
void a_Bitvec_free(bitvec_t *bvec);
gint a_Bitvec_get_bit(bitvec_t *bvec, gint pos);
void a_Bitvec_set_bit(bitvec_t *bvec, gint pos);

/*
#define a_Bitvec_get_bit(bvec,pos) \
   ((bvec)->vec[(pos)/BVEC_SIZE] & 1 << (pos) % BVEC_SIZE)

#define a_Bitvec_set_bit(bvec,pos) \
   ((bvec)->vec[(pos)/BVEC_SIZE] |= 1 << (pos) % BVEC_SIZE)
*/
#define a_Bitvec_clear_bit(bvec,pos) \
   ((bvec)->vec[(pos)/BVEC_SIZE] &= ~(1 << (pos) % BVEC_SIZE))


#endif /* __BITVEC_H__ */
