/*
 * File:   main.c
 * Author: nirma
 *
 * Created on 14 March, 2024, 10:46 PM
 */


#include <xc.h>


#pragma config WDTE = OFF

void init_config(void)
 {

   /* configure RC2 as o/p pin */
    TRISC1=0;

}

void main(void)
 {
    init_config();
    while (1)
    {
        /* Turn on the Buzzer */
        RC1 = 1;
        for(unsigned int wait = 50000; wait--;);
        /* Turn off the Buzzer */
        RC1 = 0;
        for(unsigned int wait = 50000; wait--;);


    }

}
