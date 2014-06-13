/*
 * lcd.h
 *
 * Created: 10.06.2014 15:19:07
 *  Author: Taavi Adamson, Allan Kustavus
 */ 
#ifndef LCD_H
#define LCD_H
#include <avr/io.h>

#define NO_CURSOR 0b00001100
#define HOME 0b00000010
#define CLEAR 0b00000001
#define SHIFTC_R 0b00010100

/**
 * Prints input time onto the screen
 * @param Number representing seconds (0-59)
 * @param Number representing minutes (0-59)
 * @param Number representing hours (0-23)
 * @param The position where the time is being displayed on screen
 */
void e_toggle();

/**
 * Mirrors input bytes
 * @param Input char
 */
void lcd_data(unsigned char b); 

/**
 * LCD initialisation
 */
void lcd_init();

/**
 * Sends data to the display
 * @param Message to be sent
 * @param 0 If it is a command, 1 if it is a string to be displayed
 */
void send_out(char c, uint8_t rs);

/**
 * Writes string onto the display
 * @param Char array
 */
void write_str(volatile char data[]);

/**
 * Defines where the cursor is located on screen
 * @param Position of the cursor
 */
void cursor_place(uint8_t x);

#endif