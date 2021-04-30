/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1998, 2001 Tim Janik
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

#ifndef __GTK_ACCEL_GROUP_H__
#define __GTK_ACCEL_GROUP_H__

#include <stlwrt.h>



#include <gdk.h>

#include <gtkenums.h>

G_BEGIN_DECLS


/* --- type macros --- */
#define GTK_TYPE_ACCEL_GROUP              (gtk_accel_group_get_type ())
#define GTK_ACCEL_GROUP(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GTK_TYPE_ACCEL_GROUP, GtkAccelGroup))
#define GTK_ACCEL_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ACCEL_GROUP, GtkAccelGroupClass))
#define GTK_IS_ACCEL_GROUP(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GTK_TYPE_ACCEL_GROUP))
#define GTK_IS_ACCEL_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ACCEL_GROUP))
#define GTK_ACCEL_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ACCEL_GROUP, GtkAccelGroupClass))


/* --- accel flags --- */
typedef enum 
{
  GTK_ACCEL_VISIBLE        = 1 << 0,	/* display in GtkAccelLabel? */
  GTK_ACCEL_LOCKED         = 1 << 1,	/* is it removable? */
  GTK_ACCEL_MASK           = 0x07
} GtkAccelFlags;


/* --- typedefs & structures --- */
typedef struct _GtkAccelGroupClass GtkAccelGroupClass;
typedef struct _GtkAccelKey        GtkAccelKey;
typedef struct _GtkAccelGroupEntry GtkAccelGroupEntry;
typedef gboolean (*GtkAccelGroupActivate) (GtkAccelGroup  *accel_group,
					   GObject        *acceleratable,
					   guint           keyval,
					   GdkModifierType modifier);

/**
 * GtkAccelGroupFindFunc:
 * @key: 
 * @closure: 
 * @data: 
 * 
 * Since: 2.2
 */
typedef gboolean (*GtkAccelGroupFindFunc) (GtkAccelKey    *key,
					   GClosure       *closure,
					   gpointer        data);

/**
 * GtkAccelGroup:
 * 
 * An object representing and maintaining a group of accelerators.
 */

/*
 * STLWRT NOTE:  The likelihood of somebody actually subclassing GtkAccelGroup
 * is remote as far as I can tell right now, so keep this as an FType unless
 * somebody actually subclasses this.  (Who *would*, though?)
 */

STLWRT_DECLARE_FTYPE_FPARENT(GtkAccelGroup, gtk_accel_group, GObject,
  guint                (lock_count);
  GdkModifierType      (modifier_mask);
  GSList             * (acceleratables);
  guint	               (n_accels);
  GtkAccelGroupEntry * (priv_accels);
)

struct _GtkAccelGroupClass
{
  GObjectClass parent_class;

  void	(*accel_changed)	(GtkAccelGroup	*accel_group,
				 guint           keyval,
				 GdkModifierType modifier,
				 GClosure       *accel_closure);
  
  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _GtkAccelKey
{
  guint           accel_key;
  GdkModifierType accel_mods;
  guint           accel_flags : 16;
};


/* -- Accelerator Groups --- */
GtkAccelGroup* SF(gtk_accel_group_new)	      	  (void);
gboolean       SF(gtk_accel_group_get_is_locked)      (GtkAccelGroup  *accel_group);
GdkModifierType 
               SF(gtk_accel_group_get_modifier_mask)  (GtkAccelGroup  *accel_group);
void	       SF(gtk_accel_group_lock)		  (GtkAccelGroup  *accel_group);
void	       SF(gtk_accel_group_unlock)		  (GtkAccelGroup  *accel_group);
void	       SF(gtk_accel_group_connect)		  (GtkAccelGroup  *accel_group,
						   guint	   accel_key,
						   GdkModifierType accel_mods,
						   GtkAccelFlags   accel_flags,
						   GClosure	  *closure);
void           SF(gtk_accel_group_connect_by_path)    (GtkAccelGroup  *accel_group,
						   const gchar	  *accel_path,
						   GClosure	  *closure);
gboolean       SF(gtk_accel_group_disconnect)	  (GtkAccelGroup  *accel_group,
						   GClosure	  *closure);
gboolean       SF(gtk_accel_group_disconnect_key)	  (GtkAccelGroup  *accel_group,
						   guint	   accel_key,
						   GdkModifierType accel_mods);
gboolean       SF(gtk_accel_group_activate)           (GtkAccelGroup   *accel_group,
                                                   GQuark	   accel_quark,
                                                   GObject	  *acceleratable,
                                                   guint	   accel_key,
                                                   GdkModifierType accel_mods);


/* --- GtkActivatable glue --- */
void		SF(_gtk_accel_group_attach)		(GtkAccelGroup	*accel_group,
						 GObject	*object);
void		SF(_gtk_accel_group_detach)		(GtkAccelGroup	*accel_group,
						 GObject	*object);
gboolean        SF(gtk_accel_groups_activate)      	(GObject	*object,
						 guint		 accel_key,
						 GdkModifierType accel_mods);
GSList*	        SF(gtk_accel_groups_from_object)    (GObject	*object);
GtkAccelKey*	SF(gtk_accel_group_find)		(GtkAccelGroup	      *accel_group,
						 GtkAccelGroupFindFunc find_func,
						 gpointer              data);
GtkAccelGroup*	SF(gtk_accel_group_from_accel_closure) (GClosure    *closure);


/* --- Accelerators--- */
gboolean SF(gtk_accelerator_valid)		      (guint	        keyval,
					       GdkModifierType  modifiers) G_GNUC_CONST;
void	 SF(gtk_accelerator_parse)		      (const gchar     *accelerator,
					       guint	       *accelerator_key,
					       GdkModifierType *accelerator_mods);
gchar*	 SF(gtk_accelerator_name)		      (guint	        accelerator_key,
					       GdkModifierType  accelerator_mods);
gchar*   SF(gtk_accelerator_get_label)            (guint           accelerator_key,
                                               GdkModifierType accelerator_mods);
void	 SF(gtk_accelerator_set_default_mod_mask) (GdkModifierType  default_mod_mask);
guint	 SF(gtk_accelerator_get_default_mod_mask) (void);


/* --- internal --- */
GtkAccelGroupEntry*	SF(gtk_accel_group_query)	(GtkAccelGroup	*accel_group,
						 guint		 accel_key,
						 GdkModifierType accel_mods,
						 guint          *n_entries);

void		     SF(_gtk_accel_group_reconnect) (GtkAccelGroup *accel_group,
						 GQuark         accel_path_quark);

struct _GtkAccelGroupEntry
{
  GtkAccelKey  key;
  GClosure    *closure;
  GQuark       accel_path_quark;
};

G_END_DECLS


#endif /* __GTK_ACCEL_GROUP_H__ */
