

#include "usb_primitives.h"

uint8_t hc_do_transaction(TOKEN_PACKET_T *tp, DATA_PACKET_T *dp)
{
  uint8_t transfer_direction = 0;  // in or out ?
  uint8_t data_packet_length = 0;  //length of datapacket
  uint8_t handshake = 0;  //save the handshake here

    // NOTE: transient first state is idle line state. For low speed this is D- high, D+ low, which
    // corresponds to a differential zero.
    //#define transient_first_state 1
   
  //unsigned char iter;
     //size_t sie_valid_bytes;
    
    if (tp->PID == 0) {
        #ifdef DEBUG_HC
        {
            sprintf(print_buffer, "No executing transaction! Returning to caller.\n");
            send_debug();
        }
        #endif
        return executing_transaction;
    }

    // Calculate token packet CRC.
    packet_make_crc5(tp);

    // Check if we have a data packet.
    if (tp->pid == IN) {
      transfer_direction = RECEIVE;
    }
    elseif(tp->pid == OUT || tp->pid == SETUP)
    {
      // if the data packet length is non-zero, then we're transmitting
      transfer_direction = TRANSMIT;
      // calculate data packet crc
      packet_make_crc16(dp);
      // set data packet length in SIE
      data_packet_length = (BASE_DATA_PACKET_SIZE + dp->payload_size) * 8;
    }
    else {
      debug("hc_do_transaction: unknown PID");
    }

    
    // Based on the transfer type, call the appropriate SIE transfer function -- it will perform 
    // this transfer with the data that has just been set up in its buffer and internal variables.

            #ifndef NO_AVR_GCC
    sie_transaction(transfer_direction, tp, dp, data_packet_length, &handshake);
            #endif
    
    if(transfer_direction == RECEIVE)
      {
      if(packet_validate_crc16(dp))
	{
	  return 0; //transaction successful
	}
      }else
      {
	if(handshake == ACK)
	{
	  return 0; //transaction successful  
	}
      }

  return 1;  //Error
  

    #ifdef DEBUG_HC
    debug_buffer;
    #endif

    // Recall that the handshake result was already placed into the SIE (by the SIE) during the 
    // transfer.

    //return executing_transaction;

    #undef debug_buffer
    #undef transient_first_state
}


TRANSACTION_T *Transaction(TRANSFER_TYPE_T transfer_type, TOKEN_PACKET_T *tp, DATA_PACKET_T *dp)
// Constructor. Returns a pointer to a transaction that wraps the token packet, data packet, and
// encoding. Sets the handshake to undetermined.
{
    TRANSACTION_T *return_transaction;

    return_transaction = malloc(sizeof(TRANSACTION_T));
    // initialize it with the passed variables
    return_transaction->transfer_type = transfer_type;
    return_transaction->token_packet = tp;
    return_transaction->data_packet = dp;
    return_transaction->handshake = UNDETERMINED; // undetermined handshake type on creation
    return return_transaction;
}


void DestroyTransaction(TRANSACTION_T *transaction)
// Destructor. Frees the transaction pointer, leaving the packets unaffected.
// See the DestroyWholeTransaction destructor for the ability to destroy the packets as well.
{
    if (transaction) free(transaction);
}


void DestroyWholeTransaction(TRANSACTION_T *transaction)
// Destructor. Destroys the packets which constitute the transaction in addition to destroying
// the transaction itself.
{
    if (transaction->token_packet) DestroyTokenPacket(transaction->token_packet);
    if (transaction->data_packet) DestroyDataPacket(transaction->data_packet);
    DestroyTransaction(transaction);
}


TRANSACTION_NODE_T *TransactionNode(TRANSACTION_T *transaction, TRANSACTION_NODE_T *prev,
                                    TRANSACTION_NODE_T *next)
// Constructor. Returns a pointer to a transaction node that wraps transaction, prev, and next.
{
    TRANSACTION_NODE_T *new_transaction_node;

    new_transaction_node = malloc(sizeof(TRANSACTION_NODE_T));
    new_transaction_node->ptr = transaction;
    new_transaction_node->prev = prev;
    new_transaction_node->next = next;
    return new_transaction_node;
}


void DestroyTransactionNode(TRANSACTION_NODE_T *transaction_node)
// Destructor. Frees up the node memory, as transaction nodes are just wrappers for more useful
// transaction pointers. Does not destroy transactions.
{
    free(transaction_node);
}

DATA_PACKET_T *DataPacket(PID_T pid, size_t payload_size)
// Consutrctor. Returns a pointer to a data packet created with payload of the passed variable in
// bytes, as well as allocing space for the data.
{
    DATA_PACKET_T *new_packet;

    new_packet = malloc(sizeof(DATA_PACKET_T));
    // initialize it with the passed variables
    new_packet->pid = pid;
    new_packet->payload_size = payload_size;
    new_packet->data = malloc(sizeof(uint8_t) * payload_size);
    return new_packet;
}

TOKEN_PACKET_T *TokenPacket(PID_T pid, uint8_t addr, uint8_t endp)
// Constructor. Returns a pointer to a token packet created with the passed variables. crc5 and sync
// are not set.
{
    TOKEN_PACKET_T *new_packet;

    new_packet = malloc(sizeof(TOKEN_PACKET_T));
    // initialize it with the passed variables
    new_packet->pid = pid;
    new_packet->addr = addr;
    new_packet->endp = endp;
    return new_packet;
}

void DestroyTokenPacket(TOKEN_PACKET_T *token_packet)
// Destructor. Simply frees the memory allocated for the token packet.
{
    free(token_packet);
}

void DestroyDataPacket(DATA_PACKET_T *data_packet)
// Destructor. Frees the payload and the data packet itself.
{
    free(data_packet->data);
    free(data_packet);
}
