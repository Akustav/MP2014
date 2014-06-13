/*
 * WakeClock.c
 *
 * Created: 15.05.2014 9:21:27
 *  Author: Taavi Adamson, Allan Kustavus
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include <stdio.h>
#include "WakeClock.h"
#include "lcd.h"

void alarm_init(){
	
	DDRB = (1 << PB6);
	
	a_active = 0;
	a_minutid = 0;
	a_tunnid = 12;
	
}
void joystick_init(){
	
	MCUCR |= (1<<JTD);
	MCUCR |= (1<<JTD);
	
	DDRF = 0;
	PORTF = ~((1 << PF0) | (1 << PF1) | (1 << PF2));
	
}

void alarm(){
	
	PORTA = 0xff;
	
	send_out(HOME, 0);
	
	write_str("!!!!!");
	
	display_time(0,a_minutid,a_tunnid,5);
	cursor_place(10);
	write_str("!!!!!!");
	cursor_place(40);
	write_str("!!!!!!!!!!!!!!!!");
	
	uint8_t m2 = 1;
	uint16_t m1 = 0;
	
	while(1){
		
		if (~PINF & (1<<PF6)){
			
			PORTA = 0;
			a_active = 0;
			PORTB = ~(1 << PB6);
			send_out(CLEAR, 0);
			wait_release();
			
			last_minute = minutid - 1;
			
			break;
		}
		
		if(m2){
			
			if(m1 < 0xFFFF){
				m1 ++;
				PINB = (1 << PB6);
				PINA = 0xff;
				_delay_ms(100);
				}else{
				m1 = 0;
				m2 = 0;
			}
			
		}else{
				
			if(m1 < 0xFFFF){
				m1 ++;
				PINB = (1 << PB6);
				PINA = 0xff;
				_delay_ms(1);
				}else{
				m1 = 0;
				m2 = 1;
			}
		}
		
	}
}

void alarm_setup(){
	
	send_out(CLEAR,0);
	_delay_ms(10);
	
	uint8_t active = 1;

	while(1){
		
		i2c_read_time();
		display_setup_time(sekundid, a_minutid, a_tunnid, 45, active);
		
		if (~PINF & (1<<PF5)){
			active = 1;
		}
		if (~PINF & (1<<PF3)){
			active = 0;
		}
		if(~PINF & (1<<PF6)){
			
			send_out(CLEAR,0);
			a_active = 1;
			wait_release();
			
			last_minute = minutid - 1;
			break;
		}
		
		if (active == 0){
			
			if(~PINF & (1<<PF4)){
				
				if (a_minutid == 0){
					a_minutid = 59;
					} else {
					a_minutid = a_minutid - 1;
				}
			}
			
			if(~PINF & (1<<PF7)){
				
				if (a_minutid == 59){
					a_minutid = 0;
					} else {
					a_minutid = a_minutid + 1;
				}
				
			}
			} else {
			
			if(~PINF & (1<<PF4)){
				
				if (a_tunnid == 0){
					a_tunnid = 23;
					} else {
					a_tunnid = a_tunnid - 1;
				}
			}
			
			if(~PINF & (1<<PF7)){
				
				if (a_tunnid == 23){
					a_tunnid = 0;
					} else {
					a_tunnid = a_tunnid + 1;
				}
			}
			
		}
		
		wait_release();
		
	}
	
}

void time_setup(){
	
	send_out(CLEAR,0);
	_delay_ms(10);
	
	uint8_t min = minutid;
	uint8_t h = tunnid;
	uint8_t active = 1;

	while(1){

		i2c_read_time();
		display_setup_time(sekundid, min, h, 5, active);
		
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
			
			last_minute = minutid - 1;
			break;
		}
		
		if (active == 0){
			
			if(~PINF & (1<<PF4)){
				
				if (min == 0){
					min = 59;
					} else {
					min = min - 1;
				}
			}
			
			if(~PINF & (1<<PF7)){
				
				if (min == 59){
					min = 0;
					} else {
					min = min + 1;
				}
				
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
		
	}
}

void display_digits(uint8_t x){
	
	char str[3];
	
	if(x < 10){
		str[0] = '0';
		str[1] = x + '0';
		
		} else {
		
		sprintf(str, "%d", x);
	}
	
	str[2] = '\0';
	write_str(str);
}

void display_setup_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place, uint8_t active){
	
	cursor_place(place);
	
	if(active == 0){
		
		display_digits(h);
		write_str(":");
		
		if(sec % 2 == 0){
			write_str("  ");
			
		} else {
			
			display_digits(min);
		}
		
	} else {
		
		if(sec % 2 == 0){
			write_str("  ");
			
		} else {
			display_digits(h);
		}
		
		write_str(":");
		display_digits(min);		
	}
	
}
void display_alarm_status(){
	
	cursor_place(3);
	
	if(a_active){
		write_str("*");
		cursor_place(11);
		write_str("*");
		
		cursor_place(49);
		write_str("A:");
		display_digits(a_tunnid);
		write_str(":");
		display_digits(a_minutid);
		
	} else {
		
		write_str(" ");
		cursor_place(11);
		write_str(" ");
		
		cursor_place(49);
		write_str("       ");
		
	}
	
}

void display_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place){
	
	if(last_minute != minutid){
		
		cursor_place(place);
		display_digits(h);
		
		cursor_place(place+3);
		display_digits(min);
		
		last_minute = minutid;
		
	}
	
	cursor_place(place+2);
	if(sec % 2 == 0){
		write_str(":");
		} else{
		write_str(" ");
	}
	
}

void wait_release(){
	
	while((PINF & 0xF8 )!= 0xF8){
		_delay_ms(10);
	}
	PORTA = 0;
}
void check_input(){
	
	if(~PINF & (1<<PF6)){
		wait_release();
		time_setup();
		
	}
	
	if((~PINF & (1<<PF3)) || (~PINF & (1<<PF5))){
		wait_release();
		alarm_setup();
		
	}
	
	if(~PINF & (1<<PF7)){
		wait_release();
		a_active = 1;
		
	}
	
	if(~PINF & (1<<PF4)){
		wait_release();
		a_active = 0;
		
	}
}

int main(void)
{	
	// Initsialiseerimised
	init_RTC();
	joystick_init();
	lcd_init();
	alarm_init();
	
	last_minute = minutid-1;
	
	while(1){
		
		send_out(HOME, 0);
		
		i2c_read_time();
		
		if(a_minutid == minutid && a_tunnid == tunnid && a_active){
			alarm();
		}
		
		display_alarm_status();
		display_time(sekundid, minutid, tunnid, 5);
		
		check_input();
		
		_delay_ms(100);	
		
	}
}