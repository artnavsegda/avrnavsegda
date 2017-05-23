/**************************************************
 *
 * The C++ runtime startup and destruction system. This is also used
 * by the C atexit function.
 *
 * Copyright 1998-2010, 2005 IAR Systems AB. 
 *
 **************************************************/

#define _DO_NOT_INLINE_MALLOC 1

#ifdef __cplusplus
  #error This file should be compiled in C mode
#endif

#include <stdlib.h>
#include <string.h>

#include "xcstartup.h"

#pragma language=extended


#if _DLIB_SUPPORT_FOR_TAABI

#include "cxxabi.h"

/*
 * Call each constructor for each global object defined in the
 * application. Typically, this is called from a target-specific
 * cstartup.
 */

_CPPINIT_ATTR void __call_ctors(__difunct_ptr start, __difunct_ptr end_plus_one)
{
  while (start != end_plus_one)
  {
#if _DLIB_TAABI_DIFUNC_CONTAINS_OFFSETS
    /* The field in the table is not the address, it is the offset
     * from the location to the function to call. */
    unsigned long content = * (unsigned long *)start;
    size_t location = (size_t) start;

    __func_ptr func = (__func_ptr)(content + location);
#else
    __func_ptr func = *start;
#endif

    (*func)();

    ++start;
  }
}


typedef struct __abi_dtor_rec
{
  void *         object;
  __func_ptr     dtor;
  void *         handle;
} __abi_dtor_rec;


/*
 * The linker will count number of calls to __cxa_atexit and creates
 * an array with those elements plus a number of extra in the block
 * DYNEXIT.
 */

#ifndef _DLIB_SECTION_ATTR
#define _DLIB_SECTION_ATTR
#endif

#pragma segment = ".iar.dynexit" _DLIB_SECTION_ATTR

#define ARRAY_START ((__abi_dtor_rec *)__segment_begin(".iar.dynexit"))
#define ARRAY_END   ((__abi_dtor_rec *)__segment_end  (".iar.dynexit"))

static __no_init __abi_dtor_rec DtorRecArray[1] @ ".iar.dynexit";

static __abi_dtor_rec * next_abi_dtor_rec = DtorRecArray;

static __abi_dtor_rec * __allocate_record(void)
{
  if (next_abi_dtor_rec >= ARRAY_END)
  {
    return 0;
  }

  {
    __abi_dtor_rec * this_dtor_rec = next_abi_dtor_rec;

    ++next_abi_dtor_rec;

    return this_dtor_rec;
  }
}


/*
 * Called for each dynamically initialized C++ object that must be
 * destroyed.
 */

extern void * __cstart_call_dtors;

#pragma required = __cstart_call_dtors
int __cxa_atexit(__func_ptr_voidptr func, void *obj, void *handle)
{
  /* The handle is any address in the DLL/DSO. As long at it's a
   * unique address for the DLL/DSO.
   */

#if _MULTI_THREAD
  __iar_Locksyslock_StaticGuard();
#endif

  /* allocate space for double indirection, as well */
  __abi_dtor_rec * rec = __allocate_record();

#if _MULTI_THREAD
  __iar_Unlocksyslock_StaticGuard();
#endif

  if (!rec)
  {
    return 1;               /* Non-zero for fail. */
  }

  rec->object = obj;
  rec->dtor = (__func_ptr)func;
  rec->handle = handle;

  return 0;
}


/* This is typically called from a target-specific cstartup. */
void __call_dtors(void)
{
  __cxa_finalize(0);
}


void __cxa_finalize(void * handle)
{
  /* Note: Currently we do not have support for DLL/DSO:s. */

  /*
   * Only needed to support DLL/DSOs
   *
   * This function should be called at the unloading of a DLL/DSO. It
   * should walk the destruction list for atexit destroying any object
   * that has been constructed by the DLL/DSO. Multiple calls with the
   * same handle should not result in objects destroyed several times.
   * When the main program calls exit, it must call any remaining
   * __cxa_atexit-registered functions, either by calling
   * __cxa_finalize(0), or by walking the registration list itself.
   *
   * The handle is any address in the DLL/DSO. As long at it's a
   * unique address for the DLL/DSO.
   */

  /*
   * We do this naively at the moment. Search the destructor list for
   * an element that should be destroyed, unlink that element from the
   * list, destroy it, free the element if applicable, redo the
   * search.
   *
   * One of the problems we have to deal with is that a destructor can
   * link in new elements at the start of the list.
   */

  __abi_dtor_rec * p = next_abi_dtor_rec;

  /* Note: next_abi_dtor_rec is zero before any elements has been
   * added. (See comment on static initialization of next_abi_dtor_rec
   * above.) */
  if (p == 0)
  {
    return;
  }


  /* Note, this loop does not move next_abi_dtor_rec, however it notices
   * if someone else has moved it. */

  __abi_dtor_rec * orig_end = next_abi_dtor_rec;

  while (1)
  {
    /*
     * Someone added a new element to the end -- start from the
     * beginning. (Note: the items that we've already handled is
     * skipped.
     */
    if (orig_end != next_abi_dtor_rec)
    {
      p = next_abi_dtor_rec;
      orig_end = next_abi_dtor_rec;
    }

    /*
     * Have we reached the end?
     */
    else if (p == ARRAY_START)
    {
      break;
    }

    /* This originally pointed to the next free location. */
    --p;

    __func_ptr dtor = p->dtor;

    if (dtor == 0)
    {
      continue;
    }

    /* Remove ourselves in case this is called again. */
    p->dtor = 0;


    if (p->object)
    {
      /* Call destructor. */
      ((__func_ptr_voidptr)dtor)(p->object);
    }
    else
    {
      /* Call atexit-registered function. */
      (dtor)();
    }
  }
}


/* This is needed since it's refered to by the compiler. */
void * __dso_handle;


#else /* !_DLIB_SUPPORT_FOR_TAABI */

#include "dtor_rec.h"

/*
 * Call each constructor for each global object defined in the
 * application.
 *
 * The constructors are placed in a segment named DIFUNCT. This
 * function is assumed to be called from the cstartup assembler
 * routine using code similar to:
 *      MOV   SFB DIFUNCT, <first argument register>
 *      MOV   SFE DIFUNCT, <second argument register>
 *
 * This allows some objects to be creates earlier than the rest. This
 * is performed by a two-pass loop, in the first pass only entries
 * preceded by a NULL is called. In the second all the others is
 * called.
 */

_CPPINIT_ATTR void __call_ctors(__difunct_ptr start0, __difunct_ptr end_plus_one)
{
  char pass; /* pass == 2 => first pass, pass == 1 => second pass */
  for (pass = 2; pass != 0; --pass)
  {
    __difunct_ptr start;
    for (start = start0; start != end_plus_one; ++start)
    {
      /* NULL marks that the next is to be run in the first pass only. */
      if (*start != 0 ? pass != 2 : (++start, pass == 2))
      {
        (*start)();
      }
    }
  }
}


/*
 * This is used by atexit to allocate a destruction record.
 */
int __alloc_record_needed_destruction(__func_ptr func, void *obj)
{
  /* allocate space for double indirection, as well */
  __dtor_rec * rec = malloc(sizeof(__dtor_rec) + sizeof(__dtor_rec const *));
  if (!rec)
  {
    return 1;               /* Non-zero for fail. */
  }

  __dtor_rec const * * pp = (__dtor_rec const * *) (rec + 1);
  /* Point to pointer */
  rec->next = pp;
  rec->object = obj;
  rec->dtor = func;
  /* Initialize pointer */
  *pp = 0;
  __record_needed_destruction( rec );

  return 0;
}


/*
 * Called for each static object that needs destruction. Also called
 * from __alloc_record_needed_destruction (originating from atexit).
 */

static __dtor_rec const * dtor_head /* = NULL */;

extern void * __cstart_call_dtors;

#pragma required = __cstart_call_dtors
void __record_needed_destruction( __dtor_rec const * rec )
{
  /*
   * Check if record already is linked in the list;
   */

  /* This is not a 100% perfect test, we do not detect if the last
   * element in the list is inserted a second time since it's
   * *rec->next pointer is zero. */
  if (*(rec->next) != NULL || rec == dtor_head)
  {
    abort();
  }

  /*
   * Link in the record at the beginning of the list.
   */
  *(rec->next) = dtor_head;
  dtor_head = rec;
}


/*
 * Runs destructors of dynamically initialized objects and calls
 * functions registered using atexit.
 */

void __call_dtors(void)
{
  __dtor_rec const * rec;

  while (dtor_head != NULL)
  {
    /*
     * First we unlink the record from the list so that we won't
     * call it recursively by mistake.  Also it is possible for the
     * value of dtor_head to change (e.g. if a new object is
     * created in a destructor).
     */
    rec = dtor_head;
    dtor_head = *(dtor_head->next);

    if (rec->object)
    {
      ((__func_ptr_voidptr)rec->dtor)(rec->object);
    }
    else
    {
      (rec->dtor)();
    }
  }
}


/*
 * Copies an entire array using a copy constructor for each array
 * element.
 *
 * The macro creates one function per memory.
 */

#define __VAR_MEM_HELPER1__(M, I)                                             \
void M##_vec_cctor(void M * dest, M##_size_t no, M##_size_t element_size,     \
                 M##_cctor_ptr cctor, void M * src);                          \
void M##_vec_cctor(void M * dest, M##_size_t no, M##_size_t element_size,     \
                 M##_cctor_ptr cctor, void M * src)                           \
{                                                                             \
  M##_size_t i;                                                               \
                                                                              \
  /* We must work with character pointers in order to perform                 \
   * pointer arithmetic operations on src and dest.                           \
   */                                                                         \
  char M * src_charp =  (char M *) src;                                       \
  char M * dest_charp = (char M *) dest;                                      \
                                                                              \
  if (cctor != NULL)                                                          \
  {                                                                           \
    for (i=0; i < no; i++)                                                    \
    {                                                                         \
      (*cctor)((void M *)dest_charp, (void M *)src_charp);                    \
                                                                              \
      src_charp += element_size;                                              \
      dest_charp += element_size;                                             \
    }                                                                         \
  }                                                                           \
}
__VAR_MEMORY_LIST1__()
#undef __VAR_MEM_HELPER1__

#endif /* _DLIB_SUPPORT_FOR_TAABI */
