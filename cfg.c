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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"

int
cfgsetup(struct cfgfile *cfg, char *filename)
{
	/*
	 * sets up the cfgfile struct pointed to by cfg, using the cfg
	 * file location pointed to by filename
	 */
	 cfg->count = 0;
	cfg->file = strdup(filename);
	if (!cfg->file)
		return 0;
	FILE *fp;
	fp = fopen(cfg->file, "r");
	if (!fp)
		return 0;
	if (!readfile(cfg, fp))
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	//printlist(cfg->headnode);
	return 1;
}

void
cfgfree(struct cfgfile *cfg)
{
	/*
	 * frees all memory allocated by cfg
	 */
	freenodes(cfg->headnode);
	free(cfg->file);
	free(cfg);
}

int
cfggetvalue(struct cfgfile *cfg, char *key, char *buffer, size_t buffersize)
{
	/*
	 * stores the value from the option pointed to by key in buffer with
	 * a maximum size of buffersize
	 */
	if (!cfg->headnode)
		return 0;
	struct cfgnode *tmp = cfg->headnode;
	while (tmp)
	{
		if (strcmp(key, tmp->key) == 0)
		{
			strncpy(buffer, tmp->value, buffersize);
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

void
cfgappendchar(char *dest, char ch)
{
	/*
	 * appends char ch to dest
	 * assumes there is enough allocated memory
	 */
	int len = strlen(dest);
	dest[len] = ch;
	dest[len+1] = '\0';
}

int
readfile(struct cfgfile *cfg, FILE *fp)
{
	/*
	 * read file pointer to by fp parsing lines
	 * storing them in the cfgfile structure cfg
	 */
	cfg->headnode = createnode();
	if (!cfg->headnode)
		return 0;
	char ch;
	size_t bufsize = 30;
	size_t linesize = 0;
	char *line = NULL;
	char *buffer = malloc(bufsize);
	if (!buffer)
		return 0;
	while ((ch = getc(fp)) != EOF)
	{
		if (!(ch == '\n') && !(ch == '\r'))
		{
			if (linesize >= bufsize-1)
			{
				bufsize += 10;
				buffer = realloc(buffer, bufsize);
				if (!buffer)
					return 0;
			}
			cfgappendchar(buffer, ch);
			linesize += sizeof(char);
		}
		else
		{
			free(line);
			line = strdup(buffer);
			if (!line)
			{
				free(buffer);
				return 0;
			}
			if (!parseline(line, cfg))
			{
				free(line);
				free(buffer);
				return 0;
			}
			buffer[0] = '\0';
			/* I could free buffer here, though I don't
			 * see a point, we already have the memory
			 * allocated, may as well keep it */
			linesize = 0;
		}
	}
	free(line);
	free(buffer);
	return 1;
}

int
parseline(char *line, struct cfgfile *cfg)
{
	/* parse line pointed to by line string
	 * results in a cfgnode node appended to head */
	
	/* determine if the line is a comment (#)
	 * also strip whitespaces from the beginning */
	char ch = line[0];
	while (ch == ' ' || ch == '\t')
	{
		memmove(line, line+1, strlen(line));
		ch = line[0];
	}
	if (ch == '#')
		return 1;
	else
	{
		// TODO: strip whitespaces from the key and value
		// while key[strlen(key)-1 == ' ']
		struct cfgnode *tmp;
		if (cfg->count == 0)
			tmp = cfg->headnode;
		else
		{
			tmp = createnode();
			if (!tmp)
				return 0;
			if (!addnode(cfg->headnode, tmp))
				return 0;
		}
		// malloc memory for key and value
		int c = 0;
		int doingkey = 1;
		size_t keybufsize = 1;
		size_t keysize = 255;
		size_t valbufsize = 255;
		size_t valsize = 0;
		tmp->key = malloc(keybufsize);
		tmp->value = malloc(valbufsize);
		if (!tmp->key)
			return 0;
		while ((ch = line[c]) != '\0')
		{
			if (doingkey)
			{
				if (ch == '=')
					doingkey = 0;
				else 
				{
					if (keysize >= keybufsize-1)
					{
						keybufsize += 10;
						tmp->key = realloc(tmp->key, keybufsize);
						if (!tmp->key)
							return 0;
					}
					cfgappendchar(tmp->key, ch);
					valsize += sizeof(char);
				}
			}
			else
			{
				if (valsize >= valbufsize-1)
				{
					valbufsize += 10;
					tmp->value = realloc(tmp->value, valbufsize);
					if (!tmp->value)
						return 0;
				}
				cfgappendchar(tmp->value, ch);
				valsize += sizeof(char);
			}
			c++;
		}
		cfg->count++;
	}
	return 1;
}

struct cfgnode
*createnode(void)
{
	/*
	 * create a cfgnode and return a pointer to it
	 */
	struct cfgnode *tmp = malloc(sizeof(struct cfgnode));
	if (!tmp)
		return NULL;
	tmp->next = NULL;
	tmp->key = NULL;
	tmp->value = NULL;
	return tmp;
}

int
addnode(struct cfgnode *head, struct cfgnode *node)
{
	/*
	 * add a cfgnode to the end head
	 */
	if (!head || !node)
		return 0;
	struct cfgnode *tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return 1;
}

void
freenodes(struct cfgnode *head)
{
	/*
	 * free all cfgnode in the linked list
	 */
	if (!head)
		return;
	struct cfgnode *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void
printlist(struct cfgnode *head)
{
	struct cfgnode *tmp = head;
	while (tmp)
	{
		printf("key: %s\nvalue: %s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}
