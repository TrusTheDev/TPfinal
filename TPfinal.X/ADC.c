/*
 * File:   config.c
 * Author: Trusty
 *
 * Created on 3 de junio de 2026, 16:44
 */


#include "xc.h"
#include "ADC.h"

void InitADC(void) {
    AD1PCFGL = 0xFFFC; // AN0 y AN1 como analógicos. El resto digitales
    AD1PCFGH = 0xFFFF;
    AD2PCFGL = 0xFFFF;
    
    AD1CON1bits.ASAM = 1; // El sampling comienza automáticamente después de la conversión anterior
    AD1CON1bits.SSRC = 7; // Espera un tiempo y pasa de sampling a conversión
    AD1CON1bits.FORM = 0; // Entero sin signo
    AD1CON1bits.AD12B = 0; // 10 bits
    AD1CON1bits.ADDMABM = 1; // El DMA guarda los datos en orden de conversión
    
    AD1CON2bits.SMPI = 0; // Avisa al DMA cada conversión
    AD1CON2bits.CHPS = 0; // Se usa el canal CH0
    AD1CON2bits.CSCNA = 1; // Habilita el escaneo de varios pines (AN0 y AN1)
    
    AD1CON3bits.ADCS = 0x3F; // Reloj de conversión lento para asegurar precisión
    AD1CON3bits.SAMC = 31; // Tiempo de muestreo lento para cargar bien el capacitor
    
    AD1CSSL = 0x0003; // Escaneo automático para AN0 y AN1
}

void InitDMA(void) {
    DMA0CONbits.MODE = 0; // Modo continuo (llena el buffer y vuelve a empezar)
    DMA0CONbits.AMODE = 0; // Post-incremento (avanza casillero por casillero)
    DMA0CONbits.DIR = 0; // Lee desde el ADC y escribe en la RAM
    DMA0CONbits.SIZE = 0; // Mueve palabras de 16 bits
            
    DMA0REQbits.IRQSEL = 13; // Conecta el canal 0 del DMA a la interrupción del ADC1
    DMA0PAD = (unsigned int) &ADC1BUF0; // De donde se saca el dato
    //DMA0STA = __builtin_dmaoffset(temperaturas); // Donde lo guarda en la RAM
    DMA0CNT = 5; // Mueve 6 datos antes de interrumpir a la CPU
    
    IFS0bits.DMA0IF = 0; // Se limpia la bandera
    IEC0bits.DMA0IE = 1; // Se habilita el DMA para que interrumpa a la CPU al terminar
    DMA0CONbits.CHEN = 1; // Se prende el canal del DMA
    
    AD1CON1bits.ADON = 1; // Se prende el ADC
}
