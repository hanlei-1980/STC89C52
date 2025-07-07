 #include "public.h"
 #include "uart.h"


 void uart_init(u8 baud)
 {
  	TMOD=0x20;	//设定计数器1工作模式2
	SCON=0x50;	//设定串口工作方式1
	PCON=0x80;	//波特率加倍
	TH1=baud;
	TL1=baud;
	ES=1;		//打开串口接收中断
	EA=1;		//打开总中断
	TR1=1;		//打开定时器1
 }

void uart_isr() interrupt 4 
 {
 	u8 mdata;
    if (RI) {
        RI = 0;     // 清除接收中断标志
        mdata = SBUF; // 读取数据（可选）
    }
    if (TI) {
        TI = 0;     // 清除发送中断标志
    }
}

 void uart_send(u8 send_data) 
 {
	u8 timeout=0;
    SBUF = send_data;// 启动发送
    while(!TI) {            // 增加超时保护
        delay_10us(10);     // 短暂延时
        if (timeout++ > 100) break; // 超时退出
    }
    TI = 0;          // 再次清除  
 }

 