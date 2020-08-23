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

#ifndef __STLWRT_H__
#define __STLWRT_H__

#include <glib.h>

G_BEGIN_DECLS

#ifndef G_TYPE_FLAG_NONE
 #define G_TYPE_FLAG_NONE 0
#endif

/**
 * STLWRT_DEFINE_FIXED_SIZE_TYPE:
 * STLWRT_DEFINE_VARIABLE_SIZE_TYPE:
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
 */
#define STLWRT_DEFINE_FIXED_SIZE_TYPE    (TN, t_n, PT, F, C) \
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


#define STLWRT_DEFINE_VARIABLE_SIZE_TYPE (TN, t_n, PT, F, C) \
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
  static inline gpointer \
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

G_END_DECLS

#endif /* __STLWRT_H__ */
