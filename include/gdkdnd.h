
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GDK_DND_H__
#define __GDK_DND_H__

#include <gdktypes.h>

G_BEGIN_DECLS

typedef struct _GdkDragContext        GdkDragContextFat;
typedef struct _GdkDragContext        GdkDragContextThin;

typedef enum
{
  GDK_ACTION_DEFAULT = 1 << 0,
  GDK_ACTION_COPY    = 1 << 1,
  GDK_ACTION_MOVE    = 1 << 2,
  GDK_ACTION_LINK    = 1 << 3,
  GDK_ACTION_PRIVATE = 1 << 4,
  GDK_ACTION_ASK     = 1 << 5
} GdkDragAction;

typedef enum
{
  GDK_DRAG_PROTO_MOTIF,
  GDK_DRAG_PROTO_XDND,
  GDK_DRAG_PROTO_ROOTWIN,	  /* A root window with nobody claiming
				   * drags */
  GDK_DRAG_PROTO_NONE,		  /* Not a valid drag window */
  GDK_DRAG_PROTO_WIN32_DROPFILES, /* The simple WM_DROPFILES dnd */
  GDK_DRAG_PROTO_OLE2,		  /* The complex OLE2 dnd (not implemented) */
  GDK_DRAG_PROTO_LOCAL            /* Intra-app */
} GdkDragProtocol;

/* Object that holds information about a drag in progress.
 * this is used on both source and destination sides.
 */


typedef struct _GdkDragContextClass GdkDragContextClass;

#define GDK_TYPE_DRAG_CONTEXT              (gdk_drag_context_get_type ())
#define GDK_DRAG_CONTEXT(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DRAG_CONTEXT, GdkDragContext))
#define GDK_DRAG_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_DRAG_CONTEXT, GdkDragContextClass))
#define GDK_IS_DRAG_CONTEXT(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_DRAG_CONTEXT))
#define GDK_IS_DRAG_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_DRAG_CONTEXT))
#define GDK_DRAG_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_DRAG_CONTEXT, GdkDragContextClass))

struct _GdkDragContext {
  GObject parent_instance;

  /*< public >*/
  
  GdkDragProtocol  (protocol);

  gboolean  (is_source);
  
  GdkWindow * (source_window);
  GdkWindow * (dest_window);

  GList * (targets);
  GdkDragAction  (actions);
  GdkDragAction  (suggested_action);
  GdkDragAction  (action);

  guint32  (start_time);

  /*< private >*/
  
  gpointer  (windowing_data);
};

struct _GdkDragContextClass {
  GObjectClass parent_class;

};

/* Drag and Drop */

GType            _T2_gdk_drag_context_get_type   (void) G_GNUC_CONST;
GType            _3T_gdk_drag_context_get_type   (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType            gdk_drag_context_get_type   (void) G_GNUC_CONST;
#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
GdkDragContext * SF(gdk_drag_context_new)        (void);
#endif

GList           *SF(gdk_drag_context_list_targets)         (GdkDragContext *context);
GdkDragAction    SF(gdk_drag_context_get_actions)          (GdkDragContext *context);
GdkDragAction    SF(gdk_drag_context_get_suggested_action) (GdkDragContext *context);
GdkDragAction    SF(gdk_drag_context_get_selected_action)  (GdkDragContext *context);

GdkWindow       *SF(gdk_drag_context_get_source_window)    (GdkDragContext *context);
GdkWindow       *SF(gdk_drag_context_get_dest_window)      (GdkDragContext *context);
GdkDragProtocol  SF(gdk_drag_context_get_protocol)         (GdkDragContext *context);


#ifndef GDK_DISABLE_DEPRECATED
void             SF(gdk_drag_context_ref)        (GdkDragContext *context);
void             SF(gdk_drag_context_unref)      (GdkDragContext *context);
#endif

/* Destination side */

void             SF(gdk_drag_status)        (GdkDragContext   *context,
				         GdkDragAction     action,
					 guint32           time_);
void             SF(gdk_drop_reply)         (GdkDragContext   *context,
					 gboolean          ok,
					 guint32           time_);
void             SF(gdk_drop_finish)        (GdkDragContext   *context,
					 gboolean          success,
					 guint32           time_);
GdkAtom          SF(gdk_drag_get_selection) (GdkDragContext   *context);

/* Source side */

GdkDragContext * SF(gdk_drag_begin)      (GdkWindow      *window,
				      GList          *targets);

GdkNativeWindow SF(gdk_drag_get_protocol_for_display) (GdkDisplay       *display,
						   GdkNativeWindow   xid,
						   GdkDragProtocol  *protocol);

void    SF(gdk_drag_find_window_for_screen)   (GdkDragContext   *context,
					   GdkWindow        *drag_window,
					   GdkScreen        *screen,
					   gint              x_root,
					   gint              y_root,
					   GdkWindow       **dest_window,
					   GdkDragProtocol  *protocol);

#ifndef GDK_MULTIHEAD_SAFE
#ifndef GDK_DISABLE_DEPRECATED
GdkNativeWindow SF(gdk_drag_get_protocol) (GdkNativeWindow   xid,
				       GdkDragProtocol  *protocol);

void    SF(gdk_drag_find_window)  (GdkDragContext   *context,
			       GdkWindow        *drag_window,
			       gint              x_root,
			       gint              y_root,
			       GdkWindow       **dest_window,
			       GdkDragProtocol  *protocol);
#endif /* GDK_DISABLE_DEPRECATED */
#endif /* GDK_MULTIHEAD_SAFE */

gboolean        SF(gdk_drag_motion)      (GdkDragContext *context,
				      GdkWindow      *dest_window,
				      GdkDragProtocol protocol,
				      gint            x_root, 
				      gint            y_root,
				      GdkDragAction   suggested_action,
				      GdkDragAction   possible_actions,
				      guint32         time_);
void            SF(gdk_drag_drop)        (GdkDragContext *context,
				      guint32         time_);
void            SF(gdk_drag_abort)       (GdkDragContext *context,
				      guint32         time_);
gboolean        SF(gdk_drag_drop_succeeded) (GdkDragContext *context);

G_END_DECLS

#endif /* __GDK_DND_H__ */
