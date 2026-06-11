/*
 * File:   sensores.c
 * Author: ivanb
 *
 * Created on June 5, 2026, 8:11 PM
 */

#include "xc.h"
#include "common.h"

// Variables locales
unsigned char contadorEjes = 0;
unsigned int velocidad = 0;
unsigned int tiempoInicio = 0;
unsigned int tiempoMedido = 0;

volatile unsigned int milisegundos = 0;
volatile unsigned int ms_reloj = 0;
volatile unsigned char segundos = 0;
volatile unsigned char minutos = 0;
volatile unsigned char horas = 0;

// Rutina de atención para la interrupción del Timer 6
void __attribute__((interrupt, auto_psv)) _T6Interrupt(void){
    IFS2bits.T6IF = 0; // Se baja la bandera
    
    milisegundos++; // Cronómetro para la velocidad
    ms_reloj++; // Cronómetro para el reloj real
    
    // Si pasaron 1000 milisegundos, sumamos 1 segundo al reloj
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
    IFS1bits.CNIF = 0; // Se baja la bandera
    
    // Si se activa el sensor inductivo
    if(PORTDbits.RD13 == 1){
        // Si se pisa el primer cable
        if(PORTDbits.RD6 == 1){
            contadorEjes++; // Se suma un eje
            // Si son las ruedas delanteras
            if(contadorEjes == 1){
                tiempoInicio = milisegundos; // Se toma el tiempo de inicio para el cálculo de velocidad
            }
        }
        // Si se pisa el segundo cable
        if(PORTDbits.RD7 == 1){
            // Si son las ruedas delanteras
            if(contadorEjes == 1){
                tiempoMedido = milisegundos - tiempoInicio; // Se calcula cuanto tardó para el cálculo de velocidad
                // Nos aseguramos de que no sea división por 0
                if(tiempoMedido > 0){
                    velocidad = 1440 / tiempoMedido; // Se calcula la velocidad
                    // Si va a más de 60km/h
                    if (velocidad > 60) {
                        LATAbits.LATA0 = !LATAbits.LATA0; // Se dispara la cámara
                    }
                }
            }
        }
    }
    // Cuando sale del sensor inductivo
    else {
        // Se guarda el vehículo y se resetean variables
        if(contadorEjes > 0){
            registrarVehiculo();
            contadorEjes = 0; 
            velocidad = 0;
            tiempoMedido = 0;
            tiempoInicio = 0; 
        }
    }
}

// Función auxiliar que junta los datos y los mete en el arreglo
void registrarVehiculo(void){
    vehiculos[indiceVehiculos].ejes = contadorEjes;
    vehiculos[indiceVehiculos].velocidad = (char) velocidad;
    vehiculos[indiceVehiculos].hora = horas;
    vehiculos[indiceVehiculos].minuto = minutos;
    vehiculos[indiceVehiculos].segundo = segundos;
    
    totalVehiculos++; // Se suma un vehículo al contador
    // Se ve si es auto a camión, y se suma al contador
    if(contadorEjes == 2){
        total2Ejes++;
    } else if (contadorEjes > 2){
        totalPesados++;
    }
    // Se ve si hubo infracción, y se suma al contador
    if(velocidad > 60){
        totalExcesoVelocidad++;
    }
    indiceVehiculos++; // Se avanza el índice para el próximo vehículo
    // Si se llega al final del arreglo, se vuelve a empezar
    if (indiceVehiculos >= 256) {
        indiceVehiculos = 0; 
    }
}