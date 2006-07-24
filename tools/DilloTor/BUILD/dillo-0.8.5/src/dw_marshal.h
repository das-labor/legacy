#ifndef __DW_MARSHAL_H__
#define __DW_MARSHAL_H__

#include <gtk/gtktypeutils.h>
#include <gtk/gtkobject.h>

void p_Dw_marshal_BOOL__INT_INT_INT (GtkObject *object,
                                     GtkSignalFunc Func,
                                     gpointer FuncData, GtkArg *args);
void p_Dw_marshal_BOOL__INT_INT_INT_POINTER (GtkObject *object,
                                             GtkSignalFunc Func,
                                             gpointer FuncData, GtkArg *args);
void p_Dw_marshal_BOOL__INT_INT_POINTER (GtkObject *object,
                                         GtkSignalFunc Func,
                                         gpointer FuncData, GtkArg *args);
void p_Dw_marshal_BOOL__POINTER_POINTER (GtkObject *object,
                                         GtkSignalFunc Func,
                                         gpointer FuncData, GtkArg *args);

/*
 * Marshal fuctions for standard link signals.
 */
#define p_Dw_marshal_link_enter  p_Dw_marshal_BOOL__INT_INT_INT
#define p_Dw_marshal_link_button p_Dw_marshal_BOOL__INT_INT_INT_POINTER


#endif /* __DW_MARSHAL_H__ */
