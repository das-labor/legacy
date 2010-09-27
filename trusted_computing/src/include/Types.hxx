// Copyright (C) 2008, 2009 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


#ifndef BasicTypes_hxx_included
#define BasicTypes_hxx_included

#include <Config.hxx>

/* 8 bit types */
#if (SIZEOF_CHAR == 1)

#if !defined(_typedef___UInt8)
# define _typedef___UInt8
  typedef unsigned char     UInt8;
#endif

#if !defined(_typedef___Int8)
# define _typedef___Int8
  typedef char               Int8;
#endif

#else
  #error Unsupported Char size. See __FILE__, line __LINE__.
#endif

#if (SIZEOF_SHORT == 2)

#if !defined(_typedef___UInt16)
# define _typedef___UInt16
  typedef unsigned short   UInt16;
#endif

#if !defined(_typedef___Int16)
# define _typedef___Int16
  typedef short             Int16;
#endif

#else
  #error Unsupported Short size. See __FILE__, line __LINE__.
#endif

/* 32 bit types */
#if (SIZEOF_INT == 4)

#if !defined(_typedef___UInt32)
# define _typedef___UInt32
  typedef unsigned int      UInt32;
#endif

#if !defined(_typedef___Int32)
# define _typedef___Int32
  typedef int               Int32;
#endif

#else
  #error Unsupported Long Size. See __FILE__, line __LINE__.
#endif

/**
 *  64 bit types
 */
#if (SIZEOF_LONG_LONG == 8)

#if !defined(_typedef___UInt64)
# define _typedef___UInt64
  typedef unsigned long long UInt64;
#endif

#if !defined(_typedef___Int64)
# define _typedef___Int64
  typedef long long           Int64;
#endif

#else
  #error Unsupported Long Long Size. See __FILE__, line __LINE__.
#endif

/**
 *  * @brief Default size of containers etc.
 *   *
 *    * For efficiency purposes, size should always have the size of the CPU
 *     * register. we currently assume that int has the correct size.
 *      */
#if (SIZEOF_INTP != SIZEOF_LONG)
  #error Unknown register size. Please update __FILE__, line __LINE__.
#endif

#if !defined(_typedef___Size)
# define _typedef___Size
  typedef unsigned long        Size;
#endif

#endif
