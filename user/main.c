/**************************************************************************************
																				  
***************************************************************************************/
#include "public.h"
//#include "24c02.h"
//#include "key.h"
#include "smg.h"
#include "ired.h"
#include "uart.h"



/*******************************************************************************
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	u8 ired_buf[4];
	u8 baud=0xfa;
	uart_init(baud);
	ired_init();	

	IP = 0x10; //���ڸ����ȼ�
	//���Դ���
	/*while(1){  			
		        P2 = 0xFF;  // LEDȫ��
				uart_send(0x05);
		        delay_ms(1000);
		        P2 = 0x00;  // LEDȫ��
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
			EX0=0;//���ж�
			uart_send(gired_data[0]/16);   //��ַ
			uart_send(gired_data[0]%16);
			uart_send(gired_data[2]/16);   //������
			uart_send(gired_data[2]%16);
			EX0=1;//���ж�
			hasnewdata=0;
		}     
	}
}
