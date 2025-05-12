/*
 * common.h  -- common prototypes and definitions 
 * Copyright (C) 2003-2025 Arturo Bianchi <the.craftsman@rinux.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * RCS ID: $Id: common.h,v 1.1 2006/04/08 10:57:04 IW0DER Exp $
 *
 * $Log: common.h,v $
 * Revision 1.1  2006/04/08 10:57:04  IW0DER
 * First version - DAS 1.0 compatible
 *
 */


#ifndef _COMMON_H
#define _COMMON_H



/*
 * RCS Id
 */
#ifdef USE_RCS
#define SOURCE_ID(id)		static const char rcsid[]  __attribute__((unused)) = id;
#define AUTHOR_ID(id)		static const char author[] __attribute__((unused)) = id;
#else
#define SOURCE_ID(id)		/* NONE */
#define AUTHOR_ID(id)		/* NONE */
#endif



/*
 * BOLEAN
 */
#ifndef FALSE
enum { FALSE = 0, TRUE = 1 };
#endif


/*
 * DEBUG
 */
#ifdef DEBUG
#define ERRPFIN(fmt, arg)	fprintf(stderr, "%s: ", __FILE__); fprintf(stderr, fmt, arg)
#define ERRPF(fmt, arg)		fprintf(stderr, fmt, arg)
#define ERRPFOUT(fmt, arg)	fprintf(stderr, fmt, arg);fprintf(stderr, "\n");
#else
#define ERRPFIN(fmt, arg)
#define ERRPF(fmt, arg)
#define ERRPFOUT(fmt, arg)
#endif

#endif



/*
 * End
 */