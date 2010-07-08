
#include "asn1.h"

#include "i2csw.h"
#define storage_read(address,len,data) i2cEeRead(data,address,len);
#define storage_write(address,len,data) i2cEeWrite(address,data,len);


//get offset and size of an object inside of a constructed object.
uint8_t asn1_get(asn1_obj_t * child, asn1_obj_t * parent, uint8_t id){
	uint16_t pos;
	uint16_t last;
	pos = parent->offset;
	last = pos + parent->size - 1;
	
	while(1){
		uint16_t size;
		uint8_t rb[2];
		storage_read(pos, 2, rb);
		pos+=2;
		
		if(rb[1]<128){
			size = rb[1];
		}else if(pos<last){
			if(rb[1] == 129 ){
				size = 0;
				storage_read(pos, 1, (uint8_t *) &size);
				pos+=1;
			}else if(rb[1] == 130 ){
				storage_read(pos, 2, (uint8_t *) &size);
				pos+=2;
			}else{
				return 0xff;
			}
		}else{
			return 0xff;
		}
		
		//check for overflow
		if((size + pos) < pos){
			return 0xff;
		}
		
		if((size + pos -1) > last){
			return 0xff;
		}
		
		if(rb[0]==id){
			child->offset = pos;
			child->size = size;
			return 0;
		}
		
		pos += size;
		if(pos > last){
			return 0xff;
		}
	}
}

//read data of an id in the given constructed object. returns size read or 0 on error.
uint16_t asn1_read(asn1_obj_t * parent, uint8_t id, uint8_t * buffer, uint16_t size){
	asn1_obj_t obj;
	if(asn1_get(&obj, parent, id)){
		return 0;
	}
	if(obj.size > size){
		return 0;
	}
	storage_read(obj.offset, obj.size, buffer);
	return obj.size;
}

//write data of an id in the given constructed object. returns size written or 0 on error.
uint16_t asn1_write(asn1_obj_t * parent, uint8_t id, uint8_t * buffer, uint16_t size){
	asn1_obj_t obj;
	if(asn1_get(&obj, parent, id)){
		return 0;
	}
	if(obj.size < size){
		return 0;
	}
	storage_write(obj.offset, size, buffer);
	return size;
}
