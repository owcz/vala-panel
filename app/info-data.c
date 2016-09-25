#include "info-data.h"
#include "boxed-wrapper.h"
#include <glib/gi18n.h>
#include <string.h>

/*
 * InfoData struct
 */

static char *generate_markup(const char *name, const char *sdesc)
{
	g_autofree char *nom  = g_markup_escape_text(name, strlen(name));
	g_autofree char *desc = g_markup_escape_text(sdesc, strlen(sdesc));
	return g_strdup_printf("<big>%s</big>\n<small>%s</small>", nom, desc);
}

InfoData *info_data_new_from_info(GAppInfo *info)
{
	if (g_app_info_get_executable(info) == NULL)
		return NULL;
	InfoData *data            = (InfoData *)g_malloc0(sizeof(InfoData));
	g_autofree char *icon_str = g_icon_to_string(g_app_info_get_icon(info));
	data->icon                = g_app_info_get_icon(info);
	if (!icon_str)
		data->icon = g_themed_icon_new_with_default_fallbacks("system-run-symbolic");
	else
		data->icon = g_icon_new_for_string(icon_str, NULL);
	data->disp_name    = g_strdup(g_app_info_get_display_name(info));
	const char *name =
	    g_app_info_get_name(info) ? g_app_info_get_name(info) : g_app_info_get_executable(info);
	const char *sdesc =
	    g_app_info_get_description(info) ? g_app_info_get_description(info) : "";
	data->name_markup = generate_markup(name, sdesc);
	data->command     = g_strdup(g_app_info_get_executable(info));
	return data;
}

InfoData *info_data_new_from_command(const char *command)
{
	InfoData *data        = (InfoData *)g_malloc0(sizeof(InfoData));
	data->icon            = g_themed_icon_new_with_default_fallbacks("system-run-symbolic");
	data->disp_name       = g_strdup_printf(_("Run %s"), command);
	g_autofree char *name = g_strdup_printf(_("Run %s"), command);
	const char *sdesc     = _("Run system command");
	data->name_markup     = generate_markup(name, sdesc);
	data->command         = g_strdup(command);
	return data;
}

static InfoData *info_data_dup(InfoData *base)
{
	InfoData *new_data        = (InfoData *)g_malloc0(sizeof(InfoData));
	g_autofree char *icon_str = g_icon_to_string(base->icon);
	new_data->icon            = g_icon_new_for_string(icon_str, NULL);
	new_data->disp_name       = g_strdup(base->disp_name);
	new_data->name_markup     = g_strdup(base->name_markup);
	new_data->command         = g_strdup(base->command);
	return new_data;
}

void info_data_free(InfoData *data)
{
	g_object_unref(data->icon);
	g_free(data->disp_name);
	g_free(data->name_markup);
	g_free(data->command);
	g_free(data);
}

G_DEFINE_BOXED_TYPE(InfoData, info_data, info_data_dup, info_data_free)

/*
 * InfoDataModel GObject
 */

struct _InfoDataModel
{
	GObject __parent__;
	GSequence *seq;
};

static void info_data_model_iface_init(GListModelInterface *iface);

G_DEFINE_TYPE_WITH_CODE(InfoDataModel, info_data_model, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE(G_TYPE_LIST_MODEL, info_data_model_iface_init))

static GType info_data_model_get_item_type(GListModel *lst)
{
	return info_data_get_type();
}

static uint info_data_model_get_n_items(GListModel *lst)
{
	InfoDataModel *self = VALA_PANEL_INFO_DATA_MODEL(lst);
	return (uint)g_sequence_get_length(self->seq);
}

static gpointer info_data_model_get_item(GListModel *lst, uint pos)
{
	InfoDataModel *self = VALA_PANEL_INFO_DATA_MODEL(lst);
	GSequenceIter *iter = g_sequence_get_iter_at_pos(self->seq, pos);
	return (InfoData *)g_sequence_get(iter);
}

static void info_data_model_iface_init(GListModelInterface *iface)
{
	iface->get_item_type = info_data_model_get_item_type;
	iface->get_item      = info_data_model_get_item;
	iface->get_n_items   = info_data_model_get_n_items;
}

static void info_data_model_init(InfoDataModel *self)
{
}

static void info_data_model_finalize(GObject *obj)
{
	InfoDataModel *self = VALA_PANEL_INFO_DATA_MODEL(obj);
	g_sequence_free(self->seq);
}

static void info_data_model_class_init(InfoDataModelClass *klass)
{
	G_OBJECT_CLASS(klass)->finalize = info_data_model_finalize;
}

InfoDataModel *info_data_model_new()
{
	InfoDataModel *new_data =
	    VALA_PANEL_INFO_DATA_MODEL(g_object_new(info_data_model_get_type(), NULL));
	new_data->seq = g_sequence_new((GDestroyNotify)info_data_free);
	return new_data;
}

GSequence *info_data_model_get_sequence(InfoDataModel *model)
{
	return model->seq;
}
