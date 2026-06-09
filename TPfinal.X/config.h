#ifndef CONFIG_H
#define CONFIG_H

// Inicialización de los puertos
void Init_Puertos(void);

// Inicialización del Timer
void Init_Timer1(void);

// Inicialización de Change Notification
void Init_CN(void);

// Inicialización de la UART
void Init_UART2(void);

// Función que llama a demás inicializaciones
void config(void);

#endif