/*
 * File:   main.c
 * Author: shanai
 *
 * Created on December 18, 2022, 2:21 PM
 */
#include <xc.h>

#pragma config OSC  = INTIO2, PWRT = OFF, BOR = OFF
#pragma config WDT = OFF, MCLRE = OFF, LVP = OFF

void main(void) {
    OSCCON = 0b01110010;           // Internal OSC 8MHz
    TRISB = 0;
    PORTB = 0xff;
    LATB = 0;
    ADCON1 = 0xff;                 // All Digital
    
//    int32_t i = 0;
    for (;;) {
        LATBbits.LATB2=1;
        _delay(1000000);
        LATBbits.LATB2=0;

        _delay(100000);

        LATBbits.LATB0=1;
        _delay(1000000);
        LATBbits.LATB0=0;

        _delay(5000000);
    }
}
