/*
 * ports.c
 *
 *  Created on: Nov 3, 2018
 *      Author: desta
 */
#include "ports.h"
void initMSP430(void) {

    WDTCTL = WDTPW | WDTHOLD;   // kill the watchdog
    P1DIR   |= BIT0;
    P9DIR   |= BIT7;
//    P1DIR &~ BIT2;             // P1.2 set as input
//    P1OUT |= BIT2;             // P1.2 give pullup/down resistor a '1' so it pulls up
//    P1REN |= BIT2;             // P1.2 enable pullup/down resistor
//    P1IES |= BIT2;             // P1.2 falling edge
//    P1IFG &= ~BIT2;            // P1.2 clear interrupt flag
//    P1IE |= BIT2;              // P1.2 enable interrupt
//
//
//    TA2CCR0 = 819;                // 25 ms * 32768 Hz = 819.2 ticks debounce delay
//    TA2CTL = TASSEL__ACLK | MC__STOP | TACLR;     // Configure debounce timer but don't start it

    /************************** PWM Backlight ******************************/

    P2DIR   |= BIT4;
    P2SEL0  |= BIT4;
    P2SEL1  &= ~BIT4;
    TB0CCR0  = 511;
    TB0CCTL3 = OUTMOD_7;
    TB0CCR3  = 256;
    TB0CTL   = TBSSEL__ACLK | MC__UP | TBCLR;

    /******************************** SPI **********************************/

    P2DIR  |=   LCD_DC_PIN | LCD_CS_PIN;            // DC and CS
    P1SEL0 |=   LCD_MOSI_PIN | LCD_UCBCLK_PIN;      // MOSI and UCBOCLK
    P1SEL1 &= ~(LCD_MOSI_PIN | LCD_UCBCLK_PIN);

    UCB0CTLW0 |= UCSWRST;       // Reset UCB0

    /*
     * UCBxCTLW0     - eUSCI_Bx Control Register 0
     * UCSSEL__SMCLK - SMCLK in master mode
     * UCCKPL        - Clocl polarity select
     * UCMSB         - MSB first select
     * UCMST         - Master mode select
     * UCMODE_0      - eUSCI mode 3-pin SPI select
     * UCSYNC        -  Synchronous mode enable
     */
    UCB0CTLW0 |= UCSSEL__SMCLK | UCCKPL | UCMSB | UCMST | UCMODE_0 | UCSYNC;

    UCB0BR0   |= 0x01;         // Clock = SMCLK/60
    UCB0BR1    = 0;
    UCB0CTL1  &= ~UCSWRST;     // Clear UCSWRST to release the eUSCI for operation
    PM5CTL0   &= ~LOCKLPM5;    // Unlock ports from power manager

    /************************** Ultra Sonic Sensor ****************************/

    P3DIR |= BIT3;              //Adjust the pin for Trigger

    P4DIR &= ~BIT7;             // Adjust the pin for Echo
    P4SEL1 |= BIT7;
    P4SEL0 |= BIT7;

    P9DIR |= BIT7;
    P9OUT &= ~BIT7;
    P1DIR |= BIT0;
    PM5CTL0   &= ~LOCKLPM5;    // Unlock ports from power manager

    __enable_interrupt();
}

/*
 *  Needs to write data to the device using spi. We will only want to write to
 *  the device we wont worry the reads.
 */

void writeData(uint8_t data) {
    P2OUT &= ~LCD_CS_PIN;
    P2OUT |=  LCD_DC_PIN;
    UCB0TXBUF = data;
    while(UCB0STATW & UCBUSY);
    P2OUT |= LCD_CS_PIN;
}

/*
 *  Needs to write commands to the device using spi
 */

void writeCommand(uint8_t command) {

    P2OUT &= ~(LCD_CS_PIN | LCD_DC_PIN);
    UCB0TXBUF = command;
    while(UCB0STATW & UCBUSY);
    P2OUT |= LCD_CS_PIN;
}



