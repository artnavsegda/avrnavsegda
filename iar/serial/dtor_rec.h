/* system termination destruction functions */
/* Copyright 1998-2010, 2005 IAR Systems AB.  */

/**************************************************
 *
 * List of items that must be called at system termination. Could be
 * desturctors of dynamically initalized C++ objects or functions
 * passed to the C atexit funciton.
 *
 **************************************************/

#ifndef _DTOR_REC_H
#define _DTOR_REC_H

#include <yvals.h>

#if _DLIB_SUPPORT_FOR_TAABI
#error "dtor_rec.h should not be included when buidling a taabi library."
#endif

#include "xcstartup.h"

#pragma language=save
#pragma language=extended

/* Define function pointer types for each memory. */
#define __VAR_MEM_HELPER1__(M, I)                               \
typedef FUNCATTR void (* M##_dtor_ptr) (void M *);              \
typedef FUNCATTR void (* M##_ctor_ptr) (void M *);              \
typedef FUNCATTR void (* M##_cctor_ptr)(void M *, void M *);
__VAR_MEMORY_LIST1__()
#undef __VAR_MEM_HELPER1__

#if !_DLIB_SUPPORT_FOR_TAABI


/*
 * An element in the list of items to call at system termination. This
 * includes destructor of dynamically initialized static objects and
 * functions registerd using the C function "atexit".
 *
 * For C++ objects, this is placed in ROM. This is possible since the
 * "next" pointer uses double indirection, i.e. it points to a fixed
 * location in RAM that can be modified when this item is linked into
 * the list.
 */

typedef struct __dtor_rec __dtor_rec;
struct __dtor_rec
{
  __dtor_rec const * * next;
  void *               object;
  __func_ptr           dtor;
};

#endif /* !_DLIB_SUPPORT_FOR_TAABI */


#pragma language=restore

_EXTERN_C

void __record_needed_destruction( __dtor_rec const * );
int __alloc_record_needed_destruction(__func_ptr, void *);

#pragma object_attribute = __noreturn
void __pure_virtual_called(void);

_END_EXTERN_C


#endif /*_DTOR_REC_H */
