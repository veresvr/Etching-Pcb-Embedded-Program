#include "stm8s.h"
#include "stm8s_gpio.h"
#include <stm8s_clk.h>
#include <veres_sound_stm8.h>
#include "veres_debug_via_UART_stm8.h"
#include "veres_timer2_stm8.h"
#include "veres_err_list_stm8.h"
#include "veres_ds18b20_stm8.h"



#define	DELAY_OF_DATA		20		// time of delay before next data packet
#define	LENGHT	                30		// value of array
#define	DATA_INC_READY          1		// ready for parsing packet
#define	DATA_INC_NOREADY        0		// not ready for parsing packet


// variables
uint8_t lenghtOfDataPacket = 0,
	receive_array[LENGHT],
	status = 0,
        statusData = DATA_INC_NOREADY,
        err_code = NO_ERROR,
        lenghtData = 0;

// structures
struct flag{
  unsigned changeStatus: 1; 
  unsigned soundAtEnd: 1;
  unsigned soundAtError: 1;  
} sound_flag;

// interrupt definitions
 INTERRUPT_HANDLER( UART1_RX, 0x12 );
 INTERRUPT_HANDLER( TIM2_OVF, 13 );
  
int main( void )
{
  CLK->CKDIVR &= ~(CLK_CKDIVR_HSIDIV);    // fHSI= fHSI RC output (configure to 16 MHz)
  sound_flag.changeStatus = 0;
  sound_flag.soundAtEnd = 0;
  sound_flag.soundAtError = 0;
  
  UART_Init();
  TIMER2_Init();
  initSound();
  rim();  
  
//  for led
  GPIOB->ODR |= (GPIO_PIN_5);
  GPIOB->DDR |= (GPIO_PIN_5);
  GPIOB->CR1 |= (GPIO_PIN_5);  
  GPIOB->CR2 |= (GPIO_PIN_5);
  
  
  
  while(1){
    
    if(statusData == DATA_INC_READY){
      
      statusData = DATA_INC_NOREADY;
      // UART interrupt must be OFF, for parsing reseived data packet
      if(lenghtData < 3)                                          // check for minimal lenght of data   ok
      { 
        err_code = COMMAND_LESS_3_CHAR;
        UART_sendString("COMMAND_LESS_3_CHAR  ");	          // these and next transmissions only for debug
        if (sound_flag.soundAtError == 1) makeSound(S_ERROR);
        continue;
      }
			
      if((receive_array[0] != '[') || (receive_array[lenghtData-1] != ']'))   //check first and last char of data   ok
      {
        err_code = START_STOP_UNCORR; 
        UART_sendString("START_STOP_UNCORR  ");                   // only for debug, clear before release!
        if (sound_flag.soundAtError == 1) makeSound(S_ERROR);
        continue;
      } 
      

      if((receive_array[1] == 'R') && (receive_array[2] == 'T'))   // read temperature command        half
      {
        // code for read temperature
        if (sound_flag.changeStatus == 1) makeSound(S_CHANGE_STATUS);    
        continue;
      }
      
      if((receive_array[1] == 'S') && (receive_array[2] == 'A'))   // change status sound        ok
      {
        if ((receive_array[3] == 'E'))  sound_flag.changeStatus = 1;
        if ((receive_array[3] == 'D'))  sound_flag.changeStatus = 0;
        
        makeSound(S_CHANGE_STATUS);    
        continue;
      }   
      
      if((receive_array[1] == 'B') && (receive_array[2] == 'C'))   // 
      {
        makeSound(S_AT_END_PROGR);
        continue;
      }      
      
      if((receive_array[1] == 'S') && (receive_array[2] == 'E'))   // errors occur sound        ok
      {
        if ((receive_array[3] == 'E'))  sound_flag.soundAtError = 1;
        if ((receive_array[3] == 'D'))  sound_flag.soundAtError = 0;
        
        if (sound_flag.changeStatus == 1) makeSound(S_CHANGE_STATUS);    
        continue;
      }

      if((receive_array[1] == 'B') && (receive_array[2] == 'E'))   // blow state toggle        half
      {
        // code for blow
        if (sound_flag.changeStatus == 1) makeSound(S_CHANGE_STATUS);    
        continue;
      }    
      
      if((receive_array[1] == 'I') && (receive_array[2] == 'D'))   // check ID of device        half
      {
        UART_sendString("[l9]");
        if (sound_flag.changeStatus == 1) makeSound(S_CHANGE_STATUS);    
        continue;
      }

      
    }  
      
      
//  UART_sendOnlyNumber(receive_array[lenghtData-1]);
//  delay();
//  delay();  
  
    
    
    
    

  
  }  
  return 0;
}
__interrupt void UART1_RX( void )
{  
  uint8_t received_data = 0;
	
  TIMER2_stop();
  received_data = UART1->DR;
  receive_array[lenghtOfDataPacket] = received_data;
  lenghtOfDataPacket++;
	
  TIMER2_wait_msec(DELAY_OF_DATA);  
}

__interrupt void TIM2_OVF( void )
{
  TIM2->SR1 &= ~TIM2_SR1_UIF;                           // clear an interrupt flag
  
  TIMER2_stop();
  UART_sendString("Time is gone. Data receive is ");    // only for debug, clear before release!
  
  if (lenghtOfDataPacket == 0){
    UART_sendString("zero");                            // only for debug, clear before release!
    statusData = DATA_INC_NOREADY;
  } else {
    UART_sendOnlyNumber(lenghtOfDataPacket+48);         // only for debug, clear before release!
    statusData = DATA_INC_READY;
  }
  
  lenghtData = lenghtOfDataPacket;
  lenghtOfDataPacket = 0;

}