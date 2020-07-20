
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

#ifndef __GTK_TABLE_H__
#define __GTK_TABLE_H__

#include <gtkcontainer.h>


G_BEGIN_DECLS

#define GTK_TYPE_TABLE			(gtk_table_get_type ())
#define GTK_TABLE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TABLE, GtkTable))
#define GTK_TABLE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TABLE, GtkTableClass))
#define GTK_IS_TABLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TABLE))
#define GTK_IS_TABLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TABLE))
#define GTK_TABLE_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TABLE, GtkTableClass))


typedef struct _GtkTable	GtkTableFat;
typedef struct _GtkTable	GtkTableThin;
typedef struct _GtkTableClass	GtkTableClassFat;
typedef struct _GtkTableClass	GtkTableClassThin;
typedef struct _GtkTableChild	GtkTableChild;
typedef struct _GtkTableRowCol	GtkTableRowCol;

struct _GtkTable
{
  GtkContainer container;

  GList * (children);
  GtkTableRowCol * (rows);
  GtkTableRowCol * (cols);
  guint16  (nrows);
  guint16  (ncols);
  guint16  (column_spacing);
  guint16  (row_spacing);
  guint  (homogeneous) : 1;
};

struct _GtkTableClass
{
  GtkContainerClass parent_class;
};

struct _GtkTableChild
{
  GtkWidget *widget;
  guint16 left_attach;
  guint16 right_attach;
  guint16 top_attach;
  guint16 bottom_attach;
  guint16 xpadding;
  guint16 ypadding;
  guint xexpand : 1;
  guint yexpand : 1;
  guint xshrink : 1;
  guint yshrink : 1;
  guint xfill : 1;
  guint yfill : 1;
};

struct _GtkTableRowCol
{
  guint16 requisition;
  guint16 allocation;
  guint16 spacing;
  guint need_expand : 1;
  guint need_shrink : 1;
  guint expand : 1;
  guint shrink : 1;
  guint empty : 1;
};


GType	   _T2_gtk_table_get_type	      (void) G_GNUC_CONST;
GType	   _3T_gtk_table_get_type	      (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType	   gtk_table_get_type	      (void) G_GNUC_CONST;
GtkWidget* __gtk_table_new	      (guint		rows,
				       guint		columns,
				       gboolean		homogeneous);
void	   __gtk_table_resize	      (GtkTable	       *table,
				       guint            rows,
				       guint            columns);
void	   __gtk_table_attach	      (GtkTable	       *table,
				       GtkWidget       *child,
				       guint		left_attach,
				       guint		right_attach,
				       guint		top_attach,
				       guint		bottom_attach,
				       GtkAttachOptions xoptions,
				       GtkAttachOptions yoptions,
				       guint		xpadding,
				       guint		ypadding);
void	   __gtk_table_attach_defaults  (GtkTable	       *table,
				       GtkWidget       *widget,
				       guint		left_attach,
				       guint		right_attach,
				       guint		top_attach,
				       guint		bottom_attach);
void	   __gtk_table_set_row_spacing  (GtkTable	       *table,
				       guint		row,
				       guint		spacing);
guint      __gtk_table_get_row_spacing  (GtkTable        *table,
				       guint            row);
void	   __gtk_table_set_col_spacing  (GtkTable	       *table,
				       guint		column,
				       guint		spacing);
guint      __gtk_table_get_col_spacing  (GtkTable        *table,
				       guint            column);
void	   __gtk_table_set_row_spacings (GtkTable	       *table,
				       guint		spacing);
guint      __gtk_table_get_default_row_spacing (GtkTable        *table);
void	   __gtk_table_set_col_spacings (GtkTable	       *table,
				       guint		spacing);
guint      __gtk_table_get_default_col_spacing (GtkTable        *table);
void	   __gtk_table_set_homogeneous  (GtkTable	       *table,
				       gboolean		homogeneous);
gboolean   __gtk_table_get_homogeneous  (GtkTable        *table);
void       __gtk_table_get_size         (GtkTable        *table,
                                       guint           *rows,
                                       guint           *columns);


G_END_DECLS

#endif /* __GTK_TABLE_H__ */
