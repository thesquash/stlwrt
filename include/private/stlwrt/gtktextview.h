
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtktextview.h Copyright (C) 2000 Red Hat, Inc.
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_TEXT_VIEW_H__
#define __GTK_TEXT_VIEW_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkcontainer.h>
#include <gtkimcontext.h>
#include <gtktextbuffer.h>
#include <gtkmenu.h>

G_BEGIN_DECLS

#define GTK_TYPE_TEXT_VIEW             (gtk_text_view_get_type ())
#define GTK_TEXT_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TEXT_VIEW, GtkTextView))
#define GTK_TEXT_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_VIEW, GtkTextViewClass))
#define GTK_IS_TEXT_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TEXT_VIEW))
#define GTK_IS_TEXT_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_VIEW))
#define GTK_TEXT_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_VIEW, GtkTextViewClass))

typedef enum
{
  GTK_TEXT_WINDOW_PRIVATE,
  GTK_TEXT_WINDOW_WIDGET,
  GTK_TEXT_WINDOW_TEXT,
  GTK_TEXT_WINDOW_LEFT,
  GTK_TEXT_WINDOW_RIGHT,
  GTK_TEXT_WINDOW_TOP,
  GTK_TEXT_WINDOW_BOTTOM
} GtkTextWindowType;

#define GTK_TEXT_VIEW_PRIORITY_VALIDATE (GDK_PRIORITY_REDRAW + 5)

typedef struct _GtkTextView GtkTextViewFat;
typedef struct _GtkTextView GtkTextViewThin;
typedef struct _GtkTextViewClass GtkTextViewClassFat;
typedef struct _GtkTextViewClass GtkTextViewClassThin;

/* Internal private types. */
typedef struct _GtkTextWindow GtkTextWindow;
typedef struct _GtkTextPendingScroll GtkTextPendingScroll;

struct _GtkTextView
{
  GtkContainer parent_instance;

  struct _GtkTextLayout * (layout);
  GtkTextBuffer * (buffer);

  guint  (selection_drag_handler);
  guint  (scroll_timeout);

  /* Default style settings */
  gint  (pixels_above_lines);
  gint  (pixels_below_lines);
  gint  (pixels_inside_wrap);
  GtkWrapMode  (wrap_mode);
  GtkJustification  (justify);
  gint  (left_margin);
  gint  (right_margin);
  gint  (indent);
  PangoTabArray * (tabs);
  guint  (editable) : 1;

  guint  (overwrite_mode) : 1;
  guint  (cursor_visible) : 1;

  /* if we have reset the IM since the last character entered */  
  guint  (need_im_reset) : 1;

  guint  (accepts_tab) : 1;

  guint  (width_changed) : 1;

  /* debug flag - means that we've validated onscreen since the
   * last "invalidate" signal from the layout
   */
  guint  (onscreen_validated) : 1;

  guint  (mouse_cursor_obscured) : 1;

  GtkTextWindow * (text_window);
  GtkTextWindow * (left_window);
  GtkTextWindow * (right_window);
  GtkTextWindow * (top_window);
  GtkTextWindow * (bottom_window);

  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);

  gint  (xoffset);         /* Offsets between widget coordinates and buffer coordinates */
  gint  (yoffset);
  gint  (width);           /* Width and height of the buffer */
  gint  (height);

  /* The virtual cursor position is normally the same as the
   * actual (strong) cursor position, except in two circumstances:
   *
   * a) When the cursor is moved vertically with the keyboard
   * b) When the text view is scrolled with the keyboard
   *
   * In case a), virtual_cursor_x is preserved, but not virtual_cursor_y
   * In case b), both virtual_cursor_x and virtual_cursor_y are preserved.
   */
  gint  (virtual_cursor_x);   /* -1 means use actual cursor position */
  gint  (virtual_cursor_y);   /* -1 means use actual cursor position */

  GtkTextMark * (first_para_mark); /* Mark at the beginning of the first onscreen paragraph */
  gint  (first_para_pixels);       /* Offset of top of screen in the first onscreen paragraph */

  GtkTextMark * (dnd_mark);
  guint  (blink_timeout);

  guint  (first_validate_idle);        /* Idle to revalidate onscreen portion, runs before resize */
  guint  (incremental_validate_idle);  /* Idle to revalidate offscreen portions, runs after redraw */

  GtkIMContext * (im_context);
  GtkWidget * (popup_menu);

  gint  (drag_start_x);
  gint  (drag_start_y);

  GSList * (children);

  GtkTextPendingScroll * (pending_scroll);

  gint  (pending_place_cursor_button);
};

struct _GtkTextViewClass
{
  GtkContainerClass parent_class;

  void (* set_scroll_adjustments)   (GtkTextView    *text_view,
                                     GtkAdjustment  *hadjustment,
                                     GtkAdjustment  *vadjustment);

  void (* populate_popup)           (GtkTextView    *text_view,
                                     GtkMenu        *menu);

  /* These are all RUN_ACTION signals for keybindings */

  /* move insertion point */
  void (* move_cursor) (GtkTextView    *text_view,
                        GtkMovementStep step,
                        gint            count,
                        gboolean        extend_selection);

  /* FIXME should be deprecated in favor of adding GTK_MOVEMENT_HORIZONTAL_PAGES
   * or something in GTK 2.2, was put in to avoid adding enum values during
   * the freeze.
   */
  void (* page_horizontally) (GtkTextView *text_view,
                              gint         count,
                              gboolean     extend_selection);

  /* move the "anchor" (what Emacs calls the mark) to the cursor position */
  void (* set_anchor)  (GtkTextView    *text_view);

  /* Edits */
  void (* insert_at_cursor)      (GtkTextView *text_view,
                                  const gchar *str);
  void (* delete_from_cursor)    (GtkTextView  *text_view,
                                  GtkDeleteType type,
                                  gint          count);
  void (* backspace)             (GtkTextView *text_view);

  /* cut copy paste */
  void (* cut_clipboard)   (GtkTextView *text_view);
  void (* copy_clipboard)  (GtkTextView *text_view);
  void (* paste_clipboard) (GtkTextView *text_view);
  /* overwrite */
  void (* toggle_overwrite) (GtkTextView *text_view);

  /* as of GTK+ 2.12 the "move-focus" signal has been moved to GtkWidget,
   * so this is merley a virtual function now. Overriding it in subclasses
   * continues to work though.
   */
  void (* move_focus)       (GtkTextView     *text_view,
                             GtkDirectionType direction);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
  void (*_gtk_reserved5) (void);
  void (*_gtk_reserved6) (void);
  void (*_gtk_reserved7) (void);
};

GType          _T2_gtk_text_view_get_type              (void) G_GNUC_CONST;
GType          _3T_gtk_text_view_get_type              (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType          gtk_text_view_get_type              (void) G_GNUC_CONST;
GtkWidget *    __gtk_text_view_new                   (void);
GtkWidget *    __gtk_text_view_new_with_buffer       (GtkTextBuffer *buffer);
void           __gtk_text_view_set_buffer            (GtkTextView   *text_view,
                                                    GtkTextBuffer *buffer);
GtkTextBuffer *__gtk_text_view_get_buffer            (GtkTextView   *text_view);
gboolean       __gtk_text_view_scroll_to_iter        (GtkTextView   *text_view,
                                                    GtkTextIter   *iter,
                                                    gdouble        within_margin,
                                                    gboolean       use_align,
                                                    gdouble        xalign,
                                                    gdouble        yalign);
void           __gtk_text_view_scroll_to_mark        (GtkTextView   *text_view,
                                                    GtkTextMark   *mark,
                                                    gdouble        within_margin,
                                                    gboolean       use_align,
                                                    gdouble        xalign,
                                                    gdouble        yalign);
void           __gtk_text_view_scroll_mark_onscreen  (GtkTextView   *text_view,
                                                    GtkTextMark   *mark);
gboolean       __gtk_text_view_move_mark_onscreen    (GtkTextView   *text_view,
                                                    GtkTextMark   *mark);
gboolean       __gtk_text_view_place_cursor_onscreen (GtkTextView   *text_view);

void           __gtk_text_view_get_visible_rect      (GtkTextView   *text_view,
                                                    GdkRectangle  *visible_rect);
void           __gtk_text_view_set_cursor_visible    (GtkTextView   *text_view,
                                                    gboolean       setting);
gboolean       __gtk_text_view_get_cursor_visible    (GtkTextView   *text_view);

void           __gtk_text_view_get_iter_location     (GtkTextView   *text_view,
                                                    const GtkTextIter *iter,
                                                    GdkRectangle  *location);
void           __gtk_text_view_get_iter_at_location  (GtkTextView   *text_view,
                                                    GtkTextIter   *iter,
                                                    gint           x,
                                                    gint           y);
void           __gtk_text_view_get_iter_at_position  (GtkTextView   *text_view,
                                                    GtkTextIter   *iter,
						    gint          *trailing,
                                                    gint           x,
                                                    gint           y);
void           __gtk_text_view_get_line_yrange       (GtkTextView       *text_view,
                                                    const GtkTextIter *iter,
                                                    gint              *y,
                                                    gint              *height);

void           __gtk_text_view_get_line_at_y         (GtkTextView       *text_view,
                                                    GtkTextIter       *target_iter,
                                                    gint               y,
                                                    gint              *line_top);

void __gtk_text_view_buffer_to_window_coords (GtkTextView       *text_view,
                                            GtkTextWindowType  win,
                                            gint               buffer_x,
                                            gint               buffer_y,
                                            gint              *window_x,
                                            gint              *window_y);
void __gtk_text_view_window_to_buffer_coords (GtkTextView       *text_view,
                                            GtkTextWindowType  win,
                                            gint               window_x,
                                            gint               window_y,
                                            gint              *buffer_x,
                                            gint              *buffer_y);

GtkAdjustment* __gtk_text_view_get_hadjustment (GtkTextView *text_view);
GtkAdjustment* __gtk_text_view_get_vadjustment (GtkTextView *text_view);

GdkWindow*        __gtk_text_view_get_window      (GtkTextView       *text_view,
                                                 GtkTextWindowType  win);
GtkTextWindowType __gtk_text_view_get_window_type (GtkTextView       *text_view,
                                                 GdkWindow         *window);

void __gtk_text_view_set_border_window_size (GtkTextView       *text_view,
                                           GtkTextWindowType  type,
                                           gint               size);
gint __gtk_text_view_get_border_window_size (GtkTextView       *text_view,
					   GtkTextWindowType  type);

gboolean __gtk_text_view_forward_display_line           (GtkTextView       *text_view,
                                                       GtkTextIter       *iter);
gboolean __gtk_text_view_backward_display_line          (GtkTextView       *text_view,
                                                       GtkTextIter       *iter);
gboolean __gtk_text_view_forward_display_line_end       (GtkTextView       *text_view,
                                                       GtkTextIter       *iter);
gboolean __gtk_text_view_backward_display_line_start    (GtkTextView       *text_view,
                                                       GtkTextIter       *iter);
gboolean __gtk_text_view_starts_display_line            (GtkTextView       *text_view,
                                                       const GtkTextIter *iter);
gboolean __gtk_text_view_move_visually                  (GtkTextView       *text_view,
                                                       GtkTextIter       *iter,
                                                       gint               count);

gboolean        __gtk_text_view_im_context_filter_keypress        (GtkTextView       *text_view,
                                                                 GdkEventKey       *event);
void            __gtk_text_view_reset_im_context                  (GtkTextView       *text_view);

/* Adding child widgets */
void __gtk_text_view_add_child_at_anchor (GtkTextView          *text_view,
                                        GtkWidget            *child,
                                        GtkTextChildAnchor   *anchor);

void __gtk_text_view_add_child_in_window (GtkTextView          *text_view,
                                        GtkWidget            *child,
                                        GtkTextWindowType     which_window,
                                        /* window coordinates */
                                        gint                  xpos,
                                        gint                  ypos);

void __gtk_text_view_move_child          (GtkTextView          *text_view,
                                        GtkWidget            *child,
                                        /* window coordinates */
                                        gint                  xpos,
                                        gint                  ypos);

/* Default style settings (fallbacks if no tag affects the property) */

void             __gtk_text_view_set_wrap_mode          (GtkTextView      *text_view,
                                                       GtkWrapMode       wrap_mode);
GtkWrapMode      __gtk_text_view_get_wrap_mode          (GtkTextView      *text_view);
void             __gtk_text_view_set_editable           (GtkTextView      *text_view,
                                                       gboolean          setting);
gboolean         __gtk_text_view_get_editable           (GtkTextView      *text_view);
void             __gtk_text_view_set_overwrite          (GtkTextView      *text_view,
						       gboolean          overwrite);
gboolean         __gtk_text_view_get_overwrite          (GtkTextView      *text_view);
void		 __gtk_text_view_set_accepts_tab        (GtkTextView	*text_view,
						       gboolean		 accepts_tab);
gboolean	 __gtk_text_view_get_accepts_tab        (GtkTextView	*text_view);
void             __gtk_text_view_set_pixels_above_lines (GtkTextView      *text_view,
                                                       gint              pixels_above_lines);
gint             __gtk_text_view_get_pixels_above_lines (GtkTextView      *text_view);
void             __gtk_text_view_set_pixels_below_lines (GtkTextView      *text_view,
                                                       gint              pixels_below_lines);
gint             __gtk_text_view_get_pixels_below_lines (GtkTextView      *text_view);
void             __gtk_text_view_set_pixels_inside_wrap (GtkTextView      *text_view,
                                                       gint              pixels_inside_wrap);
gint             __gtk_text_view_get_pixels_inside_wrap (GtkTextView      *text_view);
void             __gtk_text_view_set_justification      (GtkTextView      *text_view,
                                                       GtkJustification  justification);
GtkJustification __gtk_text_view_get_justification      (GtkTextView      *text_view);
void             __gtk_text_view_set_left_margin        (GtkTextView      *text_view,
                                                       gint              left_margin);
gint             __gtk_text_view_get_left_margin        (GtkTextView      *text_view);
void             __gtk_text_view_set_right_margin       (GtkTextView      *text_view,
                                                       gint              right_margin);
gint             __gtk_text_view_get_right_margin       (GtkTextView      *text_view);
void             __gtk_text_view_set_indent             (GtkTextView      *text_view,
                                                       gint              indent);
gint             __gtk_text_view_get_indent             (GtkTextView      *text_view);
void             __gtk_text_view_set_tabs               (GtkTextView      *text_view,
                                                       PangoTabArray    *tabs);
PangoTabArray*   __gtk_text_view_get_tabs               (GtkTextView      *text_view);

/* note that the return value of this changes with the theme */
GtkTextAttributes* __gtk_text_view_get_default_attributes (GtkTextView    *text_view);

G_END_DECLS

#endif /* __GTK_TEXT_VIEW_H__ */
