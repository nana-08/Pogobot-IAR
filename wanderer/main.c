/**
 * POGOBOT
 *
 * Copyright © 2022 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/

/* 
Border detection (Pogobots change color (Blue) if they have neighbors on all sides).
Green - 3 sides
Red - 2 sides
Yellow - 1 side
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pogobot.h"


#define message_length_bytes 100
#define F 30
#define HERE 1
#define MISSING 0

#define motorR 0
#define motorL 1
#define motorB 2

#define motorStop 0
#define motorQuarter 256
#define motorHalf 512
#define motorThreeQuarter 716
#define motorFull 1023

void pogobot_motor_jump_set(int power, int motor) {
    if (power > 512){
        pogobot_motor_set(power, motor);
    }
    else{
        pogobot_motor_set(512, motor);
        msleep(50);
        pogobot_motor_set(power, motor);
    }
}

void pogobot_set_direction_right(int power) {
    // pogobot_motor_set(0, motorR);
    pogobot_motor_jump_set(power, motorL);
}

void pogobot_set_direction_left(int power) {
    // pogobot_motor_set(0, motorL);
    pogobot_motor_jump_set(power, motorR);
}

int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();

    printf("init ok\n");

    // get a random seed from the Pogobot
    srand(pogobot_helper_getRandSeed());

    // set the infrared emitter power of the Pogobot -> the light will be more or less visible
    // pogobot_infrared_emitter_power_max - 3
    // pogobot_infrared_emitter_power_twoThird - 2
    // pogobot_infrared_emitter_power_oneThird - 1
    // pogobot_infrared_emitter_power_null - 0
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_twoThird );

    time_reference_t t0;
    uint32_t t1;

    while(1) {
        // we reset the timer
        pogobot_stopwatch_reset(&t0);

        // we choose a random motor between the left and the right
        int motor = rand()%2;

        // we choose a speed
        int speed = 512;

        // if (motor == motorR){
        //     printf("motor right\n");
        //     pogobot_set_direction_right(speed);
        //     int status = pogobot_motor_dir_status();
        //     printf("status : %d\n", status);
        // }
        // else{
        //     printf("motor left\n");
        //     pogobot_set_direction_left(speed);
        //     int status = pogobot_motor_dir_status();
        //     printf("status : %d\n", status);
        // }

        pogobot_motor_jump_set(speed, motor);
        // pogobot_motor_dir_set(speed, motor);
        // int status = pogobot_motor_dir_status();
        // printf("status : %d\n", status);
        printf("motor : %d\n", motor);

        // we wait for the next iteration to start
        // prevent the Pogobot from going too fast
        t1=pogobot_stopwatch_get_elapsed_microseconds(&t0);
        // we sleep for 2 seconds 
        msleep(2000);
    }

}


