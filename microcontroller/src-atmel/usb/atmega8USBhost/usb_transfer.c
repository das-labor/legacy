uint8_t hc_do_transfer_interrupt(TRANSMISSION_DIRECTION_T direction, uint8_t *data, uint8_t *data_length, uint8_t addr,uint8_t endp)
{
  uint8_t i = 0;
  uint8_t retries = TRANSFER_MAX_RETRIES;
  uint8_t error = 0;
  uint8_t packetPID = 0;
  uint8_t length = 0;
  cli();

  if(direction == TRANSMIT)
    {
      tokenPID = PID_T.OUT;
      packetPID = PID_T.DATA1;

      if(*data_length >= 8){
	length=8;
	for(i = 0; i < *data_length; i+=8)
	  {
	    retries = TRANSFER_MAX_RETRIES;
	    while(!hc_transaction(tokenPID,packetPID, addr,endp, (data+i),&length) && (retries > 0))
	      {
		retries--;  //reduce retry-counter  
	      }
    
	    if(retries == 0){ // rechead the max retry-limit exit with error
	      sei();
	      return TRANSFERERROR;     //Error, to many retries needed
	    }
	    if(i%2 == 0) //alternate DATA1 and DATA0 packets
	      {
		packetPID = PID_T.DATA1;
	      }
	    else
	      {
		packetPID = PID_T.DATA0;
	      }
	  }
      }

      length = data_length % 8; 
      retries = TRANSFER_MAX_RETRIES;
      while(!hc_transaction(tokenPID,packetPID, addr,endp, (data+*data_length-length),&length) && (retries > 0))
	{
	  retries--;  //reduce retry-counter  
	}
      
      if(retries == 0){ // rechead the max retry-limit exit with error
	sei();
	return TRANSFERERROR;     //Error, to many retries needed
      }
    }
 elseif(direction == RECEIVE)
    {
      tokenPID = PID_T.IN;
      packetPID=0;
      
      do{
	retries =  TRANSFER_MAX_RETRIES;
	length=0;
        while(hc_transaction(tokenPID,packetPID, addr,endp, (data+i),&length) && (retries > 0)) && (retries > 0))
        {
	  retries--;
        }
	if(retries == 0){
	  sei();
	  return TRANSFERERROR;     //Error to many retries needed
	}
	*data_length+=length;  //increase the received byte cound
	
      }while(length > 0)    
    }
 else
    {
      sei();
      debug("hc_do_transfer_interrupt: unknown direction");
      return TRANSFERERROR;
    }	
  sei();
  return 0;

}

uint8_t hc_do_transfer(TRANSFER_TYPE_T transfer_type,TRANSMISSION_DIRECTION_T direction, uint8_t *data, uint8_t *datalength, uint8_t addr,uint8_t endp)
{
  uint8_t i = 0;
  uint8_t retries = TRANSFER_MAX_RETRIES;
  PID_T TPID = 0;
  PID_T DPID = 0;
  uint8_t error = 0;

  DATA_PACKET_T *DataPacket;
  TOKEN_PACKET_T *TokenPacket;
  
  if(transfer_type == CONTROL)
    {
      TPID = SETUP;
      DPID = DATA0;
      endp = 0;
    }
  elseif(transfer_type == INTERRUPT)
    {
      DPID=DATA1;
      if(direction == TRANSMIT)
        TPID = OUT;
      elseif(direction == RECEIVE)
	TPID = IN;
    }
  else
    {
      debug("hc_do_transfer: unknown transfer_type");
      return 1;
    }
  TokenPacket= TockenPacket(PID,addr,endp);
  for(i = 0; i < *data_length; i+=8)
    {
    DataPacket=DataPacket(*(data+i),8);
    retries =  TRANSFER_MAX_RETRIES;
    while(!hc_do_transaction(tp,dp) && retries > 0)
      {
	retries--;
      }
    
    if(retries == 0)
      error = 1;     //Error, to many retries needed

    }
  if(data_length % 8 > 0)
    {
      DataPacket=DataPacket(*(data+data_length-(*data_length%8)) ,*data_length % 8);
      retries =  TRANSFER_MAX_RETRIES;
      while(!hc_do_transaction(tp,dp) && retries > 0)
      {
	retries--;
      }
      if(retries == 0)
	 error = 1;     //Error to many retries needed   
    }

  DestroyDataPacket(DataPacket);
 


}
