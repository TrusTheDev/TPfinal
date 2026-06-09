/*
 * File:   sensores.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:11 PM
 */

#include "xc.h"
#include "common.h"

// Variables locales


// Rutina de atención para la interrupción del Timer 1
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;
}

// Rutina de atención para change notification
void __attribute__((interrupt, auto_psv)) _CNInterrupt(void){
    IFS1bits.CNIF = 0;
}

// Función auxiliar que junta los datos y los mete en el arreglo
void registrarVehiculo(){
    
}