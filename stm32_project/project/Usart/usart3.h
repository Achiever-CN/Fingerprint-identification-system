#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
	   

#define USART3_REC_LEN  			200  	
#define EN_USART3_RX 			1
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; 
extern u16 USART3_RX_STA; 

void USART3_SendString(u8 *DAT,u8 len);
void USART3_SendData(u8 data);
void USART3_Init(u32 bound);
#endif















