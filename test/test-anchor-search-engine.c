#include <libanchor-private/anchor-search-provider.h>
#include <libanchor-private/anchor-search-engine.h>
#include <gtk/gtk.h>

static void
hits_added_cb (AnchorSearchEngine *engine, GSList *hits)
{      
	g_print ("hits added\n");
	while (hits) {
		g_print (" - %s\n", (char *)hits->data);
		hits = hits->next;
	}
}

static void
finished_cb (AnchorSearchEngine *engine)
{
	g_print ("finished!\n");
	gtk_main_quit ();
}

int 
main (int argc, char* argv[])
{
	AnchorSearchEngine *engine;
        AnchorSearchEngineModel *model;
        AnchorDirectory *directory;
	AnchorQuery *query;
        GFile *location;
	
	gtk_init (&argc, &argv);

	engine = anchor_search_engine_new ();
	g_signal_connect (engine, "hits-added", 
			  G_CALLBACK (hits_added_cb), NULL);
	g_signal_connect (engine, "finished", 
			  G_CALLBACK (finished_cb), NULL);

	query = anchor_query_new ();
	anchor_query_set_text (query, "richard hult");
	anchor_search_provider_set_query (ANCHOR_SEARCH_PROVIDER (engine), query);
	g_object_unref (query);

        location = g_file_new_for_path (g_get_home_dir ());
        directory = anchor_directory_get (location);
        g_object_unref (location);

        model = anchor_search_engine_get_model_provider (engine);
        anchor_search_engine_model_set_model (model, directory);
        g_object_unref (directory);

	anchor_search_provider_start (ANCHOR_SEARCH_PROVIDER (engine));
	anchor_search_provider_stop (ANCHOR_SEARCH_PROVIDER (engine));
        g_object_unref (engine);

	gtk_main ();
	return 0;
}
