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
#define tick 2000   // the leader will notify the environment every 2 seconds


int main(void) {

    // Initialize the Pogobot - MANDATORY
    pogobot_init();
    printf("init ok\n");
    

    // signal to send as the leader
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_twoThird );
    unsigned char message[] = "still";

    time_reference_t t0;
    uint32_t t1;
    while (1){
        pogobot_stopwatch_reset(&t0);

        pogobot_led_setColor( 255, 0, 0 );
        // send a message in every direction, with the id of the origin ir emitter
        pogobot_infrared_sendMessageAllDirectionWithId(0x1234, message, message_length_bytes);

        t1=pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep(tick - t1);
    }

}
