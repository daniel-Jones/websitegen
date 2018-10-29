/*
 * This file is part of websitegen, a website generator for https://danieljon.es
 * Copyright (C) 2018 Daniel Jones daniel@danieljon.es
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"
#include "replace.h"
#include "cfg.h"

struct
settings
{
	/*
	 * the values here are retrieved from the config file
	 * the default sizes are sane for my purposes
	 */

	/* index */
	char	indextitle[30];		/* title of the index */
	char 	indexsrc[30];		/* index source file */

	/* blog */
	char 	blogtitle[30];		/* title of blog pages */
	char	blogheader[256];	/* header for each blog page */
	char	blogindir[30]; 		/* directory under ./input/ that contains the blog posts */
	char	blogoutdir[30];		/* directory under ./output/ that will hold final blog pages */
	int 	blogpostsperpage;	/* number of blog posts per page */
};

/* definitions */
void
retrievevalues(struct cfgfile *cfg, struct settings *info);

int
main(void);

int
main(void)
{
	/* setup the cfg parser */
	struct cfgfile *cfg = malloc(sizeof(struct cfgfile));
	if (!cfgsetup(cfg, "settings.cfg"))
	{
		perror("failed to create cfg structure");
		cfgfree(cfg);
		exit(1);
	}

	/* create settings struct and fill it */
	struct settings info = {0};
	retrievevalues(cfg, &info);
	puts(info.indexsrc);
	cfgfree(cfg);
	return 0;
}

void
retrievevalues(struct cfgfile *cfg, struct settings *info)
{
	/* index */
	cfggetvalue(cfg, "indextitle", info->indextitle, sizeof(info->indextitle));
	cfggetvalue(cfg, "indexsrc", info->indexsrc, sizeof(info->indexsrc));

	/* blog */
}
