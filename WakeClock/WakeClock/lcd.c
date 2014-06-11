/*
 * lcd.c
 *
 * Created: 10.06.2014 15:15:21
 *  Author: Taavi Adamson, Allan Kustavus
 */ 
#include "lcd.h"
#include <util/delay.h>

void e_toggle(){
	E1;
	_delay_us(2);
	E0;
	_delay_us(40);
}

void lcd_data(unsigned char b) {
	
	// Baidi ümber pööramine
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

	PORTC = b;
}

void cursor_place(uint8_t x){
	
	send_out(HOME, 0);
	
	for(uint8_t i = 0; i<x; i++){
		send_out(SHIFTC_R, 0);
	}
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
	
	send_out(NO_CURSOR, 0);
	
}

// Universaalne data saatmine: 0 - command | 1 - char
void send_out(char c, uint8_t rs){
	
	if(rs) RS1;
	
	else RS0;
	
	RW0;
	lcd_data(c);
	e_toggle();
}

void write_str(volatile char data[]){
	
	uint8_t i = 0;
	
	while(data[i] != '\0'){
		send_out(data[i],1);
		i++;
	}
}
	
