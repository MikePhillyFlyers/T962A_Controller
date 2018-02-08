#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */


#ifndef UARTS_H
#define UARTS_H



#define UART_SUCCESS 0

/* Settings */
/* NOTE: baud rate lower than 9600 / 15200 might not work */
#ifndef BAUD_LOW
  #define BAUD_LOW  9600
#endif
#ifndef BAUD_HIGH
  #define BAUD_HIGH 115200
#endif




/* function to init UART1 */
extern InitUART1(uint32_t uiBaudRate);

/* function to adjust uart1 baud rate */
extern AdjustUART1(uint32_t uiBaudRate);

/* function to send char to UART */
extern SendToHost(unsigned char ucSendChar);

/* function to receive from UART */
extern uint32_t ReceiveFromHost(unsigned char* pRcvdChar);



#define uart_putc(char) SendToHost(char);
#define uart_getc(char) ReceiveFromHost(char);

/* UART implemention for different platforms */
int           uart_init(char *dev_path);
void          uart_finit(void);
void          uart_setb(uint32_t baud);

//void          uart_putc(unsigned char c);
//unsigned char uart_getc(void);




#endif // UARTS_H