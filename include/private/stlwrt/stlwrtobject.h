
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

#ifndef __STLWRT_OBJECT_H__
#define __STLWRT_OBJECT_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif


G_BEGIN_DECLS


/* Definitions which STLWRT components use. */

#define STLWRT_DEFINE_TYPE(TN, t_n, T_P) \
         STLWRT_DEFINE_TYPE_EXTENDED (TN, t_n, T_P, 0, {})


#define STLWRT_DEFINE_TYPE_WITH_CODE(TN, t_n, T_P, _C_)	\
         _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, 0) \
         {_C_;} \
         _STLWRT_DEFINE_TYPE_EXTENDED_END()


#define STLWRT_DEFINE_ABSTRACT_TYPE(TN, t_n, T_P) \
         STLWRT_DEFINE_TYPE_EXTENDED (TN, t_n, T_P, G_TYPE_FLAG_ABSTRACT, {})


#define STLWRT_DEFINE_ABSTRACT_TYPE_WITH_CODE(TN, t_n, T_P, _C_) \
         _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, G_TYPE_FLAG_ABSTRACT) \
         {_C_;} \
         _STLWRT_DEFINE_TYPE_EXTENDED_END()


#define STLWRT_DEFINE_INTERFACE(TN, t_n, T_P) \
         STLWRT_DEFINE_INTERFACE_WITH_CODE(TN, t_n, T_P, ;)


#define STLWRT_DEFINE_TYPE_WITH_PRIVATE(TN, t_n, T_P) \
         STLWRT_DEFINE_TYPE_EXTENDED (TN, t_n, T_P, 0, G_ADD_PRIVATE (TN))


/* Definitions only used within this file. */

#define STLWRT_DEFINE_TYPE_EXTENDED(TN, t_n, T_P, _f_, _C_) \
         _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN (TN, t_n, T_P, _f_) \
         {_C_;} \
         _STLWRT_DEFINE_TYPE_EXTENDED_END()


#define _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN(TypeName, type_name, TYPE_PARENT, flags) \
  _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN_PRE(TypeName, type_name, TYPE_PARENT) \
  _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN_REGISTER(TypeName, type_name, TYPE_PARENT, flags) \



#define _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN_PRE(TypeName, type_name, TYPE_PARENT) \
\
static void     type_name##_init              (TypeName        *self); \
static void     type_name##_class_init        (TypeName##Class *klass); \
static GType    type_name##_get_type_once     (void); \
static gpointer type_name##_parent_class = NULL; \
static gint     TypeName##_private_offset; \
\
_STLWRT_DEFINE_TYPE_EXTENDED_CLASS_INIT(TypeName, type_name) \
\
G_GNUC_UNUSED \
static inline gpointer \
type_name##_get_instance_private (TypeName *self) \
{ \
  return (G_STRUCT_MEMBER_P (self, TypeName##_private_offset)); \
} \
\
GType \
__##type_name##_get_type (void) \
{ \
  static volatile gsize g_define_type_id__volatile = 0;
  /* Prelude goes here */

/* Added for _STLWRT_DEFINE_TYPE_EXTENDED_WITH_PRELUDE */
#define _STLWRT_DEFINE_TYPE_EXTENDED_BEGIN_REGISTER(TypeName, type_name, TYPE_PARENT, flags) \
  if (g_once_init_enter (&g_define_type_id__volatile))  \
    { \
      GType g_define_type_id = type_name##_get_type_once (); \
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id); \
    }					\
  return g_define_type_id__volatile;	\
} /* closes type_name##_get_type() */ \
\
G_GNUC_NO_INLINE \
static GType \
type_name##_get_type_once (void) \
{ \
  GType g_define_type_id = \
        g_type_register_static_simple (TYPE_PARENT, \
                                       g_intern_static_string (#TypeName), \
                                       sizeof (TypeName##Class), \
                                       (GClassInitFunc)(void (*)(void)) type_name##_class_intern_init, \
                                       sizeof (TypeName), \
                                       (GInstanceInitFunc)(void (*)(void)) type_name##_init, \
                                       (GTypeFlags) flags); \
    { /* custom code follows */
#define _STLWRT_DEFINE_TYPE_EXTENDED_END()	\
      /* following custom code */	\
    }					\
  return g_define_type_id; \
} /* closes type_name##_get_type_once() */


#define _STLWRT_DEFINE_TYPE_EXTENDED_CLASS_INIT(TypeName, type_name) \
static void     type_name##_class_intern_init (gpointer klass) \
{ \
  type_name##_parent_class = g_type_class_peek_parent (klass); \
  if (TypeName##_private_offset != 0) \
    g_type_class_adjust_private_offset (klass, &TypeName##_private_offset); \
  type_name##_class_init ((TypeName##Class*) klass); \
}


#define _STLWRT_DEFINE_TYPE_EXTENDED_END()	\
      /* following custom code */	\
    }					\
  return g_define_type_id; \
} /* closes type_name##_get_type_once() */


#define STLWRT_DEFINE_INTERFACE_WITH_CODE(TN, t_n, T_P, _C_)     _STLWRT_DEFINE_INTERFACE_EXTENDED_BEGIN(TN, t_n, T_P) {_C_;} _STLWRT_DEFINE_INTERFACE_EXTENDED_END()


#define _STLWRT_DEFINE_INTERFACE_EXTENDED_BEGIN(TypeName, type_name, TYPE_PREREQ) \
\
static void     type_name##_default_init        (TypeName##Interface *klass); \
\
GType \
__##type_name##_get_type (void) \
{ \
  static volatile gsize g_define_type_id__volatile = 0; \
  if (g_once_init_enter (&g_define_type_id__volatile))  \
    { \
      GType g_define_type_id = \
        g_type_register_static_simple (G_TYPE_INTERFACE, \
                                       g_intern_static_string (#TypeName), \
                                       sizeof (TypeName##Interface), \
                                       (GClassInitFunc)(void (*)(void)) type_name##_default_init, \
                                       0, \
                                       (GInstanceInitFunc)NULL, \
                                       (GTypeFlags) 0); \
      if (TYPE_PREREQ != G_TYPE_INVALID) \
        g_type_interface_add_prerequisite (g_define_type_id, TYPE_PREREQ); \
      { /* custom code follows */


#define _STLWRT_DEFINE_INTERFACE_EXTENDED_END()	\
        /* following custom code */		\
      }						\
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id); \
    }						\
  return g_define_type_id__volatile;			\
} /* closes type_name##_get_type() */




/* More definitions used by STLWRT components. */

#define STLWRT_TYPE_OBJECT            (__stlwrt_object_get_type ())
#define STLWRT_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), STLWRT_TYPE_OBJECT, StlwrtObject))
#define STLWRT_OBJECT_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST ((class), STLWRT_TYPE_OBJECT, StlwrtObjectClass))
#define STLWRT_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), STLWRT_TYPE_OBJECT))
#define STLWRT_IS_OBJECT_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), STLWRT_TYPE_OBJECT))
#define STLWRT_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), STLWRT_TYPE_OBJECT, StlwrtObjectClass))

typedef struct _StlwrtObject        StlwrtObject;
typedef struct _StlwrtObjectClass   StlwrtObjectClass;
typedef struct _StlwrtObjectPrivate StlwrtObjectPrivate;

struct _StlwrtObject
{
  GInitiallyUnowned parent;
};

struct _StlwrtObjectClass
{
  GInitiallyUnownedClass parent_class;

  /*
   * The top-level type, directly or indirectly derived from StlwrtObject,
   * which is also an object that is part of the STLWRT library.  See the
   * documentation about StlwrtObject itself.
   */
  GType	top_level_stlwrt_type;
  
  gint	fat_instance_size;
  gint	skinny_instance_size;
  

  /* Padding for future expansion */
  void (*_stlwrt_reserved1) (void);
  void (*_stlwrt_reserved2) (void);
  void (*_stlwrt_reserved3) (void);
  void (*_stlwrt_reserved4) (void);
};

GType         __stlwrt_object_get_type              (void) G_GNUC_CONST;

void          __stlwrt_object_class_register_type   (StlwrtObjectClass  *klass,
						     GType high_level_type,
						     gint  fat_instance_size,
						     gint  skinny_instance_size);

StlwrtObject* __stlwrt_object_get_fat_instance      (StlwrtObjectClass  *object);

StlwrtObject* __stlwrt_object_get_skinny_instance   (StlwrtObjectClass  *object);



/* This global variable should not be set by applications; it is set by
   components of STLWRT to the major version of GTK that STLWRT is pretending
   to be at any moment.  When execution is handed off to a GTK+ 3 application,
   this variable is set to 3; when control is handed off to a GTK+ 2 module,
   this variable is set to 2, and so on.  This variable is used directly by
   StlwrtObject to determine whether to return to the calling function a new
   "fat" StlwrtObject (the kind that would be returned by GTK+ 2) or a "skinny"
   StlwrtObject (the kind returned by GTK+ 3 and later).
 */
extern int	_stlwrt_operation_mode;


G_END_DECLS

#endif  /* __STLWRT_OBJECT_H__ */
