/* 
 * eu.c -- .eu DAS client
 * Copyright 2006-2025 by Arturo Bianchi <the.craftsman@rinux.it>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * RCS ID: $Id: eu.c,v 1.2 2006/04/13 11:03:53 IW0DER Exp root $
 *
 * $Log: eu.c,v $
 * Revision 1.2  2006/04/13 11:03:53  IW0DER
 * fix so_open error, extract query code from main code,
 * use strtok for read more name from single line.
 *
 * Revision 1.1  2006/04/08 10:57:04  IW0DER
 * First version - DAS 1.0 compatible
 *
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "whenmake.h"

#include "common.h"
#include "eu.h"
#include "query.h"
#include "unique.h"


SOURCE_ID("$Id: eu.c,v 1.2 2006/04/13 11:03:53 IW0DER Exp root $")



/*
 * sig_handler
 *
 */
void sig_handler(int signum)
{
	query_abort();
	fprintf(stderr, "Interrupted by signal %d: %s.\n\n", signum, strsignal(signum));
	exit(1);
}



/*
 * usage
 *
 */
static void usage(void)
{
	fprintf(stderr, "\nUsage: eu [OPTION]... [DOMAIN]...\n"
		"\nOptions:\n"
		"  -c           Output in csv format\n"
		"  -d           Debug messages to stderr\n"
		"  -f FILE      Input domain from file ascii\n"
		"  -h           Print this help screen\n"
		"  -u           Query unique names\n"
		"  -v           Print version number\n"
		"\n");
	exit(1);
}



/*
 * release
 *
 */
static void release(void)
{
	fprintf(stderr, "\nEU DAS client ver %s, build at %s.\n"
		"Please, report bugs to <bugs@rinux.it>.\n\n", version, compiled);
	exit(1);
}



/*
 * MAIN
 *
 */
int main(int argc, char *argv[]) {
	int j, ch, nopar = 0, unique = 0;
	char *list = NULL;

	while ((ch = getopt(argc, argv, "cdf:huv")) > 0) {
		switch (ch) {
			case 'c':
				query_out(QUERY_OUT_CSV);
				break;

			case 'd':
				query_debug(TRUE);
				break;

			case 'f':
				if (!(list = optarg))
					usage();
				nopar = 1;
				unique = 1;		/* revision 1.3: Force UNIQUE */
				break;

			case 'u':
				unique = 1;
				break;

			case 'v':
				release();
				break;

			default:
				usage();
		}
	}

	argc -= optind;
	argv += optind;
	if (argc == 0 && !nopar)
		usage();

	for (j = 1; j < 32; j++) {
		if (j != 9 && j != 17 && j != 19 && signal(j, sig_handler) == SIG_ERR)
			fprintf(stderr, "unable to sig_handler signal %d", j);
        }

	query_init();
	if (!nopar) {
		while (argc > 0) {
			char *p = argv[0];

			if (normalize(p) > 2) {
				if (unique) 
					assert(u_add(p));
				else
					query(p);
			}
			argc--;
			argv++;
		}
	}
	else
		if (list) {
			FILE *fp;
			char buf[QUERYBUFSIZE];

			if ((fp = fopen(list, "r"))) {

				while (fgets(buf, sizeof(buf), fp)) {
					char *p = buf, *t;

					while ((t = strtok(p, "\t\n "))) {
						if (normalize(t) > 2) {
							if (unique)
								assert(u_add(t));
							else
								query(t);
						}
						p = NULL;
					}
				}
				fclose(fp);
			}
		}

	if (unique) {
		u_cmd(query);
		u_free();
	}

	return(0);
}



/*
 * End
 */