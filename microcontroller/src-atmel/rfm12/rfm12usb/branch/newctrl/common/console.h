#define RC_VERSION                 "0.0.0"

/* Protocol Version */
#define RC_PROTO_VER               0x00

/* PROTOCOL
 *
 * Byte   Function
 *
 *  1     Request type
 *  2     Seqnum
 *  3     Reuquest Data
 *  ...   (additional payload for request)
 *
 *  In most types the request data field is only one or zero bytes long.
 *  
 *  In cases where the request length is variable (e.g. cipher list or string transmission)
 *  the 4th byte indicates the length of the payload to follow.
 *
 *  There is no function to list all features of the remote side. If a host instance wants to
 *  find out if certain features work, it should first send the request and wait for either the
 *  expected response or an error message.
 */

/* 
 * Requests for conection handshake, establishment, etc.
 */
#define RC_REQ_PING                0x00
#define RC_REQ_PONG                0x01
#define RC_REQ_GET_PROTO_VER       0x02
#define RC_REQ_PROTO_VER           0x03

#define RC_REQ_CIPHERS             0x04
#define RC_REQ_CIPHER_LIST         0x08
#define RC_REQ_CIPHER_NULL         0x05
#define RC_REQ_CIPHER_RC4          0x06  /* just because they're defined here, doesn't mean they work ;) */
#define RC_REQ_CIPHER_AES          0x07

#define RC_REQ_END_SESSION         0x09  /* logout */
#define RC_REQ_START_SESSION       0x0F  /* login */

#define RC_REQ_CHAR                0x0A  /* transfer a single char */
#define RC_REQ_RETRANSMIT          0x0C  /* request retransmit of last packet */

#define RC_REQ_E_UNSUPPORTED       0x0D  /* feature not supported error */
#define RC_REQ_E_BUSY              0x0E  /* device is busy */
#define RC_REQ_E_NO_MORE_LOGINS    0x10  /* device is already talking to another instance */

/* Errors */
#define RC_E_WRONGTYPE             0x01  /* the expect function received a packet of unexpected type */
#define RC_E_TIMEOUT               0x02  /*timeout hit */

/* USB Protocol
 *
 */

#define USB_SENDCHAR               0x00  /* directly fire out a single character */
#define USB_COMMAND                0x01  /* configuration command for the usb device */
#define USB_SENDSTR                0x02  /* fire an entire string out into the ether */
