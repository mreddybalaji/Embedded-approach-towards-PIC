/*
 * File:   main.c
 * Author: nirma
 *
 * Created on 3 March, 2024, 3:36 PM
 */

/* to toggle when sw is pressed*/
#include <xc.h>


#pragma config WDTE = OFF

void init_config(void)
 {
    
    // configure led port as output as PORTD
    TRISD = 0x00;
    PORTD = 0x00;
    
    // config RB0 pin as input
    TRISB0 = 1;
    
    
    

}

void main(void)
 {
    unsigned char once=1;
    init_config();
    while (1) {
        // logic
        // check if switch is pressed
        if(RB0 == 0 && once)
        {
            PORTD = ~PORTD;
            once = 0;
        }
        if(RB0 == 1)
        {
            once = 1;
        }
    }

}
