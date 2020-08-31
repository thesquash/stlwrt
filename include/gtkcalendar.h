/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * GTK Calendar Widget
 * Copyright (C) 1998 Cesar Miquel and Shawn T. Amundson
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __GTK_CALENDAR_H__
#define __GTK_CALENDAR_H__


#include <gtkwidget.h>


G_BEGIN_DECLS

#define GTK_TYPE_CALENDAR                  (SF(gtk_calendar_get_type) ())
#define GTK_CALENDAR(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CALENDAR, GtkCalendar))
#define GTK_CALENDAR_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CALENDAR, GtkCalendarClass))
#define GTK_IS_CALENDAR(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CALENDAR))
#define GTK_IS_CALENDAR_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CALENDAR))
#define GTK_CALENDAR_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CALENDAR, GtkCalendarClass))


typedef struct _GtkCalendar	       GtkCalendarFat;
typedef struct _GtkCalendar	       GtkCalendarThin;

typedef struct _GtkCalendarClass       GtkCalendarClass;

typedef struct _GtkCalendarPrivate     GtkCalendarPrivate;

/**
 * GtkCalendarDisplayOptions:
 * @GTK_CALENDAR_SHOW_HEADING: Specifies that the month and year should be displayed.
 * @GTK_CALENDAR_SHOW_DAY_NAMES: Specifies that three letter day descriptions should be present.
 * @GTK_CALENDAR_NO_MONTH_CHANGE: Prevents the user from switching months with the calendar.
 * @GTK_CALENDAR_SHOW_WEEK_NUMBERS: Displays each week numbers of the current year, down the
 * left side of the calendar.
 * @GTK_CALENDAR_WEEK_START_MONDAY: Since GTK+ 2.4, this option is deprecated and ignored by GTK+.
 * The information on which day the calendar week starts is derived from the locale.
 * @GTK_CALENDAR_SHOW_DETAILS: Just show an indicator, not the full details
 * text when details are provided. See SF(gtk_calendar_set_detail_func)().
 *
 * These options can be used to influence the display and behaviour of a #GtkCalendar.
 */
typedef enum
{
  GTK_CALENDAR_SHOW_HEADING		= 1 << 0,
  GTK_CALENDAR_SHOW_DAY_NAMES		= 1 << 1,
  GTK_CALENDAR_NO_MONTH_CHANGE		= 1 << 2,
  GTK_CALENDAR_SHOW_WEEK_NUMBERS	= 1 << 3,
  GTK_CALENDAR_WEEK_START_MONDAY	= 1 << 4,
  GTK_CALENDAR_SHOW_DETAILS		= 1 << 5
} GtkCalendarDisplayOptions;

/**
 * GtkCalendarDetailFunc:
 * @calendar: a #GtkCalendar.
 * @year: the year for which details are needed.
 * @month: the month for which details are needed.
 * @day: the day of @month for which details are needed.
 * @user_data: the data passed with SF(gtk_calendar_set_detail_func)().
 *
 * This kind of functions provide Pango markup with detail information for the
 * specified day. Examples for such details are holidays or appointments. The
 * function returns %NULL when no information is available.
 *
 * Since: 2.14
 *
 * Return value: Newly allocated string with Pango markup with details
 * for the specified day, or %NULL.
 */
typedef gchar* (*GtkCalendarDetailFunc) (GtkCalendar *calendar,
                                         guint        year,
                                         guint        month,
                                         guint        day,
                                         gpointer     user_data);


struct _GtkCalendarPrivate
{
  GdkWindow *header_win;
  GdkWindow *day_name_win;
  GdkWindow *main_win;
  GdkWindow *week_win;
  GdkWindow *arrow_win[4];

  guint header_h;
  guint day_name_h;
  guint main_h;

  guint	     arrow_state[4];
  guint	     arrow_width;
  guint	     max_month_width;
  guint	     max_year_width;
  
  guint day_width;
  guint week_width;

  guint min_day_width;
  guint max_day_char_width;
  guint max_day_char_ascent;
  guint max_day_char_descent;
  guint max_label_char_ascent;
  guint max_label_char_descent;
  guint max_week_char_width;
  
  /* flags */
  guint year_before : 1;

  guint need_timer  : 1;

  guint in_drag : 1;
  guint drag_highlight : 1;

  guint32 timer;
  gint click_child;

  gint week_start;

  gint drag_start_x;
  gint drag_start_y;

  /* Optional callback, used to display extra information for each day. */
  GtkCalendarDetailFunc detail_func;
  gpointer              detail_func_user_data;
  GDestroyNotify        detail_func_destroy;

  /* Size requistion for details provided by the hook. */
  gint detail_height_rows;
  gint detail_width_chars;
  gint detail_overflow[6];
};

struct _GtkCalendar
{
  GtkWidget widget;
  
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
  
  GtkCalendarPrivate * (priv);
  gchar  (grow_space [32]);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _GtkCalendarClass
{
  GtkWidgetClass parent_class;
  
  /* Signal handlers */
  void (* month_changed)		(GtkCalendar *calendar);
  void (* day_selected)			(GtkCalendar *calendar);
  void (* day_selected_double_click)	(GtkCalendar *calendar);
  void (* prev_month)			(GtkCalendar *calendar);
  void (* next_month)			(GtkCalendar *calendar);
  void (* prev_year)			(GtkCalendar *calendar);
  void (* next_year)			(GtkCalendar *calendar);
  
};


GType	   SF(_T2_gtk_calendar_get_type)	(void) G_GNUC_CONST;
GType	   SF(_3T_gtk_calendar_get_type)	(void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType	   SF(gtk_calendar_get_type)	(void) G_GNUC_CONST;
GtkWidget* SF(gtk_calendar_new)		(void);

gboolean   SF(gtk_calendar_select_month)	(GtkCalendar *calendar,
					 guint	      month,
					 guint	      year);
void	   SF(gtk_calendar_select_day)	(GtkCalendar *calendar,
					 guint	      day);

gboolean   SF(gtk_calendar_mark_day)	(GtkCalendar *calendar,
					 guint	      day);
gboolean   SF(gtk_calendar_unmark_day)	(GtkCalendar *calendar,
					 guint	      day);
void	   SF(gtk_calendar_clear_marks)	(GtkCalendar *calendar);


void	   SF(gtk_calendar_set_display_options) (GtkCalendar    	      *calendar,
					     GtkCalendarDisplayOptions flags);
GtkCalendarDisplayOptions
           SF(gtk_calendar_get_display_options) (GtkCalendar   	      *calendar);
#ifndef GTK_DISABLE_DEPRECATED
void	   SF(gtk_calendar_display_options) (GtkCalendar		  *calendar,
					 GtkCalendarDisplayOptions flags);
#endif

void	   SF(gtk_calendar_get_date)	(GtkCalendar *calendar, 
					 guint	     *year,
					 guint	     *month,
					 guint	     *day);

void       SF(gtk_calendar_set_detail_func) (GtkCalendar           *calendar,
                                         GtkCalendarDetailFunc  func,
                                         gpointer               data,
                                         GDestroyNotify         destroy);

void       SF(gtk_calendar_set_detail_width_chars) (GtkCalendar    *calendar,
                                                gint            chars);
void       SF(gtk_calendar_set_detail_height_rows) (GtkCalendar    *calendar,
                                                gint            rows);

gint       SF(gtk_calendar_get_detail_width_chars) (GtkCalendar    *calendar);
gint       SF(gtk_calendar_get_detail_height_rows) (GtkCalendar    *calendar);

#ifndef GTK_DISABLE_DEPRECATED
void	   SF(gtk_calendar_freeze)		(GtkCalendar *calendar);
void	   SF(gtk_calendar_thaw)		(GtkCalendar *calendar);
#endif

G_END_DECLS

#endif /* __GTK_CALENDAR_H__ */
