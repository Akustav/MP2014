/*
 * i2c.h
 *
 * Created: 4.06.2014 16:12:29
 *  Author: Taavi
 */
#ifndef I2C_H
#define I2C_H 

#include <avr/io.h>

#define SLA_W 0b11011110
#define SLA_R 0b11011111
#define F_CPU 2000000

volatile uint8_t sekundid;
volatile uint8_t minutid;
volatile uint8_t tunnid;

void init_RTC();
void set_time(uint8_t min, uint8_t h);
void i2c_start();
void i2c_stop();
void i2c_slaw();
void i2c_slar();
uint8_t i2c_read();
void i2c_write(uint8_t byte);
void i2c_read_time();

#endif
