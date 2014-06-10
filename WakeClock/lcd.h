/*
 * lcd.h
 *
 * Created: 10.06.2014 15:19:07
 *  Author: Taavi
 */ 
#ifndef LCD_H
#define LCD_H

#define NO_CURSOR 0b00001100
#define HOME 0b00000010
#define CLEAR 0b00000001
#define SHIFTC_R 0b00010100

#include "WakeClock.h"

void e_toggle();
void lcd_data(unsigned char b); 
void lcd_init();
void send_out(char c, uint8_t rs);
void write_str(volatile char data[]);
void cursor_place(uint8_t x);

#endif