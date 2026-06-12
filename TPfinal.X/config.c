/*
 * File:   config.c
 * Author: trusty
 *
 * Created on June 4, 2026, 3:03 PM
 */

#include "xc.h"
#include "config.h"

// Cálculo de baudios para la UART
#define FCY 40000000
#define BAUDRATE 19200
#define BRGVAL ((FCY / BAUDRATE) / 16) - 1

void Init_Puertos(void){
    // Se configuran los puertos
    TRISDbits.TRISD6 = 1;  // Sensor cable 1 como entrada
    TRISDbits.TRISD7 = 1;  // Sensor cable 2 como entrada
    TRISDbits.TRISD13 = 1; // Sensor inductivo como entrada
    
    TRISAbits.TRISA0 = 0; // Cámara fotográfica como salida
    LATAbits.LATA0 = 0; // Arranca apagada
}

// Timer configurado para 40MHz
void Init_Timer6( void ){
    // Se configura Timer6 para medir el tiempo entre sensores
    T6CONbits.TON = 0; // Se asegura que esté apagado para configurarlo
    T6CONbits.TCKPS = 1; // Prescaler 1:8
    
    IFS2bits.T6IF = 0; // Se limpia la bandera de interrupcion
    IPC11bits.T6IP = 5; // Prioridad 5
    IEC2bits.T6IE = 1; // Se habilita la interrupción
    
    PR6 = 4999; // PR6 para 1ms
    T6CONbits.TON = 1; // Se prende el Timer
}

void Init_CN(void){
    IFS1bits.CNIF = 0; // Se limpia la bandera
    
    // Estos pines avisan cuando cambian de estado
    CNEN1bits.CN15IE = 1; // Habilita RD6 (Sensor 1)
    CNEN2bits.CN16IE = 1; // Habilita RD7 (Sensor 2)
    CNEN2bits.CN19IE = 1; // Habilita RD13 (Sensor inductivo)
    
    IPC4bits.CNIP = 4; // Prioridad 4
    
    IEC1bits.CNIE = 1; // Se habilita el CN
}

void Init_UART2(void){
    U2MODEbits.UARTEN = 0; // Se apaga el módulo para configurarlo
    
    // Configuración de la trama
    U2MODEbits.BRGH = 0; // Modo de velocidad estandar
    U2MODEbits.PDSEL = 0; // 8 bits, sin paridad
    U2MODEbits.STSEL = 0; // 1 bit de stop
	U2MODEbits.RTSMD = 1; // Modo simplex

	U2BRG = BRGVAL;	// Se carga el valor del cálculo de baudios
	IPC7 = 0x4400; // Se configura la prioridad de interrupción

	IFS1bits.U2RXIF = 0; // Se limpia la bandera de recepción
	IEC1bits.U2RXIE = 1; // Se prende la interrupción de recepción

	U2MODEbits.UARTEN = 1; // Se prende el módulo UART
	U2STAbits.UTXEN = 1; // Empieza a transmitir. Se dispara el Flag TXIF

	IFS1bits.U2TXIF = 0; // Se limpia la bandera de transmisión
    IEC1bits.U2TXIE = 1; // Se prende la interrupción de transmisión
}

void config(void){
    Init_Puertos();
    Init_Timer6();
    Init_CN();
    Init_UART2();
}