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

#ifndef __GDK_WINDOW_H__
#define __GDK_WINDOW_H__

#include <stlwrt.h>


#include <gdkdrawable.h>

#include <gdktypes.h>

#include <gdkevents.h>

G_BEGIN_DECLS

typedef struct _GdkGeometry          GdkGeometry;
typedef struct _GdkWindowAttr        GdkWindowAttr;
typedef struct _GdkPointerHooks      GdkPointerHooks;
typedef struct _GdkWindowRedirect    GdkWindowRedirect;

struct _GdkWindowAttr
{
  gchar *title;
  gint event_mask;
  gint x, y;
  gint width;
  gint height;
  GdkWindowWindowClass wclass;
  GdkVisual *visual;
  GdkColormap *colormap;
  GdkWindowType window_type;
  GdkCursor *cursor;
  gchar *wmclass_name;
  gchar *wmclass_class;
  gboolean override_redirect;
  GdkWindowTypeHint type_hint;
};

struct _GdkGeometry
{
  gint min_width;
  gint min_height;
  gint max_width;
  gint max_height;
  gint base_width;
  gint base_height;
  gint width_inc;
  gint height_inc;
  gdouble min_aspect;
  gdouble max_aspect;
  GdkGravity win_gravity;
};

struct _GdkPointerHooks 
{
  GdkWindow* (*get_pointer)       (GdkWindow	   *window,
			           gint	           *x,
			           gint   	   *y,
			           GdkModifierType *mask);
  GdkWindow* (*window_at_pointer) (GdkScreen       *screen, /* unused */
                                   gint            *win_x,
                                   gint            *win_y);
};

typedef struct _GdkWindow GdkWindow;
typedef struct _GdkWindowClass GdkWindowClass;

#define GDK_TYPE_WINDOW              (gdk_window_get_type ())
#define GDK_WINDOW(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WINDOW, GdkWindow))
#define GDK_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WINDOW, GdkWindowClass))
#define GDK_IS_WINDOW(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WINDOW))
#define GDK_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WINDOW))
#define GDK_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WINDOW, GdkWindowClass))

#ifndef STLWRT_COMPILATION

/* We used to export all of GdkWindow, but we don't want to keep doing so.
   However, there are various parts of it accessed by macros and other code,
   so we keep the old exported version public, but in reality it is larger. */

/**** DON'T CHANGE THIS STRUCT, the real version is in gdkinternals.h ****/
/********************************************************************/
STLWRT_DECLARE_FTYPE_FPARENT(GdkWindow, gdk_window, GdkDrawable,
  GdkDrawable * (impl); /* window-system-specific delegate object */
  
  GdkWindow * (parent);

  gpointer  (user_data);

  gint  (x);
  gint  (y);
  
  gint  (extension_events);

  GList * (filters);
  GList * (children);

  GdkColor  (bg_color);
  GdkPixmap * (bg_pixmap);
  
  GSList * (paint_stack);
  
  GdkRegion * (update_area);
  guint  (update_freeze_count);
  
  guint8  (window_type);
  guint8  (depth);
  guint8  (resize_count);

  GdkWindowState  (state);
  
  guint  (guffaw_gravity) : 1;
  guint  (input_only) : 1;
  guint  (modal_hint) : 1;
  guint  (composited) : 1;
  
  guint  (destroyed) : 2;

  guint  (accept_focus) : 1;
  guint  (focus_on_map) : 1;
  guint  (shaped) : 1;
  
  GdkEventMask  (event_mask);

  guint  (update_and_descendants_freeze_count);

  GdkWindowRedirect * (redirect);
)

#endif

struct _GdkWindowClass
{
  GdkDrawableClass parent_class;
};

/* Windows
 */
GdkWindow*    SF(gdk_window_new)                   (GdkWindow     *parent,
                                                GdkWindowAttr *attributes,
                                                gint           attributes_mask);
void          SF(gdk_window_destroy)               (GdkWindow     *window);
GdkWindowType SF(gdk_window_get_window_type)       (GdkWindow     *window);
gboolean      SF(gdk_window_is_destroyed)          (GdkWindow     *window);

GdkScreen*    SF(gdk_window_get_screen)            (GdkWindow     *window);
GdkDisplay*   SF(gdk_window_get_display)           (GdkWindow     *window);
GdkVisual*    SF(gdk_window_get_visual)            (GdkWindow     *window);
int           SF(gdk_window_get_width)             (GdkWindow     *window);
int           SF(gdk_window_get_height)            (GdkWindow     *window);

GdkWindow*    SF(gdk_window_at_pointer)            (gint          *win_x,
                                                gint          *win_y);
void          SF(gdk_window_show)                  (GdkWindow     *window);
void          SF(gdk_window_hide)                  (GdkWindow     *window);
void          SF(gdk_window_withdraw)              (GdkWindow     *window);
void          SF(gdk_window_show_unraised)         (GdkWindow     *window);
void          SF(gdk_window_move)                  (GdkWindow     *window,
                                                gint           x,
                                                gint           y);
void          SF(gdk_window_resize)                (GdkWindow     *window,
                                                gint           width,
                                                gint           height);
void          SF(gdk_window_move_resize)           (GdkWindow     *window,
                                                gint           x,
                                                gint           y,
                                                gint           width,
                                                gint           height);
void          SF(gdk_window_reparent)              (GdkWindow     *window,
                                                GdkWindow     *new_parent,
                                                gint           x,
                                                gint           y);
void          SF(gdk_window_clear)                 (GdkWindow     *window);
void          SF(gdk_window_clear_area)            (GdkWindow     *window,
                                                gint           x,
                                                gint           y,
                                                gint           width,
                                                gint           height);
void          SF(gdk_window_clear_area_e)          (GdkWindow     *window,
                                                gint           x,
                                                gint           y,
                                                gint           width,
                                                gint           height);
void          SF(gdk_window_raise)                 (GdkWindow     *window);
void          SF(gdk_window_lower)                 (GdkWindow     *window);
void          SF(gdk_window_restack)               (GdkWindow     *window,
						GdkWindow     *sibling,
						gboolean       above);
void          SF(gdk_window_focus)                 (GdkWindow     *window,
                                                guint32        timestamp);
void          SF(gdk_window_set_user_data)         (GdkWindow     *window,
                                                gpointer       user_data);
void          SF(gdk_window_set_override_redirect) (GdkWindow     *window,
                                                gboolean       override_redirect);
gboolean      SF(gdk_window_get_accept_focus)      (GdkWindow     *window);
void          SF(gdk_window_set_accept_focus)      (GdkWindow     *window,
					        gboolean       accept_focus);
gboolean      SF(gdk_window_get_focus_on_map)      (GdkWindow     *window);
void          SF(gdk_window_set_focus_on_map)      (GdkWindow     *window,
					        gboolean       focus_on_map);
void          SF(gdk_window_add_filter)            (GdkWindow     *window,
                                                GdkFilterFunc  function,
                                                gpointer       data);
void          SF(gdk_window_remove_filter)         (GdkWindow     *window,
                                                GdkFilterFunc  function,
                                                gpointer       data);
void          SF(gdk_window_scroll)                (GdkWindow     *window,
                                                gint           dx,
                                                gint           dy);
void	      SF(gdk_window_move_region)           (GdkWindow       *window,
						const GdkRegion *region,
						gint             dx,
						gint             dy);
gboolean      SF(gdk_window_ensure_native)        (GdkWindow       *window);

/* 
 * This allows for making shaped (partially transparent) windows
 * - cool feature, needed for Drag and Drag for example.
 *  The shape_mask can be the mask
 *  from gdk_pixmap_create_from_xpm.   Stefan Wille
 */
void SF(gdk_window_shape_combine_mask)  (GdkWindow	      *window,
                                     GdkBitmap	      *mask,
                                     gint	       x,
                                     gint	       y);
void SF(gdk_window_shape_combine_region) (GdkWindow	      *window,
                                      const GdkRegion *shape_region,
                                      gint	       offset_x,
                                      gint	       offset_y);

/*
 * This routine allows you to quickly take the shapes of all the child windows
 * of a window and use their shapes as the shape mask for this window - useful
 * for container windows that dont want to look like a big box
 * 
 * - Raster
 */
void SF(gdk_window_set_child_shapes) (GdkWindow *window);

gboolean SF(gdk_window_get_composited) (GdkWindow *window);
void SF(gdk_window_set_composited)   (GdkWindow *window,
                                  gboolean   composited);

/*
 * This routine allows you to merge (ie ADD) child shapes to your
 * own window's shape keeping its current shape and ADDING the child
 * shapes to it.
 * 
 * - Raster
 */
void SF(gdk_window_merge_child_shapes)         (GdkWindow       *window);

void SF(gdk_window_input_shape_combine_mask)   (GdkWindow       *window,
					    GdkBitmap       *mask,
					    gint             x,
					    gint             y);
void SF(gdk_window_input_shape_combine_region) (GdkWindow       *window,
                                            const GdkRegion *shape_region,
                                            gint             offset_x,
                                            gint             offset_y);
void SF(gdk_window_set_child_input_shapes)     (GdkWindow       *window);
void SF(gdk_window_merge_child_input_shapes)   (GdkWindow       *window);


/*
 * Check if a window has been shown, and whether all its
 * parents up to a toplevel have been shown, respectively.
 * Note that a window that is_viewable below is not necessarily
 * viewable in the X sense.
 */
gboolean SF(gdk_window_is_visible)     (GdkWindow *window);
gboolean SF(gdk_window_is_viewable)    (GdkWindow *window);
gboolean SF(gdk_window_is_input_only)  (GdkWindow *window);
gboolean SF(gdk_window_is_shaped)      (GdkWindow *window);

GdkWindowState SF(gdk_window_get_state) (GdkWindow *window);

/* Set static bit gravity on the parent, and static
 * window gravity on all children.
 */
gboolean SF(gdk_window_set_static_gravities) (GdkWindow *window,
					  gboolean   use_static);   

/* Functions to create/lookup windows from their native equivalents */ 
#if !defined(GDK_DISABLE_DEPRECATED) || defined(STLWRT_COMPILATION)
#ifndef GDK_MULTIHEAD_SAFE
GdkWindow*    SF(gdk_window_foreign_new) (GdkNativeWindow anid);
GdkWindow*    SF(gdk_window_lookup)      (GdkNativeWindow anid);
#endif
GdkWindow    *SF(gdk_window_foreign_new_for_display) (GdkDisplay      *display,
						  GdkNativeWindow  anid);
GdkWindow*    SF(gdk_window_lookup_for_display) (GdkDisplay      *display,
					     GdkNativeWindow  anid);
#endif


/* GdkWindow */

gboolean      SF(gdk_window_has_native)      (GdkWindow       *window);
#ifndef GDK_DISABLE_DEPRECATED
void	      SF(gdk_window_set_hints)	 (GdkWindow	  *window,
					  gint		   x,
					  gint		   y,
					  gint		   min_width,
					  gint		   min_height,
					  gint		   max_width,
					  gint		   max_height,
					  gint		   flags);
#endif
void              SF(gdk_window_set_type_hint) (GdkWindow        *window,
                                            GdkWindowTypeHint hint);
GdkWindowTypeHint SF(gdk_window_get_type_hint) (GdkWindow        *window);

gboolean      SF(gdk_window_get_modal_hint)   (GdkWindow       *window);
void          SF(gdk_window_set_modal_hint)   (GdkWindow       *window,
                                           gboolean         modal);

void SF(gdk_window_set_skip_taskbar_hint) (GdkWindow *window,
                                       gboolean   skips_taskbar);
void SF(gdk_window_set_skip_pager_hint)   (GdkWindow *window,
                                       gboolean   skips_pager);
void SF(gdk_window_set_urgency_hint)      (GdkWindow *window,
				       gboolean   urgent);

void          SF(gdk_window_set_geometry_hints) (GdkWindow          *window,
					     const GdkGeometry  *geometry,
					     GdkWindowHints      geom_mask);
#if !defined(GDK_DISABLE_DEPRECATED) || defined(STLWRT_COMPILATION)
void          SF(gdk_set_sm_client_id)          (const gchar        *sm_client_id);
#endif

void	      SF(gdk_window_begin_paint_rect)   (GdkWindow          *window,
					     const GdkRectangle *rectangle);
void	      SF(gdk_window_begin_paint_region) (GdkWindow          *window,
					     const GdkRegion    *region);
void	      SF(gdk_window_end_paint)          (GdkWindow          *window);
void	      SF(gdk_window_flush)             (GdkWindow          *window);

void	      SF(gdk_window_set_title)	   (GdkWindow	  *window,
					    const gchar	  *title);
void          SF(gdk_window_set_role)          (GdkWindow     *window,
					    const gchar   *role);
void          SF(gdk_window_set_startup_id)    (GdkWindow     *window,
					    const gchar   *startup_id);
void          SF(gdk_window_set_transient_for) (GdkWindow     *window,
					    GdkWindow     *parent);
void	      SF(gdk_window_set_background)	 (GdkWindow	  *window,
					  const GdkColor  *color);
void	      SF(gdk_window_set_back_pixmap) (GdkWindow	  *window,
					  GdkPixmap	  *pixmap,
					  gboolean	   parent_relative);
cairo_pattern_t *SF(gdk_window_get_background_pattern) (GdkWindow     *window);

void	      SF(gdk_window_set_cursor)	 (GdkWindow	  *window,
					  GdkCursor	  *cursor);
GdkCursor    *SF(gdk_window_get_cursor)      (GdkWindow       *window);
void	      SF(gdk_window_get_user_data)	 (GdkWindow	  *window,
					  gpointer	  *data);
void	      SF(gdk_window_get_geometry)	 (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y,
					  gint		  *width,
					  gint		  *height,
					  gint		  *depth);
void	      SF(gdk_window_get_position)	 (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y);
gint	      SF(gdk_window_get_origin)	 (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y);
void	      SF(gdk_window_get_root_coords) (GdkWindow	  *window,
					  gint             x,
					  gint             y,
					  gint		  *root_x,
					  gint		  *root_y);
void       SF(gdk_window_coords_to_parent)   (GdkWindow       *window,
                                          gdouble          x,
                                          gdouble          y,
                                          gdouble         *parent_x,
                                          gdouble         *parent_y);
void       SF(gdk_window_coords_from_parent) (GdkWindow       *window,
                                          gdouble          parent_x,
                                          gdouble          parent_y,
                                          gdouble         *x,
                                          gdouble         *y);

#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
/* Used by gtk_handle_box_button_changed () */
gboolean      SF(gdk_window_get_deskrelative_origin) (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y);
#endif

void	      SF(gdk_window_get_root_origin) (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y);
void          SF(gdk_window_get_frame_extents) (GdkWindow     *window,
                                            GdkRectangle  *rect);
GdkWindow*    SF(gdk_window_get_pointer)	 (GdkWindow	  *window,
					  gint		  *x,
					  gint		  *y,
					  GdkModifierType *mask);
GdkWindow *   SF(gdk_window_get_parent)      (GdkWindow       *window);
GdkWindow *   SF(gdk_window_get_toplevel)    (GdkWindow       *window);

GdkWindow *   SF(gdk_window_get_effective_parent)   (GdkWindow *window);
GdkWindow *   SF(gdk_window_get_effective_toplevel) (GdkWindow *window);

GList *	      SF(gdk_window_get_children)	 (GdkWindow	  *window);
GList *       SF(gdk_window_peek_children)   (GdkWindow       *window);
GdkEventMask  SF(gdk_window_get_events)	 (GdkWindow	  *window);
void	      SF(gdk_window_set_events)	 (GdkWindow	  *window,
					  GdkEventMask	   event_mask);

void          SF(gdk_window_set_icon_list)   (GdkWindow       *window,
					  GList           *pixbufs);
void	      SF(gdk_window_set_icon)	 (GdkWindow	  *window, 
					  GdkWindow	  *icon_window,
					  GdkPixmap	  *pixmap,
					  GdkBitmap	  *mask);
void	      SF(gdk_window_set_icon_name)	 (GdkWindow	  *window, 
					  const gchar	  *name);
void	      SF(gdk_window_set_group)	 (GdkWindow	  *window, 
					  GdkWindow	  *leader);
GdkWindow*    SF(gdk_window_get_group)	 (GdkWindow	  *window);
void	      SF(gdk_window_set_decorations) (GdkWindow	  *window,
					  GdkWMDecoration  decorations);
gboolean      SF(gdk_window_get_decorations) (GdkWindow       *window,
					  GdkWMDecoration *decorations);
void	      SF(gdk_window_set_functions)	 (GdkWindow	  *window,
					  GdkWMFunction	   functions);
#if !defined(GDK_MULTIHEAD_SAFE) && !defined(GDK_DISABLE_DEPRECATED)
GList *       SF(gdk_window_get_toplevels)   (void);
#endif

cairo_surface_t *
              SF(gdk_window_create_similar_surface) (GdkWindow *window,
                                          cairo_content_t  content,
                                          int              width,
                                          int              height);

void          SF(gdk_window_beep)            (GdkWindow       *window);
void          SF(gdk_window_iconify)         (GdkWindow       *window);
void          SF(gdk_window_deiconify)       (GdkWindow       *window);
void          SF(gdk_window_stick)           (GdkWindow       *window);
void          SF(gdk_window_unstick)         (GdkWindow       *window);
void          SF(gdk_window_maximize)        (GdkWindow       *window);
void          SF(gdk_window_unmaximize)      (GdkWindow       *window);
void          SF(gdk_window_fullscreen)      (GdkWindow       *window);
void          SF(gdk_window_unfullscreen)    (GdkWindow       *window);
void          SF(gdk_window_set_keep_above)  (GdkWindow       *window,
                                          gboolean         setting);
void          SF(gdk_window_set_keep_below)  (GdkWindow       *window,
                                          gboolean         setting);
void          SF(gdk_window_set_opacity)     (GdkWindow       *window,
                                          gdouble          opacity);
void          SF(gdk_window_register_dnd)    (GdkWindow       *window);

void SF(gdk_window_begin_resize_drag) (GdkWindow     *window,
                                   GdkWindowEdge  edge,
                                   gint           button,
                                   gint           root_x,
                                   gint           root_y,
                                   guint32        timestamp);
void SF(gdk_window_begin_move_drag)   (GdkWindow     *window,
                                   gint           button,
                                   gint           root_x,
                                   gint           root_y,
                                   guint32        timestamp);

/* Interface for dirty-region queueing */
void       SF(gdk_window_invalidate_rect)           (GdkWindow          *window,
					         const GdkRectangle *rect,
					         gboolean            invalidate_children);
void       SF(gdk_window_invalidate_region)         (GdkWindow          *window,
					         const GdkRegion    *region,
					         gboolean            invalidate_children);
void       SF(gdk_window_invalidate_maybe_recurse)  (GdkWindow          *window,
						 const GdkRegion    *region,
						 gboolean (*child_func) (GdkWindow *, gpointer),
						 gpointer   user_data);
GdkRegion *SF(gdk_window_get_update_area)     (GdkWindow    *window);

void       SF(gdk_window_freeze_updates)      (GdkWindow    *window);
void       SF(gdk_window_thaw_updates)        (GdkWindow    *window);

void       SF(gdk_window_freeze_toplevel_updates_libgtk_only) (GdkWindow *window);
void       SF(gdk_window_thaw_toplevel_updates_libgtk_only)   (GdkWindow *window);

void       SF(gdk_window_process_all_updates) (void);
void       SF(gdk_window_process_updates)     (GdkWindow    *window,
					   gboolean      update_children);

/* Enable/disable flicker, so you can tell if your code is inefficient. */
void       SF(gdk_window_set_debug_updates)   (gboolean      setting);

void       SF(gdk_window_constrain_size)      (GdkGeometry  *geometry,
                                           guint         flags,
                                           gint          width,
                                           gint          height,
                                           gint         *new_width,
                                           gint         *new_height);

void SF(gdk_window_get_internal_paint_info) (GdkWindow    *window,
					 GdkDrawable **real_drawable,
					 gint         *x_offset,
					 gint         *y_offset);

void SF(gdk_window_enable_synchronized_configure) (GdkWindow *window);
void SF(gdk_window_configure_finished)            (GdkWindow *window);

GdkWindow *SF(gdk_get_default_root_window) (void);

/* Offscreen redirection */
GdkPixmap *SF(gdk_offscreen_window_get_pixmap)     (GdkWindow     *window);
void       SF(gdk_offscreen_window_set_embedder)   (GdkWindow     *window,
						GdkWindow     *embedder);
GdkWindow *SF(gdk_offscreen_window_get_embedder)   (GdkWindow     *window);
void       SF(gdk_window_geometry_changed)         (GdkWindow     *window);

void       SF(gdk_window_redirect_to_drawable)   (GdkWindow     *window,
                                              GdkDrawable   *drawable,
                                              gint           src_x,
                                              gint           src_y,
                                              gint           dest_x,
                                              gint           dest_y,
                                              gint           width,
                                              gint           height);
void       SF(gdk_window_remove_redirection)     (GdkWindow     *window);

G_END_DECLS

#endif /* __GDK_WINDOW_H__ */
