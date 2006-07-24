#include "dw_marshal.h"


typedef gboolean (*GtkSignal_BOOL__INT_INT_INT) (GtkObject *object,
                                                 gint arg1, gint arg2,
                                                 gint arg3,
                                                 gpointer user_data);

void p_Dw_marshal_BOOL__INT_INT_INT (GtkObject *object,
                                     GtkSignalFunc Func,
                                     gpointer FuncData, GtkArg *args)
{
   GtkSignal_BOOL__INT_INT_INT rfunc;
   gboolean *return_val;
   return_val = GTK_RETLOC_BOOL (args[3]);
   rfunc = (GtkSignal_BOOL__INT_INT_INT) Func;
   *return_val = rfunc (object,
                        GTK_VALUE_INT (args[0]), GTK_VALUE_INT (args[1]),
                        GTK_VALUE_INT (args[2]), FuncData);
}


typedef gboolean (*GtkSignal_BOOL__INT_INT_INT_POINTER) (GtkObject *object,
                                                         gint arg1, gint arg2,
                                                         gint arg3,
                                                         gpointer arg4,
                                                         gpointer user_data);

void p_Dw_marshal_BOOL__INT_INT_INT_POINTER (GtkObject *object,
                                             GtkSignalFunc Func,
                                             gpointer FuncData, GtkArg *args)
{
   GtkSignal_BOOL__INT_INT_INT_POINTER rfunc;
   gboolean *return_val;
   return_val = GTK_RETLOC_BOOL (args[4]);
   rfunc = (GtkSignal_BOOL__INT_INT_INT_POINTER) Func;
   *return_val = rfunc (object,
                        GTK_VALUE_INT (args[0]), GTK_VALUE_INT (args[1]),
                        GTK_VALUE_INT (args[2]), GTK_VALUE_POINTER (args[3]),
                        FuncData);
}

typedef gboolean (*GtkSignal_BOOL__INT_INT_POINTER) (GtkObject *object,
                                                     gint arg1, gint arg2,
                                                     gpointer arg3,
                                                     gpointer user_data);

void p_Dw_marshal_BOOL__INT_INT_POINTER (GtkObject *object,
                                         GtkSignalFunc Func,
                                         gpointer FuncData, GtkArg *args)
{
   GtkSignal_BOOL__INT_INT_POINTER rfunc;
   gboolean *return_val;
   return_val = GTK_RETLOC_BOOL (args[3]);
   rfunc = (GtkSignal_BOOL__INT_INT_POINTER) Func;
   *return_val = rfunc (object,
                        GTK_VALUE_INT (args[0]), GTK_VALUE_INT (args[1]),
                        GTK_VALUE_POINTER (args[2]), FuncData);
}

typedef gboolean (*GtkSignal_BOOL__POINTER_POINTER) (GtkObject *object,
                                                     gpointer arg1,
                                                     gpointer arg2,
                                                     gpointer user_data);

void p_Dw_marshal_BOOL__POINTER_POINTER (GtkObject *object,
                                         GtkSignalFunc Func,
                                         gpointer FuncData, GtkArg *args)
{
   GtkSignal_BOOL__POINTER_POINTER rfunc;
   gboolean *return_val;
   return_val = GTK_RETLOC_BOOL (args[2]);
   rfunc = (GtkSignal_BOOL__POINTER_POINTER) Func;
   *return_val = rfunc (object,
                        GTK_VALUE_POINTER (args[0]),
                        GTK_VALUE_POINTER (args[1]), FuncData);
}
