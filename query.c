/* 
 * query.c -- DAS server query
 * Copyright 2006-2025 by Arturo Bianchi <the.craftsman@rinux.it>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * RCS ID: $Id: query.c,v 1.1 2006/04/13 11:03:53 IW0DER Exp root $
 *
 * $Log: query.c,v $
 * Revision 1.1  2006/04/13 11:03:53  IW0DER
 * fix so_open error, extract query code from main code,
 * use strtok for read more name from single line.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>


#include "whenmake.h"

#include "common.h"
#include "query.h"


SOURCE_ID("$Id: query.c,v 1.1 2006/04/13 11:03:53 IW0DER Exp root $")


int csv = 0, debug = 0, head = 1, fd = -1;


/*
 * so_open
 *
 */
static int so_open(char *host, char *port)
{
	struct hostent *he;
	struct servent *se;
	struct sockaddr_in *addr;
	int status = 0;

	errno = 0;
	if ((he = gethostbyname(host))) {
		se = getservbyname(port, "tcp");
	
		if ((addr = malloc(sizeof(struct sockaddr_in))) != NULL) {
			memset((char *)addr, 0, sizeof(struct sockaddr_in));

			memcpy((char *)&(addr->sin_addr.s_addr), he->h_addr, sizeof(struct in_addr));
			addr->sin_port = ((se) ? se->s_port : htons(atoi(port)));
			addr->sin_family = AF_INET;

			if ((fd = socket(PF_INET, SOCK_STREAM, 0)) > -1) {
				if (connect(fd, (struct sockaddr *)addr, sizeof(struct sockaddr_in)))
					status = -4;
			} else
				status = -3;

			free(addr);
		}
		else
			status = -2;
        }
	else
		status = -1;

	if (debug) {
		assert(host);
		assert(port);

		fprintf(stderr, "so_open(%s, %s): status=%d fd=%d errno=%d, %s\n", host, port, status, fd, errno, strerror(errno));
	}

	return(status);	
}



/*
 * so_close
 *
 */
static int so_close(void)
{
	if (debug)
		fprintf(stderr, "so_close(void): fd=%d\n", fd);

	if (fd > -1) {
		close(fd);
		fd = -1;
	}

	return(fd);
}



/*
 * qo
 */
static void qo(char *dom, char *sta)
{
	if (head) {
		if (csv)
	  		fprintf(stdout, "Domain;Status;\n");
		else {
			fprintf(stdout, "\n\n");
			fprintf(stdout, "%-48s %-20s\n", "Domain", "Status");
			fprintf(stdout, "%-48s %-20s\n", "--------------------------------------", "------------------------");
		}
		head = 0;
	}
	if (csv)
		fprintf(stdout, "%s;%s;\n", dom, sta);
	else
		fprintf(stdout, "%-48s %-20s\n", dom, sta);
}



/*
 * remove_crlf
 */
static void remove_line_ending(char *s) {
	char *p = s + strcspn(s, "\r\n");

	if (*p)
		*p = '\0';
}


 
/*
 * normalize
 */
int normalize(char *name)
{
	size_t	l;
	char	*p;

	assert(name);

	if ((p = strstr(name, ".eu")) && p[3] == '\0')
		*p = '\0';

	if  ((p = strrchr(name, '.'))) {
		memmove(name, p + 1, strlen(p + 1) + 1); 
	}
	
	while ((p = index(name, '.')))
		name = p + 1;

	for (p = name; *p; p++) {
		if (isalpha(*p))
			*p = tolower(*p);

		if (! (isalnum(*p) || *p == '-')) {
			*p = '\0';
			break;
		}
	}
	
	l = strlen(name);

	return(l);
}



/*
 * query
 *
 */
int query(char *query) 
{
	int status = 0;
	char buf[QUERYBUFSIZE];

	if (query) {
		if (!(status = so_open(SERVER, PORT))) {
			int len = snprintf(buf, sizeof(buf), "%s\n", query);

			if (debug)
				fprintf(stderr, "--- SEND:\n%s\n", buf);

			if (send(fd, buf, len, 0) == len) {
				char reply[REPLYBUFSIZE];

				if ((len = recv(fd, reply, sizeof(reply), MSG_WAITALL)) > 0) {
					char *dom, *sta;

					reply[len] = 0;

					if (debug)
						fprintf(stderr, "--- RECV:\n%s\n", reply);

					if ((dom = strstr(reply, DOMAINTAG)))
						dom += sizeof(DOMAINTAG);

					if ((sta = strstr(reply, STATUSTAG)))
						sta += sizeof(STATUSTAG);

					assert(dom);
					assert(sta);

					remove_line_ending(dom);
					remove_line_ending(sta);

					qo(dom, sta);
				}
			}
		}
		so_close();
	}

	return(status);
}



/*
 * query_debug
 */
int query_debug(int d)
{
	int old = debug;

	debug = d;

	return(old);
}



/*
 * query_out
 */
int query_out(int c)
{
	int old = csv;

	csv = c;

	return(old);
}



/*
 * query_abort
 */
int query_abort(void)
{
	if (!head)
		fprintf(stdout, "\n***abort\n\n");

	so_close();

	return(TRUE);
}



/*
 * query_init
 */
int query_init(void)
{
	head = 1;
	return(TRUE);
}



/*
 * End
 */