/****************************************************************************
 *                                                                          *
 *                         GNAT RUN-TIME COMPONENTS                         *
 *                                                                          *
 *                            T R A C E B A C K                             *
 *                                                                          *
 *                          C Implementation File                           *
 *                                                                          *
 *            Copyright (C) 2000-2023, Free Software Foundation, Inc.       *
 *                                                                          *
 * GNAT is free software;  you can  redistribute it  and/or modify it under *
 * terms of the  GNU General Public License as published  by the Free Soft- *
 * ware  Foundation;  either version 3,  or (at your option) any later ver- *
 * sion.  GNAT is distributed in the hope that it will be useful, but WITH- *
 * OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.                                     *
 *                                                                          *
 * As a special exception under Section 7 of GPL version 3, you are granted *
 * additional permissions described in the GCC Runtime Library Exception,   *
 * version 3.1, as published by the Free Software Foundation.               *
 *                                                                          *
 * You should have received a copy of the GNU General Public License and    *
 * a copy of the GCC Runtime Library Exception along with this program;     *
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    *
 * <http://www.gnu.org/licenses/>.                                          *
 *                                                                          *
 * GNAT was originally developed  by the GNAT team at  New York University. *
 * Extensive contributions were provided by Ada Core Technologies Inc.      *
 *                                                                          *
 ****************************************************************************/

/* This file contains low level support for stack unwinding using GCC intrinsic
   functions.
   It has been tested on the following configurations:
   PowerPC/AiX
   PowerPC/Darwin
   PowerPC/VxWorks
   PowerPC/LynxOS-178
   SPARC/Solaris
   i386/GNU/Linux
   i386/Solaris
   i386/NT
   i386/OS2
   i386/LynxOS
   Alpha/VxWorks
   Alpha/VMS
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

extern int __gnat_backtrace (void **, int, void *, void *, int);

/* The point is to provide an implementation of the __gnat_backtrace function
   above, called by the default implementation of the System.Traceback package.

   We first have a series of target specific implementations, each included
   from a separate C file for readability purposes.

   Then come two flavors of a generic implementation: one relying on static
   assumptions about the frame layout, and the other one using the GCC EH
   infrastructure.  The former uses a whole set of macros and structures which
   may be tailored on a per target basis, and is activated as soon as
   USE_GENERIC_UNWINDER is defined.  The latter uses a small subset of the
   macro definitions and is activated when USE_GCC_UNWINDER is defined. It is
   only available post GCC 3.3.

   Finally, there is a default dummy implementation, necessary to make the
   linker happy on platforms where the feature is not supported, but where the
   function is still referenced by the default System.Traceback.  */

#define Lock_Task system__soft_links__lock_task
extern void (*Lock_Task) (void);

#define Unlock_Task system__soft_links__unlock_task
extern void (*Unlock_Task) (void);

/*-------------------------------------*
 *-- Target specific implementations --*
 *-------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define IS_BAD_PTR(ptr) (IsBadCodePtr((FARPROC)ptr))

int
__gnat_backtrace (void **array,
                  int size,
                  void *exclude_min,
                  void *exclude_max,
                  int skip_frames)
{
  CONTEXT context;
  UNWIND_HISTORY_TABLE history;
  int i;

  /* Get the context.  */
  RtlCaptureContext (&context);

  /* Setup unwind history table (a cached to speed-up unwinding).  */
  memset (&history, 0, sizeof (history));

  i = 0;
  while (1)
    {
      PRUNTIME_FUNCTION RuntimeFunction;
      KNONVOLATILE_CONTEXT_POINTERS NvContext;
      ULONG64 ImageBase;
      VOID *HandlerData;
      ULONG64 EstablisherFrame;

      /* Get function metadata.  */
      RuntimeFunction = RtlLookupFunctionEntry
	(context.Rip, &ImageBase, &history);

      if (!RuntimeFunction)
	{
	  /* In case of failure, assume this is a leaf function.  */
	  context.Rip = *(ULONG64 *) context.Rsp;
	  context.Rsp += 8;
	}
      else
	{
	  /* If the last unwinding step failed somehow, stop here.  */
	  if (IS_BAD_PTR(context.Rip))
	    break;

	  /* Unwind.  */
	  memset (&NvContext, 0, sizeof (KNONVOLATILE_CONTEXT_POINTERS));
	  RtlVirtualUnwind (0, ImageBase, context.Rip, RuntimeFunction,
			    &context, &HandlerData, &EstablisherFrame,
			    &NvContext);
	}

      /* 0 means bottom of the stack.  */
      if (context.Rip == 0)
	break;

      /* Skip frames.  */
      if (skip_frames > 1)
	{
	  skip_frames--;
	  continue;
	}
      /* Excluded frames.  */
      if ((void *)context.Rip >= exclude_min
	  && (void *)context.Rip <= exclude_max)
	continue;

      array[i++] = (void *)(context.Rip - 2);
      if (i >= size)
	break;
    }
  return i;
}

#ifdef __cplusplus
}
#endif
