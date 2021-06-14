/*	discription:
	library using for send data via UART. stm8
				
	was create:	  28.07.14
	rewrite to stm8	  16.11.17
	update:		  19.11.17
	status: 	  WORK

	need make:	  none	

	other notes:      use as #include "veres_debug_via_UART_stm8.h"
	baud rate = 9600
	data bits 8 
	parity = none
	stop bit = 1
	handshaking = none


For using interrupts, u must write next code:

  rim(); 

  INTERRUPT_HANDLER( UART1_RX, 0x12 );

  __interrupt void UART1_RX( void )
  {
    uint8_t symbol;
    symbol = UART1->DR;
    UART1->DR = symbol;
  }

*/
#include <stm8s_gpio.h>
//#include <C:\Program Files\IAR Systems\Embedded Workbench 6.0\stm8\STM8S_StdPeriph_Driver\inc\stm8s_clk.h>


//void UART_sendOnlyNumber(uint32_t message);
//void UART_Init(void);
//void UART_sendString(char * Text);
//void UART_sendNumber(int32_t number);								


/*
    EXAMPLE OF USE
#define debugOnUART

#ifdef  debugOnUART
UART_sendString(message);
#endif

*/

//void UART_sendOnlyNumber(uint32_t message)
//{
//  while(((UART1->SR) & UART1_FLAG_TC) != UART1_FLAG_TC);	    //wait until Transmission is complete			  
//  UART1->DR = (unsigned char)message; 
//}
//
//void UART_Init(void)
//{	
//	// clock 
//  CLK->PCKENR1 |= (1<<CLK_PERIPHERAL_UART1);             //  /*!< USART1 clock enable */	
//	
//	// pins
//  GPIOD->ODR &= ~(GPIO_PIN_5);        // output data register
//  GPIOD->DDR |= (GPIO_PIN_5);         // data direction register
//  GPIOD->CR1 |= (GPIO_PIN_5);         // control register 1
//  GPIOD->CR2 |= (GPIO_PIN_5);         // control register 2
//  
//	// configure UART
//  UART1->BRR2 = 0x03;                 // setting a 9600 speed to UART
//  UART1->BRR1 = 0x68;                 //
//
//  UART1->CR2 |= //UART1_CR2_TIEN|       // Transmitter interrupt enable
//                UART1_CR2_RIEN|       // Receiver interrupt enable
//                UART1_CR2_TEN|        // Transmitter enable
//                UART1_CR2_REN;       // Receiver enable
//}
//
//void UART_sendString(char * Text)
//{
//  char i = 0;
//     
//  for (i = 0; Text[i] != 0; i++)
//  {		
//    while(((UART1->SR) & UART1_FLAG_TC) != UART1_FLAG_TC);	    //wait until Transmission is complete		  
//    UART1->DR = Text[i];   
//  }
//}		
//
//void UART_sendNumber(int32_t number)
//{
//  char 	array[10], 				// 10 units i think are enought
//	i = 0;	
//  
//// erase array
//  for (i = 0; i < 10; i++)  array[i] = 0;
//	
//// main algorithm
//  i = 0;
//  while (number > 0)
//  {
//    array[i] = number % 10;		 		  
//    i++;
//    number = number / 10;      
//  }	
//	
//// send to UART
//  for (; i != 0; i--)
//  {
//    while(((UART1->SR) & UART1_FLAG_TC) != UART1_FLAG_TC);	    //wait until Transmission is complete			  
//    UART1->DR = (array[i-1])+48;     	
//  }	
//}



/*
if (((UART1->SR) & UART1_FLAG_RXNE) != 0)   - if we receive something












*/
