/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkprinteroption.c: Handling possible settings for a specific printer setting
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

#include "config.h"

#include <stlwrt.h>
#include <string.h>
#include <gmodule.h>

#include <gtkprinteroption.h>


/*****************************************
 *            GtkPrinterOption           *
 *****************************************/

enum {
  CHANGED,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

STLWRT_DEFINE_VTYPE (GtkPrinterOption, gtk_printer_option, G_TYPE_OBJECT, G_TYPE_FLAG_NONE, ;)

static void
gtk_printer_option_finalize (GObject *object)
{
  GtkPrinterOption *option = GTK_PRINTER_OPTION (object);
  int i;
  
  g_free (gtk_printer_option_get_props (option)->name);
  g_free (gtk_printer_option_get_props (option)->display_text);
  g_free (gtk_printer_option_get_props (option)->value);
  for (i = 0; i < gtk_printer_option_get_props (option)->num_choices; i++)
    {
      g_free (gtk_printer_option_get_props (option)->choices[i]);
      g_free (gtk_printer_option_get_props (option)->choices_display[i]);
    }
  g_free (gtk_printer_option_get_props (option)->choices);
  g_free (gtk_printer_option_get_props (option)->choices_display);
  g_free (gtk_printer_option_get_props (option)->group);
  
  G_OBJECT_CLASS (gtk_printer_option_parent_class)->finalize (object);
}

static void
gtk_printer_option_init (GtkPrinterOption *option)
{
  gtk_printer_option_get_props (option)->value = g_strdup ("");
  gtk_printer_option_get_props (option)->activates_default = FALSE;
}

static void
gtk_printer_option_class_init (GtkPrinterOptionClass *class)
{
  GObjectClass *gobject_class = (GObjectClass *)class;

  gobject_class->finalize = gtk_printer_option_finalize;

  signals[CHANGED] =
    g_signal_new ("changed",
		  G_TYPE_FROM_CLASS (class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkPrinterOptionClass, changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
}

GtkPrinterOption *
gtk_printer_option_new (const char *name, const char *display_text,
			GtkPrinterOptionType type)
{
  GtkPrinterOption *option;

  option = g_object_new (GTK_TYPE_PRINTER_OPTION, NULL);

  gtk_printer_option_get_props (option)->name = g_strdup (name);
  gtk_printer_option_get_props (option)->display_text = g_strdup (display_text);
  gtk_printer_option_get_props (option)->type = type;
  
  return option;
}

static void
emit_changed (GtkPrinterOption *option)
{
  g_signal_emit (option, signals[CHANGED], 0);
}

void
gtk_printer_option_set (GtkPrinterOption *option,
			const char *value)
{
  if (value == NULL)
    value = "";
  
  if (strcmp (gtk_printer_option_get_props (option)->value, value) == 0)
    return;

  if ((gtk_printer_option_get_props (option)->type == GTK_PRINTER_OPTION_TYPE_PICKONE ||
       gtk_printer_option_get_props (option)->type == GTK_PRINTER_OPTION_TYPE_ALTERNATIVE) &&
      value != NULL)
    {
      int i;
      
      for (i = 0; i < gtk_printer_option_get_props (option)->num_choices; i++)
	{
	  if (g_ascii_strcasecmp (value, gtk_printer_option_get_props (option)->choices[i]) == 0)
	    {
	      value = gtk_printer_option_get_props (option)->choices[i];
	      break;
	    }
	}

      if (i == gtk_printer_option_get_props (option)->num_choices)
	return; /* Not found in available choices */
    }
  
  g_free (gtk_printer_option_get_props (option)->value);
  gtk_printer_option_get_props (option)->value = g_strdup (value);
  
  emit_changed (option);
}

void
gtk_printer_option_set_boolean (GtkPrinterOption *option,
				gboolean value)
{
  gtk_printer_option_set (option, value ? "True" : "False");
}

void
gtk_printer_option_set_has_conflict  (GtkPrinterOption *option,
				      gboolean  has_conflict)
{
  has_conflict = has_conflict != 0;
  
  if (gtk_printer_option_get_props (option)->has_conflict == has_conflict)
    return;

  gtk_printer_option_get_props (option)->has_conflict = has_conflict;
  emit_changed (option);
}

void
gtk_printer_option_clear_has_conflict (GtkPrinterOption     *option)
{
  gtk_printer_option_set_has_conflict  (option, FALSE);
}

void
gtk_printer_option_allocate_choices (GtkPrinterOption     *option,
				     int num)
{
  g_free (gtk_printer_option_get_props (option)->choices);
  g_free (gtk_printer_option_get_props (option)->choices_display);

  gtk_printer_option_get_props (option)->num_choices = num;
  if (num == 0)
    {
      gtk_printer_option_get_props (option)->choices = NULL;
      gtk_printer_option_get_props (option)->choices_display = NULL;
    }
  else
    {
      gtk_printer_option_get_props (option)->choices = g_new0 (char *, num);
      gtk_printer_option_get_props (option)->choices_display = g_new0 (char *, num);
    }
}

void
gtk_printer_option_choices_from_array (GtkPrinterOption   *option,
				       int                 num_choices,
				       char               *choices[],
				       char              *choices_display[])
{
  int i;
  
  gtk_printer_option_allocate_choices (option, num_choices);
  for (i = 0; i < num_choices; i++)
    {
      gtk_printer_option_get_props (option)->choices[i] = g_strdup (choices[i]);
      gtk_printer_option_get_props (option)->choices_display[i] = g_strdup (choices_display[i]);
    }
}

gboolean
gtk_printer_option_has_choice (GtkPrinterOption     *option,
			       const char           *choice)
{
  int i;
  
  for (i = 0; i < gtk_printer_option_get_props (option)->num_choices; i++)
    {
      if (strcmp (gtk_printer_option_get_props (option)->choices[i], choice) == 0)
	return TRUE;
    }
  
  return FALSE;
}

void
gtk_printer_option_set_activates_default (GtkPrinterOption *option,
					  gboolean          activates)
{
  g_return_if_fail (GTK_IS_PRINTER_OPTION (option));

  gtk_printer_option_get_props (option)->activates_default = activates;
}

gboolean
gtk_printer_option_get_activates_default (GtkPrinterOption *option)
{
  g_return_val_if_fail (GTK_IS_PRINTER_OPTION (option), FALSE);

  return gtk_printer_option_get_props (option)->activates_default;
}
