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
#include <errno.h>

#include "util.h"
#include "replace.h"
#include "cfg.h"


int
main(void)
{
	struct cfgfile *cfg = malloc(sizeof(struct cfgfile));
	if (!cfgsetup(cfg, "settings.cfg"))
	{
		perror("failed to create cfg structure");
		cfgfree(cfg);
		exit(1);
	}
	char test[256];
	cfggetvalue(cfg, "blogtitle", test, sizeof(test));
	puts(test);
	char val[3];
	cfggetvalue(cfg, "postsperpage", val, sizeof(val));
	printf("%d\n", atoi(val)*2);
	cfgfree(cfg);

	char *content = readfilecontent("templates/template.txt");
	if (!content)
		perror("unable to read file");
	char *replaced = replaceinmemory(content, "{CONTENT}", "test");
	puts(replaced);
	free(content);
	free(replaced);
	return 0;
}
