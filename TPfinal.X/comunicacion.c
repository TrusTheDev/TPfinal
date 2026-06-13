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
//Indica la cantidad de bytes que vamos enviando
int contadorByte = 0;
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0;
    unsigned char qty = bufferTX[1];
    //Como ahora mandamos de a 1 byte por cada caracter qty * 2
    if(indiceTX < (qty * 2)){
        unsigned char dato = bufferTX[contadorByte];
        unsigned char nibble;
        //Si indice es par entonces lo que se transmite es el nibble alto entonces lo muevo a la derecha
        if((indiceTX % 2) == 0){
            nibble = (dato >> 4) & 0x0F;
        }
        //si es impar lo que se transmite es el nibble bajo entonces lo muevo a la izquierda
        else{
            nibble = dato & 0x0F;
            //como ya transmitimos al menos 2 bytes para enviar aumentamos
            contadorByte++;
        }
        /*
         * Conversion hexa a a ASCII
         Esto pasa por que el valor F en hexa != 'F' entonces hay que convertir bases
         * Si nibble es: 0..9 en hexa
         * entonces no hay que usar letras sino números en ASCII
         * Si nibble es: A..F en hexa
         * entonces tenemos que usar letras
         */
        if(nibble < 10){
            U2TXREG = nibble + '0';
        }
        else{       
            U2TXREG = (nibble - 10) + 'A';
        }
        indiceTX++;
    }
    else{
        IEC1bits.U2TXIE = 0;
        indiceTX = 0;
        contadorByte = 0;
    }
}
// CAPA DE TRANSPORTE
// Función auxiliar para calcular el checksum
unsigned int calcularChecksum(unsigned char *datos, unsigned int cantidad){
    unsigned int suma = 0;
    // Se recorre el arreglo saltando de a 2 bytes
    for(unsigned int i = 0; i < cantidad - 1; i += 2) {
        unsigned int word = ((unsigned int)datos[i] << 8) | datos[i + 1]; // Une los dos bytes en uno de 16 bits
        suma += word;
    }
    // Si la cantidad es impar, el último byte queda suelto
    if(cantidad % 2 != 0){   
        unsigned int word = ((unsigned int)datos[cantidad - 1] << 8) | 0x00; // Se completa con ceros a la derecha
        suma += word;
    }
    return suma;
}
// Función que revisa el comienzo del mensaje (SOF) el destino (Dst) y el Checksum (BBC)
void capaTransporte(void){
    unsigned char qty = bufferRX[1];
    unsigned char paqueteValido = 1; // Bandera para controlar si está todo correcto
    
    // Si el SOF y/o el Dst es incorrecto
    if(bufferRX[0] != 0xFE || bufferRX[2] != 0x03){
        paqueteValido = 0; // El marco es inconsistente
    }
    // Si son correctos
    else{
        // Se revisa el BBC
        unsigned int bccCalculado = calcularChecksum(bufferRX, qty - 2);
        unsigned int bccRecibido = ((unsigned int)bufferRX[qty - 2] << 8) | bufferRX[qty - 1];
        // Si el BBC es incorrecto
        if(bccCalculado != bccRecibido){
            paqueteValido = 0; // El marco es inconsistente
        }
    }
    // Si el paquete es correcto
    if(paqueteValido == 1){
        capaAplicacion(); // Se pasa a la Capa de Aplicación
    }
    // Si el paquete es inconsistente
    else{
        bufferTX[5] = 'G'; // Se devuelve un NACK
        indiceTX = 6; // Se llenó hasta el índice 5
        construirPaquete();
    }
}

// CAPA DE APLICACION
// Función auxiliar que arma la trama final y dispara TX
void construirPaquete(void){
    bufferTX[0] = 0xFE; // SOF (Inicio de trama)
    bufferTX[1] = indiceTX + 2; // Qty es la cantidad de bytes que se llenaron + 2 bytes del Checksum
    bufferTX[2] = 0x02; // Dst (La PC tiene dirección 2)
    bufferTX[3] = 0x03; // Src (El microcontrolador tiene dirección 3)
    bufferTX[4] = 0x80; // Sec (Fijo en 80h)
    
    unsigned int checksum = calcularChecksum(bufferTX, indiceTX); // Se calcula el Checksum
    
    // Se guarda el Checksum (1 word) en 2 bytes
    bufferTX[indiceTX++] = (checksum >> 8); // Parte Alta (BCCH)
    bufferTX[indiceTX] = checksum & 0x00FF; // Parte Baja (BCCL)
    
    // Se dispara la transmisión
    indiceTX = 0; // Se empieza a enviar desde el índice 0
    IEC1bits.U2TXIE = 1; // Habilitamos la interrupción
    IFS1bits.U2TXIF = 1; // Se prende la interrupción TX levantando el flag
}

// Función que procesa los comandos y arma la respuesta
void capaAplicacion(void){
    unsigned char comando = bufferRX[5]; // Se lee el comando que llegó
    
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
            bufferTX[5] = 'F'; // Se devuelve ACK
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
            // Se ve cuantos lugares hay que revisar
            unsigned int cantidadRevisar = totalVehiculos;
            if(cantidadRevisar > 256){
                cantidadRevisar = 256; // Máximo 256
            }
            // Se recorre el historial
            for(unsigned int i = 0; i < cantidadRevisar; i++){
                // Se verifica que haya espacio en el buffer (5 bytes del auto + 2 del BCC)
                if(indiceTX + 7 <= 255){ 
                    // Se filtra por la hora pedida
                    if(vehiculos[i].hora >= bufferRX[6] && vehiculos[i].hora <= bufferRX[7]){
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