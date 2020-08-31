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

#include <gdkcolor.h>
#include <gdktypes.h>
#include <gdkdnd.h>
#include <gdkinput.h>

G_BEGIN_DECLS

#define GDK_TYPE_EVENT          (SF(gdk_event_get_type) ())

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

typedef union  _GdkEvent	    GdkEvent;

typedef void (*GdkEventFunc) (GdkEvent *event,
			      gpointer	data);

/* Event filtering */

typedef void GdkXEvent;	  /* Can be cast to window system specific
			   * even type, XEvent on X11, MSG on Win32.
			   */

typedef enum {
  GDK_FILTER_CONTINUE,	  /* Event not handled, continue processesing */
  GDK_FILTER_TRANSLATE,	  /* Native event translated into a GDK event and
                             stored in the "event" structure that was
                             passed in */
  GDK_FILTER_REMOVE	  /* Terminate processing, removing event */
} GdkFilterReturn;

typedef GdkFilterReturn (*GdkFilterFunc) (GdkXEvent *xevent,
					  GdkEvent *event,
					  gpointer  data);


/* Event types.
 *   Nothing: No event occurred.
 *   Delete: A window delete event was sent by the window manager.
 *	     The specified window should be deleted.
 *   Destroy: A window has been destroyed.
 *   Expose: Part of a window has been uncovered.
 *   NoExpose: Same as expose, but no expose event was generated.
 *   VisibilityNotify: A window has become fully/partially/not obscured.
 *   MotionNotify: The mouse has moved.
 *   ButtonPress: A mouse button was pressed.
 *   ButtonRelease: A mouse button was release.
 *   KeyPress: A key was pressed.
 *   KeyRelease: A key was released.
 *   EnterNotify: A window was entered.
 *   LeaveNotify: A window was exited.
 *   FocusChange: The focus window has changed. (The focus window gets
 *		  keyboard events).
 *   Resize: A window has been resized.
 *   Map: A window has been mapped. (It is now visible on the screen).
 *   Unmap: A window has been unmapped. (It is no longer visible on
 *	    the screen).
 *   Scroll: A mouse wheel was scrolled either up or down.
 */
typedef enum
{
  GDK_NOTHING		= -1,
  GDK_DELETE		= 0,
  GDK_DESTROY		= 1,
  GDK_EXPOSE		= 2,
  GDK_MOTION_NOTIFY	= 3,
  GDK_BUTTON_PRESS	= 4,
  GDK_2BUTTON_PRESS	= 5,
  GDK_3BUTTON_PRESS	= 6,
  GDK_BUTTON_RELEASE	= 7,
  GDK_KEY_PRESS		= 8,
  GDK_KEY_RELEASE	= 9,
  GDK_ENTER_NOTIFY	= 10,
  GDK_LEAVE_NOTIFY	= 11,
  GDK_FOCUS_CHANGE	= 12,
  GDK_CONFIGURE		= 13,
  GDK_MAP		= 14,
  GDK_UNMAP		= 15,
  GDK_PROPERTY_NOTIFY	= 16,
  GDK_SELECTION_CLEAR	= 17,
  GDK_SELECTION_REQUEST = 18,
  GDK_SELECTION_NOTIFY	= 19,
  GDK_PROXIMITY_IN	= 20,
  GDK_PROXIMITY_OUT	= 21,
  GDK_DRAG_ENTER        = 22,
  GDK_DRAG_LEAVE        = 23,
  GDK_DRAG_MOTION       = 24,
  GDK_DRAG_STATUS       = 25,
  GDK_DROP_START        = 26,
  GDK_DROP_FINISHED     = 27,
  GDK_CLIENT_EVENT	= 28,
  GDK_VISIBILITY_NOTIFY = 29,
  GDK_NO_EXPOSE		= 30,
  GDK_SCROLL            = 31,
  GDK_WINDOW_STATE      = 32,
  GDK_SETTING           = 33,
  GDK_OWNER_CHANGE      = 34,
  GDK_GRAB_BROKEN       = 35,
  GDK_DAMAGE            = 36,
  GDK_EVENT_LAST        /* helper variable for decls */
} GdkEventType;

/* Event masks. (Used to select what types of events a window
 *  will receive).
 */
typedef enum
{
  GDK_EXPOSURE_MASK		= 1 << 1,
  GDK_POINTER_MOTION_MASK	= 1 << 2,
  GDK_POINTER_MOTION_HINT_MASK	= 1 << 3,
  GDK_BUTTON_MOTION_MASK	= 1 << 4,
  GDK_BUTTON1_MOTION_MASK	= 1 << 5,
  GDK_BUTTON2_MOTION_MASK	= 1 << 6,
  GDK_BUTTON3_MOTION_MASK	= 1 << 7,
  GDK_BUTTON_PRESS_MASK		= 1 << 8,
  GDK_BUTTON_RELEASE_MASK	= 1 << 9,
  GDK_KEY_PRESS_MASK		= 1 << 10,
  GDK_KEY_RELEASE_MASK		= 1 << 11,
  GDK_ENTER_NOTIFY_MASK		= 1 << 12,
  GDK_LEAVE_NOTIFY_MASK		= 1 << 13,
  GDK_FOCUS_CHANGE_MASK		= 1 << 14,
  GDK_STRUCTURE_MASK		= 1 << 15,
  GDK_PROPERTY_CHANGE_MASK	= 1 << 16,
  GDK_VISIBILITY_NOTIFY_MASK	= 1 << 17,
  GDK_PROXIMITY_IN_MASK		= 1 << 18,
  GDK_PROXIMITY_OUT_MASK	= 1 << 19,
  GDK_SUBSTRUCTURE_MASK		= 1 << 20,
  GDK_SCROLL_MASK               = 1 << 21,
  GDK_ALL_EVENTS_MASK		= 0x3FFFFE
} GdkEventMask;

typedef enum
{
  GDK_VISIBILITY_UNOBSCURED,
  GDK_VISIBILITY_PARTIAL,
  GDK_VISIBILITY_FULLY_OBSCURED
} GdkVisibilityState;

typedef enum
{
  GDK_SCROLL_UP,
  GDK_SCROLL_DOWN,
  GDK_SCROLL_LEFT,
  GDK_SCROLL_RIGHT
} GdkScrollDirection;

/* Types of enter/leave notifications.
 *   Ancestor:
 *   Virtual:
 *   Inferior:
 *   Nonlinear:
 *   NonlinearVirtual:
 *   Unknown: An unknown type of enter/leave event occurred.
 */
typedef enum
{
  GDK_NOTIFY_ANCESTOR		= 0,
  GDK_NOTIFY_VIRTUAL		= 1,
  GDK_NOTIFY_INFERIOR		= 2,
  GDK_NOTIFY_NONLINEAR		= 3,
  GDK_NOTIFY_NONLINEAR_VIRTUAL	= 4,
  GDK_NOTIFY_UNKNOWN		= 5
} GdkNotifyType;

/* Enter/leave event modes.
 *   NotifyNormal
 *   NotifyGrab
 *   NotifyUngrab
 */
typedef enum
{
  GDK_CROSSING_NORMAL,
  GDK_CROSSING_GRAB,
  GDK_CROSSING_UNGRAB,
  GDK_CROSSING_GTK_GRAB,
  GDK_CROSSING_GTK_UNGRAB,
  GDK_CROSSING_STATE_CHANGED
} GdkCrossingMode;

typedef enum
{
  GDK_PROPERTY_NEW_VALUE,
  GDK_PROPERTY_DELETE
} GdkPropertyState;

typedef enum
{
  GDK_WINDOW_STATE_WITHDRAWN  = 1 << 0,
  GDK_WINDOW_STATE_ICONIFIED  = 1 << 1,
  GDK_WINDOW_STATE_MAXIMIZED  = 1 << 2,
  GDK_WINDOW_STATE_STICKY     = 1 << 3,
  GDK_WINDOW_STATE_FULLSCREEN = 1 << 4,
  GDK_WINDOW_STATE_ABOVE      = 1 << 5,
  GDK_WINDOW_STATE_BELOW      = 1 << 6
} GdkWindowState;

typedef enum
{
  GDK_SETTING_ACTION_NEW,
  GDK_SETTING_ACTION_CHANGED,
  GDK_SETTING_ACTION_DELETED
} GdkSettingAction;

typedef enum
{
  GDK_OWNER_CHANGE_NEW_OWNER,
  GDK_OWNER_CHANGE_DESTROY,
  GDK_OWNER_CHANGE_CLOSE
} GdkOwnerChange;

/********************************************************************/
struct _GdkEventAnyProps
{

  GdkWindow *window;
  gint8 send_event;
};

struct _GdkEventAnyFat
{
  GdkEventTypeFat   type;

  struct _GdkEventAnyProps instance_properties;
};

struct _GdkEventAnyThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventAnyFat   fat_instance;
  struct _GdkEventAnyThin  thin_instance;
}   GdkEventAny;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventAnyFat GdkEventAny;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventAnyThin GdkEventAny;
#endif
/********************************************************************/



struct _GdkEventExpose
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkRectangle area;
  GdkRegion *region;
  gint count; /* If non-zero, how many more events follow. */
};

/********************************************************************/
struct _GdkEventNoExposeProps
{

  GdkWindow *window;
  gint8 send_event;
};

struct _GdkEventNoExposeFat
{
  GdkEventTypeFat   type;

  struct _GdkEventNoExposeProps instance_properties;
};

struct _GdkEventNoExposeThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventNoExposeFat   fat_instance;
  struct _GdkEventNoExposeThin  thin_instance;
}   GdkEventNoExpose;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventNoExposeFat GdkEventNoExpose;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventNoExposeThin GdkEventNoExpose;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventVisibilityProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkVisibilityState state;
};

struct _GdkEventVisibilityFat
{
  GdkEventTypeFat   type;

  struct _GdkEventVisibilityProps instance_properties;
};

struct _GdkEventVisibilityThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventVisibilityFat   fat_instance;
  struct _GdkEventVisibilityThin  thin_instance;
}   GdkEventVisibility;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventVisibilityFat GdkEventVisibility;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventVisibilityThin GdkEventVisibility;
#endif
/********************************************************************/



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

/********************************************************************/
struct _GdkEventKeyProps
{

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

struct _GdkEventKeyFat
{
  GdkEventTypeFat   type;

  struct _GdkEventKeyProps instance_properties;
};

struct _GdkEventKeyThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventKeyFat   fat_instance;
  struct _GdkEventKeyThin  thin_instance;
}   GdkEventKey;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventKeyFat GdkEventKey;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventKeyThin GdkEventKey;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventCrossingProps
{

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

struct _GdkEventCrossingFat
{
  GdkEventTypeFat   type;

  struct _GdkEventCrossingProps instance_properties;
};

struct _GdkEventCrossingThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventCrossingFat   fat_instance;
  struct _GdkEventCrossingThin  thin_instance;
}   GdkEventCrossing;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventCrossingFat GdkEventCrossing;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventCrossingThin GdkEventCrossing;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventFocusProps
{

  GdkWindow *window;
  gint8 send_event;
  gint16 in;
};

struct _GdkEventFocusFat
{
  GdkEventTypeFat   type;

  struct _GdkEventFocusProps instance_properties;
};

struct _GdkEventFocusThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventFocusFat   fat_instance;
  struct _GdkEventFocusThin  thin_instance;
}   GdkEventFocus;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventFocusFat GdkEventFocus;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventFocusThin GdkEventFocus;
#endif
/********************************************************************/



struct _GdkEventConfigure
{
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint x, y;
  gint width;
  gint height;
};

/********************************************************************/
struct _GdkEventPropertyProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkAtom atom;
  guint32 time;
  guint state;
};

struct _GdkEventPropertyFat
{
  GdkEventTypeFat   type;

  struct _GdkEventPropertyProps instance_properties;
};

struct _GdkEventPropertyThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventPropertyFat   fat_instance;
  struct _GdkEventPropertyThin  thin_instance;
}   GdkEventProperty;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventPropertyFat GdkEventProperty;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventPropertyThin GdkEventProperty;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventSelectionProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkAtom selection;
  GdkAtom target;
  GdkAtom property;
  guint32 time;
  GdkNativeWindow requestor;
};

struct _GdkEventSelectionFat
{
  GdkEventTypeFat   type;

  struct _GdkEventSelectionProps instance_properties;
};

struct _GdkEventSelectionThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventSelectionFat   fat_instance;
  struct _GdkEventSelectionThin  thin_instance;
}   GdkEventSelection;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventSelectionFat GdkEventSelection;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventSelectionThin GdkEventSelection;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventOwnerChangeProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkNativeWindow owner;
  GdkOwnerChange reason;
  GdkAtom selection;
  guint32 time;
  guint32 selection_time;
};

struct _GdkEventOwnerChangeFat
{
  GdkEventTypeFat   type;

  struct _GdkEventOwnerChangeProps instance_properties;
};

struct _GdkEventOwnerChangeThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventOwnerChangeFat   fat_instance;
  struct _GdkEventOwnerChangeThin  thin_instance;
}   GdkEventOwnerChange;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventOwnerChangeFat GdkEventOwnerChange;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventOwnerChangeThin GdkEventOwnerChange;
#endif
/********************************************************************/



/* This event type will be used pretty rarely. It only is important
   for XInput aware programs that are drawing their own cursor */

/********************************************************************/
struct _GdkEventProximityProps
{

  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  GdkDevice *device;
};

struct _GdkEventProximityFat
{
  GdkEventTypeFat   type;

  struct _GdkEventProximityProps instance_properties;
};

struct _GdkEventProximityThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventProximityFat   fat_instance;
  struct _GdkEventProximityThin  thin_instance;
}   GdkEventProximity;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventProximityFat GdkEventProximity;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventProximityThin GdkEventProximity;
#endif
/********************************************************************/



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

/********************************************************************/
struct _GdkEventSettingProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkSettingAction action;
  char *name;
};

struct _GdkEventSettingFat
{
  GdkEventTypeFat   type;

  struct _GdkEventSettingProps instance_properties;
};

struct _GdkEventSettingThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventSettingFat   fat_instance;
  struct _GdkEventSettingThin  thin_instance;
}   GdkEventSetting;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventSettingFat GdkEventSetting;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventSettingThin GdkEventSetting;
#endif
/********************************************************************/



/********************************************************************/
struct _GdkEventWindowStateProps
{

  GdkWindow *window;
  gint8 send_event;
  GdkWindowState changed_mask;
  GdkWindowState new_window_state;
};

struct _GdkEventWindowStateFat
{
  GdkEventTypeFat   type;

  struct _GdkEventWindowStateProps instance_properties;
};

struct _GdkEventWindowStateThin
{
  GdkEventTypeThin  type;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GdkEventWindowStateFat   fat_instance;
  struct _GdkEventWindowStateThin  thin_instance;
}   GdkEventWindowState;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GdkEventWindowStateFat GdkEventWindowState;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GdkEventWindowStateThin GdkEventWindowState;
#endif
/********************************************************************/



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

union _GdkEvent
{
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
};

GType     SF(_T2_gdk_event_get_type)            (void) G_GNUC_CONST;
GType     SF(_3T_gdk_event_get_type)            (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType     SF(gdk_event_get_type)            (void) G_GNUC_CONST;

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
