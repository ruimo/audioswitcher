/*
 * File:   main.c
 * Author: shanai
 *
 * Created on December 18, 2022, 2:21 PM
 */
#include <xc.h>
#include "state.h"

#pragma config OSC  = INTIO2, PWRT = OFF, BOR = OFF
#pragma config WDT = OFF, MCLRE = OFF, LVP = OFF

#define TRISB_IN 1
#define TRISB_OUT 0

void tick(int32_t i) {
    tick_switch(i);
}

void main(void) {
    OSCCON = 0b01110010;           // Internal OSC 8MHz
    // Output: RB0, RB2, RB3, RB5
    TRISBbits.RB0 = TRISB_OUT;
    TRISBbits.RB2 = TRISB_OUT;
    TRISBbits.RB3 = TRISB_OUT;
    TRISBbits.RB5 = TRISB_OUT;
    TRISBbits.RB6 = TRISB_IN;
    TRISBbits.RB7 = TRISB_IN;
    INTCON2bits.nRBPU = 0;      // pull-up B port
    
    PORTB = 0xff; // Reset PORTB
    LATB = 0;
    ADCON1 = 0xff;                 // All Digital
    
    for (int32_t i = 0; i < 100000; ++i) {
        tick(i);
    }
}
