/**
 * POGOBOT
 *
 * Copyright © 2024 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pogobot.h"


// Parameters
#define message_length_bytes 6
#define tickMessage 1000000   // notify the environment every tickMessage microseconds

// State
#define ANCHOR 0
#define BOAT 1

// Motor power
#define m motorThreeQuarter  // motor power

// Direction
#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2

// L'algorithme débute avec un robot initialisé en mode "anchor", tandis que les robots suivants sont en mode "boat". 
// L'ancrage débute avec le robot "anchor" envoyant des messages sur deux axes , devant lui et à sa gauche, 
// sans se déplacer. Les robots "boat" se déplacent de manière aléatoire dans l'environnement tout en cherchant 
// à recevoir des messages sur un axe spécifique.

// L'objectif principal est que les robots "boat" s'ancrent sur les messages émis par le robot "anchor", 
// formant ainsi une grille carrée. Les robots "boat" s'arrêtent lorsqu'ils reçoivent un message et envoient 
// un message sur leur axe frontal, à condition que le message provienne de la face gauche du robot "anchor". 
// Une fois cela accompli, le robot "boat" devient également un "anchor", envoie des messages sur ses axes, 
// et le processus se répète.

// Avec quatre robots, on peut former un carré de 2x2 robots, créant ainsi une boucle de messages qui permet 
// aux groupes de robots de confirmer leur connexion. Les robots restent immobiles mais envoient des messages sur 
// leurs axes droit et arrière, ce qui permet de former une grille de 4x4 robots. Ce schéma se répète jusqu'à ce 
// que la grille atteigne la taille de l'environnement.

// En résumé, cet algorithme utilise un robot "anchor" comme point de départ, 
// établissant une communication sélective avec les robots "boat" pour former progressivement une 
// grille carrée qui s'étend jusqu'à remplir tout l'environnement.

void pogobot_motor_jump_set(int motor, int power) {
    if (power > 0){
        pogobot_motor_set(motor, 1023);
        msleep(50);
    }
    pogobot_motor_set(motor, power);
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

    // create a message to send 
    unsigned char loop_message[message_length_bytes] = "loop";
    unsigned char anchor_message[message_length_bytes] = "anchor";

    // by default the robots is in boat mode
    int state = BOAT;

    time_reference_t t0;
    uint32_t t1;
    while (1)
    {
        // we collect the messages from the infrared receiver and store them in a buffer - FIFO
        pogobot_infrared_update();
        // if it hears a signal it tries to align with the emitter
        if ( pogobot_infrared_message_available() ) {
            message_t mr;
            // get the next message in the buffer and store it in mr
            pogobot_infrared_recover_next_message(&mr);
            // if the message is "anchor" , we check if the message comes from the front of the robot
            if (strcmp(mr.data, anchor_message) == 0){
                // get the ir emitter of the sender
                int ir_emitter_id = mr.header._sender_ir_index;
                // get the ir I received it on
                int ir_receiver_id = mr.header._receiver_ir_index;    
                // if the message comes from the front of the robot
                if (ir_receiver_id == 0){
                    // we stop the robot
                    pogobot_motor_jump_set(motorR, 0); //motorR
                    pogobot_motor_jump_set(motorL, 0); //motorL
                    // we become an anchor
                    state = ANCHOR;
                }
            }
            // if the message is "loop" , we check if the message comes from the left of the robot
            if (strcmp(mr.data, loop_message) == 0){
                // get the ir emitter of the sender
                int ir_emitter_id = mr.header._sender_ir_index;
                // get the ir I received it on
                int ir_receiver_id = mr.header._receiver_ir_index;
                // if the message comes from the left of the robot
                if (ir_receiver_id == 1){
                    // here we should have a mecanism to collect the messages from other 
                    // robots and se if the message looped back to us -> the robot are forming a square
                }
            } 
        }
        // we empty the buffer
            pogobot_infrared_empty_buffer();

        // if we are in anchor mode
        if (state == ANCHOR){
            // we send a message loop in front of us and the message anchor on our left
            pogobot_infrared_sendMessageFrontWithId(0x1234, loop_message, message_length_bytes);
            pogobot_infrared_sendMessageLeftWithId(0x1234, anchor_message, message_length_bytes);
        }
        if (state == BOAT){
            // we wander around
            int direction = rand()%3;

            // turn back control
            if (((last_tick_direction == LEFT) && (direction == LEFT)) || ((last_tick_direction == RIGHT) && (direction == RIGHT))){
                direction = STRAIGHT;
            }
            
            // for debug purpose, we color the robot depending on the direction it takes

            // straight -> green
            // left -> blue
            // right -> red

            if (direction == STRAIGHT){
                pogobot_motor_jump_set(motorR, m); //motorR
                pogobot_motor_jump_set(motorL, m); //motorL
                pogobot_led_setColor(0,255,0);
            } else {
                if (direction == LEFT){
                    pogobot_motor_jump_set(motorL, m);
                    pogobot_motor_jump_set(motorR, 0);
                    pogobot_led_setColor(0,0,255);
                } else {
                    pogobot_motor_jump_set(motorL, 0);
                    pogobot_motor_jump_set(motorR, m);
                    pogobot_led_setColor(255,0,0);
                }
            }
        }

        // we wait for tickMessage microseconds
        t1=pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep((tickMessage - t1)/1000);   // msleep in milliseconds
    }
}
