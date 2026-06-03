/*
 * File:   timer.c
 * Author: Trusty
 *
 * Created on 3 de junio de 2026, 16:50
 */


#include "xc.h"
#include "TIMER.h"

void Init_Timer1(void) {
    //Se configura Timer1
    T1CONbits.TON = 0; //Se asegura que esté apagado para configurarlo
    T1CONbits.TCS = 0; //Reloj interno
    T1CONbits.TCKPS = 1; //Prescaler 1:8
    TMR1 = 0; // Se resetea el cronómetro a 0

    //Se configura la interrupción
    IPC0bits.T1IP = 1; //Se pone prioridad 1 al Timer1
    IFS0bits.T1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
    IEC0bits.T1IE = 1; //Se habilita la interrupción Timer1
    T1CONbits.TON = 1; //Se enciende el Timer1
}

void Init_INT1(void) {
    //Se configura INT1
    IFS1bits.INT1IF = 0; //Se limpia el Interrupt Flag para arrancar en limpio
    INTCON2bits.INT1EP = 0; //Se configura como flanco ascendente
    IPC5bits.INT1IP = 5; //Se pone prioridad 5 al INT1
    IEC1bits.INT1IE = 1; //Se habilita la interrupción INT1
}