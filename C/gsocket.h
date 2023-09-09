/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                              G S O C K E T                               *
 *                                                                          *
 *                              C Header File                               *
 *                                                                          *
 *         Copyright (C) 2004-2023, Free Software Foundation, Inc.          *
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

#define HAVE_SOCKETS

#define _XOPEN_SOURCE_EXTENDED 1
#define BSD_COMP 1
#define _ALL_SOURCE 1

#define FD_SETSIZE 1024

#include <winsock2.h>
#include <ws2tcpip.h>
#include <versionhelpers.h>

#undef  EACCES
#define EACCES          WSAEACCES
#undef  EADDRINUSE
#define EADDRINUSE      WSAEADDRINUSE
#undef  EADDRNOTAVAIL
#define EADDRNOTAVAIL   WSAEADDRNOTAVAIL
#undef  EAFNOSUPPORT
#define EAFNOSUPPORT    WSAEAFNOSUPPORT
#undef  EALREADY
#define EALREADY        WSAEALREADY
#undef  EBADF
#define EBADF           WSAEBADF
#undef  ECONNABORTED
#define ECONNABORTED    WSAECONNABORTED
#undef  ECONNREFUSED
#define ECONNREFUSED    WSAECONNREFUSED
#undef  ECONNRESET
#define ECONNRESET      WSAECONNRESET
#undef  EDESTADDRREQ
#define EDESTADDRREQ    WSAEDESTADDRREQ
#undef  EFAULT
#define EFAULT          WSAEFAULT
#undef  EHOSTDOWN
#define EHOSTDOWN       WSAEHOSTDOWN
#undef  EHOSTUNREACH
#define EHOSTUNREACH    WSAEHOSTUNREACH
#undef  EINPROGRESS
#define EINPROGRESS     WSAEINPROGRESS
#undef  EINTR
#define EINTR           WSAEINTR
#undef  EINVAL
#define EINVAL          WSAEINVAL
#undef  EIO
#define EIO             WSAEDISCON
#undef  EISCONN
#define EISCONN         WSAEISCONN
#undef  ELOOP
#define ELOOP           WSAELOOP
#undef  EMFILE
#define EMFILE          WSAEMFILE
#undef  EMSGSIZE
#define EMSGSIZE        WSAEMSGSIZE
#undef  ENAMETOOLONG
#define ENAMETOOLONG    WSAENAMETOOLONG
#undef  ENETDOWN
#define ENETDOWN        WSAENETDOWN
#undef  ENETRESET
#define ENETRESET       WSAENETRESET
#undef  ENETUNREACH
#define ENETUNREACH     WSAENETUNREACH
#undef  ENOBUFS
#define ENOBUFS         WSAENOBUFS
#undef  ENOPROTOOPT
#define ENOPROTOOPT     WSAENOPROTOOPT
#undef  ENOTCONN
#define ENOTCONN        WSAENOTCONN
#undef  ENOTSOCK
#define ENOTSOCK        WSAENOTSOCK
#undef  EOPNOTSUPP
#define EOPNOTSUPP      WSAEOPNOTSUPP
#undef  EPFNOSUPPORT
#define EPFNOSUPPORT    WSAEPFNOSUPPORT
#undef  EPROTONOSUPPORT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#undef  EPROTOTYPE
#define EPROTOTYPE      WSAEPROTOTYPE
#undef  ESHUTDOWN
#define ESHUTDOWN       WSAESHUTDOWN
#undef  ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
#undef  ETIMEDOUT
#define ETIMEDOUT       WSAETIMEDOUT
#undef  ETOOMANYREFS
#define ETOOMANYREFS    WSAETOOMANYREFS
#undef  EWOULDBLOCK
#define EWOULDBLOCK     WSAEWOULDBLOCK

#define SHUT_RD		SD_RECEIVE
#define SHUT_WR		SD_SEND
#define SHUT_RDWR	SD_BOTH

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <limits.h>
#include <errno.h>
#include <stddef.h>
#include <sys/time.h>


# define HAVE_THREAD_SAFE_GETxxxBYyyy 1

#if defined (HAVE_GETxxxBYyyy_R)
# define Need_Netdb_Buffer 1
# define Need_Netdb_Lock 0
#else
# define Need_Netdb_Buffer 0
# if !defined (HAVE_THREAD_SAFE_GETxxxBYyyy)
#  define Need_Netdb_Lock 1
# else
#  define Need_Netdb_Lock 0
# endif
#endif

# define Has_Sockaddr_Len (offsetof(struct sockaddr_in, sin_family) != 0)
