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


#define message_length_bytes 6
#define tickMessage 2000000   // notify the environment every tickMessage microseconds -> when still
#define tick 500000     // listen to the environment every tick microseconds -> when walking

#define WANDERING 0
#define STILL 1

int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();
    printf("init ok\n");
    

    int state = WANDERING;  // by default the robot starts walking

    // signal to send as the leader
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_twoThird );
    unsigned char message[] = "still";

    time_reference_t t0;
    uint32_t t1;
    while (1){
        pogobot_stopwatch_reset(&t0);

        if (state == WANDERING){
            // listen to signals:
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

                // alignment
                switch (ir_receiver_id)
                {
                    case 0: // front
                        /* code */
                        break;
                    case 1: // right
                        break;
                    case 2: // back
                        break;
                    case 3: // left
                        break;
                }
            }
        }
        // else {  // state = STILL

        // }

        pogobot_led_setColor(255,0,0);
        // send a message in every direction, with the id of the origin ir emitter
        pogobot_infrared_sendMessageAllDirectionWithId(0x1234, message, message_length_bytes);
        //printf("I am here!\n");
        pogobot_led_setColor(0,0,0);

        t1 = pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep((tick - t1)/1000);   // msleep in milliseconds
    }

}
