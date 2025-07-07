#include "public.h"
#include "reg52.h"

#ifndef UART_H_
	#define UART_H_
	
	
	void uart_init(u8 baud);
 	void uart_send(u8 send_data);

#endif						