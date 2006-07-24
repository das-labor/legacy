#ifndef __KLIST_H__
#define __KLIST_H__

#include <glib.h>


typedef struct _KlistNode KlistNode_t;
typedef struct _Klist Klist_t;

struct _KlistNode {
   gint Key;        /* primary key */
   gpointer *Data;  /* data reference */
};

struct _Klist {
   GSList *List;
   gint Clean;      /* check flag */
   gint Counter;    /* counter (for making keys) */
};


/*
 * Function prototypes
 */
gpointer  a_Klist_get_data(Klist_t *Klist, gint Key);
gint      a_Klist_insert(Klist_t **Klist, gpointer Data);
void      a_Klist_remove(Klist_t *Klist, gint Key);
void      a_Klist_free(Klist_t **Klist);


#endif /* __KLIST_H__ */
