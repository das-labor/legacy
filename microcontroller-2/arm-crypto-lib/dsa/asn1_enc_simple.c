/* asn1_enc_simple.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "bigint.h"
#include <avr/pgmspace.h>

#ifndef __AVR__
#define PROGMEM
#endif


/*
 * bi   - bigint
 * i8  -  8 bit signed integer
 * i16 - 16 bit signed integer
 * i32 - 32 bit signed integer
 * i64 - 64 bit signed integer
 * u8  -  8 bit unsigned integer
 * u16 - 16 bit unsigned integer
 * u32 - 32 bit unsigned integer
 * u64 - 64 bit unsigned integer
 * os  - octet string
 * bs  - bit string
 * s0  - zero terminated string
 * b   - boolean
 * r   - real
 * n   . null
 * %   - tag prefix
 * *   - progmem prefix
 */


#define BRACE_ERROR           0x01
#define TOKEN_TOO_LARGE_ERROR 0x02
#define UNKNOWN_TOKEN_ERROR   0x03
#define MAX_TOKEN_SIZE 12

typedef struct{
	uint16_t length_B;
	uint8_t *data;
} octetstring_t;

typedef struct {
	uint32_t length_b;
	uint8_t *data;
} bitstring_t;

typedef uint16_t(*gs_fpt)(void*);
typedef void(*cp_fpt)(void*, void*);

typedef struct{
	char  name[4];
	uint8_t  flags;
	uint8_t  default_tag;
	uint16_t fixsize;
	gs_fpt   get_size;
	gs_fpt   get_size_P;
	cp_fpt   copy;
	cp_fpt   copy_P;
}token_handler_t;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

uint16_t bi_length(const bigint_t* a){
	if(a->length_B==0){
		return 1;
	}
	if(a->wordv[a->length_B-1]&0x80){
		return a->length_B+1;
	}
	return a->length_B;
}

/******************************************************************************/

void bi_copy(void* dest, const bigint_t* a){
	if(a->length_B==0){
		*((uint8_t*)dest) = 0;
		return;
	}
	/* negative bigint, convert two 2's complement form */
	uint8_t sign_extended=0;
	if(a->wordv[a->length_B]&0x80){
		*((uint8_t*)dest) = 0;
		dest = (uint8_t*)dest +1;
		sign_extended = 1;
	}
	uint16_t i;
	if(a->info&BIGINT_NEG_MASK){
		/* convert to 2's complement */
		uint8_t carry=1;
		i=0;
		dest = (uint8_t*)dest+a->length_B-1+sign_extended;
		do{
			if((*((uint8_t*)dest) = (a->wordv[i++]^0xff)+carry)!=0){
				carry=0;
			}
			dest = (uint8_t*)dest -1;
		}while(i<a->length_B);
		if(sign_extended){
			*((uint8_t*)dest) = 0xff;
		}
	}else{
		i=a->length_B-1;
		do{
			*((uint8_t*)dest) = a->wordv[i];
			dest = (uint8_t*)dest +1;
		}while(i);
	}
}

/******************************************************************************/
/******************************************************************************/

uint16_t u8_length(uint8_t a){
	if(a>=0x80){
		return 2;
	}
	return 1;
}

/******************************************************************************/

void u8_copy(void* dest, uint8_t a){
	if(a>=0x80){
		((uint8_t*)dest)[0]=0;
		dest = (uint8_t*)dest +1;
	}
	((uint8_t*)dest)[0]=a;
}

/******************************************************************************/
/******************************************************************************/

uint16_t u16_length(uint16_t a){
	if(a>=0x8000){
		return 3;
	}
	return 2;
}

/******************************************************************************/

void u16_copy(void* dest, uint16_t a){
	if(a>=0x8000){
		((uint8_t*)dest)[0]=0;
		dest = (uint8_t*)dest +1;
	}
	((uint16_t*)dest)[0]=a;
}

/******************************************************************************/
/******************************************************************************/

uint16_t u32_length(uint32_t a){
	if(a>=0x80000000L){
		return 5;
	}
	return 4;
}

/******************************************************************************/

void u32_copy(void* dest, uint32_t a){
	if(a>=0x80000000L){
		((uint8_t*)dest)[0]=0;
		dest = (uint8_t*)dest +1;
	}
	((uint32_t*)dest)[0]=a;
}

/******************************************************************************/
/******************************************************************************/

uint16_t u64_length(uint32_t a){
	if(a>=0x8000000000000000LL){
		return 9;
	}
	return 8;
}

/******************************************************************************/

void u64_copy(void* dest, uint64_t a){
	if(a>=0x8000000000000000LL){
		((uint8_t*)dest)[0]=0;
		dest = (uint8_t*)dest +1;
	}
	((uint64_t*)dest)[0]=a;
}

/******************************************************************************/
/******************************************************************************/

uint16_t os_length(octetstring_t* os){
	return os->length_B;
}

/******************************************************************************/

void os_copy(void* dest, octetstring_t* os){
	memcpy(dest, os->data, os->length_B);
}

/******************************************************************************/
/******************************************************************************/

uint16_t bs_length(bitstring_t* bs){
	return 1+(bs->length_b+7)/8;
}

/******************************************************************************/

void bs_copy(void* dest, bitstring_t* bs){
	*((uint8_t*)dest) = (8-bs->length_b&7)&7;
	memcpy((uint8_t*)dest+1, bs->data, (bs->length_b+7)/8);
}

/******************************************************************************/
/******************************************************************************/

void b_copy(void* dest, uint8_t b){
	*((uint8_t*)dest) = b?0xff:0x00;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

token_handler_t handler[] PROGMEM = {
	{{'b', 'i',   0,   0}, 0, 0x02, 0, (gs_fpt)bi_length,  (gs_fpt)NULL, (cp_fpt)bi_copy,  (cp_fpt)NULL }, /* bigint */
	{{'i', '8',   0,   0}, 0, 0x02, 1, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL }, /* signed 8 bit */
	{{'i', '1', '6',   0}, 0, 0x02, 2, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL }, /* signed 16 bit */
	{{'i', '3', '2',   0}, 0, 0x02, 4, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL }, /* signed 32 bit */
	{{'i', '6', '4',   0}, 0, 0x02, 8, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL },
	{{'u', '8',   0,   0}, 0, 0x02, 0, (gs_fpt)u8_length,  (gs_fpt)NULL, (cp_fpt)u8_copy,  (cp_fpt)NULL },
	{{'u', '1', '6',   0}, 0, 0x02, 0, (gs_fpt)u16_length, (gs_fpt)NULL, (cp_fpt)u16_copy, (cp_fpt)NULL },
	{{'u', '3', '2',   0}, 0, 0x02, 0, (gs_fpt)u32_length, (gs_fpt)NULL, (cp_fpt)u32_copy, (cp_fpt)NULL },
	{{'u', '6', '4',   0}, 0, 0x02, 0, (gs_fpt)u64_length, (gs_fpt)NULL, (cp_fpt)u64_copy, (cp_fpt)NULL },
	{{'o', 's',   0,   0}, 0, 0x04, 0, (gs_fpt)os_length,  (gs_fpt)NULL, (cp_fpt)os_copy,  (cp_fpt)NULL },
	{{'b', 's',   0,   0}, 0, 0x03, 0, (gs_fpt)bs_length,  (gs_fpt)NULL, (cp_fpt)bs_copy,  (cp_fpt)NULL },
	{{'b',   0,   0,   0}, 0, 0x01, 1, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)b_copy,   (cp_fpt)NULL },
//	{{'r',   0,   0,   0}, 0, 0x09, 0, (gs_fpt)r_length,   (gs_fpt)NULL, (cp_fpt)r_copy,   (cp_fpt)NULL },
	{{'n',   0,   0,   0}, 0, 0x05, 0, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL },
	{{  0,   0,   0,   0}, 0,    0, 0, (gs_fpt)NULL,       (gs_fpt)NULL, (cp_fpt)NULL,     (cp_fpt)NULL } /* termination */
};

uint8_t get_next_token_n(char* dest, const char* str, uint8_t n){
	uint8_t i=0, skip=0;
	while(*str==' ' || *str=='\t' || *str=='\r' || *str=='\n'){
		++str; ++skip;
	}
	if (*str=='\0'){
		return 0;
	}
	while(n-1 && ((*str>='0' && *str<='9')
			 || (*str>='a' && *str<='z')
			 || (*str>='A' && *str<='Z')
			 || *str>='{' || *str<='}' || *str=='*' || *str=='%')){
		if(i!=0 && (*str=='{' || *str=='}')){
			dest[i] = '\0';
			return i;
		}
		dest[i++] = *str;
		if(*str=='{' || *str=='}'){
			dest[i] = '\0';
			return i;
		}
		--n; ++str;
	}
	dest[i] = '\0';
	return i+skip;
}

uint8_t check_and_count_token(uint16_t* paramtokencount, uint16_t* tokencount,
		uint16_t* depthout, uint8_t* token_stream, const char* str){
	uint8_t intoken=0, skip, token_length, token_offset, i;
	uint16_t ntoken=0;
	uint16_t paramtokens=0;
	uint16_t depth=0;
	uint16_t max_depth=0;
	char token[MAX_TOKEN_SIZE];
	uint32_t tokenid, refid;
	uint8_t *handler_ptr;
	uint8_t progmem_flag, tag_flag;

	while((skip=get_next_token(token, str, MAX_TOKEN_SIZE-1))&&token[0]!='\0'){
		str += skip;
		++ntoken;
		if(token[0]=='{'){
			depth += 1;
			if(token_stream){
				*token_stream++ = 0xfe;
			}
			continue;
		}
		if(token[0]=='}'){
			if(depth==0){
				return BRACE_ERROR;
			}
			if(token_stream){
				*token_stream++ = 0xfd;
			}
			depth -= 1;
			continue;
		}
		tag_flag = progmem_flag = 0;
		if(token[0]=='%'){
			tag_flag = 1;
			if(token[1]=='*'){
				progmem_flag=1;
				token_offset = 2;
			}else{
				token_offset = 1;
			}
		}
		if(token[0]=='*'){
			progmem_flag=1;
			if(token[1]=='%'){
				tag_flag = 1;
				token_offset = 2;
			}else{
				token_offset = 1;
			}
		}
		token_length=strlen(token);
		if(token_length-token_offset>3){
			return TOKEN_TOO_LARGE_ERROR;
		}
		tokenid=0;
		memcpy(&tokenid, token+token_offset, token_length);
		handler_ptr = handler;
		i=0;
		do{
			++i;
			refid = pgm_read_dword(handler_ptr);
			handler_ptr += sizeof(token_handler_t);
		}while(refid && refid!=tokenid);
		if(token_stream){
			*token_stream++=(i<<2)|(progmem_flag<<1)|(tag_flag);
		}
		++paramtokens;
	}
    if(token_stream){
    	*token_stream=0;
    }
	if(depth!=0){
		return BRACE_ERROR;
	}
	if(paramtokencount){
		*paramtokencount = paramtokens;
	}
	if(depthout){
		*depthout = depth;
	}
	if(tokens){
		*tokens = ntoken;
	}
	return 0;
}

void* asn1_encode_simple(void* dest, uint16_t max_size, const char* f, ...){
	uint8_t t;
	uint16_t depth=0;
	uint16_t idx=0;
	uint16_t paramtokens;
	uint16_t totaltokens;
	va_list ap;
	t = check_and_count_token(&paramtokens, &totaltokens, &depth, f);
	if(t){
		return t;
	}
	uint32_t sizes[max_depth+1];
	char token[12];
	memset(token, 0, 12);
	memset(sizes, 0, (max_dept+1)*sizeof(uint32_t));
//	va_start()
	for(idx=0; f[idx]; ++idx){

	}
}

