/*
	Simple Operating System for Smartcard Education
	Copyright (C) 2002	Matthias Bruestle <m@mbsks.franken.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
	\brief Crypto declarations.

	$Id: crypt.h,v 1.1 2003/03/30 12:42:21 m Exp $
*/

#ifndef SOSSE_CRYPT_H
#define SOSSE_CRYPT_H

#include <config.h>

#include <sw.h>
#include <string.h>
#include <types.h>

#define CRYPT_ALGO_TEA			0	//!< Algorithm ID: TEA
#define CRYPT_ALGO_SKIPJACK		1	//!< Algorithm ID: Skipjack
#define CRYPT_ALGO_NEWDESSK		2	//!< Algorithm ID: NEWDES-SK

#if CONF_CRYPT_ALGO==0
/* TEA */
#include <tea.h>
//! Length of key in octets.
#define CRYPT_KEY_LEN	TEA_KEY_LEN
//! Length of cipher block in octets.
#define CRYPT_BLOCK_LEN	TEA_BLOCK_LEN
//! Single block encryption function.
#define crypt_enc(v,k) tea_enc((iu32*)(v),(iu32*)(k))
//! Single block decryption function.
#define crypt_dec(v,k) tea_dec((iu32*)(v),(iu32*)(k))

#elif CONF_CRYPT_ALGO==1
/* SKIPJACK */
#include <skipjack.h>
//! Length of key in octets.
#define CRYPT_KEY_LEN	SKIPJACK_KEY_LEN
//! Length of cipher block in octets.
#define CRYPT_BLOCK_LEN	SKIPJACK_BLOCK_LEN
//! Single block encryption function.
#define crypt_enc(v,k) skipjack_enc((iu8*)(v),(iu8*)(k))
//! Single block decryption function.
#define crypt_dec(v,k) skipjack_dec((iu8*)(v),(iu8*)(k))

#elif CONF_CRYPT_ALGO==2
/* NEWDES-SK */
#include <newdes-sk.h>
//! Length of key in octets.
#define CRYPT_KEY_LEN	NEWDESSK_KEY_LEN
//! Length of cipher block in octets.
#define CRYPT_BLOCK_LEN	NEWDESSK_BLOCK_LEN
//! Single block encryption function.
#define crypt_enc(v,k) newdessk_enc((iu8*)(v),(iu8*)(k))
//! Single block decryption function.
#define crypt_dec(v,k) newdessk_dec((iu8*)(v),(iu8*)(k))

#else
#error No valid crypto algorithm selected.
#endif

#endif /* SOSSE_CRYPT_H */

