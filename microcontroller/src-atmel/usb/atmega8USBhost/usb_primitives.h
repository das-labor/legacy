// usb_primitives.h
// BCD -- Ben Hutton, Chris Leary, Devrin Talen
// 100-column width


/*
 * NOTE: On the use of fixed-width integer types
 *
 * As many specification-guzzling C programmers will know, fixed width integer types are defined by
 * the C99 standard, but not (individually) required. To quote from the ISO C99 standard:
 *
 *      These types are optional. However, if an implementation provides integer types with widths
 *      of 8, 16, 32, or 64 bits, no padding bits, and (for the signed types) that have a two's
 *      complement representation, it shall define the corresponding typedef names.
 *
 * For this reason the use of fixed width integer types is frowned upon by some members of the C 
 * community. Opting for fixed-width types in this application was a result of requiring fixed-width
 * types in one portion of the application (which involved a nibble lookup table), without which
 * the algorithm would have been terribly un-optimized.
 *
 */


#ifndef _USB_PRIMITIVES_H
#define _USB_PRIMITIVES_H


/************/
/* INCLUDES */
/************/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <string.h>

// if NO_AVR_GCC is defined at compile time, these libraries will be excluded
#ifndef NO_AVR_GCC 
#   include <avr/io.h>
    // the following definition is necessary for delay.h to quit yammerin' about not knowing the 
    // CPU speed
#   ifndef F_CPU
#       define F_CPU 15000000UL
#   endif
#   include <avr/delay.h>
#endif


/***********/
/* DEFINES */
/***********/


#define PID_BITS 8
#define ADDR_BITS 7
#define ENDP_BITS 4
#define CRC5_BITS 5
#define CRC16_BITS 16
#define TOKEN_PACKET_SIZE ((PID_BITS + ADDR_BITS + ENDP_BITS + CRC5_BITS) >> 3)
#define BASE_DATA_PACKET_SIZE ((PID_BITS + CRC16_BITS) >> 3)
#define CHAR_BITS (sizeof(char) * 8)
#define INT_BITS (sizeof(int) * 8)
#define TOCKEN_PACKET_LENGTH (TOKEN_PACKET_SIZE * CHAR_BITS)
#define SYNC 0xAB   // = 10101011
#define TRANSFER_MAX_RETRIES 3 
#define TRANSACTIONERROR 1
#define TRANSACTIONSUCCESS 0
/***********/
/* GLOBALS */
/***********/


extern const uint8_t nibble_mirror[];
extern uint8_t print_buffer[];


/*********/
/* ENUMS */
/*********/


typedef enum { UNENCODED, NRZI } ENCODING_T;
typedef enum { TRANSMIT=0, RECEIVE=1, BIDIRECTIONAL=2 } TRANSMISSION_DIRECTION_T;
typedef enum { CONTROL=0, INTERRUPT=1 } TRANSFER_TYPE_T;
typedef enum { MESSAGE, STREAM_IN, STREAM_OUT } PIPE_TYPE_T;

//HID is fake
typedef enum {
    DEVICE=0x1, CONFIGURATION=0x2, INTERFACE=0x4, ENDP=0x5, STRING=0x3, HID=0xf
} DESCRIPTOR_TYPE_T;

typedef enum 
// NOTE: since PRE and ERR have the same hex value, we have to combine them into one
// enum item so that switch statements can compile without a duplicate value error
{
    OUT = 0x87, IN = 0x96, SETUP = 0xb4, // token group
    DATA0 = 0xc3, DATA1 = 0xd2, DATA2 = 0xe1, MDATA = 0xf0, // data group
    UNDETERMINED = 0x00, ACK = 0x4b, NAK = 0x5a, STALL = 0x78, NYET = 0x69, // handshake
    PRE_ERR = 0x3c, SPLIT = 0x1e, PING = 0x2d // special group
} PID_T;

typedef enum { STUFF, UNSTUFF } STUFF_METHOD_T;

/***********/
/* STRUCTS */
/***********/


struct PtrNode
// A doubly linked list node that wraps a pointer
{
    void *ptr;
    struct PtrNode *prev;
    struct PtrNode *next;
};


/*
typedef struct TransferDescriptor
{
    // If False, you need an exact amount of data to fill the buffer. If True, then the last data 
    // packet may be smaller than the defined buffer without causing an error condition on the TD.
    BOOL_T buffer_rounding;

    // Two bit field that indicates the direction of data flow and the PID to be used for the token.
    // This field is only relevant to the HC if the D field in the ED was set to 00b or 11b 
    // indicating that the PID determination is deferred to the TD.
    // NOTE: altered from 2-bit field length.
    PID_T direction_pid;

    // Contains the interrupt delay count for this TD. When a TD is complete the HC may wait for 
    // delay_interrupt frames before generating an interrupt. If DelayInterrupt is 111b, then there 
    // is no interrupt associated with completion of this TD.
    uint8_t delay_interrupt;

    // Two bit field used to generate/compare the data PID value (DATA0 or DATA1). It is updated
    // after each successful transmission/reception of a data packet. The MSb of this field is '0'
    // when the data toggle value is acquired from the toggle_carry fieldin the ED and '1' when the 
    // data toggle value is taken from the LSb of this field.
    uint8_t data_toggle;

    // For each transmission error, this value is incremented. If error_count is 2 and another error
    // occurs, the error type is recorded in the ConditionCode field and placed on the done queue.
    // When a transaction completes without error, ErrorCount is reset to 0.
    uint8_t error_count;

    // Contains the status of the last attempted transaction.
    uint8_t condition_code;

    // Contains the physical address of the next memory location that will be accessed for transfer 
    // to/from the endpoint. A value of 0 indicates a zero-length data packet or that all bytes have
    // been transferred.
    uint8_t *current_buffer_pointer;

    // Contains the address of the last byte in the buffer for this TD.
    uint8_t *buffer_end;

    struct TransferDescriptor *prev;
    struct TransferDescriptor *next;
} TD_T;


typedef struct EndpointDescriptor
{
    // USB address of the function containing the endpoint that this ED controls
    uint8_t function_address;

    // USB address of the endpoint within the function
    uint8_t endpoint_number;

    // MISSING: endp direction

    // Indicates the speed of the endpoint: full-speed (S=0), low-speed (S=1)
    SPEED_T speed;

    // When this bit is set, the HC continues on to the next ED on the list without attempting 
    // access to the TD queue or issuing any USB token for the endpoint
    BOOL_T skip;

    // MISSING: format

    // Indicates the maximum number of bytes that can be sent to or received from the endpoint in a 
    // single data packet
    uint16_t maximum_packet_size;

    // The data toggle carry bit. Whenever a TD is retired, this bit is written to contain the last 
    // data toggle value (LSB of data toggle field) from the retired TD.
    BOOL_T toggle_carry;

    // Bit set by the HC to indicate that processing of the TD queue on the endpoint is halted, 
    // usually due to an error in processing a TD.
    BOOL_T halted;

    // Points to the next TD to be processed for this endpoint (if not NULL).
    TD_T *head;
    TD_T *tail;

    struct EndpointDescriptor *prev;

    // Points to next ED in the list, if not NULL.
    struct EndpointDescriptor *next;
} ED_T;

*/



// -----------
// - PACKETS -
// -----------


typedef struct TokenPacket
// 8 + 7 + 4 + 5 = 24 bits (3 bytes) in buffer
{
    PID_T pid;
    uint8_t addr;
    uint8_t endp;
    uint8_t crc5;
} TOKEN_PACKET_T;


typedef struct DataPacket
// 8 + 64 + 16 = 88 bits max (11 bytes max) in buffer; 3 bytes minimum
{
    PID_T pid;
    uint8_t *data; // 8 bytes (max) -- NOTE: according to spec, must be integral num bytes
    uint16_t crc16;
    size_t payload_size; // in bytes -- NOTE: this field is not actually transmitted on the bus
} DATA_PACKET_T;


// ----------------
// - TRANSACTIONS -
// ----------------


typedef struct PtrNode TRANSACTION_NODE_T;


typedef struct Transaction
// One of the fundamental types of the USB protocol, consisting of a transfer type (control, 
// interrupt), a mandatory token packet and an optional data packet. The handshake is undetermined
// upon creation, and is specified when it becomes available.
{
    TRANSFER_TYPE_T transfer_type;
    TOKEN_PACKET_T *token_packet;
    DATA_PACKET_T *data_packet;
    PID_T handshake;
} TRANSACTION_T;


// ---------------
// - Descriptors -
// ---------------


typedef struct UsbDeviceDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    uint16_t bcd_usb;
    uint8_t b_device_class;
    uint8_t b_device_subclass;
    uint8_t b_device_protocol;
    uint8_t b_max_packet_size;
    uint16_t id_vendor;
    uint16_t id_product;
    uint16_t bcd_device;
    uint8_t i_manufacturer;
    uint8_t i_product;
    uint8_t i_serial_number;
    uint8_t b_num_configurations;
} USB_DEVICE_DESCRIPTOR_T;


typedef struct UsbConfigurationDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    uint16_t w_total_length;
    uint8_t b_num_interfaces;
    uint8_t b_configuration_value;
    uint8_t i_configuration;
    uint8_t bm_attributes;
    uint8_t b_max_power;
} USB_CONF_DESCRIPTOR_T;


typedef struct UsbInterfaceDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    uint8_t b_interface_number;
    uint8_t b_alternate_setting;
    uint8_t b_num_endp;
    uint8_t b_if_class;
    uint8_t b_if_subclass;
    uint8_t b_if_protocol;
    uint8_t i_if;
} USB_IF_DESCRIPTOR_T;


typedef struct UsbEndpointDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    uint8_t b_endp_address;
    uint8_t bm_attribute;
    uint16_t w_max_packet_size;
    uint8_t b_interval;
} USB_ENDP_DESCRIPTOR_T; // 8 bytes


typedef struct UsbFirstStringDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    uint16_t w_langid0;
    uint16_t w_langid1;
    uint16_t w_langid2;
} USB_FIRST_STRING_DESCRIPTOR_T; // 9 bytes


typedef struct UsbStringDescriptor
{
    uint8_t b_length;
    DESCRIPTOR_TYPE_T b_descriptor_type;
    char *b_string; // TODO: eventually, should be unicode
} USB_STRING_DESCRIPTOR_T; // 4 bytes


/**************/
/* PROTOTYPES */
/**************/

uint16_t bitstuff_buffer(uint8_t **buf_ptr, uint16_t buf_used_bit_len, STUFF_METHOD_T stuff_method);

// -----------
// - PACKETS -
// -----------


TOKEN_PACKET_T *TokenPacket(PID_T pid, uint8_t addr, uint8_t endp);
void DestroyTokenPacket(TOKEN_PACKET_T *token_packet);
DATA_PACKET_T *DataPacket(PID_T pid, size_t payload_size);
void DestroyDataPacket(DATA_PACKET_T *data_packet);


void packet_make_crc5(TOKEN_PACKET_T *token_packet);
uint8_t packet_validate_crc5(TOKEN_PACKET_T *token_packet);
uint16_t make_crc5(uint16_t input, uint8_t bit_count);
void packet_make_crc16(DATA_PACKET_T *data_packet);
uint8_t packet_validate_crc16(DATA_PACKET_T *data_packet);
uint16_t make_crc16(uint8_t* input, uint16_t bit_count);


// ----------------
// - TRANSACTIONS -
// ----------------


TRANSACTION_T *Transaction(TRANSFER_TYPE_T transfer_type, TOKEN_PACKET_T *tp, DATA_PACKET_T *dp);
void DestroyTransaction(TRANSACTION_T *transaction);
void DestroyWholeTransaction(TRANSACTION_T *transaction);
TRANSACTION_NODE_T *TransactionNode(TRANSACTION_T *trans, TRANSACTION_NODE_T *prev, 
                                    TRANSACTION_NODE_T *next);
void DestroyTransactionNode(TRANSACTION_NODE_T *transaction_node);


inline uint8_t flip_byte(uint8_t original_byte, uint8_t relevant_bits);
void transaction_place_in_buffer(TRANSACTION_T *transaction, uint8_t *buffer, 
                                 uint8_t *token_packet_length, uint8_t *data_packet_length);
void transaction_place_in_buffer_helper(uint8_t *buffer, uint8_t *data, uint16_t data_bits, 
                                        uint8_t *buf_byte, uint8_t *buf_bit);


// ---------
// - DEBUG -
// ---------

void send_debug();

#endif
