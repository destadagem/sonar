#include "ultrasonic.h"
#include "graphics.h"
#include "lcd.h"
#include "color.h"
#include "ports.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * main.cpp
 */
void main(void)
{
    state = START;
    initMSP430();
    initLCD();
    clearScreen(1);
    while(1){
        switch(state){
            case START:
                trigger();                  //PULL up 2.4 for 10 usec
                while(current_state == 0)
                    LPM0;
                state = DATA;
                break;
            case DATA:
                getData();
                while(current_state == 1)
                    LPM0;
                distance = (float)(time*(0.0001)*340.0)/2.0;
                setColor(COLOR_16_BLACK);
                drawString(65, 15, FONT_SM, stringDistance);
                setColor(COLOR_16_WHITE);
                snprintf(stringDistance,6, "%f",distance);
                drawString(2, 15, FONT_SM, "DISTANCE = ");
                drawString(65, 15, FONT_SM, stringDistance);
                state = START;
                __delay_cycles(10000);
                break;
        }
    }
}
