/* gtkcellrenderer.c
 * Copyright (C) 2000  Red Hat, Inc. Jonathan Blandford
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stlwrt.h>
#include <gtkcellrenderer.h>
#include <gtkintl.h>

#include <gtkprivate.h>
#include <gtktreeprivate.h>


static void gtk_cell_renderer_get_property  (GObject              *object,
					     guint                 param_id,
					     GValue               *value,
					     GParamSpec           *pspec);
static void gtk_cell_renderer_set_property  (GObject              *object,
					     guint                 param_id,
					     const GValue         *value,
					     GParamSpec           *pspec);
static void set_cell_bg_color               (GtkCellRenderer      *cell,
					     GdkColor             *color);

typedef struct _GtkCellRendererPrivate GtkCellRendererPrivate;

struct _GtkCellRendererPrivate
{
  GdkColor cell_background;
};


enum {
  PROP_0,
  PROP_MODE,
  PROP_VISIBLE,
  PROP_SENSITIVE,
  PROP_XALIGN,
  PROP_YALIGN,
  PROP_XPAD,
  PROP_YPAD,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_IS_EXPANDER,
  PROP_IS_EXPANDED,
  PROP_CELL_BACKGROUND,
  PROP_CELL_BACKGROUND_GDK,
  PROP_CELL_BACKGROUND_SET,
  PROP_EDITING
};

/* Signal IDs */
enum {
  EDITING_CANCELED,
  EDITING_STARTED,
  LAST_SIGNAL
};

static guint cell_renderer_signals[LAST_SIGNAL] = { 0 };

STLWRT_DEFINE_VTYPE (GtkCellRenderer, gtk_cell_renderer, G_TYPE_OBJECT, G_TYPE_FLAG_ABSTRACT,
                     G_ADD_PRIVATE (GtkCellRenderer))

static void
gtk_cell_renderer_init (GtkCellRenderer *cell)
{
  gtk_cell_renderer_get_props (cell)->mode = GTK_CELL_RENDERER_MODE_INERT;
  gtk_cell_renderer_get_props (cell)->visible = TRUE;
  gtk_cell_renderer_get_props (cell)->width = -1;
  gtk_cell_renderer_get_props (cell)->height = -1;
  gtk_cell_renderer_get_props (cell)->xalign = 0.5;
  gtk_cell_renderer_get_props (cell)->yalign = 0.5;
  gtk_cell_renderer_get_props (cell)->xpad = 0;
  gtk_cell_renderer_get_props (cell)->ypad = 0;
  gtk_cell_renderer_get_props (cell)->sensitive = TRUE;
  gtk_cell_renderer_get_props (cell)->is_expander = FALSE;
  gtk_cell_renderer_get_props (cell)->is_expanded = FALSE;
  gtk_cell_renderer_get_props (cell)->editing = FALSE;
}

static void
gtk_cell_renderer_class_init (GtkCellRendererClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->get_property = gtk_cell_renderer_get_property;
  object_class->set_property = gtk_cell_renderer_set_property;

  class->render = NULL;
  class->get_size = NULL;

  /**
   * GtkCellRenderer::editing-canceled:
   * @renderer: the object which received the signal
   *
   * This signal gets emitted when the user cancels the process of editing a
   * cell.  For example, an editable cell renderer could be written to cancel
   * editing when the user presses Escape. 
   *
   * See also: __gtk_cell_renderer_stop_editing().
   *
   * Since: 2.4
   */
  cell_renderer_signals[EDITING_CANCELED] =
    g_signal_new (I_("editing-canceled"),
		  G_OBJECT_CLASS_TYPE (object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GtkCellRendererClass, editing_canceled),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 0);

  /**
   * GtkCellRenderer::editing-started:
   * @renderer: the object which received the signal
   * @editable: the #GtkCellEditable
   * @path: the path identifying the edited cell
   *
   * This signal gets emitted when a cell starts to be edited.
   * The intended use of this signal is to do special setup
   * on @editable, e.g. adding a #GtkEntryCompletion or setting
   * up additional columns in a #GtkComboBox.
   *
   * Note that GTK+ doesn't guarantee that cell renderers will
   * continue to use the same kind of widget for editing in future
   * releases, therefore you should check the type of @editable
   * before doing any specific setup, as in the following example:
   * |[
   * static void
   * text_editing_started (GtkCellRenderer *cell,
   *                       GtkCellEditable *editable,
   *                       const gchar     *path,
   *                       gpointer         data)
   * {
   *   if (GTK_IS_ENTRY (editable)) 
   *     {
   *       GtkEntry *entry = GTK_ENTRY (editable);
   *       
   *       /&ast; ... create a GtkEntryCompletion &ast;/
   *       
   *       __gtk_entry_set_completion (entry, completion);
   *     }
   * }
   * ]|
   *
   * Since: 2.6
   */
  cell_renderer_signals[EDITING_STARTED] =
    g_signal_new (I_("editing-started"),
		  G_OBJECT_CLASS_TYPE (object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GtkCellRendererClass, editing_started),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 2,
		  GTK_TYPE_CELL_EDITABLE,
		  G_TYPE_STRING);

  g_object_class_install_property (object_class,
				   PROP_MODE,
				   g_param_spec_enum ("mode",
						      P_("mode"),
						      P_("Editable mode of the CellRenderer"),
						      GTK_TYPE_CELL_RENDERER_MODE,
						      GTK_CELL_RENDERER_MODE_INERT,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_VISIBLE,
				   g_param_spec_boolean ("visible",
							 P_("visible"),
							 P_("Display the cell"),
							 TRUE,
							 GTK_PARAM_READWRITE));
  g_object_class_install_property (object_class,
				   PROP_SENSITIVE,
				   g_param_spec_boolean ("sensitive",
							 P_("Sensitive"),
							 P_("Display the cell sensitive"),
							 TRUE,
							 GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_XALIGN,
				   g_param_spec_float ("xalign",
						       P_("xalign"),
						       P_("The x-align"),
						       0.0,
						       1.0,
						       0.5,
						       GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_YALIGN,
				   g_param_spec_float ("yalign",
						       P_("yalign"),
						       P_("The y-align"),
						       0.0,
						       1.0,
						       0.5,
						       GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_XPAD,
				   g_param_spec_uint ("xpad",
						      P_("xpad"),
						      P_("The xpad"),
						      0,
						      G_MAXUINT,
						      0,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_YPAD,
				   g_param_spec_uint ("ypad",
						      P_("ypad"),
						      P_("The ypad"),
						      0,
						      G_MAXUINT,
						      0,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_WIDTH,
				   g_param_spec_int ("width",
						     P_("width"),
						     P_("The fixed width"),
						     -1,
						     G_MAXINT,
						     -1,
						     GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_HEIGHT,
				   g_param_spec_int ("height",
						     P_("height"),
						     P_("The fixed height"),
						     -1,
						     G_MAXINT,
						     -1,
						     GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_IS_EXPANDER,
				   g_param_spec_boolean ("is-expander",
							 P_("Is Expander"),
							 P_("Row has children"),
							 FALSE,
							 GTK_PARAM_READWRITE));


  g_object_class_install_property (object_class,
				   PROP_IS_EXPANDED,
				   g_param_spec_boolean ("is-expanded",
							 P_("Is Expanded"),
							 P_("Row is an expander row, and is expanded"),
							 FALSE,
							 GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_CELL_BACKGROUND,
				   g_param_spec_string ("cell-background",
							P_("Cell background color name"),
							P_("Cell background color as a string"),
							NULL,
							GTK_PARAM_WRITABLE));

  g_object_class_install_property (object_class,
				   PROP_CELL_BACKGROUND_GDK,
				   g_param_spec_boxed ("cell-background-gdk",
						       P_("Cell background color"),
						       P_("Cell background color as a GdkColor"),
						       GDK_TYPE_COLOR,
						       GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_EDITING,
				   g_param_spec_boolean ("editing",
							 P_("Editing"),
							 P_("Whether the cell renderer is currently in editing mode"),
							 FALSE,
							 GTK_PARAM_READABLE));


#define ADD_SET_PROP(propname, propval, nick, blurb) g_object_class_install_property (object_class, propval, g_param_spec_boolean (propname, nick, blurb, FALSE, GTK_PARAM_READWRITE))

  ADD_SET_PROP ("cell-background-set", PROP_CELL_BACKGROUND_SET,
                P_("Cell background set"),
                P_("Whether this tag affects the cell background color"));
}

static void
gtk_cell_renderer_get_property (GObject     *object,
				guint        param_id,
				GValue      *value,
				GParamSpec  *pspec)
{
  GtkCellRenderer *cell = GTK_CELL_RENDERER (object);
  GtkCellRendererPrivate *priv = gtk_cell_renderer_get_instance_private (object);

  switch (param_id)
    {
    case PROP_MODE:
      g_value_set_enum (value, gtk_cell_renderer_get_props (cell)->mode);
      break;
    case PROP_VISIBLE:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->visible);
      break;
    case PROP_SENSITIVE:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->sensitive);
      break;
    case PROP_EDITING:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->editing);
      break;
    case PROP_XALIGN:
      g_value_set_float (value, gtk_cell_renderer_get_props (cell)->xalign);
      break;
    case PROP_YALIGN:
      g_value_set_float (value, gtk_cell_renderer_get_props (cell)->yalign);
      break;
    case PROP_XPAD:
      g_value_set_uint (value, gtk_cell_renderer_get_props (cell)->xpad);
      break;
    case PROP_YPAD:
      g_value_set_uint (value, gtk_cell_renderer_get_props (cell)->ypad);
      break;
    case PROP_WIDTH:
      g_value_set_int (value, gtk_cell_renderer_get_props (cell)->width);
      break;
    case PROP_HEIGHT:
      g_value_set_int (value, gtk_cell_renderer_get_props (cell)->height);
      break;
    case PROP_IS_EXPANDER:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->is_expander);
      break;
    case PROP_IS_EXPANDED:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->is_expanded);
      break;
    case PROP_CELL_BACKGROUND_GDK:
      {
	GdkColor color;

	color.red = priv->cell_background.red;
	color.green = priv->cell_background.green;
	color.blue = priv->cell_background.blue;

	g_value_set_boxed (value, &color);
      }
      break;
    case PROP_CELL_BACKGROUND_SET:
      g_value_set_boolean (value, gtk_cell_renderer_get_props (cell)->cell_background_set);
      break;
    case PROP_CELL_BACKGROUND:
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }

}

static void
gtk_cell_renderer_set_property (GObject      *object,
				guint         param_id,
				const GValue *value,
				GParamSpec   *pspec)
{
  GtkCellRenderer *cell = GTK_CELL_RENDERER (object);

  switch (param_id)
    {
    case PROP_MODE:
      gtk_cell_renderer_get_props (cell)->mode = g_value_get_enum (value);
      break;
    case PROP_VISIBLE:
      gtk_cell_renderer_get_props (cell)->visible = g_value_get_boolean (value);
      break;
    case PROP_SENSITIVE:
      gtk_cell_renderer_get_props (cell)->sensitive = g_value_get_boolean (value);
      break;
    case PROP_EDITING:
      gtk_cell_renderer_get_props (cell)->editing = g_value_get_boolean (value);
      break;
    case PROP_XALIGN:
      gtk_cell_renderer_get_props (cell)->xalign = g_value_get_float (value);
      break;
    case PROP_YALIGN:
      gtk_cell_renderer_get_props (cell)->yalign = g_value_get_float (value);
      break;
    case PROP_XPAD:
      gtk_cell_renderer_get_props (cell)->xpad = g_value_get_uint (value);
      break;
    case PROP_YPAD:
      gtk_cell_renderer_get_props (cell)->ypad = g_value_get_uint (value);
      break;
    case PROP_WIDTH:
      gtk_cell_renderer_get_props (cell)->width = g_value_get_int (value);
      break;
    case PROP_HEIGHT:
      gtk_cell_renderer_get_props (cell)->height = g_value_get_int (value);
      break;
    case PROP_IS_EXPANDER:
      gtk_cell_renderer_get_props (cell)->is_expander = g_value_get_boolean (value);
      break;
    case PROP_IS_EXPANDED:
      gtk_cell_renderer_get_props (cell)->is_expanded = g_value_get_boolean (value);
      break;
    case PROP_CELL_BACKGROUND:
      {
	GdkColor color;

	if (!g_value_get_string (value))
	  set_cell_bg_color (cell, NULL);
	else if (__gdk_color_parse (g_value_get_string (value), &color))
	  set_cell_bg_color (cell, &color);
	else
	  g_warning ("Don't know color `%s'", g_value_get_string (value));

	g_object_notify (object, "cell-background-gdk");
      }
      break;
    case PROP_CELL_BACKGROUND_GDK:
      set_cell_bg_color (cell, g_value_get_boxed (value));
      break;
    case PROP_CELL_BACKGROUND_SET:
      gtk_cell_renderer_get_props (cell)->cell_background_set = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }
}

static void
set_cell_bg_color (GtkCellRenderer *cell,
		   GdkColor        *color)
{
  GtkCellRendererPrivate *priv = gtk_cell_renderer_get_instance_private (cell);

  if (color)
    {
      if (!gtk_cell_renderer_get_props (cell)->cell_background_set)
        {
	  gtk_cell_renderer_get_props (cell)->cell_background_set = TRUE;
	  g_object_notify (G_OBJECT (cell), "cell-background-set");
	}

      priv->cell_background.red = color->red;
      priv->cell_background.green = color->green;
      priv->cell_background.blue = color->blue;
    }
  else
    {
      if (gtk_cell_renderer_get_props (cell)->cell_background_set)
        {
	  gtk_cell_renderer_get_props (cell)->cell_background_set = FALSE;
	  g_object_notify (G_OBJECT (cell), "cell-background-set");
	}
    }
}

/**
 * __gtk_cell_renderer_get_size:
 * @cell: a #GtkCellRenderer
 * @widget: the widget the renderer is rendering to
 * @cell_area: (allow-none): The area a cell will be allocated, or %NULL
 * @x_offset: (out) (allow-none): location to return x offset of cell relative to @cell_area, or %NULL
 * @y_offset: (out) (allow-none): location to return y offset of cell relative to @cell_area, or %NULL
 * @width: (out) (allow-none): location to return width needed to render a cell, or %NULL
 * @height: (out) (allow-none): location to return height needed to render a cell, or %NULL
 *
 * Obtains the width and height needed to render the cell. Used by view 
 * widgets to determine the appropriate size for the cell_area passed to
 * __gtk_cell_renderer_render().  If @cell_area is not %NULL, fills in the
 * x and y offsets (if set) of the cell relative to this location. 
 *
 * Please note that the values set in @width and @height, as well as those 
 * in @x_offset and @y_offset are inclusive of the xpad and ypad properties.
 **/
void
__gtk_cell_renderer_get_size (GtkCellRenderer    *cell,
			    GtkWidget          *widget,
			    const GdkRectangle *cell_area,
			    gint               *x_offset,
			    gint               *y_offset,
			    gint               *width,
			    gint               *height)
{
  gint *real_width = width;
  gint *real_height = height;

  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));
  g_return_if_fail (GTK_CELL_RENDERER_GET_CLASS (cell)->get_size != NULL);

  if (width && gtk_cell_renderer_get_props (cell)->width != -1)
    {
      real_width = NULL;
      *width = gtk_cell_renderer_get_props (cell)->width;
    }
  if (height && gtk_cell_renderer_get_props (cell)->height != -1)
    {
      real_height = NULL;
      *height = gtk_cell_renderer_get_props (cell)->height;
    }

  GTK_CELL_RENDERER_GET_CLASS (cell)->get_size (cell,
						widget,
						(GdkRectangle *) cell_area,
						x_offset,
						y_offset,
						real_width,
						real_height);
}

/**
 * __gtk_cell_renderer_render:
 * @cell: a #GtkCellRenderer
 * @window: a #GdkDrawable to draw to
 * @widget: the widget owning @window
 * @background_area: entire cell area (including tree expanders and maybe 
 *    padding on the sides)
 * @cell_area: area normally rendered by a cell renderer
 * @expose_area: area that actually needs updating
 * @flags: flags that affect rendering
 *
 * Invokes the virtual render function of the #GtkCellRenderer. The three
 * passed-in rectangles are areas of @window. Most renderers will draw within
 * @cell_area; the xalign, yalign, xpad, and ypad fields of the #GtkCellRenderer
 * should be honored with respect to @cell_area. @background_area includes the
 * blank space around the cell, and also the area containing the tree expander;
 * so the @background_area rectangles for all cells tile to cover the entire
 * @window.  @expose_area is a clip rectangle.
 **/
void
__gtk_cell_renderer_render (GtkCellRenderer      *cell,
			  GdkWindow            *window,
			  GtkWidget            *widget,
			  const GdkRectangle   *background_area,
			  const GdkRectangle   *cell_area,
			  const GdkRectangle   *expose_area,
			  GtkCellRendererState  flags)
{
  gboolean selected = FALSE;
  GtkCellRendererPrivate *priv = gtk_cell_renderer_get_instance_private (cell);

  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));
  g_return_if_fail (GTK_CELL_RENDERER_GET_CLASS (cell)->render != NULL);

  selected = (flags & GTK_CELL_RENDERER_SELECTED) == GTK_CELL_RENDERER_SELECTED;

  if (gtk_cell_renderer_get_props (cell)->cell_background_set && !selected)
    {
      cairo_t *cr = __gdk_cairo_create ((GdkDrawable *) (window));

      __gdk_cairo_rectangle (cr, background_area);
      __gdk_cairo_set_source_color (cr, &priv->cell_background);
      cairo_fill (cr);
      
      cairo_destroy (cr);
    }

  GTK_CELL_RENDERER_GET_CLASS (cell)->render (cell,
					      window,
					      widget,
					      (GdkRectangle *) background_area,
					      (GdkRectangle *) cell_area,
					      (GdkRectangle *) expose_area,
					      flags);
}

/**
 * __gtk_cell_renderer_activate:
 * @cell: a #GtkCellRenderer
 * @event: a #GdkEvent
 * @widget: widget that received the event
 * @path: widget-dependent string representation of the event location; 
 *    e.g. for #GtkTreeView, a string representation of #GtkTreePath
 * @background_area: background area as passed to __gtk_cell_renderer_render()
 * @cell_area: cell area as passed to __gtk_cell_renderer_render()
 * @flags: render flags
 *
 * Passes an activate event to the cell renderer for possible processing.  
 * Some cell renderers may use events; for example, #GtkCellRendererToggle 
 * toggles when it gets a mouse click.
 *
 * Return value: %TRUE if the event was consumed/handled
 **/
gboolean
__gtk_cell_renderer_activate (GtkCellRenderer      *cell,
			    GdkEvent             *event,
			    GtkWidget            *widget,
			    const gchar          *path,
			    const GdkRectangle   *background_area,
			    const GdkRectangle   *cell_area,
			    GtkCellRendererState  flags)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER (cell), FALSE);

  if (gtk_cell_renderer_get_props (cell)->mode != GTK_CELL_RENDERER_MODE_ACTIVATABLE)
    return FALSE;

  if (GTK_CELL_RENDERER_GET_CLASS (cell)->activate == NULL)
    return FALSE;

  return GTK_CELL_RENDERER_GET_CLASS (cell)->activate (cell,
						       event,
						       widget,
						       path,
						       (GdkRectangle *) background_area,
						       (GdkRectangle *) cell_area,
						       flags);
}

/**
 * __gtk_cell_renderer_start_editing:
 * @cell: a #GtkCellRenderer
 * @event: a #GdkEvent
 * @widget: widget that received the event
 * @path: widget-dependent string representation of the event location;
 *    e.g. for #GtkTreeView, a string representation of #GtkTreePath
 * @background_area: background area as passed to __gtk_cell_renderer_render()
 * @cell_area: cell area as passed to __gtk_cell_renderer_render()
 * @flags: render flags
 *
 * Passes an activate event to the cell renderer for possible processing.
 *
 * Return value: (transfer none): A new #GtkCellEditable, or %NULL
 **/
GtkCellEditable *
__gtk_cell_renderer_start_editing (GtkCellRenderer      *cell,
				 GdkEvent             *event,
				 GtkWidget            *widget,
				 const gchar          *path,
				 const GdkRectangle   *background_area,
				 const GdkRectangle   *cell_area,
				 GtkCellRendererState  flags)

{
  GtkCellEditable *editable;

  g_return_val_if_fail (GTK_IS_CELL_RENDERER (cell), NULL);

  if (gtk_cell_renderer_get_props (cell)->mode != GTK_CELL_RENDERER_MODE_EDITABLE)
    return NULL;

  if (GTK_CELL_RENDERER_GET_CLASS (cell)->start_editing == NULL)
    return NULL;

  editable = GTK_CELL_RENDERER_GET_CLASS (cell)->start_editing (cell,
								event,
								widget,
								path,
								(GdkRectangle *) background_area,
								(GdkRectangle *) cell_area,
								flags);

  g_signal_emit (cell, 
		 cell_renderer_signals[EDITING_STARTED], 0,
		 editable, path);

  gtk_cell_renderer_get_props (cell)->editing = TRUE;

  return editable;
}

/**
 * __gtk_cell_renderer_set_fixed_size:
 * @cell: A #GtkCellRenderer
 * @width: the width of the cell renderer, or -1
 * @height: the height of the cell renderer, or -1
 *
 * Sets the renderer size to be explicit, independent of the properties set.
 **/
void
__gtk_cell_renderer_set_fixed_size (GtkCellRenderer *cell,
				  gint             width,
				  gint             height)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));
  g_return_if_fail (width >= -1 && height >= -1);

  if ((width != gtk_cell_renderer_get_props (cell)->width) || (height != gtk_cell_renderer_get_props (cell)->height))
    {
      g_object_freeze_notify (G_OBJECT (cell));

      if (width != gtk_cell_renderer_get_props (cell)->width)
        {
          gtk_cell_renderer_get_props (cell)->width = width;
          g_object_notify (G_OBJECT (cell), "width");
        }

      if (height != gtk_cell_renderer_get_props (cell)->height)
        {
          gtk_cell_renderer_get_props (cell)->height = height;
          g_object_notify (G_OBJECT (cell), "height");
        }

      g_object_thaw_notify (G_OBJECT (cell));
    }
}

/**
 * __gtk_cell_renderer_get_fixed_size:
 * @cell: A #GtkCellRenderer
 * @width: (out) (allow-none): location to fill in with the fixed width of the cell, or %NULL
 * @height: (out) (allow-none): location to fill in with the fixed height of the cell, or %NULL
 *
 * Fills in @width and @height with the appropriate size of @cell.
 **/
void
__gtk_cell_renderer_get_fixed_size (GtkCellRenderer *cell,
				  gint            *width,
				  gint            *height)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (width)
    *width = gtk_cell_renderer_get_props (cell)->width;
  if (height)
    *height = gtk_cell_renderer_get_props (cell)->height;
}

/**
 * __gtk_cell_renderer_set_alignment:
 * @cell: A #GtkCellRenderer
 * @xalign: the x alignment of the cell renderer
 * @yalign: the y alignment of the cell renderer
 *
 * Sets the renderer's alignment within its available space.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_set_alignment (GtkCellRenderer *cell,
                                 gfloat           xalign,
                                 gfloat           yalign)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));
  g_return_if_fail (xalign >= 0.0 && xalign <= 1.0);
  g_return_if_fail (yalign >= 0.0 && yalign <= 1.0);

  if ((xalign != gtk_cell_renderer_get_props (cell)->xalign) || (yalign != gtk_cell_renderer_get_props (cell)->yalign))
    {
      g_object_freeze_notify (G_OBJECT (cell));

      if (xalign != gtk_cell_renderer_get_props (cell)->xalign)
        {
          gtk_cell_renderer_get_props (cell)->xalign = xalign;
          g_object_notify (G_OBJECT (cell), "xalign");
        }

      if (yalign != gtk_cell_renderer_get_props (cell)->yalign)
        {
          gtk_cell_renderer_get_props (cell)->yalign = yalign;
          g_object_notify (G_OBJECT (cell), "yalign");
        }

      g_object_thaw_notify (G_OBJECT (cell));
    }
}

/**
 * __gtk_cell_renderer_get_alignment:
 * @cell: A #GtkCellRenderer
 * @xalign: (out) (allow-none): location to fill in with the x alignment of the cell, or %NULL
 * @yalign: (out) (allow-none): location to fill in with the y alignment of the cell, or %NULL
 *
 * Fills in @xalign and @yalign with the appropriate values of @cell.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_get_alignment (GtkCellRenderer *cell,
                                 gfloat          *xalign,
                                 gfloat          *yalign)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (xalign)
    *xalign = gtk_cell_renderer_get_props (cell)->xalign;
  if (yalign)
    *yalign = gtk_cell_renderer_get_props (cell)->yalign;
}

/**
 * __gtk_cell_renderer_set_padding:
 * @cell: A #GtkCellRenderer
 * @xpad: the x padding of the cell renderer
 * @ypad: the y padding of the cell renderer
 *
 * Sets the renderer's padding.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_set_padding (GtkCellRenderer *cell,
                               gint             xpad,
                               gint             ypad)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));
  g_return_if_fail (xpad >= 0 && xpad >= 0);

  if ((xpad != gtk_cell_renderer_get_props (cell)->xpad) || (ypad != gtk_cell_renderer_get_props (cell)->ypad))
    {
      g_object_freeze_notify (G_OBJECT (cell));

      if (xpad != gtk_cell_renderer_get_props (cell)->xpad)
        {
          gtk_cell_renderer_get_props (cell)->xpad = xpad;
          g_object_notify (G_OBJECT (cell), "xpad");
        }

      if (ypad != gtk_cell_renderer_get_props (cell)->ypad)
        {
          gtk_cell_renderer_get_props (cell)->ypad = ypad;
          g_object_notify (G_OBJECT (cell), "ypad");
        }

      g_object_thaw_notify (G_OBJECT (cell));
    }
}

/**
 * __gtk_cell_renderer_get_padding:
 * @cell: A #GtkCellRenderer
 * @xpad: (out) (allow-none): location to fill in with the x padding of the cell, or %NULL
 * @ypad: (out) (allow-none): location to fill in with the y padding of the cell, or %NULL
 *
 * Fills in @xpad and @ypad with the appropriate values of @cell.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_get_padding (GtkCellRenderer *cell,
                               gint            *xpad,
                               gint            *ypad)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (xpad)
    *xpad = gtk_cell_renderer_get_props (cell)->xpad;
  if (ypad)
    *ypad = gtk_cell_renderer_get_props (cell)->ypad;
}

/**
 * __gtk_cell_renderer_set_visible:
 * @cell: A #GtkCellRenderer
 * @visible: the visibility of the cell
 *
 * Sets the cell renderer's visibility.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_set_visible (GtkCellRenderer *cell,
                               gboolean         visible)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (gtk_cell_renderer_get_props (cell)->visible != visible)
    {
      gtk_cell_renderer_get_props (cell)->visible = visible ? TRUE : FALSE;
      g_object_notify (G_OBJECT (cell), "visible");
    }
}

/**
 * __gtk_cell_renderer_get_visible:
 * @cell: A #GtkCellRenderer
 *
 * Returns the cell renderer's visibility.
 *
 * Returns: %TRUE if the cell renderer is visible
 *
 * Since: 2.18
 */
gboolean
__gtk_cell_renderer_get_visible (GtkCellRenderer *cell)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER (cell), FALSE);

  return gtk_cell_renderer_get_props (cell)->visible;
}

/**
 * __gtk_cell_renderer_set_sensitive:
 * @cell: A #GtkCellRenderer
 * @sensitive: the sensitivity of the cell
 *
 * Sets the cell renderer's sensitivity.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_set_sensitive (GtkCellRenderer *cell,
                                 gboolean         sensitive)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (gtk_cell_renderer_get_props (cell)->sensitive != sensitive)
    {
      gtk_cell_renderer_get_props (cell)->sensitive = sensitive ? TRUE : FALSE;
      g_object_notify (G_OBJECT (cell), "sensitive");
    }
}

/**
 * __gtk_cell_renderer_get_sensitive:
 * @cell: A #GtkCellRenderer
 *
 * Returns the cell renderer's sensitivity.
 *
 * Returns: %TRUE if the cell renderer is sensitive
 *
 * Since: 2.18
 */
gboolean
__gtk_cell_renderer_get_sensitive (GtkCellRenderer *cell)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER (cell), FALSE);

  return gtk_cell_renderer_get_props (cell)->sensitive;
}

/**
 * __gtk_cell_renderer_editing_canceled:
 * @cell: A #GtkCellRenderer
 * 
 * Causes the cell renderer to emit the #GtkCellRenderer::editing-canceled 
 * signal.  
 *
 * This function is for use only by implementations of cell renderers that 
 * need to notify the client program that an editing process was canceled 
 * and the changes were not committed.
 *
 * Since: 2.4
 * Deprecated: 2.6: Use __gtk_cell_renderer_stop_editing() instead
 **/
void
__gtk_cell_renderer_editing_canceled (GtkCellRenderer *cell)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  __gtk_cell_renderer_stop_editing (cell, TRUE);
}

/**
 * __gtk_cell_renderer_stop_editing:
 * @cell: A #GtkCellRenderer
 * @canceled: %TRUE if the editing has been canceled
 * 
 * Informs the cell renderer that the editing is stopped.
 * If @canceled is %TRUE, the cell renderer will emit the 
 * #GtkCellRenderer::editing-canceled signal. 
 *
 * This function should be called by cell renderer implementations 
 * in response to the #GtkCellEditable::editing-done signal of 
 * #GtkCellEditable.
 *
 * Since: 2.6
 **/
void
__gtk_cell_renderer_stop_editing (GtkCellRenderer *cell,
				gboolean         canceled)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  if (gtk_cell_renderer_get_props (cell)->editing)
    {
      gtk_cell_renderer_get_props (cell)->editing = FALSE;
      if (canceled)
	g_signal_emit (cell, cell_renderer_signals[EDITING_CANCELED], 0);
    }
}
