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
 #include "util.h"
 #include "replace.h"

char *
readfilecontent(char *file)
{
	/* read file and return pointer to string containing the content */
	FILE *fp = fopen(file, "r");
	if (!fp)
		return NULL;
	size_t destsize = getfilelength(fp);
	char *dest = malloc(destsize+1);
	if (!dest)
		goto skipfread;
	fread(dest, destsize, 1, fp);
skipfread:
	fclose(fp);
	return dest;
}
