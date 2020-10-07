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



#include "config.h"

#include <stlwrt.h>
#include <time.h>

#include <gdkscreen.h>
#include <gdkcolor.h>
#include <gdkinternals.h>


/**
 * __gdk_colormap_ref:
 * @cmap: a #GdkColormap
 *
 * Deprecated function; use g_object_ref() instead.
 *
 * Return value: the colormap
 *
 * Deprecated: 2.0: Use g_object_ref() instead.
 **/
GdkColormap*
__gdk_colormap_ref (GdkColormap *cmap)
{
  return (GdkColormap *) g_object_ref (cmap);
}

/**
 * __gdk_colormap_unref:
 * @cmap: a #GdkColormap
 *
 * Deprecated function; use g_object_unref() instead.
 *
 * Deprecated: 2.0: Use g_object_unref() instead.
 **/
void
__gdk_colormap_unref (GdkColormap *cmap)
{
  g_object_unref (cmap);
}


/**
 * __gdk_colormap_get_visual:
 * @colormap: a #GdkColormap.
 * 
 * Returns the visual for which a given colormap was created.
 * 
 * Return value: the visual of the colormap.
 **/
GdkVisual *
__gdk_colormap_get_visual (GdkColormap *colormap)
{
  g_return_val_if_fail (GDK_IS_COLORMAP (colormap), NULL);

  return colormap->visual;
}

/**
 * __gdk_colors_store:
 * @colormap: a #GdkColormap.
 * @colors: the new color values.
 * @ncolors: the number of colors to change.
 * 
 * Changes the value of the first @ncolors colors in
 * a private colormap. This function is obsolete and
 * should not be used. See __gdk_color_change().
 **/     
void
__gdk_colors_store (GdkColormap   *colormap,
		  GdkColor      *colors,
		  gint           ncolors)
{
  gint i;

  for (i = 0; i < ncolors; i++)
    {
      colormap->colors[i].pixel = colors[i].pixel;
      colormap->colors[i].red = colors[i].red;
      colormap->colors[i].green = colors[i].green;
      colormap->colors[i].blue = colors[i].blue;
    }

  __gdk_colormap_change (colormap, ncolors);
}

/**
 * __gdk_color_copy:
 * @color: a #GdkColor.
 * 
 * Makes a copy of a color structure. The result
 * must be freed using __gdk_color_free().
 * 
 * Return value: a copy of @color.
 **/
GdkColor*
__gdk_color_copy (const GdkColor *color)
{
  GdkColor *new_color;
  
  g_return_val_if_fail (color != NULL, NULL);

  new_color = g_slice_new (GdkColor);
  *new_color = *color;
  return new_color;
}

/**
 * __gdk_color_free:
 * @color: a #GdkColor.
 * 
 * Frees a color structure created with 
 * __gdk_color_copy().
 **/
void
__gdk_color_free (GdkColor *color)
{
  g_return_if_fail (color != NULL);

  g_slice_free (GdkColor, color);
}

/**
 * __gdk_color_white:
 * @colormap: a #GdkColormap.
 * @color: the location to store the color.
 * 
 * Returns the white color for a given colormap. The resulting
 * value has already allocated been allocated. 
 * 
 * Return value: %TRUE if the allocation succeeded.
 **/
gboolean
__gdk_color_white (GdkColormap *colormap,
		 GdkColor    *color)
{
  gint return_val;

  g_return_val_if_fail (colormap != NULL, FALSE);

  if (color)
    {
      color->red = 65535;
      color->green = 65535;
      color->blue = 65535;

      return_val = __gdk_colormap_alloc_color (colormap, color, FALSE, TRUE);
    }
  else
    return_val = FALSE;

  return return_val;
}

/**
 * __gdk_color_black:
 * @colormap: a #GdkColormap.
 * @color: the location to store the color.
 * 
 * Returns the black color for a given colormap. The resulting
 * value has already been allocated. 
 * 
 * Return value: %TRUE if the allocation succeeded.
 **/
gboolean
__gdk_color_black (GdkColormap *colormap,
		 GdkColor    *color)
{
  gint return_val;

  g_return_val_if_fail (colormap != NULL, FALSE);

  if (color)
    {
      color->red = 0;
      color->green = 0;
      color->blue = 0;

      return_val = __gdk_colormap_alloc_color (colormap, color, FALSE, TRUE);
    }
  else
    return_val = FALSE;

  return return_val;
}

/********************
 * Color allocation *
 ********************/

/**
 * __gdk_colormap_alloc_color:
 * @colormap: a #GdkColormap.
 * @color: the color to allocate. On return the
 *    <structfield>pixel</structfield> field will be
 *    filled in if allocation succeeds.
 * @writeable: If %TRUE, the color is allocated writeable
 *    (their values can later be changed using __gdk_color_change()).
 *    Writeable colors cannot be shared between applications.
 * @best_match: If %TRUE, GDK will attempt to do matching against
 *    existing colors if the color cannot be allocated as requested.
 *
 * Allocates a single color from a colormap.
 * 
 * Return value: %TRUE if the allocation succeeded.
 **/
gboolean
__gdk_colormap_alloc_color (GdkColormap *colormap,
			  GdkColor    *color,
			  gboolean     writeable,
			  gboolean     best_match)
{
  gboolean success;

  __gdk_colormap_alloc_colors (colormap, color, 1, writeable, best_match,
			     &success);

  return success;
}

/**
 * __gdk_color_alloc:
 * @colormap: a #GdkColormap.
 * @color: The color to allocate. On return, the 
 *    <structfield>pixel</structfield> field will be filled in.
 * 
 * Allocates a single color from a colormap.
 * 
 * Return value: %TRUE if the allocation succeeded.
 *
 * Deprecated: 2.2: Use __gdk_colormap_alloc_color() instead.
 **/
gboolean
__gdk_color_alloc (GdkColormap *colormap,
		 GdkColor    *color)
{
  gboolean success;

  __gdk_colormap_alloc_colors (colormap, color, 1, FALSE, TRUE, &success);

  return success;
}

/**
 * __gdk_color_hash:
 * @colora: a #GdkColor.
 * 
 * A hash function suitable for using for a hash
 * table that stores #GdkColor's.
 * 
 * Return value: The hash function applied to @colora
 **/
guint
__gdk_color_hash (const GdkColor *colora)
{
  return ((colora->red) +
	  (colora->green << 11) +
	  (colora->blue << 22) +
	  (colora->blue >> 6));
}

/**
 * __gdk_color_equal:
 * @colora: a #GdkColor.
 * @colorb: another #GdkColor.
 * 
 * Compares two colors. 
 * 
 * Return value: %TRUE if the two colors compare equal
 **/
gboolean
__gdk_color_equal (const GdkColor *colora,
		 const GdkColor *colorb)
{
  g_return_val_if_fail (colora != NULL, FALSE);
  g_return_val_if_fail (colorb != NULL, FALSE);

  return ((colora->red == colorb->red) &&
	  (colora->green == colorb->green) &&
	  (colora->blue == colorb->blue));
}

STLWRT_DEFINE_BOXED_TYPE (GdkColor, gdk_color, __gdk_color_copy, __gdk_color_free, ;)

/**
 * __gdk_color_parse:
 * @spec: the string specifying the color.
 * @color: (out): the #GdkColor to fill in
 *
 * Parses a textual specification of a color and fill in the
 * <structfield>red</structfield>, <structfield>green</structfield>,
 * and <structfield>blue</structfield> fields of a #GdkColor
 * structure. The color is <emphasis>not</emphasis> allocated, you
 * must call __gdk_colormap_alloc_color() yourself. The string can
 * either one of a large set of standard names. (Taken from the X11
 * <filename>rgb.txt</filename> file), or it can be a hex value in the
 * form '&num;rgb' '&num;rrggbb' '&num;rrrgggbbb' or
 * '&num;rrrrggggbbbb' where 'r', 'g' and 'b' are hex digits of the
 * red, green, and blue components of the color, respectively. (White
 * in the four forms is '&num;fff' '&num;ffffff' '&num;fffffffff' and
 * '&num;ffffffffffff')
 * 
 * Return value: %TRUE if the parsing succeeded.
 **/
gboolean
__gdk_color_parse (const gchar *spec,
		 GdkColor    *color)
{
  PangoColor pango_color;

  if (pango_color_parse (&pango_color, spec))
    {
      color->red = pango_color.red;
      color->green = pango_color.green;
      color->blue = pango_color.blue;

      return TRUE;
    }
  else
    return FALSE;
}

/**
 * __gdk_color_to_string:
 * @color: a #GdkColor
 *
 * Returns a textual specification of @color in the hexadecimal form
 * <literal>&num;rrrrggggbbbb</literal>, where <literal>r</literal>,
 * <literal>g</literal> and <literal>b</literal> are hex digits
 * representing the red, green and blue components respectively.
 *
 * Return value: a newly-allocated text string
 *
 * Since: 2.12
 **/
gchar *
__gdk_color_to_string (const GdkColor *color)
{
  PangoColor pango_color;

  g_return_val_if_fail (color != NULL, NULL);

  pango_color.red = color->red;
  pango_color.green = color->green;
  pango_color.blue = color->blue;

  return pango_color_to_string (&pango_color);
}

/**
 * __gdk_colormap_get_system:
 * 
 * Gets the system's default colormap for the default screen. (See
 * __gdk_colormap_get_system_for_screen ())
 * 
 * Return value: the default colormap.
 **/
GdkColormap*
__gdk_colormap_get_system (void)
{
  return __gdk_screen_get_system_colormap (__gdk_screen_get_default ());
}

#define __GDK_COLOR_C__
