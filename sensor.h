//ifndef SENSOR_H

/* Functions to read input from the TOF sensors and translate those into actions of the paddles on the screen.*/
#include "gl.h"
#include "gpio.h"
#include "printf.h"
#include "i2c.h"
#include "LSM6DS33.h"

typedef struct{
        int x, y;
        int dx, dy; 
} sensor_object_t;

unsigned int get_distnace(void);

sensor_object_t sensor_init(const unsigned lsm6sd33_address);

sensor_object_t sonar_init();

sensor_object_t updatesonarpos(sensor_object_t player);

sensor_object_t updateposition(sensor_object_t player, const unsigned lsm6ds33_address);

//#endif
