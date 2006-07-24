#ifndef __DW_EXT_ITERATOR_H__
#define __DW_EXT_ITERATOR_H__

#include "dw_widget.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _DwExtIterator
{
   DwIterator **stack;
   gint stack_top;
   gint stack_max;

   DwContent content;
} DwExtIterator;

typedef struct _DwWordIterator
{
   /* The current word. NULL, when at the end or at the beginning */
   gchar *word;

   /* private stuff */
   DwExtIterator *iterator;
   gint *word_splitpos;     /* the positions of the words within
                             * iterator->content.data.text, as returned
                             * by a_Misc_strsplitpos */
   gint word_pos;           /* the  current position within word_splitpos */
   gint content_hl_start, content_hl_end;
} DwWordIterator;

DwExtIterator* a_Dw_ext_iterator_new         (DwIterator *it);
DwExtIterator* a_Dw_ext_iterator_new_variant (DwExtIterator *base,
                                              DwIterator *it);
gboolean       a_Dw_ext_iterator_next        (DwExtIterator *eit);
gboolean       a_Dw_ext_iterator_prev        (DwExtIterator *eit);
DwExtIterator* a_Dw_ext_iterator_clone       (DwExtIterator *eit);
gint           a_Dw_ext_iterator_compare     (DwExtIterator *eit1,
                                              DwExtIterator *eit2);
void           a_Dw_ext_iterator_free        (DwExtIterator *eit);

#define        a_Dw_ext_iterator_highlight(eit, s, e, l) \
                  a_Dw_iterator_highlight((eit)->stack[(eit)->stack_top], \
                                          s, e, l)
#define        a_Dw_ext_iterator_unhighlight(eit, l) \
                  a_Dw_iterator_unhighlight((eit)->stack[(eit)->stack_top], l)
#define        a_Dw_ext_iterator_get_allocation(eit, s, e, a) \
                  a_Dw_iterator_get_allocation((eit)->stack[(eit)->stack_top],\
                                               s, e, a)
#define        a_Dw_ext_iterator_scroll_to(eit1, eit2, s, e, hp, vp) \
                  a_Dw_iterator_scroll_to((eit1)->stack[(eit1)->stack_top], \
                                          (eit2)->stack[(eit2)->stack_top], \
                                           s, e, hp, vp)
#define        a_Dw_ext_iterator_text(eit) ((eit) ? \
                     a_Dw_iterator_text((eit)->stack[(eit)->stack_top]) : \
                     "[NULL]")

DwWordIterator* a_Dw_word_iterator_new           (DwWidget *widget);
gboolean        a_Dw_word_iterator_next          (DwWordIterator *it);
gboolean        a_Dw_word_iterator_prev          (DwWordIterator *it);
void            a_Dw_word_iterator_highlight      (DwWordIterator *it,
                                                   gint start,
                                                   gint end,
                                                   DwHighlightLayer layer);
void            a_Dw_word_iterator_get_allocation (DwWordIterator *it,
                                                   gint start,
                                                   gint end,
                                                   DwAllocation *allocation);
void            a_Dw_word_iterator_scroll_to      (DwWordIterator *it1,
                                                   DwWordIterator *it2,
                                                   gint start,
                                                   gint end,
                                                   DwHPosition hpos,
                                                   DwVPosition vpos);
DwWordIterator* a_Dw_word_iterator_clone          (DwWordIterator *it);
void            a_Dw_word_iterator_free           (DwWordIterator *it);

#define         a_Dw_word_iterator_unhighlight(it, l) \
                   a_Dw_word_iterator_highlight(it, -1, -1, l)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_EXT_ITERATOR_H__ */
