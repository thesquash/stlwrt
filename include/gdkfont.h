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

#if !defined(GDK_DISABLE_DEPRECATED) || defined(STLWRT_COMPILATION)

#ifndef __GDK_FONT_H__
#define __GDK_FONT_H__

#include <stlwrt.h>


#include <gdktypes.h>

#include <pango/pango.h>

G_BEGIN_DECLS

#define GDK_TYPE_FONT SF(gdk_font_get_type) ()


STLWRT_DECLARE_BOXED_TYPE(GdkFont, gdk_font,
  GdkFontType   type;
  gint ascent;
  gint descent;
)

GdkFont* SF(gdk_font_ref)	    (GdkFont        *font);
void	 SF(gdk_font_unref)	    (GdkFont        *font);
gint	 SF(gdk_font_id)	    (const GdkFont  *font);
gboolean SF(gdk_font_equal)	    (const GdkFont  *fonta,
			     const GdkFont  *fontb);

GdkFont *SF(gdk_font_load_for_display)             (GdkDisplay           *display,
						const gchar          *font_name);
GdkFont *SF(gdk_fontset_load_for_display)          (GdkDisplay           *display,
						const gchar          *fontset_name);
GdkFont *SF(gdk_font_from_description_for_display) (GdkDisplay           *display,
						PangoFontDescription *font_desc);

#ifndef GDK_DISABLE_DEPRECATED

#ifndef GDK_MULTIHEAD_SAFE
GdkFont* SF(gdk_font_load)             (const gchar          *font_name);
GdkFont* SF(gdk_fontset_load)          (const gchar          *fontset_name);
GdkFont* SF(gdk_font_from_description) (PangoFontDescription *font_desc);
#endif

gint	 SF(gdk_string_width)   (GdkFont        *font,
			     const gchar    *string);
gint	 SF(gdk_text_width)	    (GdkFont        *font,
			     const gchar    *text,
			     gint            text_length);
gint	 SF(gdk_text_width_wc)  (GdkFont        *font,
			     const GdkWChar *text,
			     gint            text_length);
gint	 SF(gdk_char_width)	    (GdkFont        *font,
			     gchar           character);
gint	 SF(gdk_char_width_wc)  (GdkFont        *font,
			     GdkWChar        character);
gint	 SF(gdk_string_measure) (GdkFont        *font,
			     const gchar    *string);
gint	 SF(gdk_text_measure)   (GdkFont        *font,
			     const gchar    *text,
			     gint            text_length);
gint	 SF(gdk_char_measure)   (GdkFont        *font,
			     gchar           character);
gint	 SF(gdk_string_height)  (GdkFont        *font,
			     const gchar    *string);
gint	 SF(gdk_text_height)    (GdkFont        *font,
			     const gchar    *text,
			     gint            text_length);
gint	 SF(gdk_char_height)    (GdkFont        *font,
			     gchar           character);

void     SF(gdk_text_extents)   (GdkFont     *font,
			     const gchar *text,
			     gint         text_length,
			     gint        *lbearing,
			     gint        *rbearing,
			     gint        *width,
			     gint        *ascent,
			     gint        *descent);
void    SF(gdk_text_extents_wc) (GdkFont        *font,
			     const GdkWChar *text,
			     gint            text_length,
			     gint           *lbearing,
			     gint           *rbearing,
			     gint           *width,
			     gint           *ascent,
			     gint           *descent);
void     SF(gdk_string_extents) (GdkFont     *font,
			     const gchar *string,
			     gint        *lbearing,
			     gint        *rbearing,
			     gint        *width,
			     gint        *ascent,
			     gint        *descent);

GdkDisplay * SF(gdk_font_get_display) (GdkFont *font);

#endif /* GDK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GDK_FONT_H__ */

#endif /* !GDK_DISABLE_DEPRECATED || STLWRT_COMPILATION || STLWRT_COMPILATION */
