/* ds18b20.c	*/
#include "ds18b20.h"
#include "intrins.h"

/*
* 函数名：		ds18b20_reset
* 函数功能：	复位DS18B20
* 输入：		无
* 输出：		无
*/
void ds18b20_reset(void)
{
	DS18B20_PORT=0;
	delay_10us(75);		//拉低750us
	DS18B20_PORT=1;
	delay_10us(2);		//拉高20us
}

/*
* 函数名：		ds18b20_check
* 函数功能：	检测DS18B20是否存在
* 输入：		无
* 输出：		1：未检测到DS18B20的存在
*				0：存在
*/
u8 ds18b20_check(void)
{
	u8 time_temp=0;
	
	while(DS18B20_PORT&&time_temp<20)		//等待DQ为低电平
	{
		time_temp++;
		delay_10us(1);
	}
	if(time_temp>=20) return 1;				//高超时退出
	else time_temp=0;
	while((!DS18B20_PORT)&&time_temp<20)	//等待DQ为高电平
	{
		time_temp++;
		delay_10us(1);
	}
	if(time_temp>=20)	return 1;			//低电平超时退出
	return 0;
}


/************************************
* 函 数 名 : ds18b20_init
* 函数功能 : 初始化 DS18B20 的 IO 口 DQ 同时检测 DS 的存在
* 输 入 : 无
* 输 出 : 1:不存在，0:存在
**************************************************/
u8 ds18b20_init(void)
{
	ds18b20_reset();
	return ds18b20_check();
}


/******************
*函数名：		ds18b20_read_bit
*函数功能：		从DS18B20读取一个位
*输入：			无
*输出：			1/0		
*****************/
u8 ds18b20_read_bit(void)
{
	u8 dat=0;
	DS18B20_PORT=0;		 //拉低总线至少1us
	_nop_();
	_nop_();
	DS18B20_PORT=1;		 //1、单片机读取前先置1，然后等待被动拉低或拉高
	_nop_();_nop_();
						 //2、释放总线，并在15us内读取
	if(DS18B20_PORT)
		dat=1;
	else
		dat=0;
	delay_10us(5);		 //所有读时序至少需要60us
	return dat;
}

/******************
*函数名：		ds18b20_read_byte
*函数功能：		从DS18B20读取一个字节
*输入：			无
*输出：			byte		
*****************/
u8 ds18b20_read_byte(void)
{
	u8 i=0;
	u8 dat=0;
	u8 temp=0;

	for(i=0;i<8;i++)	//循环8次，从低到高装入dat中
	{
		temp=ds18b20_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;
}

/******************
*函数名：		ds18b20_write_byte
*函数功能：		向DS18B20写入一个字节
*输入：			byte
*输出：			无		
*****************/
void ds18b20_write_byte(u8 dat)
{
	u8 i=0;
	u8 temp=0;

	for(i=0;i<8;i++)
	{
		temp=dat&0x01;	//取最低位
		dat>>=1;		//数据右移位
		if(temp)		//写1
		{
			DS18B20_PORT=0;
			_nop_();_nop_();
			DS18B20_PORT=1;
			delay_10us(6);
		}
		else			//写0
		{
			DS18B20_PORT=0;
			delay_10us(6);
			DS18B20_PORT=1;
			_nop_();_nop_();	   			
		}
	}
}



/****************
*函数名	：	ds18b20_start
*函数功能：	开始转换温度
*输入：		无
*输出：		无
*****************/
void ds18b20_start(void)
{
	ds18b20_reset();	//复位
	ds18b20_check();	//检查是否存在
	ds18b20_write_byte(0xcc);	//SKIP ROM
	ds18b20_write_byte(0x44);	//转换命令
}





/*************************
*函数名：	ds18b20_read_temperture
*函数功能：	从ds18b20读取温度值
*输入：		无
*输出：		温度数据 ，float类型
**************************/
float ds18b20_read_temperture(void)
{
	float temp;
	u8 data_h	=0;
	u8 data_l	=0;
	u16 value	=0;

	ds18b20_start();	//开始转换
	ds18b20_reset();	//发出复位指令
	ds18b20_check();	//检查是否存在18b20
	ds18b20_write_byte(0xcc);	//SKIP ROM命令
	ds18b20_write_byte(0xbe);	//读存储器命令

	data_l=ds18b20_read_byte();	//读低字节
	data_h=ds18b20_read_byte();	//读高字节

	value=(data_h<<8)+data_l;	//合并为16位数据

	if((value&0xf800)==0xf800)	//判断是否前五位为1，负温度
	{
		value=(~value)+1;		//负数，补码取反再+1
		temp=value*(-0.0625);	//固定精度
	}
	else		//正温度
	{
		temp=value*0.0625;
	}
	return temp;
}