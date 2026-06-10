/*
 * File:   sensores.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:11 PM
 */

#include "xc.h"
#include "common.h"

// Variables locales
int contadorEjes = 0;
int auxTMR;
// Rutina de atención para la interrupción del Timer 1

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
}

// Rutina de atención para change notification

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
    //Hay presencia de vehiculo
    if (PORTDbits.RD13) {
        //Si solamente se tocaron las primeras ruedas 1 0
        if (PORTDbits.RD6) {
            //Encender timer
            contadorEjes++;
            if (contadorEjes == 1) {
                TMR1 = 0;
                T1CONbits.TON = 1;
            }
            if (PORTDbits.RD7) {
                if (contadorEjes == 1) {
                    T1CONbits.TON = 0;
                }
                auxTMR = TMR1;
            }
        }
    }
}

// Función auxiliar que junta los datos y los mete en el arreglo
void registrarVehiculo(){
    
}