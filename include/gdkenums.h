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

#ifndef __GDK_ENUMS_H__
#define __GDK_ENUMS_H__

#include <stlwrt.h>


#include <glib-object.h>

G_BEGIN_DECLS

/* Cursor types.
 */
typedef enum 
{
  GDK_X_CURSOR 		  = 0,
  GDK_ARROW 		  = 2,
  GDK_BASED_ARROW_DOWN    = 4,
  GDK_BASED_ARROW_UP 	  = 6,
  GDK_BOAT 		  = 8,
  GDK_BOGOSITY 		  = 10,
  GDK_BOTTOM_LEFT_CORNER  = 12,
  GDK_BOTTOM_RIGHT_CORNER = 14,
  GDK_BOTTOM_SIDE 	  = 16,
  GDK_BOTTOM_TEE 	  = 18,
  GDK_BOX_SPIRAL 	  = 20,
  GDK_CENTER_PTR 	  = 22,
  GDK_CIRCLE 		  = 24,
  GDK_CLOCK	 	  = 26,
  GDK_COFFEE_MUG 	  = 28,
  GDK_CROSS 		  = 30,
  GDK_CROSS_REVERSE 	  = 32,
  GDK_CROSSHAIR 	  = 34,
  GDK_DIAMOND_CROSS 	  = 36,
  GDK_DOT 		  = 38,
  GDK_DOTBOX 		  = 40,
  GDK_DOUBLE_ARROW 	  = 42,
  GDK_DRAFT_LARGE 	  = 44,
  GDK_DRAFT_SMALL 	  = 46,
  GDK_DRAPED_BOX 	  = 48,
  GDK_EXCHANGE 		  = 50,
  GDK_FLEUR 		  = 52,
  GDK_GOBBLER 		  = 54,
  GDK_GUMBY 		  = 56,
  GDK_HAND1 		  = 58,
  GDK_HAND2 		  = 60,
  GDK_HEART 		  = 62,
  GDK_ICON 		  = 64,
  GDK_IRON_CROSS 	  = 66,
  GDK_LEFT_PTR 		  = 68,
  GDK_LEFT_SIDE 	  = 70,
  GDK_LEFT_TEE 		  = 72,
  GDK_LEFTBUTTON 	  = 74,
  GDK_LL_ANGLE 		  = 76,
  GDK_LR_ANGLE 	 	  = 78,
  GDK_MAN 		  = 80,
  GDK_MIDDLEBUTTON 	  = 82,
  GDK_MOUSE 		  = 84,
  GDK_PENCIL 		  = 86,
  GDK_PIRATE 		  = 88,
  GDK_PLUS 		  = 90,
  GDK_QUESTION_ARROW 	  = 92,
  GDK_RIGHT_PTR 	  = 94,
  GDK_RIGHT_SIDE 	  = 96,
  GDK_RIGHT_TEE 	  = 98,
  GDK_RIGHTBUTTON 	  = 100,
  GDK_RTL_LOGO 		  = 102,
  GDK_SAILBOAT 		  = 104,
  GDK_SB_DOWN_ARROW 	  = 106,
  GDK_SB_H_DOUBLE_ARROW   = 108,
  GDK_SB_LEFT_ARROW 	  = 110,
  GDK_SB_RIGHT_ARROW 	  = 112,
  GDK_SB_UP_ARROW 	  = 114,
  GDK_SB_V_DOUBLE_ARROW   = 116,
  GDK_SHUTTLE 		  = 118,
  GDK_SIZING 		  = 120,
  GDK_SPIDER		  = 122,
  GDK_SPRAYCAN 		  = 124,
  GDK_STAR 		  = 126,
  GDK_TARGET 		  = 128,
  GDK_TCROSS 		  = 130,
  GDK_TOP_LEFT_ARROW 	  = 132,
  GDK_TOP_LEFT_CORNER 	  = 134,
  GDK_TOP_RIGHT_CORNER 	  = 136,
  GDK_TOP_SIDE 		  = 138,
  GDK_TOP_TEE 		  = 140,
  GDK_TREK 		  = 142,
  GDK_UL_ANGLE 		  = 144,
  GDK_UMBRELLA 		  = 146,
  GDK_UR_ANGLE 		  = 148,
  GDK_WATCH 		  = 150,
  GDK_XTERM 		  = 152,
  GDK_LAST_CURSOR,
  GDK_BLANK_CURSOR        = -2,
  GDK_CURSOR_IS_PIXMAP 	  = -1
} GdkCursorType;

typedef enum 
{
  GDK_ACTION_DEFAULT = 1 << 0,
  GDK_ACTION_COPY    = 1 << 1,
  GDK_ACTION_MOVE    = 1 << 2,
  GDK_ACTION_LINK    = 1 << 3,
  GDK_ACTION_PRIVATE = 1 << 4,
  GDK_ACTION_ASK     = 1 << 5
} GdkDragAction;

typedef enum 
{
  GDK_DRAG_PROTO_MOTIF,
  GDK_DRAG_PROTO_XDND,
  GDK_DRAG_PROTO_ROOTWIN,	  /* A root window with nobody claiming
				   * drags */
  GDK_DRAG_PROTO_NONE,		  /* Not a valid drag window */
  GDK_DRAG_PROTO_WIN32_DROPFILES, /* The simple WM_DROPFILES dnd */
  GDK_DRAG_PROTO_OLE2,		  /* The complex OLE2 dnd (not implemented) */
  GDK_DRAG_PROTO_LOCAL            /* Intra-app */
} GdkDragProtocol;

typedef enum  {
  GDK_FILTER_CONTINUE,	  /* Event not handled, continue processesing */
  GDK_FILTER_TRANSLATE,	  /* Native event translated into a GDK event and
                             stored in the "event" structure that was
                             passed in */
  GDK_FILTER_REMOVE	  /* Terminate processing, removing event */
} GdkFilterReturn;

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

/* Types of font.
 *   GDK_FONT_FONT: the font is an XFontStruct.
 *   GDK_FONT_FONTSET: the font is an XFontSet used for I18N.
 */
typedef enum 
{
  GDK_FONT_FONT,
  GDK_FONT_FONTSET
} GdkFontType;

/* GC cap styles
 *  CapNotLast:
 *  CapButt:
 *  CapRound:
 *  CapProjecting:
 */
typedef enum 
{
  GDK_CAP_NOT_LAST,
  GDK_CAP_BUTT,
  GDK_CAP_ROUND,
  GDK_CAP_PROJECTING
} GdkCapStyle;

/* GC fill types.
 *  Solid:
 *  Tiled:
 *  Stippled:
 *  OpaqueStippled:
 */
typedef enum 
{
  GDK_SOLID,
  GDK_TILED,
  GDK_STIPPLED,
  GDK_OPAQUE_STIPPLED
} GdkFill;

/* GC function types.
 *   Copy: Overwrites destination pixels with the source pixels.
 *   Invert: Inverts the destination pixels.
 *   Xor: Xor's the destination pixels with the source pixels.
 *   Clear: set pixels to 0
 *   And: source AND destination
 *   And Reverse: source AND (NOT destination)
 *   And Invert: (NOT source) AND destination
 *   Noop: destination
 *   Or: source OR destination
 *   Nor: (NOT source) AND (NOT destination)
 *   Equiv: (NOT source) XOR destination
 *   Xor Reverse: source OR (NOT destination)
 *   Copy Inverted: NOT source
 *   Xor Inverted: (NOT source) OR destination
 *   Nand: (NOT source) OR (NOT destination)
 *   Set: set pixels to 1
 */
typedef enum 
{
  GDK_COPY,
  GDK_INVERT,
  GDK_XOR,
  GDK_CLEAR,
  GDK_AND,
  GDK_AND_REVERSE,
  GDK_AND_INVERT,
  GDK_NOOP,
  GDK_OR,
  GDK_EQUIV,
  GDK_OR_REVERSE,
  GDK_COPY_INVERT,
  GDK_OR_INVERT,
  GDK_NAND,
  GDK_NOR,
  GDK_SET
} GdkFunction;

/* GC join styles
 *  JoinMiter:
 *  JoinRound:
 *  JoinBevel:
 */
typedef enum 
{
  GDK_JOIN_MITER,
  GDK_JOIN_ROUND,
  GDK_JOIN_BEVEL
} GdkJoinStyle;

/* GC line styles
 *  Solid:
 *  OnOffDash:
 *  DoubleDash:
 */
typedef enum 
{
  GDK_LINE_SOLID,
  GDK_LINE_ON_OFF_DASH,
  GDK_LINE_DOUBLE_DASH
} GdkLineStyle;

typedef enum 
{
  GDK_CLIP_BY_CHILDREN	= 0,
  GDK_INCLUDE_INFERIORS = 1
} GdkSubwindowMode;

typedef enum 
{
  GDK_GC_FOREGROUND    = 1 << 0,
  GDK_GC_BACKGROUND    = 1 << 1,
  GDK_GC_FONT	       = 1 << 2,
  GDK_GC_FUNCTION      = 1 << 3,
  GDK_GC_FILL	       = 1 << 4,
  GDK_GC_TILE	       = 1 << 5,
  GDK_GC_STIPPLE       = 1 << 6,
  GDK_GC_CLIP_MASK     = 1 << 7,
  GDK_GC_SUBWINDOW     = 1 << 8,
  GDK_GC_TS_X_ORIGIN   = 1 << 9,
  GDK_GC_TS_Y_ORIGIN   = 1 << 10,
  GDK_GC_CLIP_X_ORIGIN = 1 << 11,
  GDK_GC_CLIP_Y_ORIGIN = 1 << 12,
  GDK_GC_EXPOSURES     = 1 << 13,
  GDK_GC_LINE_WIDTH    = 1 << 14,
  GDK_GC_LINE_STYLE    = 1 << 15,
  GDK_GC_CAP_STYLE     = 1 << 16,
  GDK_GC_JOIN_STYLE    = 1 << 17
} GdkGCValuesMask;

/* Types of images.
 *   Normal: Normal X image type. These are slow as they involve passing
 *	     the entire image through the X connection each time a draw
 *	     request is required. On Win32, a bitmap.
 *   Shared: Shared memory X image type. These are fast as the X server
 *	     and the program actually use the same piece of memory. They
 *	     should be used with care though as there is the possibility
 *	     for both the X server and the program to be reading/writing
 *	     the image simultaneously and producing undesired results.
 *	     On Win32, also a bitmap.
 */
typedef enum 
{
  GDK_IMAGE_NORMAL,
  GDK_IMAGE_SHARED,
  GDK_IMAGE_FASTEST
} GdkImageType;

typedef enum 
{
  GDK_EXTENSION_EVENTS_NONE,
  GDK_EXTENSION_EVENTS_ALL,
  GDK_EXTENSION_EVENTS_CURSOR
} GdkExtensionMode;

typedef enum 
{
  GDK_SOURCE_MOUSE,
  GDK_SOURCE_PEN,
  GDK_SOURCE_ERASER,
  GDK_SOURCE_CURSOR
} GdkInputSource;

typedef enum 
{
  GDK_MODE_DISABLED,
  GDK_MODE_SCREEN,
  GDK_MODE_WINDOW
} GdkInputMode;

typedef enum 
{
  GDK_AXIS_IGNORE,
  GDK_AXIS_X,
  GDK_AXIS_Y,
  GDK_AXIS_PRESSURE,
  GDK_AXIS_XTILT,
  GDK_AXIS_YTILT,
  GDK_AXIS_WHEEL,
  GDK_AXIS_LAST
} GdkAxisUse;

typedef enum 
{
  GDK_PROP_MODE_REPLACE,
  GDK_PROP_MODE_PREPEND,
  GDK_PROP_MODE_APPEND
} GdkPropMode;

#ifndef GDK_DISABLE_DEPRECATED
/* GC fill rule for polygons
 *  EvenOddRule
 *  WindingRule
 */
typedef enum 
{
  GDK_EVEN_ODD_RULE,
  GDK_WINDING_RULE
} GdkFillRule;
#endif

/* Types of overlapping between a rectangle and a region
 * GDK_OVERLAP_RECTANGLE_IN: rectangle is in region
 * GDK_OVERLAP_RECTANGLE_OUT: rectangle in not in region
 * GDK_OVERLAP_RECTANGLE_PART: rectangle in partially in region
 */
typedef enum 
{
  GDK_OVERLAP_RECTANGLE_IN,
  GDK_OVERLAP_RECTANGLE_OUT,
  GDK_OVERLAP_RECTANGLE_PART
} GdkOverlapType;

typedef enum 
{
  GDK_RGB_DITHER_NONE,
  GDK_RGB_DITHER_NORMAL,
  GDK_RGB_DITHER_MAX
} GdkRgbDither;

typedef enum 
{
  GDK_LSB_FIRST,
  GDK_MSB_FIRST
} GdkByteOrder;

/* Types of modifiers.
 */
typedef enum 
{
  GDK_SHIFT_MASK    = 1 << 0,
  GDK_LOCK_MASK	    = 1 << 1,
  GDK_CONTROL_MASK  = 1 << 2,
  GDK_MOD1_MASK	    = 1 << 3,
  GDK_MOD2_MASK	    = 1 << 4,
  GDK_MOD3_MASK	    = 1 << 5,
  GDK_MOD4_MASK	    = 1 << 6,
  GDK_MOD5_MASK	    = 1 << 7,
  GDK_BUTTON1_MASK  = 1 << 8,
  GDK_BUTTON2_MASK  = 1 << 9,
  GDK_BUTTON3_MASK  = 1 << 10,
  GDK_BUTTON4_MASK  = 1 << 11,
  GDK_BUTTON5_MASK  = 1 << 12,

  /* The next few modifiers are used by XKB, so we skip to the end.
   * Bits 15 - 25 are currently unused. Bit 29 is used internally.
   */
  
  GDK_SUPER_MASK    = 1 << 26,
  GDK_HYPER_MASK    = 1 << 27,
  GDK_META_MASK     = 1 << 28,
  
  GDK_RELEASE_MASK  = 1 << 30,

  GDK_MODIFIER_MASK = 0x5c001fff
} GdkModifierType;

typedef enum 
{
  GDK_INPUT_READ       = 1 << 0,
  GDK_INPUT_WRITE      = 1 << 1,
  GDK_INPUT_EXCEPTION  = 1 << 2
} GdkInputCondition;

typedef enum 
{
  GDK_OK	  = 0,
  GDK_ERROR	  = -1,
  GDK_ERROR_PARAM = -2,
  GDK_ERROR_FILE  = -3,
  GDK_ERROR_MEM	  = -4
} GdkStatus;

/* We define specific numeric values for these constants,
 * since old application code may depend on them matching the X values
 * We don't actually depend on the matchup ourselves.
 */
typedef enum 
{
  GDK_GRAB_SUCCESS         = 0,
  GDK_GRAB_ALREADY_GRABBED = 1,
  GDK_GRAB_INVALID_TIME    = 2,
  GDK_GRAB_NOT_VIEWABLE    = 3,
  GDK_GRAB_FROZEN          = 4
} GdkGrabStatus;

/* Types of visuals.
 *   StaticGray:
 *   Grayscale:
 *   StaticColor:
 *   PseudoColor:
 *   TrueColor:
 *   DirectColor:
 */
typedef enum 
{
  GDK_VISUAL_STATIC_GRAY,
  GDK_VISUAL_GRAYSCALE,
  GDK_VISUAL_STATIC_COLOR,
  GDK_VISUAL_PSEUDO_COLOR,
  GDK_VISUAL_TRUE_COLOR,
  GDK_VISUAL_DIRECT_COLOR
} GdkVisualType;

/* Classes of windows.
 *   InputOutput: Almost every window should be of this type. Such windows
 *		  receive events and are also displayed on screen.
 *   InputOnly: Used only in special circumstances when events need to be
 *		stolen from another window or windows. Input only windows
 *		have no visible output, so they are handy for placing over
 *		top of a group of windows in order to grab the events (or
 *		filter the events) from those windows.
 */
typedef enum 
{
  GDK_INPUT_OUTPUT,
  GDK_INPUT_ONLY
} GdkWindowWindowClass;

/* Types of windows.
 *   Root: There is only 1 root window and it is initialized
 *	   at startup. Creating a window of type GDK_WINDOW_ROOT
 *	   is an error.
 *   Toplevel: Windows which interact with the window manager.
 *   Child: Windows which are children of some other type of window.
 *	    (Any other type of window). Most windows are child windows.
 *   Dialog: A special kind of toplevel window which interacts with
 *	     the window manager slightly differently than a regular
 *	     toplevel window. Dialog windows should be used for any
 *	     transient window.
 *   Foreign: A window that actually belongs to another application
 */
typedef enum 
{
  GDK_WINDOW_ROOT,
  GDK_WINDOW_TOPLEVEL,
  GDK_WINDOW_CHILD,
  GDK_WINDOW_DIALOG,
  GDK_WINDOW_TEMP,
  GDK_WINDOW_FOREIGN,
  GDK_WINDOW_OFFSCREEN
} GdkWindowType;

/* Window attribute mask values.
 *   GDK_WA_TITLE: The "title" field is valid.
 *   GDK_WA_X: The "x" field is valid.
 *   GDK_WA_Y: The "y" field is valid.
 *   GDK_WA_CURSOR: The "cursor" field is valid.
 *   GDK_WA_COLORMAP: The "colormap" field is valid.
 *   GDK_WA_VISUAL: The "visual" field is valid.
 */
typedef enum 
{
  GDK_WA_TITLE	   = 1 << 1,
  GDK_WA_X	   = 1 << 2,
  GDK_WA_Y	   = 1 << 3,
  GDK_WA_CURSOR	   = 1 << 4,
  GDK_WA_COLORMAP  = 1 << 5,
  GDK_WA_VISUAL	   = 1 << 6,
  GDK_WA_WMCLASS   = 1 << 7,
  GDK_WA_NOREDIR   = 1 << 8,
  GDK_WA_TYPE_HINT = 1 << 9
} GdkWindowAttributesType;

/* Size restriction enumeration.
 */
typedef enum 
{
  GDK_HINT_POS	       = 1 << 0,
  GDK_HINT_MIN_SIZE    = 1 << 1,
  GDK_HINT_MAX_SIZE    = 1 << 2,
  GDK_HINT_BASE_SIZE   = 1 << 3,
  GDK_HINT_ASPECT      = 1 << 4,
  GDK_HINT_RESIZE_INC  = 1 << 5,
  GDK_HINT_WIN_GRAVITY = 1 << 6,
  GDK_HINT_USER_POS    = 1 << 7,
  GDK_HINT_USER_SIZE   = 1 << 8
} GdkWindowHints;


/* Window type hints.
 * These are hints for the window manager that indicate
 * what type of function the window has. The window manager
 * can use this when determining decoration and behaviour
 * of the window. The hint must be set before mapping the
 * window.
 *
 *   Normal: Normal toplevel window
 *   Dialog: Dialog window
 *   Menu: Window used to implement a menu.
 *   Toolbar: Window used to implement toolbars.
 */
typedef enum 
{
  GDK_WINDOW_TYPE_HINT_NORMAL,
  GDK_WINDOW_TYPE_HINT_DIALOG,
  GDK_WINDOW_TYPE_HINT_MENU,		/* Torn off menu */
  GDK_WINDOW_TYPE_HINT_TOOLBAR,
  GDK_WINDOW_TYPE_HINT_SPLASHSCREEN,
  GDK_WINDOW_TYPE_HINT_UTILITY,
  GDK_WINDOW_TYPE_HINT_DOCK,
  GDK_WINDOW_TYPE_HINT_DESKTOP,
  GDK_WINDOW_TYPE_HINT_DROPDOWN_MENU,	/* A drop down menu (from a menubar) */
  GDK_WINDOW_TYPE_HINT_POPUP_MENU,	/* A popup menu (from right-click) */
  GDK_WINDOW_TYPE_HINT_TOOLTIP,
  GDK_WINDOW_TYPE_HINT_NOTIFICATION,
  GDK_WINDOW_TYPE_HINT_COMBO,
  GDK_WINDOW_TYPE_HINT_DND
} GdkWindowTypeHint;

/* The next two enumeration values current match the
 * Motif constants. If this is changed, the implementation
 * of gdk_window_set_decorations/gdk_window_set_functions
 * will need to change as well.
 */
typedef enum 
{
  GDK_DECOR_ALL		= 1 << 0,
  GDK_DECOR_BORDER	= 1 << 1,
  GDK_DECOR_RESIZEH	= 1 << 2,
  GDK_DECOR_TITLE	= 1 << 3,
  GDK_DECOR_MENU	= 1 << 4,
  GDK_DECOR_MINIMIZE	= 1 << 5,
  GDK_DECOR_MAXIMIZE	= 1 << 6
} GdkWMDecoration;

typedef enum 
{
  GDK_FUNC_ALL		= 1 << 0,
  GDK_FUNC_RESIZE	= 1 << 1,
  GDK_FUNC_MOVE		= 1 << 2,
  GDK_FUNC_MINIMIZE	= 1 << 3,
  GDK_FUNC_MAXIMIZE	= 1 << 4,
  GDK_FUNC_CLOSE	= 1 << 5
} GdkWMFunction;

/* Currently, these are the same values numerically as in the
 * X protocol. If you change that, gdkwindow-x11.c/SF(gdk_window_set_geometry_hints)()
 * will need fixing.
 */
typedef enum 
{
  GDK_GRAVITY_NORTH_WEST = 1,
  GDK_GRAVITY_NORTH,
  GDK_GRAVITY_NORTH_EAST,
  GDK_GRAVITY_WEST,
  GDK_GRAVITY_CENTER,
  GDK_GRAVITY_EAST,
  GDK_GRAVITY_SOUTH_WEST,
  GDK_GRAVITY_SOUTH,
  GDK_GRAVITY_SOUTH_EAST,
  GDK_GRAVITY_STATIC
} GdkGravity;


typedef enum 
{
  GDK_WINDOW_EDGE_NORTH_WEST,
  GDK_WINDOW_EDGE_NORTH,
  GDK_WINDOW_EDGE_NORTH_EAST,
  GDK_WINDOW_EDGE_WEST,
  GDK_WINDOW_EDGE_EAST,
  GDK_WINDOW_EDGE_SOUTH_WEST,
  GDK_WINDOW_EDGE_SOUTH,
  GDK_WINDOW_EDGE_SOUTH_EAST  
} GdkWindowEdge;

G_END_DECLS

#endif /* __GDK_ENUMS_H__ */
