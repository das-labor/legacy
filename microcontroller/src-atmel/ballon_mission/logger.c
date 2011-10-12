
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "at45db.h"
#include "ioport.h"
#include "adc.h"

#define COMPASS_CS_PORT   B
#define COMPASS_CS_BIT    3

#define COMPASS_SCK_PORT  B
#define COMPASS_SCK_BIT   2

#define COMPASS_DATA_PORT B
#define COMPASS_DATA_BIT  1

typedef struct{
	uint16_t magic;
	uint16_t timestamp;
	uint16_t temperature_1;
	uint16_t temperature_2;
	uint16_t temperature_3;
	uint16_t pressure;
	uint16_t voltage;
	uint8_t compass_data[24];
}log_dataset_t;

uint8_t buffer_offset;
uint16_t current_page;

void log_dataset(log_dataset_t * data){
	static uint8_t buffer[240];
	memcpy(buffer + buffer_offset, data, 40);

	buffer_offset += 40;
	
	if(buffer_offset == 240){
		buffer_offset = 0;
		df_memory_page_programm(buffer, current_page);
		current_page++;
		if(current_page == 2048) current_page = 2047;
	}
}


uint8_t get_compass_byte(){
	uint8_t i;
	uint8_t b = 0;
	for(i=0;i<8;i++){
		OUTPUT_ON(COMPASS_SCK);
		b <<= 1;
		_delay_us(1);
		if(INPUT(COMPASS_DATA)){
			b |= 1;
		}
		OUTPUT_OFF(COMPASS_SCK);
		_delay_us(1);
	}
	return b;
}

void get_compass_data(uint8_t * data){
	OUTPUT_ON(COMPASS_CS);
	_delay_us(10);
	uint8_t i;
	for(i=0;i<24;i++){
		data[i] = get_compass_byte();
	}
	OUTPUT_OFF(COMPASS_CS);

	
	
}

extern uint16_t current_second;

void build_dataset(log_dataset_t * dataset){
	dataset->magic = 0x1337;
	dataset->timestamp =     current_second;
	dataset->temperature_1 = analogRead(0);
	dataset->temperature_2 = analogRead(1);
	dataset->temperature_3 = analogRead(2);
	dataset->pressure      = analogRead(7);
	dataset->voltage       = analogRead(6);
	get_compass_data(dataset->compass_data);
}


void print_temperature(uint16_t w){
	double d = w * 5;
	d /= 10.240;
	d -= 273.15 + 3.0;
	char buf[10];
	dtostrf(d, 6, 2, buf);
	printf("%s�C,", buf);
}


uint8_t undervoltage_counter;
uint8_t undervoltage;

void print_voltage(uint16_t w){
	double d = w;
	d *= (3.7 * 5.0 * 1.01 / 1024.0);
	
	if(d < 9.0){
		undervoltage_counter ++;
		printf("uc %d\r\n", undervoltage_counter);
		if(undervoltage_counter == 20){
			undervoltage = 1;
			
		}
	}else{
		undervoltage_counter = 0;
	}
	
	char buf[10];
	dtostrf(d, 6, 2, buf);
	printf("%sV,", buf);
}

void print_dataset(log_dataset_t * dataset){
	
	printf("%d,", dataset->timestamp);
	//printf("%d,", dataset->temperature_1);
	//printf("%d,", dataset->temperature_2);
	//printf("%d,", dataset->temperature_3);
	print_temperature(dataset->temperature_1);
	print_temperature(dataset->temperature_2);
	print_temperature(dataset->temperature_3);
	print_voltage(dataset->voltage);	
	printf("%d,", dataset->pressure);
	//printf("%d,", dataset->voltage);
	
	uint8_t j;
	uint8_t x=0;
	for(j=0;j<6;j++){
		uint8_t i;
		for(i=0;i<3;i++){
			printf("%02X", dataset->compass_data[x++]);
		}
		printf(",");
	
	}
	
	printf("\r\n");
}

void print_dataset_raw(log_dataset_t * dataset){
	printf("%d,", dataset->timestamp);
	printf("%d,", dataset->temperature_1);
	printf("%d,", dataset->temperature_2);
	printf("%d,", dataset->temperature_3);
	printf("%d,", dataset->pressure);
	printf("%d,", dataset->voltage);
	
	uint8_t j;
	uint8_t x=0;
	for(j=0;j<6;j++){
		uint8_t i;
		for(i=0;i<3;i++){
			printf("%02X", dataset->compass_data[x++]);
		}
		printf(",");
	
	}
	
	printf("\r\n");
}

void logger_init(){
	SET_DDR(COMPASS_SCK);
	SET_DDR(COMPASS_CS);
}

void logger_tick(){
	log_dataset_t dataset;
	build_dataset(&dataset);
	log_dataset(&dataset);
	print_dataset(&dataset);
}

void logger_show(){
	log_dataset_t dataset;
	build_dataset(&dataset);
	print_dataset(&dataset);
}

void dump_log(){
	uint16_t i;
	for(i=0;i<2048;i++){
		uint8_t j;
		uint8_t buffer[256];
		df_memory_page_read(buffer, i);
		for(j=0; j< 240; j+=40){
			print_dataset_raw((log_dataset_t *)&buffer[j]);
		}
	}
}