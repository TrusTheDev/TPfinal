/*
 * File:   config.c
 * Author: trusty
 *
 * Created on June 4, 2026, 3:03 PM
 */

#include "xc.h"
#include "config.h"

/*HAY QUE VER BIEN ESTO
 * Ivan: El proyecto esta a 40 mhz.
#define FCY 40000000
#define BAUDRATE 19200
#define BRGVAL ((FCY / BAUDRATE) / 16) - 1
*/

void Init_Puertos(void){
    // Se configuran los puertos
    TRISDbits.TRISD6 = 1;  // Sensor cable 1 como entrada
    TRISDbits.TRISD7 = 1;  // Sensor cable 2 como entrada
    TRISDbits.TRISD13 = 1; // Sensor inductivo como entrada
    
    TRISAbits.TRISA0 = 0; // Cï¿½mara fotogrï¿½fica como salida
    LATAbits.LATA0 = 0; // Arranca apagada
}

void Init_Timer1(void){
    // Se configura Timer1 para medir el tiempo entre sensores
    T1CONbits.TON = 0; // Se asegura que estï¿½ apagado para configurarlo
    T1CONbits.TCS = 0; // Reloj interno
    //T1CONbits.TCKPS = ; // Hay que calcularlo
    //PR1 = 65535 ; // Hay que calcularlo
    //Ivan: Se supone que solamente tenemos que apagarlo y resetearlo-
    //cuando es requerido, utilizo el máximo.
    //Capaz es una buena idea tener un contador de overflows para tener mejor el tiempo?
    TMR1 = 0; // Se resetea el cronï¿½metro a 0

    // Se configura la interrupciï¿½n
    IPC0bits.T1IP = 1; // Se pone prioridad 1 al Timer1
    IFS0bits.T1IF = 0; // Se limpia el Interrupt Flag para arrancar en limpio
    IEC0bits.T1IE = 1; // Se habilita la interrupciï¿½n Timer1
    T1CONbits.TON = 0; // Se va a prender cuando el vehiculo pise el primer sensor
}
//Ojo, el timer esta configurado para 40mhz
void Init_Timer6( void )
{
	/* ensure Timer 6 is in reset state */
	T6CON = 0;
	T6CONbits.TCKPS = 1; //Prescaler 8
	/* reset Timer 6 interrupt flag */
 	IFS2bits.T6IF = 0;
 	/* set Timer interrupt priority level */
	IPC11bits.T6IP = 5;
	/* enable Timer interrupt */
 	IEC2bits.T6IE = 1;
	/* set Timer period register */
	PR6 = 4999;
	T6CONbits.TON = 1; 	//habilito Timer
}

void Init_CN(void){
    IFS1bits.CNIF = 0; // Se limpia la bandera
    
    // Estos pines avisan cuando cambian de estado
    CNEN1bits.CN15IE = 1; // Habilita RD6 (Sensor 1)
    CNEN2bits.CN16IE = 1; // Habilita RD7 (Sensor 2)
    CNEN2bits.CN19IE = 1; // Habilita RD13 (Sensor inductivo)
    
    IPC4bits.CNIP = 4; // Prioridad 4
    
    IEC1bits.CNIE = 1; // Se habilita el CN
}

void Init_UART2(void){
    U2MODEbits.UARTEN = 0; // Se apaga el mï¿½dulo para configurarlo
    
    /* HAY QUE VER BIEN ESTO
    // Configuraciï¿½n de la trama
    U2MODEbits.BRGH = 0; // Modo de velocidad estandar
    U2MODEbits.PDSEL = 0; // 8 bits, sin paridad
    U2MODEbits.STSEL = 0; // 1 bit de stop
	U2MODEbits.RTSMD = 1; // Modo simplex
    */ 

	//U2BRG = BRGVAL;	// Se carga el valor del cï¿½lculo de baudios
	IPC7 = 0x4400; // Se configura la prioridad de interrupciï¿½n

	IFS1bits.U2RXIF = 0; // Se limpia la bandera de recepciï¿½n
	IEC1bits.U2RXIE = 1; // Se prende la interrupciï¿½n de recepciï¿½n

	U2MODEbits.UARTEN = 1; // Se prende el mï¿½dulo UART
	U2STAbits.UTXEN = 1; // Empieza a transmitir. Se dispara el Flag TXIF

	IFS1bits.U2TXIF = 0; // Se limpia la bandera de transmisiï¿½n
    IEC1bits.U2TXIE = 1; // Se prende la interrupciï¿½n de transmisiï¿½n
}

void config(void){
    Init_Puertos();
    Init_Timer1();
    Init_Timer6();
    Init_CN();
    Init_UART2();
}