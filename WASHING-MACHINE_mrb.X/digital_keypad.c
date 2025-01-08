#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    // set keypad port as input
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;           
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
     static unsigned char longpress;
    static unsigned char prev_key;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES; 
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;         
            longpress=0;
            prev_key =KEYPAD_PORT & INPUT_LINES;        
        }
        else if( !once && (prev_key == (KEYPAD_PORT & INPUT_LINES)) && longpress < 30)
        {
                   longpress++;
            
        
        }
        else if(longpress == 30)
        {
            longpress++;
            return  0x80 | prev_key ;  // 0011 0111 | 1000 0000 -> 0xB7
            
        }
        else if (((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED) && !once) // Fixed extra parenthesis error here
        {
            once = 1;        
            
            if(longpress < 30)
            {
                return prev_key;
            }
                
        }
    }
    return ALL_RELEASED;
}
 

