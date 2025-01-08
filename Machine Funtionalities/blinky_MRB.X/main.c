/*
 * File:   main.c
 * Author: M REDDY BALAJI
 *
 * Created on 29 February, 2024, 10:29 PM
 */


#include <xc.h>
#include "main.h"


#pragma config WDTE = OFF

void init_config(void)

{

	// Config PORTB as output 
    LED_ARRAY2_DDR  = 0x00;
    
    

}


void main(void)

{
   init_config();
   while(1)
   {
	// logic
       /* turn on led */
       LED_ARRAY2  = 0xFF;
       for (unsigned int wait= 50000; wait--;);
       /* turn on led */
       LED_ARRAY2  = 0x00;
       for (unsigned int wait= 50000; wait--;);
       
       
 	
   }  
}