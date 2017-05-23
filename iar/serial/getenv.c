/* Copyright 2005-2010 IAR Systems AB. */

/*******************
 * This is the default implementation of the "getenv" function of the
 * standard library.  It can be replaced with a system-specific
 * implementation.  (Note: Typically the global variable __environ
 * should be used to specify the environment used.  This function
 * should only be replaced if that is not sufficient.)
 *
 * The "getenv" function is called with name of an environment
 * variable and should return a pointer to a string representing the
 * value of that variable, or 0.  An environment variable is used on
 * many host system to specify system or user information.  The
 * standard library does not rely on the presence of any environment
 * variables.
 *
 ********************/

#include <stdlib.h>
#include <string.h>
#include "yfuns.h"
_STD_BEGIN

#pragma module_name = "?getenv"

char *(getenv)(const char *name)
	{	/* search environment list for named entry */
	const char *s;
	size_t n = strlen(name);

	for (s = __environ; *s; s += strlen(s) + 1)
		{	/* look for name match */
		if (!strncmp(s, name, n) && s[n] == '=')
			return ((char *)&s[n + 1]);
		}
	return (0);
	}
_STD_END

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
