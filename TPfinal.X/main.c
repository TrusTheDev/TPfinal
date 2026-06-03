/*
 * File:   main.c
 * Author: ivanb
 *
 * Created on 3 de junio de 2026, 16:44
 */


#include "xc.h"
#include "UART.h"
#include "TIMER.h"
#include "ADC.h"

//Colocar rutinas de atencion aqui
//Configuracion de puertos e inicio del programa

int main(void) {
    InitADC();
    InitDMA();
    Init_Timer1();
    Init_INT1();
    InitUART();
    while (1) {
    };
}
