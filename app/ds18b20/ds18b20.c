/* ds18b20.c	*/
#include "ds18b20.h"
#include "intrins.h"

/*
* ��������		ds18b20_reset
* �������ܣ�	��λDS18B20
* ���룺		��
* �����		��
*/
void ds18b20_reset(void)
{
	DS18B20_PORT=0;
	delay_10us(75);		//����750us
	DS18B20_PORT=1;
	delay_10us(2);		//����20us
}

/*
* ��������		ds18b20_check
* �������ܣ�	���DS18B20�Ƿ����
* ���룺		��
* �����		1��δ��⵽DS18B20�Ĵ���
*				0������
*/
u8 ds18b20_check(void)
{
	u8 time_temp=0;
	
	while(DS18B20_PORT&&time_temp<20)		//�ȴ�DQΪ�͵�ƽ
	{
		time_temp++;
		delay_10us(1);
	}
	if(time_temp>=20) return 1;				//�߳�ʱ�˳�
	else time_temp=0;
	while((!DS18B20_PORT)&&time_temp<20)	//�ȴ�DQΪ�ߵ�ƽ
	{
		time_temp++;
		delay_10us(1);
	}
	if(time_temp>=20)	return 1;			//�͵�ƽ��ʱ�˳�
	return 0;
}


/************************************
* �� �� �� : ds18b20_init
* �������� : ��ʼ�� DS18B20 �� IO �� DQ ͬʱ��� DS �Ĵ���
* �� �� : ��
* �� �� : 1:�����ڣ�0:����
**************************************************/
u8 ds18b20_init(void)
{
	ds18b20_reset();
	return ds18b20_check();
}


/******************
*��������		ds18b20_read_bit
*�������ܣ�		��DS18B20��ȡһ��λ
*���룺			��
*�����			1/0		
*****************/
u8 ds18b20_read_bit(void)
{
	u8 dat=0;
	DS18B20_PORT=0;		 //������������1us
	_nop_();
	_nop_();
	DS18B20_PORT=1;		 //1����Ƭ����ȡǰ����1��Ȼ��ȴ��������ͻ�����
	_nop_();_nop_();
						 //2���ͷ����ߣ�����15us�ڶ�ȡ
	if(DS18B20_PORT)
		dat=1;
	else
		dat=0;
	delay_10us(5);		 //���ж�ʱ��������Ҫ60us
	return dat;
}

/******************
*��������		ds18b20_read_byte
*�������ܣ�		��DS18B20��ȡһ���ֽ�
*���룺			��
*�����			byte		
*****************/
u8 ds18b20_read_byte(void)
{
	u8 i=0;
	u8 dat=0;
	u8 temp=0;

	for(i=0;i<8;i++)	//ѭ��8�Σ��ӵ͵���װ��dat��
	{
		temp=ds18b20_read_bit();
		dat=(temp<<7)|(dat>>1);
	}
	return dat;
}

/******************
*��������		ds18b20_write_byte
*�������ܣ�		��DS18B20д��һ���ֽ�
*���룺			byte
*�����			��		
*****************/
void ds18b20_write_byte(u8 dat)
{
	u8 i=0;
	u8 temp=0;

	for(i=0;i<8;i++)
	{
		temp=dat&0x01;	//ȡ���λ
		dat>>=1;		//��������λ
		if(temp)		//д1
		{
			DS18B20_PORT=0;
			_nop_();_nop_();
			DS18B20_PORT=1;
			delay_10us(6);
		}
		else			//д0
		{
			DS18B20_PORT=0;
			delay_10us(6);
			DS18B20_PORT=1;
			_nop_();_nop_();	   			
		}
	}
}



/****************
*������	��	ds18b20_start
*�������ܣ�	��ʼת���¶�
*���룺		��
*�����		��
*****************/
void ds18b20_start(void)
{
	ds18b20_reset();	//��λ
	ds18b20_check();	//����Ƿ����
	ds18b20_write_byte(0xcc);	//SKIP ROM
	ds18b20_write_byte(0x44);	//ת������
}





/*************************
*��������	ds18b20_read_temperture
*�������ܣ�	��ds18b20��ȡ�¶�ֵ
*���룺		��
*�����		�¶����� ��float����
**************************/
float ds18b20_read_temperture(void)
{
	float temp;
	u8 data_h	=0;
	u8 data_l	=0;
	u16 value	=0;

	ds18b20_start();	//��ʼת��
	ds18b20_reset();	//������λָ��
	ds18b20_check();	//����Ƿ����18b20
	ds18b20_write_byte(0xcc);	//SKIP ROM����
	ds18b20_write_byte(0xbe);	//���洢������

	data_l=ds18b20_read_byte();	//�����ֽ�
	data_h=ds18b20_read_byte();	//�����ֽ�

	value=(data_h<<8)+data_l;	//�ϲ�Ϊ16λ����

	if((value&0xf800)==0xf800)	//�ж��Ƿ�ǰ��λΪ1�����¶�
	{
		value=(~value)+1;		//����������ȡ����+1
		temp=value*(-0.0625);	//�̶�����
	}
	else		//���¶�
	{
		temp=value*0.0625;
	}
	return temp;
}