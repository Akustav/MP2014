/*
 * WakeClock.h
 *
 * Created: 10.06.2014 15:21:16
 *  Author: Taavi
 */ 
#ifndef WAKE_H
#define WAKE_H

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

#define F_CPU 2000000

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

void display_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place);
void joystick_init();
void time_setup();
void wait_release();

#endif


