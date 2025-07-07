/**************************************************************************************
																				  
***************************************************************************************/
#include "public.h"
//#include "24c02.h"
//#include "key.h"
#include "smg.h"
#include "ired.h"
#include "uart.h"



/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	u8 ired_buf[4];
	u8 baud=0xfa;
	uart_init(baud);
	ired_init();	

	IP = 0x10; //串口高优先级
	//测试串口
	/*while(1){  			
		        P2 = 0xFF;  // LED全亮
				uart_send(0x05);
		        delay_ms(1000);
		        P2 = 0x00;  // LED全灭
				uart_send(0x03);
		        delay_ms(1000);

			 }*/

	while(1)
	{

		ired_buf[0]=gsmg_code[gired_data[2]/16];
		ired_buf[1]=gsmg_code[gired_data[2]%16];
		ired_buf[2]=gsmg_code[gired_data[0]/16];
		ired_buf[3]=gsmg_code[gired_data[0]%16];
		smg_display(ired_buf,5);
		
		if (hasnewdata>0){
			EX0=0;//关中断
			uart_send(gired_data[0]/16);   //地址
			uart_send(gired_data[0]%16);
			uart_send(gired_data[2]/16);   //控制码
			uart_send(gired_data[2]%16);
			EX0=1;//开中断
			hasnewdata=0;
		}     
	}
}
