#ifndef __FINDTEXT_H__
#define __FINDTEXT_H__

#include "dw_ext_iterator.h"

/*
 * Return values of a_Findtext_search.
 */
typedef enum {
   FINDTEXT_SUCCESS,     /* The next occurance of the pattern has been
                            found. */
   FINDTEXT_RESTART,     /* There is no further occurance of the pattern,
                            instead, the first occurance has been selected. */
   FINDTEXT_NOT_FOUND    /* The patten does not at all occur in the text. */
} FindtextResult;

/*
 * See Findtext_compare for an explanation.
 */
typedef enum {
   FINDTEXT_FULL,
   FINDTEXT_FREE,
   FINDTEXT_LEFT,
   FINDTEXT_RIGHT
} FindtextPatternType;

typedef struct _FindtextState FindtextState;
typedef struct _FindtextKey   FindtextKey;

/*
 * This structure is associated by the GtkDwViewport.
 */
struct _FindtextState
{
   FindtextKey *key;
   gchar *keystr;
   gboolean case_sens;
   DwWidget *widget;             /* The top of the widget tree, in which
                                    the search is done. From this, the
                                    iterator will be constructed. */
   DwWordIterator *iterator;     /* The position from where the next search
                                    will start. */
   DwWordIterator *hl_iterator;  /* The position from where key->len words
                                    are highlighted. */
   gint first_hl_start, last_hl_end;
};

struct _FindtextKey
{
   gint len;
   gchar **words;
   FindtextPatternType *types;
   gint *nexttab;
};

FindtextState* a_Findtext_state_new        (void);
void           a_Findtext_state_destroy    (FindtextState *state);
void           a_Findtext_state_set_widget (FindtextState *state,
                                            DwWidget *widget);

FindtextResult a_Findtext_search           (FindtextState *state,
                                            gchar *str,
                                            gboolean case_sens);
void           a_Findtext_reset_search     (FindtextState *state);

#endif /* __FINDTEXT_H__ */
