/*
 * File:   sensores.c
 * Author: Trusty
 *
 * Created on June 5, 2026, 8:11 PM
 */

#include "xc.h"
#include "common.h"

// Variables locales
int contadorEjes = 0;
int velocidad;
int tiempoInicio;
volatile unsigned int milisegundos = 0;
volatile unsigned int ms_reloj = 0;
volatile unsigned int segundos = 0;
volatile unsigned int minutos = 0;
volatile unsigned int horas = 0;
int rd6_prev = 0;
int rd7_prev = 0;
// Rutina de atención para la interrupción del Timer 1

//Eliminar?
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T6Interrupt(void)
{
    IFS2bits.T6IF = 0;
    milisegundos++;
    ms_reloj++;
    if(ms_reloj >= 1000){
        ms_reloj = 0;
        segundos++;
        if(segundos >= 60){
            segundos = 0;
            minutos++;
            if(minutos >= 60){
                minutos = 0;
                horas++;
                if(horas >= 24){
                    horas = 0;
                }
            }
        }
    }
}

// Rutina de atención para change notification
void __attribute__((interrupt, auto_psv)) _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    if(PORTDbits.RD13){
        // Flanco sensor 1
        if(PORTDbits.RD6 && !rd6_prev){
            contadorEjes++;
            if(contadorEjes == 1){
                tiempoInicio = milisegundos;
            }
        }
        // Flanco sensor 2
        if(PORTDbits.RD7 && !rd7_prev){
            if(contadorEjes == 1){
                int delta = milisegundos - tiempoInicio;
                if(delta > 0){
                    velocidad = 1440 / delta;
                    if (velocidad > 60) {
                        PORTAbits.RA0 = !PORTAbits.RA0;
                    }
                }
            }
        }
    }
    else{
        if(contadorEjes > 0){
            registrarVehiculo();
            contadorEjes = 0;
        }
    }
    rd6_prev = PORTDbits.RD6;
    rd7_prev = PORTDbits.RD7;
}
// Función auxiliar que junta los datos y los mete en el arreglo
void registrarVehiculo(){
    vehiculos[indiceAutos].ejes = (char) contadorEjes;
    vehiculos[indiceAutos].velocidad = (char) velocidad;
    vehiculos[indiceAutos].infraccion = velocidad > 60 ? 'S' : 'N';
    vehiculos[indiceAutos].hora = (char) horas;
    vehiculos[indiceAutos].minuto = (char) minutos;
    vehiculos[indiceAutos].segundo = (char) segundos;
    
    if(indiceAutos <= 256){
        indiceAutos++;
    }
}