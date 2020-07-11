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
 * Some functions in the GTK accept a variable number of parameters / arguments.
 * It turns out that passing a variable number of arguments to a function is a
 * complicated procedure; therefore, we handle the special case of functions
 * which accept variable numbers of arguments here.
 */

/*
 * The include files below are NOT supposed to be the public include files;
 * they are supposed to be the private ones, since the private ones declare the
 * low-level STLWRT functions like '__gtk_widget_get_style' and we need those
 * function declarations.  In fact, we don't need the public headers in this
 * source file at all; here we are _defining_ the functions which are
 * _declared_ in the public header files.
 */

#include <gtk.h>
#include <stdarg.h>



/* From gtkaboutdialog.c and gtkaboutdialog.h: */

void	gtk_show_about_dialog(GtkWindow   *parent,
                              const gchar *first_property_name,
                              ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_property_name);
	
	__gtk_show_about_dialog(parent, first_property_name, parameters);
	
	va_end(parameters);
}


/* From gtkbindings.c and gtkbindings.h: */
void	gtk_binding_entry_add_signal(GtkBindingSet  *binding_set,
                                     guint           keyval,
                                     GdkModifierType modifiers,
                                     const gchar    *signal_name,
                                     guint           n_args,
                                     ...)
{
	va_list	parameters;
	
	va_start(parameters, n_args);
	
	__gtk_binding_entry_add_signal(binding_set, keyval, modifiers,
	                               signal_name, n_args, parameters);
	
	va_end(parameters);
}


/* From gtkcelllayout.c and gtkcelllayout.h: */
void  gtk_cell_layout_set_attributes     (GtkCellLayout         *cell_layout,
                                          GtkCellRenderer       *cell,
                                          ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, cell);
	
	__gtk_cell_layout_set_attributes(cell_layout, cell, parameters);
	
	va_end(parameters);
}


/* From gtkcontainer.c and gtkcontainer.h: */
void         gtk_container_add_with_properties	(GtkContainer	   *container,
						 GtkWidget	   *widget,
						 const gchar	   *first_prop_name,
						 ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_prop_name);
	
	__gtk_container_add_with_properties(container, widget, first_prop_name,
	                                    parameters);
	
	va_end(parameters);
}

void         gtk_container_child_set	(GtkContainer	   *container,
					 GtkWidget	   *child,
					 const gchar	   *first_prop_name,
					 ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_prop_name);
	
	__gtk_container_child_set(container, child, first_prop_name,
	                          parameters);
	
	va_end(parameters);
}

void         gtk_container_child_get	(GtkContainer	   *container,
					 GtkWidget	   *child,
					 const gchar	   *first_prop_name,
					 ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_prop_name);
	
	__gtk_container_child_get(container, child, first_prop_name,
	                          parameters);
	
	va_end(parameters);
}


/* From gtkdialog.c and gtkdialog.h: */
GtkWidget* gtk_dialog_new_with_buttons (const gchar     *title,
                                        GtkWindow       *parent,
                                        GtkDialogFlags   flags,
                                        const gchar     *first_button_text,
                                        ...)
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_dialog_new_with_buttons(title, parent, flags,
	                                             first_button_text,
	                                             parameters);
	
	va_end(parameters);
	
	return return_value;
}

void       gtk_dialog_add_buttons       (GtkDialog   *dialog,
                                         const gchar *first_button_text,
                                         ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_button_text);
	
	__gtk_dialog_add_buttons(dialog, first_button_text, parameters);
	
	va_end(parameters);
}

void     gtk_dialog_set_alternative_button_order (GtkDialog *dialog,
						  gint       first_response_id,
						  ...)
{
	va_list	parameters;
	
	va_start(parameters, first_response_id);
	
	__gtk_dialog_set_alternative_button_order(dialog, first_response_id,
	                                          parameters);
	
	va_end(parameters);
}


/* From gtkfilechooserdialog.c and gtkfilechooserdialog.h: */
GtkWidget *gtk_file_chooser_dialog_new  (const gchar          *title,
					 GtkWindow            *parent,
					 GtkFileChooserAction  action,
					 const gchar          *first_button_text,
					 ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_file_chooser_dialog_new(title, parent, action,
	                                             first_button_text,
	                                             parameters);
	
	va_end(parameters);
	
	return return_value;
}

GtkWidget *gtk_file_chooser_dialog_new_with_backend (const gchar          *title,
						     GtkWindow            *parent,
						     GtkFileChooserAction  action,
						     const gchar          *backend,
						     const gchar          *first_button_text,
						     ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_file_chooser_dialog_new_with_backend(title, parent,
	                                                          action, backend,
	                                                          first_button_text,
	                                                          parameters);
	
	va_end(parameters);
	
	return return_value;
}


/* From gtkinfobar.c and gtkinfobar.h: */
GtkWidget     *gtk_info_bar_new_with_buttons (const gchar    *first_button_text,
                                              ...)
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_info_bar_new_with_buttons (first_button_text,
	                                                parameters);
	
	va_end(parameters);
	
	return return_value;
}

void           gtk_info_bar_add_buttons   (GtkInfoBar     *info_bar,
                                           const gchar    *first_button_text,
                                           ...)
{
	va_list	parameters;
	
	va_start(parameters, first_button_text);
	
	__gtk_info_bar_add_buttons(info_bar, first_button_text, parameters);
	
	va_end(parameters);
}


/* From gtkliststore.c and gtkliststore.h: */
GtkListStore *gtk_list_store_new  (gint          n_columns,
				   ...)
{
	va_list	parameters;
	GtkListStore * return_value;
	
	va_start(parameters, n_columns);
	
	return_value = __gtk_list_store_new (n_columns,
	                                     parameters);
	
	va_end(parameters);
	
	return return_value;
}

void          gtk_list_store_set  (GtkListStore *list_store,
				   GtkTreeIter  *iter,
				   ...)
{
	va_list	parameters;
	
	va_start(parameters, iter);
	
	__gtk_list_store_set(list_store, iter, parameters);
	
	va_end(parameters);
}

void          gtk_list_store_insert_with_values  (GtkListStore *list_store,
						  GtkTreeIter  *iter,
						  gint          position,
						  ...)
{
	va_list	parameters;
	
	va_start(parameters, position);
	
	__gtk_list_store_insert_with_values(list_store, iter, position,
	                                    parameters);
	
	va_end(parameters);
}


/* From gtkmessagedialog.c and gtkmessagedialog.h: */
GtkWidget* gtk_message_dialog_new  (GtkWindow      *parent,
                                    GtkDialogFlags  flags,
                                    GtkMessageType  type,
                                    GtkButtonsType  buttons,
                                    const gchar    *message_format,
                                    ...) G_GNUC_PRINTF (5, 6)
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, message_format);
	
	return_value = __gtk_message_dialog_new (parent, flags, type, buttons,
	                                         message_format, parameters);
	
	va_end(parameters);
	
	return return_value;
}

GtkWidget* gtk_message_dialog_new_with_markup  (GtkWindow      *parent,
                                                GtkDialogFlags  flags,
                                                GtkMessageType  type,
                                                GtkButtonsType  buttons,
                                                const gchar    *message_format,
                                                ...) G_GNUC_PRINTF (5, 6)
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, message_format);
	
	return_value = __gtk_message_dialog_new (parent, flags, type, buttons,
	                                         message_format, parameters);
	
	va_end(parameters);
	
	return return_value;
}

void       gtk_message_dialog_format_secondary_text (GtkMessageDialog *message_dialog,
                                                     const gchar      *message_format,
                                                     ...) G_GNUC_PRINTF (2, 3)
{
	va_list	parameters;
	
	va_start(parameters, message_format);
	
	__gtk_message_dialog_format_secondary_text(message_dialog,
	                                           message_format, parameters);
	
	va_end(parameters);
}

void       gtk_message_dialog_format_secondary_markup (GtkMessageDialog *message_dialog,
                                                       const gchar      *message_format,
                                                       ...) G_GNUC_PRINTF (2, 3)
{
	va_list	parameters;
	
	va_start(parameters, message_format);
	
	__gtk_message_dialog_format_secondary_markup(message_dialog,
	                                             message_format,
	                                             parameters);
	
	va_end(parameters);
}


/* From gtkrecentchooserdialog.c and gtkrecentchooserdialog.h: */
GtkWidget *gtk_recent_chooser_dialog_new   (const gchar      *title,
					    GtkWindow        *parent,
					    const gchar      *first_button_text,
					    ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_recent_chooser_dialog_new (title, parent,
	                                                first_button_text,
	                                                parameters);
	
	va_end(parameters);
	
	return return_value;
}

GtkWidget *gtk_recent_chooser_dialog_new_for_manager (const gchar      *title,
						      GtkWindow        *parent,
						      GtkRecentManager *manager,
						      const gchar      *first_button_text,
						      ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_button_text);
	
	return_value = __gtk_recent_chooser_dialog_new_for_manager(title,
	                                                           parent,
	                                                           manager,
	                                                           first_button_text,
	                                                           parameters);
	
	va_end(parameters);
	
	return return_value;
}


/* From gtkstyle.c and gtkstyle.h: */
void gtk_style_get  (GtkStyle    *style,
                     GType        widget_type,
                     const gchar *first_property_name,
                     ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_property_name);
	
	__gtk_style_get(style, widget_type, first_property_name, parameters);
	
	va_end(parameters);
}


/* From gtktextbuffer.c and gtktextbuffer.h: */
void    gtk_text_buffer_insert_with_tags  (GtkTextBuffer     *buffer,
                                           GtkTextIter       *iter,
                                           const gchar       *text,
                                           gint               len,
                                           GtkTextTag        *first_tag,
                                           ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_tag);
	
	__gtk_text_buffer_insert_with_tags(buffer, iter, text, len, first_tag,
	                                   parameters);
	
	va_end(parameters);
}

void    gtk_text_buffer_insert_with_tags_by_name  (GtkTextBuffer     *buffer,
                                                   GtkTextIter       *iter,
                                                   const gchar       *text,
                                                   gint               len,
                                                   const gchar       *first_tag_name,
                                                   ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_tag_name);
	
	__gtk_text_buffer_insert_with_tags_by_name(buffer, iter, text, len,
	                                           first_tag_name, parameters);
	
	va_end(parameters);
}

GtkTextTag    *gtk_text_buffer_create_tag (GtkTextBuffer *buffer,
                                           const gchar   *tag_name,
                                           const gchar   *first_property_name,
                                           ...)
{
	va_list	parameters;
	GtkTextTag * return_value;
	
	va_start(parameters, first_property_name);
	
	return_value = __gtk_text_buffer_create_tag(buffer, tag_name,
	                                            first_property_name,
	                                            parameters);
	
	va_end(parameters);
	
	return return_value;
}


/* From gtktreemodel.c and gtktreemodel.h: */
GtkTreePath *gtk_tree_path_new_from_indices (gint  first_index,
					     ...)
{
	va_list	parameters;
	GtkTreePath * return_value;
	
	va_start(parameters, first_index);
	
	return_value = __gtk_tree_path_new_from_indices(first_index,
	                                                parameters);
	
	va_end(parameters);
	
	return return_value;
}

void  gtk_tree_model_get  (GtkTreeModel *tree_model,
			   GtkTreeIter  *iter,
			   ...)
{
	va_list	parameters;
	
	va_start(parameters, iter);
	
	__gtk_tree_model_get(tree_model, iter, parameters);
	
	va_end(parameters);
}


/* From gtktreestore.c and gtktreestore.h: */
GtkTreeStore *gtk_tree_store_new  (gint          n_columns,
				   ...)
{
	va_list	parameters;
	GtkTreeStore * return_value;
	
	va_start(parameters, n_columns);
	
	return_value = __gtk_tree_store_new(n_columns, parameters);
	
	va_end(parameters);
	
	return return_value;
}

void          gtk_tree_store_set  (GtkTreeStore *tree_store,
				   GtkTreeIter  *iter,
				   ...)
{
	va_list	parameters;
	
	va_start(parameters, iter);
	
	__gtk_tree_store_set(tree_store, iter, parameters);
	
	va_end(parameters);
}

void          gtk_tree_store_insert_with_values (GtkTreeStore *tree_store,
						 GtkTreeIter  *iter,
						 GtkTreeIter  *parent,
						 gint          position,
						 ...)
{
	va_list	parameters;
	
	va_start(parameters, position);
	
	__gtk_tree_store_insert_with_values(tree_store, iter, parent, position,
	                                    parameters);
	
	va_end(parameters);
}


/* From gtktreeviewcolumn.c and gtktreeviewcolumn.h: */
GtkTreeViewColumn  *gtk_tree_view_column_new_with_attributes (const gchar             *title,
							      GtkCellRenderer         *cell,
							      ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	GtkTreeStore * return_value;
	
	va_start(parameters, cell);
	
	return_value = __gtk_tree_view_column_new_with_attributes(title, cell,
	                                                          parameters);
	
	va_end(parameters);
	
	return return_value;
}

void  gtk_tree_view_column_set_attributes  (GtkTreeViewColumn       *tree_column,
					    GtkCellRenderer         *cell_renderer,
					    ...)
{
	va_list	parameters;
	
	va_start(parameters, cell_renderer);
	
	__gtk_tree_view_column_set_attributes(tree_column, cell_renderer,
	                                      parameters);
	
	va_end(parameters);
}


/* From gtktreeview.c and gtktreeview.h: */
gint  gtk_tree_view_insert_column_with_attributes (GtkTreeView     *tree_view,
						   gint             position,
						   const gchar     *title,
						   GtkCellRenderer *cell,
						   ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	gint * return_value;
	
	va_start(parameters, cell);
	
	return_value= __gtk_tree_view_insert_column_with_attributes(title,
	                                                            cell,
	                                                            parameters);
	
	va_end(parameters);
	
	return return_value;
}


/* From gtkwidget.c and gtkwidget.h: */
GtkWidget* gtk_widget_new  (GType		type,
			    const gchar	       *first_property_name,
			    ...)
{
	va_list	parameters;
	GtkWidget * return_value;
	
	va_start(parameters, first_property_name);
	
	return_value = __gtk_widget_new(type, first_property_name, parameters);
	
	va_end(parameters);
	
	return return_value;
}

void	   gtk_widget_set		  (GtkWidget	       *widget,
					   const gchar         *first_property_name,
					   ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_property_name);
	
	__gtk_widget_set(widget, first_property_name, parameters);
	
	va_end(parameters);
}

void gtk_widget_style_get          (GtkWidget	     *widget,
				    const gchar      *first_property_name,
				    ...) G_GNUC_NULL_TERMINATED
{
	va_list	parameters;
	
	va_start(parameters, first_property_name);
	
	__gtk_widget_style_get(widget, first_property_name, parameters);
	
	va_end(parameters);
}
