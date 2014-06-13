/*
 * WakeClock.h
 *
 * Created: 10.06.2014 15:21:16
 *  Author: Taavi Adamson, Allan Kustavus
 */ 
#ifndef WAKE_H
#define WAKE_H

#define E PE0
#define RW PD6
#define RS PD7

#define RS1 PORTD |= 1<<7
#define RS0 PORTD &= ~(1<<7)
#define RW1 PORTD |= 1<<6
#define RW0 PORTD &= ~(1<<6)
#define E1 PORTE |= 1
#define E0 PORTE &= ~1

#define F_CPU 2000000

#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t a_active;
volatile uint8_t a_minutid;
volatile uint8_t a_tunnid;
volatile uint8_t last_minute;

void display_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place);
void display_setup_time(uint8_t sec, uint8_t min, uint8_t h, uint8_t place, uint8_t active);
void joystick_init();
void time_setup();
void wait_release();

#endif


