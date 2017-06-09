/*******************
 *
 * Copyright 1998-2010 IAR Systems AB. 
 *
 * This is the default implementation of the "__exit" function of the
 * standard library.  Note that a debug version of this function is
 * used when the application is build in debug mode.
 *
 * The purpose of the __exit is to halt the system.
 *
 ********************/

#include <stdlib.h>
#include <yfuns.h>

_STD_BEGIN

#pragma module_name = "?__exit"

void __exit(int x)
{
  while (1)
  {
  }
}

_STD_END
