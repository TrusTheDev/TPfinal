/*
 * File:   ISRs.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:11 PM
 */


#include "xc.h"

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    /* Rutina de atención para la interrupción del Timer 1 */
    IFS0bits.T1IF = 0;
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
}