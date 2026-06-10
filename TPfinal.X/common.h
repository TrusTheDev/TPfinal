#ifndef COMMON_H
#define COMMON_H

// Variables globales

// Estructura para cada auto
typedef struct{
    unsigned char hora;
    unsigned char minuto;
    unsigned char segundo;
    unsigned char velocidad;
    unsigned char infraccion;
    unsigned char ejes;
} vehiculo;

extern vehiculo vehiculos[256]; // Arreglo circular de estructuras
extern unsigned char indiceAutos; // Indice para el arreglo (0 a 255)

// Contadores
extern unsigned int totalVehiculos; 
extern unsigned int total2Ejes;
extern unsigned int totalPesados;
extern unsigned int totalExcesoVelocidad;

// Banderas
extern unsigned char paqueteRecibido; 

#endif