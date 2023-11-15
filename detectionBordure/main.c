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

int main(void) {

    pogobot_init();

    printf("init ok\n");
    srand(pogobot_helper_getRandSeed());
    // pogobot_infrared_emitter_power_max - 3
    // pogobot_infrared_emitter_power_twoThird - 2
    // pogobot_infrared_emitter_power_oneThird - 1
    // pogobot_infrared_emitter_power_null - 0
    pogobot_infrared_set_power( pogobot_infrared_emitter_power_twoThird );

    unsigned char message[message_length_bytes];
    int i;
    for (i=0;i<message_length_bytes;i++){
        message[i] = 'a';
    }
    message[i-1] = '\0';

    // time_reference_t timers[4];
    // for (int i=0; i<4; i++) {
    //     pogobot_stopwatch_reset(&timers[i]);
    // }

    time_reference_t t0;
    uint32_t t1;

    // match avec sender_id: pour chaque i on regarde le nombre de fois où sender_id est à -1
    // et on le stock dans neighborsMissing.
    // ex: si sender_id[0] ne vaut pas -1, il y a un voisin, on met neighborsMsg[0] à 0
    // si sender_id[0] vaut -1, on incrémente neighborsMsg[0]
    // si pendant 70 (50 c'est pas tout à fait assez, 100 c'est un peu trop long) 
    // itérations sender_id[0] vaut -1, alors le voisin est parti 
    // -> on met neighborsHere[0] à 0 dès que neighborsMissing[0] dépasse 100, on le remet à 1 dès que neighborsMissing[0] = 0
    int neighborsMissing[4] = {0,0,0,0};   
    int neighborsHere[4] = {MISSING,MISSING,MISSING,MISSING}; 

    while (1)
    {
        int sender_id[4] = {-1,-1,-1,-1};
        pogobot_stopwatch_reset(&t0);
        pogobot_infrared_update();

        /* read reception fifo buffer */
        if ( pogobot_infrared_message_available() )
        {
            while ( pogobot_infrared_message_available() )
            {
                message_t mr;
                pogobot_infrared_recover_next_message( &mr );

                int s_id = mr.header._sender_id;
                int i=0;
                for (i=0; i<4; i++){
                    if (sender_id[i]==-1){
                        sender_id[i]=s_id;
                        break;
                    }
                    if (sender_id[i] == s_id){
                        break;
                    }
                }  

                
            }
        }

        for (int i=0; i<4; i++){
            if (sender_id[i]!=-1) {
                neighborsMissing[i] = 0;
                neighborsHere[i] = HERE;
            }
            else {
                neighborsMissing[i] += 1;
                if (neighborsMissing[i] >= 100){
                    neighborsHere[i] = MISSING;
                }
            }
        }
        printf("Sender_id = %d,%d,%d,%d\n", sender_id[0], sender_id[1], sender_id[2], sender_id[3]);
        printf("NeighborsHere = %d,%d,%d,%d\n",neighborsHere[0],neighborsHere[1],neighborsHere[2],neighborsHere[3]);

        int nb_voisins = 0;
        for (int i=0; i<4; i++){
            if (neighborsHere[i] == HERE){
                nb_voisins++;
            }
        }
        switch (nb_voisins){
            case 0:
                pogobot_led_setColor( 0, 0, 0 );
                break;
            case 1:
                pogobot_led_setColor( 0, 255, 255 );
                break;
            case 2:
                pogobot_led_setColor( 255, 0, 0 );
                break;
            case 3:
                pogobot_led_setColor( 0, 255, 0 );
                break;
            case 4:
                pogobot_led_setColor( 0, 0, 255 );
                break;
        }
        
        // if (nb_voisins==4){
        //     pogobot_led_setColor( 0, 0, 255 );
        // }
        // else{
        //     pogobot_led_setColor( 0, 0, 0 );
        // }
        if (rand()%100<50){  
            pogobot_infrared_sendMessageAllDirection( 0x1234, message,message_length_bytes);
        }

        pogobot_infrared_clear_message_queue();
        pogobot_infrared_update();
    
        t1=pogobot_stopwatch_get_elapsed_microseconds(&t0);
        msleep( (1000000/F - t1)/1000 );
    }

}
