/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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

#ifndef __GTK_CONTAINER_H__
#define __GTK_CONTAINER_H__

#include <stlwrt.h>


#include <gtkwidget.h>

#include <gtkadjustment.h>


G_BEGIN_DECLS

#define GTK_TYPE_CONTAINER              (gtk_container_get_type ())
#define GTK_CONTAINER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CONTAINER, GtkContainer))
#define GTK_CONTAINER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CONTAINER, GtkContainerClass))
#define GTK_IS_CONTAINER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CONTAINER))
#define GTK_IS_CONTAINER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CONTAINER))
#define GTK_CONTAINER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CONTAINER, GtkContainerClass))

#define GTK_IS_RESIZE_CONTAINER(widget) (GTK_IS_CONTAINER (widget) && __gtk_container_get_resize_mode ((GtkContainer*) (widget)) != GTK_RESIZE_PARENT)

typedef struct _GtkContainerClass  GtkContainerClass;


STLWRT_DECLARE_VTYPE_VPARENT(GtkContainer, gtk_container, GtkWidget,
  GtkWidget * (focus_child);

  guint  (border_width) : 16;

  /*< private >*/
  guint  (need_resize) : 1;
  guint  (resize_mode) : 2;
  guint  (reallocate_redraws) : 1;
  guint  (has_focus_chain) : 1;
)

struct _GtkContainerClass
{
  GtkWidgetClass parent_class;

  void    (*add)       		(GtkContainer	 *container,
				 GtkWidget	 *widget);
  void    (*remove)    		(GtkContainer	 *container,
				 GtkWidget	 *widget);
  void    (*check_resize)	(GtkContainer	 *container);
  void    (*forall)    		(GtkContainer	 *container,
				 gboolean	  include_internals,
				 GtkCallback	  callback,
				 gpointer	  callback_data);
  void    (*set_focus_child)	(GtkContainer	 *container,
				 GtkWidget	 *widget);
  GType   (*child_type)		(GtkContainer	 *container);
  gchar*  (*composite_name)	(GtkContainer	 *container,
				 GtkWidget	 *child);
  void    (*set_child_property) (GtkContainer    *container,
				 GtkWidget       *child,
				 guint            property_id,
				 const GValue    *value,
				 GParamSpec      *pspec);
  void    (*get_child_property) (GtkContainer    *container,
                                 GtkWidget       *child,
				 guint            property_id,
				 GValue          *value,
				 GParamSpec      *pspec);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

/* Application-level methods */

void    SF(gtk_container_set_border_width)	 (GtkContainer	   *container,
					  guint		    border_width);
guint   SF(gtk_container_get_border_width)   (GtkContainer     *container);
void    SF(gtk_container_add)		 (GtkContainer	   *container,
					  GtkWidget	   *widget);
void    SF(gtk_container_remove)		 (GtkContainer	   *container,
					  GtkWidget	   *widget);

void    SF(gtk_container_set_resize_mode)    (GtkContainer     *container,
					  GtkResizeMode     resize_mode);
GtkResizeMode SF(gtk_container_get_resize_mode) (GtkContainer     *container);

void    SF(gtk_container_check_resize)       (GtkContainer     *container);

void     SF(gtk_container_foreach)      (GtkContainer       *container,
				     GtkCallback         callback,
				     gpointer            callback_data);

GList*   SF(gtk_container_get_children)     (GtkContainer       *container);

void     SF(gtk_container_propagate_expose) (GtkContainer   *container,
					 GtkWidget      *child,
					 GdkEventExpose *event);

void     SF(gtk_container_set_focus_chain)  (GtkContainer   *container,
                                         GList          *focusable_widgets);
gboolean SF(gtk_container_get_focus_chain)  (GtkContainer   *container,
					 GList         **focusable_widgets);
void     SF(gtk_container_unset_focus_chain) (GtkContainer  *container);

/* Widget-level methods */

void   SF(gtk_container_set_reallocate_redraws) (GtkContainer    *container,
					     gboolean         needs_redraws);
void   SF(gtk_container_set_focus_child)	   (GtkContainer     *container,
					    GtkWidget	     *child);
GtkWidget *
       SF(gtk_container_get_focus_child)	   (GtkContainer     *container);
void   SF(gtk_container_set_focus_vadjustment) (GtkContainer     *container,
					    GtkAdjustment    *adjustment);
GtkAdjustment *SF(gtk_container_get_focus_vadjustment) (GtkContainer *container);
void   SF(gtk_container_set_focus_hadjustment) (GtkContainer     *container,
					    GtkAdjustment    *adjustment);
GtkAdjustment *SF(gtk_container_get_focus_hadjustment) (GtkContainer *container);

void    SF(gtk_container_resize_children)      (GtkContainer     *container);

GType   SF(gtk_container_child_type)	   (GtkContainer     *container);


void         SF(gtk_container_class_install_child_property) (GtkContainerClass *cclass,
							 guint		    property_id,
							 GParamSpec	   *pspec);
GParamSpec*  SF(gtk_container_class_find_child_property)	(GObjectClass	   *cclass,
							 const gchar	   *property_name);
GParamSpec** SF(gtk_container_class_list_child_properties)	(GObjectClass	   *cclass,
							 guint		   *n_properties);
void         SF(gtk_container_add_with_properties)		(GtkContainer	   *container,
							 GtkWidget	   *widget,
							 const gchar	   *first_prop_name,
							 ...) G_GNUC_NULL_TERMINATED;
void         SF(gtk_container_child_set)			(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *first_prop_name,
							 ...) G_GNUC_NULL_TERMINATED;
void         SF(gtk_container_child_get)			(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *first_prop_name,
							 ...) G_GNUC_NULL_TERMINATED;
void         SF(gtk_container_child_set_valist)		(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *first_property_name,
							 va_list	    var_args);
void         SF(gtk_container_child_get_valist)		(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *first_property_name,
							 va_list	    var_args);
void	     SF(gtk_container_child_set_property)		(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *property_name,
							 const GValue	   *value);
void	     SF(gtk_container_child_get_property)		(GtkContainer	   *container,
							 GtkWidget	   *child,
							 const gchar	   *property_name,
							 GValue		   *value);

#define GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID(object, property_id, pspec) \
    G_OBJECT_WARN_INVALID_PSPEC ((object), "child property id", (property_id), (pspec))


void    SF(gtk_container_forall)		     (GtkContainer *container,
					      GtkCallback   callback,
					      gpointer	    callback_data);

/* Non-public methods */
void	SF(_gtk_container_queue_resize)	     (GtkContainer *container);
void    SF(_gtk_container_clear_resize_widgets)   (GtkContainer *container);
gchar*	SF(_gtk_container_child_composite_name)   (GtkContainer *container,
					      GtkWidget	   *child);
void   SF(_gtk_container_dequeue_resize_handler) (GtkContainer *container);
GList *SF(_gtk_container_focus_sort)             (GtkContainer     *container,
					      GList            *children,
					      GtkDirectionType  direction,
					      GtkWidget        *old_focus);

#ifndef GTK_DISABLE_DEPRECATED
#define	gtk_container_border_width		gtk_container_set_border_width
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_CONTAINER_H__ */
