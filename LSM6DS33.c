/*
  MinIMU-9 v5
    https://www.pololu.com/product/2738
  Raspberry Pi I2C
	SDA = gpio pin2
	SCL = gpio pin3
    void i2c_init(void);
    void i2c_read(unsigned slave_address, char *data, int data_length);
    void i2c_write(unsigned slave_address, char *data, int data_length);
    i2c slave address:
      Below are the i2c addresses for https://www.pololu.com/product/2738
  	  LSM6DS33 (gyro and accelerometer)	0b1101011  0x6b
	  LIS3MDL (magnetometer)    		0b0011110  0x1e
      NOTE: The address for your sensor may be different! Read spec sheet and
      check for label on your part. (This helpful advice brought to you by
      Jenna Ruzekowicz)
 */
#include "timer.h"
#include "printf.h"
#include "i2c.h"
#include "LSM6DS33.h"

//This is 0x6A
/*const unsigned ls6mds33_address1 = 0b1101010;
//This is 0x6b
const unsigned lsm6ds33_address2 = 0b1101010; // this is for the gyro/accel;
*/
void lsm6ds33_write_reg(unsigned char reg, unsigned char v, const unsigned lsm6ds33_address) {
	char data[2] = {reg, v};
	i2c_write(lsm6ds33_address, data, 2);
}

unsigned lsm6ds33_read_reg(unsigned char reg, const unsigned lsm6ds33_address) {
	i2c_write(lsm6ds33_address, (void*) &reg, 1);
	unsigned char uc = 0;
	i2c_read(lsm6ds33_address, (void*) &uc, 1);
	return uc;
}

void lsm6ds33_init(const unsigned lsm6ds33_address) {
	lsm6ds33_write_reg(CTRL2_G, 0x80, lsm6ds33_address);   // 1600Hz (high perf mode)
	lsm6ds33_write_reg(CTRL1_XL, 0x80, lsm6ds33_address);  // 1600Hz (high perf mode)
}

unsigned lsm6ds33_get_whoami(const unsigned lsm6ds33_address) {
	// should return 0x69 ...
    return lsm6ds33_read_reg(WHO_AM_I, lsm6ds33_address);
}

// default is enabled
void lsm6ds33_enable_gyroscope(const unsigned lsm6ds33_address) {
	// gyroscope _G registers
	lsm6ds33_write_reg(CTRL10_C, 0x38, lsm6ds33_address);  // GYRO: x,y,z enabled (bits 4-6)
}

// default is enabled
void lsm6ds33_enable_accelerometer(const unsigned lsm6ds33_address) {
    // accelerator _XL registers
	lsm6ds33_write_reg(CTRL9_XL, 0x38, lsm6ds33_address);  // ACCEL: x,y,z enabled (bits 4-6)
}

void lsm6ds33_read_gyroscope(short *x, short *y, short *z, const unsigned lsm6ds33_address) {
    *x =  lsm6ds33_read_reg(OUTX_L_G, lsm6ds33_address);
    *x |= lsm6ds33_read_reg(OUTX_H_G, lsm6ds33_address) << 8;

    *y =  lsm6ds33_read_reg(OUTY_L_G, lsm6ds33_address);
    *y |= lsm6ds33_read_reg(OUTY_H_G, lsm6ds33_address) << 8;

    *z =  lsm6ds33_read_reg(OUTZ_L_G, lsm6ds33_address);
    *z |= lsm6ds33_read_reg(OUTZ_H_G, lsm6ds33_address) << 8;
}

void lsm6ds33_read_accelerometer(short *x, short *y, short *z, const unsigned lsm6ds33_address) {
    *x =  lsm6ds33_read_reg(OUTX_L_XL, lsm6ds33_address);
    *x |= lsm6ds33_read_reg(OUTX_H_XL, lsm6ds33_address) << 8;

    *y =  lsm6ds33_read_reg(OUTY_L_XL, lsm6ds33_address);
    *y |= lsm6ds33_read_reg(OUTY_H_XL, lsm6ds33_address) << 8;

    *z =  lsm6ds33_read_reg(OUTZ_L_XL, lsm6ds33_address);
    *z |= lsm6ds33_read_reg(OUTZ_H_XL, lsm6ds33_address) << 8;
}
