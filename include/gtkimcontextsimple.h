/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat Software
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

#ifndef __GTK_IM_CONTEXT_SIMPLE_H__
#define __GTK_IM_CONTEXT_SIMPLE_H__

#include <stlwrt.h>


#include <gtkimcontext.h>


G_BEGIN_DECLS


#define GTK_TYPE_IM_CONTEXT_SIMPLE              (gtk_im_context_simple_get_type ())
#define GTK_IM_CONTEXT_SIMPLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_IM_CONTEXT_SIMPLE, GtkIMContextSimple))
#define GTK_IM_CONTEXT_SIMPLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_IM_CONTEXT_SIMPLE, GtkIMContextSimpleClass))
#define GTK_IS_IM_CONTEXT_SIMPLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_IM_CONTEXT_SIMPLE))
#define GTK_IS_IM_CONTEXT_SIMPLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_IM_CONTEXT_SIMPLE))
#define GTK_IM_CONTEXT_SIMPLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_IM_CONTEXT_SIMPLE, GtkIMContextSimpleClass))

typedef struct _GtkIMContextSimpleClass  GtkIMContextSimpleClass;

#define GTK_MAX_COMPOSE_LEN 7

STLWRT_DECLARE_VTYPE_FPARENT(GtkIMContextSimple, gtk_im_context_simple, GtkIMContext,
  GSList * (tables);

  guint  (compose_buffer[GTK_MAX_COMPOSE_LEN + 1]);
  gunichar  (tentative_match);
  gint  (tentative_match_len);

  guint  (in_hex_sequence) : 1;
  guint  (modifiers_dropped) : 1;
)

struct _GtkIMContextSimpleClass
{
  GtkIMContextClass parent_class;
};


GtkIMContext *SF(gtk_im_context_simple_new)       (void);

void          SF(gtk_im_context_simple_add_table) (GtkIMContextSimple *context_simple,
					       guint16            *data,
					       gint                max_seq_len,
					       gint                n_seqs);


G_END_DECLS


#endif /* __GTK_IM_CONTEXT_SIMPLE_H__ */
