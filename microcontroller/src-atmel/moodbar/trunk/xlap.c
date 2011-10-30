
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"
#include "xlap.h"
#include "tlv5604.h"

#include "pl_dim.h"
#include "mood.h"

can_addr myaddr;

/*

enum lapDataType{lapUint8 = 1, lapUint16 = 2, lapDir = 0x80};

enum lapRequestType{read = 1, write = 2, getName = 3};

enum lapSdoFlags{isReadable=0x01, isWriteable=0x02, hasName=0x08 };


typedef struct{
	enum lapDataType type;
	uint8_t flags;
	uint32_t data;
}SdoObjectSimpleRam;

typedef struct{
	uint16_t size;	
	uint16_t entries[];
}SdoDir;


typedef struct{
	uint16_t identifier;
	uint16_t (*access)(uint8_t requestType, uint8_t dataType, void * data);
	void * data;
}SdoTableEntry;



SdoDir rootObject = { 0x02, {0x8500, 0x8501}};
SdoObjectSimpleRam lObject = {lapUint16, 0x09, 0x55AA};

static SdoTableEntry sdoTable[]={
	{0, &accessDir, rootObject},
	{0x8500, &accessSimpleRam, rootObject},
};

static can_message_t sdo_reply_msg = {0, 0, PORT_SDO, PORT_SDO,};


uint16_t accessDir(uint8_t requestType, uint8_t dataType, void * data){
	if(requestType != read)
		return errorUnsupported;
	
	if (dataType != lapDir){
		if(dataType == 0){
			sdoInfoMessage(0x80, );
		}else{
			return errorDatatype;		
		}
	} 
	SdoDir * dir = data;
		
}





// looks if a matching identifier is in our table, and returns its object
SdoObject * getSdoObject(uint16_t identifier){
	uint8_t x;
	for(x = 0;x < 2; x++){
		if(sdoTable[x].identifier == identifier){
				return sdoTable[x].object;
			}
	}
}

void process_sdo_msg(){
	static uint8_t sdoState = 0
	if(rx_msg.dlc<3) 
		return;
	uint8_t requestType = rx_msg.data[0];
	uint16_t identifier = *(uint16_t*)rx_msg.data[1];
	
	uint8_t dataType;
	if(rx_msg.dlc<4){
		dataType = 0;
	}else{
		dataType = rx_msg.data[3];	
	}
	
	SdoObject *object = getSdoObject(identifier);
	if(!object){
		sdo_return_error(ErrorIdentifier);
	}
	
	

	switch(requestType){
		case read:{
{
				
			}
		}break;
			
		
	}
	
}


*/

void process_mgt_msg(){
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	switch(rx_msg.data[0]){
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
	}	
}

void process_lamp_msg(){
	switch(rx_msg.data[0]){
		case FKT_LAMPE_SET:
			lampe_set(rx_msg.data[1]%8, rx_msg.data[2]);
			break;
		case FKT_LAMPE_ADD:
			lampe_dim(rx_msg.data[1]%8, rx_msg.data[2]);
			break;
	}
}

void process_mood_msg(){
	uint8_t module;
	switch(rx_msg.data[0]){
		case FKT_MOOD_SET://2
			module = rx_msg.data[1];
			switch (module){
				case 0:
					da_set (0, rx_msg.data[2]<<2);
					da_set (1, rx_msg.data[3]<<2);
					da_set (2, rx_msg.data[4]<<2);
					da_set (3, rx_msg.data[5]<<2);
					break;
				case 1:
					//da_set1 (0, rx_msg.data[2]<<2);
					//da_set1 (1, rx_msg.data[3]<<2);
					//da_set1 (2, rx_msg.data[4]<<2);
					//da_set1 (3, rx_msg.data[5]<<2);
					break;
			}
			break;
		case FKT_MOOD_ONOFF://3
			if(rx_msg.data[1]){
				AvrXRunTask(TCB(moodtask));	
			}else{
				AvrXTerminate(PID(moodtask));
			}			
			break;
	}
}

AVRX_GCC_TASKDEF(laptask, 50, 3)
{
    while (1)
    {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr){
			if(rx_msg.port_dst == PORT_MGT){
				process_mgt_msg();	
			}else if(rx_msg.port_dst == PORT_MOOD){
				process_mood_msg();
			}else if(rx_msg.port_dst == PORT_LAMPE){
				process_lamp_msg();
//			}else if(rx_msg.port_dst == PORT_SDO){
//				process_sdo_msg();
			}
		}
    }
};

void xlap_init(){
	myaddr = eeprom_read_byte(0x00);
//	sdo_reply_msg.addr_src = myaddr;
	spi_init();
	can_init();
}
