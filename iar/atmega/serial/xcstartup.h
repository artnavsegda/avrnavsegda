/**************************************************
 *
 * Interface between the DLib library and a target-specific cstartup.
 *
 * Copyright 2006-2010 IAR Systems AB. 
 *
 **************************************************/

#ifndef XCSTARTUP_H
#define XCSTARTUP_H

#include <yvals.h>
#include <DLib_Defaults.h>

#pragma language=save
#pragma language=extended

#ifndef FUNCATTR
#define FUNCATTR
#endif

#ifndef _CPPINIT_ATTR
#define _CPPINIT_ATTR
#endif

typedef FUNCATTR void (* __func_ptr) (void);
typedef FUNCATTR void (* __func_ptr_voidptr) (void *);


/* Define _DIFUNCT_MEMORY if the DIFUNCT segment is not located in
   default memory. */
#ifdef _DIFUNCT_MEMORY
  typedef __func_ptr _DIFUNCT_MEMORY * __difunct_ptr;
#else /* !_DIFUNCT_MEMORY */
  typedef __func_ptr * __difunct_ptr;
#endif /* _DIFUNCT_MEMORY */


_EXTERN_C

_CPPINIT_ATTR void __call_ctors(__difunct_ptr, __difunct_ptr);
void __call_dtors(void);

_END_EXTERN_C


#pragma language=restore

#endif /* XCSTARTUP_H */
