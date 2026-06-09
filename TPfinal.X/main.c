/*
 * File:   main.c
 * Author: Trusty
 *
 * Created on 3 de junio de 2026, 16:44
 */

#include "xc.h"
#include "config.h"
#include "common.h"

vehiculo vehiculos[256];
unsigned char indiceAutos = 0;
unsigned int totalVehiculos = 0; 
unsigned int total2Ejes = 0;
unsigned int totalPesados = 0;
unsigned int totalExcesoVelocidad = 0;
unsigned char paqueteRecibido = 0;

// Función para simular vehículos
void preCargarVehiculos(void){
    
}

int main(void){
    config(); // Inicialización
    
    preCargarVehiculos(); // Precarga para simular
    
    while(1){
        // Si llega un paquete
        if(paqueteRecibido == 1){
            paqueteRecibido = 0; // Se baja la bandera
            
            // Logica del main
        }
    }
}