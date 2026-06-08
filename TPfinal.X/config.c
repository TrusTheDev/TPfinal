/*
 * File:   config.c
 * Author: trusty
 *
 * Created on June 4, 2026, 3:03 PM
 */


#include "xc.h"

// Configuración de la velocidad para FCY = 1MHz (El proyecto esta en esta config.)
#define FCY 1000000
#define BAUDRATE 19200
#define BRGVAL ((FCY / BAUDRATE) / 4) - 1 //El TP4 hace algo raro con esto

void Init_puertos(void) {
    // Se configuran los puertos
    TRISD = 0xFFFF; // RD6,RD7, RD13 como entrada
    TRISA = 0x0001; // RD0 como camara fotografica (entrada)
}
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

void Init_UART(void) {
    	U1MODEbits.UARTEN = 0; // Se apaga el módulo para configurarlo
    
    // Configuración de la trama (19200 8N1)
    U1MODEbits.BRGH = 1; // Modo de alta velocidad (divisor de 4)
    U1MODEbits.PDSEL = 0; // 8 bits, sin paridad
    U1MODEbits.STSEL = 0; // 1 bit de stop
	U1MODEbits.RTSMD = 1; // Modo simplex

	//U2BRG = BRGVAL;	// Se carga el valor del cálculo de baudios
	IPC7 = 0x4400; // Se configura la prioridad de interrupción

	//IFS1bits.U1RXIF = 0; // Se limpia la bandera de recepción
	//IEC1bits.U1RXIE = 1; // Se prende la interrupción de recepción

	U1MODEbits.UARTEN = 1; // Se prende el módulo UART
	U1STAbits.UTXEN = 1; // Empieza a transmitir. Se dispara el Flag TXIF

	//IFS1bits.U1TXIF = 0; // Se limpia la bandera de transmisión
	//IEC1bits.U1TXIE = 0; // La interrupción está apagada
}

void config(void) {
    Init_puertos();
    Init_Timer1();
    Init_INT1();
    Init_UART();
}