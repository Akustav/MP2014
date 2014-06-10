/*
 * WakeClock.c
 *
 * Created: 15.05.2014 9:21:27
 *  Author: Taavi
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include <stdio.h>
#include "WakeClock.h"
#include "lcd.h"

#define F_CPU 2000000

void joystick_init(){
	
	MCUCR |= (1<<JTD);
	MCUCR |= (1<<JTD);
	
	DDRF = 0;
	PORTF = ~((1 << PF0) | (1 << PF1) | (1 << PF2));
	
}

void time_setup(){
	
	send_out(CLEAR,0);
	_delay_ms(10);
	
	uint8_t min = minutid;
	uint8_t h = tunnid;
	uint8_t active = 1;
	
	while(1){
		
		min = minutid;
		h = tunnid;	
		
		PORTA = h;
		
		display_time(0, min, h, 5);
		
		if (~PINF & (1<<PF5)){
			active = 1;
		}
		if (~PINF & (1<<PF3)){
			active = 0;
		}
		if(~PINF & (1<<PF6)){
			
			send_out(CLEAR,0);
			set_time(min, h);
			wait_release();
			break;
		}
		
		//wait_release();
		
		if (active == 0){
			
			if(~PINF & (1<<PF4)){
				
				if (min == 0){
					min = 59;
					} else {
					min = min - 1;
				}
				//wait_release();
			}
			
			if(~PINF & (1<<PF7)){
				
				if (min == 59){
					min = 0;
					} else {
					min = min + 1;
				}
				//wait_release();
				
			} 
		} else {
			
			if(~PINF & (1<<PF4)){
				
				if (h == 0){
					h = 23;
					} else {
					h = h - 1;
				}
			}
			
			if(~PINF & (1<<PF7)){
				
				if (h == 23){
					h = 0;
					} else {
					h = h + 1;
				}
			}
		
		
		
		}
		
		wait_release();
		
		set_time(min, h);
		i2c_read_time();
		display_time(0, minutid, tunnid, 5);
		
		
	}
}

void display_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place){
	
	cursor_place(place);
	
	char str[3];
	
	if(h < 10){
		str[0] = '0';
		str[1] = h + '0';
		
	} else {
		
		sprintf(str, "%d", h);
	}
	
	str[2] = '\0';
	write_str(str);
	
	if(sec % 2 == 0){
		str[0] = ':';
		str[1] = '\0';
	} else{
		str[0] = ' ';
		str[1] = '\0';
	}
	
	write_str(str);
	
	
	if(min < 10){
		str[0] = '0';
		str[1] = min + '0';
		
		} else {
		
		sprintf(str, "%d", min);
	}
	
	str[2] = '\0';
	write_str(str);
	
}

void wait_release(){
	
	while(PINF != 0xFC){
		PORTA = PINF;
	}
	PORTA = 0;
}
int main(void)
{	
	init_RTC();
	joystick_init();
	lcd_init();
	
	send_out(NO_CURSOR, 0);
	
	while(1){
		
		send_out(HOME, 0);
		
		i2c_read_time();
		display_time(sekundid, minutid, tunnid, 5);
		
		if(~PINF & (1<<PF6)){
			
			wait_release();
			time_setup();
			
		}
		
		PORTA = PINF;
		
		_delay_ms(100);	
		
	}
}