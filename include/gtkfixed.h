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

#ifndef __GTK_FIXED_H__
#define __GTK_FIXED_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkcontainer.h>


G_BEGIN_DECLS

#define GTK_TYPE_FIXED                  (SF(gtk_fixed_get_type) ())
#define GTK_FIXED(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FIXED, GtkFixed))
#define GTK_FIXED_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_FIXED, GtkFixedClass))
#define GTK_IS_FIXED(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FIXED))
#define GTK_IS_FIXED_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FIXED))
#define GTK_FIXED_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_FIXED, GtkFixedClass))


typedef struct _GtkFixed        GtkFixedFat;
typedef struct _GtkFixed        GtkFixedThin;

typedef struct _GtkFixedClass   GtkFixedClass;
typedef struct _GtkFixedChild   GtkFixedChild;

/********************************************************************/
struct _GtkFixedProps
{


  GList * (children);
};

struct _GtkFixedFat
{
  GtkContainerFat   container;

  struct _GtkFixedProps instance_properties;
};

struct _GtkFixedThin
{
  GtkContainerThin  container;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkFixedFat   fat_instance;
  struct _GtkFixedThin  thin_instance;
}   GtkFixed;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkFixedFat GtkFixed;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkFixedThin GtkFixed;
#endif
/********************************************************************/



struct _GtkFixedClass
{
  GtkContainerClass parent_class;
};

struct _GtkFixedChild
{
  GtkWidget *widget;
  gint x;
  gint y;
};


GType      SF(_T2_gtk_fixed_get_type)          (void) G_GNUC_CONST;
GType      SF(_3T_gtk_fixed_get_type)          (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      SF(gtk_fixed_get_type)          (void) G_GNUC_CONST;
GtkWidget* SF(gtk_fixed_new)               (void);
void       SF(gtk_fixed_put)               (GtkFixed       *fixed,
                                        GtkWidget      *widget,
                                        gint            x,
                                        gint            y);
void       SF(gtk_fixed_move)              (GtkFixed       *fixed,
                                        GtkWidget      *widget,
                                        gint            x,
                                        gint            y);
#ifndef GTK_DISABLE_DEPRECATED
void       SF(gtk_fixed_set_has_window)    (GtkFixed       *fixed,
					gboolean        has_window);
gboolean   SF(gtk_fixed_get_has_window)    (GtkFixed       *fixed);
#endif

G_END_DECLS

#endif /* __GTK_FIXED_H__ */
