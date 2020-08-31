/* gtkcellrendererprogress.h
 * Copyright (C) 2002 Naba Kumar <kh_naba@users.sourceforge.net>
 * modified by Jörgen Scheibengruber <mfcn@gmx.de>
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

#ifndef __GTK_CELL_RENDERER_PROGRESS_H__
#define __GTK_CELL_RENDERER_PROGRESS_H__

#include <gtkcellrenderer.h>

G_BEGIN_DECLS

#define GTK_TYPE_CELL_RENDERER_PROGRESS (SF(gtk_cell_renderer_progress_get_type) ())
#define GTK_CELL_RENDERER_PROGRESS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_PROGRESS, GtkCellRendererProgress))
#define GTK_CELL_RENDERER_PROGRESS_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_PROGRESS, GtkCellRendererProgressClass))
#define GTK_IS_CELL_RENDERER_PROGRESS(obj)	  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_PROGRESS))
#define GTK_IS_CELL_RENDERER_PROGRESS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_PROGRESS))
#define GTK_CELL_RENDERER_PROGRESS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_PROGRESS, GtkCellRendererProgressClass))

typedef struct _GtkCellRendererProgress         GtkCellRendererProgress;

typedef struct _GtkCellRendererProgressClass    GtkCellRendererProgressClass;
typedef struct _GtkCellRendererProgressPrivate  GtkCellRendererProgressPrivate;


struct _GtkCellRendererProgressPrivate
{
  gint value;
  gchar *text;
  gchar *label;
  gint min_h;
  gint min_w;
  gint pulse;
  gint offset;
  gfloat text_xalign;
  gfloat text_yalign;
  GtkProgressBarOrientation orientation;
};

struct _GtkCellRendererProgress
{
  GtkCellRenderer parent_instance;

  /*< private >*/
  GtkCellRendererProgressPrivate * (priv);
};

struct _GtkCellRendererProgressClass
{
  GtkCellRendererClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType		 SF(gtk_cell_renderer_progress_get_type) (void) G_GNUC_CONST;
GtkCellRenderer* SF(gtk_cell_renderer_progress_new)      (void);

G_END_DECLS

#endif  /* __GTK_CELL_RENDERER_PROGRESS_H__ */
