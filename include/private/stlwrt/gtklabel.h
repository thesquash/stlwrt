
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_LABEL_H__
#define __GTK_LABEL_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkmisc.h>
#include <gtkwindow.h>
#include <gtkmenu.h>


G_BEGIN_DECLS

#define GTK_TYPE_LABEL		  (gtk_label_get_type ())
#define GTK_LABEL(obj)		  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LABEL, GtkLabel))
#define GTK_LABEL_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LABEL, GtkLabelClass))
#define GTK_IS_LABEL(obj)	  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LABEL))
#define GTK_IS_LABEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LABEL))
#define GTK_LABEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LABEL, GtkLabelClass))


typedef struct _GtkLabel       GtkLabelFat;
typedef struct _GtkLabel       GtkLabelSkinny;
typedef struct _GtkLabelClass  GtkLabelClassFat;
typedef struct _GtkLabelClass  GtkLabelClassSkinny;

typedef struct _GtkLabelSelectionInfo GtkLabelSelectionInfo;

struct _GtkLabel
{
  GtkMisc misc;

  /*< private >*/
  gchar  * (label);
  guint    (jtype)            : 2;
  guint    (wrap)             : 1;
  guint    (use_underline)    : 1;
  guint    (use_markup)       : 1;
  guint    (ellipsize)        : 3;
  guint    (single_line_mode) : 1;
  guint    (have_transform)   : 1;
  guint    (in_click)         : 1;
  guint    (wrap_mode)        : 3;
  guint    (pattern_set)      : 1;
  guint    (track_links)      : 1;

  guint    (mnemonic_keyval);

  gchar  * (text);
  PangoAttrList * (attrs);
  PangoAttrList * (effective_attrs);

  PangoLayout * (layout);

  GtkWidget * (mnemonic_widget);
  GtkWindow * (mnemonic_window);

  GtkLabelSelectionInfo * (select_info);
};

struct _GtkLabelClass
{
  GtkMiscClass parent_class;

  void (* move_cursor)     (GtkLabel       *label,
			    GtkMovementStep step,
			    gint            count,
			    gboolean        extend_selection);
  void (* copy_clipboard)  (GtkLabel       *label);

  /* Hook to customize right-click popup for selectable labels */
  void (* populate_popup)   (GtkLabel       *label,
                             GtkMenu        *menu);

  gboolean (*activate_link) (GtkLabel       *label,
                             const gchar    *uri);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
};

GType                 _T2_gtk_label_get_type          (void) G_GNUC_CONST;
GType                 _3T_gtk_label_get_type          (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType                 gtk_label_get_type          (void) G_GNUC_CONST;
GtkWidget*            __gtk_label_new               (const gchar   *str);
GtkWidget*            __gtk_label_new_with_mnemonic (const gchar   *str);
void                  __gtk_label_set_text          (GtkLabel      *label,
						   const gchar   *str);
const gchar *         __gtk_label_get_text          (GtkLabel      *label);
void                  __gtk_label_set_attributes    (GtkLabel      *label,
						   PangoAttrList *attrs);
PangoAttrList        *__gtk_label_get_attributes    (GtkLabel      *label);
void                  __gtk_label_set_label         (GtkLabel      *label,
						   const gchar   *str);
const gchar *         __gtk_label_get_label         (GtkLabel      *label);
void                  __gtk_label_set_markup        (GtkLabel      *label,
						   const gchar   *str);
void                  __gtk_label_set_use_markup    (GtkLabel      *label,
						   gboolean       setting);
gboolean              __gtk_label_get_use_markup    (GtkLabel      *label);
void                  __gtk_label_set_use_underline (GtkLabel      *label,
						   gboolean       setting);
gboolean              __gtk_label_get_use_underline (GtkLabel      *label);

void     __gtk_label_set_markup_with_mnemonic       (GtkLabel         *label,
						   const gchar      *str);
guint    __gtk_label_get_mnemonic_keyval            (GtkLabel         *label);
void     __gtk_label_set_mnemonic_widget            (GtkLabel         *label,
						   GtkWidget        *widget);
GtkWidget *__gtk_label_get_mnemonic_widget          (GtkLabel         *label);
void     __gtk_label_set_text_with_mnemonic         (GtkLabel         *label,
						   const gchar      *str);
void     __gtk_label_set_justify                    (GtkLabel         *label,
						   GtkJustification  jtype);
GtkJustification __gtk_label_get_justify            (GtkLabel         *label);
void     __gtk_label_set_ellipsize		  (GtkLabel         *label,
						   PangoEllipsizeMode mode);
PangoEllipsizeMode __gtk_label_get_ellipsize        (GtkLabel         *label);
void     __gtk_label_set_width_chars		  (GtkLabel         *label,
						   gint              n_chars);
gint     __gtk_label_get_width_chars                (GtkLabel         *label);
void     __gtk_label_set_max_width_chars    	  (GtkLabel         *label,
					  	   gint              n_chars);
gint     __gtk_label_get_max_width_chars  	  (GtkLabel         *label);
void     __gtk_label_set_pattern                    (GtkLabel         *label,
						   const gchar      *pattern);
void     __gtk_label_set_line_wrap                  (GtkLabel         *label,
						   gboolean          wrap);
gboolean __gtk_label_get_line_wrap                  (GtkLabel         *label);
void     __gtk_label_set_line_wrap_mode             (GtkLabel         *label,
						   PangoWrapMode     wrap_mode);
PangoWrapMode __gtk_label_get_line_wrap_mode        (GtkLabel         *label);
void     __gtk_label_set_selectable                 (GtkLabel         *label,
						   gboolean          setting);
gboolean __gtk_label_get_selectable                 (GtkLabel         *label);
void     __gtk_label_set_angle                      (GtkLabel         *label,
						   gdouble           angle);
gdouble  __gtk_label_get_angle                      (GtkLabel         *label);
void     __gtk_label_select_region                  (GtkLabel         *label,
						   gint              start_offset,
						   gint              end_offset);
gboolean __gtk_label_get_selection_bounds           (GtkLabel         *label,
                                                   gint             *start,
                                                   gint             *end);

PangoLayout *__gtk_label_get_layout         (GtkLabel *label);
void         __gtk_label_get_layout_offsets (GtkLabel *label,
                                           gint     *x,
                                           gint     *y);

void         __gtk_label_set_single_line_mode  (GtkLabel *label,
                                              gboolean single_line_mode);
gboolean     __gtk_label_get_single_line_mode  (GtkLabel *label);

const gchar *__gtk_label_get_current_uri          (GtkLabel *label);
void         __gtk_label_set_track_visited_links  (GtkLabel *label,
                                                 gboolean  track_links);
gboolean     __gtk_label_get_track_visited_links  (GtkLabel *label);

#ifndef GTK_DISABLE_DEPRECATED

#define  gtk_label_set           gtk_label_set_text
void       __gtk_label_get           (GtkLabel          *label,
                                    gchar            **str);

/* Convenience function to set the name and pattern by parsing
 * a string with embedded underscores, and return the appropriate
 * key symbol for the accelerator.
 */
guint __gtk_label_parse_uline            (GtkLabel    *label,
					const gchar *string);

#endif /* GTK_DISABLE_DEPRECATED */

/* private */

void ___gtk_label_mnemonics_visible_apply_recursively (GtkWidget *widget,
                                                     gboolean   mnemonics_visible);

G_END_DECLS

#endif /* __GTK_LABEL_H__ */
