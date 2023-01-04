/*
 * File:   main.c
 * Author: shanai
 *
 * Created on December 18, 2022, 2:21 PM
 */
#include <xc.h>
#include "switch_state.h"
#include "uart_state.h"
#include "ad_state.h"

#pragma config OSC  = INTIO2, PWRT = OFF, BOR = OFF
#pragma config WDT = OFF, MCLRE = OFF, LVP = OFF

#define TRISB_IN 1
#define TRISB_OUT 0

uint16_t i = 0;

void tick() {
  tick_uart();
  tick_switch();
  tick_ad();
}

void init(void) {
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

  TXSTA = 0x2c;
  RCSTA = 0x90;
  BAUDCTL = 0x08;
  SPBRG = 16;

  ADCON0bits.VCFG = 0; // Vref = Vss/Vdd
  ADCON0bits.ADON = 1; // Enable A/D
  ADCON1 = 0xff; // All Digital but AN0, AN1
  ADCON1bits.PCFG0 = 0;  
  ADCON1bits.PCFG1 = 0;  
  ADCON2bits.ADFM = 1; // Right Justify
  ADCON2bits.ACQT = 4; // 8 Tad
  ADCON2bits.ADCS = 5; // 16 Tosc
}

void main(void) {
  init();
    
  for (;;) {
    tick();
  }
}
