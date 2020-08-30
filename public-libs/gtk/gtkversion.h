/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2020 Gordon N. Squash.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#ifndef __GTK_VERSION_H__
#define __GTK_VERSION_H__

#if !defined(STLWRT_GTK_COMPAT)
  #error "You must specify what version of GTK to emulate by defining"
  #error "`STLWRT_GTK_COMPAT' to the version of GTK to emulate."
  #error "Consider using `pkg-config' in your application instead."

#elif STLWRT_GTK_COMPAT == 2
  /*
     Originally, STLWRT was based on GTK+ 2.24.32, the final release
     of GTK+ 2.  However, my intention is to improve upon GTK+ 2 (to
     a certain extent at least) and to tip off applications that this
     is not GTK+ 2 as they know it, I want to set both the micro
     version and interface age to 33.  In actual practice, I have not
     seen an application which actually cares about the interface age
     anyway, so this should not break very much at all.
   */
  #define GTK_MAJOR_VERSION	2
  #define GTK_MINOR_VERSION	24
  #define GTK_MICRO_VERSION	33
  #define GTK_BINARY_AGE	(100*GTK_MINOR_VERSION + GTK_MICRO_VERSION)
  #define GTK_INTERFACE_AGE	33

#elif STLWRT_GTK_COMPAT == 3
  #define GTK_MAJOR_VERSION	3
  #define GTK_MINOR_VERSION	24
  #define GTK_MICRO_VERSION	20
  #define GTK_BINARY_AGE	(100*GTK_MINOR_VERSION + GTK_MICRO_VERSION)
  #define GTK_INTERFACE_AGE	17

#else
  #error "Sorry, STLWRT cannot yet emulate that version of GTK."
#endif

/* check whether a Gtk+ version equal to or greater than
 * major.minor.micro is present.
 */
#define	GTK_CHECK_VERSION(major,minor,micro)	\
    (gtk_major_version > (major) || \
     (gtk_major_version == (major) && gtk_minor_version > (minor)) || \
     (gtk_major_version == (major) && gtk_minor_version == (minor) && \
      gtk_micro_version >= (micro)))

extern const int gtk_major_version;
extern const int gtk_minor_version;
extern const int gtk_micro_version;
extern const int gtk_binary_age;
extern const int gtk_interface_age;

#endif /* __GTK_VERSION_H__ */
