
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
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <stlwrtobject.h>



int	_stlwrt_operation_mode = 2;



/* The following are some things originally defined in GLib, but need to be
   duplicated here because we're doing something totally unorthodox in STLWRT;
   in other words, STLWRT is different enough from GTK that unlike GTK, STLWRT
   needs to re-implement some functions and re-define some structures from GLib.
 */

#define	TYPE_ID_MASK		((GType) ((1 << G_TYPE_FUNDAMENTAL_SHIFT) - 1))

struct _TypeNode
{
  guint volatile ref_count;
#ifdef G_ENABLE_DEBUG
  guint volatile instance_count;
#endif
  GTypePlugin *plugin;
  guint        n_children; /* writable with lock */
  guint        n_supers : 8;
  guint        n_prerequisites : 9;
  guint        is_classed : 1;
  guint        is_instantiatable : 1;
  guint        mutatable_check_cache : 1;	/* combines some common path checks */
  GType       *children; /* writable with lock */
  struct _InstanceData * volatile instance_data;
  
  /* The rest of the structure contents we don't care about. */
};

struct _InstanceData
{
  struct _CommonData common;
  guint16            class_size;
  guint16            class_private_size;
  int volatile       init_state; /* atomic - g_type_class_ref reads it unlocked */
  GBaseInitFunc      class_init_base;
  GBaseFinalizeFunc  class_finalize_base;
  GClassInitFunc     class_init;
  GClassFinalizeFunc class_finalize;
  gconstpointer      class_data;
  gpointer           class;
  guint16            instance_size;
  guint16            private_size;
  guint16            n_preallocs;
  GInstanceInitFunc  instance_init;
};

struct _CommonData
{
  GTypeValueTable  *value_table;
};


static inline TypeNode*
lookup_type_node (GType utype)
{
  /* If StlwrtObject is involved at all, then that means obviously we're not
     dealing with a fundamental type, so we don't need the code for the special
     case of a fundamental type. */
  return (TypeNode*) (utype & ~TYPE_ID_MASK);
}

/* The following is stuff related to StlwrtObject and its type definition. */


#define STLWRT_OBJECT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), STLWRT_TYPE_OBJECT, StlwrtObjectPrivate))

struct _StlwrtObjectPrivate
{
  StlwrtObject*	fat_instance;
  
  StlwrtObject*	skinny_instance;
};

STLWRT_DEFINE_ABSTRACT_TYPE_WITH_CODE (StlwrtObject, stlwrt_object,
				       G_TYPE_INITIALLY_UNOWNED,
				       G_ADD_PRIVATE (StlwrtObject))


static GObject * stlwrt_object_constructor  (GType type,
                                             guint n_constructor_properties,
                                             GObjectConstructParam *construct_params);

static void stlwrt_object_finalize     (GObject *object);



static void
stlwrt_object_class_init (StlwrtObjectClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->constructor  = stlwrt_object_constructor;
  gobject_class->finalize     = stlwrt_object_finalize;

  stlwrt_object_class_register_type (klass, STLWRT_TYPE_OBJECT,
                                     sizeof (StlwrtObject),
                                     sizeof (StlwrtObject));
}


/**
   The following function contains code from GLib, specifically the following
   functions from GLib:
   
    - g_object_constructor()   (from gobject/gobject.c)
    - g_type_create_instance() (from gobject/gtype.c)
   
   However, this function significantly deviates from the original function from
   GLib, so I shall discuss the topic in depth here.
   
   First of all, understand that one of STLWRT's goals is to impose the same
   makeover / look-and-feel over all STLWRT applications.  Another goal of
   STLWRT is to ensure that (eventually) all GTK applications run properly under
   STLWRT, no matter what version of GTK they target; so a GTK+ 3 application
   can use STLWRT just as easily as a GTK+ 2 application can use STLWRT, all
   without requiring those applications to be recompiled.
   
   That said, the ideal is not easy to achieve.  Despite GTK+ 3 being somewhat
   API compatible with GTK+ 2, they are not at all ABI compatible.  What that
   means is you might be able to compile a GTK+ 2 application on GTK+ 3, and
   under rare circumstances vice versa; but you almost never can compile a GTK+
   3 application and then run the compiled version on GTK+ 2.  Many factors
   contribute to this incompatibility, but one of the leading factors (that
   concerns STLWRT, at least) is the issue of objects.  Details follow.
   
   In all versions of GTK, "objects" are little more than glorified C
   structures.  Like normal C structures, these objects may (and do) contain
   elements.  Some of these elements may be referenced or even modified by
   applications and GTK modules, at least in GTK versions 2 and prior.  For
   example, it used to be commonplace to write programs that would add a custom
   widget (onscreen element) to part of a GTK dialog box widget, using code like
   this:
   
      // Assume that "dlg" is a pointer to an instance of GtkDialog, a dialog
      // box widget.  Also assume that "btn" is a pointer to an instance of
      // GtkButton, a clickable button widget, which is the widget we want to
      // add to the dialog.
      gtk_container_add ( GTK_CONTAINER(dlg->vbox), btn);
   
   In this example, "vbox" is a member of the structure associated with the
   GtkDialog widget which is a container to hold other widgets to be placed
   within the boundaries of the dialog.  One side effect of having extra members
   of these structures such as "vbox" is that the structures are physically
   large, with some complicated widgets (such as GtkDialog) using 160 bytes to
   store the structure.  Though 160 bytes is a drop in the bucket memory-wise
   today, the size issue is an important point which matters in the context of
   an all-encompassing toolkit such as STLWRT, as you will see.
   
   For one reason or another (the reasoning for the decision is unclear), in
   GTK+ 3 all of these so-called "public members" were removed.  This shrank
   the size of the structures considerably; the same GtkDialog instance
   described above would occupy a mere 32 bytes under GTK+ 3.
   
   So why does this all matter?  Well, 
**/
static GObject*
stlwrt_object_constructor (GType                  type,
		           guint                  n_construct_properties,
		           GObjectConstructParam *construct_params)
{
  StlwrtObjectClass	*stlwrt_object_class;
  GType			 top_level_stlwrt_type;
  
  struct _TypeNode	*original_node;
  struct _TypeNode	*stlwrt_node;
  
  gint			 original_private_size;
  gint			 original_instance_size;
  gint			 stlwrt_private_size;
  gint			 stlwrt_instance_size;
  
  void			*original_allocated_block;
  void			*stlwrt_allocated_block;
  
  GTypeInstance		*original_instance;
  GTypeInstance		*stlwrt_instance;


  /* Find out more about the object we're supposed to instantiate. */
  original_node = lookup_type_node (type);
  
  
  if (original_node == NULL || ! original_node->is_instantiatable)
  {
    g_error ("Cannot create new instance of invalid (non-instantiatable) type '%s'",
             NODE_NAME (original_node));
  }
  /* G_TYPE_IS_ABSTRACT() is an external call: _U */
  if (! original_node->mutatable_check_cache && G_TYPE_IS_ABSTRACT (type))
  {
    g_error ("Cannot create instance of abstract (non-instantiatable) type '%s'",
             NODE_NAME (original_node));
  }
  
  
  stlwrt_object_class = STLWRT_OBJECT_CLASS (g_type_class_ref (STLWRT_TYPE_OBJECT));
  
  
  top_level_stlwrt_type = stlwrt_object_class->top_level_stlwrt_type;
  
  
  stlwrt_node = lookup_type_node (top_level_stlwrt_type);
  
  
  original_private_size = original_node->instance_data->private_size;
  original_instance_size = original_node->instance_data->instance_size;
  
  stlwrt_private_size = stlwrt_node->instance_data->private_size;
  stlwrt_instance_size = stlwrt_node->instance_data->instance_size;
  
  
  original_allocated_block = g_slice_alloc0 (private_size + ivar_size);
  
  
  
  /* Now that we've instantiated both versions of the object, set the above
     properties on both new objects. */
  if (n_construct_properties)
  {
    /* set construct properties */
    while (n_construct_properties--)
    {
      GValue *value = construct_params->value;
      GParamSpec *pspec = construct_params->pspec;

      construct_params++;
      g_object_set_property (object, construct_params->pspec->name,
                             construct_params->value);
    }
  }

  return object;
}


void	__stlwrt_object_class_register_type (StlwrtObjectClass *klass,
					     GType              high_level_type,
					     gint		fat_instance_size,
					     gint		skinny_instance_size)
{
	
	klass->top_level_stlwrt_type = high_level_type;
	
	klass->fat_instance_size = fat_instance_size;
	klass->skinny_instance_size = skinny_instance_size;
	
}


StlwrtObject *	__stlwrt_object_get_fat_instance (StlwrtObject *object)
{
  StlwrtObjectPrivate *private;
  g_return_val_if_fail (STLWRT_IS_OBJECT(object), NULL);
  
  private = G_TYPE_INSTANCE_GET_PRIVATE (object, STLWRT_TYPE_OBJECT, StlwrtObject);
  
  g_return_val_if_fail (private != NULL, NULL);
  
  return private->fat_instance;
}


StlwrtObject *	__stlwrt_object_get_skinny_instance (StlwrtObject *object)
{
  StlwrtObjectPrivate *private;
  g_return_val_if_fail (STLWRT_IS_OBJECT(object), NULL);
  
  private = G_TYPE_INSTANCE_GET_PRIVATE (object, STLWRT_TYPE_OBJECT, StlwrtObject);
  
  g_return_val_if_fail (private != NULL, NULL);
  
  return private->skinny_instance;
}

#define __STLWRT_OBJECT_C__
