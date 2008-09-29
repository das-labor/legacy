/*
	From: rscott@falcon.ic.net (Robert Scott)
	Newsgroups: sci.crypt
	Subject: Revision of NEWDES
	Date: 2 Mar 1996 21:47:53 GMT
	Organization: ICNet ... Your Link to the Internet ... 313-998-0090
	Lines: 164
	Message-ID: <4hafm9$r51@condor.ic.net>
	NNTP-Posting-Host: falcon.ic.net
	X-Newsreader: TIN [version 1.2 PL2]


	                         Revision of NEWDES

	                              -by Robert Scott 

	When I designed NEWDES in 1984-1985, I had a very simple key 
	expansion.  I expanded 15 bytes of key into 60 bytes by repeating 
	the 15 bytes four times.  It now appears that such a simple key 
	expansion creates a vulnerability to a related-key attack.  In a 
	related-key attack the attacker uses the fact that if the key is 
	changed to a related key, there is some information he has about the 
	resulting ciphertext without actually running the entire encryption 
	algorithm.  In the case of NEWDES, this information comes from the 
	fact that if the 15-byte key is rotated seven bytes, the expanded 
	60-byte key is also rotated seven bytes, which is exactly two
	round's worth of key.  Therefore the encryption algorithm using the 
	rotated key does 17 rounds where the last 15 rounds are the same as 
	the first 15 rounds using the un-rotated key.

	I therefore would like to revise my NEWDES algorithm to add a good 
	key expansion algorithm.  This will be done in keeping with the 
	original design goals, which were: 

	   1. open design - no hidden structure
	   2. simple and fast software implementation
	   3. functional replacement for DES with more security

	My first inclination was to simply re-order the 15 key bytes as they 
	are used in the expansion.  This would break the pattern of having a 
	rotation of seven bytes causing the same key bytes to appear in 
	similar positions in the encryption.  Such an expansion would not 
	incur any additional delay in a hardware implementation since only 
	the ordering of the expanded key bytes would be changed.  However, 
	upon more reflection, the risk of making such a minimal change was 
	too great.  There may be more complex related key attacks.  

	My next thought was to ensure that all 60 bytes of expanded key are 
	potentially different.  A simple way to do this is to form each of 
	the 60 bytes of expanded key as the exclusive-or of an original key 
	byte and one of four other bytes.  For these other four bytes I 
	have chosen 0 and three of the 15 key bytes.  Specifically, if 
	K0...K14 are the 15 key bytes, then the 60 expanded key bytes would 
	be 

	  K0       K1       K2       . . . . K13       K14   
	  K0^K7    K1^K7    K2^K7    . . . . K13^K7    K14^K7 
	  K0^K8    K1^K8    K2^K8    . . . . K13^K8    K14^K8 
	  K0^K9    K1^K9    K2^K9    . . . . K13^K9    K14^K9 

	where '^' denotes exclusive-or.  I am aware of the fact that there 
	are three zeros in this list of 60 bytes.  However, this does not 
	weaken the algorithm.  The zeroes occur in single instances of what 
	used to be K7, K8, and K9.  However, these key bytes appear more 
	than any others in the total list, so they can afford to miss one 
	showing apiece.

	The following C-code can be used to implement this revised version 
	of NEWDES.  By the way, the algorithm uses a permutation on bytes 
	called the f-function.  This function is given in tabular form 
	within the following C-code.  But if anyone would like to verify or 
	generate the randomizing permutation, f, as described in the 
	original Cryptologia article, please e-mail me and I will e-mail you 
	the C-code to generate the f-function, together with my 
	transcription of the Declaration of Independence on which the f-
	function is based. 
*/

/* newdes.c  -  Revised 3-2-96 to include better key expansion */
/*           -  Released to the public domain by Robert Scott  */
/*           -  Originally published in Cryptologia, Jan. 1985 */

/* $Id: newdes-sk.c,v 1.1 2003/03/30 12:42:21 m Exp $ */

#include <config.h>
#include <types.h>

#if CONF_WITH_CRYPT_ALGO==2

static const uint8_t f[256] = {
	32,137,239,188,102,125,221,72,212,68,81,37,86,237,147,149,
	70,229,17,124,115,207,33,20,122,143,25,215,51,183,138,142,
	146,211,110,173,1,228,189,14,103,78,162,36,253,167,116,255,
	158,45,185,50,98,168,250,235,54,141,195,247,240,63,148,2,
	224,169,214,180,62,22,117,108,19,172,161,159,160,47,43,171,
	194,175,178,56,196,112,23,220,89,21,164,130,157,8,85,251,
	216,44,94,179,226,38,90,119,40,202,34,206,35,69,231,246,
	29,109,74,71,176,6,60,145,65,13,77,151,12,127,95,199,
	57,101,5,232,150,210,129,24,181,10,121,187,48,193,139,252,
	219,64,88,233,96,128,80,53,191,144,218,11,106,132,155,104,
	91,136,31,42,243,66,126,135,30,26,87,186,182,154,242,123,
	82,166,208,39,152,190,113,205,114,105,225,84,73,163,99,111,
	204,61,200,217,170,15,198,28,192,254,134,234,222,7,236,248,
	201,41,177,156,92,131,67,249,245,184,203,9,241,0,27,46,
	133,174,75,18,93,209,100,120,76,213,16,83,4,107,140,52,
	58,55,3,244,97,197,238,227,118,49,79,230,223,165,153,59
};

void newdessk_enc( uint8_t* v, uint8_t* k )
{
	uint8_t i;
	uint8_t ex;

	ex = 0;
	i = 0;

	for(;;)
	{
		v[4] = v[4] ^ f[v[0] ^ k[i] ^ ex];
		if(++i==15) {i = 0;  ex = k[7];}
		v[5] = v[5] ^ f[v[1] ^ k[i] ^ ex];
		if(++i==15) {i = 0;  ex = k[8];}
		v[6] = v[6] ^ f[v[2] ^ k[i] ^ ex];
		if(++i==15) {i = 0;  ex = k[9];}
		v[7] = v[7] ^ f[v[3] ^ k[i] ^ ex];
		if(++i==15) return;

		v[1] = v[1] ^ f[v[4] ^ k[i++] ^ ex];
		v[2] = v[2] ^ f[v[4] ^ v[5]];
		v[3] = v[3] ^ f[v[6] ^ k[i++] ^ ex];
		v[0] = v[0] ^ f[v[7] ^ k[i++] ^ ex];
	}
}

#if CONF_WITH_DECRYPT==1
void newdessk_dec( uint8_t* v, uint8_t* k )
{
	uint8_t i;
	uint8_t ex;

	ex = k[9];
	i = 14;

	for(;;)
	{
		v[7] = v[7] ^ f[v[3] ^ k[i] ^ ex];
		if(i--==0) {i = 14;  ex = k[8];}
		v[6] = v[6] ^ f[v[2] ^ k[i] ^ ex];
		if(i--==0) {i = 14;  ex = k[7];}
		v[5] = v[5] ^ f[v[1] ^ k[i] ^ ex];
		if(i--==0) {i = 14;  ex = 0;}
		v[4] = v[4] ^ f[v[0] ^ k[i] ^ ex];

		if(i--==0) return;

		v[0] = v[0] ^ f[v[7] ^ k[i--] ^ ex];
		v[3] = v[3] ^ f[v[6] ^ k[i--] ^ ex];
		v[2] = v[2] ^ f[v[4] ^ v[5]];
		v[1] = v[1] ^ f[v[4] ^ k[i--] ^ ex];
	}
}
#endif /* CONF_WITH_DECRYPT==1 */

/* Notes:

     The original NEWDES algorithm was vulnerable to a related-key 
attack using a large number of chosen plaintexts.  The weakness was 
due to the simple key expansion algorithm which made key rotation by 
seven bytes cause the last 15 rounds to be the same as the first 15 
rounds using the un-rotated key.  The revised key expansion uses 
the 15 key bytes exclusive-ored with 0, 1, 2 and 3 to generate 60 
unique bytes of expanded key.

     The original C-language representation of the algorithm used a 
single function to do both encryption and decryption, the difference 
being in the setting of three global variables.  Even though the 
idea could be extended to include this new key expansion algorithm, 
it seemed that the benefits of streamlined encrypt() and decrypt() 
functions now outweigh the advantages using only one function.  Note 
that the algorithm could run even faster if the 60-byte expanded key 
were pre-computed in an array.
*/

#endif /* CONF_WITH_CRYPT_ALGO==2 */

