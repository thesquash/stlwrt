/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkprintsettings.h: Print Settings
 * Copyright (C) 2006, Red Hat, Inc.
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

#ifndef __GTK_PRINT_SETTINGS_H__
#define __GTK_PRINT_SETTINGS_H__

#include <stlwrt.h>


#include <gtkpapersize.h>

G_BEGIN_DECLS

#define GTK_TYPE_PRINT_SETTINGS    (gtk_print_settings_get_type ())
#define GTK_PRINT_SETTINGS(obj)    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_SETTINGS, GtkPrintSettings))
#define GTK_IS_PRINT_SETTINGS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRINT_SETTINGS))

STLWRT_DECLARE_OPAQUE_TYPE(GtkPrintSettings, gtk_print_settings)

typedef void  (*GtkPrintSettingsFunc)  (const gchar *key,
					const gchar *value,
					gpointer     user_data);

typedef struct _GtkPageRange GtkPageRange;
struct _GtkPageRange
{
  gint start;
  gint end;
};

GtkPrintSettings *SF(gtk_print_settings_new)                     (void);

GtkPrintSettings *SF(gtk_print_settings_copy)                    (GtkPrintSettings     *other);

GtkPrintSettings *SF(gtk_print_settings_new_from_file)           (const gchar          *file_name,
							      GError              **error);
gboolean          SF(gtk_print_settings_load_file)               (GtkPrintSettings     *settings,
							      const gchar          *file_name,
							      GError              **error);
gboolean          SF(gtk_print_settings_to_file)                 (GtkPrintSettings     *settings,
							      const gchar          *file_name,
							      GError              **error);
GtkPrintSettings *SF(gtk_print_settings_new_from_key_file)       (GKeyFile             *key_file,
							      const gchar          *group_name,
							      GError              **error);
gboolean          SF(gtk_print_settings_load_key_file)           (GtkPrintSettings     *settings,
							      GKeyFile             *key_file,
							      const gchar          *group_name,
							      GError              **error);
void              SF(gtk_print_settings_to_key_file)             (GtkPrintSettings     *settings,
							      GKeyFile             *key_file,
							      const gchar          *group_name);
gboolean          SF(gtk_print_settings_has_key)                 (GtkPrintSettings     *settings,
							      const gchar          *key);
const gchar *     SF(gtk_print_settings_get)                 (GtkPrintSettings     *settings,
							      const gchar          *key);
void              SF(gtk_print_settings_set)                     (GtkPrintSettings     *settings,
							      const gchar          *key,
							      const gchar          *value);
void              SF(gtk_print_settings_unset)                   (GtkPrintSettings     *settings,
							      const gchar          *key);
void              SF(gtk_print_settings_foreach)                 (GtkPrintSettings     *settings,
							      GtkPrintSettingsFunc  func,
							      gpointer              user_data);
gboolean          SF(gtk_print_settings_get_bool)                (GtkPrintSettings     *settings,
							      const gchar          *key);
void              SF(gtk_print_settings_set_bool)                (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gboolean              value);
gdouble           SF(gtk_print_settings_get_double)              (GtkPrintSettings     *settings,
							      const gchar          *key);
gdouble           SF(gtk_print_settings_get_double_with_default) (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gdouble               def);
void              SF(gtk_print_settings_set_double)              (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gdouble               value);
gdouble           SF(gtk_print_settings_get_length)              (GtkPrintSettings     *settings,
							      const gchar          *key,
							      GtkUnit               unit);
void              SF(gtk_print_settings_set_length)              (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gdouble               value,
							      GtkUnit               unit);
gint              SF(gtk_print_settings_get_int)                 (GtkPrintSettings     *settings,
							      const gchar          *key);
gint              SF(gtk_print_settings_get_int_with_default)    (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gint                  def);
void              SF(gtk_print_settings_set_int)                 (GtkPrintSettings     *settings,
							      const gchar          *key,
							      gint                  value);

#define GTK_PRINT_SETTINGS_PRINTER          "printer"
#define GTK_PRINT_SETTINGS_ORIENTATION      "orientation"
#define GTK_PRINT_SETTINGS_PAPER_FORMAT     "paper-format"
#define GTK_PRINT_SETTINGS_PAPER_WIDTH      "paper-width"
#define GTK_PRINT_SETTINGS_PAPER_HEIGHT     "paper-height"
#define GTK_PRINT_SETTINGS_N_COPIES         "n-copies"
#define GTK_PRINT_SETTINGS_DEFAULT_SOURCE   "default-source"
#define GTK_PRINT_SETTINGS_QUALITY          "quality"
#define GTK_PRINT_SETTINGS_RESOLUTION       "resolution"
#define GTK_PRINT_SETTINGS_USE_COLOR        "use-color"
#define GTK_PRINT_SETTINGS_DUPLEX           "duplex"
#define GTK_PRINT_SETTINGS_COLLATE          "collate"
#define GTK_PRINT_SETTINGS_REVERSE          "reverse"
#define GTK_PRINT_SETTINGS_MEDIA_TYPE       "media-type"
#define GTK_PRINT_SETTINGS_DITHER           "dither"
#define GTK_PRINT_SETTINGS_SCALE            "scale"
#define GTK_PRINT_SETTINGS_PRINT_PAGES      "print-pages"
#define GTK_PRINT_SETTINGS_PAGE_RANGES      "page-ranges"
#define GTK_PRINT_SETTINGS_PAGE_SET         "page-set"
#define GTK_PRINT_SETTINGS_FINISHINGS       "finishings"
#define GTK_PRINT_SETTINGS_NUMBER_UP        "number-up"
#define GTK_PRINT_SETTINGS_NUMBER_UP_LAYOUT "number-up-layout"
#define GTK_PRINT_SETTINGS_OUTPUT_BIN       "output-bin"
#define GTK_PRINT_SETTINGS_RESOLUTION_X     "resolution-x"
#define GTK_PRINT_SETTINGS_RESOLUTION_Y     "resolution-y"
#define GTK_PRINT_SETTINGS_PRINTER_LPI      "printer-lpi"

#define GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT  "output-file-format"
#define GTK_PRINT_SETTINGS_OUTPUT_URI          "output-uri"

#define GTK_PRINT_SETTINGS_WIN32_DRIVER_VERSION "win32-driver-version"
#define GTK_PRINT_SETTINGS_WIN32_DRIVER_EXTRA   "win32-driver-extra"

/* Helpers: */

const gchar *SF(gtk_print_settings_get_printer)           (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_printer)           (GtkPrintSettings   *settings,
								const gchar        *printer);
GtkPageOrientation    SF(gtk_print_settings_get_orientation)       (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_orientation)       (GtkPrintSettings   *settings,
								GtkPageOrientation  orientation);
GtkPaperSize *        SF(gtk_print_settings_get_paper_size)        (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_paper_size)        (GtkPrintSettings   *settings,
								GtkPaperSize       *paper_size);
gdouble               SF(gtk_print_settings_get_paper_width)       (GtkPrintSettings   *settings,
								GtkUnit             unit);
void                  SF(gtk_print_settings_set_paper_width)       (GtkPrintSettings   *settings,
								gdouble             width,
								GtkUnit             unit);
gdouble               SF(gtk_print_settings_get_paper_height)      (GtkPrintSettings   *settings,
								GtkUnit             unit);
void                  SF(gtk_print_settings_set_paper_height)      (GtkPrintSettings   *settings,
								gdouble             height,
								GtkUnit             unit);
gboolean              SF(gtk_print_settings_get_use_color)         (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_use_color)         (GtkPrintSettings   *settings,
								gboolean            use_color);
gboolean              SF(gtk_print_settings_get_collate)           (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_collate)           (GtkPrintSettings   *settings,
								gboolean            collate);
gboolean              SF(gtk_print_settings_get_reverse)           (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_reverse)           (GtkPrintSettings   *settings,
								gboolean            reverse);
GtkPrintDuplex        SF(gtk_print_settings_get_duplex)            (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_duplex)            (GtkPrintSettings   *settings,
								GtkPrintDuplex      duplex);
GtkPrintQuality       SF(gtk_print_settings_get_quality)           (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_quality)           (GtkPrintSettings   *settings,
								GtkPrintQuality     quality);
gint                  SF(gtk_print_settings_get_n_copies)          (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_n_copies)          (GtkPrintSettings   *settings,
								gint                num_copies);
gint                  SF(gtk_print_settings_get_number_up)         (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_number_up)         (GtkPrintSettings   *settings,
								gint                number_up);
GtkNumberUpLayout     SF(gtk_print_settings_get_number_up_layout)  (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_number_up_layout)  (GtkPrintSettings   *settings,
								GtkNumberUpLayout   number_up_layout);
gint                  SF(gtk_print_settings_get_resolution)        (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_resolution)        (GtkPrintSettings   *settings,
								gint                resolution);
gint                  SF(gtk_print_settings_get_resolution_x)      (GtkPrintSettings   *settings);
gint                  SF(gtk_print_settings_get_resolution_y)      (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_resolution_xy)     (GtkPrintSettings   *settings,
								gint                resolution_x,
								gint                resolution_y);
gdouble               SF(gtk_print_settings_get_printer_lpi)       (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_printer_lpi)       (GtkPrintSettings   *settings,
								gdouble             lpi);
gdouble               SF(gtk_print_settings_get_scale)             (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_scale)             (GtkPrintSettings   *settings,
								gdouble             scale);
GtkPrintPages         SF(gtk_print_settings_get_print_pages)       (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_print_pages)       (GtkPrintSettings   *settings,
								GtkPrintPages       pages);
GtkPageRange *        SF(gtk_print_settings_get_page_ranges)       (GtkPrintSettings   *settings,
								gint               *num_ranges);
void                  SF(gtk_print_settings_set_page_ranges)       (GtkPrintSettings   *settings,
								GtkPageRange       *page_ranges,
								gint                num_ranges);
GtkPageSet            SF(gtk_print_settings_get_page_set)          (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_page_set)          (GtkPrintSettings   *settings,
								GtkPageSet          page_set);
const gchar *         SF(gtk_print_settings_get_default_source)    (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_default_source)    (GtkPrintSettings   *settings,
								const gchar        *default_source);
const gchar *         SF(gtk_print_settings_get_media_type)        (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_media_type)        (GtkPrintSettings   *settings,
								const gchar        *media_type);
const gchar *         SF(gtk_print_settings_get_dither)            (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_dither)            (GtkPrintSettings   *settings,
								const gchar        *dither);
const gchar *         SF(gtk_print_settings_get_finishings)        (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_finishings)        (GtkPrintSettings   *settings,
								const gchar        *finishings);
const gchar *         SF(gtk_print_settings_get_output_bin)        (GtkPrintSettings   *settings);
void                  SF(gtk_print_settings_set_output_bin)        (GtkPrintSettings   *settings,
								const gchar        *output_bin);

G_END_DECLS

#endif /* __GTK_PRINT_SETTINGS_H__ */
