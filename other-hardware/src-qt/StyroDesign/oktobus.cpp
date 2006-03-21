
#include "ppi.h"
#include "oktobus.h"

Oktobus::Oktobus(){
	stepcount[0] = 0;
	stepcount[1] = 0;
	stepcount[2] = 0;
}

int Oktobus::init(QString pardev){
	fd = ppi_open(pardev.toAscii().data());
	if (fd < 0)
		return 1;
	ppi_setall(fd, PPIDATA, 0);
	return 0;
}

uint8_t Oktobus::readInputs(){
	uint8_t tmp;
	
	ppi_setall(fd, 15, PPICTRL);
	tmp = ppi_getall(fd, PPISTATUS);
	tmp >>=4;
	
	ppi_setall(fd, 15, PPICTRL);
	tmp |= ppi_getall(fd, PPISTATUS) & 0xf0;
	
	tmp ^= 0x88;
	
	return tmp;
}

const static uint8_t motor_sequence[4] = {0, 1, 3, 2};

void Oktobus::motorStep(uint8_t motor, bool dir){
	if(dir){
		stepcount[motor] += 1;
	}else{
		stepcount[motor] -= 1;
	}
	switch (motor){
		case 0:
			emit motor0Changed(stepcount[motor]);
			break;
		case 1:
			emit motor1Changed(stepcount[motor]);
			break;
		case 2:
			emit motor2Changed(stepcount[motor]);
			break;
		
	}
	ppi_setmask(fd, PPIDATA, 0x03 << (2*motor), (motor_sequence[(unsigned char)stepcount[motor]%4]) << (2*motor) );
}

void Oktobus::setOutput(uint8_t output, bool value) {
	if(value){
		ppi_set(fd, PPIDATA, output);
	}else{
		ppi_clr(fd, PPIDATA, output);
	}
}
