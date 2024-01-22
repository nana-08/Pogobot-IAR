/**
 * POGOBOT
 *
 * Copyright © 2024 Sorbonne Université ISIR
 * This file is licensed under the Expat License, sometimes known as the MIT License.
 * Please refer to file LICENCE for details.
**/

/* 
Algorithm of election of a leader
> robots are placed in a circle and send messages to their right exclusively
> behavior of each robot
    - the election happens in 3 rounds
    - every robot stores a list of random picks
    - each round, robots pick a random number between 1 and 10 and stores it in a list of picks
    - at the end of the three rounds, each robot computes its score, i.e the weighted sum of its picks. 
    The leader is chosen as the robot with the smallest difference between the three random picks
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pogobot.h"


#define message_length_bytes 6
#define tickMessage 1000000   // notify the environment every tickMessage microseconds

#define rounds 3
#define FOLLOWER 0
#define LEADER 1

int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();
    printf("init ok\n");

    int i = 0;
    int randomPicks[] = {0,0,0};
    int state = FOLLOWER;   // before the election
                           
    time_reference_t t0;
    uint32_t t1;
    while (1){
        
        if (i < rounds){
            randomPicks[i]= rand()%1000;
            printf("random pick %d: %d\n", i, randomPicks[i]);
            i++;
        } else {
            if (i == rounds){
                // send the score
                int score = 3*randomPicks[0] + 2*randomPicks[1] + randomPicks[2];

                printf("score = %d\n", score);
                state = LEADER;
                i++;
            } else {
                if (state == LEADER){
                    pogobot_led_setColor(255,0,0);
                } else {
                    pogobot_led_setColor(0,0,255);
                }
            }
        }

    }

}
