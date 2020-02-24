/*
  pw13_gtk - A Patchwork13! GTK+2 User Interface
  Copyright (C) 2005  Thomas de Grivel <billitch@yahoo.fr>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*

  pw_draw.c  -  patchwork drawing

*/
#include <math.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <pw13/pw13.h>
#include "pw_draw.h"
#include "connect.h"
#include "patch.h"
#include "patchwork.h"
#include "output.h"
#include "input.h"


#if ! GTK_CHECK_VERSION(2,7,0)
#include <gdk/gdkx.h>
#include <cairo-xlib.h>
/* copied from gtk+/gdk/gdkcairo.c and gtk+/gdk/x11/gdkdrawable-x11.c
   gdk_cairo_create() which is available in 2.7.0 and later.
*/
static cairo_t *
gdk_cairo_create (GdkDrawable *drawable)
{
    int width, height;
    cairo_t *cr = NULL;
    cairo_surface_t *surface = NULL;
    GdkVisual *visual = gdk_drawable_get_visual (drawable);

    gdk_drawable_get_size (drawable, &width, &height);
    if (visual)
      surface = cairo_xlib_surface_create (GDK_DRAWABLE_XDISPLAY (drawable),
					   GDK_DRAWABLE_XID (drawable),
					   GDK_VISUAL_XVISUAL (visual),
					   width, height);
    else if (gdk_drawable_get_depth (drawable) == 1)
      surface = cairo_xlib_surface_create_for_bitmap
	(GDK_PIXMAP_XDISPLAY (drawable),
	 GDK_PIXMAP_XID (drawable),
	 GDK_SCREEN_XSCREEN (gdk_drawable_get_screen (drawable)),
	 width, height);
    else {
      g_warning ("Using Cairo rendering requires the drawable argument to\n"
		 "have a specified colormap. All windows have a colormap,\n"
		 "however, pixmaps only have colormap by default if they\n"
		 "were created with a non-NULL window argument. Otherwise\n"
		 "a colormap must be set on them with "
		 "gdk_drawable_set_colormap");
      return NULL;
    }
    if (surface) {
      cr = cairo_create (surface);
      cairo_surface_destroy (surface);
    }
    return cr;
}

#endif


static void pw_draw_arrow (int x1, int y1, int x2, int y2, cairo_t *cr)
{
  /* size of arrow */
  int taille_fleche = 4;
  int mx = (x2 - x1) / 3;
  if (mx < 0) {
    mx *= -1;
    mx += mx / 2;
  }

  cairo_move_to (cr,  x1, y1);
  cairo_curve_to (cr,
		  x1 + mx, y1,
		  x2 - mx - taille_fleche, y2,
		  x2 - taille_fleche, y2);

  /* drawing arrow */
  /* tail */
  cairo_move_to (cr, x2, y2);
  cairo_line_to (cr, x2 - taille_fleche, y2);
  /* head */
  cairo_move_to (cr, x2 - taille_fleche, y2 - taille_fleche);
  cairo_line_to (cr, x2, y2);
  cairo_line_to (cr, x2 - taille_fleche, y2 + taille_fleche);
}


static void pw_draw_connect (Pw13_GtkConnect *c, cairo_t *cr)
{
  int x1, y1, x2, y2;
  if (gtk_expander_get_expanded (GTK_EXPANDER (c->o->p->expander))) {
    y1 = 0;
    /* transform the coordinates 0 and cop[y] in the checkbox to coordinates in the top widget*/
    /*maybe false allocation with c->o->checkbox*/
    gtk_widget_translate_coordinates (c->o->checkbox, c->o->p->widget,
				      0, c->o->p->y, &x1, &y1);
    y1 += c->o->widget->allocation.height / 2;
  }
  else
    y1 = c->o->p->y + c->o->p->expander->allocation.height / 2;

  if (gtk_expander_get_expanded (GTK_EXPANDER (c->i->p->expander))) {
    gtk_widget_translate_coordinates (c->i->checkbox, c->i->p->widget,
				      0, c->i->p->y, &x2, &y2);
    y2 += c->i->widget->allocation.height / 2;
  }
  else
    y2 = c->i->p->y + c->i->p->expander->allocation.height / 2;

  x1 = c->o->p->x + c->o->p->widget->allocation.width;
  x2 = c->i->p->x - 1;

  if (c->o->active && c->i->active)
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1.0);
  else
    cairo_set_source_rgba (cr, 0.6, 0.6, 0.6, 1.0);

  pw_draw_arrow (x1, y1, x2, y2, cr);
  cairo_stroke (cr);
}


void pw13_gtk_patchwork_draw (Pw13_GtkPatchwork *pw, GtkWidget *widget)
{
  if (pw) {
    GdkWindow *window = GTK_LAYOUT (widget)->bin_window;
    cairo_t *cr = gdk_cairo_create (window);
    Pw13_List *t;

    /*pw_draw_connect en boucle sur liste de connect */
    t = pw->connections;
    while (t != NULL) {
      pw_draw_connect( t->elem, cr);
      t = t->suiv;
    }
    /* */
    cairo_destroy (cr);
  }
}
