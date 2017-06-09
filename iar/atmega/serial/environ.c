/* Copyright 2003-2010 IAR Systems AB. */

/*******************
 *
 * This is the default implementation of the "__environ" variable used
 * by the standard library.  It can be replaced by a system-specific
 * implementation.
 *
 * The variable "__environ" contains the environment for the default
 * implementation of the standard function "getenv".
 *
 * To define an enviroment set the variable "__environ" to a string
 * containing any number of "KEY=value" each terminated by a nil
 * ('\0') character.  Note that the string itself must be terminated
 * by an extra nil character.
 *
 * For example, the following definition of "__environ" specifies two
 * environment variables "COLOR" and "SPEED" with values "red" and
 * "500", respectively.
 *
 *    const char *__environ = "COLOR=red\0SPEED=500\0\0";
 *
 ********************/

#include <yfuns.h>

_DLIB_DATA_ATTR const char *__environ = "";
