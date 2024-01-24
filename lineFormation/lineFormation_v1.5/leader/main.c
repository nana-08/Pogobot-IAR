/**
 * POGOBOT
 *
 * Copyright © 2022 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/

/* 
Algorithm of line formation v1
> behavior of the leader: staying still and sending signals
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pogobot.h"


#define message_length_bytes 6
#define tick 1000000   // the leader will notify the environment every tick microseconds

#define SENDING_ALL_DIRECTIONS 0
#define SENDING_ONE_DIRECTION 2
#define LISTENING 1

int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();
    printf("init ok\n");
    

    int state = LISTENING;

    ir_direction dir;   // direction to emit in
    // signal to send as the leader
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_oneThird );
    unsigned char message[] = "still";

    time_reference_t t0;
    uint32_t t1;
    while (1){
        pogobot_stopwatch_reset(&t0);

        pogobot_led_setColor(255,0,0);
        if (state == LISTENING){
            // if it hears a signal it tries to align with the emitter
            // listen to signals:
            pogobot_infrared_update();
            if ( pogobot_infrared_message_available() ) {
                printf("je reçois un msg\n");
                message_t mr;
                // get the next message in the buffer and store it in mr
                pogobot_infrared_recover_next_message(&mr);
                    
                // get the sender id of the message
                int sender_id = mr.header._sender_id;  
                // get the ir emitter of the sender
                int ir_emitter_id = mr.header._sender_ir_index;
                // get the ir I received it on
                int ir_receiver_id = mr.header._receiver_ir_index;    
                // get the payload of the message
                unsigned char *payload = mr.payload;


                // if someone is following the leader, it starts emitting in only one direction. Else it emits in all directions
                if (strcmp(payload, "still") == 0) {
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

                    state = SENDING_ONE_DIRECTION;
                } else {
                    state = SENDING_ALL_DIRECTIONS;
                }
            } else {
                state = SENDING_ALL_DIRECTIONS;
            }
        }
        else {
            if (state == SENDING_ALL_DIRECTIONS){
                // send a message in every direction, with the id of the origin ir emitter
                pogobot_infrared_sendMessageAllDirectionWithId(0x1234, message, message_length_bytes);
                //printf("I am here!\n");
                state = LISTENING;
            } else {
                pogobot_infrared_sendMessageOneDirection(dir, 0x1234, message, message_length_bytes);
                state = LISTENING;
            }
        } 
        

        pogobot_led_setColor(0,0,0);

        t1 = pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep((tick - t1)/1000);   // msleep in milliseconds
    }

}
