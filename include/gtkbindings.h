/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GtkBindingSet: Keybinding manager for GObjects.
 * Copyright (C) 1998 Tim Janik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GTK_BINDINGS_H__
#define __GTK_BINDINGS_H__

#include <stlwrt.h>



#include <gdk.h>


G_BEGIN_DECLS


/* Binding sets
 */

typedef struct _GtkBindingSet		GtkBindingSet;
typedef struct _GtkBindingEntry		GtkBindingEntry;
typedef struct _GtkBindingSignal	GtkBindingSignal;
typedef struct _GtkBindingArg		GtkBindingArg;

struct _GtkBindingSet
{
  gchar			*set_name;
  gint			 priority;
  GSList		*widget_path_pspecs;
  GSList		*widget_class_pspecs;
  GSList		*class_branch_pspecs;
  GtkBindingEntry	*entries;
  GtkBindingEntry	*current;
  guint                  parsed : 1; /* From RC content */
};

struct _GtkBindingEntry
{
  /* key portion
   */
  guint			 keyval;
  GdkModifierType	 modifiers;
  
  GtkBindingSet		*binding_set;
  guint			destroyed : 1;
  guint			in_emission : 1;
  guint                 marks_unbound : 1;
  GtkBindingEntry	*set_next;
  GtkBindingEntry	*hash_next;
  GtkBindingSignal	*signals;
};

struct _GtkBindingArg
{
  GType		 arg_type;
  union {
    glong	 long_data;
    gdouble	 double_data;
    gchar	*string_data;
  } d;
};

struct _GtkBindingSignal
{
  GtkBindingSignal	*next;
  gchar 		*signal_name;
  guint			 n_args;
  GtkBindingArg		*args;
};

/* Application-level methods */

GtkBindingSet*	SF(gtk_binding_set_new)	(const gchar	*set_name);
GtkBindingSet*	SF(gtk_binding_set_by_class)(gpointer	 object_class);
GtkBindingSet*	SF(gtk_binding_set_find)	(const gchar	*set_name);
gboolean SF(gtk_bindings_activate)		(GObject	*object,
					 guint		 keyval,
					 GdkModifierType modifiers);
gboolean SF(gtk_bindings_activate_event)    (GObject      *object,
					 GdkEventKey    *event);
gboolean SF(gtk_binding_set_activate)	(GtkBindingSet	*binding_set,
					 guint		 keyval,
					 GdkModifierType modifiers,
					 GObject	*object);

#ifndef GTK_DISABLE_DEPRECATED
#define	 gtk_binding_entry_add		gtk_binding_entry_clear
void	 SF(gtk_binding_entry_clear)	(GtkBindingSet	*binding_set,
					 guint		 keyval,
					 GdkModifierType modifiers);
guint	 SF(gtk_binding_parse_binding)      (GScanner       *scanner);
#endif /* GTK_DISABLE_DEPRECATED */

void	 SF(gtk_binding_entry_skip)         (GtkBindingSet  *binding_set,
                                         guint           keyval,
                                         GdkModifierType modifiers);
void	 SF(gtk_binding_entry_add_signal)   (GtkBindingSet  *binding_set,
                                         guint           keyval,
                                         GdkModifierType modifiers,
                                         const gchar    *signal_name,
                                         guint           n_args,
                                         ...);
void	 SF(gtk_binding_entry_add_signall)	(GtkBindingSet	*binding_set,
					 guint		 keyval,
					 GdkModifierType modifiers,
					 const gchar	*signal_name,
					 GSList		*binding_args);
void	 SF(gtk_binding_entry_remove)	(GtkBindingSet	*binding_set,
					 guint		 keyval,
					 GdkModifierType modifiers);

void	 SF(gtk_binding_set_add_path)	(GtkBindingSet	*binding_set,
					 GtkPathType	 path_type,
					 const gchar	*path_pattern,
					 GtkPathPriorityType priority);


/* Non-public methods */

guint	 SF(_gtk_binding_parse_binding)     (GScanner       *scanner);
void     SF(_gtk_binding_reset_parsed)      (void);
void	 SF(_gtk_binding_entry_add_signall) (GtkBindingSet  *binding_set,
					 guint		 keyval,
					 GdkModifierType modifiers,
					 const gchar	*signal_name,
					 GSList		*binding_args);

G_END_DECLS

#endif /* __GTK_BINDINGS_H__ */
