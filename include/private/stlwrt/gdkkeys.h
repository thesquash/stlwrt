
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat, Inc.
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

#ifndef __GDK_KEYS_H__
#define __GDK_KEYS_H__

#include <gdktypes.h>

G_BEGIN_DECLS

typedef struct _GdkKeymapKey GdkKeymapKey;

/* GdkKeymapKey is a hardware key that can be mapped to a keyval */
struct _GdkKeymapKey
{
  guint keycode;
  gint  group;
  gint  level;
};

/* A GdkKeymap defines the translation from keyboard state
 * (including a hardware key, a modifier mask, and active keyboard group)
 * to a keyval. This translation has two phases. The first phase is
 * to determine the effective keyboard group and level for the keyboard
 * state; the second phase is to look up the keycode/group/level triplet
 * in the keymap and see what keyval it corresponds to.
 */

typedef struct _GdkKeymap      GdkKeymapFat;
typedef struct _GdkKeymap      GdkKeymapThin;

typedef struct _GdkKeymapClass GdkKeymapClass;

#define GDK_TYPE_KEYMAP              (gdk_keymap_get_type ())
#define GDK_KEYMAP(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_KEYMAP, GdkKeymap))
#define GDK_KEYMAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_KEYMAP, GdkKeymapClass))
#define GDK_IS_KEYMAP(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_KEYMAP))
#define GDK_IS_KEYMAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_KEYMAP))
#define GDK_KEYMAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_KEYMAP, GdkKeymapClass))

/********************************************************************/
struct _GdkKeymapInstanceProps
{

  GdkDisplay * (display);
};

struct _GdkKeymapFat
{
  GObjectFat       parent_instance;

  struct _GdkKeymapInstanceProps instance_properties;
};

struct _GdkKeymapThin
{
  GObjectThin      parent_instance;

  gpointer reserved;
};


typedef union
{
  struct _GdkKeymapFat   fat_instance;
  struct _GdkKeymapThin  thin_instance;
}   GdkKeymap;
/********************************************************************/



struct _GdkKeymapClass
{
  GObjectClass parent_class;

  void (*direction_changed) (GdkKeymap *keymap);
  void (*keys_changed)      (GdkKeymap *keymap);
  void (*state_changed)     (GdkKeymap *keymap);
};

GType _T2_gdk_keymap_get_type (void) G_GNUC_CONST;
GType _3T_gdk_keymap_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType gdk_keymap_get_type (void) G_GNUC_CONST;

#ifndef GDK_MULTIHEAD_SAFE
GdkKeymap* __gdk_keymap_get_default     (void);
#endif
GdkKeymap* __gdk_keymap_get_for_display (GdkDisplay *display);


guint          __gdk_keymap_lookup_key               (GdkKeymap           *keymap,
						    const GdkKeymapKey  *key);
gboolean       __gdk_keymap_translate_keyboard_state (GdkKeymap           *keymap,
						    guint                hardware_keycode,
						    GdkModifierType      state,
						    gint                 group,
						    guint               *keyval,
						    gint                *effective_group,
						    gint                *level,
						    GdkModifierType     *consumed_modifiers);
gboolean       __gdk_keymap_get_entries_for_keyval   (GdkKeymap           *keymap,
						    guint                keyval,
						    GdkKeymapKey       **keys,
						    gint                *n_keys);
gboolean       __gdk_keymap_get_entries_for_keycode  (GdkKeymap           *keymap,
						    guint                hardware_keycode,
						    GdkKeymapKey       **keys,
						    guint              **keyvals,
						    gint                *n_entries);
PangoDirection __gdk_keymap_get_direction            (GdkKeymap           *keymap);
gboolean       __gdk_keymap_have_bidi_layouts        (GdkKeymap           *keymap);
gboolean       __gdk_keymap_get_caps_lock_state      (GdkKeymap           *keymap);
void           __gdk_keymap_add_virtual_modifiers    (GdkKeymap           *keymap,
                                                    GdkModifierType     *state);
gboolean       __gdk_keymap_map_virtual_modifiers    (GdkKeymap           *keymap,
                                                    GdkModifierType     *state);

/* Key values
 */
gchar*   __gdk_keyval_name         (guint        keyval) G_GNUC_CONST;
guint    __gdk_keyval_from_name    (const gchar *keyval_name);
void     __gdk_keyval_convert_case (guint        symbol,
				  guint       *lower,
				  guint       *upper);
guint    __gdk_keyval_to_upper     (guint        keyval) G_GNUC_CONST;
guint    __gdk_keyval_to_lower     (guint        keyval) G_GNUC_CONST;
gboolean __gdk_keyval_is_upper     (guint        keyval) G_GNUC_CONST;
gboolean __gdk_keyval_is_lower     (guint        keyval) G_GNUC_CONST;

guint32  __gdk_keyval_to_unicode   (guint        keyval) G_GNUC_CONST;
guint    __gdk_unicode_to_keyval   (guint32      wc) G_GNUC_CONST;


G_END_DECLS

#endif /* __GDK_KEYS_H__ */
