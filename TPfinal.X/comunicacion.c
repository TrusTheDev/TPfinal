/*
 * File:   comunicacion.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:15 PM
 */


#include "xc.h"

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void ){
	IFS1bits.U2RXIF = 0; // Se limpia la bandera de interrupción
}

//Rutina de INT para transmisión (no hace nada)
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0; // Se limpia la bandera por seguridad
}

//Comando que llega por UART
int recepcion[20];
extern int indiceX;
extern int indiceY;
extern vehiculos;

int validarPaquete(){
    
}

int destinoCorrecto(){
    
}

int cumpleQTY(){
    
}

int calcularchecksum(){
    
}

void construirPaquete(char comando){
    
}
