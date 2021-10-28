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


#ifndef __GDK_EVENTS_H__
#define __GDK_EVENTS_H__

#include <stlwrt.h>


#include <gdkcolor.h>

#include <gdktypes.h>

#include <gdkdnd.h>

#include <gdkinput.h>

G_BEGIN_DECLS

#define GDK_TYPE_EVENT          (gdk_event_get_type ())

#define GDK_PRIORITY_EVENTS	(G_PRIORITY_DEFAULT)
#define GDK_PRIORITY_REDRAW     (G_PRIORITY_HIGH_IDLE + 20)


typedef struct _GdkEventAny	    GdkEventAny;
typedef struct _GdkEventExpose	    GdkEventExpose;
typedef struct _GdkEventNoExpose    GdkEventNoExpose;
typedef struct _GdkEventVisibility  GdkEventVisibility;
typedef struct _GdkEventMotion	    GdkEventMotion;
typedef struct _GdkEventButton	    GdkEventButton;
typedef struct _GdkEventScroll      GdkEventScroll;  
typedef struct _GdkEventKey	    GdkEventKey;
typedef struct _GdkEventFocus	    GdkEventFocus;
typedef struct _GdkEventCrossing    GdkEventCrossing;
typedef struct _GdkEventConfigure   GdkEventConfigure;
typedef struct _GdkEventProperty    GdkEventProperty;
typedef struct _GdkEventSelection   GdkEventSelection;
typedef struct _GdkEventOwnerChange GdkEventOwnerChange;
typedef struct _GdkEventProximity   GdkEventProximity;
typedef struct _GdkEventClient	    GdkEventClient;
typedef struct _GdkEventDND         GdkEventDND;
typedef struct _GdkEventWindowState GdkEventWindowState;
typedef struct _GdkEventSetting     GdkEventSetting;
typedef struct _GdkEventGrabBroken  GdkEventGrabBroken;

typedef void (*GdkEventFunc) (GdkEvent *event,
			      gpointer	data);

/* Event filtering */

typedef void GdkXEvent;	  /* Can be cast to window system specific
			   * even type, XEvent on X11, MSG on Win32.
			   */

typedef GdkFilterReturn (*GdkFilterFunc) (GdkXEvent *xevent,
					  GdkEvent *event,
					  gpointer  data);


struct _GdkEventAny
{
  GdkEventType   type;
  GdkWindow *window;
  gint8 send_event;
};

struct _GdkEventExpose
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkRectangle area;
  GdkRegion *region;
  gint count; /* If non-zero, how many more events follow. */
};

struct _GdkEventNoExpose
{
  GdkEventType   type;
  GdkWindow *window;
  gint8 send_event;
};

struct _GdkEventVisibility
{
  GdkEventType   type;
  GdkWindow *window;
  gint8 send_event;
  GdkVisibilityState state;
};

struct _GdkEventMotion
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  gdouble x;
  gdouble y;
  gdouble *axes;
  guint state;
  gint16 is_hint;
  GdkDevice *device;
  gdouble x_root, y_root;
};

struct _GdkEventButton
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  gdouble x;
  gdouble y;
  gdouble *axes;
  guint state;
  guint button;
  GdkDevice *device;
  gdouble x_root, y_root;
};

struct _GdkEventScroll
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  gdouble x;
  gdouble y;
  guint state;
  GdkScrollDirection direction;
  GdkDevice *device;
  gdouble x_root, y_root;
};

struct _GdkEventKey
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  guint state;
  guint keyval;
  gint length;
  gchar *string;
  guint16 hardware_keycode;
  guint8 group;
  guint is_modifier : 1;
};

struct _GdkEventCrossing
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkWindow *subwindow;
  guint32 time;
  gdouble x;
  gdouble y;
  gdouble x_root;
  gdouble y_root;
  GdkCrossingMode mode;
  GdkNotifyType detail;
  gboolean focus;
  guint state;
};

struct _GdkEventFocus
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint16 in;
};

struct _GdkEventConfigure
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint x, y;
  gint width;
  gint height;
};

struct _GdkEventProperty
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkAtom atom;
  guint32 time;
  guint state;
};

struct _GdkEventSelection
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkAtom selection;
  GdkAtom target;
  GdkAtom property;
  guint32 time;
  GdkNativeWindow requestor;
};

struct _GdkEventOwnerChange
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkNativeWindow owner;
  GdkOwnerChange reason;
  GdkAtom selection;
  guint32 time;
  guint32 selection_time;
};

/* This event type will be used pretty rarely. It only is important
   for XInput aware programs that are drawing their own cursor */

struct _GdkEventProximity
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  GdkDevice *device;
};

struct _GdkEventClient
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkAtom message_type;
  gushort data_format;
  union {
    char b[20];
    short s[10];
    long l[5];
  } data;
};

struct _GdkEventSetting
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkSettingAction action;
  char *name;
};

struct _GdkEventWindowState
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkWindowState changed_mask;
  GdkWindowState new_window_state;
};

struct _GdkEventGrabBroken {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gboolean keyboard;
  gboolean implicit;
  GdkWindow *grab_window;
};

/* Event types for DND */

struct _GdkEventDND {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkDragContext *context;

  guint32 time;
  gshort x_root, y_root;
};

STLWRT_DECLARE_UNION(GdkEvent,
  GdkEventType		    type;
  GdkEventAny		    any;
  GdkEventExpose	    expose;
  GdkEventNoExpose	    no_expose;
  GdkEventVisibility	    visibility;
  GdkEventMotion	    motion;
  GdkEventButton	    button;
  GdkEventScroll            scroll;
  GdkEventKey		    key;
  GdkEventCrossing	    crossing;
  GdkEventFocus		    focus_change;
  GdkEventConfigure	    configure;
  GdkEventProperty	    property;
  GdkEventSelection	    selection;
  GdkEventOwnerChange  	    owner_change;
  GdkEventProximity	    proximity;
  GdkEventClient	    client;
  GdkEventDND               dnd;
  GdkEventWindowState       window_state;
  GdkEventSetting           setting;
  GdkEventGrabBroken        grab_broken;
)

STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(gdk_event)

gboolean  SF(gdk_events_pending)	 	(void);
GdkEvent* SF(gdk_event_get)			(void);

GdkEvent* SF(gdk_event_peek)                (void);
#ifndef GDK_DISABLE_DEPRECATED
GdkEvent* SF(gdk_event_get_graphics_expose) (GdkWindow 	*window);
#endif
void      SF(gdk_event_put)	 		(const GdkEvent *event);

GdkEvent* SF(gdk_event_new)                 (GdkEventType    type);
GdkEvent* SF(gdk_event_copy)     		(const GdkEvent *event);
void	  SF(gdk_event_free)     		(GdkEvent 	*event);

guint32   SF(gdk_event_get_time)            (const GdkEvent  *event);
gboolean  SF(gdk_event_get_state)           (const GdkEvent  *event,
                                         GdkModifierType *state);
gboolean  SF(gdk_event_get_coords)		(const GdkEvent  *event,
					 gdouble	 *x_win,
					 gdouble	 *y_win);
gboolean  SF(gdk_event_get_root_coords)	(const GdkEvent  *event,
					 gdouble	 *x_root,
					 gdouble	 *y_root);
gboolean  SF(gdk_event_get_axis)            (const GdkEvent  *event,
                                         GdkAxisUse       axis_use,
                                         gdouble         *value);
void      SF(gdk_event_request_motions)     (const GdkEventMotion *event);
void	  SF(gdk_event_handler_set) 	(GdkEventFunc    func,
					 gpointer        data,
					 GDestroyNotify  notify);

void       SF(gdk_event_set_screen)         (GdkEvent        *event,
                                         GdkScreen       *screen);
GdkScreen *SF(gdk_event_get_screen)         (const GdkEvent  *event);

void	  SF(gdk_set_show_events)		(gboolean	 show_events);
gboolean  SF(gdk_get_show_events)		(void);

#ifndef GDK_MULTIHEAD_SAFE
void SF(gdk_add_client_message_filter) (GdkAtom       message_type,
				    GdkFilterFunc func,
				    gpointer      data);

gboolean SF(gdk_setting_get) (const gchar *name,
			  GValue      *value); 
#endif /* GDK_MULTIHEAD_SAFE */

G_END_DECLS

#endif /* __GDK_EVENTS_H__ */
