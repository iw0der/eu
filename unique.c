/* 
 * unique.c -- unique word list functions
 * Copyright 2006-2025 by Arturo Bianchi <the.craftsman@rinux.it>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * RCS ID: $Id: $
 *
 * $Log: $
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>


#include "whenmake.h"
#include "common.h"
#include "unique.h"


SOURCE_ID("$Id: $")


static uni_t		*uni_r = NULL;

static unsigned long	urc = 0;


/*
 * U_FREE_R
 */
static void u_free_r(uni_t *u)
{
	if (u) {
		u_free_r(u->next);
		free(u->name);
		free(u);
		urc--;
	}
}



/*
 * U_FREE
 */
void u_free(void)
{
	u_free_r(uni_r);
	uni_r = NULL;

	assert(!urc);
}



/*
 * U_ALLOC
 */
static uni_t *u_alloc(char *name)
{
	uni_t	*t;

	assert(name);
	if ((t = calloc(sizeof(uni_t), 1))) {
		t->name  = strdup(name);
		t->len   = strlen(name);
		t->count = 1;
		urc++;
	}
	return(t);
}



/*
 * U_ADD
 */
int u_add(char *name)
{
	size_t	nl;
	uni_t	*l = NULL, *t = uni_r;

	assert(name);
	nl = strlen(name);

	while (t && (t->len != nl || strcmp(t->name, name))) {
		l = t; t = l->next;
	}

	if (t)
		t->count++;
	else {
		if ((t = u_alloc(name))) {
			if (l)
				l->next = t;
			else
				uni_r = t;
		}
	}

#ifdef DEBUG
	fprintf(stderr, "name: %s, count: %d\n", name, (t ? t->count : -1));
#endif

	return((t ? t->count : 0));
}



/*
 * U_CMD
 */
int u_cmd(int (*f)(char *))
{
	int	count = 0;
	uni_t	*t = uni_r;

	while (t) {
		if (f(t->name))
			break;

		t = t->next;
		count++;
	}

	return(count);
}



/*
 * End
 */