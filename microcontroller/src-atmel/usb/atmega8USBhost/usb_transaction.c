

#include "usb_primitives.h"

#ifndef NO_AVR_GCC
void hc_transaction(uint8_t tokenPID, uint8_t packetPID, uint8_t addr, uint8_t endp, uint8_t *data, uint8_t *data_length)
{
  uint16_t CRCbuffer=0;
  uint8_t *tokenbuffer = 0;
  uint8_t *sendbuffer = 0;
  uint8_t *receivebuffer = 0;
  uint8_t receivedbytes = 0;

  //create a temporary buffer for the tocken packet
  tokenbuffer = malloc(sizeof(uint8_t) * (TOCKEN_PACKET_LENGTH + 1));
  *tokenbuffer = SYNC; //send sync first
  *(tockenbuffer+1)=tockenPID;  //send PID
  *(tockenbuffer+2)=(addr<<1)|(endp>>3);
  *(tockenbuffer+3)=(endp<<5)|make_crc5(((addr<<ENDP_BITS)|endp), ENDP_BITS + ADDR_BITS);  //two byte payload
  

  //create a temporary buffer for the handshake
  receivebuffer = malloc(sizeof(uint8_t) * 9); //max 9 bytes to receive : PID + DATA + CRC
  
  if((tokenPID == OUT) || (tockenPID == SETUP))
    {
      //lets create the packet
      sendbuffer = malloc(sizeof(uint8_t) * (*data_length +4));
      *sendbuffer = SYNC;  //send sync first
      *(sendbuffer+1) = packetPID;  //send PID
      if(*data_length > 0){
      memcpy((sendbuffer+2), data, *data_length); //payload
      *(sendbuffer+2+*data_length)=make_crc16(data,*data_length*CHAR_BITS); //2 byte CRC
      }
       #ifdef DEBUG_HC
      debug("sending token OUT & data packet");
       #endif
      sie_send(tokenbuffer, (TOCKEN_PACKET_LENGTH + 1)); //send the token
      
      if(*data_length > 0)
	{
	  sie_send(sendbuffer,(*data_length + 4)); //send the DATA
	}
      else
	{
	  sie_send(sendbuffer, 2); //send a ZLP, TODO: dont send CRC?
	}
      
      sie_receive(receivebuffer, &receivedbytes);  //receive handshake
      
      if(*receivebuffer != PID_T.ACK)
	{
	  #ifdef DEBUG_HC
	  debug("didn't received an ACK");
          #endif
	  free(tokenbuffer);
	  free(sendbuffer);
	  free(receivebuffer);
	  return TRANSACTIONERROR;
	}

      
      free(receivebuffer);
      free(tokenbuffer);
      free(sendbuffer);
    }
  elseif(tockenPID == IN)
    {
      //lets create the handshake packet
      sendbuffer = malloc(sizeof(uint8_t) * 2);  //SYNC + PID
      *sendbuffer = SYNC;  //send sync first
      *(sendbuffer+1) = PID_T.ACK;  //send ACK, well check the data later

      #ifdef DEBUG_HC
      debug("sending token IN");
      #endif
      sie_send(tokenbuffer, (TOCKEN_PACKET_LENGTH + 1)); //send the token
      
      sie_receive(receivebuffer, &receivedbytes);  //receive data or handshake
      
      if((*receivebuffer == PID_T.DATA0) ||( *receivebuffer == PID_T.DATA1))
	{
	  sie_send(sendbuffer,2);  //send the handshake (ACK)
	  if(receivedbytes >= 0) //check if we received something
	    {
	      if(*(receivebuffer+receivedbytes+1) == make_crc16(receivebuffer+1, receivedbytes - 3))  //check if the checksum is OK
		{
		  memcpy(data, receivebuffer+1, receivedbytes - 3); //make a copy of the data
		  (*data_length)+=(receivedbytes-3);  //save the received byte count
		  free(receivebuffer);
		  free(tokenbuffer);
		  free(sendbuffer);
		}
	      else
		{
		  #ifdef DEBUG_HC
		  debug("CRC16 on received packet failed");
		  #endif
		  free(receivebuffer);
		  free(tokenbuffer);
		  free(sendbuffer);
		  return TRANSACTIONERROR;
		}
	    }
	  else
	    {
	      #ifdef DEBUG_HC
	      debug("number of received bytes equals 0");
	      #endif
	      free(receivebuffer);
	      free(tokenbuffer);
	      free(sendbuffer);
	      return TRANSACTIONERROR;
	    }
	}
      else
	{
	  #ifdef DEBUG_HC
	  debug("received not a data packet");
	  #endif
	  free(receivebuffer);
	  free(tokenbuffer);
	  free(sendbuffer);
	  return TRANSACTIONERROR;
	}
    }
  else
    {
      #ifdef DEBUG_HC
      debug("unknown TOKENPID");
      #endif
      free(receivebuffer);
      free(tokenbuffer);
      free(sendbuffer);
      return TRANSACTIONERROR;
    }
  return TRANSACTIONSUCCESS;
}
#endif
