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
	if (!g_spawn_command_line_sync ("git-rev-list --all --parents", &out, NULL, NULL, &error)) {
		if (error) {
			g_printerr ("%s", error->message);
		}
		return 1;
	}

	gchar**lines = g_strsplit (out, "\n", 0);
	g_free (out);

	g_strfreev (lines);
	return 0;
}

