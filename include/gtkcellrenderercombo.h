/* GtkCellRendererCombo
 * Copyright (C) 2004 Lorenzo Gil Sanchez
 *
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
 */

#ifndef __GTK_CELL_RENDERER_COMBO_H__
#define __GTK_CELL_RENDERER_COMBO_H__

#include <stlwrt.h>


#include <gtktreemodel.h>

#include <gtkcellrenderertext.h>

G_BEGIN_DECLS

#define GTK_TYPE_CELL_RENDERER_COMBO		(gtk_cell_renderer_combo_get_type ())
#define GTK_CELL_RENDERER_COMBO(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_COMBO, GtkCellRendererCombo))
#define GTK_CELL_RENDERER_COMBO_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_COMBO, GtkCellRendererComboClass))
#define GTK_IS_CELL_RENDERER_COMBO(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_COMBO))
#define GTK_IS_CELL_RENDERER_COMBO_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_COMBO))
#define GTK_CELL_RENDERER_COMBO_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_COMBO, GtkCellRendererTextClass))

typedef struct _GtkCellRendererComboClass GtkCellRendererComboClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererCombo, gtk_cell_renderer_combo, GtkCellRendererText,
  GtkTreeModel * (model);
  gint           (text_column);
  gboolean       (has_entry);

  /*< private >*/
  guint          (focus_out_id);
)

struct _GtkCellRendererComboClass
{
  GtkCellRendererTextClass parent;
};


GtkCellRenderer *SF(gtk_cell_renderer_combo_new)      (void);

G_END_DECLS

#endif /* __GTK_CELL_RENDERER_COMBO_H__ */
