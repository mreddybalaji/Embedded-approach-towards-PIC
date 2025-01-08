#include <xc.h>
#include "main.h"
#include "digital_keypad.h"
#include "timers.h"
#include "clcd.h"

#pragma config WDTE = OFF

void init_config(void) {
    init_digital_keypad();
    init_clcd();
    TRISC2 = 0;
    FAN_DDR = 0; // Fixed missing semicolon
    BUZZER_DDR = 0; // Fixed missing semicolon
    BUZZER = OFF; // Added semicolon and fixed ON/OFF assignment
    init_timer2();
    PEIE = 1;
    GIE = 1;
}

unsigned char operation_mode = WASHING_PROGRAM_DISPLAY;
unsigned char reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
unsigned char program_no = 0, water_level_index = 0;
unsigned char *washing_prog[] = { "DAILY", "HEAVY", "DELICATES", "WHITES", "STAINWASH", "COTTONS", "WOOLENS", "BEADSHEETS", "RINSE+DRY", "DRY ONLY", "WASH ONLY", "AQUA STORE" };
unsigned char *water_level_option[] = { "AUTO", "LOW", "MEDIUM", "HIGH", "MAX" };

unsigned char min, sec;
unsigned int rinse_time, wash_time, spin_time;

void main(void) {
    init_config();
    unsigned char key;

    clcd_print("PRESS KEY 5 TO", LINE1(1));
    clcd_print("POWER ON", LINE2(4));
    clcd_print("WASHING MACHINE", LINE3(1));

    while (read_digital_keypad(STATE) != SW5) {
        for (unsigned int wait = 3000; wait--;);
    }

    power_on_screen();

    while (1) {
        key = read_digital_keypad(STATE);
        for (unsigned int wait = 100; wait--;);

        if (key == LSW4 && operation_mode == WASHING_PROGRAM_DISPLAY) {
            operation_mode = WATER_LEVEL;
            reset_mode = WATER_LEVEL_RESET;
        } else if (key == LSW4 && operation_mode == WATER_LEVEL) {
            set_time();
            operation_mode = START_STOP_SCREEN;
            clear_screen();
            clcd_print("PRESS SWITCH", LINE1(1));
            clcd_print("SW5: START", LINE2(1));
            clcd_print("SW6: STOP", LINE3(1));
        }
        
        if (key == SW5 && operation_mode == PAUSE)
        {
            TMR2ON=1;
            FAN=ON;
            operation_mode=START_PROGRAM;
        }

        
        switch (operation_mode) {
            case WASHING_PROGRAM_DISPLAY:
                washing_program_display(key);
                break;
            case WATER_LEVEL:
                water_level_display(key);
                break;
            case START_STOP_SCREEN:
                if (key == SW5) {
                    operation_mode = START_PROGRAM;
                    reset_mode = START_PROGRAM_RESET;
                    continue;
                } else if (key == SW6) {
                    operation_mode = WASHING_PROGRAM_DISPLAY;
                    reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
                    continue;
                }
                break;
            case START_PROGRAM:
                run_program(key);
                break;
        }

        reset_mode = RESET_NOTHING;
    }
}

void power_on_screen(void) {
    for (unsigned char i = 0; i < 16; i++) {
        clcd_putch(BLOCK, LINE1(i));
    }

    clcd_print("POWERING ON", LINE2(2));
    clcd_print("WASHING MACHINE", LINE3(1));

    for (unsigned char i = 0; i < 16; i++) {
        clcd_putch(BLOCK, LINE4(i));
    }

    __delay_us(1000);

    clear_screen();
}

void washing_program_display(unsigned char key) {
    if (reset_mode == WASHING_PROGRAM_DISPLAY_RESET) {
        clear_screen();
        program_no = 0;
    }

    if (key == SW4) {
        program_no++;
        clear_screen();
        if (program_no == 12) {
            program_no = 0;
        }
    }

    clcd_print("WASHING PROGRAMS", LINE1(0));
    clcd_putch('*', LINE2(0));

    if (program_no <= 9) {
        clcd_print(washing_prog[program_no], LINE2(2));
        clcd_print(washing_prog[program_no + 1], LINE3(2));
        clcd_print(washing_prog[program_no + 2], LINE4(2));
    } else if (program_no == 10) {
        clcd_print(washing_prog[program_no], LINE2(2));
        clcd_print(washing_prog[program_no + 1], LINE3(2));
        clcd_print(washing_prog[0], LINE4(2));
    } else if (program_no == 11) {
        clcd_print(washing_prog[program_no], LINE2(2));
        clcd_print(washing_prog[0], LINE3(2));
        clcd_print(washing_prog[1], LINE4(2));
    }
}

void water_level_display(unsigned char key) {
    if (reset_mode == WATER_LEVEL_RESET) {
        water_level_index = 0;
        clear_screen();
    }
    if (key == SW4) {
        water_level_index++;
        if (water_level_index == 5) {
            water_level_index = 0;
        }
        clear_screen();
    }

    clcd_print("WATER LEVEL:", LINE1(0));
    clcd_putch('*', LINE2(0));

    if (water_level_index <= 2) {
        clcd_print(water_level_option[water_level_index], LINE2(2));
        clcd_print(water_level_option[water_level_index + 1], LINE3(2));
        clcd_print(water_level_option[water_level_index + 2], LINE4(2));
    } else if (water_level_index == 3) {
        clcd_print(water_level_option[water_level_index], LINE2(2));
        clcd_print(water_level_option[water_level_index + 1], LINE3(2));
        clcd_print(water_level_option[0], LINE4(2));
    } else if (water_level_index == 4) {
        clcd_print(water_level_option[water_level_index], LINE2(2));
        clcd_print(water_level_option[0], LINE3(2));
        clcd_print(water_level_option[1], LINE4(2));
    }
}

void set_time(void) {
    switch (program_no) {
        case 0:
            switch (water_level_index) {
                case 1:
                    sec = 33;
                    min = 0;
                    break;
                case 0:
                case 2:
                    sec = 41;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 45;
                    min = 0;
                    break;
            }
            break;

        case 1:
            switch (water_level_index) {
                case 1:
                    sec = 43;
                    min = 0;
                    break;
                case 0:
                case 2:
                    sec = 50;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 57;
                    min = 0;
                    break;
            }
            break;

        case 2:
            switch (water_level_index) {
                case 1:
                case 0:
                    sec = 26;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 31;
                    min = 0;
                    break;
            }
            break;

        case 3:
        case 4:
        case 5:
        case 6:
            sec = 16;
            min = 1;
            break;

        case 7:
            switch (water_level_index) {
                case 1:
                    sec = 46;
                    min = 0;
                    break;
                case 0:
                case 2:
                    sec = 53;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 0;
                    min = 1;
                    break;
            }
            break;

        case 8:
            switch (water_level_index) {
                case 1:
                    sec = 18;
                    min = 0;
                    break;
                case 0:
                case 2:
                case 3:
                case 4:
                    sec = 20;
                    min = 0;
                    break;
            }
            break;

        case 9:
            sec = 6;
            min = 0;
            break;

        case 10:
        case 11:
            switch (water_level_index) {
                case 1:
                    sec = 16;
                    min = 0;
                    break;
                case 0:
                case 2:
                    sec = 21;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 26;
                    min = 0;
                    break;
            }
            break;
    }
}

void run_program(unsigned char key) {
    door_status_check();
    static int total_time, time, wash_time, rinse_time, spin_time;
    // Assuming variables like min, sec, program_no, reset_mode, washing_prog[], FAN, TMR2ON, BUZZER, operation_mode are declared elsewhere

    if (reset_mode == START_PROGRAM_RESET) {
        clear_screen();
        clcd_print("PROG: ", LINE1(0));
        clcd_print(washing_prog[program_no], LINE1(6));
        clcd_print("TIME:", LINE2(0));

        clcd_putch((min / 10) + '0', LINE2(6));
        clcd_putch((min % 10) + '0', LINE2(7));
        clcd_putch(':', LINE2(8));
        clcd_putch((sec / 10) + '0', LINE2(9));
        clcd_putch((sec % 10) + '0', LINE2(10));
        clcd_print("(MM:SS)", LINE3(5));

        __delay_ms(2000);
        clear_screen();
        clcd_print("FUNCTION - ", LINE1(0));
        clcd_print("TIME:", LINE2(0));
        clcd_print("5-START  6-PAUSE", LINE4(0));

        time = total_time = (min * 60) + sec;
        wash_time = (int)total_time * (0.46);
        rinse_time = (int)total_time * (0.12);
        spin_time = total_time - wash_time - rinse_time;

        FAN = ON;
        TMR2ON = 1; // Assuming ON is defined as 1 in your code
    }
    
    if (key == SW6)
    {
        FAN=OFF;
        TMR2ON=0;
        operation_mode=PAUSE;
    }

    // Your existing logic for checking program_no and printing on the LCD
    total_time = (min * 60) + sec;
    if (program_no <= 7) {
        if (total_time >= (time - wash_time)) {
            clcd_print("WASH", LINE1(11));
        } else if (total_time >= (time - wash_time - spin_time)) {
            clcd_print("RINSE", LINE1(11));
        } else {
            clcd_print("SPIN", LINE1(11));
        }
    } else if (program_no == 8) {
        if (total_time >= (time - (0.40 * time))) {
            clcd_print("SPIN", LINE1(11));
        } else {
            clcd_print("SPIN", LINE1(11));
        }
    } else if (program_no == 9) {
        clcd_print("SPIN", LINE1(11));
    } else {
        clcd_print("WASH", LINE1(11));
    }

    clcd_putch((min / 10) + '0', LINE2(6));
    clcd_putch((min % 10) + '0', LINE2(7));
    clcd_putch(':', LINE2(8));
    clcd_putch((sec / 10) + '0', LINE2(9));
    clcd_putch((sec % 10) + '0', LINE2(10));

    if ((sec == 0) && (min == 0)) {
        FAN = OFF;
        TMR2ON = OFF;

        BUZZER = ON;
        clear_screen();
        clcd_print("PROG COMPLETED", LINE1(0));
        clcd_print("REMOVE CLOTHES", LINE2(0));
        __delay_ms(2000);
        BUZZER = OFF;
        operation_mode = WASHING_PROGRAM_DISPLAY;
        reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
        clear_screen();
    }
}
void door_status_check(void)
{
    if(RB0 == 0)
    {
        FAN=OFF;
        TMR2ON=OFF;
        BUZZER=ON;
        clear_screen();
        clcd_print("DOOR OPEN", LINE1(0));
        clcd_print("PLEASE CLOSE", LINE3(0));
        while(RB0 == 0);
        {
            ;
        }
        clear_screen();
        clcd_print("FUNCTION - ", LINE1(0));
        clcd_print("TIME:", LINE2(0));
        clcd_print("5-START  6-PAUSE", LINE4(0));
        FAN = ON;
        TMR2ON = 1;
        BUZZER=OFF;
        
        
    }
}
