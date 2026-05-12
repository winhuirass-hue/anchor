

#ifndef __NAUTILUS_RECENT_H__
#define __NAUTILUS_RECENT_H__

#include <gtk/gtk.h>
#include <libanchor-private/anchor-file.h>
#include <gio/gio.h>

void anchor_recent_add_file (AnchorFile *file,
			       GAppInfo *application);

#endif
