#ifndef __DW_ALIGNED_PAGE_H__
#define __DW_ALIGNED_PAGE_H__

#include "dw_page.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_ALIGNED_PAGE          (a_Dw_aligned_page_get_type ())
#define DW_ALIGNED_PAGE(obj)          GTK_CHECK_CAST (obj, \
                                         DW_TYPE_ALIGNED_PAGE, DwAlignedPage)
#define DW_ALIGNED_PAGE_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, \
                                         DW_TYPE_ALIGNED_PAGE, \
                                         DwAlignedPageClass)
#define DW_IS_ALIGNED_PAGE(obj)       GTK_CHECK_TYPE (obj, \
                                         DW_TYPE_ALIGNED_PAGE)

typedef struct _DwAlignedPage      DwAlignedPage;
typedef struct _DwAlignedPageClass DwAlignedPageClass;
typedef struct _DwAlignedPageList  DwAlignedPageList;

struct _DwAlignedPage
{
   DwPage page;

   DwAlignedPageList *list;
   gint list_pos;
};

struct _DwAlignedPageClass
{
   DwPageClass parent_class;

   gint32 (*get_value)     (DwAlignedPage *aligned_page);
   void   (*set_max_value) (DwAlignedPage *aligned_page,
                            gint32 max_value,
                            gint32 value);
};


struct _DwAlignedPageList
{
   gint num;      /* The index of the last page added (minus one). */
   gint refcount; /* The numbers of non-NULL pages, may be smaller than num.
                   * The distinction is necessary, because removing is
                   * simply done by assigning NULL. */
   DwAlignedPage **pages;
   gint32 *values;
   gint num_pages_max;
   gint num_values_max;

   gint32 max_value;
};


GtkType a_Dw_aligned_page_get_type (void);

void    p_Dw_aligned_page_set_ref_page  (DwAlignedPage *aligned_page,
                                         DwAlignedPage *ref_page);
void    p_Dw_aligned_page_update_value  (DwAlignedPage *aligned_page);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_ALIGNED_PAGE_H__ */
