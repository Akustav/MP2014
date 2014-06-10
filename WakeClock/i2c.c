/*
 * i2c.c
 *
 * Created: 4.06.2014 15:03:36
 *  Author: Taavi
 */ 

#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>

void i2c_write(uint8_t byte){
	
	TWDR = byte;
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	while (TWSR!=0x28);
}

void init_RTC(){
	
	TWBR = 9;
	TWSR &= 0xFC;
	DDRA = 0xff;
	
	i2c_start();
	i2c_slaw();
	i2c_write(0);
	i2c_write(0x80);
	i2c_write(89);
	i2c_write(17);
	i2c_stop();
	
	_delay_us(200);
	
}

void set_time(uint8_t min, uint8_t h){

	i2c_start();
	i2c_slaw();
	i2c_write(0);
	i2c_write(0x80);
	i2c_write(min);
	i2c_write(h);
	i2c_stop();
	
	_delay_us(200);
}

void i2c_start(){
	
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWEA);
	
	while(!(TWCR & (1 << TWINT)));
	
}

void i2c_restart(){
	
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWEA);
	
	while(TWSR != 0x10);
	
}

void i2c_stop(){
	
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_slaw(){
	
	TWDR = SLA_W;
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	while(TWSR != 0x18);
}

void i2c_slar(){
	
	TWDR = SLA_R;
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	while(TWSR != 0x40);
}

uint8_t i2c_readACK(){
	
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	while (TWSR!=0x50);
	
	return TWDR;
}

uint8_t i2c_readNACK(){
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while (TWSR!=0x58);
	
	return TWDR;
	
}
void translate_to_dec(){
	
	sekundid = (sekundid & 0x0f) + ((sekundid >> 4) & 0x07) * 10;
	minutid = (minutid & 0x0f) + ((minutid >> 4) & 0x07) * 10;
	tunnid = (tunnid & 0x0f)+ ((tunnid >> 4) & 0x03) * 10;
}

void i2c_read_time(){
	
	i2c_start();
	//PORTA = 1;
	i2c_slaw();
	//PORTA = 2;
	i2c_write(0);
	//PORTA = 4;
	i2c_restart();
	//PORTA = 8;
	i2c_slar();
	//PORTA = 16;
	sekundid = i2c_readACK();
	//PORTA = 32;
	minutid = i2c_readACK();
	//PORTA = 64;
	tunnid = i2c_readNACK();
	//PORTA = 128;
	i2c_stop();
	
	translate_to_dec();
}

