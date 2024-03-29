/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Authors: Cody Russell <crussell@canonical.com>
 *          Alexander Larsson <alexl@redhat.com>
 */

#undef GDK_DISABLE_DEPRECATED /* We need __gdk_drawable_get_size() */

#include <gtkoffscreenwindow.h>


/**
 * SECTION:gtkoffscreenwindow
 * @short_description: A toplevel container widget used to manage offscreen
 *    rendering of child widgets.
 * @title: GtkOffscreenWindow
 *
 * #GtkOffscreenWindow is strictly intended to be used for obtaining
 * snapshots of widgets that are not part of a normal widget hierarchy.
 * It differs from __gtk_widget_get_snapshot() in that the widget you
 * want to get a snapshot of need not be displayed on the user's screen
 * as a part of a widget hierarchy.  However, since #GtkOffscreenWindow
 * is a toplevel widget you cannot obtain snapshots of a full window
 * with it since you cannot pack a toplevel widget in another toplevel.
 *
 * The idea is to take a widget and manually set the state of it,
 * add it to a #GtkOffscreenWindow and then retrieve the snapshot
 * as a #GdkPixmap or #GdkPixbuf.
 *
 * #GtkOffscreenWindow derives from #GtkWindow only as an implementation
 * detail.  Applications should not use any API specific to #GtkWindow
 * to operate on this object.  It should be treated as a #GtkBin that
 * has no parent widget.
 *
 * When contained offscreen widgets are redrawn, #GtkOffscreenWindow
 * will emit a #GtkWidget::damage-event signal.
 */

STLWRT_DEFINE_FTYPE_VPARENT (GtkOffscreenWindow, gtk_offscreen_window, GTK_TYPE_WINDOW,
                             G_TYPE_FLAG_NONE, ;);

static void
gtk_offscreen_window_size_request (GtkWidget *widget,
                                   GtkRequisition *requisition)
{
  GtkBin *bin = GTK_BIN (widget);
  gint border_width;
  gint default_width, default_height;

  border_width = __gtk_container_get_border_width (GTK_CONTAINER (widget));

  requisition->width = border_width * 2;
  requisition->height = border_width * 2;

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      GtkRequisition child_req;

      __gtk_widget_size_request (gtk_bin_get_props (bin)->child, &child_req);

      requisition->width += child_req.width;
      requisition->height += child_req.height;
    }

  __gtk_window_get_default_size (GTK_WINDOW (widget),
                               &default_width, &default_height);
  if (default_width > 0)
    requisition->width = default_width;

  if (default_height > 0)
    requisition->height = default_height;
}

static void
gtk_offscreen_window_size_allocate (GtkWidget *widget,
                                    GtkAllocation *allocation)
{
  GtkBin *bin = GTK_BIN (widget);
  gint border_width;

  gtk_widget_get_props (widget)->allocation = *allocation;

  border_width = __gtk_container_get_border_width (GTK_CONTAINER (widget));

  if (__gtk_widget_get_realized (widget))
    __gdk_window_move_resize (gtk_widget_get_props (widget)->window,
                            allocation->x,
                            allocation->y,
                            allocation->width,
                            allocation->height);

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      GtkAllocation  child_alloc;

      child_alloc.x = border_width;
      child_alloc.y = border_width;
      child_alloc.width = allocation->width - 2 * border_width;
      child_alloc.height = allocation->height - 2 * border_width;

      __gtk_widget_size_allocate (gtk_bin_get_props (bin)->child, &child_alloc);
    }

  __gtk_widget_queue_draw (widget);
}

static void
gtk_offscreen_window_realize (GtkWidget *widget)
{
  GtkBin *bin;
  GdkWindowAttr attributes;
  gint attributes_mask;

  bin = GTK_BIN (widget);

  __gtk_widget_set_realized (widget, TRUE);

  attributes.x = gtk_widget_get_props (widget)->allocation.x;
  attributes.y = gtk_widget_get_props (widget)->allocation.y;
  attributes.width = gtk_widget_get_props (widget)->allocation.width;
  attributes.height = gtk_widget_get_props (widget)->allocation.height;
  attributes.window_type = GDK_WINDOW_OFFSCREEN;
  attributes.event_mask = __gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;
  attributes.visual = __gtk_widget_get_visual (widget);
  attributes.colormap = __gtk_widget_get_colormap (widget);
  attributes.wclass = GDK_INPUT_OUTPUT;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
                                   &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, gtk_widget_get_props (widget));

  if (gtk_bin_get_props (bin)->child)
    __gtk_widget_set_parent_window (gtk_bin_get_props (bin)->child, gtk_widget_get_props (widget)->window);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);

  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);
}

static void
gtk_offscreen_window_resize (GtkWidget *widget)
{
  GtkAllocation allocation = { 0, 0 };
  GtkRequisition requisition;

  __gtk_widget_size_request (widget, &requisition);

  allocation.width  = requisition.width;
  allocation.height = requisition.height;
  __gtk_widget_size_allocate (widget, &allocation);
}

static void
move_focus (GtkWidget       *widget,
            GtkDirectionType dir)
{
  __gtk_widget_child_focus (widget, dir);

  if (!gtk_container_get_props (GTK_CONTAINER (widget))->focus_child)
    __gtk_window_set_focus (GTK_WINDOW (widget), NULL);
}

static void
gtk_offscreen_window_show (GtkWidget *widget)
{
  gboolean need_resize;
  GtkContainer *container;

  /* Enable the widget's visibility flag. */
  gtk_widget_get_props (widget)->flags |= GTK_VISIBLE;

  container = GTK_CONTAINER (widget);
  need_resize = gtk_container_get_props (container)->need_resize || !__gtk_widget_get_realized (widget);
  gtk_container_get_props (container)->need_resize = FALSE;

  if (need_resize)
    gtk_offscreen_window_resize (widget);

  __gtk_widget_map (widget);

  /* Try to make sure that we have some focused widget */
  if (!__gtk_window_get_focus (GTK_WINDOW (widget)))
    move_focus (widget, GTK_DIR_TAB_FORWARD);
}

static void
gtk_offscreen_window_hide (GtkWidget *widget)
{
  /* Disable the widget's visibility flag. */
  gtk_widget_get_props (widget)->flags &= ~GTK_VISIBLE;
  __gtk_widget_unmap (widget);
}

static void
gtk_offscreen_window_check_resize (GtkContainer *container)
{
  GtkWidget *widget = GTK_WIDGET (container);

  if (__gtk_widget_get_visible (widget))
    gtk_offscreen_window_resize (widget);
}

static void
gtk_offscreen_window_class_init (GtkOffscreenWindowClass *class)
{
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;

  widget_class = GTK_WIDGET_CLASS (class);
  container_class = GTK_CONTAINER_CLASS (class);

  widget_class->realize = gtk_offscreen_window_realize;
  widget_class->show = gtk_offscreen_window_show;
  widget_class->hide = gtk_offscreen_window_hide;
  widget_class->size_request = gtk_offscreen_window_size_request;
  widget_class->size_allocate = gtk_offscreen_window_size_allocate;

  container_class->check_resize = gtk_offscreen_window_check_resize;
}

static void
gtk_offscreen_window_init (GtkOffscreenWindow *window)
{
}

/* --- functions --- */
/**
 * __gtk_offscreen_window_new:
 *
 * Creates a toplevel container widget that is used to retrieve
 * snapshots of widgets without showing them on the screen.  For
 * widgets that are on the screen and part of a normal widget
 * hierarchy, __gtk_widget_get_snapshot() can be used instead.
 *
 * Return value: A pointer to a #GtkWidget
 *
 * Since: 2.20
 */
GtkWidget *
__gtk_offscreen_window_new (void)
{
  return g_object_new (gtk_offscreen_window_get_type (), NULL);
}

/**
 * __gtk_offscreen_window_get_pixmap:
 * @offscreen: the #GtkOffscreenWindow contained widget.
 *
 * Retrieves a snapshot of the contained widget in the form of
 * a #GdkPixmap.  If you need to keep this around over window
 * resizes then you should add a reference to it.
 *
 * Returns: (transfer none): A #GdkPixmap pointer to the offscreen pixmap,
 *     or %NULL.
 *
 * Since: 2.20
 */
GdkPixmap *
__gtk_offscreen_window_get_pixmap (GtkOffscreenWindow *offscreen)
{
  g_return_val_if_fail (GTK_IS_OFFSCREEN_WINDOW (offscreen), NULL);

  return __gdk_offscreen_window_get_pixmap (gtk_widget_get_props (GTK_WIDGET (offscreen))->window);
}

/**
 * __gtk_offscreen_window_get_pixbuf:
 * @offscreen: the #GtkOffscreenWindow contained widget.
 *
 * Retrieves a snapshot of the contained widget in the form of
 * a #GdkPixbuf.  This is a new pixbuf with a reference count of 1,
 * and the application should unreference it once it is no longer
 * needed.
 *
 * Returns: (transfer full): A #GdkPixbuf pointer, or %NULL.
 *
 * Since: 2.20
 */
GdkPixbuf *
__gtk_offscreen_window_get_pixbuf (GtkOffscreenWindow *offscreen)
{
  GdkPixmap *pixmap = NULL;
  GdkPixbuf *pixbuf = NULL;

  g_return_val_if_fail (GTK_IS_OFFSCREEN_WINDOW (offscreen), NULL);

  pixmap = __gdk_offscreen_window_get_pixmap (gtk_widget_get_props (GTK_WIDGET (offscreen))->window);

  if (pixmap != NULL)
    {
      gint width, height;

      __gdk_drawable_get_size ((GdkDrawable *) pixmap, &width, &height);

      pixbuf = __gdk_pixbuf_get_from_drawable (NULL, pixmap, NULL,
                                             0, 0, 0, 0,
                                             width, height);
    }

  return pixbuf;
}
