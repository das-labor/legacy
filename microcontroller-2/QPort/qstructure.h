/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */



#ifndef QSTRUCTURE_H_
#define QSTRUCTURE_H_


/********************************************************************************************************/

/* Message:
 * 
 * +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 * | T=MSG |  L_0  |  L_1  |  D_0  |  D_1  |  ...  |  D_l  |  S_0  |  S_1  |  ...  |  S_sz | 
 * +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 *     P       C       C       C       C       C       C
 * 
 */

/* Key packet:
 * +-------+
 * | T=KEY |
 * +-------+
 * #=====================================================================================================#
 * #                                                                                                     #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * # | RAND  | RAND  | CNT0  | CNT1  | CNT2  | CNT3  | KTA_0 |  ...  | KTA_n | KTB_0 |  ...  | KTB_m |   #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * #                                                                                                     #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * # | KRA_0 |  ...  | KRA_n | KRB_0 |  ...  | KRB_n | KTS_0 |  ...  | KTS_x | KRS_0 |  ...  | KRS_x |   #
 * # +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+   #
 * #                                                                                                     #
 * # +-------+-------+-------+                                                                           #
 * # |  S_0  |  ...  |  S_sz |                                                                           #
 * # +-------+-------+-------+                                                                           #
 * #                                                                                                     #
 * #=====================================================================================================#
 * 
 * CNT0..CNT3 := Counter xor hash(RAND)
 * 
 * the actual keys are hashes from the here transfared keys
 * 
 */
 
 
#define KP_RAND_SIZE	12
#define KP_CNT_SIZE		4
 /* may be replaced by union later ... */
#define KP_RAND_OFFSET	0
#define KP_CNT_OFFSET	(KP_RAND_OFFSET + KP_RAND_SIZE)
#define KP_KTA_OFFSET	(KP_CNT_OFFSET + KP_CNT_SIZE)
#define KP_KTB_OFFSET	(KP_KTA_OFFSET + KEYA_SIZE) 
#define KP_KRA_OFFSET	(KP_KTB_OFFSET + KEYB_SIZE)
#define KP_KRB_OFFSET	(KP_KRA_OFFSET + KEYA_SIZE)
#ifdef QP_MSG_CHECK_HMAC
	#define KP_KTS_OFFSET	(KP_KRB_OFFSET + KEYB_SIZE)
	#define KP_KRS_OFFSET	(KP_KTS_OFFSET + KEYS_SIZE)
	#define KP_SIGN_OFFSET	(KP_KRS_OFFSET + KEYS_SIZE)
#else
	#define KP_SIGN_OFFSET	(KP_KRB_OFFSET + KEYB_SIZE)
#endif


/* QPort type indicators */
#define QP_RESET_CODE		0x23
#define QP_ESC_CODE			0x42
#define QP_XON_CODE			0x11
#define QP_XOFF_CODE		0x13

#define QP_RESET_ESC		0x01
#define QP_ESC_ESC			0x02
#define QP_XON_ESC			0x03
#define QP_XOFF_ESC			0x04

#define QP_TYPE_KEYTX		0x10
#define QP_TYPE_KEYREQ		0x14
#define QP_TYPE_KEYACK		0x12
#define QP_TYPE_ACKACK		0x15
#define QP_TYPE_NULL		0x00
#define QP_TYPE_SECMSG		0x06
#define QP_TYPE_SECSTR		0x07
#define QP_TYPE_EOSTR		0x05

/* QPort signals */
#define ERROR_ILLEGAL_SEQUENCE	(void*)0, 3, 2,1,1
#define ERROR_IDLE_RECV		 	(void*)0, 3, 2,1,2
#define ERROR_INVALID_VAR		(void*)0, 3, 2,1,3
#define ERROR_MSG_TOO_LONG		(void*)0, 3, 2,1,4
#define ERROR_INVALID_STATE_TRASITION (void*)0, 3, 2,1,5

#define ERROR_BAD_MSG_SIG		(void*)0, 3, 2,2,1
#define ERROR_BAD_KEYPACKET_SIG	(void*)0, 3, 2,2,2
#define ERROR_BAD_COUNTER		(void*)0, 3, 2,2,3
#define ERROR_MALLOC_FAILED		(void*)0, 2, 1,1
#define STUPID_BUT_EVIL_PROGRAMMER_ERROR	(void*)0, 3, 6,6,6

#endif /*QSTRUCTURE_H_*/
