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
unsigned char indiceVehiculos = 0;
unsigned int totalVehiculos = 0; 
unsigned int total2Ejes = 0;
unsigned int totalPesados = 0;
unsigned int totalExcesoVelocidad = 0;
unsigned char paqueteRecibido = 0;

// Función para simular vehículos

void preCargarVehiculos(void) {
    // Se cargan 8 vehiculos de prueba (hora, minuto, segundo, velocidad, ejes)
    vehiculos[0] = (vehiculo){18, 7, 26, 95, 2};
    vehiculos[1] = (vehiculo){19, 35, 12, 50, 3};
    vehiculos[2] = (vehiculo){12, 32, 5, 40, 2};
    vehiculos[3] = (vehiculo){12, 33, 40, 65, 4}; // Infracción y pesado
    vehiculos[4] = (vehiculo){12, 34, 20, 50, 2};
    vehiculos[5] = (vehiculo){12, 35, 40, 80, 6}; // Infracción y pesado
    vehiculos[6] = (vehiculo){12, 36, 10, 55, 2};
    vehiculos[7] = (vehiculo){12, 37, 50, 90, 4}; // Infracción y pesado

    // Se actualizan las variables
    indiceVehiculos = 8;
    totalVehiculos = 8;
    total2Ejes = 12;
    totalPesados = 5;
    totalExcesoVelocidad = 20;
}

int main(void){
    config(); // Inicialización
    preCargarVehiculos(); // Precarga para simular
    while(1){
        // Si llega un paquete
        if(paqueteRecibido == 1){
            capaTransporte(); // Se pasa a la Capa de Transporte
            paqueteRecibido = 0; // Se baja la bandera
        }
    }
}