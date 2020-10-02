/*
 * STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2020 Gordon N. Squash.
 *
 * This Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * A FIRST NOTE ABOUT THIS FILE:
 *
 * All I can say is that I hope it's not as much of a preprocessor macro soup
 * as it used to be.  It's still a little tough to read in places, but the code
 * is much cleaner than the internals of gtype.h, for instance!
 */

#ifndef __STLWRT_H__
#define __STLWRT_H__


#include <glib.h>

G_BEGIN_DECLS

/*
 * At the moment GLib does not have any macro or anything defined to indicate
 * a type should be defined with no special flags; the conventional way to
 * define such a type is with the flags set to 0.  However, I think (for now)
 * that having a preprocessor macro for this purpose makes it clearer that
 * we're setting the type flags (and not something else) to zero.  If in the
 * future GLib implements this as an enum or a preprocessor macro, uncomment
 * the 'if' and 'endif' lines below and change the phony version number to the
 * version of GLib when the macro was officially added.
 */
//#if GLIB_CHECK_VERSION(2,68,0)
 #define G_TYPE_FLAG_NONE 0
//#endif

/****************************************************************************/
/* The following macros are for declaring functions in header files. */

#ifdef STLWRT_COMPILATION

 #define SF(function_name)  __##function_name

 #define SVF(function_name, version)  \
   _##version##_##function_name

 #define STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(type_name) \
   GType __##type_name##_get_type (void) G_GNUC_CONST; \
   GType     type_name##_get_type (void) G_GNUC_CONST;

 #define STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(type_name) \
   GType _T2_##type_name##_get_type (void) G_GNUC_CONST; \
   GType _3T_##type_name##_get_type (void) G_GNUC_CONST; \
   GType       type_name##_get_type (void) G_GNUC_CONST;

#else

 #define SF(function_name)      function_name

 #define SVF(function_name, version)  function_name

 #define STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(type_name) \
   GType     type_name##_get_type (void) G_GNUC_CONST;

 #define STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(type_name) \
   GType     type_name##_get_type (void) G_GNUC_CONST;

#endif

/****************************************************************************/
/* The following stuff deals with _defining_ types, in C source files. */

#define STLWRT_DEFINE_BOXED_TYPE(TN, t_n, copy_func, free_func, C) \
  GType \
  t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_boxed_type_register_static (g_intern_static_string (#TN), \
                                                       (GBoxedCopyFunc) copy_func, \
                                                       (GBoxedFreeFunc) free_func); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  }

/**
 * STLWRT_DEFINE_FTYPE:
 * STLWRT_DEFINE_VTYPE:
 *  @TN:  The name of the type in Camel Case (like GtkVBox)
 *  @t_n: The name of the type in lowercase (like gtk_vbox)
 *  @PT:  The actual type of this type's parent (like GTK_TYPE_BOX)
 *  @F:   Type flags.  If you want to create an abstract type, use
 *        #G_TYPE_FLAG_ABSTRACT.  If not, pass either 0 or #G_TYPE_FLAG_NONE.
 *        (Please note that #G_TYPE_FLAG_NONE is defined in this file, not
 *        GLib.  It may someday become part of GLib, however.)
 *  @C:   Code to insert into the type definition function.  If you want to
 *        implement an interface, you can use #G_IMPLEMENT_INTERFACE.  If
 *        you want to add private data, you may use #G_ADD_PRIVATE.
 *        If you want neither, insert a single semicolon.
 *
 * Defines a new type for a STLWRT object.  The #STLWRT_DEFINE_FTYPE macro
 * defines a "fixed-size type" -- a type which has the same instance size no
 * matter what version of GTK STLWRT is trying to emulate; the
 * #STLWRT_DEFINE_VTYPE macro defines a "variable-size type" -- a type which
 * has a different instance size when STLWRT is running in GTK+ 2 mode than
 * when STLWRT is running in GTK+ 3 mode.  GtkTreeView is an Ftype; GtkDialog
 * is a Vtype.
 */
#define STLWRT_DEFINE_FTYPE(TN, t_n, PT, F, C) \
  static void     t_n##_init              (TN        *self); \
  static void     t_n##_class_init        (TN##Class *klass); \
  static gpointer t_n##_parent_class  = NULL; \
  static gint     TN##_private_offset = 0; \
 \
  static void     t_n##_class_intern_init (gpointer klass) \
  { \
    t_n##_parent_class = g_type_class_peek_parent (klass); \
    if (TN##_private_offset != 0) \
      g_type_class_adjust_private_offset (klass, &TN##_private_offset); \
    t_n##_class_init ((TN##Class*) klass); \
  } \
 \
  G_GNUC_UNUSED \
  static inline gpointer \
  t_n##_get_instance_private (TN *self) \
  { \
    return (G_STRUCT_MEMBER_P (self, TN##_private_offset)); \
  } \
 \
  GType \
  __##t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_type_register_static_simple (PT, \
                                     g_intern_static_string (#TN), \
                                     sizeof (TN##Class), \
                                     (GClassInitFunc)(void (*)(void)) t_n##_class_intern_init, \
                                     sizeof (TN), \
                                     (GInstanceInitFunc)(void (*)(void)) t_n##_init, \
                                     (GTypeFlags) F); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  }


#define STLWRT_DEFINE_FTYPE_VPARENT(TN, t_n, PT, F, C) \
  static void     t_n##_init              (TN        *self); \
  static void     t_n##_class_init        (TN##Class *klass); \
  static gpointer t_n##_parent_class  = NULL; \
  static gint     TN##_private_offset = 0; \
 \
  static void     t_n##_class_intern_init (gpointer klass) \
  { \
    t_n##_parent_class = g_type_class_peek_parent (klass); \
 \
    g_type_class_adjust_private_offset (klass, &TN##_private_offset); \
 \
    t_n##_class_init ((TN##Class*) klass); \
  } \
 \
  G_GNUC_UNUSED \
  gpointer \
  t_n##_get_instance_private (TN *self) \
  { \
    return (G_STRUCT_MEMBER_P (self, TN##_private_offset)); \
  } \
 \
  GType \
  _T2_##t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_type_register_static_simple (PT, \
                                     g_intern_static_string (#TN), \
                                     sizeof (TN##Class), \
                                     (GClassInitFunc)(void (*)(void)) t_n##_class_intern_init, \
                                     sizeof (TN##Fat), \
                                     (GInstanceInitFunc)(void (*)(void)) t_n##_init, \
                                     (GTypeFlags) F); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  } \
 \
  GType \
  _3T_##t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_type_register_static_simple (PT, \
                                     g_intern_static_string (#TN), \
                                     sizeof (TN##Class), \
                                     (GClassInitFunc)(void (*)(void)) t_n##_class_intern_init, \
                                     sizeof (TN##Thin), \
                                     (GInstanceInitFunc)(void (*)(void)) t_n##_init, \
                                     (GTypeFlags) F); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  }


#define STLWRT_DEFINE_VTYPE(TN, t_n, PT, F, C) \
  static void     t_n##_init              (TN        *self); \
  static void     t_n##_class_init        (TN##Class *klass); \
  static gpointer t_n##_parent_class  = NULL; \
  static gint     TN##_private_offset = 0; \
  static gint     TN##_props_offset   = 0; \
 \
  static void     t_n##_class_intern_init (gpointer klass) \
  { \
    gint       props_size = TN##_props_offset; \
    GTypeQuery parent_type_query; \
 \
    t_n##_parent_class = g_type_class_peek_parent (klass); \
 \
    TN##_private_offset += props_size; \
 \
    if (TN##_private_offset != 0) \
    { \
      g_type_class_adjust_private_offset (klass, &TN##_private_offset); \
      TN##_props_offset = TN##_private_offset; \
      TN##_private_offset += TN##_props_size; \
    } \
    else \
    { \
      g_type_query (PT, &parent_type_query); \
      TN##_props_offset = parent_type_query.instance_size; \
    } \
 \
    t_n##_class_init ((TN##Class*) klass); \
  } \
 \
  G_GNUC_UNUSED \
  gpointer \
  t_n##_get_instance_private (TN *self) \
  { \
    return (G_STRUCT_MEMBER_P (self, TN##_private_offset)); \
  } \
 \
  G_GNUC_UNUSED \
  static inline gpointer \
  t_n##_get_props (TN *self) \
  { \
    return (G_STRUCT_MEMBER_P (self, TN##_props_offset)); \
  } \
 \
  GType \
  _T2_##t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_type_register_static_simple (PT, \
                                     g_intern_static_string (#TN), \
                                     sizeof (TN##Class), \
                                     (GClassInitFunc)(void (*)(void)) t_n##_class_intern_init, \
                                     sizeof (TN##Fat), \
                                     (GInstanceInitFunc)(void (*)(void)) t_n##_init, \
                                     (GTypeFlags) F); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  } \
 \
  GType \
  _3T_##t_n##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0)  \
    { \
      g_define_type_id = g_type_register_static_simple (PT, \
                                     g_intern_static_string (#TN), \
                                     sizeof (TN##Class), \
                                     (GClassInitFunc)(void (*)(void)) t_n##_class_intern_init, \
                                     sizeof (TN##Thin), \
                                     (GInstanceInitFunc)(void (*)(void)) t_n##_init, \
                                     (GTypeFlags) F); \
      TN##_props_offset = sizeof(TN##Props); \
      { \
        C \
      } \
    } \
    return g_define_type_id; \
  }

#define STLWRT_DEFINE_INTERFACE(TypeName, type_name, TYPE_PREREQ, init_func, C) \
  GType \
  __##type_name##_get_type (void) \
  { \
    static GType g_define_type_id = 0; \
    if (g_define_type_id == 0) \
    { \
      g_type_register_static_simple (G_TYPE_INTERFACE, \
                                     g_intern_static_string (#TypeName), \
                                     sizeof (TypeName##Interface), \
                                     (GClassInitFunc)(void (*)(void)) init_func, \
                                     0, \
                                     (GInstanceInitFunc)NULL, \
                                     (GTypeFlags) 0); \
      if (TYPE_PREREQ != G_TYPE_INVALID) \
        g_type_interface_add_prerequisite (g_define_type_id, TYPE_PREREQ); \
      { \
        C \
      }						\
    }						\
  return g_define_type_id;			\
}

/****************************************************************************/
/* The following stuff deals with _declaring_ types, in C header files. */

#define STLWRT_DECLARE_OPAQUE_TYPE(TN, t_n) \
  typedef struct __##TN TN; \
 \
  STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(t_n)

#define STLWRT_DECLARE_BOXED_TYPE(TN, t_n, Properties) \
  typedef struct _##TN TN; \
 \
  struct _##TN \
  { \
    Properties \
  }; \
 \
  STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(t_n)

#define STLWRT_DECLARE_FTYPE_FPARENT(TN, t_n, PTN, Properties) \
  typedef struct _##TN TN; \
 \
  struct _##TN \
  { \
    PTN parent; \
 \
    Properties \
  }; \
 \
  STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(t_n)

#ifdef STLWRT_COMPILATION

 #define STLWRT_DECLARE_FTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN##Fat   TN##Fat; \
   typedef struct _##TN##Thin  TN##Thin; \
   typedef union  _##TN        TN; \
  \
   struct _##TN##Fat \
   { \
     PTN##Fat parent; \
  \
     Properties \
   }; \
  \
   struct _##TN##Thin \
   { \
     PTN##Thin parent; \
  \
     Properties \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_FPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN##Props TN##Props; \
   typedef struct _##TN##Fat   TN##Fat; \
   typedef struct _##TN##Thin  TN##Thin; \
   typedef union  _##TN        TN; \
  \
   struct _##TN##Props \
   { \
     Properties \
   }; \
  \
   struct _##TN##Fat \
   { \
     PTN parent; \
  \
     Properties \
   }; \
  \
   struct _##TN##Thin \
   { \
     PTN parent; \
  \
     gpointer reserved; \
   }; \
  \
   gpointer t_n##_get_props (TN * instance); \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN##Props TN##Props; \
   typedef struct _##TN##Fat   TN##Fat; \
   typedef struct _##TN##Thin  TN##Thin; \
   typedef union  _##TN        TN; \
  \
   struct _##TN##Props \
   { \
     Properties \
   }; \
  \
   struct _##TN##Fat \
   { \
     PTN##Fat parent; \
  \
     Properties \
   }; \
  \
   struct _##TN##Thin \
   { \
     PTN##Thin parent; \
  \
     gpointer reserved; \
   }; \
  \
   gpointer t_n##_get_props (TN * instance); \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

#elif STLWRT_GTK_VERSION <= 2

 #define STLWRT_DECLARE_FTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN   TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     Properties \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_FPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN   TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     Properties \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN   TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     Properties \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

#elif STLWRT_GTK_VERSION >= 3

 #define STLWRT_DECLARE_FTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN  TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     Properties \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_FPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN  TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     gpointer reserved; \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

 #define STLWRT_DECLARE_VTYPE_VPARENT(TN, t_n, PTN, Properties) \
   typedef struct _##TN  TN; \
  \
   struct _##TN \
   { \
     PTN parent; \
  \
     gpointer reserved; \
   }; \
  \
   STLWRT_DECLARE_GET_VTYPE_FUNCTIONS(t_n)

#endif

#define STLWRT_DECLARE_INTERFACE(TN, t_n, Properties) \
  STLWRT_DECLARE_OPAQUE_TYPE(TN, t_n) \
  typedef struct _##TN##Iface TN##Iface; \
 \
  struct _##TN##Iface \
  { \
    GTypeInterface g_iface; \
 \
    Properties \
  }; \
 \
  STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(t_n)

G_END_DECLS

#endif /* __STLWRT_H__ */
