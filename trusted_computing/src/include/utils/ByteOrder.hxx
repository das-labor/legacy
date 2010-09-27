// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef BYTEORDER_HXX_
#define BYTEORDER_HXX_

#include <iostream>

#ifdef WORDS_BIGENDIAN
   #define ntohll(x) (x)
   #define htonll(x) (x)
#else
   #if defined( __CYGWIN32__ )
	 #include <asm/byteorder.h>
	 #define ___my_swap64(x) \
        ((u_int64_t)( \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x00000000000000ffULL) << 56) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x000000000000ff00ULL) << 40) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x0000000000ff0000ULL) << 24) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x00000000ff000000ULL) <<  8) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x000000ff00000000ULL) >>  8) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x0000ff0000000000ULL) >> 24) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0x00ff000000000000ULL) >> 40) | \
        (u_int64_t)(((u_int64_t)(x) & (u_int64_t)0xff00000000000000ULL) >> 56) ))

     #define ntohll(x)  ___my_swap64(x)
     #define htonll(x)  ___my_swap64(x)
   #elif defined( __WIN32__ )
//      #include <winsock.h>
//      #include <windows.h>

//	#define ntohs(x)  (x)
//     #define htons(x)  (x)
//     #define ntohl(x)  (x)
//     #define htonl(x)  (x)
//     #define ntohll(x) (x)
//     #define htonll(x) (x)


    #define ___my_swap64(x) \
        ((uint64_t)( \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56) ))

    #define ___my_swap32(x) \
        ((uint32_t)( \
        (uint32_t)(((uint32_t)(x) & (uint32_t)0x000000ffULL) << 24) | \
        (uint32_t)(((uint32_t)(x) & (uint32_t)0x0000ff00ULL) << 8) | \
        (uint32_t)(((uint32_t)(x) & (uint32_t)0x00ff0000ULL) >> 8) | \
        (uint32_t)(((uint32_t)(x) & (uint32_t)0xff000000ULL) >>  24) ) )

    #define ___my_swap16(x) \
        ((uint16_t)( \
        (uint16_t)(((uint16_t)(x) & (uint16_t)0x00ffULL) << 8) | \
        (uint16_t)(((uint16_t)(x) & (uint16_t)0xff00ULL) >>  8) ) )

     #define ntohll(x)  ___my_swap64(x)
     #define htonll(x)  ___my_swap64(x)

     #define htonl(x)  ___my_swap32(x)
     #define ntohl(x)  ___my_swap32(x)

     #define htons(x) ___my_swap16(x)
     #define ntohs(x) ___my_swap16(x)

#elif defined( __linux__ )
     #include <netinet/in.h>          /// @todo System-specific byte ordering has to be implemented again under Windows. Use winsock2.h
     #include <asm/byteorder.h>       /// @warning 64bit byteordering is Linux specific

     #define ntohll(x)  __be64_to_cpu(x)
     #define htonll(x)  __cpu_to_be64(x)
   #else
     #error Unsupported platform
   #endif
#endif

namespace utils {
        /**
         * @brief Template function to convert number from host into network byte order.
         *
         * @see    networkToHost
         * @author Christian Stueble
         */
        template<typename NUM> inline NUM hostToNetwork( const NUM &number );

        /**
         * @brief Template function to convert number from network into host byte order.
         *
         * @see    hostToNetwork
         * @author Christian Stueble
         */
        template<typename NUM> inline NUM networkToHost( const NUM &number );

        // -------------------- inline impementations --------------------------
        template<typename NUM>
        inline NUM hostToNetwork( const NUM &number ) {
                std::cerr << "hostToNetwork not implemented for this type" << std::endl;
                return number;
        }

        // not-so-special implementation for bool
        template<>
        inline bool hostToNetwork<bool>( const bool &val ) {
                return val;
        }
        // not-so-special implementation for unsigned char
        template<>
        inline unsigned char hostToNetwork<unsigned char>( const unsigned char &val ) {
                return val;
        }
        // special implementation for unsigned int
        template<>
        inline unsigned int hostToNetwork<unsigned int>( const unsigned int &number ) {
                return htonl( number );
        }
        // special implementation for unsigned short
        template<>
        inline unsigned short hostToNetwork<unsigned short>( const unsigned short &number ) {
                return htons( number );
        }
        // special implementation for unsigned long long
        template<>
        inline unsigned long long hostToNetwork<unsigned long long>( const unsigned long long &number ) {
                return htonll( number );
        }

        template<typename NUM>
        inline NUM networkToHost( const NUM& number ) {
                std::cerr << "networkToHost not implemented for this type" << std::endl;
                return number;
        }

        // not-so-special implementation for bool
        template<>
        inline bool networkToHost<bool>( const bool &val ) {
                return val;
        }
        // not-so-special implementation for unsigned char
        template<>
        inline unsigned char networkToHost<unsigned char>( const unsigned char &val ) {
                return val;
        }
        // special implementation for unsigned int
        template<>
        inline unsigned int networkToHost<unsigned int>( const unsigned int &number ) {
                return ntohl( number );
        }
        // special implementation for unsigned short
        template<>
        inline unsigned short networkToHost<unsigned short>( const unsigned short &number ) {
                return ntohs( number );
        }
        // special implementation for unsigned long long
        template<>
        inline unsigned long long networkToHost<unsigned long long>( const unsigned long long &number ) {
                return ntohll( number );
        }
}

#endif /*BYTEORDER_HXX_*/
