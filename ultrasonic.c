/*
 * ultrasonic.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: desta
 */

#include <ultrasonic.h>

void trigger(){

    P9OUT |= BIT7;

    P3OUT |= BIT3;

    TA0CCR0 = 10;              // (32768 Hz / 4) / 100 ticks = 81.92 Hz
    TA0CTL = TASSEL__SMCLK | MC__UP | ID_1 | TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
}

void getData(){

    P1OUT |= BIT0;


    TA1CCTL2 = CM_1 | CCIS_0 | SCS | CAP | CCIE;
    TA1CTL = MC_2 | TASSEL_2 | TAIE | TACLR;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR()
{

    TA0CTL = MC_0 | TACLR;
    TA0CCTL0 &= ~CCIE;
    P9OUT &= ~BIT7;
    // disable timer and interrupt
    P3OUT &= ~BIT3;
    if(current_state == 0){
        current_state = 1;
    }
    LPM0_EXIT;
}

// Interrupt Service Routine for Timer A1
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
    switch(__even_in_range(TA1IV, TA1IV_TAIFG))
    {
      case TA0IV_TA0CCR2:                                 // TA0CCR2
          if(edge == 0){
              start=TA1CCR2;
              edge = 1;
              TA1CCTL2 &= ~CM_1;
              TA1CCTL2 |= CM_2;
          }
          else{
              time=TA1CCR2-start;
              TA1CTL = MC_0 | TASSEL_2 | TACLR;
              TA1CCTL2 &= ~CCIE;
              P1OUT &= ~BIT0;
              if(current_state == 1){
                  current_state = 0;
              }
              edge = 0;
              LPM0_EXIT;
          }
          break;
      case TA1IV_TA1IFG:
          break;
      default: break;
    }//store data into five unsigned integers
}
