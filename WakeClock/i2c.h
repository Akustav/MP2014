/*
 * i2c.h
 *
 * Created: 4.06.2014 16:12:29
 *  Author: Taavi Adamson, Allan Kustavus
 */
#ifndef I2C_H
#define I2C_H 

#define SLA_W 0b11011110
#define SLA_R 0b11011111

volatile uint8_t sekundid;
volatile uint8_t minutid;
volatile uint8_t tunnid;

uint8_t i2c_read();

/**
 * Initialises the RTC clock
 */
void init_RTC();

/**
 * Sets the time of the RTC
 * @param Sets hours
 * @param Sets minutes
 */
void set_time(uint8_t min, uint8_t h);

/**
 * Starts the RTC
 */
void i2c_start();

/**
 * Stops the RTC
 */
void i2c_stop();

/**
 * Sends info that you are writing to the interface
 */
void i2c_slaw();

/**
 * Sends info that you are reading from the interface
 */
void i2c_slar();

/**
 * Writes a byte onto the I2C interface
 * @param Byte to write
 */
void i2c_write(uint8_t byte);

/**
 * Reads the time from the RTC
 */
void i2c_read_time();

#endif
