/*
 * query.h  -- prototypes and definitions for query.c
 * Copyright (C) 2006-2025 Arturo Bianchi <the.craftsman@rinux.it>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * RCS ID: $Id: query.h,v 1.1 2006/04/13 11:03:53 IW0DER Exp root $
 *
 * $Log: query.h,v $
 * Revision 1.1  2006/04/13 11:03:53  IW0DER
 * fix so_open error, extract query code from main code,
 * use strtok for read more name from single line.
 *
 */

#ifndef _QUERY_H
#define _QUERY_H


/*
 * DEF
 */
#define SERVER			"das.eu"	/* default server */
#define PORT			"4343"		/* default port */

#define QUERYBUFSIZE	128
#define REPLYBUFSIZE	4096

#define DOMAINTAG		"Domain:"
#define STATUSTAG		"Status:"


/*
 * QUERY_OUT MODE
 */
#define QUERY_OUT_VIDEO	0
#define QUERY_OUT_CSV	1



/*
 * FUNCTION
 */
int normalize(char *);
int query(char *);
int query_debug(int);
int query_out(int);
int query_abort(void);
int query_init(void);


#endif		/* !_QUERY_H */



/*
 * End
 */