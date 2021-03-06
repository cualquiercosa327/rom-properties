/***************************************************************************
 * ROM Properties Page shell extension. (XFCE)                             *
 * rom-properties-plugin.c: ThunarX Plugin Definition.                     *
 *                                                                         *
 * Copyright (c) 2017-2019 by David Korth.                                 *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#include "rom-properties-provider.hpp"
#include "rom-properties-page.hpp"

// GLib on non-Windows platforms defines G_MODULE_EXPORT to a no-op.
// This doesn't work when we use symbol visibility settings.
#if !defined(_WIN32) && (defined(__GNUC__) && __GNUC__ >= 4)
#ifdef G_MODULE_EXPORT
#undef G_MODULE_EXPORT
#endif
#define G_MODULE_EXPORT __attribute__ ((visibility ("default")))
#endif /* !_WIN32 && __GNUC__ >= 4 */

G_MODULE_EXPORT void thunar_extension_initialize	(ThunarxProviderPlugin *plugin);
G_MODULE_EXPORT void thunar_extension_shutdown		(void);
G_MODULE_EXPORT void thunar_extension_list_types	(const GType	**types,
							 gint		 *n_types);

static GType type_list[1];

G_MODULE_EXPORT void
thunar_extension_initialize(ThunarxProviderPlugin *plugin)
{
	if (getuid() == 0 || geteuid() == 0) {
		g_critical("*** " G_LOG_DOMAIN " does not support running as root.");
		return;
	}

	/* verify that the thunarx versions are compatible */
	const gchar *mismatch = thunarx_check_version(THUNARX_MAJOR_VERSION, THUNARX_MINOR_VERSION, THUNARX_MICRO_VERSION);
	if (G_UNLIKELY(mismatch != NULL)) {
		g_warning ("Version mismatch: %s", mismatch);
		return;
	}

#ifdef G_ENABLE_DEBUG
	g_message("Initializing " G_LOG_DOMAIN " extension");
#endif

	/* Register the types provided by this plugin */
	rom_properties_provider_register_type(plugin);
	rom_properties_page_register_type(plugin);

	/* Setup the plugin provider type list */
	type_list[0] = TYPE_ROM_PROPERTIES_PROVIDER;
}

G_MODULE_EXPORT void
thunar_extension_shutdown(void)
{
#ifdef G_ENABLE_DEBUG
	g_message("Shutting down " G_LOG_DOMAIN " extension");
#endif
}

G_MODULE_EXPORT void
thunar_extension_list_types(const GType	**types,
			    gint	 *n_types)
{
	*types = type_list;
	*n_types = G_N_ELEMENTS(type_list);
}
