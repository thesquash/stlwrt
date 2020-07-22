
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

#ifndef __GTK_WINDOW_H__
#define __GTK_WINDOW_H__

#include <gtkaccelgroup.h>
#include <gtkbin.h>


G_BEGIN_DECLS

#define GTK_TYPE_WINDOW			(gtk_window_get_type ())
#define GTK_WINDOW(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_WINDOW, GtkWindow))
#define GTK_WINDOW_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_WINDOW, GtkWindowClass))
#define GTK_IS_WINDOW(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_WINDOW))
#define GTK_IS_WINDOW_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_WINDOW))
#define GTK_WINDOW_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_WINDOW, GtkWindowClass))



typedef struct _GtkWindowClass        GtkWindowClass;
typedef struct _GtkWindowGeometryInfo GtkWindowGeometryInfo;
typedef struct _GtkWindowGroup        GtkWindowGroup;
typedef struct _GtkWindowGroupClass   GtkWindowGroupClass;


struct _GtkWindowPrivate
{
  GtkMnemonicHash *mnemonic_hash;
  
  guint above_initially : 1;
  guint below_initially : 1;
  guint fullscreen_initially : 1;
  guint skips_taskbar : 1;
  guint skips_pager : 1;
  guint urgent : 1;
  guint accept_focus : 1;
  guint focus_on_map : 1;
  guint deletable : 1;
  guint transient_parent_group : 1;

  guint reset_type_hint : 1;
  guint opacity_set : 1;
  guint builder_visible : 1;

  guint mnemonics_visible : 1;
  guint mnemonics_visible_set : 1;

  GdkWindowTypeHint type_hint;

  gdouble opacity;

  gchar *startup_id;
};

/********************************************************************/
struct _GtkWindowInstanceProps
{


  gchar * (title);
  gchar * (wmclass_name);
  gchar * (wmclass_class);
  gchar * (wm_role);

  GtkWidget * (focus_widget);
  GtkWidget * (default_widget);
  GtkWindow * (transient_parent);
  GtkWindowGeometryInfo * (geometry_info);
  GdkWindow * (frame);
  GtkWindowGroup * (group);

  guint16  (configure_request_count);
  guint  (allow_shrink) : 1;
  guint  (allow_grow) : 1;
  guint  (configure_notify_received) : 1;
  /* The following flags are initially TRUE (before a window is mapped).
   * They cause us to compute a configure request that involves
   * default-only parameters. Once mapped, we set them to FALSE.
   * Then we set them to TRUE again on unmap (for position)
   * and on unrealize (for size).
   */
  guint  (need_default_position) : 1;
  guint  (need_default_size) : 1;
  guint  (position) : 3;
  guint  (type) : 4; /* GtkWindowType */ 
  guint  (has_user_ref_count) : 1;
  guint  (has_focus) : 1;

  guint  (modal) : 1;
  guint  (destroy_with_parent) : 1;
  
  guint  (has_frame) : 1;

  /* __gtk_window_iconify() called before realization */
  guint  (iconify_initially) : 1;
  guint  (stick_initially) : 1;
  guint  (maximize_initially) : 1;
  guint  (decorated) : 1;
  
  guint  (type_hint) : 3; /* GdkWindowTypeHint if the hint is one of the original eight. If not, then
				* it contains GDK_WINDOW_TYPE_HINT_NORMAL
				*/
  guint  (gravity) : 5; /* GdkGravity */

  guint  (is_active) : 1;
  guint  (has_toplevel_focus) : 1;
  
  guint  (frame_left);
  guint  (frame_top);
  guint  (frame_right);
  guint  (frame_bottom);

  guint  (keys_changed_handler);
  
  GdkModifierType  (mnemonic_modifier);
  GdkScreen      * (screen);
};

struct _GtkWindowFat
{
  GtkBinFat   bin;

  struct _GtkWindowInstanceProps instance_properties;
};

struct _GtkWindowThin
{
  GtkBinThin  bin;

  gpointer reserved;
};


typedef union
{
  struct _GtkWindowFat   fat_instance;
  struct _GtkWindowThin  thin_instance;
}   GtkWindow;
/********************************************************************/



struct _GtkWindowClass
{
  GtkBinClass parent_class;

  void     (* set_focus)   (GtkWindow *window,
			    GtkWidget *focus);
  gboolean (* frame_event) (GtkWindow *window,
			    GdkEvent  *event);

  /* G_SIGNAL_ACTION signals for keybindings */

  void     (* activate_focus)          (GtkWindow       *window);
  void     (* activate_default)        (GtkWindow       *window);

  /* as of GTK+ 2.12 the "move-focus" signal has been moved to GtkWidget,
   * so this is merley a virtual function now. Overriding it in subclasses
   * continues to work though.
   */
  void     (* move_focus)              (GtkWindow       *window,
                                        GtkDirectionType direction);
  
  void	   (*keys_changed)	       (GtkWindow	*window);
  
  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

#define GTK_TYPE_WINDOW_GROUP             (gtk_window_group_get_type ())
#define GTK_WINDOW_GROUP(object)          (G_TYPE_CHECK_INSTANCE_CAST ((object), GTK_TYPE_WINDOW_GROUP, GtkWindowGroup))
#define GTK_WINDOW_GROUP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_WINDOW_GROUP, GtkWindowGroupClass))
#define GTK_IS_WINDOW_GROUP(object)       (G_TYPE_CHECK_INSTANCE_TYPE ((object), GTK_TYPE_WINDOW_GROUP))
#define GTK_IS_WINDOW_GROUP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_WINDOW_GROUP))
#define GTK_WINDOW_GROUP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_WINDOW_GROUP, GtkWindowGroupClass))

/********************************************************************/
struct _GtkWindowGroupInstanceProps
{


  GSList * (grabs);
};

struct _GtkWindowGroupFat
{
  GObjectFat   parent_instance;

  struct _GtkWindowGroupInstanceProps instance_properties;
};

struct _GtkWindowGroupThin
{
  GObjectThin  parent_instance;

  gpointer reserved;
};


typedef union
{
  struct _GtkWindowGroupFat   fat_instance;
  struct _GtkWindowGroupThin  thin_instance;
}   GtkWindowGroup;
/********************************************************************/



struct _GtkWindowGroupClass
{
  GObjectClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

#ifdef G_OS_WIN32
/* Reserve old names for DLL ABI backward compatibility */
#define gtk_window_set_icon_from_file gtk_window_set_icon_from_file_utf8
#define gtk_window_set_default_icon_from_file gtk_window_set_default_icon_from_file_utf8
#endif

GType      _T2_gtk_window_get_type                 (void) G_GNUC_CONST;
GType      _3T_gtk_window_get_type                 (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_window_get_type                 (void) G_GNUC_CONST;
GtkWidget* __gtk_window_new                      (GtkWindowType        type);
void       __gtk_window_set_title                (GtkWindow           *window,
						const gchar         *title);
const gchar *__gtk_window_get_title              (GtkWindow           *window);
void       __gtk_window_set_wmclass              (GtkWindow           *window,
						const gchar         *wmclass_name,
						const gchar         *wmclass_class);
void       __gtk_window_set_role                 (GtkWindow           *window,
                                                const gchar         *role);
void       __gtk_window_set_startup_id           (GtkWindow           *window,
                                                const gchar         *startup_id);
const gchar *__gtk_window_get_role               (GtkWindow           *window);
void       __gtk_window_add_accel_group          (GtkWindow           *window,
						GtkAccelGroup	    *accel_group);
void       __gtk_window_remove_accel_group       (GtkWindow           *window,
						GtkAccelGroup	    *accel_group);
void       __gtk_window_set_position             (GtkWindow           *window,
						GtkWindowPosition    position);
gboolean   __gtk_window_activate_focus	       (GtkWindow           *window);
void       __gtk_window_set_focus                (GtkWindow           *window,
						GtkWidget           *focus);
GtkWidget *__gtk_window_get_focus                (GtkWindow           *window);
void       __gtk_window_set_default              (GtkWindow           *window,
						GtkWidget           *default_widget);
GtkWidget *__gtk_window_get_default_widget       (GtkWindow           *window);
gboolean   __gtk_window_activate_default	       (GtkWindow           *window);

void       __gtk_window_set_transient_for        (GtkWindow           *window, 
						GtkWindow           *parent);
GtkWindow *__gtk_window_get_transient_for        (GtkWindow           *window);
void       __gtk_window_set_opacity              (GtkWindow           *window, 
						gdouble              opacity);
gdouble    __gtk_window_get_opacity              (GtkWindow           *window);
void       __gtk_window_set_type_hint            (GtkWindow           *window, 
						GdkWindowTypeHint    hint);
GdkWindowTypeHint gtk_window_get_type_hint     (GtkWindow           *window);
void       __gtk_window_set_skip_taskbar_hint    (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_skip_taskbar_hint    (GtkWindow           *window);
void       __gtk_window_set_skip_pager_hint      (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_skip_pager_hint      (GtkWindow           *window);
void       __gtk_window_set_urgency_hint         (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_urgency_hint         (GtkWindow           *window);
void       __gtk_window_set_accept_focus         (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_accept_focus         (GtkWindow           *window);
void       __gtk_window_set_focus_on_map         (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_focus_on_map         (GtkWindow           *window);
void       __gtk_window_set_destroy_with_parent  (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_destroy_with_parent  (GtkWindow           *window);
void       __gtk_window_set_mnemonics_visible    (GtkWindow           *window,
                                                gboolean             setting);
gboolean   __gtk_window_get_mnemonics_visible    (GtkWindow           *window);

void       __gtk_window_set_resizable            (GtkWindow           *window,
                                                gboolean             resizable);
gboolean   __gtk_window_get_resizable            (GtkWindow           *window);

void       __gtk_window_set_gravity              (GtkWindow           *window,
                                                GdkGravity           gravity);
GdkGravity __gtk_window_get_gravity              (GtkWindow           *window);


void       __gtk_window_set_geometry_hints       (GtkWindow           *window,
						GtkWidget           *geometry_widget,
						GdkGeometry         *geometry,
						GdkWindowHints       geom_mask);

void	   __gtk_window_set_screen	       (GtkWindow	    *window,
						GdkScreen	    *screen);
GdkScreen* __gtk_window_get_screen	       (GtkWindow	    *window);

gboolean   __gtk_window_is_active                (GtkWindow           *window);
gboolean   __gtk_window_has_toplevel_focus       (GtkWindow           *window);


#ifndef GTK_DISABLE_DEPRECATED
/* __gtk_window_set_has_frame () must be called before realizing the window_*/
void       __gtk_window_set_has_frame            (GtkWindow *window, 
						gboolean   setting);
gboolean   __gtk_window_get_has_frame            (GtkWindow *window);
void       __gtk_window_set_frame_dimensions     (GtkWindow *window, 
						gint       left,
						gint       top,
						gint       right,
						gint       bottom);
void       __gtk_window_get_frame_dimensions     (GtkWindow *window, 
						gint      *left,
						gint      *top,
						gint      *right,
						gint      *bottom);
#endif
void       __gtk_window_set_decorated            (GtkWindow *window,
                                                gboolean   setting);
gboolean   __gtk_window_get_decorated            (GtkWindow *window);
void       __gtk_window_set_deletable            (GtkWindow *window,
                                                gboolean   setting);
gboolean   __gtk_window_get_deletable            (GtkWindow *window);

void       __gtk_window_set_icon_list                (GtkWindow  *window,
                                                    GList      *list);
GList*     __gtk_window_get_icon_list                (GtkWindow  *window);
void       __gtk_window_set_icon                     (GtkWindow  *window,
                                                    GdkPixbuf  *icon);
void       __gtk_window_set_icon_name                (GtkWindow   *window,
						    const gchar *name);
gboolean   __gtk_window_set_icon_from_file           (GtkWindow   *window,
						    const gchar *filename,
						    GError     **err);
GdkPixbuf* __gtk_window_get_icon                     (GtkWindow  *window);
const gchar *
           __gtk_window_get_icon_name                (GtkWindow  *window);
void       __gtk_window_set_default_icon_list        (GList      *list);
GList*     __gtk_window_get_default_icon_list        (void);
void       __gtk_window_set_default_icon             (GdkPixbuf  *icon);
void       __gtk_window_set_default_icon_name        (const gchar *name);
const gchar *
           __gtk_window_get_default_icon_name        (void);
gboolean   __gtk_window_set_default_icon_from_file   (const gchar *filename,
						    GError     **err);

void       __gtk_window_set_auto_startup_notification (gboolean setting);

/* If window is set modal, input will be grabbed when show and released when hide */
void       __gtk_window_set_modal      (GtkWindow *window,
				      gboolean   modal);
gboolean   __gtk_window_get_modal      (GtkWindow *window);
GList*     __gtk_window_list_toplevels (void);

void     __gtk_window_add_mnemonic          (GtkWindow       *window,
					   guint            keyval,
					   GtkWidget       *target);
void     __gtk_window_remove_mnemonic       (GtkWindow       *window,
					   guint            keyval,
					   GtkWidget       *target);
gboolean __gtk_window_mnemonic_activate     (GtkWindow       *window,
					   guint            keyval,
					   GdkModifierType  modifier);
void     __gtk_window_set_mnemonic_modifier (GtkWindow       *window,
					   GdkModifierType  modifier);
GdkModifierType __gtk_window_get_mnemonic_modifier (GtkWindow *window);

gboolean __gtk_window_activate_key          (GtkWindow        *window,
					   GdkEventKey      *event);
gboolean __gtk_window_propagate_key_event   (GtkWindow        *window,
					   GdkEventKey      *event);

void     __gtk_window_present            (GtkWindow *window);
void     __gtk_window_present_with_time  (GtkWindow *window,
				        guint32    timestamp);
void     __gtk_window_iconify       (GtkWindow *window);
void     __gtk_window_deiconify     (GtkWindow *window);
void     __gtk_window_stick         (GtkWindow *window);
void     __gtk_window_unstick       (GtkWindow *window);
void     __gtk_window_maximize      (GtkWindow *window);
void     __gtk_window_unmaximize    (GtkWindow *window);
void     __gtk_window_fullscreen    (GtkWindow *window);
void     __gtk_window_unfullscreen  (GtkWindow *window);
void     __gtk_window_set_keep_above    (GtkWindow *window, gboolean setting);
void     __gtk_window_set_keep_below    (GtkWindow *window, gboolean setting);

void __gtk_window_begin_resize_drag (GtkWindow     *window,
                                   GdkWindowEdge  edge,
                                   gint           button,
                                   gint           root_x,
                                   gint           root_y,
                                   guint32        timestamp);
void __gtk_window_begin_move_drag   (GtkWindow     *window,
                                   gint           button,
                                   gint           root_x,
                                   gint           root_y,
                                   guint32        timestamp);

#ifndef GTK_DISABLE_DEPRECATED
void       __gtk_window_set_policy               (GtkWindow           *window,
						gint                 allow_shrink,
						gint                 allow_grow,
						gint                 auto_shrink);
#define	gtk_window_position			gtk_window_set_position
#endif /* GTK_DISABLE_DEPRECATED */

/* Set initial default size of the window (does not constrain user
 * resize operations)
 */
void     __gtk_window_set_default_size (GtkWindow   *window,
                                      gint         width,
                                      gint         height);
void     __gtk_window_get_default_size (GtkWindow   *window,
                                      gint        *width,
                                      gint        *height);
void     __gtk_window_resize           (GtkWindow   *window,
                                      gint         width,
                                      gint         height);
void     __gtk_window_get_size         (GtkWindow   *window,
                                      gint        *width,
                                      gint        *height);
void     __gtk_window_move             (GtkWindow   *window,
                                      gint         x,
                                      gint         y);
void     __gtk_window_get_position     (GtkWindow   *window,
                                      gint        *root_x,
                                      gint        *root_y);
gboolean __gtk_window_parse_geometry   (GtkWindow   *window,
                                      const gchar *geometry);
GtkWindowGroup *__gtk_window_get_group (GtkWindow   *window);
gboolean __gtk_window_has_group        (GtkWindow   *window);

/* Ignore this unless you are writing a GUI builder */
void     __gtk_window_reshow_with_initial_size (GtkWindow *window);

GtkWindowType __gtk_window_get_window_type     (GtkWindow     *window);

/* Window groups
 */
GType            _T2_gtk_window_group_get_type      (void) G_GNUC_CONST;
GType            _3T_gtk_window_group_get_type      (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType            gtk_window_group_get_type      (void) G_GNUC_CONST;

GtkWindowGroup * __gtk_window_group_new           (void);
void             __gtk_window_group_add_window    (GtkWindowGroup     *window_group,
						 GtkWindow          *window);
void             __gtk_window_group_remove_window (GtkWindowGroup     *window_group,
					         GtkWindow          *window);
GList *          __gtk_window_group_list_windows  (GtkWindowGroup     *window_group);


/* --- internal functions --- */
void            ___gtk_window_internal_set_focus (GtkWindow *window,
						GtkWidget *focus);
void            __gtk_window_remove_embedded_xid (GtkWindow       *window,
						GdkNativeWindow  xid);
void            __gtk_window_add_embedded_xid    (GtkWindow       *window,
						GdkNativeWindow  xid);
void            ___gtk_window_reposition         (GtkWindow *window,
						gint       x,
						gint       y);
void            ___gtk_window_constrain_size     (GtkWindow *window,
						gint       width,
						gint       height,
						gint      *new_width,
						gint      *new_height);
GtkWidget      *__gtk_window_group_get_current_grab (GtkWindowGroup *window_group);

void            ___gtk_window_set_has_toplevel_focus (GtkWindow *window,
						    gboolean   has_toplevel_focus);
void            ___gtk_window_unset_focus_and_default (GtkWindow *window,
						     GtkWidget *widget);

void            ___gtk_window_set_is_active          (GtkWindow *window,
						    gboolean   is_active);

void            ___gtk_window_set_is_toplevel        (GtkWindow *window,
						    gboolean   is_toplevel);

typedef void (*GtkWindowKeysForeachFunc) (GtkWindow      *window,
					  guint           keyval,
					  GdkModifierType modifiers,
					  gboolean        is_mnemonic,
					  gpointer        data);

void ___gtk_window_keys_foreach (GtkWindow               *window,
			       GtkWindowKeysForeachFunc func,
			       gpointer                 func_data);

/* --- internal (GtkAcceleratable) --- */
gboolean	___gtk_window_query_nonaccels	(GtkWindow	*window,
						 guint		 accel_key,
						 GdkModifierType accel_mods);

G_END_DECLS

#endif /* __GTK_WINDOW_H__ */
