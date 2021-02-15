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



#include "config.h"

#include <stlwrt.h>
#include <gtktable.h>
#include <gtkprivate.h>
#include <gtkintl.h>


enum
{
  PROP_0,
  PROP_N_ROWS,
  PROP_N_COLUMNS,
  PROP_COLUMN_SPACING,
  PROP_ROW_SPACING,
  PROP_HOMOGENEOUS
};

enum
{
  CHILD_PROP_0,
  CHILD_PROP_LEFT_ATTACH,
  CHILD_PROP_RIGHT_ATTACH,
  CHILD_PROP_TOP_ATTACH,
  CHILD_PROP_BOTTOM_ATTACH,
  CHILD_PROP_X_OPTIONS,
  CHILD_PROP_Y_OPTIONS,
  CHILD_PROP_X_PADDING,
  CHILD_PROP_Y_PADDING
};
  

static void gtk_table_finalize	    (GObject	    *object);
static void gtk_table_size_request  (GtkWidget	    *widget,
				     GtkRequisition *requisition);
static void gtk_table_size_allocate (GtkWidget	    *widget,
				     GtkAllocation  *allocation);
static void gtk_table_add	    (GtkContainer   *container,
				     GtkWidget	    *widget);
static void gtk_table_remove	    (GtkContainer   *container,
				     GtkWidget	    *widget);
static void gtk_table_forall	    (GtkContainer   *container,
				     gboolean	     include_internals,
				     GtkCallback     callback,
				     gpointer	     callback_data);
static void gtk_table_get_property  (GObject         *object,
				     guint            prop_id,
				     GValue          *value,
				     GParamSpec      *pspec);
static void gtk_table_set_property  (GObject         *object,
				     guint            prop_id,
				     const GValue    *value,
				     GParamSpec      *pspec);
static void gtk_table_set_child_property (GtkContainer    *container,
					  GtkWidget       *child,
					  guint            property_id,
					  const GValue    *value,
					  GParamSpec      *pspec);
static void gtk_table_get_child_property (GtkContainer    *container,
					  GtkWidget       *child,
					  guint            property_id,
					  GValue          *value,
					  GParamSpec      *pspec);
static GType gtk_table_child_type   (GtkContainer   *container);


static void gtk_table_size_request_init	 (GtkTable *table);
static void gtk_table_size_request_pass1 (GtkTable *table);
static void gtk_table_size_request_pass2 (GtkTable *table);
static void gtk_table_size_request_pass3 (GtkTable *table);

static void gtk_table_size_allocate_init  (GtkTable *table);
static void gtk_table_size_allocate_pass1 (GtkTable *table);
static void gtk_table_size_allocate_pass2 (GtkTable *table);


STLWRT_DEFINE_VTYPE (GtkTable, gtk_table, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE, ;)

static void
gtk_table_class_init (GtkTableClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);
  GtkContainerClass *container_class = GTK_CONTAINER_CLASS (class);
  
  gobject_class->finalize = gtk_table_finalize;

  gobject_class->get_property = gtk_table_get_property;
  gobject_class->set_property = gtk_table_set_property;
  
  widget_class->size_request = gtk_table_size_request;
  widget_class->size_allocate = gtk_table_size_allocate;
  
  container_class->add = gtk_table_add;
  container_class->remove = gtk_table_remove;
  container_class->forall = gtk_table_forall;
  container_class->child_type = gtk_table_child_type;
  container_class->set_child_property = gtk_table_set_child_property;
  container_class->get_child_property = gtk_table_get_child_property;
  

  g_object_class_install_property (gobject_class,
                                   PROP_N_ROWS,
                                   g_param_spec_uint ("n-rows",
						     P_("Rows"),
						     P_("The number of rows in the table"),
						     1,
						     65535,
						     1,
						     GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_N_COLUMNS,
                                   g_param_spec_uint ("n-columns",
						     P_("Columns"),
						     P_("The number of columns in the table"),
						     1,
						     65535,
						     1,
						     GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_ROW_SPACING,
                                   g_param_spec_uint ("row-spacing",
						     P_("Row spacing"),
						     P_("The amount of space between two consecutive rows"),
						     0,
						     65535,
						     0,
						     GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_COLUMN_SPACING,
                                   g_param_spec_uint ("column-spacing",
						     P_("Column spacing"),
						     P_("The amount of space between two consecutive columns"),
						     0,
						     65535,
						     0,
						     GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_HOMOGENEOUS,
                                   g_param_spec_boolean ("homogeneous",
							 P_("Homogeneous"),
							 P_("If TRUE, the table cells are all the same width/height"),
							 FALSE,
							 GTK_PARAM_READWRITE));

  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_LEFT_ATTACH,
					      g_param_spec_uint ("left-attach", 
								 P_("Left attachment"), 
								 P_("The column number to attach the left side of the child to"),
								 0, 65535, 0,
								 GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_RIGHT_ATTACH,
					      g_param_spec_uint ("right-attach", 
								 P_("Right attachment"), 
								 P_("The column number to attach the right side of a child widget to"),
								 1, 65535, 1,
								 GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_TOP_ATTACH,
					      g_param_spec_uint ("top-attach", 
								 P_("Top attachment"), 
								 P_("The row number to attach the top of a child widget to"),
								 0, 65535, 0,
								 GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_BOTTOM_ATTACH,
					      g_param_spec_uint ("bottom-attach",
								 P_("Bottom attachment"), 
								 P_("The row number to attach the bottom of the child to"),
								 1, 65535, 1,
								 GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_X_OPTIONS,
					      g_param_spec_flags ("x-options", 
								  P_("Horizontal options"), 
								  P_("Options specifying the horizontal behaviour of the child"),
								  GTK_TYPE_ATTACH_OPTIONS, GTK_EXPAND | GTK_FILL,
								  GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_Y_OPTIONS,
					      g_param_spec_flags ("y-options", 
								  P_("Vertical options"), 
								  P_("Options specifying the vertical behaviour of the child"),
								  GTK_TYPE_ATTACH_OPTIONS, GTK_EXPAND | GTK_FILL,
								  GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_X_PADDING,
					      g_param_spec_uint ("x-padding", 
								 P_("Horizontal padding"), 
								 P_("Extra space to put between the child and its left and right neighbors, in pixels"),
								 0, 65535, 0,
								 GTK_PARAM_READWRITE));
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_Y_PADDING,
					      g_param_spec_uint ("y-padding", 
								 P_("Vertical padding"), 
								 P_("Extra space to put between the child and its upper and lower neighbors, in pixels"),
								 0, 65535, 0,
								 GTK_PARAM_READWRITE));
}

static GType
gtk_table_child_type (GtkContainer   *container)
{
  return GTK_TYPE_WIDGET;
}

static void
gtk_table_get_property (GObject      *object,
			guint         prop_id,
			GValue       *value,
			GParamSpec   *pspec)
{
  GtkTable *table;

  table = GTK_TABLE (object);

  switch (prop_id)
    {
    case PROP_N_ROWS:
      g_value_set_uint (value, gtk_table_get_props (table)->nrows);
      break;
    case PROP_N_COLUMNS:
      g_value_set_uint (value, gtk_table_get_props (table)->ncols);
      break;
    case PROP_ROW_SPACING:
      g_value_set_uint (value, gtk_table_get_props (table)->row_spacing);
      break;
    case PROP_COLUMN_SPACING:
      g_value_set_uint (value, gtk_table_get_props (table)->column_spacing);
      break;
    case PROP_HOMOGENEOUS:
      g_value_set_boolean (value, gtk_table_get_props (table)->homogeneous);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_table_set_property (GObject      *object,
			guint         prop_id,
			const GValue *value,
			GParamSpec   *pspec)
{
  GtkTable *table;

  table = GTK_TABLE (object);

  switch (prop_id)
    {
    case PROP_N_ROWS:
      __gtk_table_resize (gtk_table_get_props (table), g_value_get_uint (value), gtk_table_get_props (table)->ncols);
      break;
    case PROP_N_COLUMNS:
      __gtk_table_resize (gtk_table_get_props (table), gtk_table_get_props (table)->nrows, g_value_get_uint (value));
      break;
    case PROP_ROW_SPACING:
      __gtk_table_set_row_spacings (table, g_value_get_uint (value));
      break;
    case PROP_COLUMN_SPACING:
      __gtk_table_set_col_spacings (table, g_value_get_uint (value));
      break;
    case PROP_HOMOGENEOUS:
      __gtk_table_set_homogeneous (table, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_table_set_child_property (GtkContainer    *container,
			      GtkWidget       *child,
			      guint            property_id,
			      const GValue    *value,
			      GParamSpec      *pspec)
{
  GtkTable *table = GTK_TABLE (container);
  GtkTableChild *table_child;
  GList *list;

  table_child = NULL;
  for (list = table->children; list; list = list->next)
    {
      table_child = list->data;

      if (table_child->widget == child)
	break;
    }
  if (!list)
    {
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      return;
    }

  switch (property_id)
    {
    case CHILD_PROP_LEFT_ATTACH:
      table_child->left_attach = g_value_get_uint (value);
      if (table_child->right_attach <= table_child->left_attach)
	table_child->right_attach = table_child->left_attach + 1;
      if (table_child->right_attach >= table->ncols)
	__gtk_table_resize (table, table->nrows, table_child->right_attach);
      break;
    case CHILD_PROP_RIGHT_ATTACH:
      table_child->right_attach = g_value_get_uint (value);
      if (table_child->right_attach <= table_child->left_attach)
	table_child->left_attach = table_child->right_attach - 1;
      if (table_child->right_attach >= table->ncols)
	__gtk_table_resize (table, table->nrows, table_child->right_attach);
      break;
    case CHILD_PROP_TOP_ATTACH:
      table_child->top_attach = g_value_get_uint (value);
      if (table_child->bottom_attach <= table_child->top_attach)
	table_child->bottom_attach = table_child->top_attach + 1;
      if (table_child->bottom_attach >= table->nrows)
	__gtk_table_resize (table, table_child->bottom_attach, table->ncols);
      break;
    case CHILD_PROP_BOTTOM_ATTACH:
      table_child->bottom_attach = g_value_get_uint (value);
      if (table_child->bottom_attach <= table_child->top_attach)
	table_child->top_attach = table_child->bottom_attach - 1;
      if (table_child->bottom_attach >= table->nrows)
	__gtk_table_resize (table, table_child->bottom_attach, table->ncols);
      break;
    case CHILD_PROP_X_OPTIONS:
      table_child->xexpand = (g_value_get_flags (value) & GTK_EXPAND) != 0;
      table_child->xshrink = (g_value_get_flags (value) & GTK_SHRINK) != 0;
      table_child->xfill = (g_value_get_flags (value) & GTK_FILL) != 0;
      break;
    case CHILD_PROP_Y_OPTIONS:
      table_child->yexpand = (g_value_get_flags (value) & GTK_EXPAND) != 0;
      table_child->yshrink = (g_value_get_flags (value) & GTK_SHRINK) != 0;
      table_child->yfill = (g_value_get_flags (value) & GTK_FILL) != 0;
      break;
    case CHILD_PROP_X_PADDING:
      table_child->xpadding = g_value_get_uint (value);
      break;
    case CHILD_PROP_Y_PADDING:
      table_child->ypadding = g_value_get_uint (value);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
  if (__gtk_widget_get_visible (child) &&
      __gtk_widget_get_visible (GTK_WIDGET (table)))
    __gtk_widget_queue_resize (child);
}

static void
gtk_table_get_child_property (GtkContainer    *container,
			      GtkWidget       *child,
			      guint            property_id,
			      GValue          *value,
			      GParamSpec      *pspec)
{
  GtkTable *table = GTK_TABLE (container);
  GtkTableChild *table_child;
  GList *list;

  table_child = NULL;
  for (list = table->children; list; list = list->next)
    {
      table_child = list->data;

      if (table_child->widget == child)
	break;
    }
  if (!list)
    {
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      return;
    }

  switch (property_id)
    {
    case CHILD_PROP_LEFT_ATTACH:
      g_value_set_uint (value, table_child->left_attach);
      break;
    case CHILD_PROP_RIGHT_ATTACH:
      g_value_set_uint (value, table_child->right_attach);
      break;
    case CHILD_PROP_TOP_ATTACH:
      g_value_set_uint (value, table_child->top_attach);
      break;
    case CHILD_PROP_BOTTOM_ATTACH:
      g_value_set_uint (value, table_child->bottom_attach);
      break;
    case CHILD_PROP_X_OPTIONS:
      g_value_set_flags (value, (table_child->xexpand * GTK_EXPAND |
				 table_child->xshrink * GTK_SHRINK |
				 table_child->xfill * GTK_FILL));
      break;
    case CHILD_PROP_Y_OPTIONS:
      g_value_set_flags (value, (table_child->yexpand * GTK_EXPAND |
				 table_child->yshrink * GTK_SHRINK |
				 table_child->yfill * GTK_FILL));
      break;
    case CHILD_PROP_X_PADDING:
      g_value_set_uint (value, table_child->xpadding);
      break;
    case CHILD_PROP_Y_PADDING:
      g_value_set_uint (value, table_child->ypadding);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_table_init (GtkTable *table)
{
  __gtk_widget_set_has_window (GTK_WIDGET (table), FALSE);
  __gtk_widget_set_redraw_on_allocate (GTK_WIDGET (table), FALSE);
  
  gtk_table_get_props (table)->children = NULL;
  gtk_table_get_props (table)->rows = NULL;
  gtk_table_get_props (table)->cols = NULL;
  gtk_table_get_props (table)->nrows = 0;
  gtk_table_get_props (table)->ncols = 0;
  gtk_table_get_props (table)->column_spacing = 0;
  gtk_table_get_props (table)->row_spacing = 0;
  gtk_table_get_props (table)->homogeneous = FALSE;

  __gtk_table_resize (table, 1, 1);
}

GtkWidget*
__gtk_table_new (guint	rows,
	       guint	columns,
	       gboolean homogeneous)
{
  GtkTable *table;

  if (rows == 0)
    rows = 1;
  if (columns == 0)
    columns = 1;
  
  table = g_object_new (GTK_TYPE_TABLE, NULL);
  
  gtk_table_get_props (table)->homogeneous = (homogeneous ? TRUE : FALSE);

  __gtk_table_resize (table, rows, columns);
  
  return GTK_WIDGET (table);
}

void
__gtk_table_resize (GtkTable *table,
		  guint     n_rows,
		  guint     n_cols)
{
  g_return_if_fail (GTK_IS_TABLE (table));
  g_return_if_fail (n_rows > 0 && n_rows <= 65535);
  g_return_if_fail (n_cols > 0 && n_cols <= 65535);

  n_rows = MAX (n_rows, 1);
  n_cols = MAX (n_cols, 1);

  if (n_rows != gtk_table_get_props (table)->nrows ||
      n_cols != gtk_table_get_props (table)->ncols)
    {
      GList *list;
      
      for (list = gtk_table_get_props (table)->children; list; list = list->next)
	{
	  GtkTableChild *child;
	  
	  child = list->data;
	  
	  n_rows = MAX (n_rows, child->bottom_attach);
	  n_cols = MAX (n_cols, child->right_attach);
	}
      
      if (n_rows != gtk_table_get_props (table)->nrows)
	{
	  guint i;

	  i = gtk_table_get_props (table)->nrows;
	  gtk_table_get_props (table)->nrows = n_rows;
	  gtk_table_get_props (table)->rows = g_realloc (gtk_table_get_props (table)->rows, gtk_table_get_props (table)->nrows * sizeof (GtkTableRowCol));
	  
	  for (; i < gtk_table_get_props (table)->nrows; i++)
	    {
	      gtk_table_get_props (table)->rows[i].requisition = 0;
	      gtk_table_get_props (table)->rows[i].allocation = 0;
	      gtk_table_get_props (table)->rows[i].spacing = gtk_table_get_props (table)->row_spacing;
	      gtk_table_get_props (table)->rows[i].need_expand = 0;
	      gtk_table_get_props (table)->rows[i].need_shrink = 0;
	      gtk_table_get_props (table)->rows[i].expand = 0;
	      gtk_table_get_props (table)->rows[i].shrink = 0;
	    }

	  g_object_notify (G_OBJECT (table), "n-rows");
	}

      if (n_cols != gtk_table_get_props (table)->ncols)
	{
	  guint i;

	  i = gtk_table_get_props (table)->ncols;
	  gtk_table_get_props (table)->ncols = n_cols;
	  gtk_table_get_props (table)->cols = g_realloc (gtk_table_get_props (table)->cols, gtk_table_get_props (table)->ncols * sizeof (GtkTableRowCol));
	  
	  for (; i < gtk_table_get_props (table)->ncols; i++)
	    {
	      gtk_table_get_props (table)->cols[i].requisition = 0;
	      gtk_table_get_props (table)->cols[i].allocation = 0;
	      gtk_table_get_props (table)->cols[i].spacing = gtk_table_get_props (table)->column_spacing;
	      gtk_table_get_props (table)->cols[i].need_expand = 0;
	      gtk_table_get_props (table)->cols[i].need_shrink = 0;
	      gtk_table_get_props (table)->cols[i].expand = 0;
	      gtk_table_get_props (table)->cols[i].shrink = 0;
	    }

	  g_object_notify (G_OBJECT (table), "n-columns");
	}
    }
}

void
__gtk_table_attach (GtkTable	  *table,
		  GtkWidget	  *child,
		  guint		   left_attach,
		  guint		   right_attach,
		  guint		   top_attach,
		  guint		   bottom_attach,
		  GtkAttachOptions xoptions,
		  GtkAttachOptions yoptions,
		  guint		   xpadding,
		  guint		   ypadding)
{
  GtkTableChild *table_child;
  
  g_return_if_fail (GTK_IS_TABLE (table));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (gtk_widget_get_props (child)->parent == NULL);
  
  /* g_return_if_fail (left_attach >= 0); */
  g_return_if_fail (left_attach < right_attach);
  /* g_return_if_fail (top_attach >= 0); */
  g_return_if_fail (top_attach < bottom_attach);
  
  if (right_attach >= table->ncols)
    __gtk_table_resize (table, table->nrows, right_attach);
  
  if (bottom_attach >= table->nrows)
    __gtk_table_resize (table, bottom_attach, table->ncols);
  
  table_child = g_new (GtkTableChild, 1);
  table_child->widget = child;
  table_child->left_attach = left_attach;
  table_child->right_attach = right_attach;
  table_child->top_attach = top_attach;
  table_child->bottom_attach = bottom_attach;
  table_child->xexpand = (xoptions & GTK_EXPAND) != 0;
  table_child->xshrink = (xoptions & GTK_SHRINK) != 0;
  table_child->xfill = (xoptions & GTK_FILL) != 0;
  table_child->xpadding = xpadding;
  table_child->yexpand = (yoptions & GTK_EXPAND) != 0;
  table_child->yshrink = (yoptions & GTK_SHRINK) != 0;
  table_child->yfill = (yoptions & GTK_FILL) != 0;
  table_child->ypadding = ypadding;
  
  table->children = g_list_prepend (table->children, table_child);
  
  __gtk_widget_set_parent (child, GTK_WIDGET (table));
}

void
__gtk_table_attach_defaults (GtkTable  *table,
			   GtkWidget *widget,
			   guint      left_attach,
			   guint      right_attach,
			   guint      top_attach,
			   guint      bottom_attach)
{
  __gtk_table_attach (table, widget,
		    left_attach, right_attach,
		    top_attach, bottom_attach,
		    GTK_EXPAND | GTK_FILL,
		    GTK_EXPAND | GTK_FILL,
		    0, 0);
}

void
__gtk_table_set_row_spacing (GtkTable *table,
			   guint     row,
			   guint     spacing)
{
  g_return_if_fail (GTK_IS_TABLE (table));
  g_return_if_fail (row < gtk_table_get_props (table)->nrows);
  
  if (gtk_table_get_props (table)->rows[row].spacing != spacing)
    {
      gtk_table_get_props (table)->rows[row].spacing = spacing;
      
      if (__gtk_widget_get_visible (GTK_WIDGET (table)))
	__gtk_widget_queue_resize (GTK_WIDGET (table));
    }
}

/**
 * __gtk_table_get_row_spacing:
 * @table: a #GtkTable
 * @row: a row in the table, 0 indicates the first row
 *
 * Gets the amount of space between row @row, and
 * row @row + 1. See __gtk_table_set_row_spacing().
 *
 * Return value: the row spacing
 **/
guint
__gtk_table_get_row_spacing (GtkTable *table,
			   guint     row)
{
  g_return_val_if_fail (GTK_IS_TABLE (table), 0);
  g_return_val_if_fail (row < gtk_table_get_props (table)->nrows - 1, 0);
 
  return gtk_table_get_props (table)->rows[row].spacing;
}

void
__gtk_table_set_col_spacing (GtkTable *table,
			   guint     column,
			   guint     spacing)
{
  g_return_if_fail (GTK_IS_TABLE (table));
  g_return_if_fail (column < gtk_table_get_props (table)->ncols);
  
  if (gtk_table_get_props (table)->cols[column].spacing != spacing)
    {
      gtk_table_get_props (table)->cols[column].spacing = spacing;
      
      if (__gtk_widget_get_visible (GTK_WIDGET (table)))
	__gtk_widget_queue_resize (GTK_WIDGET (table));
    }
}

/**
 * __gtk_table_get_col_spacing:
 * @table: a #GtkTable
 * @column: a column in the table, 0 indicates the first column
 *
 * Gets the amount of space between column @col, and
 * column @col + 1. See __gtk_table_set_col_spacing().
 *
 * Return value: the column spacing
 **/
guint
__gtk_table_get_col_spacing (GtkTable *table,
			   guint     column)
{
  g_return_val_if_fail (GTK_IS_TABLE (table), 0);
  g_return_val_if_fail (column < gtk_table_get_props (table)->ncols, 0);

  return gtk_table_get_props (table)->cols[column].spacing;
}

void
__gtk_table_set_row_spacings (GtkTable *table,
			    guint     spacing)
{
  guint row;
  
  g_return_if_fail (GTK_IS_TABLE (table));
  
  gtk_table_get_props (table)->row_spacing = spacing;
  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
    gtk_table_get_props (table)->rows[row].spacing = spacing;
  
  if (__gtk_widget_get_visible (GTK_WIDGET (table)))
    __gtk_widget_queue_resize (GTK_WIDGET (table));

  g_object_notify (G_OBJECT (table), "row-spacing");
}

/**
 * __gtk_table_get_default_row_spacing:
 * @table: a #GtkTable
 *
 * Gets the default row spacing for the table. This is
 * the spacing that will be used for newly added rows.
 * (See __gtk_table_set_row_spacings())
 *
 * Return value: the default row spacing
 **/
guint
__gtk_table_get_default_row_spacing (GtkTable *table)
{
  g_return_val_if_fail (GTK_IS_TABLE (table), 0);

  return gtk_table_get_props (table)->row_spacing;
}

void
__gtk_table_set_col_spacings (GtkTable *table,
			    guint     spacing)
{
  guint col;
  
  g_return_if_fail (GTK_IS_TABLE (table));
  
  gtk_table_get_props (table)->column_spacing = spacing;
  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
    gtk_table_get_props (table)->cols[col].spacing = spacing;
  
  if (__gtk_widget_get_visible (GTK_WIDGET (table)))
    __gtk_widget_queue_resize (GTK_WIDGET (table));

  g_object_notify (G_OBJECT (table), "column-spacing");
}

/**
 * __gtk_table_get_default_col_spacing:
 * @table: a #GtkTable
 *
 * Gets the default column spacing for the table. This is
 * the spacing that will be used for newly added columns.
 * (See __gtk_table_set_col_spacings())
 *
 * Return value: the default column spacing
 **/
guint
__gtk_table_get_default_col_spacing (GtkTable *table)
{
  g_return_val_if_fail (GTK_IS_TABLE (table), 0);

  return gtk_table_get_props (table)->column_spacing;
}

void
__gtk_table_set_homogeneous (GtkTable *table,
			   gboolean  homogeneous)
{
  g_return_if_fail (GTK_IS_TABLE (table));

  homogeneous = (homogeneous != 0);
  if (homogeneous != gtk_table_get_props (table)->homogeneous)
    {
      gtk_table_get_props (table)->homogeneous = homogeneous;
      
      if (__gtk_widget_get_visible (GTK_WIDGET (table)))
	__gtk_widget_queue_resize (GTK_WIDGET (table));

      g_object_notify (G_OBJECT (table), "homogeneous");
    }
}

/**
 * __gtk_table_get_homogeneous:
 * @table: a #GtkTable
 *
 * Returns whether the table cells are all constrained to the same
 * width and height. (See gtk_table_set_homogenous ())
 *
 * Return value: %TRUE if the cells are all constrained to the same size
 **/
gboolean
__gtk_table_get_homogeneous (GtkTable *table)
{
  g_return_val_if_fail (GTK_IS_TABLE (table), FALSE);

  return gtk_table_get_props (table)->homogeneous;
}

/**
 * __gtk_table_get_size:
 * @table: a #GtkTable
 * @rows: (out) (allow-none): return location for the number of
 *   rows, or %NULL
 * @columns: (out) (allow-none): return location for the number
 *   of columns, or %NULL
 *
 * Returns the number of rows and columns in the table.
 *
 * Since: 2.22
 **/
void
__gtk_table_get_size (GtkTable *table,
                    guint    *rows,
                    guint    *columns)
{
  g_return_if_fail (GTK_IS_TABLE (table));

  if (rows)
    *rows = gtk_table_get_props (table)->nrows;

  if (columns)
    *columns = gtk_table_get_props (table)->ncols;
}

static void
gtk_table_finalize (GObject *object)
{
  GtkTable *table = GTK_TABLE (object);

  g_free (gtk_table_get_props (table)->rows);
  g_free (gtk_table_get_props (table)->cols);
  
  G_OBJECT_CLASS (gtk_table_parent_class)->finalize (object);
}

static void
gtk_table_size_request (GtkWidget      *widget,
			GtkRequisition *requisition)
{
  GtkTable *table = GTK_TABLE (widget);
  gint row, col;

  requisition->width = 0;
  requisition->height = 0;
  
  gtk_table_size_request_init (table);
  gtk_table_size_request_pass1 (table);
  gtk_table_size_request_pass2 (table);
  gtk_table_size_request_pass3 (table);
  gtk_table_size_request_pass2 (table);
  
  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
    requisition->width += gtk_table_get_props (table)->cols[col].requisition;
  for (col = 0; col + 1 < gtk_table_get_props (table)->ncols; col++)
    requisition->width += gtk_table_get_props (table)->cols[col].spacing;
  
  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
    requisition->height += gtk_table_get_props (table)->rows[row].requisition;
  for (row = 0; row + 1 < gtk_table_get_props (table)->nrows; row++)
    requisition->height += gtk_table_get_props (table)->rows[row].spacing;
  
  requisition->width += GTK_CONTAINER (table)->border_width * 2;
  requisition->height += GTK_CONTAINER (table)->border_width * 2;
}

static void
gtk_table_size_allocate (GtkWidget     *widget,
			 GtkAllocation *allocation)
{
  GtkTable *table = GTK_TABLE (widget);

  gtk_widget_get_props (widget)->allocation = *allocation;

  gtk_table_size_allocate_init (table);
  gtk_table_size_allocate_pass1 (table);
  gtk_table_size_allocate_pass2 (table);
}

static void
gtk_table_add (GtkContainer *container,
	       GtkWidget    *widget)
{
  __gtk_table_attach_defaults (GTK_TABLE (container), widget, 0, 1, 0, 1);
}

static void
gtk_table_remove (GtkContainer *container,
		  GtkWidget    *widget)
{
  GtkTable *table = GTK_TABLE (container);
  GtkTableChild *child;
  GtkWidget *widget_container = GTK_WIDGET (container);
  GList *children;

  children = gtk_table_get_props (table)->children;
  
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (child->widget == widget)
	{
	  gboolean was_visible = __gtk_widget_get_visible (widget);
	  
	  __gtk_widget_unparent (widget);
	  
	  gtk_table_get_props (table)->children = g_list_remove (gtk_table_get_props (table)->children, child);
	  g_free (child);
	  
	  if (was_visible && __gtk_widget_get_visible (widget_container))
	    __gtk_widget_queue_resize (widget_container);
	  break;
	}
    }
}

static void
gtk_table_forall (GtkContainer *container,
		  gboolean	include_internals,
		  GtkCallback	callback,
		  gpointer	callback_data)
{
  GtkTable *table = GTK_TABLE (container);
  GtkTableChild *child;
  GList *children;

  children = gtk_table_get_props (table)->children;
  
  while (children)
    {
      child = children->data;
      children = children->next;
      
      (* callback) (child->widget, callback_data);
    }
}

static void
gtk_table_size_request_init (GtkTable *table)
{
  GtkTableChild *child;
  GList *children;
  gint row, col;
  
  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
    {
      gtk_table_get_props (table)->rows[row].requisition = 0;
      gtk_table_get_props (table)->rows[row].expand = FALSE;
    }
  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
    {
      gtk_table_get_props (table)->cols[col].requisition = 0;
      gtk_table_get_props (table)->cols[col].expand = FALSE;
    }
  
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	__gtk_widget_size_request (child->widget, NULL);

      if (child->left_attach == (child->right_attach - 1) && child->xexpand)
	gtk_table_get_props (table)->cols[child->left_attach].expand = TRUE;
      
      if (child->top_attach == (child->bottom_attach - 1) && child->yexpand)
	gtk_table_get_props (table)->rows[child->top_attach].expand = TRUE;
    }
}

static void
gtk_table_size_request_pass1 (GtkTable *table)
{
  GtkTableChild *child;
  GList *children;
  gint width;
  gint height;
  
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  GtkRequisition child_requisition;
	  __gtk_widget_get_child_requisition (child->widget, &child_requisition);

	  /* Child spans a single column.
	   */
	  if (child->left_attach == (child->right_attach - 1))
	    {
	      width = child_requisition.width + child->xpadding * 2;
	      gtk_table_get_props (table)->cols[child->left_attach].requisition = MAX (gtk_table_get_props (table)->cols[child->left_attach].requisition, width);
	    }
	  
	  /* Child spans a single row.
	   */
	  if (child->top_attach == (child->bottom_attach - 1))
	    {
	      height = child_requisition.height + child->ypadding * 2;
	      gtk_table_get_props (table)->rows[child->top_attach].requisition = MAX (gtk_table_get_props (table)->rows[child->top_attach].requisition, height);
	    }
	}
    }
}

static void
gtk_table_size_request_pass2 (GtkTable *table)
{
  gint max_width;
  gint max_height;
  gint row, col;
  
  if (gtk_table_get_props (table)->homogeneous)
    {
      max_width = 0;
      max_height = 0;
      
      for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	max_width = MAX (max_width, gtk_table_get_props (table)->cols[col].requisition);
      for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	max_height = MAX (max_height, gtk_table_get_props (table)->rows[row].requisition);
      
      for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	gtk_table_get_props (table)->cols[col].requisition = max_width;
      for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	gtk_table_get_props (table)->rows[row].requisition = max_height;
    }
}

static void
gtk_table_size_request_pass3 (GtkTable *table)
{
  GtkTableChild *child;
  GList *children;
  gint width, height;
  gint row, col;
  gint extra;
  
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  /* Child spans multiple columns.
	   */
	  if (child->left_attach != (child->right_attach - 1))
	    {
	      GtkRequisition child_requisition;

	      __gtk_widget_get_child_requisition (child->widget, &child_requisition);
	      
	      /* Check and see if there is already enough space
	       *  for the child.
	       */
	      width = 0;
	      for (col = child->left_attach; col < child->right_attach; col++)
		{
		  width += gtk_table_get_props (table)->cols[col].requisition;
		  if ((col + 1) < child->right_attach)
		    width += gtk_table_get_props (table)->cols[col].spacing;
		}
	      
	      /* If we need to request more space for this child to fill
	       *  its requisition, then divide up the needed space amongst the
	       *  columns it spans, favoring expandable columns if any.
	       */
	      if (width < child_requisition.width + child->xpadding * 2)
		{
		  gint n_expand = 0;
		  gboolean force_expand = FALSE;
		  
		  width = child_requisition.width + child->xpadding * 2 - width;

		  for (col = child->left_attach; col < child->right_attach; col++)
		    if (gtk_table_get_props (table)->cols[col].expand)
		      n_expand++;

		  if (n_expand == 0)
		    {
		      n_expand = (child->right_attach - child->left_attach);
		      force_expand = TRUE;
		    }
		    
		  for (col = child->left_attach; col < child->right_attach; col++)
		    if (force_expand || gtk_table_get_props (table)->cols[col].expand)
		      {
			extra = width / n_expand;
			gtk_table_get_props (table)->cols[col].requisition += extra;
			width -= extra;
			n_expand--;
		      }
		}
	    }
	  
	  /* Child spans multiple rows.
	   */
	  if (child->top_attach != (child->bottom_attach - 1))
	    {
	      GtkRequisition child_requisition;

	      __gtk_widget_get_child_requisition (child->widget, &child_requisition);

	      /* Check and see if there is already enough space
	       *  for the child.
	       */
	      height = 0;
	      for (row = child->top_attach; row < child->bottom_attach; row++)
		{
		  height += gtk_table_get_props (table)->rows[row].requisition;
		  if ((row + 1) < child->bottom_attach)
		    height += gtk_table_get_props (table)->rows[row].spacing;
		}
	      
	      /* If we need to request more space for this child to fill
	       *  its requisition, then divide up the needed space amongst the
	       *  rows it spans, favoring expandable rows if any.
	       */
	      if (height < child_requisition.height + child->ypadding * 2)
		{
		  gint n_expand = 0;
		  gboolean force_expand = FALSE;
		  
		  height = child_requisition.height + child->ypadding * 2 - height;
		  
		  for (row = child->top_attach; row < child->bottom_attach; row++)
		    {
		      if (gtk_table_get_props (table)->rows[row].expand)
			n_expand++;
		    }

		  if (n_expand == 0)
		    {
		      n_expand = (child->bottom_attach - child->top_attach);
		      force_expand = TRUE;
		    }
		    
		  for (row = child->top_attach; row < child->bottom_attach; row++)
		    if (force_expand || gtk_table_get_props (table)->rows[row].expand)
		      {
			extra = height / n_expand;
			gtk_table_get_props (table)->rows[row].requisition += extra;
			height -= extra;
			n_expand--;
		      }
		}
	    }
	}
    }
}

static void
gtk_table_size_allocate_init (GtkTable *table)
{
  GtkTableChild *child;
  GList *children;
  gint row, col;
  gint has_expand;
  gint has_shrink;
  
  /* Initialize the rows and cols.
   *  By default, rows and cols do not expand and do shrink.
   *  Those values are modified by the children that occupy
   *  the rows and cols.
   */
  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
    {
      gtk_table_get_props (table)->cols[col].allocation = gtk_table_get_props (table)->cols[col].requisition;
      gtk_table_get_props (table)->cols[col].need_expand = FALSE;
      gtk_table_get_props (table)->cols[col].need_shrink = TRUE;
      gtk_table_get_props (table)->cols[col].expand = FALSE;
      gtk_table_get_props (table)->cols[col].shrink = TRUE;
      gtk_table_get_props (table)->cols[col].empty = TRUE;
    }
  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
    {
      gtk_table_get_props (table)->rows[row].allocation = gtk_table_get_props (table)->rows[row].requisition;
      gtk_table_get_props (table)->rows[row].need_expand = FALSE;
      gtk_table_get_props (table)->rows[row].need_shrink = TRUE;
      gtk_table_get_props (table)->rows[row].expand = FALSE;
      gtk_table_get_props (table)->rows[row].shrink = TRUE;
      gtk_table_get_props (table)->rows[row].empty = TRUE;
    }
  
  /* Loop over all the children and adjust the row and col values
   *  based on whether the children want to be allowed to expand
   *  or shrink. This loop handles children that occupy a single
   *  row or column.
   */
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  if (child->left_attach == (child->right_attach - 1))
	    {
	      if (child->xexpand)
		gtk_table_get_props (table)->cols[child->left_attach].expand = TRUE;
	      
	      if (!child->xshrink)
		gtk_table_get_props (table)->cols[child->left_attach].shrink = FALSE;
	      
	      gtk_table_get_props (table)->cols[child->left_attach].empty = FALSE;
	    }
	  
	  if (child->top_attach == (child->bottom_attach - 1))
	    {
	      if (child->yexpand)
		gtk_table_get_props (table)->rows[child->top_attach].expand = TRUE;
	      
	      if (!child->yshrink)
		gtk_table_get_props (table)->rows[child->top_attach].shrink = FALSE;

	      gtk_table_get_props (table)->rows[child->top_attach].empty = FALSE;
	    }
	}
    }
  
  /* Loop over all the children again and this time handle children
   *  which span multiple rows or columns.
   */
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  if (child->left_attach != (child->right_attach - 1))
	    {
	      for (col = child->left_attach; col < child->right_attach; col++)
		gtk_table_get_props (table)->cols[col].empty = FALSE;

	      if (child->xexpand)
		{
		  has_expand = FALSE;
		  for (col = child->left_attach; col < child->right_attach; col++)
		    if (gtk_table_get_props (table)->cols[col].expand)
		      {
			has_expand = TRUE;
			break;
		      }
		  
		  if (!has_expand)
		    for (col = child->left_attach; col < child->right_attach; col++)
		      gtk_table_get_props (table)->cols[col].need_expand = TRUE;
		}
	      
	      if (!child->xshrink)
		{
		  has_shrink = TRUE;
		  for (col = child->left_attach; col < child->right_attach; col++)
		    if (!gtk_table_get_props (table)->cols[col].shrink)
		      {
			has_shrink = FALSE;
			break;
		      }
		  
		  if (has_shrink)
		    for (col = child->left_attach; col < child->right_attach; col++)
		      gtk_table_get_props (table)->cols[col].need_shrink = FALSE;
		}
	    }
	  
	  if (child->top_attach != (child->bottom_attach - 1))
	    {
	      for (row = child->top_attach; row < child->bottom_attach; row++)
		gtk_table_get_props (table)->rows[row].empty = FALSE;

	      if (child->yexpand)
		{
		  has_expand = FALSE;
		  for (row = child->top_attach; row < child->bottom_attach; row++)
		    if (gtk_table_get_props (table)->rows[row].expand)
		      {
			has_expand = TRUE;
			break;
		      }
		  
		  if (!has_expand)
		    for (row = child->top_attach; row < child->bottom_attach; row++)
		      gtk_table_get_props (table)->rows[row].need_expand = TRUE;
		}
	      
	      if (!child->yshrink)
		{
		  has_shrink = TRUE;
		  for (row = child->top_attach; row < child->bottom_attach; row++)
		    if (!gtk_table_get_props (table)->rows[row].shrink)
		      {
			has_shrink = FALSE;
			break;
		      }
		  
		  if (has_shrink)
		    for (row = child->top_attach; row < child->bottom_attach; row++)
		      gtk_table_get_props (table)->rows[row].need_shrink = FALSE;
		}
	    }
	}
    }
  
  /* Loop over the columns and set the expand and shrink values
   *  if the column can be expanded or shrunk.
   */
  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
    {
      if (gtk_table_get_props (table)->cols[col].empty)
	{
	  gtk_table_get_props (table)->cols[col].expand = FALSE;
	  gtk_table_get_props (table)->cols[col].shrink = FALSE;
	}
      else
	{
	  if (gtk_table_get_props (table)->cols[col].need_expand)
	    gtk_table_get_props (table)->cols[col].expand = TRUE;
	  if (!gtk_table_get_props (table)->cols[col].need_shrink)
	    gtk_table_get_props (table)->cols[col].shrink = FALSE;
	}
    }
  
  /* Loop over the rows and set the expand and shrink values
   *  if the row can be expanded or shrunk.
   */
  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
    {
      if (gtk_table_get_props (table)->rows[row].empty)
	{
	  gtk_table_get_props (table)->rows[row].expand = FALSE;
	  gtk_table_get_props (table)->rows[row].shrink = FALSE;
	}
      else
	{
	  if (gtk_table_get_props (table)->rows[row].need_expand)
	    gtk_table_get_props (table)->rows[row].expand = TRUE;
	  if (!gtk_table_get_props (table)->rows[row].need_shrink)
	    gtk_table_get_props (table)->rows[row].shrink = FALSE;
	}
    }
}

static void
gtk_table_size_allocate_pass1 (GtkTable *table)
{
  gint real_width;
  gint real_height;
  gint width, height;
  gint row, col;
  gint nexpand;
  gint nshrink;
  gint extra;
  
  /* If we were allocated more space than we requested
   *  then we have to expand any expandable rows and columns
   *  to fill in the extra space.
   */
  
  real_width = GTK_WIDGET (table)->allocation.width - GTK_CONTAINER (table)->border_width * 2;
  real_height = GTK_WIDGET (table)->allocation.height - GTK_CONTAINER (table)->border_width * 2;
  
  if (gtk_table_get_props (table)->homogeneous)
    {
      if (!gtk_table_get_props (table)->children)
	nexpand = 1;
      else
	{
	  nexpand = 0;
	  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	    if (gtk_table_get_props (table)->cols[col].expand)
	      {
		nexpand += 1;
		break;
	      }
	}
      if (nexpand)
	{
	  width = real_width;
	  for (col = 0; col + 1 < gtk_table_get_props (table)->ncols; col++)
	    width -= gtk_table_get_props (table)->cols[col].spacing;
	  
	  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	    {
	      extra = width / (gtk_table_get_props (table)->ncols - col);
	      gtk_table_get_props (table)->cols[col].allocation = MAX (1, extra);
	      width -= extra;
	    }
	}
    }
  else
    {
      width = 0;
      nexpand = 0;
      nshrink = 0;
      
      for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	{
	  width += gtk_table_get_props (table)->cols[col].requisition;
	  if (gtk_table_get_props (table)->cols[col].expand)
	    nexpand += 1;
	  if (gtk_table_get_props (table)->cols[col].shrink)
	    nshrink += 1;
	}
      for (col = 0; col + 1 < gtk_table_get_props (table)->ncols; col++)
	width += gtk_table_get_props (table)->cols[col].spacing;
      
      /* Check to see if we were allocated more width than we requested.
       */
      if ((width < real_width) && (nexpand >= 1))
	{
	  width = real_width - width;
	  
	  for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
	    if (gtk_table_get_props (table)->cols[col].expand)
	      {
		extra = width / nexpand;
		gtk_table_get_props (table)->cols[col].allocation += extra;
		
		width -= extra;
		nexpand -= 1;
	      }
	}
      
      /* Check to see if we were allocated less width than we requested,
       * then shrink until we fit the size give.
       */
      if (width > real_width)
	{
	  gint total_nshrink = nshrink;

	  extra = width - real_width;
	  while (total_nshrink > 0 && extra > 0)
	    {
	      nshrink = total_nshrink;
	      for (col = 0; col < gtk_table_get_props (table)->ncols; col++)
		if (gtk_table_get_props (table)->cols[col].shrink)
		  {
		    gint allocation = gtk_table_get_props (table)->cols[col].allocation;

		    gtk_table_get_props (table)->cols[col].allocation = MAX (1, (gint) gtk_table_get_props (table)->cols[col].allocation - extra / nshrink);
		    extra -= allocation - gtk_table_get_props (table)->cols[col].allocation;
		    nshrink -= 1;
		    if (gtk_table_get_props (table)->cols[col].allocation < 2)
		      {
			total_nshrink -= 1;
			gtk_table_get_props (table)->cols[col].shrink = FALSE;
		      }
		  }
	    }
	}
    }
  
  if (gtk_table_get_props (table)->homogeneous)
    {
      if (!gtk_table_get_props (table)->children)
	nexpand = 1;
      else
	{
	  nexpand = 0;
	  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	    if (gtk_table_get_props (table)->rows[row].expand)
	      {
		nexpand += 1;
		break;
	      }
	}
      if (nexpand)
	{
	  height = real_height;
	  
	  for (row = 0; row + 1 < gtk_table_get_props (table)->nrows; row++)
	    height -= gtk_table_get_props (table)->rows[row].spacing;
	  
	  
	  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	    {
	      extra = height / (gtk_table_get_props (table)->nrows - row);
	      gtk_table_get_props (table)->rows[row].allocation = MAX (1, extra);
	      height -= extra;
	    }
	}
    }
  else
    {
      height = 0;
      nexpand = 0;
      nshrink = 0;
      
      for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	{
	  height += gtk_table_get_props (table)->rows[row].requisition;
	  if (gtk_table_get_props (table)->rows[row].expand)
	    nexpand += 1;
	  if (gtk_table_get_props (table)->rows[row].shrink)
	    nshrink += 1;
	}
      for (row = 0; row + 1 < gtk_table_get_props (table)->nrows; row++)
	height += gtk_table_get_props (table)->rows[row].spacing;
      
      /* Check to see if we were allocated more height than we requested.
       */
      if ((height < real_height) && (nexpand >= 1))
	{
	  height = real_height - height;
	  
	  for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
	    if (gtk_table_get_props (table)->rows[row].expand)
	      {
		extra = height / nexpand;
		gtk_table_get_props (table)->rows[row].allocation += extra;
		
		height -= extra;
		nexpand -= 1;
	      }
	}
      
      /* Check to see if we were allocated less height than we requested.
       * then shrink until we fit the size give.
       */
      if (height > real_height)
	{
	  gint total_nshrink = nshrink;
	  
	  extra = height - real_height;
	  while (total_nshrink > 0 && extra > 0)
	    {
	      nshrink = total_nshrink;
	      for (row = 0; row < gtk_table_get_props (table)->nrows; row++)
		if (gtk_table_get_props (table)->rows[row].shrink)
		  {
		    gint allocation = gtk_table_get_props (table)->rows[row].allocation;
		    
		    gtk_table_get_props (table)->rows[row].allocation = MAX (1, (gint) gtk_table_get_props (table)->rows[row].allocation - extra / nshrink);
		    extra -= allocation - gtk_table_get_props (table)->rows[row].allocation;
		    nshrink -= 1;
		    if (gtk_table_get_props (table)->rows[row].allocation < 2)
		      {
			total_nshrink -= 1;
			gtk_table_get_props (table)->rows[row].shrink = FALSE;
		      }
		  }
	    }
	}
    }
}

static void
gtk_table_size_allocate_pass2 (GtkTable *table)
{
  GtkTableChild *child;
  GList *children;
  gint max_width;
  gint max_height;
  gint x, y;
  gint row, col;
  GtkAllocation allocation;
  GtkWidget *widget = GTK_WIDGET (table);
  
  children = gtk_table_get_props (table)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  GtkRequisition child_requisition;
	  __gtk_widget_get_child_requisition (child->widget, &child_requisition);

	  x = GTK_WIDGET (table)->allocation.x + GTK_CONTAINER (table)->border_width;
	  y = GTK_WIDGET (table)->allocation.y + GTK_CONTAINER (table)->border_width;
	  max_width = 0;
	  max_height = 0;
	  
	  for (col = 0; col < child->left_attach; col++)
	    {
	      x += gtk_table_get_props (table)->cols[col].allocation;
	      x += gtk_table_get_props (table)->cols[col].spacing;
	    }
	  
	  for (col = child->left_attach; col < child->right_attach; col++)
	    {
	      max_width += gtk_table_get_props (table)->cols[col].allocation;
	      if ((col + 1) < child->right_attach)
		max_width += gtk_table_get_props (table)->cols[col].spacing;
	    }
	  
	  for (row = 0; row < child->top_attach; row++)
	    {
	      y += gtk_table_get_props (table)->rows[row].allocation;
	      y += gtk_table_get_props (table)->rows[row].spacing;
	    }
	  
	  for (row = child->top_attach; row < child->bottom_attach; row++)
	    {
	      max_height += gtk_table_get_props (table)->rows[row].allocation;
	      if ((row + 1) < child->bottom_attach)
		max_height += gtk_table_get_props (table)->rows[row].spacing;
	    }
	  
	  if (child->xfill)
	    {
	      allocation.width = MAX (1, max_width - (gint)child->xpadding * 2);
	      allocation.x = x + (max_width - allocation.width) / 2;
	    }
	  else
	    {
	      allocation.width = child_requisition.width;
	      allocation.x = x + (max_width - allocation.width) / 2;
	    }
	  
	  if (child->yfill)
	    {
	      allocation.height = MAX (1, max_height - (gint)child->ypadding * 2);
	      allocation.y = y + (max_height - allocation.height) / 2;
	    }
	  else
	    {
	      allocation.height = child_requisition.height;
	      allocation.y = y + (max_height - allocation.height) / 2;
	    }

	  if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	    allocation.x = gtk_widget_get_props (widget)->allocation.x + gtk_widget_get_props (widget)->allocation.width
	      - (allocation.x - gtk_widget_get_props (widget)->allocation.x) - allocation.width;
	  
	  __gtk_widget_size_allocate (child->widget, &allocation);
	}
    }
}
