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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_STYLE_H__
#define __GTK_STYLE_H__

#include <stlwrt.h>


#include <gdk.h>

#include <gtkenums.h>


G_BEGIN_DECLS

#define GTK_TYPE_STYLE              (gtk_style_get_type ())
#define GTK_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GTK_TYPE_STYLE, GtkStyle))
#define GTK_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_STYLE, GtkStyleClass))
#define GTK_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GTK_TYPE_STYLE))
#define GTK_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_STYLE))
#define GTK_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_STYLE, GtkStyleClass))

#define GTK_TYPE_BORDER             (gtk_border_get_type ())

typedef struct _GtkStyleClass GtkStyleClass;

/* Some forward declarations needed to rationalize the header
 * files.
 */

typedef gboolean (*GtkRcPropertyParser) (const GParamSpec *pspec,
					 const GString    *rc_string,
					 GValue           *property_value);

/* We make this forward declaration here, since we pass
 * GtkWidget's to the draw functions.
 */

#define GTK_STYLE_ATTACHED(style)	(GTK_STYLE (style)->attach_count > 0)


/*
 * GtkStyle should not be used in GTK+ 3 and later applications, so we can
 * make it an Ftype in STLWRT.
 */
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

struct _GtkStyleClass
{
  GObjectClass parent_class;

  /* Initialize for a particular colormap/depth
   * combination. style->colormap/style->depth will have
   * been set at this point. Will typically chain to parent.
   */
  void (*realize)               (GtkStyle               *style);

  /* Clean up for a particular colormap/depth combination. Will
   * typically chain to parent.
   */
  void (*unrealize)             (GtkStyle               *style);

  /* Make style an exact duplicate of src.
   */
  void (*copy)                  (GtkStyle               *style,
				 GtkStyle               *src);

  /* Create an empty style of the same type as this style.
   * The default implementation, which does
   * g_object_new (G_OBJECT_TYPE (style), NULL);
   * should work in most cases.
   */
  GtkStyle *(*clone)             (GtkStyle               *style);

  /* Initialize the GtkStyle with the values in the GtkRcStyle.
   * should chain to the parent implementation.
   */
  void     (*init_from_rc)      (GtkStyle               *style,
				 GtkRcStyle             *rc_style);

  void (*set_background)        (GtkStyle               *style,
				 GdkWindow              *window,
				 GtkStateType            state_type);


  GdkPixbuf * (* render_icon)   (GtkStyle               *style,
                                 const GtkIconSource    *source,
                                 GtkTextDirection        direction,
                                 GtkStateType            state,
                                 GtkIconSize             size,
                                 GtkWidget              *widget,
                                 const gchar            *detail);

  /* Drawing functions
   */

  void (*draw_hline)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x1,
				 gint			 x2,
				 gint			 y);
  void (*draw_vline)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 y1_,
				 gint			 y2_,
				 gint			 x);
  void (*draw_shadow)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_polygon)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 GdkPoint		*point,
				 gint			 npoints,
				 gboolean		 fill);
  void (*draw_arrow)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 GtkArrowType		 arrow_type,
				 gboolean		 fill,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_diamond)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_string)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 const gchar		*string);
  void (*draw_box)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_flat_box)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_check)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_option)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_tab)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_shadow_gap)	(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height,
				 GtkPositionType	 gap_side,
				 gint			 gap_x,
				 gint			 gap_width);
  void (*draw_box_gap)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height,
				 GtkPositionType	 gap_side,
				 gint			 gap_x,
				 gint			 gap_width);
  void (*draw_extension)	(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height,
				 GtkPositionType	 gap_side);
  void (*draw_focus)		(GtkStyle		*style,
				 GdkWindow		*window,
                                 GtkStateType            state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_slider)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height,
				 GtkOrientation		 orientation);
  void (*draw_handle)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GtkShadowType		 shadow_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height,
				 GtkOrientation		 orientation);

  void (*draw_expander)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
                                 GtkExpanderStyle        expander_style);
  void (*draw_layout)		(GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 gboolean                use_text,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 gint			 x,
				 gint			 y,
                                 PangoLayout            *layout);
  void (*draw_resize_grip)      (GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
                                 GdkWindowEdge           edge,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);
  void (*draw_spinner)          (GtkStyle		*style,
				 GdkWindow		*window,
				 GtkStateType		 state_type,
				 GdkRectangle		*area,
				 GtkWidget		*widget,
				 const gchar		*detail,
				 guint                   step,
				 gint			 x,
				 gint			 y,
				 gint			 width,
				 gint			 height);

  /* Padding for future expansion */
  void (*_gtk_reserved1)  (void);
  void (*_gtk_reserved2)  (void);
  void (*_gtk_reserved3)  (void);
  void (*_gtk_reserved4)  (void);
  void (*_gtk_reserved5)  (void);
  void (*_gtk_reserved6)  (void);
  void (*_gtk_reserved7)  (void);
  void (*_gtk_reserved8)  (void);
  void (*_gtk_reserved9)  (void);
  void (*_gtk_reserved10) (void);
  void (*_gtk_reserved11) (void);
};

STLWRT_DECLARE_BOXED_TYPE(GtkBorder, gtk_border,
  gint left;
  gint right;
  gint top;
  gint bottom;
)


GtkStyle* SF(gtk_style_new)			     (void);
GtkStyle* SF(gtk_style_copy)		     (GtkStyle	   *style);
GtkStyle* SF(gtk_style_attach)		     (GtkStyle	   *style,
					      GdkWindow	   *window) G_GNUC_WARN_UNUSED_RESULT;
void	  SF(gtk_style_detach)		     (GtkStyle	   *style);

#ifndef GTK_DISABLE_DEPRECATED
GtkStyle* SF(gtk_style_ref)			     (GtkStyle	   *style);
void	  SF(gtk_style_unref)		     (GtkStyle	   *style);

GdkFont * SF(gtk_style_get_font)                 (GtkStyle     *style);
void      SF(gtk_style_set_font)                 (GtkStyle     *style,
					      GdkFont      *font);
#endif /* GTK_DISABLE_DEPRECATED */

void	  SF(gtk_style_set_background)	     (GtkStyle	   *style,
					      GdkWindow	   *window,
					      GtkStateType  state_type);
void	  SF(gtk_style_apply_default_background) (GtkStyle	   *style,
					      GdkWindow	   *window,
					      gboolean	    set_bg,
					      GtkStateType  state_type,
					      const GdkRectangle *area,
					      gint	    x,
					      gint	    y,
					      gint	    width,
					      gint	    height);

GtkIconSet* SF(gtk_style_lookup_icon_set)        (GtkStyle     *style,
                                              const gchar  *stock_id);
gboolean    SF(gtk_style_lookup_color)           (GtkStyle     *style,
                                              const gchar  *color_name,
                                              GdkColor     *color);

GdkPixbuf*  SF(gtk_style_render_icon)     (GtkStyle            *style,
                                       const GtkIconSource *source,
                                       GtkTextDirection     direction,
                                       GtkStateType         state,
                                       GtkIconSize          size,
                                       GtkWidget           *widget,
                                       const gchar         *detail);

#ifndef GTK_DISABLE_DEPRECATED
void SF(gtk_draw_hline)      (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  gint             x1,
			  gint             x2,
			  gint             y);
void SF(gtk_draw_vline)      (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  gint             y1_,
			  gint             y2_,
			  gint             x);
void SF(gtk_draw_shadow)     (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_polygon)    (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  GdkPoint        *points,
			  gint             npoints,
			  gboolean         fill);
void SF(gtk_draw_arrow)      (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  GtkArrowType     arrow_type,
			  gboolean         fill,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_diamond)    (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_box)        (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_flat_box)   (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_check)      (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_option)     (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_tab)        (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_shadow_gap) (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height,
			  GtkPositionType  gap_side,
			  gint             gap_x,
			  gint             gap_width);
void SF(gtk_draw_box_gap)    (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height,
			  GtkPositionType  gap_side,
			  gint             gap_x,
			  gint             gap_width);
void SF(gtk_draw_extension)  (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height,
			  GtkPositionType  gap_side);
void SF(gtk_draw_focus)      (GtkStyle        *style,
			  GdkWindow       *window,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height);
void SF(gtk_draw_slider)     (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height,
			  GtkOrientation   orientation);
void SF(gtk_draw_handle)     (GtkStyle        *style,
			  GdkWindow       *window,
			  GtkStateType     state_type,
			  GtkShadowType    shadow_type,
			  gint             x,
			  gint             y,
			  gint             width,
			  gint             height,
			  GtkOrientation   orientation);
void SF(gtk_draw_expander)   (GtkStyle        *style,
                          GdkWindow       *window,
                          GtkStateType     state_type,
                          gint             x,
                          gint             y,
			  GtkExpanderStyle expander_style);
void SF(gtk_draw_layout)     (GtkStyle        *style,
                          GdkWindow       *window,
                          GtkStateType     state_type,
			  gboolean         use_text,
                          gint             x,
                          gint             y,
                          PangoLayout     *layout);
void SF(gtk_draw_resize_grip) (GtkStyle       *style,
                           GdkWindow      *window,
                           GtkStateType    state_type,
                           GdkWindowEdge   edge,
                           gint            x,
                           gint            y,
                           gint            width,
                           gint            height);
#endif /* GTK_DISABLE_DEPRECATED */

void SF(gtk_paint_hline)       (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x1,
			    gint                x2,
			    gint                y);
void SF(gtk_paint_vline)       (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                y1_,
			    gint                y2_,
			    gint                x);
void SF(gtk_paint_shadow)      (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_polygon)     (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    const GdkPoint     *points,
			    gint                n_points,
			    gboolean            fill);
void SF(gtk_paint_arrow)       (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    GtkArrowType        arrow_type,
			    gboolean            fill,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_diamond)     (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_box)         (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_flat_box)    (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_check)       (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_option)      (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_tab)         (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_shadow_gap)  (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height,
			    GtkPositionType     gap_side,
			    gint                gap_x,
			    gint                gap_width);
void SF(gtk_paint_box_gap)     (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height,
			    GtkPositionType     gap_side,
			    gint                gap_x,
			    gint                gap_width);
void SF(gtk_paint_extension)   (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height,
			    GtkPositionType     gap_side);
void SF(gtk_paint_focus)       (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);
void SF(gtk_paint_slider)      (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height,
			    GtkOrientation      orientation);
void SF(gtk_paint_handle)      (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
			    GtkShadowType       shadow_type,
			    const GdkRectangle *area,
			    GtkWidget          *widget,
			    const gchar        *detail,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height,
			    GtkOrientation      orientation);
void SF(gtk_paint_expander)    (GtkStyle           *style,
                            GdkWindow          *window,
                            GtkStateType        state_type,
                            const GdkRectangle *area,
                            GtkWidget          *widget,
                            const gchar        *detail,
                            gint                x,
                            gint                y,
			    GtkExpanderStyle    expander_style);
void SF(gtk_paint_layout)      (GtkStyle           *style,
                            GdkWindow          *window,
                            GtkStateType        state_type,
			    gboolean            use_text,
                            const GdkRectangle *area,
                            GtkWidget          *widget,
                            const gchar        *detail,
                            gint                x,
                            gint                y,
                            PangoLayout        *layout);
void SF(gtk_paint_resize_grip) (GtkStyle           *style,
                            GdkWindow          *window,
                            GtkStateType        state_type,
                            const GdkRectangle *area,
                            GtkWidget          *widget,
                            const gchar        *detail,
                            GdkWindowEdge       edge,
                            gint                x,
                            gint                y,
                            gint                width,
                            gint                height);
void SF(gtk_paint_spinner)     (GtkStyle           *style,
			    GdkWindow          *window,
			    GtkStateType        state_type,
                            const GdkRectangle *area,
                            GtkWidget          *widget,
                            const gchar        *detail,
			    guint               step,
			    gint                x,
			    gint                y,
			    gint                width,
			    gint                height);

GtkBorder *SF(gtk_border_new)      (void) G_GNUC_MALLOC;
GtkBorder *SF(gtk_border_copy)     (const GtkBorder *border_);
void       SF(gtk_border_free)     (GtkBorder       *border_);

void SF(gtk_style_get_style_property) (GtkStyle    *style,
                                   GType        widget_type,
                                   const gchar *property_name,
                                   GValue      *value);
void SF(gtk_style_get_valist)         (GtkStyle    *style,
                                   GType        widget_type,
                                   const gchar *first_property_name,
                                   va_list      var_args);
void SF(gtk_style_get)                (GtkStyle    *style,
                                   GType        widget_type,
                                   const gchar *first_property_name,
                                   ...) G_GNUC_NULL_TERMINATED;

/* --- private API --- */
const GValue* SF(_gtk_style_peek_property_value) (GtkStyle           *style,
					      GType               widget_type,
					      GParamSpec         *pspec,
					      GtkRcPropertyParser parser);

void          SF(_gtk_style_init_for_settings)   (GtkStyle           *style,
                                              GtkSettings        *settings);

void          SF(_gtk_style_shade)               (const GdkColor     *a,
                                              GdkColor           *b,
                                              gdouble             k);

/* deprecated */
#ifndef GTK_DISABLE_DEPRECATED
#define gtk_style_apply_default_pixmap(s,gw,st,a,x,y,w,h) gtk_style_apply_default_background (s,gw,1,st,a,x,y,w,h)
void SF(gtk_draw_string)      (GtkStyle           *style,
			   GdkWindow          *window,
                           GtkStateType        state_type,
                           gint                x,
                           gint                y,
                           const gchar        *string);
void SF(gtk_paint_string)     (GtkStyle           *style,
			   GdkWindow          *window,
			   GtkStateType        state_type,
			   const GdkRectangle *area,
			   GtkWidget          *widget,
			   const gchar        *detail,
			   gint                x,
			   gint                y,
			   const gchar        *string);
#endif /* GTK_DISABLE_DEPRECATED */

void   SF(gtk_draw_insertion_cursor)    (GtkWidget          *widget,
                                     GdkDrawable        *drawable,
                                     const GdkRectangle *area,
                                     const GdkRectangle *location,
                                     gboolean            is_primary,
                                     GtkTextDirection    direction,
                                     gboolean            draw_arrow);
GdkGC *SF(_gtk_widget_get_cursor_gc)    (GtkWidget          *widget);
void   SF(_gtk_widget_get_cursor_color) (GtkWidget          *widget,
				     GdkColor           *color);

G_END_DECLS

#endif /* __GTK_STYLE_H__ */
