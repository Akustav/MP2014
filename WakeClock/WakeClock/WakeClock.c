/*
 * WakeClock.c
 *
 * Created: 15.05.2014 9:21:27
 *  Author: Taavi
 */ 

#define DB7 PC0
#define DB6 PC1
#define DB5 PC2
#define DB4 PC3
#define DB3 PC4
#define DB2 PC5
#define DB1 PC6
#define DB0 PC7
#define E PE0
#define RW PD6
#define RS PD7
#define F_CPU 2000000

#define RS1 PORTD |= 1<<7
#define RS0 PORTD &= ~(1<<7)
#define RW1 PORTD |= 1<<6
#define RW0 PORTD &= ~(1<<6)
#define E1 PORTE |= 1
#define E0 PORTE &= ~1

#define NO_CURSOR 0b00001100
#define HOME 0b00000010
#define CLEAR 0b00000001

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "i2c.h"

void e_toggle(){
	E1;
	_delay_us(2);
	E0;
	_delay_us(40);
}

void lcd_data(unsigned char b) {
	
	//Ümber pööramine
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

	PORTC = b;
}

void lcd_init(){
	
	DDRD |= (1<<7) | (1<<6);
	DDRE |= 1;
	DDRC = 0xff;
	
	RS0;
	RW0;
	lcd_data(0b00111000);
	
	e_toggle();
	e_toggle();
	
	lcd_data(0b00001111);
	e_toggle();
	
	lcd_data(0b00000001);
	_delay_us(2);
	e_toggle();
	_delay_ms(2);
	
	lcd_data(0b00000110);
	e_toggle();
	_delay_ms(2);
	
}
void send_out(char c, uint8_t rs){
	
	if(rs) RS1;
	
	else RS0;
	
	RW0;
	lcd_data(c);
	e_toggle();
}

void write_str(char data[]){
	
	uint8_t i = 0;
	
	while(data[i] != '\0'){
		send_out(data[i],1);
		i++;
	}
	
}
int main(void)
{	
	init_RTC();
	
	lcd_init();
	send_out(NO_CURSOR, 0);
	
	char str[3];
	
	while(1){
		
		send_out(HOME, 0);
		
		i2c_read_time();
		
		str[2] = '\0';
		str[1] = (sekundid & 0x0f) + '0';
		str[0] = ((sekundid >> 4) & 0x03) + '0';
		
		write_str(str);
		write_str("string");
		
		_delay_ms(100);	
		
		send_out(CLEAR, 0);
		_delay_ms(5);
		
	}
	
	while(1){}
	
}