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

// Funcion para simular vehiculos descomentar, rellena del 0 al 7
void preCargarVehiculos(void){
    indiceAutos =  8;
    vehiculos[0] = (vehiculo){12,30,15,45,0,2}; vehiculos[1]=(vehiculo){12,31,10,72,1,2}; vehiculos[2]=(vehiculo){12,32,5,38,0,2}; vehiculos[3]=(vehiculo){12,33,40,65,1,4}; vehiculos[4]=(vehiculo){12,34,22,50,0,2}; vehiculos[5]=(vehiculo){12,35,55,80,1,6}; vehiculos[6]=(vehiculo){12,36,12,42,0,2}; vehiculos[7]=(vehiculo){12,37,50,90,1,4};
}

int main(void){
    config(); // Inicializaci�n
    //preCargarVehiculos(); // Precarga para simular
    while(1){
        // Si llega un paquete
        if(paqueteRecibido == 1){
            paqueteRecibido = 0; // Se baja la bandera
            // Logica del main
        }
    }
}
