
/* gtkcellrendererpixbuf.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
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

#ifndef __GTK_CELL_RENDERER_PIXBUF_H__
#define __GTK_CELL_RENDERER_PIXBUF_H__

#include <gtkcellrenderer.h>


G_BEGIN_DECLS


#define GTK_TYPE_CELL_RENDERER_PIXBUF			(gtk_cell_renderer_pixbuf_get_type ())
#define GTK_CELL_RENDERER_PIXBUF(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_PIXBUF, GtkCellRendererPixbuf))
#define GTK_CELL_RENDERER_PIXBUF_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_PIXBUF, GtkCellRendererPixbufClass))
#define GTK_IS_CELL_RENDERER_PIXBUF(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_PIXBUF))
#define GTK_IS_CELL_RENDERER_PIXBUF_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_PIXBUF))
#define GTK_CELL_RENDERER_PIXBUF_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_PIXBUF, GtkCellRendererPixbufClass))

typedef struct _GtkCellRendererPixbuf GtkCellRendererPixbufFat;
typedef struct _GtkCellRendererPixbuf GtkCellRendererPixbufThin;

typedef struct _GtkCellRendererPixbufClass GtkCellRendererPixbufClass;


/********************************************************************/
struct _GtkCellRendererPixbufInstanceProps
{


  /*< private >*/
  GdkPixbuf * (pixbuf);
  GdkPixbuf * (pixbuf_expander_open);
  GdkPixbuf * (pixbuf_expander_closed);
};

struct _GtkCellRendererPixbufPrivate
{
  struct _GtkCellRendererPixbufInstanceProps  instance_properties;
  
  gchar *stock_id;
  GtkIconSize stock_size;
  gchar *stock_detail;
  gboolean follow_state;
  gchar *icon_name;
  GIcon *gicon;
};

struct _GtkCellRendererPixbufFat
{
  GtkCellRendererFat   parent;

  struct _GtkCellRendererPixbufInstanceProps instance_properties;
};

struct _GtkCellRendererPixbufThin
{
  GtkCellRendererThin  parent;

  gpointer reserved;
};


typedef union
{
  struct _GtkCellRendererPixbufFat   fat_instance;
  struct _GtkCellRendererPixbufThin  thin_instance;
}   GtkCellRendererPixbuf;
/********************************************************************/



struct _GtkCellRendererPixbufClass
{
  GtkCellRendererClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType            _T2_gtk_cell_renderer_pixbuf_get_type (void) G_GNUC_CONST;
GType            _3T_gtk_cell_renderer_pixbuf_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType            gtk_cell_renderer_pixbuf_get_type (void) G_GNUC_CONST;
GtkCellRenderer *__gtk_cell_renderer_pixbuf_new      (void);


G_END_DECLS


#endif /* __GTK_CELL_RENDERER_PIXBUF_H__ */
