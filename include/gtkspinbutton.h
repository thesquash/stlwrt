/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GtkSpinButton widget for GTK+
 * Copyright (C) 1998 Lars Hamann and Stefan Jeske
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

#ifndef __GTK_SPIN_BUTTON_H__
#define __GTK_SPIN_BUTTON_H__

#include <gtkentry.h>
#include <gtkadjustment.h>


G_BEGIN_DECLS

#define GTK_TYPE_SPIN_BUTTON                  (SF(gtk_spin_button_get_type) ())
#define GTK_SPIN_BUTTON(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SPIN_BUTTON, GtkSpinButton))
#define GTK_SPIN_BUTTON_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SPIN_BUTTON, GtkSpinButtonClass))
#define GTK_IS_SPIN_BUTTON(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SPIN_BUTTON))
#define GTK_IS_SPIN_BUTTON_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SPIN_BUTTON))
#define GTK_SPIN_BUTTON_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SPIN_BUTTON, GtkSpinButtonClass))

#define GTK_INPUT_ERROR -1

typedef enum
{
  GTK_UPDATE_ALWAYS,
  GTK_UPDATE_IF_VALID
} GtkSpinButtonUpdatePolicy;

typedef enum
{
  GTK_SPIN_STEP_FORWARD,
  GTK_SPIN_STEP_BACKWARD,
  GTK_SPIN_PAGE_FORWARD,
  GTK_SPIN_PAGE_BACKWARD,
  GTK_SPIN_HOME,
  GTK_SPIN_END,
  GTK_SPIN_USER_DEFINED
} GtkSpinType;


typedef struct _GtkSpinButton	    GtkSpinButtonFat;
typedef struct _GtkSpinButton	    GtkSpinButtonThin;

typedef struct _GtkSpinButtonClass  GtkSpinButtonClass;


/********************************************************************/
struct _GtkSpinButtonProps
{


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
};

struct _GtkSpinButtonFat
{
  GtkEntryFat   entry;

  struct _GtkSpinButtonProps instance_properties;
};

struct _GtkSpinButtonThin
{
  GtkEntryThin  entry;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkSpinButtonFat   fat_instance;
  struct _GtkSpinButtonThin  thin_instance;
}   GtkSpinButton;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkSpinButtonFat GtkSpinButton;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkSpinButtonThin GtkSpinButton;
#endif
/********************************************************************/



struct _GtkSpinButtonClass
{
  GtkEntryClass parent_class;

  gint (*input)  (GtkSpinButton *spin_button,
		  gdouble       *new_value);
  gint (*output) (GtkSpinButton *spin_button);
  void (*value_changed) (GtkSpinButton *spin_button);

  /* Action signals for keybindings, do not connect to these */
  void (*change_value) (GtkSpinButton *spin_button,
			GtkScrollType  scroll);

  void (*wrapped) (GtkSpinButton *spin_button);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
};


GType		SF(_T2_gtk_spin_button_get_type)	   (void) G_GNUC_CONST;
GType		SF(_3T_gtk_spin_button_get_type)	   (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType		SF(gtk_spin_button_get_type)	   (void) G_GNUC_CONST;

void		SF(gtk_spin_button_configure)	   (GtkSpinButton  *spin_button,
						    GtkAdjustment  *adjustment,
						    gdouble	    climb_rate,
						    guint	    digits);

GtkWidget*	SF(gtk_spin_button_new)		   (GtkAdjustment  *adjustment,
						    gdouble	    climb_rate,
						    guint	    digits);

GtkWidget*	SF(gtk_spin_button_new_with_range)	   (gdouble  min,
						    gdouble  max,
						    gdouble  step);

void		SF(gtk_spin_button_set_adjustment)	   (GtkSpinButton  *spin_button,
						    GtkAdjustment  *adjustment);

GtkAdjustment*	SF(gtk_spin_button_get_adjustment)	   (GtkSpinButton  *spin_button);

void		SF(gtk_spin_button_set_digits)	   (GtkSpinButton  *spin_button,
						    guint	    digits);
guint           SF(gtk_spin_button_get_digits)         (GtkSpinButton  *spin_button);

void		SF(gtk_spin_button_set_increments)	   (GtkSpinButton  *spin_button,
						    gdouble         step,
						    gdouble         page);
void            SF(gtk_spin_button_get_increments)     (GtkSpinButton  *spin_button,
						    gdouble        *step,
						    gdouble        *page);

void		SF(gtk_spin_button_set_range)	   (GtkSpinButton  *spin_button,
						    gdouble         min,
						    gdouble         max);
void            SF(gtk_spin_button_get_range)          (GtkSpinButton  *spin_button,
						    gdouble        *min,
						    gdouble        *max);

gdouble		SF(gtk_spin_button_get_value)          (GtkSpinButton  *spin_button);

gint		SF(gtk_spin_button_get_value_as_int)   (GtkSpinButton  *spin_button);

void		SF(gtk_spin_button_set_value)	   (GtkSpinButton  *spin_button,
						    gdouble	    value);

void		SF(gtk_spin_button_set_update_policy)  (GtkSpinButton  *spin_button,
						    GtkSpinButtonUpdatePolicy  policy);
GtkSpinButtonUpdatePolicy SF(gtk_spin_button_get_update_policy) (GtkSpinButton *spin_button);

void		SF(gtk_spin_button_set_numeric)	   (GtkSpinButton  *spin_button,
						    gboolean	    numeric);
gboolean        SF(gtk_spin_button_get_numeric)        (GtkSpinButton  *spin_button);

void		SF(gtk_spin_button_spin)		   (GtkSpinButton  *spin_button,
						    GtkSpinType     direction,
						    gdouble	    increment);

void		SF(gtk_spin_button_set_wrap)	   (GtkSpinButton  *spin_button,
						    gboolean	    wrap);
gboolean        SF(gtk_spin_button_get_wrap)           (GtkSpinButton  *spin_button);

void		SF(gtk_spin_button_set_snap_to_ticks)  (GtkSpinButton  *spin_button,
						    gboolean	    snap_to_ticks);
gboolean        SF(gtk_spin_button_get_snap_to_ticks)  (GtkSpinButton  *spin_button);
void            SF(gtk_spin_button_update)             (GtkSpinButton  *spin_button);


#ifndef GTK_DISABLE_DEPRECATED
#define gtk_spin_button_get_value_as_float gtk_spin_button_get_value
#endif

G_END_DECLS

#endif /* __GTK_SPIN_BUTTON_H__ */
