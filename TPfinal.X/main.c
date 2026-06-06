/*
 * File:   main.c
 * Author: Trusty
 *
 * Created on 3 de junio de 2026, 16:44
 */


#include "xc.h"
#include "config.h"

//Configuracion de puertos e inicio del programa

typedef struct vehiculo {
    int velocidad;
    int ejes;
    int timestamp;
    int hora;
    int minuto;
    int segundo;
} vehiculo;
int totalVehiculos;
int total2Ejes;
int totalPesados;
int totalExcesoVelocidad;

vehiculo vehiculos[100];
// Indice X es siempre la posicion 0
// Indice Y apunta al ultimo vehiculo registrado
int indiceX = 0;
int indiceY = 0;
//Precargar arreglo con vehiculos (opcional)



void preCargar(void){
    
}

void registrarVehiculo(void){
    
}

void obtenerCantidad2Ejes(void){
    
}

void obtenerCantidadPesados(void){
    
}

int obtenerCantidadExceso(void){
    
}

int buscarVehiculosEntreHoras(void){
    
}
int main(void) {
    config();
    while (1) {
    };
}
