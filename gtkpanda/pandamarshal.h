
#ifndef __panda_marshal_MARSHAL_H__
#define __panda_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:INT,INT (pandamarshal.list:25) */
extern void panda_marshal_VOID__INT_INT (GClosure     *closure,
                                         GValue       *return_value,
                                         guint         n_param_values,
                                         const GValue *param_values,
                                         gpointer      invocation_hint,
                                         gpointer      marshal_data);

/* VOID:POINTER (pandamarshal.list:26) */
#define panda_marshal_VOID__POINTER	g_cclosure_marshal_VOID__POINTER

/* VOID:INT,INT,POINTER (pandamarshal.list:27) */
extern void panda_marshal_VOID__INT_INT_POINTER (GClosure     *closure,
                                                 GValue       *return_value,
                                                 guint         n_param_values,
                                                 const GValue *param_values,
                                                 gpointer      invocation_hint,
                                                 gpointer      marshal_data);

/* VOID:VOID (pandamarshal.list:28) */
#define panda_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

/* VOID:STRING,STRING (pandamarshal.list:29) */
extern void panda_marshal_VOID__STRING_STRING (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

G_END_DECLS

#endif /* __panda_marshal_MARSHAL_H__ */

