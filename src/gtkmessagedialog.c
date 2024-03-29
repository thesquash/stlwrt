/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 2 -*- */
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */



#include "config.h"

#include <stlwrt.h>
#include <string.h>

#include <gtkmessagedialog.h>
#include <gtkaccessible.h>
#include <gtkbuildable.h>
#include <gtklabel.h>
#include <gtkhbox.h>
#include <gtkvbox.h>
#include <gtkimage.h>
#include <gtkstock.h>
#include <gtkiconfactory.h>
#include <gtkintl.h>
#include <gtkprivate.h>


/**
 * SECTION:gtkmessagedialog
 * @Short_description: A convenient message window
 * @Title: GtkMessageDialog
 * @See_also:#GtkDialog
 *
 * #GtkMessageDialog presents a dialog with an image representing the type of
 * message (Error, Question, etc.) alongside some message text. It's simply a
 * convenience widget; you could construct the equivalent of #GtkMessageDialog
 * from #GtkDialog without too much effort, but #GtkMessageDialog saves typing.
 *
 * The easiest way to do a modal message dialog is to use __gtk_dialog_run(), though
 * you can also pass in the %GTK_DIALOG_MODAL flag, __gtk_dialog_run() automatically
 * makes the dialog modal and waits for the user to respond to it. __gtk_dialog_run()
 * returns when any dialog button is clicked.
 * <example>
 * <title>A modal dialog.</title>
 * <programlisting>
 *  dialog = __gtk_message_dialog_new (main_application_window,
 *                                   GTK_DIALOG_DESTROY_WITH_PARENT,
 *                                   GTK_MESSAGE_ERROR,
 *                                   GTK_BUTTONS_CLOSE,
 *                                   "Error loading file '&percnt;s': &percnt;s",
 *                                   filename, g_strerror (errno));
 *  __gtk_dialog_run (GTK_DIALOG (dialog));
 *  __gtk_widget_destroy (dialog);
 * </programlisting>
 * </example>
 * You might do a non-modal #GtkMessageDialog as follows:
 * <example>
 * <title>A non-modal dialog.</title>
 * <programlisting>
 *  dialog = __gtk_message_dialog_new (main_application_window,
 *                                   GTK_DIALOG_DESTROY_WITH_PARENT,
 *                                   GTK_MESSAGE_ERROR,
 *                                   GTK_BUTTONS_CLOSE,
 *                                   "Error loading file '&percnt;s': &percnt;s",
 *                                   filename, g_strerror (errno));
 *
 *  /&ast; Destroy the dialog when the user responds to it (e.g. clicks a button) &ast;/
 *  g_signal_connect_swapped (dialog, "response",
 *                            G_CALLBACK (__gtk_widget_destroy),
 *                            dialog);
 * </programlisting>
 * </example>
 *
 * <refsect2 id="GtkMessageDialog-BUILDER-UI">
 * <title>GtkMessageDialog as GtkBuildable</title>
 * <para>
 * The GtkMessageDialog implementation of the GtkBuildable interface exposes
 * the message area as an internal child with the name "message_area".
 * </para>
 * </refsect2>
 */



typedef struct _GtkMessageDialogPrivate GtkMessageDialogPrivate;

struct _GtkMessageDialogPrivate
{
  GtkWidget *message_area; /* vbox for the primary and secondary labels, and any extra content from the caller */
  GtkWidget *secondary_label;
  guint message_type : 3;
  guint has_primary_markup : 1;
  guint has_secondary_text : 1;
};

static void gtk_message_dialog_style_set  (GtkWidget             *widget,
                                           GtkStyle              *prev_style);

static void gtk_message_dialog_set_property (GObject          *object,
					     guint             prop_id,
					     const GValue     *value,
					     GParamSpec       *pspec);
static void gtk_message_dialog_get_property (GObject          *object,
					     guint             prop_id,
					     GValue           *value,
					     GParamSpec       *pspec);
static void gtk_message_dialog_add_buttons  (GtkMessageDialog *message_dialog,
					     GtkButtonsType    buttons);
static void      gtk_message_dialog_buildable_interface_init     (GtkBuildableIface *iface);
static GObject * gtk_message_dialog_buildable_get_internal_child (GtkBuildable  *buildable,
                                                                  GtkBuilder    *builder,
                                                                  const gchar   *childname);


enum {
  PROP_0,
  PROP_MESSAGE_TYPE,
  PROP_BUTTONS,
  PROP_TEXT,
  PROP_USE_MARKUP,
  PROP_SECONDARY_TEXT,
  PROP_SECONDARY_USE_MARKUP,
  PROP_IMAGE,
  PROP_MESSAGE_AREA
};

STLWRT_DEFINE_VTYPE (GtkMessageDialog, gtk_message_dialog, GTK_TYPE_DIALOG, G_TYPE_FLAG_NONE,
                     G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                            gtk_message_dialog_buildable_interface_init)
                     G_ADD_PRIVATE (GtkMessageDialog))

static GtkBuildableIface *parent_buildable_iface;

static void
gtk_message_dialog_buildable_interface_init (GtkBuildableIface *iface)
{
  parent_buildable_iface = g_type_interface_peek_parent (iface);
  iface->get_internal_child = gtk_message_dialog_buildable_get_internal_child;
  iface->custom_tag_start = parent_buildable_iface->custom_tag_start;
  iface->custom_finished = parent_buildable_iface->custom_finished;
}

static GObject *
gtk_message_dialog_buildable_get_internal_child (GtkBuildable *buildable,
                                                 GtkBuilder   *builder,
                                                 const gchar  *childname)
{
  if (strcmp (childname, "message_area") == 0)
    return G_OBJECT (__gtk_message_dialog_get_message_area (GTK_MESSAGE_DIALOG (buildable)));

  return parent_buildable_iface->get_internal_child (buildable, builder, childname);
}


static void
gtk_message_dialog_class_init (GtkMessageDialogClass *class)
{
  GtkWidgetClass *widget_class;
  GObjectClass *gobject_class;

  widget_class = GTK_WIDGET_CLASS (class);
  gobject_class = G_OBJECT_CLASS (class);
  
  widget_class->style_set = gtk_message_dialog_style_set;

  gobject_class->set_property = gtk_message_dialog_set_property;
  gobject_class->get_property = gtk_message_dialog_get_property;
  
  __gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("message-border",
                                                             P_("Image/label border"),
                                                             P_("Width of border around the label and image in the message dialog"),
                                                             0,
                                                             G_MAXINT,
                                                             12,
                                                             GTK_PARAM_READABLE));
  /**
   * GtkMessageDialog:use-separator:
   *
   * Whether to draw a separator line between the message label and the buttons
   * in the dialog.
   *
   * Since: 2.4
   *
   * Deprecated: 2.22: This style property will be removed in GTK+ 3
   */
  __gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_boolean ("use-separator",
								 P_("Use separator"),
								 P_("Whether to put a separator between the message dialog's text and the buttons"),
								 FALSE,
								 GTK_PARAM_READABLE));
  /**
   * GtkMessageDialog:message-type:
   *
   * The type of the message. The type is used to determine
   * the image that is shown in the dialog, unless the image is 
   * explicitly set by the ::image property.
   */
  g_object_class_install_property (gobject_class,
                                   PROP_MESSAGE_TYPE,
                                   g_param_spec_enum ("message-type",
						      P_("Message Type"),
						      P_("The type of message"),
						      GTK_TYPE_MESSAGE_TYPE,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_PARAM_READWRITE | G_PARAM_CONSTRUCT));
  g_object_class_install_property (gobject_class,
                                   PROP_BUTTONS,
                                   g_param_spec_enum ("buttons",
						      P_("Message Buttons"),
						      P_("The buttons shown in the message dialog"),
						      GTK_TYPE_BUTTONS_TYPE,
                                                      GTK_BUTTONS_NONE,
                                                      GTK_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

  /**
   * GtkMessageDialog:text:
   * 
   * The primary text of the message dialog. If the dialog has 
   * a secondary text, this will appear as the title.
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
                                   PROP_TEXT,
                                   g_param_spec_string ("text",
                                                        P_("Text"),
                                                        P_("The primary text of the message dialog"),
                                                        "",
                                                        GTK_PARAM_READWRITE));

  /**
   * GtkMessageDialog:use-markup:
   * 
   * %TRUE if the primary text of the dialog includes Pango markup. 
   * See pango_parse_markup(). 
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
				   PROP_USE_MARKUP,
				   g_param_spec_boolean ("use-markup",
							 P_("Use Markup"),
							 P_("The primary text of the title includes Pango markup."),
							 FALSE,
							 GTK_PARAM_READWRITE));
  
  /**
   * GtkMessageDialog:secondary-text:
   * 
   * The secondary text of the message dialog. 
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
                                   PROP_SECONDARY_TEXT,
                                   g_param_spec_string ("secondary-text",
                                                        P_("Secondary Text"),
                                                        P_("The secondary text of the message dialog"),
                                                        NULL,
                                                        GTK_PARAM_READWRITE));

  /**
   * GtkMessageDialog:secondary-use-markup:
   * 
   * %TRUE if the secondary text of the dialog includes Pango markup. 
   * See pango_parse_markup(). 
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
				   PROP_SECONDARY_USE_MARKUP,
				   g_param_spec_boolean ("secondary-use-markup",
							 P_("Use Markup in secondary"),
							 P_("The secondary text includes Pango markup."),
							 FALSE,
							 GTK_PARAM_READWRITE));

  /**
   * GtkMessageDialog:image:
   * 
   * The image for this dialog.
   *
   * Since: 2.10
   */
  g_object_class_install_property (gobject_class,
                                   PROP_IMAGE,
                                   g_param_spec_object ("image",
                                                        P_("Image"),
                                                        P_("The image"),
                                                        GTK_TYPE_WIDGET,
                                                        GTK_PARAM_READWRITE));

  /**
   * GtkMessageDialog:message-area
   *
   * The #GtkVBox that corresponds to the message area of this dialog.  See
   * __gtk_message_dialog_get_message_area() for a detailed description of this
   * area.
   *
   * Since: 2.22
   */
  g_object_class_install_property (gobject_class,
				   PROP_MESSAGE_AREA,
				   g_param_spec_object ("message-area",
							P_("Message area"),
							P_("GtkVBox that holds the dialog's primary and secondary labels"),
							GTK_TYPE_WIDGET,
							GTK_PARAM_READABLE));
}

static void
gtk_message_dialog_init (GtkMessageDialog *dialog)
{
  GtkWidget *hbox;
  GtkMessageDialogPrivate *priv;

  priv = gtk_message_dialog_get_instance_private (dialog);

  __gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
  __gtk_window_set_title (GTK_WINDOW (dialog), "");
  __gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dialog), TRUE);

  priv->has_primary_markup = FALSE;
  priv->has_secondary_text = FALSE;
  priv->secondary_label = __gtk_label_new (NULL);
  __gtk_widget_set_no_show_all (priv->secondary_label, TRUE);
  
  gtk_message_dialog_get_props (dialog)->label = __gtk_label_new (NULL);
  gtk_message_dialog_get_props (dialog)->image = __gtk_image_new_from_stock (NULL, GTK_ICON_SIZE_DIALOG);
  __gtk_misc_set_alignment (GTK_MISC (gtk_message_dialog_get_props (dialog)->image), 0.5, 0.0);
  
  __gtk_label_set_line_wrap  (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label), TRUE);
  __gtk_label_set_selectable (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label), TRUE);
  __gtk_misc_set_alignment   (GTK_MISC  (gtk_message_dialog_get_props (dialog)->label), 0.0, 0.0);
  
  __gtk_label_set_line_wrap  (GTK_LABEL (priv->secondary_label), TRUE);
  __gtk_label_set_selectable (GTK_LABEL (priv->secondary_label), TRUE);
  __gtk_misc_set_alignment   (GTK_MISC  (priv->secondary_label), 0.0, 0.0);

  hbox = __gtk_hbox_new (FALSE, 12);
  priv->message_area = __gtk_vbox_new (FALSE, 12);

  __gtk_box_pack_start (GTK_BOX (priv->message_area), gtk_message_dialog_get_props (dialog)->label,
                      FALSE, FALSE, 0);

  __gtk_box_pack_start (GTK_BOX (priv->message_area), priv->secondary_label,
                      TRUE, TRUE, 0);

  __gtk_box_pack_start (GTK_BOX (hbox), gtk_message_dialog_get_props (dialog)->image,
                      FALSE, FALSE, 0);

  __gtk_box_pack_start (GTK_BOX (hbox), priv->message_area,
                      TRUE, TRUE, 0);

  __gtk_box_pack_start (GTK_BOX (gtk_dialog_get_props (GTK_DIALOG (dialog))->vbox),
                      hbox,
                      FALSE, FALSE, 0);

  __gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
  __gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
  __gtk_box_set_spacing (GTK_BOX (gtk_dialog_get_props (GTK_DIALOG (dialog))->vbox), 14); /* 14 + 2 * 5 = 24 */
  __gtk_container_set_border_width (GTK_CONTAINER (gtk_dialog_get_props (GTK_DIALOG (dialog))->action_area), 5);
  __gtk_box_set_spacing (GTK_BOX (gtk_dialog_get_props (GTK_DIALOG (dialog))->action_area), 6);

  __gtk_widget_show_all (hbox);

  ___gtk_dialog_set_ignore_separator (GTK_DIALOG (dialog), TRUE);
}

static void
setup_primary_label_font (GtkMessageDialog *dialog)
{
  gint size;
  PangoFontDescription *font_desc;
  GtkMessageDialogPrivate *priv;

  priv = gtk_message_dialog_get_instance_private (dialog);

  /* unset the font settings */
  __gtk_widget_modify_font (gtk_message_dialog_get_props (dialog)->label, NULL);

  if (priv->has_secondary_text && !priv->has_primary_markup)
    {
      size = pango_font_description_get_size (gtk_widget_get_props (gtk_message_dialog_get_props (dialog)->label)->style->font_desc);
      font_desc = pango_font_description_new ();
      pango_font_description_set_weight (font_desc, PANGO_WEIGHT_BOLD);
      pango_font_description_set_size (font_desc, size * PANGO_SCALE_LARGE);
      __gtk_widget_modify_font (gtk_message_dialog_get_props (dialog)->label, font_desc);
      pango_font_description_free (font_desc);
    }
}

static void
setup_type (GtkMessageDialog *dialog,
	    GtkMessageType    type)
{
  GtkMessageDialogPrivate *priv = gtk_message_dialog_get_instance_private (dialog);
  const gchar *stock_id = NULL;
  AtkObject *atk_obj;
 
  priv->message_type = type;

  switch (type)
    {
    case GTK_MESSAGE_INFO:
      stock_id = GTK_STOCK_DIALOG_INFO;
      break;

    case GTK_MESSAGE_QUESTION:
      stock_id = GTK_STOCK_DIALOG_QUESTION;
      break;

    case GTK_MESSAGE_WARNING:
      stock_id = GTK_STOCK_DIALOG_WARNING;
      break;
      
    case GTK_MESSAGE_ERROR:
      stock_id = GTK_STOCK_DIALOG_ERROR;
      break;

    case GTK_MESSAGE_OTHER:
      break;

    default:
      g_warning ("Unknown GtkMessageType %u", type);
      break;
    }

  if (stock_id)
    __gtk_image_set_from_stock (GTK_IMAGE (gtk_message_dialog_get_props (dialog)->image), stock_id,
                              GTK_ICON_SIZE_DIALOG);
      
  atk_obj = __gtk_widget_get_accessible (GTK_WIDGET (dialog));
  if (GTK_IS_ACCESSIBLE (atk_obj))
    {
      atk_object_set_role (atk_obj, ATK_ROLE_ALERT);
      if (stock_id)
        {
          GtkStockItem item;

          __gtk_stock_lookup (stock_id, &item);
          atk_object_set_name (atk_obj, item.label);
        }
    }
}

static void 
gtk_message_dialog_set_property (GObject      *object,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
  GtkMessageDialog *dialog;
  GtkMessageDialogPrivate *priv;

  dialog = GTK_MESSAGE_DIALOG (object);
  priv = gtk_message_dialog_get_instance_private (dialog);
  
  switch (prop_id)
    {
    case PROP_MESSAGE_TYPE:
      setup_type (dialog, g_value_get_enum (value));
      break;
    case PROP_BUTTONS:
      gtk_message_dialog_add_buttons (dialog, g_value_get_enum (value));
      break;
    case PROP_TEXT:
      if (priv->has_primary_markup)
	__gtk_label_set_markup (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label), 
			      g_value_get_string (value));
      else
	__gtk_label_set_text (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label), 
			    g_value_get_string (value));
      break;
    case PROP_USE_MARKUP:
      priv->has_primary_markup = g_value_get_boolean (value) != FALSE;
      __gtk_label_set_use_markup (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label), 
				priv->has_primary_markup);
      setup_primary_label_font (dialog);
      break;
    case PROP_SECONDARY_TEXT:
      {
	const gchar *txt = g_value_get_string (value);
	
	if (__gtk_label_get_use_markup (GTK_LABEL (priv->secondary_label)))
	  __gtk_label_set_markup (GTK_LABEL (priv->secondary_label), txt);
	else
	  __gtk_label_set_text (GTK_LABEL (priv->secondary_label), txt);

	if (txt)
	  {
	    priv->has_secondary_text = TRUE;
	    __gtk_widget_show (priv->secondary_label);
	  }
	else
	  {
	    priv->has_secondary_text = FALSE;
	    __gtk_widget_hide (priv->secondary_label);
	  }
	setup_primary_label_font (dialog);
      }
      break;
    case PROP_SECONDARY_USE_MARKUP:
      __gtk_label_set_use_markup (GTK_LABEL (priv->secondary_label), 
				g_value_get_boolean (value));
      break;
    case PROP_IMAGE:
      __gtk_message_dialog_set_image (dialog, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void 
gtk_message_dialog_get_property (GObject     *object,
				 guint        prop_id,
				 GValue      *value,
				 GParamSpec  *pspec)
{
  GtkMessageDialog *dialog;
  GtkMessageDialogPrivate *priv;

  dialog = GTK_MESSAGE_DIALOG (object);
  priv = gtk_message_dialog_get_instance_private (dialog);
    
  switch (prop_id)
    {
    case PROP_MESSAGE_TYPE:
      g_value_set_enum (value, (GtkMessageType) priv->message_type);
      break;
    case PROP_TEXT:
      g_value_set_string (value, __gtk_label_get_label (GTK_LABEL (gtk_message_dialog_get_props (dialog)->label)));
      break;
    case PROP_USE_MARKUP:
      g_value_set_boolean (value, priv->has_primary_markup);
      break;
    case PROP_SECONDARY_TEXT:
      if (priv->has_secondary_text)
      g_value_set_string (value, 
			  __gtk_label_get_label (GTK_LABEL (priv->secondary_label)));
      else
	g_value_set_string (value, NULL);
      break;
    case PROP_SECONDARY_USE_MARKUP:
      if (priv->has_secondary_text)
	g_value_set_boolean (value, 
			     __gtk_label_get_use_markup (GTK_LABEL (priv->secondary_label)));
      else
	g_value_set_boolean (value, FALSE);
      break;
    case PROP_IMAGE:
      g_value_set_object (value, gtk_message_dialog_get_props (dialog)->image);
      break;
    case PROP_MESSAGE_AREA:
      g_value_set_object (value, priv->message_area);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/**
 * __gtk_message_dialog_new:
 * @parent: (allow-none): transient parent, or %NULL for none
 * @flags: flags
 * @type: type of message
 * @buttons: set of buttons to use
 * @message_format: (allow-none): printf()-style format string, or %NULL
 * @Varargs: arguments for @message_format
 *
 * Creates a new message dialog, which is a simple dialog with an icon
 * indicating the dialog type (error, warning, etc.) and some text the
 * user may want to see. When the user clicks a button a "response"
 * signal is emitted with response IDs from #GtkResponseType. See
 * #GtkDialog for more details.
 *
 * Return value: (transfer none): a new #GtkMessageDialog
 **/
GtkWidget*
__gtk_message_dialog_new (GtkWindow     *parent,
                        GtkDialogFlags flags,
                        GtkMessageType type,
                        GtkButtonsType buttons,
                        const gchar   *message_format,
                        ...)
{
  GtkWidget *widget;
  GtkDialog *dialog;
  gchar* msg = NULL;
  va_list args;

  g_return_val_if_fail (parent == NULL || GTK_IS_WINDOW (parent), NULL);

  widget = g_object_new (GTK_TYPE_MESSAGE_DIALOG,
			 "message-type", type,
			 "buttons", buttons,
			 NULL);
  dialog = GTK_DIALOG (widget);

  if (flags & GTK_DIALOG_NO_SEPARATOR)
    {
      g_warning ("The GTK_DIALOG_NO_SEPARATOR flag cannot be used for GtkMessageDialog");
      flags &= ~GTK_DIALOG_NO_SEPARATOR;
    }

  if (message_format)
    {
      va_start (args, message_format);
      msg = g_strdup_vprintf (message_format, args);
      va_end (args);

      __gtk_label_set_text (GTK_LABEL (gtk_message_dialog_get_props (GTK_MESSAGE_DIALOG (widget))->label),
                          msg);

      g_free (msg);
    }

  if (parent != NULL)
    __gtk_window_set_transient_for (GTK_WINDOW (widget),
                                  GTK_WINDOW (parent));
  
  if (flags & GTK_DIALOG_MODAL)
    __gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  if (flags & GTK_DIALOG_DESTROY_WITH_PARENT)
    __gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);

  return widget;
}

/**
 * __gtk_message_dialog_new_with_markup:
 * @parent: (allow-none): transient parent, or %NULL for none 
 * @flags: flags
 * @type: type of message
 * @buttons: set of buttons to use
 * @message_format: (allow-none): printf()-style format string, or %NULL
 * @Varargs: arguments for @message_format
 * 
 * Creates a new message dialog, which is a simple dialog with an icon
 * indicating the dialog type (error, warning, etc.) and some text which
 * is marked up with the <link linkend="PangoMarkupFormat">Pango text markup language</link>.
 * When the user clicks a button a "response" signal is emitted with
 * response IDs from #GtkResponseType. See #GtkDialog for more details.
 *
 * Special XML characters in the printf() arguments passed to this
 * function will automatically be escaped as necessary.
 * (See g_markup_printf_escaped() for how this is implemented.)
 * Usually this is what you want, but if you have an existing
 * Pango markup string that you want to use literally as the
 * label, then you need to use __gtk_message_dialog_set_markup()
 * instead, since you can't pass the markup string either
 * as the format (it might contain '%' characters) or as a string
 * argument.
 * |[
 *  GtkWidget *dialog;
 *  dialog = __gtk_message_dialog_new (main_application_window,
 *                                   GTK_DIALOG_DESTROY_WITH_PARENT,
 *                                   GTK_MESSAGE_ERROR,
 *                                   GTK_BUTTONS_CLOSE,
 *                                   NULL);
 *  __gtk_message_dialog_set_markup (GTK_MESSAGE_DIALOG (dialog),
 *                                 markup);
 * ]|
 * 
 * Return value: a new #GtkMessageDialog
 *
 * Since: 2.4
 **/
GtkWidget*
__gtk_message_dialog_new_with_markup (GtkWindow     *parent,
                                    GtkDialogFlags flags,
                                    GtkMessageType type,
                                    GtkButtonsType buttons,
                                    const gchar   *message_format,
                                    ...)
{
  GtkWidget *widget;
  va_list args;
  gchar *msg = NULL;

  g_return_val_if_fail (parent == NULL || GTK_IS_WINDOW (parent), NULL);

  widget = __gtk_message_dialog_new (parent, flags, type, buttons, NULL);

  if (message_format)
    {
      va_start (args, message_format);
      msg = g_markup_vprintf_escaped (message_format, args);
      va_end (args);

      __gtk_message_dialog_set_markup (GTK_MESSAGE_DIALOG (widget), msg);

      g_free (msg);
    }

  return widget;
}

/**
 * __gtk_message_dialog_set_image:
 * @dialog: a #GtkMessageDialog
 * @image: the image
 * 
 * Sets the dialog's image to @image.
 *
 * Since: 2.10
 **/
void
__gtk_message_dialog_set_image (GtkMessageDialog *dialog,
			      GtkWidget        *image)
{
  GtkMessageDialogPrivate *priv;
  GtkWidget *parent;

  g_return_if_fail (GTK_IS_MESSAGE_DIALOG (dialog));
  g_return_if_fail (image == NULL || GTK_IS_WIDGET (image));

  if (image == NULL)
    {
      image = __gtk_image_new_from_stock (NULL, GTK_ICON_SIZE_DIALOG);
      __gtk_misc_set_alignment (GTK_MISC (image), 0.5, 0.0);
    }

  priv = gtk_message_dialog_get_instance_private (dialog);

  priv->message_type = GTK_MESSAGE_OTHER;
  
  parent = gtk_widget_get_props (gtk_message_dialog_get_props (dialog)->image)->parent;
  __gtk_container_add (GTK_CONTAINER (parent), image);
  __gtk_container_remove (GTK_CONTAINER (parent), gtk_message_dialog_get_props (dialog)->image);
  __gtk_box_reorder_child (GTK_BOX (parent), image, 0);

  gtk_message_dialog_get_props (dialog)->image = image;

  g_object_notify (G_OBJECT (dialog), "image");
}

/**
 * __gtk_message_dialog_get_image:
 * @dialog: a #GtkMessageDialog
 *
 * Gets the dialog's image.
 *
 * Return value: (transfer none): the dialog's image
 *
 * Since: 2.14
 **/
GtkWidget *
__gtk_message_dialog_get_image (GtkMessageDialog *dialog)
{
  g_return_val_if_fail (GTK_IS_MESSAGE_DIALOG (dialog), NULL);

  return gtk_message_dialog_get_props (dialog)->image;
}

/**
 * __gtk_message_dialog_set_markup:
 * @message_dialog: a #GtkMessageDialog
 * @str: markup string (see <link linkend="PangoMarkupFormat">Pango markup format</link>)
 * 
 * Sets the text of the message dialog to be @str, which is marked
 * up with the <link linkend="PangoMarkupFormat">Pango text markup
 * language</link>.
 *
 * Since: 2.4
 **/
void
__gtk_message_dialog_set_markup (GtkMessageDialog *message_dialog,
                               const gchar      *str)
{
  GtkMessageDialogPrivate *priv;

  g_return_if_fail (GTK_IS_MESSAGE_DIALOG (message_dialog));

  priv = gtk_message_dialog_get_instance_private (message_dialog);
  priv->has_primary_markup = TRUE;
  __gtk_label_set_markup (GTK_LABEL (gtk_message_dialog_get_props (message_dialog)->label), str);
}

/**
 * __gtk_message_dialog_format_secondary_text:
 * @message_dialog: a #GtkMessageDialog
 * @message_format: (allow-none): printf()-style format string, or %NULL
 * @Varargs: arguments for @message_format
 * 
 * Sets the secondary text of the message dialog to be @message_format 
 * (with printf()-style).
 *
 * Note that setting a secondary text makes the primary text become
 * bold, unless you have provided explicit markup.
 *
 * Since: 2.6
 **/
void
__gtk_message_dialog_format_secondary_text (GtkMessageDialog *message_dialog,
                                          const gchar      *message_format,
                                          ...)
{
  va_list args;
  gchar *msg = NULL;
  GtkMessageDialogPrivate *priv;

  g_return_if_fail (GTK_IS_MESSAGE_DIALOG (message_dialog));

  priv = gtk_message_dialog_get_instance_private (message_dialog);

  if (message_format)
    {
      priv->has_secondary_text = TRUE;

      va_start (args, message_format);
      msg = g_strdup_vprintf (message_format, args);
      va_end (args);

      __gtk_widget_show (priv->secondary_label);
      __gtk_label_set_text (GTK_LABEL (priv->secondary_label), msg);

      g_free (msg);
    }
  else
    {
      priv->has_secondary_text = FALSE;
      __gtk_widget_hide (priv->secondary_label);
    }

  setup_primary_label_font (message_dialog);
}

/**
 * __gtk_message_dialog_format_secondary_markup:
 * @message_dialog: a #GtkMessageDialog
 * @message_format: printf()-style markup string (see 
     <link linkend="PangoMarkupFormat">Pango markup format</link>), or %NULL
 * @Varargs: arguments for @message_format
 * 
 * Sets the secondary text of the message dialog to be @message_format (with 
 * printf()-style), which is marked up with the 
 * <link linkend="PangoMarkupFormat">Pango text markup language</link>.
 *
 * Note that setting a secondary text makes the primary text become
 * bold, unless you have provided explicit markup.
 *
 * Due to an oversight, this function does not escape special XML characters
 * like __gtk_message_dialog_new_with_markup() does. Thus, if the arguments 
 * may contain special XML characters, you should use g_markup_printf_escaped()
 * to escape it.

 * <informalexample><programlisting>
 * gchar *msg;
 *  
 * msg = g_markup_printf_escaped (message_format, ...);
 * __gtk_message_dialog_format_secondary_markup (message_dialog, "&percnt;s", msg);
 * g_free (msg);
 * </programlisting></informalexample>
 *
 * Since: 2.6
 **/
void
__gtk_message_dialog_format_secondary_markup (GtkMessageDialog *message_dialog,
                                            const gchar      *message_format,
                                            ...)
{
  va_list args;
  gchar *msg = NULL;
  GtkMessageDialogPrivate *priv;

  g_return_if_fail (GTK_IS_MESSAGE_DIALOG (message_dialog));

  priv = gtk_message_dialog_get_instance_private (message_dialog);

  if (message_format)
    {
      priv->has_secondary_text = TRUE;

      va_start (args, message_format);
      msg = g_strdup_vprintf (message_format, args);
      va_end (args);

      __gtk_widget_show (priv->secondary_label);
      __gtk_label_set_markup (GTK_LABEL (priv->secondary_label), msg);

      g_free (msg);
    }
  else
    {
      priv->has_secondary_text = FALSE;
      __gtk_widget_hide (priv->secondary_label);
    }

  setup_primary_label_font (message_dialog);
}

/**
 * __gtk_message_dialog_get_message_area:
 * @message_dialog: a #GtkMessageDialog
 *
 * Returns the message area of the dialog. This is the box where the
 * dialog's primary and secondary labels are packed. You can add your
 * own extra content to that box and it will appear below those labels,
 * on the right side of the dialog's image (or on the left for right-to-left
 * languages).  See __gtk_dialog_get_content_area() for the corresponding
 * function in the parent #GtkDialog.
 *
 * Return value: (transfer none): A #GtkVBox corresponding to the
 *     "message area" in the @message_dialog.
 *
 * Since: 2.22
 **/
GtkWidget *
__gtk_message_dialog_get_message_area (GtkMessageDialog *message_dialog)
{
  GtkMessageDialogPrivate *priv;

  g_return_val_if_fail (GTK_IS_MESSAGE_DIALOG (message_dialog), NULL);

  priv = gtk_message_dialog_get_instance_private (message_dialog);

  return priv->message_area;
}

static void
gtk_message_dialog_add_buttons (GtkMessageDialog* message_dialog,
				GtkButtonsType buttons)
{
  GtkDialog* dialog = GTK_DIALOG (message_dialog);

  switch (buttons)
    {
    case GTK_BUTTONS_NONE:
      /* nothing */
      break;

    case GTK_BUTTONS_OK:
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_OK,
                             GTK_RESPONSE_OK);
      break;

    case GTK_BUTTONS_CLOSE:
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_CLOSE,
                             GTK_RESPONSE_CLOSE);
      break;

    case GTK_BUTTONS_CANCEL:
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_CANCEL,
                             GTK_RESPONSE_CANCEL);
      break;

    case GTK_BUTTONS_YES_NO:
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_NO,
                             GTK_RESPONSE_NO);
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_YES,
                             GTK_RESPONSE_YES);
      __gtk_dialog_set_alternative_button_order (GTK_DIALOG (dialog),
					       GTK_RESPONSE_YES,
					       GTK_RESPONSE_NO,
					       -1);
      break;

    case GTK_BUTTONS_OK_CANCEL:
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_CANCEL,
                             GTK_RESPONSE_CANCEL);
      __gtk_dialog_add_button (dialog,
                             GTK_STOCK_OK,
                             GTK_RESPONSE_OK);
      __gtk_dialog_set_alternative_button_order (GTK_DIALOG (dialog),
					       GTK_RESPONSE_OK,
					       GTK_RESPONSE_CANCEL,
					       -1);
      break;
      
    default:
      g_warning ("Unknown GtkButtonsType");
      break;
    } 

  g_object_notify (G_OBJECT (message_dialog), "buttons");
}

static void
gtk_message_dialog_style_set (GtkWidget *widget,
                              GtkStyle  *prev_style)
{
  GtkMessageDialog *dialog = GTK_MESSAGE_DIALOG (widget);
  gboolean use_separator;
  GtkWidget *parent;
  gint border_width;

  parent = GTK_WIDGET (gtk_widget_get_props (gtk_message_dialog_get_props (widget)->image)->parent);

  if (parent)
    {
      __gtk_widget_style_get (widget, "message-border",
                            &border_width, NULL);
      
      __gtk_container_set_border_width (GTK_CONTAINER (parent),
                                      MAX (0, border_width - 7));
    }

  __gtk_widget_style_get (widget,
			"use-separator", &use_separator,
			NULL);

  ___gtk_dialog_set_ignore_separator (GTK_DIALOG (widget), FALSE);
  __gtk_dialog_set_has_separator (GTK_DIALOG (widget), use_separator);
  ___gtk_dialog_set_ignore_separator (GTK_DIALOG (widget), TRUE);

  setup_primary_label_font (dialog);

  GTK_WIDGET_CLASS (gtk_message_dialog_parent_class)->style_set (widget, prev_style);
}
