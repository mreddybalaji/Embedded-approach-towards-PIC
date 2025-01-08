/*
 * File:   main.c
 * Author: nirma
 *
 * Created on 3 March, 2024, 4:18 PM
 */


#include <xc.h>
#include "main.h"
#include "digital_keypad.h"
#pragma config WDTE = OFF

void init_config(void)
 {

    // one time initialization
    LED_ARRAY1    = 0x00;
    LED_ARRAY1_DDR= 0x00;
    
    init_digital_keypad();
}

void main(void)
 {
    unsigned char key, mask = 0x55;
    init_config();
    while (1) {
        // logic
        key = read_digital_keypad(LEVEL);
        if (key == SW1)
        {
            LED_ARRAY1 = ~LED_ARRAY1;
            for(unsigned int wait = 50000; wait--;);
        }
        if (key == SW2)
        {
            mask = ~mask;
            LED_ARRAY1 = mask;
            for(unsigned int wait = 50000; wait--;);
        }

    }

}
