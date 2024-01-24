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

    int neighbor = -1;

    // signal to send as the leader
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_oneThird );
    unsigned char message[] = "still";

    int nb_neighbors = 0;
    int state = LISTENING;
    
    ir_direction dir = -1;   // direction to emit in
    time_reference_t t0;
    uint32_t t1;
    while (1){
        pogobot_stopwatch_reset(&t0);

        if (nb_neighbors != 2){
            printf("je n'ai pas encore 2 voisins\n");

            if (state == LISTENING){
                // listen to signals:
                pogobot_infrared_update();
                // if it hears a signal it tries to align with the emitter
                if ( pogobot_infrared_message_available() ) {
                    printf("je reçois un message\n");
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


                    // checking if the message content is correct
                    if (strcmp(payload, "still") == 0) {
                        
                        // checking the neighbors
                        if (neighbor != -1){    // a neighbor has already been noticed previously
                            if (neighbor != sender_id){ // it is a new robot => current robot is inside the line and no longer at the extremity
                                printf("c'est un second voisin\n");
                                // stops emitting
                                nb_neighbors = 2;
                                pogobot_led_setColor(255,0,0);
                                continue;
                            }
                        }
                        else {  // there was no neighbors yet
                            printf("c'est mon premier voisin\n");
                            neighbor = sender_id;   // register the new and only neighbor
                            nb_neighbors = 1;
                            pogobot_led_setColor(0,0,255);

                            // 1 neighbor => someone is following the leader, the leader starts emitting in the opposite direction
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
                        }
                    } 
                } 
                
                
                if (nb_neighbors == 1){
                    state = SENDING_ONE_DIRECTION;
                } else {
                    state = SENDING_ALL_DIRECTIONS;
                }
            } else {
                if (state == SENDING_ONE_DIRECTION){
                    printf("j'envoie dans la direction %d\n", dir);
                    pogobot_led_setColor(0,0,255);
                    pogobot_infrared_sendMessageOneDirection(dir, 0x1234, message, message_length_bytes);
                    state = LISTENING;
                } else {
                    pogobot_led_setColor(0,255,0);
                    // send a message in every direction, with the id of the origin ir emitter
                    pogobot_infrared_sendMessageAllDirectionWithId(0x1234, message, message_length_bytes);
                    //printf("I am here!\n");
                    state = LISTENING;
                }
            }
        } else {
            pogobot_led_setColor(255,0,0);
        }
        

        t1 = pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep((tick - t1)/1000);   // msleep in milliseconds
    }

}
