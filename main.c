/* This file is part of herzi's playground
 *
 * AUTHORS
 *     Sven Herzberg  <herzi@gnome-de.org>
 *
 * Copyright (C) 2008  Sven Herzberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include <glib.h>

int
main (int argc, char** argv)
{
	GError* error = NULL;
	gchar* out = NULL;

	gboolean  show_all = FALSE;
	gchar   **commits  = NULL;
	gchar   **commit;
	GString* command_line;
	GHashTable *revset = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTableIter iter;
    char *key, *value;

	GOptionEntry entries[] = {
		{"all", '\0', 0, G_OPTION_ARG_NONE, &show_all,
		 "Pretend as if all the refs in $GIT_DIR/refs/ are listed on "
		 "the command line as <commit>. (Default)", NULL},
		{G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_STRING_ARRAY, &commits,
		 "Commits to include in the graph."},
		{NULL}
	};

	GOptionContext* context = g_option_context_new ("COMMITS");
	g_option_context_add_main_entries (context, entries, NULL);
	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		if (error) {
			g_printerr ("%s\n", error->message);
			g_error_free (error);
		}
		g_option_context_free (context);
		return 1;
	}
	g_option_context_free (context);

	if (!commits && !show_all) {
		/* no commit given, show all */
		show_all = TRUE;
	}

	command_line = g_string_new ("git rev-list --parents");
	if (show_all) {
		g_string_append (command_line, " --all");
	}
	for (commit = commits; commit && *commit; commit++) {
		// FIXME: pretty-print the tip commits in the tree
		gchar* escaped = g_shell_quote (*commit);
		g_string_append_printf (command_line, " %s", escaped);
		g_free (escaped);
	}
	g_strfreev (commits);

	if (!g_spawn_command_line_sync (command_line->str, &out, NULL, NULL, &error)) {
		g_string_free (command_line, TRUE);
		if (error) {
			g_printerr ("%s", error->message);
			g_error_free (error);
		}
		return 2;
	}
	g_string_free (command_line, TRUE);

	gchar**lines = g_strsplit (out, "\n", 0);
	gchar**line;
	g_free (out);

	// FIXME: detect git-dir and provide a nice graph name
	g_print ("digraph {\n");

	for (line = lines; line && *line && **line; line++) {
		gchar** revs = g_strsplit (*line, " ", 0);
		gchar**parent;
		g_free (*line);
		*line = NULL;

		for (parent = revs+1; parent && *parent; parent++) {
			g_print ("r%s -> r%s;\n",
				 *parent,
				 revs[0]);
            g_hash_table_insert(revset, g_strdup(*parent), NULL);
		}
        g_hash_table_insert(revset, g_strdup(revs[0]), NULL);

		g_strfreev (revs);
	}

    // print all of the nodes
    g_hash_table_iter_init(&iter, revset);
    while (g_hash_table_iter_next (&iter, (void **)&key, (void **)&value)) {
        gchar *command = g_strdup_printf("git log -1 --pretty=%%s %s", key);

        if (!g_spawn_command_line_sync (command, &out, NULL, NULL, &error)) {
            g_string_free (command_line, TRUE);
            if (error) {
                g_printerr ("%s", error->message);
                g_error_free (error);
            }
            return 2;
        }
        g_strchomp(out);
        g_print("r%s [label=\"%.8s\", labeltooltip=\"%s\"];\n", key, key, out);
    }

	g_print ("}\n");

	g_free (lines);
	return 0;
}

