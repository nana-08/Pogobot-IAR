/**
 * POGOBOT
 *
 * Copyright © 2024 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/

/* 
Algorithm of line formation v1
> behavior of a follower: wandering
    - if it receives a signal, check the ir emitter, check its own ir receiver and tries to find the right orientation. then stays still and sends a signal
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pogobot.h"
#include "kalman.h"


#define message_length_bytes 6
#define tickMessage 2000000   // notify the environment every tickMessage microseconds -> when still
#define tickWander 500000   // move for a short period of time then stop
#define tick 2000000     // listen to the environment every tick microseconds -> when walking

#define WANDERING 0
#define STILL 1

int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();
    printf("init ok\n");
    msleep(1000);   // safe unplug
    

    // First of all: motor calibration
    int leftMotorVal;
    int rightMotorVal;

    pogobot_quick_calibrate(400, &leftMotorVal, &rightMotorVal);
    pogobot_led_setColor(0,255,0);
    msleep(500);
    pogobot_led_setColor(0,0,0);


    int state = WANDERING;  // by default the robot starts walking
    ir_direction dir;   // the direction to emit when still

    // signal to send as the leader
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_twoThird );
    unsigned char message[] = "still";

    time_reference_t t0;
    uint32_t t1;
    while (1){

        // stop and check your messages
        pogobot_motor_set(motorL,0); //motorR
        pogobot_motor_set(motorR,0); //motorL

        if (state == WANDERING){
            pogobot_stopwatch_reset(&t0);
            // listen to signals:
            pogobot_infrared_update();
            // if it hears a signal it tries to align with the emitter
            if ( pogobot_infrared_message_available() ) {
                message_t mr;
                // get the next message in the buffer and store it in mr
                pogobot_infrared_recover_next_message(&mr);
                
                // get the sender id of the message
                int sender_id = mr.header._sender_id;  
                // get the ir emitter of the sender
                int ir_emitter_id = mr.header._sender_ir_index;
                // get the ir I received it on
                int ir_receiver_id = mr.header._receiver_ir_index;    

                // alignment: nothing too crazy here, just emit from the ir opposed to the receiving ir for now
                switch (ir_receiver_id)
                {
                    case 0: // front => emit back
                        printf("signal received on IR captor front\n");
                        dir = 2;
                        break;
                    case 1: // right => emit left
                        printf("signal received on IR captor right\n");
                        dir = 3;
                        break;
                    case 2: // back => emit front
                        printf("signal received on IR captor back\n");
                        dir = 0;
                        break;
                    case 3: // left => emit right
                        printf("signal received on IR captor left\n");
                        dir = 1;
                        break;
                }

                // no longer wandering
                state = STILL;
                printf("state => still\n");
            } 
            else {  // no signal, the robot just moves for tickWander period of time then stops
                printf("no signal\n");

                pogobot_motor_jump_set(motorL, leftMotorVal);
                pogobot_motor_jump_set(motorR, rightMotorVal); 

                time_reference_t t2;
                pogobot_stopwatch_reset(&t2);
                uint32_t t3;
                while (1){
                    t3 = pogobot_stopwatch_get_elapsed_microseconds(&t2);
                    if (t3 >= tickWander){
                        break;
                    }
                }
                
                pogobot_motor_set(motorL,0);
                pogobot_motor_set(motorR,0); 
            }


            pogobot_led_setColor(0,0,255);
            pogobot_led_setColor(0,0,0);

            t1 = pogobot_stopwatch_get_elapsed_microseconds(&t0);
            msleep((tick - t1)/1000);   // msleep in milliseconds
        }
        else {  // state = STILL
            // same behavior as the leader except for the directions of the messages
            pogobot_stopwatch_reset(&t0);

            pogobot_led_setColor(255,0,0);
            pogobot_infrared_sendMessageOneDirection(dir, 0x1234, message, message_length_bytes);
            //printf("I am here!\n");
            pogobot_led_setColor(0,0,0);

            t1 = pogobot_stopwatch_get_elapsed_microseconds(&t0);
            msleep((tickMessage - t1)/1000);   // msleep in milliseconds
        }
    }

}
