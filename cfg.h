/*
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

#ifndef CFG_H
#define CFG_H

struct
cfgfile
{
	char *file;
	struct cfgnode *headnode;
	int count;
};

struct
cfgnode
{
	char *key;
	char *value;
	struct cfgnode *next;
};

int
cfgsetup(struct cfgfile *cfg, char *filename);

void
cfgfree(struct cfgfile *cfg);

int
cfggetvalue(struct cfgfile *cfg, char *key, char *buffer, size_t buffersize);

void
cfgappendchar(char *dest, char ch);

int
readfile(struct cfgfile *cfg, FILE *fp);

int
parseline(char *line, struct cfgfile *cfg);

struct cfgnode
*createnode(void);

int
addnode(struct cfgnode *head, struct cfgnode *node);

void
freenodes(struct cfgnode *head);

void
printlist(struct cfgnode *head);

#endif
