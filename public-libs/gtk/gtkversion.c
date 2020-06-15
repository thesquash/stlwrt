/* STLWRT - A modern implementation of GTK+ 2 capable of running GTK+ 3 applications
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

#include <gtk/gtkversion.h>

const int gtk_major_version	= (@GTK_MAJOR_VERSION@);
const int gtk_minor_version	= (@GTK_MINOR_VERSION@);
const int gtk_micro_version	= (@GTK_MICRO_VERSION@);
const int gtk_binary_age	= (@GTK_BINARY_AGE@);
const int gtk_interface_age	= (@GTK_INTERFACE_AGE@);
