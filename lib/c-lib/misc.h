#ifndef MISC_H
#define MISC_H

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

void vala_panel_scale_button_set_range (GtkScaleButton* b, gint lower, gint upper);
void vala_panel_scale_button_set_value_labeled (GtkScaleButton* b, gint value);
void vala_panel_add_prop_as_action(GActionMap* map,const char* prop);
void vala_panel_add_gsettings_as_action(GActionMap* map, GSettings* settings,const char* prop);
void vala_panel_bind_gsettings(GObject* obj, GSettings* settings, const gchar* prop);
void vala_panel_apply_window_icon(GtkWindow* win);

G_END_DECLS

#endif // MISC_H
