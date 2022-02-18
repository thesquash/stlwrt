#ifndef __STLWRT_OBJECT_DECLARATIONS_H__
#define __STLWRT_OBJECT_DECLARATIONS_H__

#include <stlwrt.h>

STLWRT_DECLARE_FTYPE_FPARENT (GdkAppLaunchContext, gdk_app_launch_context, GAppLaunchContext,

  GdkAppLaunchContextPrivate *priv;

)

STLWRT_DECLARE_FTYPE_FPARENT (GdkColormap, gdk_colormap, GObject,

  /*< public >*/
  gint       (size);
  GdkColor * (colors);

  /*< private >*/
  GdkVisual * (visual);

  gpointer  (windowing_data);

)

STLWRT_DECLARE_FTYPE_FPARENT(GdkDisplay, gdk_display, GObject,
  /*< private >*/
  GList * (queued_events);
  GList * (queued_tail);

  /* Information for determining if the latest button click
   * is part of a double-click or triple-click
   */
  guint32  (button_click_time[2]);	/* The last 2 button click times. */
  GdkWindow * (button_window[2]);  /* The last 2 windows to receive button presses. */
  gint  (button_number[2]);        /* The last 2 buttons to be pressed. */

  guint  (double_click_time);	/* Maximum time between clicks in msecs */
  GdkDevice * (core_pointer);	/* Core pointer device */

  const GdkDisplayPointerHooks * (pointer_hooks); /* Current hooks for querying pointer */
  
  guint  (closed) : 1;		/* Whether this display has been closed */
  guint  (ignore_core_events) : 1; /* Don't send core motion and button event */

  guint  (double_click_distance);	/* Maximum distance between clicks in pixels */
  gint  (button_x[2]);             /* The last 2 button click positions. */
  gint  (button_y[2]);

  GList * (pointer_grabs);
  GdkKeyboardGrabInfo  (keyboard_grab);
  GdkPointerWindowInfo  (pointer_info);

  /* Last reported event time from server */
  guint32  (last_event_time);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkDragContext, gdk_drag_context, GObject,
  /*< public >*/
  
  GdkDragProtocol  (protocol);

  gboolean  (is_source);
  
  GdkWindow * (source_window);
  GdkWindow * (dest_window);

  GList * (targets);
  GdkDragAction  (actions);
  GdkDragAction  (suggested_action);
  GdkDragAction  (action);

  guint32  (start_time);

  /*< private >*/
  
  gpointer  (windowing_data);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkDrawable, gdk_drawable, GObject, ;)

STLWRT_DECLARE_FTYPE_FPARENT(GdkGC, gdk_gc, GObject,
  gint  (clip_x_origin);
  gint  (clip_y_origin);
  gint  (ts_x_origin);
  gint  (ts_y_origin);

  GdkColormap * (colormap);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkImage, gdk_image, GObject,
  /*< public >*/
  
  GdkImageType	 (type); /* read only. */
  GdkVisual    * (visual);	    /* read only. visual used to create the image */
  GdkByteOrder	 (byte_order); /* read only. */
  gint		 (width);  /* read only. */
  gint		 (height); /* read only. */
  guint16	 (depth);  /* read only. */
  guint16	 (bpp);    /* read only. bytes per pixel */
  guint16	 (bpl);    /* read only. bytes per line */
  guint16        (bits_per_pixel); /* read only. bits per pixel */
  gpointer	 (mem);

  GdkColormap  * (colormap); /* read only. */

  /*< private >*/
  gpointer  (windowing_data); /* read only. */
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkDevice, gdk_device, GObject,
  /* All fields are read-only */
	  
  gchar * (name);
  GdkInputSource  (source);
  GdkInputMode  (mode);
  gboolean  (has_cursor);   /* TRUE if the X pointer follows device motion */
	  
  gint  (num_axes);
  GdkDeviceAxis * (axes);
	  
  gint  (num_keys);
  GdkDeviceKey * (keys);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkWindow, gdk_window, GdkDrawable,
  GdkDrawable *impl; /* window-system-specific delegate object */  
  
  GdkWindow *parent;

  gpointer user_data;

  gint x;
  gint y;
  
  gint extension_events;

  GList *filters;
  GList *children;

  GdkColor bg_color;
  GdkPixmap *bg_pixmap;
  
  GSList *paint_stack;
  
  GdkRegion *update_area;
  guint update_freeze_count;
  
  guint8 window_type;
  guint8 depth;
  guint8 resize_count;

  GdkWindowState state;
  
  guint guffaw_gravity : 1;
  guint input_only : 1;
  guint modal_hint : 1;
  guint composited : 1;
  
  guint destroyed : 2;

  guint accept_focus : 1;
  guint focus_on_map : 1;
  guint shaped : 1;
  
  GdkEventMask event_mask;

  guint update_and_descendants_freeze_count;

  GdkWindowRedirect *redirect;

  /* ^^^^^^^^^^ THIS PART IS PUBLIC. DON'T CHANGE ^^^^^^^^^^ */
  
  /* The GdkWindow that has the impl, ref:ed if another window.
   * This ref is required to keep the wrapper of the impl window alive
   * for as long as any GdkWindow references the impl. */
  GdkWindow *impl_window; 
  int abs_x;
  int abs_y; /* Absolute offset in impl */
  gint width;
  gint height;
  guint32 clip_tag;
  GdkRegion *clip_region; /* Clip region (wrt toplevel) in window coords */
  GdkRegion *clip_region_with_children; /* Clip region in window coords */
  GdkCursor *cursor;
  gint8 toplevel_window_type;
  guint synthesize_crossing_event_queued : 1;
  guint effective_visibility : 2;
  guint visibility : 2; /* The visibility wrt the toplevel (i.e. based on clip_region) */
  guint native_visibility : 2; /* the native visibility of a impl windows */
  guint viewable : 1; /* mapped and all parents mapped */
  guint applied_shape : 1;

  guint num_offscreen_children;
  GdkWindowPaint *implicit_paint;
  GdkInputWindow *input_window; /* only set for impl windows */

  GList *outstanding_moves;

  GdkRegion *shape;
  GdkRegion *input_shape;

  cairo_surface_t *cairo_surface;
  guint outstanding_surfaces; /* only set on impl window */

  cairo_pattern_t *background;
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkPangoRenderer, gdk_pango_renderer, PangoRenderer,
  GdkPangoRendererPrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkPixmap, gdk_pixmap, GdkDrawable,
  GdkDrawable * (impl);  /* window-system-specific delegate object */
  gint  (depth);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkScreen, gdk_screen, GObject,
  guint  (closed) : 1;

  GdkGC * (normal_gcs[32]);
  GdkGC * (exposure_gcs[32]);
  GdkGC * (subwindow_gcs[32]);

  cairo_font_options_t * (font_options);
  double  (resolution);	/* pixels/points scale factor for fonts */
)

STLWRT_DECLARE_FTYPE_FPARENT (GdkVisual, gdk_visual, GObject,
  GdkVisualType  (type);
  gint  (depth);
  GdkByteOrder  (byte_order);
  gint  (colormap_size);
  gint  (bits_per_rgb);

  guint32  (red_mask);
  gint  (red_shift);
  gint  (red_prec);

  guint32  (green_mask);
  gint  (green_shift);
  gint  (green_prec);

  guint32  (blue_mask);
  gint  (blue_shift);
  gint  (blue_prec);
)

STLWRT_DECLARE_FTYPE_FPARENT(GdkWindow, gdk_window, GdkDrawable,
  GdkDrawable * (impl); /* window-system-specific delegate object */
  
  GdkWindow * (parent);

  gpointer  (user_data);

  gint  (x);
  gint  (y);
  
  gint  (extension_events);

  GList * (filters);
  GList * (children);

  GdkColor  (bg_color);
  GdkPixmap * (bg_pixmap);
  
  GSList * (paint_stack);
  
  GdkRegion * (update_area);
  guint  (update_freeze_count);
  
  guint8  (window_type);
  guint8  (depth);
  guint8  (resize_count);

  GdkWindowState  (state);
  
  guint  (guffaw_gravity) : 1;
  guint  (input_only) : 1;
  guint  (modal_hint) : 1;
  guint  (composited) : 1;
  
  guint  (destroyed) : 2;

  guint  (accept_focus) : 1;
  guint  (focus_on_map) : 1;
  guint  (shaped) : 1;
  
  GdkEventMask  (event_mask);

  guint  (update_and_descendants_freeze_count);

  GdkWindowRedirect * (redirect);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkAboutDialog, gtk_about_dialog, GtkDialog,
  /*< private >*/
  gpointer  (private_data);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkAccelGroup, gtk_accel_group, GObject,
  guint                (lock_count);
  GdkModifierType      (modifier_mask);
  GSList             * (acceleratables);
  guint	               (n_accels);
  GtkAccelGroupEntry * (priv_accels);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkAccelLabel, gtk_accel_label, GtkLabel,
  guint           (gtk_reserved);
  guint           (accel_padding);      /* should be style property? */
  GtkWidget     * (accel_widget);       /* done*/
  GClosure      * (accel_closure);      /* has set function */
  GtkAccelGroup * (accel_group);        /* set by set_accel_closure() */
  gchar         * (accel_string);       /* has set function */
  guint16         (accel_string_width); /* seems to be private */
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkAccessible, gtk_accessible, AtkObject,
  /*
   * The GtkWidget whose properties and features are exported via this 
   * accessible instance.
   */
  GtkWidget * (widget);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkActionGroup, gtk_action_group, GObject,
  /*< private >*/

  GtkActionGroupPrivate * (private_data);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkAction, gtk_action, GObject,
  /*< private >*/

  GtkActionPrivate * (private_data);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkAdjustment, gtk_adjustment, GObject,
  gdouble  (lower);
  gdouble  (upper);
  gdouble  (value);
  gdouble  (step_increment);
  gdouble  (page_increment);
  gdouble  (page_size);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkAlignment, gtk_alignment, GtkBin,
  gfloat  (xalign);
  gfloat  (yalign);
  gfloat  (xscale);
  gfloat  (yscale);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkArrow, gtk_arrow, GtkMisc,
  gint16  (arrow_type);
  gint16  (shadow_type);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkAspectFrame, gtk_aspect_frame, GtkFrame,
  gfloat    (xalign);
  gfloat    (yalign);
  gfloat    (ratio);
  gboolean  (obey_child);

  GtkAllocation  (center_allocation);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkAssistant, gtk_assistant, GtkWindow,
  GtkWidget * (cancel);
  GtkWidget * (forward);
  GtkWidget * (back);
  GtkWidget * (apply);
  GtkWidget * (close);
  GtkWidget * (last);

  /*< private >*/
  gpointer * (dummy);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkButtonBox, gtk_button_box, GtkBox,
  gint  (child_min_width);
  gint  (child_min_height);
  gint  (child_ipad_x);
  gint  (child_ipad_y);
  GtkButtonBoxStyle  (layout_style);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkBin, gtk_bin, GtkContainer,
  GtkWidget * (child);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkBox, gtk_box, GtkContainer,
  /*< public >*/
  GList * (children);
  gint16  (spacing);
  guint  (homogeneous) : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkBuilder, gtk_builder, GObject,
  GtkBuilderPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT (GtkButton, gtk_button, GtkBin,
  GdkWindow * (event_window);

  gchar * (label_text);

  guint  (activate_timeout);

  guint  (constructed) : 1;
  guint  (in_button) : 1;
  guint  (button_down) : 1;
  guint  (relief) : 2;
  guint  (use_underline) : 1;
  guint  (use_stock) : 1;
  guint  (depressed) : 1;
  guint  (depress_on_activate) : 1;
  guint  (focus_on_click) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCalendar, gtk_calendar, GtkWidget,
  GtkStyle  * (header_style);
  GtkStyle  * (label_style);
  
  gint  (month);
  gint  (year);
  gint  (selected_day);
  
  gint  (day_month[6][7]);
  gint  (day[6][7]);
  
  gint  (num_marked_dates);
  gint  (marked_date[31]);
  GtkCalendarDisplayOptions   (display_flags);
  GdkColor  (marked_date_color[31]);
  
  GdkGC * (gc);			/* unused */
  GdkGC * (xor_gc);		/* unused */

  gint  (focus_row);
  gint  (focus_col);

  gint  (highlight_row);
  gint  (highlight_col);
  
  gpointer (priv);
  gchar  (grow_space [32]);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererAccel, gtk_cell_renderer_accel, GtkCellRendererText,
  /*< private >*/
  guint  (accel_key);
  GdkModifierType  (accel_mods);
  guint  (keycode);
  GtkCellRendererAccelMode  (accel_mode);

  GtkWidget * (edit_widget);
  GtkWidget * (grab_widget);
  GtkWidget * (sizing_label);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererCombo, gtk_cell_renderer_combo, GtkCellRendererText,
  GtkTreeModel * (model);
  gint           (text_column);
  gboolean       (has_entry);

  /*< private >*/
  guint          (focus_out_id);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkCellRenderer, gtk_cell_renderer, GObject,
  gfloat  (xalign);
  gfloat  (yalign);

  gint  (width);
  gint  (height);

  guint16  (xpad);
  guint16  (ypad);

  guint  (mode) : 2;
  guint  (visible) : 1;
  guint  (is_expander) : 1;
  guint  (is_expanded) : 1;
  guint  (cell_background_set) : 1;
  guint  (sensitive) : 1;
  guint  (editing) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererPixbuf, gtk_cell_renderer_pixbuf, GtkCellRenderer,
  /*< private >*/
  GdkPixbuf * (pixbuf);
  GdkPixbuf * (pixbuf_expander_open);
  GdkPixbuf * (pixbuf_expander_closed);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererProgress, gtk_cell_renderer_progress, GtkCellRenderer,
  /*< private >*/
  GtkCellRendererProgressPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkCellRendererSpin, gtk_cell_renderer_spin, GtkCellRendererText)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererSpinner, gtk_cell_renderer_spinner, GtkCellRenderer,
  GtkCellRendererSpinnerPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererText, gtk_cell_renderer_text, GtkCellRenderer,
  /*< private >*/
  gchar * (text);
  PangoFontDescription * (font);
  gdouble  (font_scale);
  PangoColor  (foreground);
  PangoColor  (background);

  PangoAttrList * (extra_attrs);

  PangoUnderline  (underline_style);

  gint  (rise);
  gint  (fixed_height_rows);

  guint  (strikethrough) : 1;

  guint  (editable)  : 1;

  guint  (scale_set) : 1;

  guint  (foreground_set) : 1;
  guint  (background_set) : 1;

  guint  (underline_set) : 1;

  guint  (rise_set) : 1;

  guint  (strikethrough_set) : 1;

  guint  (editable_set) : 1;
  guint  (calc_fixed_height) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererToggle, gtk_cell_renderer_toggle, GtkCellRenderer,
  /*< private >*/
  guint  (active) : 1;
  guint  (activatable) : 1;
  guint  (radio) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCellView, gtk_cell_view, GtkWidget,
  /*< private >*/
  GtkCellViewPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkCheckButton, gtk_check_button, GtkToggleButton)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCheckMenuItem, gtk_check_menu_item, GtkMenuItem,
  guint  (active) : 1;
  guint  (always_show_toggle) : 1;
  guint  (inconsistent) : 1;
  guint  (draw_as_radio) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkColorButton, gtk_color_button, GtkButton,
  /*< private >*/

  GtkColorButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkColorSelectionDialog, gtk_color_selection_dialog, GtkDialog,
  GtkWidget * (colorsel);
  GtkWidget * (ok_button);
  GtkWidget * (cancel_button);
  GtkWidget * (help_button);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkColorSelection, gtk_color_selection, GtkVBox,
  /* < private_data > */
  gpointer  (private_data);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkComboBoxEntry, gtk_combo_box_entry, GtkComboBox,
  /*< private >*/
  GtkComboBoxEntryPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkComboBox, gtk_combo_box, GtkBin,
  /*< private >*/
  GtkComboBoxPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkComboBoxText, gtk_combo_box_text, GtkComboBox,
  GtkComboBoxTextPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkContainer, gtk_container, GtkWidget,
  GtkWidget * (focus_child);

  guint  (border_width) : 16;

  /*< private >*/
  guint  (need_resize) : 1;
  guint  (resize_mode) : 2;
  guint  (reallocate_redraws) : 1;
  guint  (has_focus_chain) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkCustomPaperUnixDialog, gtk_custom_paper_unix_dialog, GtkDialog,
  GtkCustomPaperUnixDialogPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkDialog, gtk_dialog, GtkWindow,
  /*< public >*/
  GtkWidget * (vbox);
  GtkWidget * (action_area);

  /*< private >*/
  GtkWidget * (separator);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkDrawingArea, gtk_drawing_area, GtkWidget,
  gpointer  (draw_data);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkEntryBuffer, gtk_entry_buffer, GObject,
  /*< private >*/
  GtkEntryBufferPrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkEntryCompletion, gtk_entry_completion, GObject,
  /*< private >*/
  GtkEntryCompletionPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkEntry, gtk_entry, GtkWidget,
  gchar       * (text);                        /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */

  guint         (editable) : 1;
  guint         (visible)  : 1;
  guint         (overwrite_mode) : 1;
  guint         (in_drag) : 1;	            /* FIXME: Should be private?
                                                       Dragging within the selection */

  guint16       (text_length);                 /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */
  guint16       (text_max_length);             /* COMPAT: Deprecated, not used. Remove in GTK+ 3.x */

  /*< private >*/
  GdkWindow    * (text_area);
  GtkIMContext * (im_context);
  GtkWidget    * (popup_menu);

  gint           (current_pos);
  gint           (selection_bound);

  PangoLayout  * (cached_layout);

  guint          (cache_includes_preedit) : 1;
  guint          (need_im_reset)          : 1;
  guint          (has_frame)              : 1;
  guint          (activates_default)      : 1;
  guint          (cursor_visible)         : 1;
  guint          (in_click)               : 1; /* Flag so we don't select all when clicking in entry to focus in */
  guint          (is_cell_renderer)       : 1;
  guint          (editing_canceled)       : 1; /* Only used by GtkCellRendererText */ 
  guint          (mouse_cursor_obscured)  : 1;
  guint          (select_words)           : 1;
  guint          (select_lines)           : 1;
  guint          (resolved_dir)           : 4; /* PangoDirection */
  guint          (truncate_multiline)     : 1;

  guint          (button);
  guint          (blink_timeout);
  guint          (recompute_idle);
  gint           (scroll_offset);
  gint           (ascent);	                    /* font ascent in pango units  */
  gint           (descent);	            /* font descent in pango units */

  guint16        (x_text_size);	            /* allocated size, in bytes */
  guint16        (x_n_bytes);	            /* length in use, in bytes */

  guint16        (preedit_length);	            /* length of preedit string, in bytes */
  guint16        (preedit_cursor);	            /* offset of cursor within preedit string, in chars */

  gint           (dnd_position);		    /* In chars, -1 == no DND cursor */

  gint           (drag_start_x);
  gint           (drag_start_y);

  gunichar       (invisible_char);

  gint           (width_chars);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkEventBox, gtk_event_box, GtkBin)

STLWRT_DECLARE_VTYPE_VPARENT(GtkExpander, gtk_expander, GtkBin,
  GtkExpanderPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFileChooserButton, gtk_file_chooser_button, GtkHBox,
  /*< private >*/
  GtkHBox parent;

  GtkFileChooserButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFileChooserDialog, gtk_file_chooser_dialog, GtkDialog,
  GtkFileChooserDialogPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFileChooserDefault, _gtk_file_chooser_default, GtkVBox,
  GtkFileChooserAction action;

  GtkFileSystem *file_system;

  /* Save mode widgets */
  GtkWidget *save_widgets;
  GtkWidget *save_widgets_table;

  GtkWidget *save_folder_label;

  /* The file browsing widgets */
  GtkWidget *browse_widgets_box;
  GtkWidget *browse_header_box;
  GtkWidget *browse_shortcuts_tree_view;
  GtkWidget *browse_shortcuts_add_button;
  GtkWidget *browse_shortcuts_remove_button;
  GtkWidget *browse_shortcuts_popup_menu;
  GtkWidget *browse_shortcuts_popup_menu_remove_item;
  GtkWidget *browse_shortcuts_popup_menu_rename_item;
  GtkWidget *browse_files_tree_view;
  GtkWidget *browse_files_popup_menu;
  GtkWidget *browse_files_popup_menu_add_shortcut_item;
  GtkWidget *browse_files_popup_menu_hidden_files_item;
  GtkWidget *browse_files_popup_menu_size_column_item;
  GtkWidget *browse_new_folder_button;
  GtkWidget *browse_path_bar_hbox;
  GtkSizeGroup *browse_path_bar_size_group;
  GtkWidget *browse_path_bar;
  GtkWidget *browse_special_mode_icon;
  GtkWidget *browse_special_mode_label;
  GtkWidget *browse_select_a_folder_info_bar;
  GtkWidget *browse_select_a_folder_label;
  GtkWidget *browse_select_a_folder_icon;

  gulong toplevel_unmapped_id;

  GtkFileSystemModel *browse_files_model;
  char *browse_files_last_selected_name;

  StartupMode startup_mode;

  /* OPERATION_MODE_SEARCH */
  GtkWidget *search_hbox;
  GtkWidget *search_entry;
  GtkSearchEngine *search_engine;
  GtkQuery *search_query;
  GtkFileSystemModel *search_model;

  /* OPERATION_MODE_RECENT */
  GtkRecentManager *recent_manager;
  GtkFileSystemModel *recent_model;
  guint load_recent_id;

  GtkWidget *filter_combo_hbox;
  GtkWidget *filter_combo;
  GtkWidget *preview_box;
  GtkWidget *preview_label;
  GtkWidget *preview_widget;
  GtkWidget *extra_align;
  GtkWidget *extra_widget;

  GtkWidget *location_button;
  GtkWidget *location_entry_box;
  GtkWidget *location_label;
  GtkWidget *location_entry;
  LocationMode location_mode;

  GtkListStore *shortcuts_model;

  /* Filter for the shortcuts pane.  We filter out the "current folder" row and
   * the separator that we use for the "Save in folder" combo.
   */
  GtkTreeModel *shortcuts_pane_filter_model;
  
  /* Handles */
  GSList *loading_shortcuts;
  GSList *reload_icon_cancellables;
  GCancellable *file_list_drag_data_received_cancellable;
  GCancellable *update_current_folder_cancellable;
  GCancellable *should_respond_get_info_cancellable;
  GCancellable *file_exists_get_info_cancellable;
  GCancellable *update_from_entry_cancellable;
  GCancellable *shortcuts_activate_iter_cancellable;

  LoadState load_state;
  ReloadState reload_state;
  guint load_timeout_id;

  OperationMode operation_mode;

  GSList *pending_select_files;

  GtkFileFilter *current_filter;
  GSList *filters;

  int num_volumes;
  int num_shortcuts;
  int num_bookmarks;

  gulong volumes_changed_id;
  gulong bookmarks_changed_id;

  GFile *current_volume_file;
  GFile *current_folder;
  GFile *preview_file;
  char *preview_display_name;

  GtkTreeViewColumn *list_name_column;
  GtkCellRenderer *list_name_renderer;
  GtkTreeViewColumn *list_mtime_column;
  GtkTreeViewColumn *list_size_column;

  GSource *edited_idle;
  char *edited_new_text;

  gulong settings_signal_id;
  int icon_size;

  GSource *focus_entry_idle;

  gulong toplevel_set_focus_id;
  GtkWidget *toplevel_last_focus_widget;

  gint sort_column;
  GtkSortType sort_order;

#if 0
  GdkDragContext *shortcuts_drag_context;
  GSource *shortcuts_drag_outside_idle;
#endif

  /* Flags */

  guint local_only : 1;
  guint preview_widget_active : 1;
  guint use_preview_label : 1;
  guint select_multiple : 1;
  guint show_hidden : 1;
  guint do_overwrite_confirmation : 1;
  guint list_sort_ascending : 1;
  guint changing_folder : 1;
  guint shortcuts_current_folder_active : 1;
  guint has_cwd : 1;
  guint has_home : 1;
  guint has_desktop : 1;
  guint has_search : 1;
  guint show_size_column : 1;
  guint create_folders : 1;

#if 0
  guint shortcuts_drag_outside : 1;
#endif
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkFileChooserSettings, _gtk_file_chooser_settings, GObject,
  LocationMode location_mode;

  GtkSortType sort_order;
  gint sort_column;
  StartupMode startup_mode;

  int geometry_x;
  int geometry_y;
  int geometry_width;
  int geometry_height;

  guint settings_read    : 1;
  guint show_hidden      : 1;
  guint show_size_column : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFileChooserWidget, gtk_file_chooser_widget, GtkVBox,
  GtkFileChooserWidgetPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkFileSystem, _gtk_file_system, GObject, ;)

STLWRT_DECLARE_FTYPE_FPARENT(GtkFolder, _gtk_folder, GObject, ;)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFixed, gtk_fixed, GtkContainer,
  GList * (children);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFontButton, gtk_font_button, GtkButton,
  /*< private >*/
  GtkFontButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFontSelection, gtk_font_selection, GtkVBox,
  GtkWidget * (font_entry);        /* Used _get_family_entry() for consistency, -mr */
  GtkWidget * (family_list);
  GtkWidget * (font_style_entry);  /* Used _get_face_entry() for consistency, -mr */
  GtkWidget * (face_list);
  GtkWidget * (size_entry);
  GtkWidget * (size_list);
  GtkWidget * (pixels_button);     /* Unused, -mr */
  GtkWidget * (points_button);     /* Unused, -mr */
  GtkWidget * (filter_button);     /* Unused, -mr */
  GtkWidget * (preview_entry);

  PangoFontFamily * (family);	/* Current family */
  PangoFontFace * (face);		/* Current face */
  
  gint  (size);
  
  GdkFont * (font);		/* Cache for gdk_font_selection_get_font, so we can preserve
                                         * refcounting behavior
                                         */
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFontSelectionDialog, gtk_font_selection_dialog, GtkDialog,
  /*< private >*/
  GtkWidget * (fontsel);

  GtkWidget * (main_vbox);     /* Not wrapped with an API, can use GTK_DIALOG->vbox instead, -mr */
  GtkWidget * (action_area);   /* Not wrapped with an API, can use GTK_DIALOG->action_area instead, -mr */

  /*< public >*/
  GtkWidget * (ok_button);
  GtkWidget * (apply_button);
  GtkWidget * (cancel_button);

  /*< private >*/

  /* If the user changes the width of the dialog, we turn auto-shrink off.
   * (Unused now, autoshrink doesn't mean anything anymore -Yosh)
   */
  gint  (dialog_width);
  gboolean  (auto_resize);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkFrame, gtk_frame, GtkBin,
  GtkWidget * (label_widget);
  gint16  (shadow_type);
  gfloat  (label_xalign);
  gfloat  (label_yalign);

  GtkAllocation  (child_allocation);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkHandleBox, gtk_handle_box, GtkBin,
  GdkWindow      * (bin_window);	/* parent window for children */
  GdkWindow      * (float_window);
  GtkShadowType    (shadow_type);
  guint            (handle_position) : 2;
  guint            (float_window_mapped) : 1;
  guint            (child_detached) : 1;
  guint            (in_drag) : 1;
  guint            (shrink_on_detach) : 1;

  signed int       (snap_edge : 3); /* -1 == unset */

  /* Variables used during a drag
   */
  gint             (deskoff_x); /* Offset between root relative coords */
  gint             (deskoff_y); /* and deskrelative coords             */

  GtkAllocation    (attach_allocation);
  GtkAllocation    (float_allocation);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHButtonBox, gtk_hbutton_box, GtkButtonBox)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHBox, gtk_hbox, GtkBox)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHPaned, gtk_hpaned, GtkPaned)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHRuler, gtk_hruler, GtkRuler)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHScale, gtk_hscale, GtkScale)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHScrollbar, gtk_hscrollbar, GtkScrollbar)

STLWRT_DECLARE_FTYPE_VPARENT(GtkHSeparator, gtk_hseparator, GtkSeparator)

STLWRT_DECLARE_VTYPE_VPARENT(GtkHSV, gtk_hsv, GtkWidget,
  /* Private data */
  gpointer  (priv);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkIconFactory, gtk_icon_factory, GObject,
  GHashTable * (icons);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkIconTheme, gtk_icon_theme, GObject,
  GtkIconThemePrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkIconView, gtk_icon_view, GtkContainer,
  GtkIconViewPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkImage, gtk_image, GtkMisc,
  GtkImageType  (storage_type);

  union
  {
    GtkImagePixmapData pixmap;
    GtkImageImageData image;
    GtkImagePixbufData pixbuf;
    GtkImageStockData stock;
    GtkImageIconSetData icon_set;
    GtkImageAnimationData anim;
    GtkImageIconNameData name;
    GtkImageGIconData gicon;
  }  (data);

  /* Only used with GTK_IMAGE_PIXMAP, GTK_IMAGE_IMAGE */
  GdkBitmap * (mask);

  /* Only used with GTK_IMAGE_STOCK, GTK_IMAGE_ICON_SET, GTK_IMAGE_ICON_NAME */
  GtkIconSize  (icon_size);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkImageMenuItem, gtk_image_menu_item, GtkMenuItem,
  /*< private >*/
  GtkWidget      * (image);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkIMContext, gtk_im_context, GObject, ;)

STLWRT_DECLARE_VTYPE_FPARENT(GtkIMContextSimple, gtk_im_context_simple, GtkIMContext,
  GSList * (tables);

  guint  (compose_buffer[GTK_MAX_COMPOSE_LEN + 1]);
  gunichar  (tentative_match);
  gint  (tentative_match_len);

  guint  (in_hex_sequence) : 1;
  guint  (modifiers_dropped) : 1;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkIMMulticontext, gtk_im_multicontext, GtkIMContext,
  GtkIMContext * (slave);

  GtkIMMulticontextPrivate * (priv);

  gchar * (context_id);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkInfoBar, gtk_info_bar, GtkHBox,
  /*< private > */
  GtkInfoBarPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkInvisible, gtk_invisible, GtkWidget,
  gboolean    (has_user_ref_count);
  GdkScreen * (screen);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkLabel, gtk_label, GtkMisc,
  /*< private >*/
  gchar  * (label);
  guint    (jtype)            : 2;
  guint    (wrap)             : 1;
  guint    (use_underline)    : 1;
  guint    (use_markup)       : 1;
  guint    (ellipsize)        : 3;
  guint    (single_line_mode) : 1;
  guint    (have_transform)   : 1;
  guint    (in_click)         : 1;
  guint    (wrap_mode)        : 3;
  guint    (pattern_set)      : 1;
  guint    (track_links)      : 1;

  guint    (mnemonic_keyval);

  gchar  * (text);
  PangoAttrList * (attrs);
  PangoAttrList * (effective_attrs);

  PangoLayout * (layout);

  GtkWidget * (mnemonic_widget);
  GtkWindow * (mnemonic_window);

  GtkLabelSelectionInfo * (select_info);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkLayout, gtk_layout, GtkContainer,
  GList * (children);

  guint  (width);
  guint  (height);

  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);

  /*< public >*/
  GdkWindow * (bin_window);

  /*< private >*/
  GdkVisibilityState  (visibility);
  gint  (scroll_x);
  gint  (scroll_y);

  guint  (freeze_count);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkLinkButton, gtk_link_button, GtkButton,
  GtkLinkButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkListStore, gtk_list_store, GObject,
  /*< private >*/
  gint  (stamp);
  gpointer  (seq);		/* head of the list */
  gpointer  (_gtk_reserved1);
  GList * (sort_list);
  gint  (n_columns);
  gint  (sort_column_id);
  GtkSortType  (order);
  GType * (column_headers);
  gint  (length);
  GtkTreeIterCompareFunc  (default_sort_func);
  gpointer  (default_sort_data);
  GDestroyNotify  (default_sort_destroy);
  guint  (columns_dirty) : 1;
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkMenuBar, gtk_menu_bar, GtkMenuShell)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMenu, gtk_menu, GtkMenuShell,
  GtkWidget * (parent_menu_item);
  GtkWidget * (old_active_menu_item);

  GtkAccelGroup * (accel_group);
  gchar         * (accel_path);
  GtkMenuPositionFunc  (position_func);
  gpointer  (position_func_data);

  guint  (toggle_size);
  /* Do _not_ touch these widgets directly. We hide the reference
   * count from the toplevel to the menu, so it must be restored
   * before operating on these widgets
   */
  GtkWidget * (toplevel);
  
  GtkWidget * (tearoff_window);
  GtkWidget * (tearoff_hbox);
  GtkWidget * (tearoff_scrollbar);
  GtkAdjustment * (tearoff_adjustment);

  GdkWindow * (view_window);
  GdkWindow * (bin_window);

  gint  (scroll_offset);
  gint  (saved_scroll_offset);
  gint  (scroll_step);
  guint  (timeout_id);
  
  /* When a submenu of this menu is popped up, motion in this
   * region is ignored
   */
  GdkRegion * (navigation_region); /* unused */
  guint  (navigation_timeout);

  guint  (needs_destruction_ref_count) : 1;
  guint  (torn_off) : 1;
  /* The tearoff is active when it is torn off and the not-torn-off
   * menu is not popped up.
   */
  guint  (tearoff_active) : 1;

  guint  (scroll_fast) : 1;

  guint  (upper_arrow_visible) : 1;
  guint  (lower_arrow_visible) : 1;
  guint  (upper_arrow_prelight) : 1;
  guint  (lower_arrow_prelight) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMenuItem, gtk_menu_item, GtkBin,
  GtkWidget * (submenu);
  GdkWindow * (event_window);

  guint16  (toggle_size);
  guint16  (accelerator_width);
  gchar  * (accel_path);

  guint  (show_submenu_indicator) : 1;
  guint  (submenu_placement) : 1;
  guint  (submenu_direction) : 1;
  guint  (right_justify): 1;
  guint  (timer_from_keypress) : 1;
  guint  (from_menubar) : 1;
  guint  (timer);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMenuShell, gtk_menu_shell, GtkContainer,
  GList * (children);
  GtkWidget * (active_menu_item);
  GtkWidget * (parent_menu_shell);

  guint  (button);
  guint32  (activate_time);

  guint  (active) : 1;
  guint  (have_grab) : 1;
  guint  (have_xgrab) : 1;
  guint  (ignore_leave) : 1; /* unused */
  guint  (menu_flag) : 1;    /* unused */
  guint  (ignore_enter) : 1;
  guint  (keyboard_mode) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMenuToolButton, gtk_menu_tool_button, GtkToolButton,
  /*< private >*/
  GtkMenuToolButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMessageDialog, gtk_message_dialog, GtkDialog,
  GtkWidget * (image);
  GtkWidget * (label);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkMisc, gtk_misc, GtkWidget,
  gfloat  (xalign);
  gfloat  (yalign);

  guint16  (xpad);
  guint16  (ypad);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkMountOperation, gtk_mount_operation, GMountOperation,
  GtkMountOperationPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkNotebook, gtk_notebook, GtkContainer,
#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
  GtkNotebookPage * (cur_page);
#else
  gpointer  (cur_page);
#endif
  GList * (children);
  GList * (first_tab);		/* The first tab visible (for scrolling notebooks) */
  GList * (focus_tab);
  
  GtkWidget * (menu);
  GdkWindow * (event_window);
  
  guint32  (timer);
  
  guint16  (tab_hborder);
  guint16  (tab_vborder);
  
  guint  (show_tabs)          : 1;
  guint  (homogeneous)        : 1;
  guint  (show_border)        : 1;
  guint  (tab_pos)            : 2;
  guint  (scrollable)         : 1;
  guint  (in_child)           : 3;
  guint  (click_child)        : 3;
  guint  (button)             : 2;
  guint  (need_timer)         : 1;
  guint  (child_has_focus)    : 1;
  guint  (have_visible_child) : 1;
  guint  (focus_out)          : 1;	/* Flag used by ::move-focus-out implementation */

  guint  (has_before_previous) : 1;
  guint  (has_before_next)     : 1;
  guint  (has_after_previous)  : 1;
  guint  (has_after_next)      : 1;
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkOffscreenWindow, gtk_offscreen_window, GtkWindow)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPageSetupUnixDialog, gtk_page_setup_unix_dialog, GtkDialog,
  GtkPageSetupUnixDialogPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPaned, gtk_paned, GtkContainer,
  GtkWidget * (child1);
  GtkWidget * (child2);

  GdkWindow * (handle);
  GdkGC * (xor_gc);
  GdkCursorType  (cursor_type);

  /*< private >*/
  GdkRectangle  (handle_pos);

  gint  (child1_size);
  gint  (last_allocation);
  gint  (min_position);
  gint  (max_position);

  guint  (position_set) : 1;
  guint  (in_drag) : 1;
  guint  (child1_shrink) : 1;
  guint  (child1_resize) : 1;
  guint  (child2_shrink) : 1;
  guint  (child2_resize) : 1;
  guint  (orientation) : 1;
  guint  (in_recursion) : 1;
  guint  (handle_prelit) : 1;

  GtkWidget * (last_child1_focus);
  GtkWidget * (last_child2_focus);
  GtkPanedPrivate * (priv);

  gint  (drag_pos);
  gint  (original_position);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPathBar, gtk_path_bar, GtkContainer,
  GtkFileSystem *file_system;
  GFile *root_file;
  GFile *home_file;
  GFile *desktop_file;

  GCancellable *get_info_cancellable;

  GdkPixbuf *root_icon;
  GdkPixbuf *home_icon;
  GdkPixbuf *desktop_icon;

  GdkWindow *event_window;

  GList *button_list;
  GList *first_scrolled_button;
  GList *fake_root;
  GtkWidget *up_slider_button;
  GtkWidget *down_slider_button;
  guint settings_signal_id;
  gint icon_size;
  gint16 slider_width;
  gint16 spacing;
  gint16 button_offset;
  guint timer;
  guint slider_visible : 1;
  guint need_timer     : 1;
  guint ignore_click   : 1;
  guint scrolling_up   : 1;
  guint scrolling_down : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPlug, gtk_plug, GtkWindow,
  GdkWindow * (socket_window);
  GtkWidget * (modality_window);
  GtkWindowGroup * (modality_group);
  GHashTable * (grabbed_keys);

  guint  (same_app) : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkPrintBackend, gtk_print_backend, GObject,
  GtkPrintBackendPrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkPrinter, gtk_printer, GObject,
  GtkPrinterPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkPrinterOption, gtk_printer_option, GObject,
  char *name;
  char *display_text;
  GtkPrinterOptionType type;

  char *value;
  
  int num_choices;
  char **choices;
  char **choices_display;
  
  gboolean activates_default;

  gboolean has_conflict;
  char *group;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkPrinterOptionSet, gtk_printer_option_set, GObject,
  /*< private >*/
  GPtrArray *array;
  GHashTable *hash;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPrinterOptionWidget, gtk_printer_option_widget, GtkHBox,
  GtkPrinterOptionWidgetPrivate *priv;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkPrintJob, gtk_print_job, GObject,
  GtkPrintJobPrivate * (priv);

  /* Settings the client has to implement:
   * (These are read-only, set at initialization)
   */
  GtkPrintPages  (print_pages);
  GtkPageRange * (page_ranges);
  gint  (num_page_ranges);
  GtkPageSet  (page_set);
  gint  (num_copies);
  gdouble  (scale);
  guint  (rotate_to_orientation) : 1;
  guint  (collate)               : 1;
  guint  (reverse)               : 1;
  guint  (number_up);
  GtkNumberUpLayout  (number_up_layout);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkPrintOperation, gtk_print_operation, GObject,
  GtkPrintOperationPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkPrintUnixDialog, gtk_print_unix_dialog, GtkDialog,
  GtkPrintUnixDialogPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkProgressBar, gtk_progress_bar, GtkWidget,
  /* From gtkprogress.h */
  GtkAdjustment *adjustment;
  GdkPixmap     *offscreen_pixmap;
  gchar         *format;
  gfloat         x_align;
  gfloat         y_align;

  guint          show_text : 1;
  guint          activity_mode : 1;
  guint          use_text_format : 1;

  /* From gtkprogressbar.h */
  GtkProgressBarStyle        (bar_style);
  GtkProgressBarOrientation  (orientation);

  guint  (blocks);
  gint   (in_block);

  gint   (activity_pos);
  guint  (activity_step);
  guint  (activity_blocks);

  gdouble  (pulse_fraction);

  guint  (activity_dir) : 1;
  guint  (ellipsize) : 3;
  guint  (dirty) : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkQuery, _gtk_query, GObject,
  GtkQueryPrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkRadioAction, gtk_radio_action, GtkToggleAction,
  GtkRadioActionPrivate * (private_data);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRadioButton, gtk_radio_button, GtkCheckButton,
  GSList * (group);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRadioMenuItem, gtk_radio_menu_item, GtkCheckMenuItem,
  GSList * (group);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkRadioToolButton, gtk_radio_tool_button, GtkToggleToolButton)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRange, gtk_range, GtkWidget,
  GtkAdjustment * (adjustment);
  GtkUpdateType  (update_policy);
  guint  (inverted) : 1;

  /*< protected >*/

  guint  (flippable) : 1;

  /* Steppers are: < > ---- < >
   *               a b      c d
   */

  guint  (has_stepper_a) : 1;
  guint  (has_stepper_b) : 1;
  guint  (has_stepper_c) : 1;
  guint  (has_stepper_d) : 1;

  guint  (need_recalc) : 1;

  guint  (slider_size_fixed) : 1;

  gint  (min_slider_size);

  GtkOrientation  (orientation);

  /* Area of entire stepper + trough assembly in widget->window coords */
  GdkRectangle  (range_rect);
  /* Slider range along the long dimension, in widget->window coords */
  gint  (slider_start);
  gint  (slider_end);

  /* Round off value to this many digits, -1 for no rounding */
  gint  (round_digits);

  /*< private >*/
  guint  (trough_click_forward) : 1;  /* trough click was on the forward side of slider */
  guint  (update_pending) : 1;        /* need to emit value_changed */
  GtkRangeLayout * (layout);
  GtkRangeStepTimer * (timer);
  gint  (slide_initial_slider_position);
  gint  (slide_initial_coordinate);
  guint  (update_timeout_id);
  GdkWindow * (event_window);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkRcStyle, gtk_rc_style, GObject,
  /*< public >*/

  gchar *name;
  gchar *bg_pixmap_name[5];
  PangoFontDescription *font_desc;

  GtkRcFlags color_flags[5];
  GdkColor   fg[5];
  GdkColor   bg[5];
  GdkColor   text[5];
  GdkColor   base[5];

  gint xthickness;
  gint ythickness;

  /*< private >*/
  GArray *rc_properties;

  /* list of RC style lists including this RC style */
  GSList *rc_style_lists;

  GSList *icon_factories;

  guint engine_specified : 1;	/* The RC file specified the engine */
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkRecentAction, gtk_recent_action, GtkAction,
  /*< private >*/
  GtkRecentActionPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRecentChooserDialog, gtk_recent_chooser_dialog, GtkDialog,
  GtkRecentChooserDialogPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRecentChooserMenu, gtk_recent_chooser_menu, GtkMenu,
  GtkRecentChooserMenuPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRecentChooserWidget, gtk_recent_chooser_widget, GtkVBox,
  GtkRecentChooserWidgetPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkRecentManager, gtk_recent_manager, GObject,
  GtkRecentManagerPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkRuler, gtk_ruler, GtkWidget,
  GdkPixmap * (backing_store);
  GdkGC * (non_gr_exp_gc);		/* unused */
  GtkRulerMetric * (metric);
  gint  (xsrc);
  gint  (ysrc);
  gint  (slider_size);

  /* The upper limit of the ruler (in points) */
  gdouble  (lower);
  /* The lower limit of the ruler */
  gdouble  (upper);
  /* The position of the mark on the ruler */
  gdouble  (position);
  /* The maximum size of the ruler */
  gdouble  (max_size);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkScaleButton, gtk_scale_button, GtkButton,
  GtkWidget * (plus_button);
  GtkWidget * (minus_button);

  /*< private >*/
  GtkScaleButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkScale, gtk_scale, GtkRange,
  gint   (digits);
  guint  (draw_value) : 1;
  guint  (value_pos) : 2;
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkScrollbar, gtk_scrollbar, GtkRange)

STLWRT_DECLARE_VTYPE_VPARENT(GtkScrolledWindow, gtk_scrolled_window, GtkBin,
  /*< public >*/
  GtkWidget * (hscrollbar);
  GtkWidget * (vscrollbar);

  /*< private >*/
  guint  (hscrollbar_policy)      : 2;
  guint  (vscrollbar_policy)      : 2;
  guint  (hscrollbar_visible)     : 1;
  guint  (vscrollbar_visible)     : 1;
  guint  (window_placement)       : 2;
  guint  (focus_out)              : 1;	/* Flag used by ::move-focus-out implementation */

  guint16  (shadow_type);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkSearchEngineBeagle, _gtk_search_engine_beagle, GtkSearchEngine,
  GtkSearchEngineBeaglePrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkSearchEngine, _gtk_search_engine, GObject,
  GtkSearchEnginePrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkSearchEngineSimple, _gtk_search_engine_simple, GtkSearchEngine,
  GtkSearchEngineSimplePrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkSearchEngineTracker, _gtk_search_engine_tracker, GtkSearchEngine,
  GtkSearchEngineTrackerPrivate *priv;
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkSeparator, gtk_separator, GtkWidget)

STLWRT_DECLARE_FTYPE_VPARENT(GtkSeparatorMenuItem, gtk_separator_menu_item, GtkMenuItem)

STLWRT_DECLARE_VTYPE_VPARENT(GtkSeparatorToolItem, gtk_separator_tool_item, GtkToolItem,
  /*< private >*/
  GtkSeparatorToolItemPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkSettings, gtk_settings, GObject,
  GData  * (queued_settings);	/* of type GtkSettingsValue* */
  GtkSettingsPropertyValue * (property_values);

  GtkRcContext * (rc_context);
  GdkScreen    * (screen);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkSizeGroup, gtk_size_group, GObject,
  /* <private> */
  GSList * (widgets);

  guint8  (mode);

  guint  (have_width) : 1;
  guint  (have_height) : 1;
  guint  (ignore_hidden) : 1;

  GtkRequisition  (requisition);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkSocket, gtk_socket, GtkContainer,
  guint16  (request_width);
  guint16  (request_height);
  guint16  (current_width);
  guint16  (current_height);

  GdkWindow * (plug_window);
  GtkWidget * (plug_widget);

  gshort  (xembed_version); /* -1 == not xembed */
  guint  (same_app) : 1;
  guint  (focus_in) : 1;
  guint  (have_size) : 1;
  guint  (need_map) : 1;
  guint  (is_mapped) : 1;
  guint  (active) : 1;

  GtkAccelGroup * (accel_group);
  GtkWidget * (toplevel);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkSpinButton, gtk_spin_button, GtkEntry,
  GtkAdjustment * (adjustment);

  GdkWindow * (panel);

  guint32  (timer);

  gdouble  (climb_rate);
  gdouble  (timer_step);

  GtkSpinButtonUpdatePolicy  (update_policy);

  guint  (in_child) : 2;
  guint  (click_child) : 2; /* valid: GTK_ARROW_UP=0, GTK_ARROW_DOWN=1 or 2=NONE/BOTH */
  guint  (button) : 2;
  guint  (need_timer) : 1;
  guint  (timer_calls) : 3;
  guint  (digits) : 10;
  guint  (numeric) : 1;
  guint  (wrap) : 1;
  guint  (snap_to_ticks) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkSpinner, gtk_spinner, GtkDrawingArea,
  GtkSpinnerPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkStatusbar, gtk_status_bar, GtkHBox,
  GtkWidget * (frame);
  GtkWidget * (label);

  GSList * (messages);
  GSList * (keys);

  guint  (seq_context_id);
  guint  (seq_message_id);

  GdkWindow * (grip_window);

  guint  (has_resize_grip) : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkStatusIcon, gtk_status_icon, GObject,
  GtkStatusIconPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkStyle, gtk_style, GObject,
  /*< public >*/

  GdkColor fg[5];
  GdkColor bg[5];
  GdkColor light[5];
  GdkColor dark[5];
  GdkColor mid[5];
  GdkColor text[5];
  GdkColor base[5];
  GdkColor text_aa[5];		/* Halfway between text/base */

  GdkColor black;
  GdkColor white;
  PangoFontDescription *font_desc;

  gint xthickness;
  gint ythickness;

  GdkGC *fg_gc[5];
  GdkGC *bg_gc[5];
  GdkGC *light_gc[5];
  GdkGC *dark_gc[5];
  GdkGC *mid_gc[5];
  GdkGC *text_gc[5];
  GdkGC *base_gc[5];
  GdkGC *text_aa_gc[5];
  GdkGC *black_gc;
  GdkGC *white_gc;

  GdkPixmap *bg_pixmap[5];

  /*< private >*/

  gint attach_count;

  gint depth;
  GdkColormap *colormap;
  GdkFont *private_font;
  PangoFontDescription *private_font_desc; /* Font description for style->private_font or %NULL */

  /* the RcStyle from which this style was created */
  GtkRcStyle	 *rc_style;

  GSList	 *styles;	  /* of type GtkStyle* */
  GArray	 *property_cache;
  GSList         *icon_factories; /* of type GtkIconFactory* */
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkTable, gtk_table, GtkContainer,
  GList * (children);
  GtkTableRowCol * (rows);
  GtkTableRowCol * (cols);
  guint16  (nrows);
  guint16  (ncols);
  guint16  (column_spacing);
  guint16  (row_spacing);
  guint  (homogeneous) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkTearoffMenuItem, gtk_tearoff_menu_item, GtkMenuItem,
  guint  (torn_off) : 1;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTextBuffer, gtk_text_buffer, GObject,
  GtkTextTagTable * (tag_table);
  GtkTextBTree * (btree);

  GSList * (clipboard_contents_buffers);
  GSList * (selection_clipboards);

  GtkTextLogAttrCache * (log_attr_cache);

  guint  (user_action_count);

  /* Whether the buffer has been modified since last save */
  guint  (modified) : 1;

  guint  (has_selection) : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkTextChildAnchor, gtk_text_child_anchor, GObject,
  gpointer  (segment);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTextLayout, gtk_text_layout, GObject,
  /* width of the display area on-screen,
   * i.e. pixels we should wrap to fit inside. */
  gint screen_width;

  /* width/height of the total logical area being layed out */
  gint width;
  gint height;

  /* Pixel offsets from the left and from the top to be used when we
   * draw; these allow us to create left/top margins. We don't need
   * anything special for bottom/right margins, because those don't
   * affect drawing.
   */
  /* gint left_edge; */
  /* gint top_edge; */

  GtkTextBuffer *buffer;

  /* Default style used if no tags override it */
  GtkTextAttributes *default_style;

  /* Pango contexts used for creating layouts */
  PangoContext *ltr_context;
  PangoContext *rtl_context;

  /* A cache of one style; this is used to ensure
   * we don't constantly regenerate the style
   * over long runs with the same style. */
  GtkTextAttributes *one_style_cache;

  /* A cache of one line display. Getting the same line
   * many times in a row is the most common case.
   */
  GtkTextLineDisplay *one_display_cache;

  /* Whether we are allowed to wrap right now */
  gint wrap_loop_count;
  
  /* Whether to show the insertion cursor */
  guint cursor_visible : 1;

  /* For what GtkTextDirection to draw cursor GTK_TEXT_DIR_NONE -
   * means draw both cursors.
   */
  guint cursor_direction : 2;

  /* The keyboard direction is used to default the alignment when
     there are no strong characters.
  */
  guint keyboard_direction : 2;

  /* The preedit string and attributes, if any */

  gchar *preedit_string;
  PangoAttrList *preedit_attrs;
  gint preedit_len;
  gint preedit_cursor;

  guint overwrite_mode : 1;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkTextMark, gtk_text_mark, GObject,
  gpointer  (segment);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTextTag, gtk_text_tag, GObject,
  GtkTextTagTable * (table);

  char * (name);           /* Name of this tag.  This field is actually
                                 * a pointer to the key from the entry in
                                 * tkxt->tagTable, so it needn't be freed
                                 * explicitly. */
  int  (priority);  /* Priority of this tag within widget.  0
                         * means lowest priority.  Exactly one tag
                         * has each integer value between 0 and
                         * numTags-1. */
  /*
   * Information for displaying text with this tag.  The information
   * belows acts as an override on information specified by lower-priority
   * tags.  If no value is specified, then the next-lower-priority tag
   * on the text determins the value.  The text widget itself provides
   * defaults if no tag specifies an override.
   */

  GtkTextAttributes * (values);
  
  /* Flags for whether a given value is set; if a value is unset, then
   * this tag does not affect it.
   */
  guint  (bg_color_set) : 1;
  guint  (bg_stipple_set) : 1;
  guint  (fg_color_set) : 1;
  guint  (scale_set) : 1;
  guint  (fg_stipple_set) : 1;
  guint  (justification_set) : 1;
  guint  (left_margin_set) : 1;
  guint  (indent_set) : 1;
  guint  (rise_set) : 1;
  guint  (strikethrough_set) : 1;
  guint  (right_margin_set) : 1;
  guint  (pixels_above_lines_set) : 1;
  guint  (pixels_below_lines_set) : 1;
  guint  (pixels_inside_wrap_set) : 1;
  guint  (tabs_set) : 1;
  guint  (underline_set) : 1;
  guint  (wrap_mode_set) : 1;
  guint  (bg_full_height_set) : 1;
  guint  (invisible_set) : 1;
  guint  (editable_set) : 1;
  guint  (language_set) : 1;
  guint  (pg_bg_color_set) : 1;

  /* Whether these margins accumulate or override */
  guint  (accumulative_margin) : 1;

  guint  (pad1) : 1;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTextTagTable, gtk_text_tag_table, GObject,
  GHashTable * (hash);
  GSList * (anonymous);
  gint  (anon_count);

  GSList * (buffers);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkTextView, gtk_text_view, GtkContainer,
  struct _GtkTextLayout * (layout);
  GtkTextBuffer * (buffer);

  guint  (selection_drag_handler);
  guint  (scroll_timeout);

  /* Default style settings */
  gint  (pixels_above_lines);
  gint  (pixels_below_lines);
  gint  (pixels_inside_wrap);
  GtkWrapMode  (wrap_mode);
  GtkJustification  (justify);
  gint  (left_margin);
  gint  (right_margin);
  gint  (indent);
  PangoTabArray * (tabs);
  guint  (editable) : 1;

  guint  (overwrite_mode) : 1;
  guint  (cursor_visible) : 1;

  /* if we have reset the IM since the last character entered */  
  guint  (need_im_reset) : 1;

  guint  (accepts_tab) : 1;

  guint  (width_changed) : 1;

  /* debug flag - means that we've validated onscreen since the
   * last "invalidate" signal from the layout
   */
  guint  (onscreen_validated) : 1;

  guint  (mouse_cursor_obscured) : 1;

  GtkTextWindow * (text_window);
  GtkTextWindow * (left_window);
  GtkTextWindow * (right_window);
  GtkTextWindow * (top_window);
  GtkTextWindow * (bottom_window);

  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);

  gint  (xoffset);         /* Offsets between widget coordinates and buffer coordinates */
  gint  (yoffset);
  gint  (width);           /* Width and height of the buffer */
  gint  (height);

  /* The virtual cursor position is normally the same as the
   * actual (strong) cursor position, except in two circumstances:
   *
   * a) When the cursor is moved vertically with the keyboard
   * b) When the text view is scrolled with the keyboard
   *
   * In case a), virtual_cursor_x is preserved, but not virtual_cursor_y
   * In case b), both virtual_cursor_x and virtual_cursor_y are preserved.
   */
  gint  (virtual_cursor_x);   /* -1 means use actual cursor position */
  gint  (virtual_cursor_y);   /* -1 means use actual cursor position */

  GtkTextMark * (first_para_mark); /* Mark at the beginning of the first onscreen paragraph */
  gint  (first_para_pixels);       /* Offset of top of screen in the first onscreen paragraph */

  GtkTextMark * (dnd_mark);
  guint  (blink_timeout);

  guint  (first_validate_idle);        /* Idle to revalidate onscreen portion, runs before resize */
  guint  (incremental_validate_idle);  /* Idle to revalidate offscreen portions, runs after redraw */

  GtkIMContext * (im_context);
  GtkWidget * (popup_menu);

  gint  (drag_start_x);
  gint  (drag_start_y);

  GSList * (children);

  GtkTextPendingScroll * (pending_scroll);

  gint  (pending_place_cursor_button);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkToggleAction, gtk_toggle_action, GtkAction,
  /*< private >*/

  GtkToggleActionPrivate * (private_data);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToggleButton, gtk_toggle_button, GtkButton,
  guint  (active) : 1;
  guint  (draw_indicator) : 1;
  guint  (inconsistent) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToggleToolButton, gtk_toggle_tool_button, GtkToolButton,
  /*< private >*/
  GtkToggleToolButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolbar, gtk_toolbar, GtkContainer,
  /*< public >*/
  gint              (num_children);
  GList           * (children);
  GtkOrientation    (orientation);
  GtkToolbarStyle   (style);
  GtkIconSize       (icon_size);

  gpointer          (_tooltips);

  /*< private >*/
  gint              (button_maxw);		/* maximum width of homogeneous children */
  gint              (button_maxh);		/* maximum height of homogeneous children */

  guint            _gtk_reserved1;
  guint            _gtk_reserved2;

  guint             (style_set) : 1;
  guint             (icon_size_set) : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolButton, gtk_tool_button, GtkToolItem,
  /*< private >*/
  GtkToolButtonPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolItemGroup, gtk_tool_item_group, GtkContainer,
  GtkToolItemGroupPrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolItem, gtk_tool_item, GtkBin,
  /*< private >*/
  GtkToolItemPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolPalette, gtk_tool_palette, GtkContainer,
  GtkToolPalettePrivate *priv;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkTrayIcon, gtk_tray_icon, GtkPlug,
  GtkTrayIconPrivate *priv;
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkTreeModelFilter, gtk_tree_model_filter, GObject,
  /*< private >*/
  GtkTreeModelFilterPrivate * (priv);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTreeModelSort, gtk_tree_model_sort, GObject,
  /* < private > */
  gpointer  (root);
  gint  (stamp);
  guint  (child_flags);
  GtkTreeModel * (child_model);
  gint  (zero_ref_count);

  /* sort information */
  GList * (sort_list);
  gint  (sort_column_id);
  GtkSortType  (order);

  /* default sort */
  GtkTreeIterCompareFunc  (default_sort_func);
  gpointer  (default_sort_data);
  GDestroyNotify  (default_sort_destroy);

  /* signal ids */
  guint  (changed_id);
  guint  (inserted_id);
  guint  (has_child_toggled_id);
  guint  (deleted_id);
  guint  (reordered_id);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTreeSelection, gtk_tree_selection, GObject,
  /*< private >*/

  GtkTreeView * (tree_view);
  GtkSelectionMode  (type);
  GtkTreeSelectionFunc  (user_func);
  gpointer  (user_data);
  GDestroyNotify  (destroy);
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTreeStore, gtk_tree_store, GObject,
  gint  (stamp);
  gpointer  (root);
  gpointer  (last);
  gint  (n_columns);
  gint  (sort_column_id);
  GList * (sort_list);
  GtkSortType  (order);
  GType * (column_headers);
  GtkTreeIterCompareFunc  (default_sort_func);
  gpointer  (default_sort_data);
  GDestroyNotify  (default_sort_destroy);
  guint  (columns_dirty) : 1;
)

STLWRT_DECLARE_VTYPE_FPARENT(GtkTreeViewColumn, gtk_tree_view_column, GObject,
  GtkWidget * (tree_view);
  GtkWidget * (button);
  GtkWidget * (child);
  GtkWidget * (arrow);
  GtkWidget * (alignment);
  GdkWindow * (window);
  GtkCellEditable * (editable_widget);
  gfloat  (xalign);
  guint  (property_changed_signal);
  gint  (spacing);

  /* Sizing fields */
  /* see gtk+/doc/tree-column-sizing.txt for more information on them */
  GtkTreeViewColumnSizing  (column_type);
  gint  (requested_width);
  gint  (button_request);
  gint  (resized_width);
  gint  (width);
  gint  (fixed_width);
  gint  (min_width);
  gint  (max_width);

  /* dragging columns */
  gint  (drag_x);
  gint  (drag_y);

  gchar * (title);
  GList * (cell_list);

  /* Sorting */
  guint  (sort_clicked_signal);
  guint  (sort_column_changed_signal);
  gint  (sort_column_id);
  GtkSortType  (sort_order);

  /* Flags */
  guint  (visible)             : 1;
  guint  (resizable)           : 1;
  guint  (clickable)           : 1;
  guint  (dirty)               : 1;
  guint  (show_sort_indicator) : 1;
  guint  (maybe_reordered)     : 1;
  guint  (reorderable)         : 1;
  guint  (use_resized_width)   : 1;
  guint  (expand)              : 1;
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkTreeView, gtk_tree_view, GtkContainer,
  GtkTreeViewPrivate * (priv);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkUIManager, gtk_ui_manager, GObject,
  /*< private >*/

  GtkUIManagerPrivate * (private_data);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVButtonBox, gtk_vbutton_box, GtkButtonBox)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVBox, gtk_vbox, GtkBox)

STLWRT_DECLARE_VTYPE_VPARENT(GtkViewport, gtk_viewport, GtkBin,
  GtkShadowType  (shadow_type);
  GdkWindow * (view_window);
  GdkWindow * (bin_window);
  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);
)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVolumeButton, gtk_volume_button, GtkScaleButton)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVPaned, gtk_vpaned, GtkPaned)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVRuler, gtk_vruler, GtkRuler)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVScale, gtk_vscale, GtkScale)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVScrollbar, gtk_vscrollbar, GtkScrollbar)

STLWRT_DECLARE_FTYPE_VPARENT(GtkVSeparator, gtk_vseparator, GtkSeparator)

STLWRT_DECLARE_VTYPE_FPARENT(GtkWidget, gtk_widget, GObject,
  /* 16 bits of internally used private flags.
   * this will be packed into the same 4 byte alignment frame that
   * state and saved_state go. we therefore don't waste any new
   * space on this.
   * (Ed. -- Unless we're running on a 64-bit processor; then this is not
   * aligned correctly at all!)
   */
  guint16  (private_flags);
  
  /* The state of the widget. There are actually only
   *  5 widget states (defined in "gtkenums.h").
   */
  guint8  (state);
  
  /* The saved state of the widget. When a widget's state
   *  is changed to GTK_STATE_INSENSITIVE via
   *  "gtk_widget_set_state" or "gtk_widget_set_sensitive"
   *  the old state is kept around in this field. The state
   *  will be restored once the widget gets sensitive again.
   */
  guint8  (saved_state);
  
  /* The widget's name. If the widget does not have a name
   *  (the name is NULL), then its name (as returned by
   *  "gtk_widget_get_name") is its class's name.
   * Among other things, the widget name is used to determine
   *  the style to use for a widget.
   */
  gchar * (name);
  
  /*< public >*/

  /* The style for the widget. The style contains the
   *  colors the widget should be drawn in for each state
   *  along with graphics contexts used to draw with and
   *  the font to use for text.
   */
  GtkStyle * (style);
  
  /* The widget's desired size.
   */
  GtkRequisition  (requisition);
  
  /* The widget's allocated size.
   */
  GtkAllocation  (allocation);
  
  /* The widget's window or its parent window if it does
   *  not have a window. (Which will be indicated by the
   *  GTK_NO_WINDOW flag being set).
   */
  GdkWindow * (window);
  
  /* The widget's parent.
   */
  GtkWidget * (parent);
)

STLWRT_DECLARE_VTYPE_VPARENT(GtkWindow, gtk_window, GtkBin,
  gchar * (title);
  gchar * (wmclass_name);
  gchar * (wmclass_class);
  gchar * (wm_role);

  GtkWidget * (focus_widget);
  GtkWidget * (default_widget);
  GtkWindow * (transient_parent);
  GtkWindowGeometryInfo * (geometry_info);
  GdkWindow * (frame);
  GtkWindowGroup * (group);

  guint16  (configure_request_count);
  guint  (allow_shrink) : 1;
  guint  (allow_grow) : 1;
  guint  (configure_notify_received) : 1;
  /* The following flags are initially TRUE (before a window is mapped).
   * They cause us to compute a configure request that involves
   * default-only parameters. Once mapped, we set them to FALSE.
   * Then we set them to TRUE again on unmap (for position)
   * and on unrealize (for size).
   */
  guint  (need_default_position) : 1;
  guint  (need_default_size) : 1;
  guint  (position) : 3;
  guint  (type) : 4; /* GtkWindowType */ 
  guint  (has_user_ref_count) : 1;
  guint  (has_focus) : 1;

  guint  (modal) : 1;
  guint  (destroy_with_parent) : 1;
  
  guint  (has_frame) : 1;

  /* SF(gtk_window_iconify)() called before realization */
  guint  (iconify_initially) : 1;
  guint  (stick_initially) : 1;
  guint  (maximize_initially) : 1;
  guint  (decorated) : 1;
  
  guint  (type_hint) : 3; /* GdkWindowTypeHint if the hint is one of the original eight. If not, then
				* it contains GDK_WINDOW_TYPE_HINT_NORMAL
				*/
  guint  (gravity) : 5; /* GdkGravity */

  guint  (is_active) : 1;
  guint  (has_toplevel_focus) : 1;
  
  guint  (frame_left);
  guint  (frame_top);
  guint  (frame_right);
  guint  (frame_bottom);

  guint  (keys_changed_handler);
  
  GdkModifierType  (mnemonic_modifier);
  GdkScreen      * (screen);
)

STLWRT_DECLARE_FTYPE_FPARENT(GtkWindowGroup, gtk_window_group, GObject,
  GSList * (grabs);
)

#endif /* __STLWRT_OBJECT_DECLARATIONS_H__ */
