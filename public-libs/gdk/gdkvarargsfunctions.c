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

/*
 * Some functions in the GDK accept a variable number of parameters / arguments.
 * It turns out that passing a variable number of arguments to a function is a
 * complicated procedure; therefore, we handle the special case of functions
 * which accept variable numbers of arguments here.
 */

/* The include files below are NOT supposed to be the public include files;
 * they are supposed to be the private ones, since the private ones declare the
 * low-level STLWRT functions like '__gtk_widget_get_style' and we need those
 * function declarations.  In fact, we don't need the public headers in this
 * source file at all; here we are _defining_ the functions which are
 * _declared_ in the public header files.
 */

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <stdarg.h>



/* From gdkdisplay-x11.c and gdkx.h: */

void	gdk_x11_display_broadcast_startup_message(GdkDisplay *display,
                                                  const char *message_type,
                                                  ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, message_type);
	
	__gdk_x11_display_broadcast_startup_message(display, message_type,
	                                            parameters);
	
	va_end(parameters);
}
