/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                               C T R L _ C                                *
 *                                                                          *
 *                          C Implementation File                           *
 *                                                                          *
 *        Copyright (C) 2002-2018, Free Software Foundation, Inc.           *
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

void __gnat_install_int_handler (void (*) (void));
void __gnat_uninstall_int_handler (void);

#include <sys/stat.h>
#include <windows.h>

void (*sigint_intercepted) (void) = NULL;

static BOOL WINAPI
__gnat_int_handler  (DWORD dwCtrlType)
{
  switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
      if (sigint_intercepted != 0)
        {
          sigint_intercepted ();
          return TRUE;
        }
      break;

    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
      break;
    }

  return FALSE;
}

void
__gnat_install_int_handler (void (*proc) (void))
{
  if (sigint_intercepted == NULL)
    SetConsoleCtrlHandler (__gnat_int_handler, TRUE);

  sigint_intercepted = proc;
}

void
__gnat_uninstall_int_handler (void)
{
  if (sigint_intercepted != NULL)
    SetConsoleCtrlHandler (__gnat_int_handler, FALSE);

  sigint_intercepted = NULL;
}
