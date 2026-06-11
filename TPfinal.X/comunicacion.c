/*
 * File:   comunicacion.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:15 PM
 */

#include "xc.h"
#include "common.h"

// Variables locales
unsigned char bufferRX[20]; // Aca se guarda lo que llega de la PC
unsigned char bufferTX[20]; // Aca se arma la respuesta para la PC

// Indices para los arreglos
unsigned char indiceRX = 0;
unsigned char indiceTX = 0;

// CAPA FISICA

// Rutina de interrupción para recepción
void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void ){
	IFS1bits.U2RXIF = 0; // Se limpia la bandera de interrupción
}

//Rutina de interrupción para transmisión
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0; // Se limpia la bandera de interrupción
}

// CAPA DE TRANSPORTE

// Función auxiliar para calcular el checksum
unsigned char calcularChecksum(){
    
}

// Función que revisa el destino y el checksum
void capaTransporte(void){
    
}

// CAPA DE APLICACION

// Función auxiliar que arma la trama final y dispara TX
void construirPaquete(void){
    
}

// Procesa los comandos y arma la respuesta
void capaAplicacion(void){
    
}