 #include "public.h"
 #include "uart.h"


 void uart_init(u8 baud)
 {
  	TMOD=0x20;	//�趨������1����ģʽ2
	SCON=0x50;	//�趨���ڹ�����ʽ1
	PCON=0x80;	//�����ʼӱ�
	TH1=baud;
	TL1=baud;
	ES=1;		//�򿪴��ڽ����ж�
	EA=1;		//�����ж�
	TR1=1;		//�򿪶�ʱ��1
 }

void uart_isr() interrupt 4 
 {
 	u8 mdata;
    if (RI) {
        RI = 0;     // ��������жϱ�־
        mdata = SBUF; // ��ȡ���ݣ���ѡ��
    }
    if (TI) {
        TI = 0;     // ��������жϱ�־
    }
}

 void uart_send(u8 send_data) 
 {
	u8 timeout=0;
    SBUF = send_data;// ��������
    while(!TI) {            // ���ӳ�ʱ����
        delay_10us(10);     // ������ʱ
        if (timeout++ > 100) break; // ��ʱ�˳�
    }
    TI = 0;          // �ٴ����  
 }

 