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

#include <string.h>

#include <gtkprogressbar.h>
#include <gtkprivate.h>
#include <gtkintl.h>



#define MIN_HORIZONTAL_BAR_WIDTH   150
#define MIN_HORIZONTAL_BAR_HEIGHT  20
#define MIN_VERTICAL_BAR_WIDTH     22
#define MIN_VERTICAL_BAR_HEIGHT    80

enum {
  PROP_0,

  /* Supported args */
  PROP_FRACTION,
  PROP_PULSE_STEP,
  PROP_ORIENTATION,
  PROP_TEXT,
  PROP_ELLIPSIZE,
  
  /* Deprecated args */
  PROP_ADJUSTMENT,
  PROP_BAR_STYLE,
  PROP_ACTIVITY_STEP,
  PROP_ACTIVITY_BLOCKS,
  PROP_DISCRETE_BLOCKS
};

static void gtk_progress_bar_set_property  (GObject             *object,
					    guint                prop_id,
					    const GValue        *value,
					    GParamSpec          *pspec);
static void gtk_progress_bar_get_property  (GObject             *object,
					    guint                prop_id,
					    GValue              *value,
					    GParamSpec          *pspec);
static gboolean gtk_progress_bar_expose    (GtkWidget           *widget,
					    GdkEventExpose      *event);
static void gtk_progress_bar_size_request  (GtkWidget           *widget,
					    GtkRequisition      *requisition);
static void gtk_progress_bar_style_set     (GtkWidget           *widget,
					    GtkStyle            *previous);
static void gtk_progress_bar_real_update   (GtkProgress         *progress);
static void gtk_progress_bar_paint         (GtkProgress         *progress);
static void gtk_progress_bar_act_mode_enter (GtkProgress        *progress);

static void __gtk_progress_bar_set_bar_style_internal       (GtkProgressBar *pbar,
							   GtkProgressBarStyle style);
static void __gtk_progress_bar_set_discrete_blocks_internal (GtkProgressBar *pbar,
							   guint           blocks);
static void __gtk_progress_bar_set_activity_step_internal   (GtkProgressBar *pbar,
							   guint           step);
static void __gtk_progress_bar_set_activity_blocks_internal (GtkProgressBar *pbar,
							   guint           blocks);


STLWRT_DEFINE_VTYPE (GtkProgressBar, gtk_progress_bar, GTK_TYPE_WIDGET, G_TYPE_FLAG_NONE, ;)

static void
gtk_progress_bar_class_init (GtkProgressBarClass *class)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;
  GtkProgressClass *progress_class;
  
  gobject_class = G_OBJECT_CLASS (class);
  widget_class = (GtkWidgetClass *) class;
  progress_class = (GtkProgressClass *) class;

  gobject_class->set_property = gtk_progress_bar_set_property;
  gobject_class->get_property = gtk_progress_bar_get_property;
  
  widget_class->expose_event = gtk_progress_bar_expose;
  widget_class->size_request = gtk_progress_bar_size_request;
  widget_class->style_set = gtk_progress_bar_style_set;

  progress_class->paint = gtk_progress_bar_paint;
  progress_class->update = gtk_progress_bar_real_update;
  progress_class->act_mode_enter = gtk_progress_bar_act_mode_enter;

  g_object_class_install_property (gobject_class,
                                   PROP_ADJUSTMENT,
                                   g_param_spec_object ("adjustment",
                                                        P_("Adjustment"),
                                                        P_("The GtkAdjustment connected to the progress bar (Deprecated)"),
                                                        GTK_TYPE_ADJUSTMENT,
                                                        GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_ORIENTATION,
                                   g_param_spec_enum ("orientation",
						      P_("Orientation"),
						      P_("Orientation and growth direction of the progress bar"),
						      GTK_TYPE_PROGRESS_BAR_ORIENTATION,
						      GTK_PROGRESS_LEFT_TO_RIGHT,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_BAR_STYLE,
                                   g_param_spec_enum ("bar-style",
						      P_("Bar style"),
						      P_("Specifies the visual style of the bar in percentage mode (Deprecated)"),
						      GTK_TYPE_PROGRESS_BAR_STYLE,
						      GTK_PROGRESS_CONTINUOUS,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_ACTIVITY_STEP,
                                   g_param_spec_uint ("activity-step",
						      P_("Activity Step"),
						      P_("The increment used for each iteration in activity mode (Deprecated)"),
						      0, G_MAXUINT, 3,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_ACTIVITY_BLOCKS,
                                   g_param_spec_uint ("activity-blocks",
						      P_("Activity Blocks"),
						      P_("The number of blocks which can fit in the progress bar area in activity mode (Deprecated)"),
						      2, G_MAXUINT, 5,
						      GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_DISCRETE_BLOCKS,
                                   g_param_spec_uint ("discrete-blocks",
						      P_("Discrete Blocks"),
						      P_("The number of discrete blocks in a progress bar (when shown in the discrete style)"),
						      2, G_MAXUINT, 10,
						      GTK_PARAM_READWRITE));
  
  g_object_class_install_property (gobject_class,
				   PROP_FRACTION,
				   g_param_spec_double ("fraction",
							P_("Fraction"),
							P_("The fraction of total work that has been completed"),
							0.0, 1.0, 0.0,
							GTK_PARAM_READWRITE));  
  
  g_object_class_install_property (gobject_class,
				   PROP_PULSE_STEP,
				   g_param_spec_double ("pulse-step",
							P_("Pulse Step"),
							P_("The fraction of total progress to move the bouncing block when pulsed"),
							0.0, 1.0, 0.1,
							GTK_PARAM_READWRITE));  
  
  g_object_class_install_property (gobject_class,
				   PROP_TEXT,
				   g_param_spec_string ("text",
							P_("Text"),
							P_("Text to be displayed in the progress bar"),
							NULL,
							GTK_PARAM_READWRITE));

  /**
   * GtkProgressBar:ellipsize:
   *
   * The preferred place to ellipsize the string, if the progressbar does 
   * not have enough room to display the entire string, specified as a 
   * #PangoEllisizeMode. 
   *
   * Note that setting this property to a value other than 
   * %PANGO_ELLIPSIZE_NONE has the side-effect that the progressbar requests 
   * only enough space to display the ellipsis "...". Another means to set a 
   * progressbar's width is __gtk_widget_set_size_request().
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_ELLIPSIZE,
                                   g_param_spec_enum ("ellipsize",
                                                      P_("Ellipsize"),
                                                      P_("The preferred place to ellipsize the string, if the progress bar "
                                                         "does not have enough room to display the entire string, if at all."),
						      PANGO_TYPE_ELLIPSIZE_MODE,
						      PANGO_ELLIPSIZE_NONE,
                                                      GTK_PARAM_READWRITE));
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("xspacing",
                                                             P_("XSpacing"),
                                                             P_("Extra spacing applied to the width of a progress bar."),
                                                             0, G_MAXINT, 7,
                                                             G_PARAM_READWRITE));
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("yspacing",
                                                             P_("YSpacing"),
                                                             P_("Extra spacing applied to the height of a progress bar."),
                                                             0, G_MAXINT, 7,
                                                             G_PARAM_READWRITE));

  /**
   * GtkProgressBar:min-horizontal-bar-width:
   *
   * The minimum horizontal width of the progress bar.
   *
   * Since: 2.14
   */
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("min-horizontal-bar-width",
                                                             P_("Min horizontal bar width"),
                                                             P_("The minimum horizontal width of the progress bar"),
                                                             1, G_MAXINT, MIN_HORIZONTAL_BAR_WIDTH,
                                                             G_PARAM_READWRITE));
  /**
   * GtkProgressBar:min-horizontal-bar-height:
   *
   * Minimum horizontal height of the progress bar.
   *
   * Since: 2.14
   */
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("min-horizontal-bar-height",
                                                             P_("Min horizontal bar height"),
                                                             P_("Minimum horizontal height of the progress bar"),
                                                             1, G_MAXINT, MIN_HORIZONTAL_BAR_HEIGHT,
                                                             G_PARAM_READWRITE));
  /**
   * GtkProgressBar:min-vertical-bar-width:
   *
   * The minimum vertical width of the progress bar.
   *
   * Since: 2.14
   */
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("min-vertical-bar-width",
                                                             P_("Min vertical bar width"),
                                                             P_("The minimum vertical width of the progress bar"),
                                                             1, G_MAXINT, MIN_VERTICAL_BAR_WIDTH,
                                                             G_PARAM_READWRITE));
  /**
   * GtkProgressBar:min-vertical-bar-height:
   *
   * The minimum vertical height of the progress bar.
   *
   * Since: 2.14
   */
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_int ("min-vertical-bar-height",
                                                             P_("Min vertical bar height"),
                                                             P_("The minimum vertical height of the progress bar"),
                                                             1, G_MAXINT, MIN_VERTICAL_BAR_HEIGHT,
                                                             G_PARAM_READWRITE));
}

static void
gtk_progress_bar_init (GtkProgressBar *pbar)
{
  gtk_progress_bar_get_props (pbar)->bar_style = GTK_PROGRESS_CONTINUOUS;
  gtk_progress_bar_get_props (pbar)->blocks = 10;
  gtk_progress_bar_get_props (pbar)->in_block = -1;
  gtk_progress_bar_get_props (pbar)->orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
  gtk_progress_bar_get_props (pbar)->pulse_fraction = 0.1;
  gtk_progress_bar_get_props (pbar)->activity_pos = 0;
  gtk_progress_bar_get_props (pbar)->activity_dir = 1;
  gtk_progress_bar_get_props (pbar)->activity_step = 3;
  gtk_progress_bar_get_props (pbar)->activity_blocks = 5;
  gtk_progress_bar_get_props (pbar)->ellipsize = PANGO_ELLIPSIZE_NONE;
}

static void
gtk_progress_bar_set_property (GObject      *object,
			       guint         prop_id,
			       const GValue *value,
			       GParamSpec   *pspec)
{
  GtkProgressBar *pbar;

  pbar = GTK_PROGRESS_BAR (object);

  switch (prop_id)
    {
    case PROP_ADJUSTMENT:
      gtk_progress_set_adjustment (GTK_PROGRESS (pbar),
				   GTK_ADJUSTMENT (g_value_get_object (value)));
      break;
    case PROP_ORIENTATION:
      __gtk_progress_bar_set_orientation (pbar, g_value_get_enum (value));
      break;
    case PROP_BAR_STYLE:
      __gtk_progress_bar_set_bar_style_internal (pbar, g_value_get_enum (value));
      break;
    case PROP_ACTIVITY_STEP:
      __gtk_progress_bar_set_activity_step_internal (pbar, g_value_get_uint (value));
      break;
    case PROP_ACTIVITY_BLOCKS:
      __gtk_progress_bar_set_activity_blocks_internal (pbar, g_value_get_uint (value));
      break;
    case PROP_DISCRETE_BLOCKS:
      __gtk_progress_bar_set_discrete_blocks_internal (pbar, g_value_get_uint (value));
      break;
    case PROP_FRACTION:
      __gtk_progress_bar_set_fraction (pbar, g_value_get_double (value));
      break;
    case PROP_PULSE_STEP:
      __gtk_progress_bar_set_pulse_step (pbar, g_value_get_double (value));
      break;
    case PROP_TEXT:
      __gtk_progress_bar_set_text (pbar, g_value_get_string (value));
      break;
    case PROP_ELLIPSIZE:
      __gtk_progress_bar_set_ellipsize (pbar, g_value_get_enum (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_progress_bar_get_property (GObject      *object,
			       guint         prop_id,
			       GValue       *value,
			       GParamSpec   *pspec)
{
  GtkProgressBar *pbar;

  pbar = GTK_PROGRESS_BAR (object);

  switch (prop_id)
    {
    case PROP_ADJUSTMENT:
      g_value_set_object (value, GTK_PROGRESS (pbar)->adjustment);
      break;
    case PROP_ORIENTATION:
      g_value_set_enum (value, gtk_progress_bar_get_props (pbar)->orientation);
      break;
    case PROP_BAR_STYLE:
      g_value_set_enum (value, gtk_progress_bar_get_props (pbar)->bar_style);
      break;
    case PROP_ACTIVITY_STEP:
      g_value_set_uint (value, gtk_progress_bar_get_props (pbar)->activity_step);
      break;
    case PROP_ACTIVITY_BLOCKS:
      g_value_set_uint (value, gtk_progress_bar_get_props (pbar)->activity_blocks);
      break;
    case PROP_DISCRETE_BLOCKS:
      g_value_set_uint (value, gtk_progress_bar_get_props (pbar)->blocks);
      break;
    case PROP_FRACTION:
      g_value_set_double (value, gtk_progress_get_current_percentage (GTK_PROGRESS (pbar)));
      break;
    case PROP_PULSE_STEP:
      g_value_set_double (value, gtk_progress_bar_get_props (pbar)->pulse_fraction);
      break;
    case PROP_TEXT:
      g_value_set_string (value, __gtk_progress_bar_get_text (pbar));
      break;
    case PROP_ELLIPSIZE:
      g_value_set_enum (value, gtk_progress_bar_get_props (pbar)->ellipsize);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/**
 * __gtk_progress_bar_new:
 *
 * Creates a new #GtkProgressBar.
 *
 * Returns: a #GtkProgressBar.
 */
GtkWidget*
__gtk_progress_bar_new (void)
{
  GtkWidget *pbar;

  pbar = g_object_new (GTK_TYPE_PROGRESS_BAR, NULL);

  return pbar;
}

/**
 * __gtk_progress_bar_new_with_adjustment:
 * @adjustment: (allow-none):
 *
 * Creates a new #GtkProgressBar with an associated #GtkAdjustment.
 *
 * Returns: (transfer none): a #GtkProgressBar.
 */
GtkWidget*
__gtk_progress_bar_new_with_adjustment (GtkAdjustment *adjustment)
{
  GtkWidget *pbar;

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), NULL);

  pbar = g_object_new (GTK_TYPE_PROGRESS_BAR,
			 "adjustment", adjustment,
			 NULL);

  return pbar;
}

static void
gtk_progress_bar_real_update (GtkProgress *progress)
{
  GtkProgressBar *pbar;
  GtkWidget *widget;

  g_return_if_fail (GTK_IS_PROGRESS (progress));

  pbar = GTK_PROGRESS_BAR (progress);
  widget = GTK_WIDGET (progress);
 
  if (gtk_progress_bar_get_props (pbar)->bar_style == GTK_PROGRESS_CONTINUOUS ||
      GTK_PROGRESS (pbar)->activity_mode)
    {
      if (GTK_PROGRESS (pbar)->activity_mode)
	{
	  guint size;
          
	  /* advance the block */

	  if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_LEFT_TO_RIGHT ||
	      gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	    {
              /* Update our activity step. */
              
              gtk_progress_bar_get_props (gtk_progress_bar_get_props (pbar))->activity_step = gtk_widget_get_props (widget)->allocation.width * gtk_progress_bar_get_props (gtk_progress_bar_get_props (pbar))->pulse_fraction;
              
	      size = MAX (2, gtk_widget_get_props (widget)->allocation.width / gtk_progress_bar_get_props (pbar)->activity_blocks);

	      if (gtk_progress_bar_get_props (pbar)->activity_dir == 0)
		{
		  gtk_progress_bar_get_props (pbar)->activity_pos += gtk_progress_bar_get_props (pbar)->activity_step;
		  if (gtk_progress_bar_get_props (pbar)->activity_pos + size >=
		      gtk_widget_get_props (widget)->allocation.width -
		      gtk_widget_get_props (widget)->style->xthickness)
		    {
		      gtk_progress_bar_get_props (pbar)->activity_pos = MAX (0, gtk_widget_get_props (widget)->allocation.width -
			gtk_widget_get_props (widget)->style->xthickness - size);
		      gtk_progress_bar_get_props (pbar)->activity_dir = 1;
		    }
		}
	      else
		{
		  gtk_progress_bar_get_props (pbar)->activity_pos -= gtk_progress_bar_get_props (pbar)->activity_step;
		  if (gtk_progress_bar_get_props (pbar)->activity_pos <= gtk_widget_get_props (widget)->style->xthickness)
		    {
		      gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->style->xthickness;
		      gtk_progress_bar_get_props (pbar)->activity_dir = 0;
		    }
		}
	    }
	  else
	    {
              /* Update our activity step. */
              
              gtk_progress_bar_get_props (gtk_progress_bar_get_props (pbar))->activity_step = gtk_widget_get_props (widget)->allocation.height * gtk_progress_bar_get_props (gtk_progress_bar_get_props (pbar))->pulse_fraction;
              
	      size = MAX (2, gtk_widget_get_props (widget)->allocation.height / gtk_progress_bar_get_props (pbar)->activity_blocks);

	      if (gtk_progress_bar_get_props (pbar)->activity_dir == 0)
		{
		  gtk_progress_bar_get_props (pbar)->activity_pos += gtk_progress_bar_get_props (pbar)->activity_step;
		  if (gtk_progress_bar_get_props (pbar)->activity_pos + size >=
		      gtk_widget_get_props (widget)->allocation.height -
		      gtk_widget_get_props (widget)->style->ythickness)
		    {
		      gtk_progress_bar_get_props (pbar)->activity_pos = MAX (0, gtk_widget_get_props (widget)->allocation.height -
			gtk_widget_get_props (widget)->style->ythickness - size);
		      gtk_progress_bar_get_props (pbar)->activity_dir = 1;
		    }
		}
	      else
		{
		  gtk_progress_bar_get_props (pbar)->activity_pos -= gtk_progress_bar_get_props (pbar)->activity_step;
		  if (gtk_progress_bar_get_props (pbar)->activity_pos <= gtk_widget_get_props (widget)->style->ythickness)
		    {
		      gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->style->ythickness;
		      gtk_progress_bar_get_props (pbar)->activity_dir = 0;
		    }
		}
	    }
	}
      gtk_progress_bar_get_props (pbar)->dirty = TRUE;
      __gtk_widget_queue_draw (GTK_WIDGET (progress));
    }
  else
    {
      gint in_block;
      
      in_block = -1 + (gint)(gtk_progress_get_current_percentage (progress) *
			     (gdouble)gtk_progress_bar_get_props (pbar)->blocks);
      
      if (gtk_progress_bar_get_props (pbar)->in_block != in_block)
	{
	  gtk_progress_bar_get_props (pbar)->in_block = in_block;
	  gtk_progress_bar_get_props (pbar)->dirty = TRUE;
	  __gtk_widget_queue_draw (GTK_WIDGET (progress));
	}
    }
}

static gboolean
gtk_progress_bar_expose (GtkWidget      *widget,
		     GdkEventExpose *event)
{
  GtkProgressBar *pbar;

  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (widget), FALSE);

  pbar = GTK_PROGRESS_BAR (widget);

  if (gtk_progress_bar_get_props (pbar)->dirty && __gtk_widget_is_drawable (widget))
    gtk_progress_bar_paint (GTK_PROGRESS (pbar));

  return GTK_WIDGET_CLASS (gtk_progress_bar_parent_class)->expose_event (widget, event);
}

static void
gtk_progress_bar_size_request (GtkWidget      *widget,
			       GtkRequisition *requisition)
{
  GtkProgress *progress;
  GtkProgressBar *pbar;
  gchar *buf;
  PangoRectangle logical_rect;
  PangoLayout *layout;
  gint width, height;
  gint xspacing, yspacing;
  gint min_width, min_height;

  g_return_if_fail (GTK_IS_PROGRESS_BAR (widget));
  g_return_if_fail (requisition != NULL);

  __gtk_widget_style_get (widget,
                        "xspacing", &xspacing,
                        "yspacing", &yspacing,
                        NULL);

  progress = GTK_PROGRESS (widget);
  pbar = GTK_PROGRESS_BAR (widget);

  width = 2 * gtk_widget_get_props (widget)->style->xthickness + xspacing;
  height = 2 * gtk_widget_get_props (widget)->style->ythickness + yspacing;

  if (progress->show_text && gtk_progress_bar_get_props (pbar)->bar_style != GTK_PROGRESS_DISCRETE)
    {
      if (!progress->adjustment)
	gtk_progress_set_adjustment (progress, NULL);

      buf = gtk_progress_get_text_from_value (progress, gtk_adjustment_get_props (progress->adjustment)->upper);

      layout = __gtk_widget_create_pango_layout (widget, buf);

      pango_layout_get_pixel_extents (layout, NULL, &logical_rect);
	  
      if (gtk_progress_bar_get_props (pbar)->ellipsize)
	{
	  PangoContext *context;
	  PangoFontMetrics *metrics;
	  gint char_width;
	  
	  /* The minimum size for ellipsized text is ~ 3 chars */
	  context = pango_layout_get_context (layout);
	  metrics = pango_context_get_metrics (context, gtk_widget_get_props (widget)->style->font_desc, pango_context_get_language (context));
	  
	  char_width = pango_font_metrics_get_approximate_char_width (metrics);
	  pango_font_metrics_unref (metrics);
	  
	  width += PANGO_PIXELS (char_width) * 3;
	}
      else
	width += logical_rect.width;
      
      height += logical_rect.height;

      g_object_unref (layout);
      g_free (buf);
    }
  
  if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_LEFT_TO_RIGHT ||
      gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
    __gtk_widget_style_get (widget,
			  "min-horizontal-bar-width", &min_width,
			  "min-horizontal-bar-height", &min_height,
			  NULL);
  else
    __gtk_widget_style_get (widget,
			  "min-vertical-bar-width", &min_width,
			  "min-vertical-bar-height", &min_height,
			  NULL);

  requisition->width = MAX (min_width, width);
  requisition->height = MAX (min_height, height);
}

static void
gtk_progress_bar_style_set (GtkWidget      *widget,
    GtkStyle *previous)
{
  GtkProgressBar *pbar = GTK_PROGRESS_BAR (widget);

  gtk_progress_bar_get_props (pbar)->dirty = TRUE;

  GTK_WIDGET_CLASS (gtk_progress_bar_parent_class)->style_set (widget, previous);
}

static void
gtk_progress_bar_act_mode_enter (GtkProgress *progress)
{
  GtkProgressBar *pbar;
  GtkWidget *widget;
  GtkProgressBarOrientation orientation;

  pbar = GTK_PROGRESS_BAR (progress);
  widget = GTK_WIDGET (progress);

  orientation = gtk_progress_bar_get_props (pbar)->orientation;
  if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) 
    {
      if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
	orientation = GTK_PROGRESS_RIGHT_TO_LEFT;
      else if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
    }
  
  /* calculate start pos */

  if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT ||
      orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
    {
      if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
	{
	  gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->style->xthickness;
	  gtk_progress_bar_get_props (pbar)->activity_dir = 0;
	}
      else
	{
	  gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->allocation.width - 
	    gtk_widget_get_props (widget)->style->xthickness - (gtk_widget_get_props (widget)->allocation.height - 
		gtk_widget_get_props (widget)->style->ythickness * 2);
	  gtk_progress_bar_get_props (pbar)->activity_dir = 1;
	}
    }
  else
    {
      if (orientation == GTK_PROGRESS_TOP_TO_BOTTOM)
	{
	  gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->style->ythickness;
	  gtk_progress_bar_get_props (pbar)->activity_dir = 0;
	}
      else
	{
	  gtk_progress_bar_get_props (pbar)->activity_pos = gtk_widget_get_props (widget)->allocation.height -
	    gtk_widget_get_props (widget)->style->ythickness - (gtk_widget_get_props (widget)->allocation.width - 
		gtk_widget_get_props (widget)->style->xthickness * 2);
	  gtk_progress_bar_get_props (pbar)->activity_dir = 1;
	}
    }
}

static void
gtk_progress_bar_get_activity (GtkProgressBar            *pbar,
			       GtkProgressBarOrientation  orientation,
			       gint                      *offset,
			       gint                      *amount)
{
  GtkWidget *widget = GTK_WIDGET (pbar);

  *offset = gtk_progress_bar_get_props (pbar)->activity_pos;

  switch (orientation)
    {
    case GTK_PROGRESS_LEFT_TO_RIGHT:
    case GTK_PROGRESS_RIGHT_TO_LEFT:
      *amount = MAX (2, gtk_widget_get_props (widget)->allocation.width / gtk_progress_bar_get_props (pbar)->activity_blocks);
      break;

    case GTK_PROGRESS_TOP_TO_BOTTOM:
    case GTK_PROGRESS_BOTTOM_TO_TOP:
      *amount = MAX (2, gtk_widget_get_props (widget)->allocation.height / gtk_progress_bar_get_props (pbar)->activity_blocks);
      break;
    }
}

static void
gtk_progress_bar_paint_activity (GtkProgressBar            *pbar,
				 GtkProgressBarOrientation  orientation)
{
  GtkWidget *widget = GTK_WIDGET (pbar);
  GtkProgress *progress = GTK_PROGRESS (pbar);
  GdkRectangle area;

  switch (orientation)
    {
    case GTK_PROGRESS_LEFT_TO_RIGHT:
    case GTK_PROGRESS_RIGHT_TO_LEFT:
      gtk_progress_bar_get_activity (pbar, orientation, &area.x, &area.width);
      area.y = gtk_widget_get_props (widget)->style->ythickness;
      area.height = gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness;
      break;

    case GTK_PROGRESS_TOP_TO_BOTTOM:
    case GTK_PROGRESS_BOTTOM_TO_TOP:
      gtk_progress_bar_get_activity (pbar, orientation, &area.y, &area.height);
      area.x = gtk_widget_get_props (widget)->style->xthickness;
      area.width = gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness;
      break;

    default:
      return;
      break;
    }

  __gtk_paint_box (gtk_widget_get_props (widget)->style,
		 progress->offscreen_pixmap,
		 GTK_STATE_PRELIGHT, GTK_SHADOW_OUT,
		 &area, widget, "bar",
		 area.x, area.y, area.width, area.height);
}

static void
gtk_progress_bar_paint_continuous (GtkProgressBar            *pbar,
				   gint                       amount,
				   GtkProgressBarOrientation  orientation)
{
  GdkRectangle area;
  GtkWidget *widget = GTK_WIDGET (pbar);

  if (amount <= 0)
    return;

  switch (orientation)
    {
    case GTK_PROGRESS_LEFT_TO_RIGHT:
    case GTK_PROGRESS_RIGHT_TO_LEFT:
      area.width = amount;
      area.height = gtk_widget_get_props (widget)->allocation.height - gtk_widget_get_props (widget)->style->ythickness * 2;
      area.y = gtk_widget_get_props (widget)->style->ythickness;
      
      area.x = gtk_widget_get_props (widget)->style->xthickness;
      if (orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	area.x = gtk_widget_get_props (widget)->allocation.width - amount - area.x;
      break;
      
    case GTK_PROGRESS_TOP_TO_BOTTOM:
    case GTK_PROGRESS_BOTTOM_TO_TOP:
      area.width = gtk_widget_get_props (widget)->allocation.width - gtk_widget_get_props (widget)->style->xthickness * 2;
      area.height = amount;
      area.x = gtk_widget_get_props (widget)->style->xthickness;
      
      area.y = gtk_widget_get_props (widget)->style->ythickness;
      if (orientation == GTK_PROGRESS_BOTTOM_TO_TOP)
	area.y = gtk_widget_get_props (widget)->allocation.height - amount - area.y;
      break;
      
    default:
      return;
      break;
    }
  
  __gtk_paint_box (gtk_widget_get_props (widget)->style,
		 GTK_PROGRESS (pbar)->offscreen_pixmap,
		 GTK_STATE_PRELIGHT, GTK_SHADOW_OUT,
		 &area, widget, "bar",
		 area.x, area.y, area.width, area.height);
}

static void
gtk_progress_bar_paint_discrete (GtkProgressBar            *pbar,
				 GtkProgressBarOrientation  orientation)
{
  GtkWidget *widget = GTK_WIDGET (pbar);
  gint i;

  for (i = 0; i <= gtk_progress_bar_get_props (pbar)->in_block; i++)
    {
      GdkRectangle area;
      gint space;

      switch (orientation)
	{
	case GTK_PROGRESS_LEFT_TO_RIGHT:
	case GTK_PROGRESS_RIGHT_TO_LEFT:
	  space = gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness;
	  
	  area.x = gtk_widget_get_props (widget)->style->xthickness + (i * space) / gtk_progress_bar_get_props (pbar)->blocks;
	  area.y = gtk_widget_get_props (widget)->style->ythickness;
	  area.width = gtk_widget_get_props (widget)->style->xthickness + ((i + 1) * space) / gtk_progress_bar_get_props (pbar)->blocks - area.x;
	  area.height = gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness;

	  if (orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	    area.x = gtk_widget_get_props (widget)->allocation.width - area.width - area.x;
	  break;
	  
	case GTK_PROGRESS_TOP_TO_BOTTOM:
	case GTK_PROGRESS_BOTTOM_TO_TOP:
	  space = gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness;
	  
	  area.x = gtk_widget_get_props (widget)->style->xthickness;
	  area.y = gtk_widget_get_props (widget)->style->ythickness + (i * space) / gtk_progress_bar_get_props (pbar)->blocks;
	  area.width = gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness;
	  area.height = gtk_widget_get_props (widget)->style->ythickness + ((i + 1) * space) / gtk_progress_bar_get_props (pbar)->blocks - area.y;
	  
	  if (orientation == GTK_PROGRESS_BOTTOM_TO_TOP)
	    area.y = gtk_widget_get_props (widget)->allocation.height - area.height - area.y;
	  break;

	default:
	  return;
	  break;
	}
      
      __gtk_paint_box (gtk_widget_get_props (widget)->style,
		     GTK_PROGRESS (pbar)->offscreen_pixmap,
		     GTK_STATE_PRELIGHT, GTK_SHADOW_OUT,
		     &area, widget, "bar",
		     area.x, area.y, area.width, area.height);
    }
}

static void
gtk_progress_bar_paint_text (GtkProgressBar            *pbar,
			     gint                       offset,
			     gint			amount,
			     GtkProgressBarOrientation  orientation)
{
  GtkProgress *progress = GTK_PROGRESS (pbar);
  GtkWidget *widget = GTK_WIDGET (pbar);
  gint x;
  gint y;
  gchar *buf;
  GdkRectangle rect;
  PangoLayout *layout;
  PangoRectangle logical_rect;
  GdkRectangle prelight_clip, start_clip, end_clip;
  gfloat text_xalign = progress->x_align;
  gfloat text_yalign = progress->y_align;

  if (__gtk_widget_get_direction (widget) != GTK_TEXT_DIR_LTR)
    text_xalign = 1.0 - text_xalign;

  buf = gtk_progress_get_current_text (progress);
  
  layout = __gtk_widget_create_pango_layout (widget, buf);
  pango_layout_set_ellipsize (layout, gtk_progress_bar_get_props (pbar)->ellipsize);
  if (gtk_progress_bar_get_props (pbar)->ellipsize)
    pango_layout_set_width (layout, gtk_widget_get_props (widget)->allocation.width * PANGO_SCALE);

  pango_layout_get_pixel_extents (layout, NULL, &logical_rect);
  
  x = gtk_widget_get_props (widget)->style->xthickness + 1 + text_xalign *
      (gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness -
       2 - logical_rect.width);

  y = gtk_widget_get_props (widget)->style->ythickness + 1 + text_yalign *
      (gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness -
       2 - logical_rect.height);

  rect.x = gtk_widget_get_props (widget)->style->xthickness;
  rect.y = gtk_widget_get_props (widget)->style->ythickness;
  rect.width = gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness;
  rect.height = gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness;

  prelight_clip = start_clip = end_clip = rect;

  switch (orientation)
    {
    case GTK_PROGRESS_LEFT_TO_RIGHT:
      if (offset != -1)
	prelight_clip.x = offset;
      prelight_clip.width = amount;
      start_clip.width = prelight_clip.x - start_clip.x;
      end_clip.x = start_clip.x + start_clip.width + prelight_clip.width;
      end_clip.width -= prelight_clip.width + start_clip.width;
      break;
      
    case GTK_PROGRESS_RIGHT_TO_LEFT:
      if (offset != -1)
	prelight_clip.x = offset;
      else
	prelight_clip.x = rect.x + rect.width - amount;
      prelight_clip.width = amount;
      start_clip.width = prelight_clip.x - start_clip.x;
      end_clip.x = start_clip.x + start_clip.width + prelight_clip.width;
      end_clip.width -= prelight_clip.width + start_clip.width;
      break;
       
    case GTK_PROGRESS_TOP_TO_BOTTOM:
      if (offset != -1)
	prelight_clip.y = offset;
      prelight_clip.height = amount;
      start_clip.height = prelight_clip.y - start_clip.y;
      end_clip.y = start_clip.y + start_clip.height + prelight_clip.height;
      end_clip.height -= prelight_clip.height + start_clip.height;
      break;
      
    case GTK_PROGRESS_BOTTOM_TO_TOP:
      if (offset != -1)
	prelight_clip.y = offset;
      else
	prelight_clip.y = rect.y + rect.height - amount;
      prelight_clip.height = amount;
      start_clip.height = prelight_clip.y - start_clip.y;
      end_clip.y = start_clip.y + start_clip.height + prelight_clip.height;
      end_clip.height -= prelight_clip.height + start_clip.height;
      break;
    }

  if (start_clip.width > 0 && start_clip.height > 0)
    __gtk_paint_layout (gtk_widget_get_props (widget)->style,
		      progress->offscreen_pixmap,
		      GTK_STATE_NORMAL,
		      FALSE,
		      &start_clip,
		      widget,
		      "progressbar",
		      x, y,
		      layout);

  if (end_clip.width > 0 && end_clip.height > 0)
    __gtk_paint_layout (gtk_widget_get_props (widget)->style,
		      progress->offscreen_pixmap,
		      GTK_STATE_NORMAL,
		      FALSE,
		      &end_clip,
		      widget,
		      "progressbar",
		      x, y,
		      layout);

  __gtk_paint_layout (gtk_widget_get_props (widget)->style,
		    progress->offscreen_pixmap,
		    GTK_STATE_PRELIGHT,
		    FALSE,
		    &prelight_clip,
		    widget,
		    "progressbar",
		    x, y,
		    layout);

  g_object_unref (layout);
  g_free (buf);
}

static void
gtk_progress_bar_paint (GtkProgress *progress)
{
  GtkProgressBar *pbar;
  GtkWidget *widget;

  GtkProgressBarOrientation orientation;

  g_return_if_fail (GTK_IS_PROGRESS_BAR (progress));

  pbar = GTK_PROGRESS_BAR (progress);
  widget = GTK_WIDGET (progress);

  orientation = gtk_progress_bar_get_props (pbar)->orientation;
  if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) 
    {
      if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
	orientation = GTK_PROGRESS_RIGHT_TO_LEFT;
      else if (gtk_progress_bar_get_props (pbar)->orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
    }
 
  if (progress->offscreen_pixmap)
    {
      __gtk_paint_box (gtk_widget_get_props (widget)->style,
		     progress->offscreen_pixmap,
		     GTK_STATE_NORMAL, GTK_SHADOW_IN, 
		     NULL, widget, "trough",
		     0, 0,
		     gtk_widget_get_props (widget)->allocation.width,
		     gtk_widget_get_props (widget)->allocation.height);
      
      if (progress->activity_mode)
	{
	  gtk_progress_bar_paint_activity (pbar, orientation);

	  if (GTK_PROGRESS (pbar)->show_text)
	    {
	      gint offset;
	      gint amount;

	      gtk_progress_bar_get_activity (pbar, orientation, &offset, &amount);
	      gtk_progress_bar_paint_text (pbar, offset, amount, orientation);
	    }
	}
      else
	{
	  gint amount;
	  gint space;
	  
	  if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT ||
	      orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	    space = gtk_widget_get_props (widget)->allocation.width - 2 * gtk_widget_get_props (widget)->style->xthickness;
	  else
	    space = gtk_widget_get_props (widget)->allocation.height - 2 * gtk_widget_get_props (widget)->style->ythickness;
	  
	  amount = space *
	    gtk_progress_get_current_percentage (GTK_PROGRESS (pbar));
	  
	  if (gtk_progress_bar_get_props (pbar)->bar_style == GTK_PROGRESS_CONTINUOUS)
	    {
	      gtk_progress_bar_paint_continuous (pbar, amount, orientation);

	      if (GTK_PROGRESS (pbar)->show_text)
		gtk_progress_bar_paint_text (pbar, -1, amount, orientation);
	    }
	  else
	    gtk_progress_bar_paint_discrete (pbar, orientation);
	}

      gtk_progress_bar_get_props (pbar)->dirty = FALSE;
    }
}

static void
__gtk_progress_bar_set_bar_style_internal (GtkProgressBar     *pbar,
					 GtkProgressBarStyle bar_style)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  if (gtk_progress_bar_get_props (pbar)->bar_style != bar_style)
    {
      gtk_progress_bar_get_props (pbar)->bar_style = bar_style;

      if (__gtk_widget_is_drawable (GTK_WIDGET (pbar)))
	__gtk_widget_queue_resize (GTK_WIDGET (pbar));

      g_object_notify (G_OBJECT (pbar), "bar-style");
    }
}

static void
__gtk_progress_bar_set_discrete_blocks_internal (GtkProgressBar *pbar,
					       guint           blocks)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  g_return_if_fail (blocks > 1);

  if (gtk_progress_bar_get_props (pbar)->blocks != blocks)
    {
      gtk_progress_bar_get_props (pbar)->blocks = blocks;

      if (__gtk_widget_is_drawable (GTK_WIDGET (pbar)))
	__gtk_widget_queue_resize (GTK_WIDGET (pbar));

      g_object_notify (G_OBJECT (pbar), "discrete-blocks");
    }
}

static void
__gtk_progress_bar_set_activity_step_internal (GtkProgressBar *pbar,
					     guint           step)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  if (gtk_progress_bar_get_props (pbar)->activity_step != step)
    {
      gtk_progress_bar_get_props (pbar)->activity_step = step;
      g_object_notify (G_OBJECT (pbar), "activity-step");
    }
}

static void
__gtk_progress_bar_set_activity_blocks_internal (GtkProgressBar *pbar,
					       guint           blocks)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  g_return_if_fail (blocks > 1);

  if (gtk_progress_bar_get_props (pbar)->activity_blocks != blocks)
    {
      gtk_progress_bar_get_props (pbar)->activity_blocks = blocks;
      g_object_notify (G_OBJECT (pbar), "activity-blocks");
    }
}

/*******************************************************************/

/**
 * __gtk_progress_bar_set_fraction:
 * @pbar: a #GtkProgressBar
 * @fraction: fraction of the task that's been completed
 * 
 * Causes the progress bar to "fill in" the given fraction
 * of the bar. The fraction should be between 0.0 and 1.0,
 * inclusive.
 * 
 **/
void
__gtk_progress_bar_set_fraction (GtkProgressBar *pbar,
                               gdouble         fraction)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  /* If we know the percentage, we don't want activity mode. */
  gtk_progress_set_activity_mode (GTK_PROGRESS (pbar), FALSE);
  
  /* We use the deprecated GtkProgress interface internally.
   * Once everything's been deprecated for a good long time,
   * we can clean up all this code.
   */
  gtk_progress_set_percentage (GTK_PROGRESS (pbar), fraction);

  g_object_notify (G_OBJECT (pbar), "fraction");
}

/**
 * __gtk_progress_bar_pulse:
 * @pbar: a #GtkProgressBar
 * 
 * Indicates that some progress is made, but you don't know how much.
 * Causes the progress bar to enter "activity mode," where a block
 * bounces back and forth. Each call to __gtk_progress_bar_pulse()
 * causes the block to move by a little bit (the amount of movement
 * per pulse is determined by __gtk_progress_bar_set_pulse_step()).
 **/
void
__gtk_progress_bar_pulse (GtkProgressBar *pbar)
{  
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  /* If we don't know the percentage, we must want activity mode. */
  gtk_progress_set_activity_mode (GTK_PROGRESS (pbar), TRUE);

  /* Sigh. */
  gtk_progress_bar_real_update (GTK_PROGRESS (pbar));
}

/**
 * __gtk_progress_bar_set_text:
 * @pbar: a #GtkProgressBar
 * @text: (allow-none): a UTF-8 string, or %NULL 
 * 
 * Causes the given @text to appear superimposed on the progress bar.
 **/
void
__gtk_progress_bar_set_text (GtkProgressBar *pbar,
                           const gchar    *text)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  
  gtk_progress_set_show_text (GTK_PROGRESS (pbar), text && *text);
  gtk_progress_set_format_string (GTK_PROGRESS (pbar), text);
  
  /* We don't support formats in this interface, but turn
   * them back on for NULL, which should put us back to
   * the initial state.
   */
  GTK_PROGRESS (pbar)->use_text_format = (text == NULL);
  
  g_object_notify (G_OBJECT (pbar), "text");
}

/**
 * __gtk_progress_bar_set_pulse_step:
 * @pbar: a #GtkProgressBar
 * @fraction: fraction between 0.0 and 1.0
 * 
 * Sets the fraction of total progress bar length to move the
 * bouncing block for each call to __gtk_progress_bar_pulse().
 **/
void
__gtk_progress_bar_set_pulse_step   (GtkProgressBar *pbar,
                                   gdouble         fraction)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  
  gtk_progress_bar_get_props (pbar)->pulse_fraction = fraction;

  g_object_notify (G_OBJECT (pbar), "pulse-step");
}

void
__gtk_progress_bar_update (GtkProgressBar *pbar,
			 gdouble         percentage)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  /* Use of __gtk_progress_bar_update() is deprecated ! 
   * Use gtk_progress_bar_set_percentage ()
   */   

  gtk_progress_set_percentage (GTK_PROGRESS (pbar), percentage);
}

/**
 * __gtk_progress_bar_set_orientation:
 * @pbar: a #GtkProgressBar
 * @orientation: orientation of the progress bar
 * 
 * Causes the progress bar to switch to a different orientation
 * (left-to-right, right-to-left, top-to-bottom, or bottom-to-top). 
 **/
void
__gtk_progress_bar_set_orientation (GtkProgressBar           *pbar,
				  GtkProgressBarOrientation orientation)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  if (gtk_progress_bar_get_props (pbar)->orientation != orientation)
    {
      gtk_progress_bar_get_props (pbar)->orientation = orientation;

      if (__gtk_widget_is_drawable (GTK_WIDGET (pbar)))
	__gtk_widget_queue_resize (GTK_WIDGET (pbar));

      g_object_notify (G_OBJECT (pbar), "orientation");
    }
}

/**
 * __gtk_progress_bar_get_text:
 * @pbar: a #GtkProgressBar
 * 
 * Retrieves the text displayed superimposed on the progress bar,
 * if any, otherwise %NULL. The return value is a reference
 * to the text, not a copy of it, so will become invalid
 * if you change the text in the progress bar.
 * 
 * Return value: text, or %NULL; this string is owned by the widget
 * and should not be modified or freed.
 **/
const gchar*
__gtk_progress_bar_get_text (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), NULL);

  if (GTK_PROGRESS (pbar)->use_text_format)
    return NULL;
  else
    return GTK_PROGRESS (pbar)->format;
}

/**
 * __gtk_progress_bar_get_fraction:
 * @pbar: a #GtkProgressBar
 * 
 * Returns the current fraction of the task that's been completed.
 * 
 * Return value: a fraction from 0.0 to 1.0
 **/
gdouble
__gtk_progress_bar_get_fraction (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), 0);

  return gtk_progress_get_current_percentage (GTK_PROGRESS (pbar));
}

/**
 * __gtk_progress_bar_get_pulse_step:
 * @pbar: a #GtkProgressBar
 * 
 * Retrieves the pulse step set with __gtk_progress_bar_set_pulse_step()
 * 
 * Return value: a fraction from 0.0 to 1.0
 **/
gdouble
__gtk_progress_bar_get_pulse_step (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), 0);

  return gtk_progress_bar_get_props (pbar)->pulse_fraction;
}

/**
 * __gtk_progress_bar_get_orientation:
 * @pbar: a #GtkProgressBar
 * 
 * Retrieves the current progress bar orientation.
 * 
 * Return value: orientation of the progress bar
 **/
GtkProgressBarOrientation
__gtk_progress_bar_get_orientation (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), 0);

  return gtk_progress_bar_get_props (pbar)->orientation;
}

void
__gtk_progress_bar_set_bar_style (GtkProgressBar     *pbar,
				GtkProgressBarStyle bar_style)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  __gtk_progress_bar_set_bar_style_internal (pbar, bar_style);
}

void
__gtk_progress_bar_set_discrete_blocks (GtkProgressBar *pbar,
				      guint           blocks)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  g_return_if_fail (blocks > 1);

  __gtk_progress_bar_set_discrete_blocks_internal (pbar, blocks);
}

void
__gtk_progress_bar_set_activity_step (GtkProgressBar *pbar,
                                    guint           step)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));

  __gtk_progress_bar_set_activity_step_internal (pbar, step);
}

void
__gtk_progress_bar_set_activity_blocks (GtkProgressBar *pbar,
				      guint           blocks)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  g_return_if_fail (blocks > 1);

  __gtk_progress_bar_set_activity_blocks_internal (pbar, blocks);
}

/**
 * __gtk_progress_bar_set_ellipsize:
 * @pbar: a #GtkProgressBar
 * @mode: a #PangoEllipsizeMode
 *
 * Sets the mode used to ellipsize (add an ellipsis: "...") the text 
 * if there is not enough space to render the entire string.
 *
 * Since: 2.6
 **/
void
__gtk_progress_bar_set_ellipsize (GtkProgressBar     *pbar,
				PangoEllipsizeMode  mode)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  g_return_if_fail (mode >= PANGO_ELLIPSIZE_NONE && 
		    mode <= PANGO_ELLIPSIZE_END);
  
  if ((PangoEllipsizeMode)gtk_progress_bar_get_props (pbar)->ellipsize != mode)
    {
      gtk_progress_bar_get_props (pbar)->ellipsize = mode;

      g_object_notify (G_OBJECT (pbar), "ellipsize");
      __gtk_widget_queue_resize (GTK_WIDGET (pbar));
    }
}

/**
 * __gtk_progress_bar_get_ellipsize:
 * @pbar: a #GtkProgressBar
 *
 * Returns the ellipsizing position of the progressbar. 
 * See __gtk_progress_bar_set_ellipsize().
 *
 * Return value: #PangoEllipsizeMode
 *
 * Since: 2.6
 **/
PangoEllipsizeMode 
__gtk_progress_bar_get_ellipsize (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), PANGO_ELLIPSIZE_NONE);

  return gtk_progress_bar_get_props (pbar)->ellipsize;
}

