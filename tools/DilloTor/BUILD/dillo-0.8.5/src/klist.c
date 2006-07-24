/*
 * File: klist.c
 *
 * Copyright 2001 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * A simple ADT for Key-Data pairs
 *
 * NOTE: this ADT is not perfect. The possibility of holding a Key, after
 * its data has been removed, long enough for the key-counter to reset and
 * reuse the same key is very low, but EXISTS. So, the responsibility
 * remains with the caller.
 */

#include "klist.h"


/*
 * Compare function for searching data by its key
 */
static gint Klist_key_cmp(gconstpointer Node, gconstpointer key)
{
   return ( GPOINTER_TO_INT(key) != ((KlistNode_t *)Node)->Key );
}

/*
 * Return the data pointer for a given Key (or NULL if not found)
 */
gpointer a_Klist_get_data(Klist_t *Klist, gint Key)
{
   GSList *list;

   if (!Klist)
      return NULL;
   list= g_slist_find_custom(Klist->List, GINT_TO_POINTER(Key), Klist_key_cmp);
   return (list) ? ((KlistNode_t *)list->data)->Data : NULL;
}

/*
 * Insert a data pointer and return a key for it.
 */
gint a_Klist_insert(Klist_t **Klist, gpointer Data)
{
   KlistNode_t *Node;

   if (!*Klist) {
      (*Klist) = g_new(Klist_t, 1);
      (*Klist)->List = NULL;
      (*Klist)->Clean = 1;
      (*Klist)->Counter = 0;
   }

   /* This avoids repeated keys at the same time */
   do {
      if ( ++((*Klist)->Counter) == 0 ) {
         (*Klist)->Counter = 1;
         (*Klist)->Clean = 0;
      }
   } while (!((*Klist)->Clean) &&
            a_Klist_get_data((*Klist), (*Klist)->Counter));

   Node = g_new(KlistNode_t, 1);
   Node->Key  = (*Klist)->Counter;
   Node->Data = Data;
   (*Klist)->List = g_slist_prepend((*Klist)->List, Node);

   return (*Klist)->Counter;
}

/*
 * Remove data by Key
 */
void a_Klist_remove(Klist_t *Klist, gint Key)
{
   GSList *list;

   list= g_slist_find_custom(Klist->List, GINT_TO_POINTER(Key), Klist_key_cmp);
   if (list) {
      g_free(list->data);
      Klist->List = g_slist_remove(Klist->List, list->data);
   }
   if (Klist->List == NULL)
      Klist->Clean = 1;
}

/*
 * Free a Klist
 */
void a_Klist_free(Klist_t **KlistPtr)
{
   gpointer node;
   Klist_t *Klist = *KlistPtr;

   if (!Klist)
      return;

   while (Klist && Klist->List) {
      node = Klist->List->data;
      Klist->List = g_slist_remove(Klist->List, node);
      g_free(node);
   }
   g_free(Klist);
   *KlistPtr = NULL;
}

