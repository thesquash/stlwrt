/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * Copyright (C) 2004-2006 Christian Hammond
 * Copyright (C) 2008 Cody Russell
 * Copyright (C) 2008 Red Hat, Inc.
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

#ifndef __GTK_ENTRY_H__
#define __GTK_ENTRY_H__

#include <gtkeditable.h>
#include <gtkimcontext.h>
#include <gtkmenu.h>
#include <gtkentrybuffer.h>
#include <gtkentrycompletion.h>
#include <gtkimage.h>
#include <gtkselection.h>


G_BEGIN_DECLS

#define GTK_TYPE_ENTRY                  (SF(gtk_entry_get_type) ())
#define GTK_ENTRY(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ENTRY, GtkEntry))
#define GTK_ENTRY_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ENTRY, GtkEntryClass))
#define GTK_IS_ENTRY(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ENTRY))
#define GTK_IS_ENTRY_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ENTRY))
#define GTK_ENTRY_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ENTRY, GtkEntryClass))

typedef enum
{
  GTK_ENTRY_ICON_PRIMARY,
  GTK_ENTRY_ICON_SECONDARY
} GtkEntryIconPosition;

typedef struct _GtkEntryClass  GtkEntryClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkEntry, gtk_entry, GtkWidget,
  gchar       * (text);                        /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */

  guint         (editable) : 1;
  guint         (visible)  : 1;
  guint         (overwrite_mode) : 1;
  guint         (in_drag) : 1;	            /* FIXME: Should be private?
                                                       Dragging within the selection */

  guint16       (text_length);                 /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */
  guint16       (text_max_length);             /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */

  /*< private >*/
  GdkWindow    * (text_area);
  GtkIMContext * (im_context);
  GtkWidget    * (popup_menu);

  gint           (current_pos);
  gint           (selection_bound);

  PangoLayout  * (cached_layout);

  guint          (cache_includes_preedit) : 1;
  guint          (need_im_reset)          : 1;
  guint          (has_frame)              : 1;
  guint          (activates_default)      : 1;
  guint          (cursor_visible)         : 1;
  guint          (in_click)               : 1; /* Flag so we don't select all when clicking in entry to focus in */
  guint          (is_cell_renderer)       : 1;
  guint          (editing_canceled)       : 1; /* Only used by GtkCellRendererText */ 
  guint          (mouse_cursor_obscured)  : 1;
  guint          (select_words)           : 1;
  guint          (select_lines)           : 1;
  guint          (resolved_dir)           : 4; /* PangoDirection */
  guint          (truncate_multiline)     : 1;

  guint          (button);
  guint          (blink_timeout);
  guint          (recompute_idle);
  gint           (scroll_offset);
  gint           (ascent);	                    /* font ascent in pango units  */
  gint           (descent);	            /* font descent in pango units */

  guint16        (x_text_size);	            /* allocated size, in bytes */
  guint16        (x_n_bytes);	            /* length in use, in bytes */

  guint16        (preedit_length);	            /* length of preedit string, in bytes */
  guint16        (preedit_cursor);	            /* offset of cursor within preedit string, in chars */

  gint           (dnd_position);		    /* In chars, -1 == no DND cursor */

  gint           (drag_start_x);
  gint           (drag_start_y);

  gunichar       (invisible_char);

  gint           (width_chars);
)

struct _GtkEntryPrivate 
{
  GtkEntryBuffer* buffer;

  gfloat xalign;
  gint insert_pos;
  guint blink_time;  /* time in msec the cursor has blinked since last user event */
  guint interior_focus          : 1;
  guint real_changed            : 1;
  guint invisible_char_set      : 1;
  guint caps_lock_warning       : 1;
  guint caps_lock_warning_shown : 1;
  guint change_count            : 8;
  guint progress_pulse_mode     : 1;
  guint progress_pulse_way_back : 1;

  gint focus_width;
  GtkShadowType shadow_type;

  gdouble progress_fraction;
  gdouble progress_pulse_fraction;
  gdouble progress_pulse_current;

  EntryIconInfo *icons[MAX_ICONS];
  gint icon_margin;
  gint start_x;
  gint start_y;

  gchar *im_module;
};

struct _GtkEntryClass
{
  GtkWidgetClass parent_class;

  /* Hook to customize right-click popup */
  void (* populate_popup)   (GtkEntry       *entry,
                             GtkMenu        *menu);

  /* Action signals
   */
  void (* activate)           (GtkEntry             *entry);
  void (* move_cursor)        (GtkEntry             *entry,
			       GtkMovementStep       step,
			       gint                  count,
			       gboolean              extend_selection);
  void (* insert_at_cursor)   (GtkEntry             *entry,
			       const gchar          *str);
  void (* delete_from_cursor) (GtkEntry             *entry,
			       GtkDeleteType         type,
			       gint                  count);
  void (* backspace)          (GtkEntry             *entry);
  void (* cut_clipboard)      (GtkEntry             *entry);
  void (* copy_clipboard)     (GtkEntry             *entry);
  void (* paste_clipboard)    (GtkEntry             *entry);
  void (* toggle_overwrite)   (GtkEntry             *entry);

  /* hook to add other objects beside the entry (like in GtkSpinButton) */
  void (* get_text_area_size) (GtkEntry       *entry,
			       gint           *x,
			       gint           *y,
			       gint           *width,
			       gint           *height);

  /* Padding for future expansion */
  void (*_gtk_reserved1)      (void);
  void (*_gtk_reserved2)      (void);
};


GtkWidget* SF(gtk_entry_new)            		(void);
GtkWidget* SF(gtk_entry_new_with_buffer)            (GtkEntryBuffer *buffer);

GtkEntryBuffer* SF(gtk_entry_get_buffer)            (GtkEntry       *entry);
void       SF(gtk_entry_set_buffer)                 (GtkEntry       *entry,
                                                 GtkEntryBuffer *buffer);

GdkWindow *SF(gtk_entry_get_text_window)            (GtkEntry      *entry);

void       SF(gtk_entry_set_visibility) 		(GtkEntry      *entry,
						 gboolean       visible);
gboolean   SF(gtk_entry_get_visibility)             (GtkEntry      *entry);

void       SF(gtk_entry_set_invisible_char)         (GtkEntry      *entry,
                                                 gunichar       ch);
gunichar   SF(gtk_entry_get_invisible_char)         (GtkEntry      *entry);
void       SF(gtk_entry_unset_invisible_char)       (GtkEntry      *entry);

void       SF(gtk_entry_set_has_frame)              (GtkEntry      *entry,
                                                 gboolean       setting);
gboolean   SF(gtk_entry_get_has_frame)              (GtkEntry      *entry);

void       SF(gtk_entry_set_inner_border)                (GtkEntry        *entry,
                                                      const GtkBorder *border);
const GtkBorder* SF(gtk_entry_get_inner_border)          (GtkEntry        *entry);

void       SF(gtk_entry_set_overwrite_mode)         (GtkEntry      *entry,
                                                 gboolean       overwrite);
gboolean   SF(gtk_entry_get_overwrite_mode)         (GtkEntry      *entry);

/* text is truncated if needed */
void       SF(gtk_entry_set_max_length) 		(GtkEntry      *entry,
						 gint           max);
gint       SF(gtk_entry_get_max_length)             (GtkEntry      *entry);
guint16    SF(gtk_entry_get_text_length)            (GtkEntry      *entry);

void       SF(gtk_entry_set_activates_default)      (GtkEntry      *entry,
                                                 gboolean       setting);
gboolean   SF(gtk_entry_get_activates_default)      (GtkEntry      *entry);

void       SF(gtk_entry_set_width_chars)            (GtkEntry      *entry,
                                                 gint           n_chars);
gint       SF(gtk_entry_get_width_chars)            (GtkEntry      *entry);

/* Somewhat more convenient than the GtkEditable generic functions
 */
void       SF(gtk_entry_set_text)                   (GtkEntry      *entry,
                                                 const gchar   *text);
/* returns a reference to the text */
const gchar* SF(gtk_entry_get_text)                 (GtkEntry      *entry);

PangoLayout* SF(gtk_entry_get_layout)               (GtkEntry      *entry);
void         SF(gtk_entry_get_layout_offsets)       (GtkEntry      *entry,
                                                 gint          *x,
                                                 gint          *y);
void       SF(gtk_entry_set_alignment)              (GtkEntry      *entry,
                                                 gfloat         xalign);
gfloat     SF(gtk_entry_get_alignment)              (GtkEntry      *entry);

void                SF(gtk_entry_set_completion) (GtkEntry           *entry,
                                              GtkEntryCompletion *completion);
GtkEntryCompletion *SF(gtk_entry_get_completion) (GtkEntry           *entry);

gint       SF(gtk_entry_layout_index_to_text_index) (GtkEntry      *entry,
                                                 gint           layout_index);
gint       SF(gtk_entry_text_index_to_layout_index) (GtkEntry      *entry,
                                                 gint           text_index);

/* For scrolling cursor appropriately
 */
void           SF(gtk_entry_set_cursor_hadjustment) (GtkEntry      *entry,
                                                 GtkAdjustment *adjustment);
GtkAdjustment* SF(gtk_entry_get_cursor_hadjustment) (GtkEntry      *entry);

/* Progress API
 */
void           SF(gtk_entry_set_progress_fraction)   (GtkEntry     *entry,
                                                  gdouble       fraction);
gdouble        SF(gtk_entry_get_progress_fraction)   (GtkEntry     *entry);

void           SF(gtk_entry_set_progress_pulse_step) (GtkEntry     *entry,
                                                  gdouble       fraction);
gdouble        SF(gtk_entry_get_progress_pulse_step) (GtkEntry     *entry);

void           SF(gtk_entry_progress_pulse)          (GtkEntry     *entry);

/* Setting and managing icons
 */
void           SF(gtk_entry_set_icon_from_pixbuf)            (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  GdkPixbuf            *pixbuf);
void           SF(gtk_entry_set_icon_from_stock)             (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  const gchar          *stock_id);
void           SF(gtk_entry_set_icon_from_icon_name)         (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  const gchar          *icon_name);
void           SF(gtk_entry_set_icon_from_gicon)             (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  GIcon                *icon);
GtkImageType SF(gtk_entry_get_icon_storage_type)             (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
GdkPixbuf*   SF(gtk_entry_get_icon_pixbuf)                   (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
const gchar* SF(gtk_entry_get_icon_stock)                    (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
const gchar* SF(gtk_entry_get_icon_name)                     (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
GIcon*       SF(gtk_entry_get_icon_gicon)                    (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
void         SF(gtk_entry_set_icon_activatable)              (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  gboolean              activatable);
gboolean     SF(gtk_entry_get_icon_activatable)              (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
void         SF(gtk_entry_set_icon_sensitive)                (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  gboolean              sensitive);
gboolean     SF(gtk_entry_get_icon_sensitive)                (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos);
gint         SF(gtk_entry_get_icon_at_pos)                   (GtkEntry             *entry,
							  gint                  x,
							  gint                  y);
void         SF(gtk_entry_set_icon_tooltip_text)             (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  const gchar          *tooltip);
gchar *      SF(gtk_entry_get_icon_tooltip_text)             (GtkEntry             *entry,
                                                          GtkEntryIconPosition  icon_pos);
void         SF(gtk_entry_set_icon_tooltip_markup)           (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  const gchar          *tooltip);
gchar *      SF(gtk_entry_get_icon_tooltip_markup)           (GtkEntry             *entry,
                                                          GtkEntryIconPosition  icon_pos);
void         SF(gtk_entry_set_icon_drag_source)              (GtkEntry             *entry,
							  GtkEntryIconPosition  icon_pos,
							  GtkTargetList        *target_list,
							  GdkDragAction         actions);
gint         SF(gtk_entry_get_current_icon_drag_source)      (GtkEntry             *entry);

GdkWindow  * SF(gtk_entry_get_icon_window)                   (GtkEntry             *entry,
                                                          GtkEntryIconPosition  icon_pos);

gboolean    SF(gtk_entry_im_context_filter_keypress)         (GtkEntry             *entry,
                                                          GdkEventKey          *event);
void        SF(gtk_entry_reset_im_context)                   (GtkEntry             *entry);


/* Deprecated compatibility functions
 */

#ifndef GTK_DISABLE_DEPRECATED
GtkWidget* SF(gtk_entry_new_with_max_length)	(gint           max);
void       SF(gtk_entry_append_text)    		(GtkEntry      *entry,
						 const gchar   *text);
void       SF(gtk_entry_prepend_text)   		(GtkEntry      *entry,
						 const gchar   *text);
void       SF(gtk_entry_set_position)   		(GtkEntry      *entry,
						 gint           position);
void       SF(gtk_entry_select_region)  		(GtkEntry      *entry,
						 gint           start,
						 gint           end);
void       SF(gtk_entry_set_editable)   		(GtkEntry      *entry,
						 gboolean       editable);
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_ENTRY_H__ */
