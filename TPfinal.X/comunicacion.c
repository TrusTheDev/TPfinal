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
//Aumentar bufferTX a 256? para enviar varios autos comando H
unsigned char bufferTX[256]; // Aca se arma la respuesta para la PC

// Indices para los arreglos
unsigned char indiceRX = 0;
unsigned char indiceTX = 0;

// CAPA FISICA

// Rutina de interrupción para recepción
void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void ){
    IFS1bits.U2RXIF = 0; // Se limpia la bandera de interrupción
    
    unsigned char byteRecibido = U2RXREG; // Se lee el byte que acaba de llegar
    
    // Si el índice es 0, se espera el inicio del mensaje con SOF (0xFE)
    if(indiceRX == 0){
        if(byteRecibido == 0xFE){
            bufferRX[indiceRX] = byteRecibido;
            indiceRX++;
        }
    }
    // Si ya pasó el SOF, se gusrda el dato y se verifica el tamaño
    else{
        bufferRX[indiceRX] = byteRecibido;
        indiceRX++;
        // Si ya se recibieron 2 bytes, la posición 1 indica el tamaño
        if(indiceRX > 1){
            unsigned char qty = bufferRX[1]; // Se guarda el tamaño
            // Si se alcanzó la cantidad total que pide el paquete
            if(indiceRX == qty){ 
                paqueteRecibido = 1; // Se levanta la bandera para avisarle al main
                indiceRX = 0; // Se resetea el índice para el próximo paquete
            }
        }
    }
}

//Rutina de interrupción para transmisión
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0; // Se baja la bandera de interrupción
    
    unsigned char qty = bufferTX[1]; // Se obtiene la cantidad de bytes del paquete
    
    // Si todavía no se mandaron todos los bytes
    if(indiceTX < qty){
        U2TXREG = bufferTX[indiceTX]; // Se manda el byte actual
        indiceTX++; // Se avanza al siguiente
    }
}

// CAPA DE TRANSPORTE

// Función auxiliar para calcular el checksum
unsigned int calcularChecksum(){
    
}

// Función que revisa el destino y el checksum
void capaTransporte(void){
    
}

// CAPA DE APLICACION

// Función auxiliar que arma la trama final y dispara TX
void construirPaquete(void){
    bufferTX[0] = 0xFE; // SOF (Inicio de trama)
    bufferTX[1] = indiceTX + 2; // Qty es la cantidad de bytes que se llenaron + 2 bytes del Checksum
    bufferTX[2] = 0x02; // Dst (La PC tiene dirección 2)
    bufferTX[3] = 0x03; // Src (El microcontrolador tiene dirección 3)
    bufferTX[4] = 0x80; // Sec (Fijo en 80h)
    //bufferTX[indiceTX] = calcularChecksum();
    // Falta agregar el BCC (Checksum)
    
    indiceTX = 0; // Se empieza a enviar desde el índice 0
    IFS1bits.U2TXIF = 1; // Se prende la interrupción TX levantando el flag
}

// Función que procesa los comandos y arma la respuesta
void capaAplicacion(void){
    // Se lee el comando que llegó
    unsigned char comando = bufferRX[5];
    
    switch(comando){
        // Consultar cantidad de vehículos de dos ejes
        case 'A':
            bufferTX[6] = (unsigned char) total2Ejes; // Se obtiene el dato
            bufferTX[5] = 'A'; // Se devuelve el mismo comando
            indiceTX = 7; // Se llenó hasta el índice 6
            break;
        // Resetear la cantidad de vehículos a 0 y borrar todos los registros
        case 'B':
            // Se resetean los registros
            indiceVehiculos = 0;
            totalVehiculos = 0;
            total2Ejes = 0;
            totalPesados = 0;
            totalExcesoVelocidad = 0;
            bufferTX[5] = 'B'; // Se devuelve el mismo comando
            indiceTX = 6; // Se llenó hasta el índice 5
            break;
        // Consultar cantidad de vehículos con más de dos ejes
        case 'C':
            bufferTX[6] = (unsigned char) totalPesados; // Se obtiene el dato
            bufferTX[5] = 'C'; // Se devuelve el mismo comando
            indiceTX = 7; // Se llenó hasta el índice 6
            break;
        // Accionar la cámara fotográfica
        case 'E':
            LATAbits.LATA0 = !LATAbits.LATA0; // Se dispara la cámara
            bufferTX[5] = 'E'; // Se devuelve el mismo comando
            indiceTX = 6; // Se llenó hasta el índice 5
            break;
        // Consultar cantidad de vehículos que superaron la velocidad máxima de 60km/h
        case 'I':
            bufferTX[6] = (unsigned char) totalExcesoVelocidad; // Se obtiene el dato
            bufferTX[5] = 'I'; // Se devuelve el mismo comando
            indiceTX = 7; // Se llenó hasta el índice 6
            break;
        // Consulta detallada de vehículos que pasaron entre dos horas
        case 'H':
            bufferTX[5] = 'H';
            indiceTX = 6;
            for (int i = 0; i < indiceVehiculos; i++) {
                if (indiceTX + 7 < sizeof (bufferTX)) {
                    if (vehiculos[i].hora >= bufferRX[6] &&
                            vehiculos[i].hora <= bufferRX[7]) {
                        bufferTX[indiceTX++] = vehiculos[i].hora;
                        bufferTX[indiceTX++] = vehiculos[i].minuto;
                        bufferTX[indiceTX++] = vehiculos[i].segundo;
                        bufferTX[indiceTX++] = vehiculos[i].velocidad;
                        bufferTX[indiceTX++] = vehiculos[i].ejes;
                    }
                }
            }

            break; 
        // Comando inconsistente
        default:
            bufferTX[5] = 'G'; // Se devuelve un NAK
            indiceTX = 6; // Se llenó hasta el índice 5
            break;
    }    
    construirPaquete();
}