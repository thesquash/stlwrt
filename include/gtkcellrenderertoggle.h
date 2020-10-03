/* gtkcellrenderertoggle.h
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

#ifndef __GTK_CELL_RENDERER_TOGGLE_H__
#define __GTK_CELL_RENDERER_TOGGLE_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkcellrenderer.h>


G_BEGIN_DECLS


#define GTK_TYPE_CELL_RENDERER_TOGGLE			(gtk_cell_renderer_toggle_get_type ())
#define GTK_CELL_RENDERER_TOGGLE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggle))
#define GTK_CELL_RENDERER_TOGGLE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggleClass))
#define GTK_IS_CELL_RENDERER_TOGGLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE))
#define GTK_IS_CELL_RENDERER_TOGGLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_TOGGLE))
#define GTK_CELL_RENDERER_TOGGLE_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggleClass))

typedef struct _GtkCellRendererToggleClass GtkCellRendererToggleClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererToggle, gtk_cell_renderer_toggle, GtkCellRenderer,
  /*< private >*/
  guint  (active) : 1;
  guint  (activatable) : 1;
  guint  (radio) : 1;
};

struct _GtkCellRendererTogglePrivate
{
  gint indicator_size;

  guint inconsistent : 1;
};

struct _GtkCellRendererToggleClass
{
  GtkCellRendererClass parent_class;

  void (* toggled) (GtkCellRendererToggle *cell_renderer_toggle,
		    const gchar                 *path);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkCellRenderer *SF(gtk_cell_renderer_toggle_new)            (void);

gboolean         SF(gtk_cell_renderer_toggle_get_radio)      (GtkCellRendererToggle *toggle);
void             SF(gtk_cell_renderer_toggle_set_radio)      (GtkCellRendererToggle *toggle,
                                                          gboolean               radio);

gboolean        SF(gtk_cell_renderer_toggle_get_active)      (GtkCellRendererToggle *toggle);
void            SF(gtk_cell_renderer_toggle_set_active)      (GtkCellRendererToggle *toggle,
                                                          gboolean               setting);

gboolean        SF(gtk_cell_renderer_toggle_get_activatable) (GtkCellRendererToggle *toggle);
void            SF(gtk_cell_renderer_toggle_set_activatable) (GtkCellRendererToggle *toggle,
                                                          gboolean               setting);


G_END_DECLS

#endif /* __GTK_CELL_RENDERER_TOGGLE_H__ */
