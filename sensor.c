#include "gpio.h"
#include "printf.h"
#include "i2c.h"
#include "LSM6DS33.h"
#include "sensor.h"
#include "gl.h"
#include "gpio_extra.h"
#include "uart.h"
#include "timer.h"
#include "armtimer.h"
#include "interrupts.h"
#include "ringbuffer.h"
#include <stdbool.h>

const unsigned int trigger = GPIO_PIN16;
const unsigned int echo = GPIO_PIN18;

extern int SCREENWIDTH;
extern int SCREENHEIGHT;
extern color_t PADDLE;             //color of paddle
extern color_t BACKGROUND;
float prev_time = 0;
rb_t* rb;  //ringbuffer for sensor values 
int prev_angle = 0;
const unsigned lsm6ds33_address = 0b1101011;
extern sensor_object_t person2;

sensor_object_t sensor_init(const unsigned lsm6ds33_address){
	lsm6ds33_init(lsm6ds33_address);	
        lsm6ds33_enable_gyroscope(lsm6ds33_address);
  //      rb = rb_new();
	sensor_object_t player;
	player.x = 0;
	player.y = SCREENHEIGHT/2; 

	player.dx = 0;
	player.dy = 0;

	return player;
}

sensor_object_t updateposition(sensor_object_t player, const unsigned lsm6ds33_address){
        
	int x;
        bool success = rb_dequeue(rb, &x);
        //dequeue until you are successful 
        while(!success) {
	    printf("nope");
	    success = rb_dequeue(rb, &x);
        }
	printf("dequeed\n");
	printf("angle is %d deg\n", x);


	sensor_object_t updatedplayer;
	updatedplayer.dx = 0;

	if (x>0){
		updatedplayer.dy = -7;
	}

	else if (x < 0) {
		updatedplayer.dy = +7;
	}
	else {
		updatedplayer.dy = 0;
	}

	updatedplayer.x = player.x;

	updatedplayer.y = player.y + updatedplayer.dy;
	prev_time = timer_get_ticks();
	return updatedplayer;

}
