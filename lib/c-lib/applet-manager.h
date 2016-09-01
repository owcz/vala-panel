#ifndef APPLETMANAGER_C
#define APPLETMANAGER_C

#include "applet-engine-module.h"
#include "lib/applets-new/applet-api.h"
#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(ValaPanelAppletManager, vala_panel_applet_manager, VALA_PANEL, APPLET_MANAGER,
		     GObject)

char **vala_panel_applet_manager_get_available_types(ValaPanelAppletManager *self);
ValaPanelAppletInfo *vala_panel_applet_manager_get_applet_info_for_type(
    ValaPanelAppletManager *self, const char *applet_type);
ValaPanelAppletWidget *vala_panel_applet_manager_get_applet_widget_for_type(
    ValaPanelAppletManager *self, const char *applet_type, const char *uuid);

G_END_DECLS

#endif // APPLETMANAGER_C
