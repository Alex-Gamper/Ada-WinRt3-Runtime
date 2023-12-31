/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                                A R G V                                   *
 *                                                                          *
 *                          C Implementation File                           *
 *                                                                          *
 *         Copyright (C) 1992-2018, Free Software Foundation, Inc.          *
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

#ifdef __cplusplus
extern "C" {
#endif

int gnat_argc = 0;
const char **gnat_argv = (const char **) 0;
const char **gnat_envp = (const char **) 0;

# include <string.h>
# include <stdlib.h>
# include "env.h"

//----------------------------------------------------------------------------//
int
__gnat_arg_count (void)
{
  return gnat_argc;
}

//----------------------------------------------------------------------------//
int
__gnat_len_arg (int arg_num)
{
  if (gnat_argv != NULL)
    return strlen (gnat_argv[arg_num]);
  else
    return 0;
}

//----------------------------------------------------------------------------//
void
__gnat_fill_arg (char *a, int i)
{
  if (gnat_argv != NULL)
    memcpy (a, gnat_argv[i], strlen (gnat_argv[i]));
}

//----------------------------------------------------------------------------//
int
__gnat_env_count (void)
{
    char **env = __gnat_environ();
    int i;
    for (i = 0; env[i]; i++)
        ;
    return i;
}

//----------------------------------------------------------------------------//
int
__gnat_len_env (int env_num)
{
    char **env = __gnat_environ();
    if (env != NULL)
        return strlen (env[env_num]);
    else
        return 0;
}

//----------------------------------------------------------------------------//
void
__gnat_fill_env (char *a, int i)
{
    char **env = __gnat_environ();
    if (env != NULL)
    {
        if (env[i] != NULL)
        {
            memcpy(a, env[i], strlen(env[i]));
        }
    }
}

#ifdef __cplusplus
}
#endif
