/*
 * unique.h  -- prototypes and definitions for unique.c
 * Copyright (C) 2006-2025 Arturo Bianchi <the.craftsman@rinux.it>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * RCS ID: $Id: $
 *
 * $Log: $
 */
#ifndef _UNIQUE_H
#define _UNIQUE_H


/*
 * DEF
 */


/*
 * VAR
 */
typedef struct uni_s uni_t;

struct uni_s {
	char	*name;
	size_t	len;
	int	count;
	int	flags;
	uni_t	*next;
};



/*
 * FUNCTION
 */
void	u_free(void);
int		u_add(char *);
int		u_cmd(int (*)(char *));



#endif		/* !_UNIQUE_H */



/*
 * End
 */