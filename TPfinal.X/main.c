/*
 * File:   main.c
 * Author: Trusty
 *
 * Created on 3 de junio de 2026, 16:44
 */


#include "xc.h"
#include "UART.h"
#include "TIMER.h"
#include "ADC.h"

//Colocar rutinas de atencion aqui
//Configuracion de puertos e inicio del programa

void __attribute__((interrupt, auto_psv)) _DMA0Interrupt(void) {
    /* Rutina de atención para la interrupción del DMA */
    IFS0bits.DMA0IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    /* Rutina de atención para la interrupción del Timer 1 */
    IFS0bits.T1IF = 0;
}

int main(void) {
    InitADC();
    InitDMA();
    Init_Timer1();
    Init_INT1();
    InitUART();
    while (1) {
    };
}
