#include "project_nrf51822.c"//包含51822工程需要用到的头文件（晶振，延时，io脚什么的
#include "nRF_DS18B20.h"
#include <stdbool.h>//调用bool
#include "stdio.h"//标准输入输出
#include <stdint.h>//声明已知大小的整数或显示特征的整数



//------------------------------------------
//DS18b20初始化函数
//------------------------------------------
static uint8_t Ds18b20Init(uint8_t DataIO)
{
	uint8_t i = 0;

	gpio_init_out( DataIO , DataIO );		// 数据口为输出
	nrf_gpio_pin_set( DataIO );					//	先置位再清零	
	nrf_gpio_pin_clear( DataIO );				// 时序要求将总线拉低480us~960us
	
	nrf_delay_us(750);									// 实际延时750us，符合480-960之间的条件
	
	nrf_gpio_pin_set( DataIO );					// 然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线被拉低

	i = 0;
	gpio_init_in( DataIO , DataIO ,0);							// 数据口为输入 无拉
	while (nrf_gpio_pin_read( DataIO ) == 1)				// 等待DS18B20拉低总线
	{
		i++;
		if(i>5)														// 等待 15*5=75us，如果还没拉低则可以认为初始化失败了
		{
			nrf_gpio_pin_set(19);						//初始化失败LED1亮
			return 1;												// 初始化失败
		}
		nrf_delay_us(15);									// 隔15us查看一下是否收到DS18B20的回应
	}
	nrf_gpio_pin_clear(19);
	nrf_gpio_pin_set(18);								//初始化成功LED0亮	
	
	gpio_init_out( DataIO , DataIO );		//将数据口变回输出
	nrf_gpio_pin_set( DataIO );					//输出口重新拉高 单线通信数据线常态为高
	
	return 0;														//初始化成功
}


//------------------------------------------
//DS18b20写命令函数
//------------------------------------------
static void Ds18b20WriteByte(uint8_t dat, uint8_t DataIO)
{
	uint16_t i = 0, j = 0;
	gpio_init_out( DataIO , DataIO );			//将数据口配置输出

	for (j=0; j<8; j++)
	{
		nrf_gpio_pin_clear( DataIO );	     	// 每写入一位数据之前先把总线拉低1us
		i++;
		if(dat&(1<<j))   									 	//写数据，先写低位
		{
			nrf_gpio_pin_set( DataIO );				//如果dat为1 DataIO置1否则置0
		}
		else nrf_gpio_pin_clear( DataIO );	
		
		nrf_delay_us(60);										// 时序要求最少60us
		nrf_gpio_pin_set( DataIO );					// 然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值

	}
	nrf_gpio_pin_set( DataIO );						//输出口重新拉高 单线通信数据线常态为高
}


//------------------------------------------
//DS18b20读数据函数
//------------------------------------------
uint8_t Ds18b20ReadByte(uint8_t DataIO)
{
	uint8_t byte = 0, bi = 0;
	uint16_t i = 0, j = 0;
	
	gpio_init_out( DataIO , DataIO ); 						//将数据口配置输出
		
	for (j=0; j<8; j++)
	{
		nrf_gpio_pin_clear( DataIO );								// 先将总线拉低1us
		i++;
		nrf_gpio_pin_set( DataIO );		  						// 然后释放总线
		i++;
		i++;																				// 延时6us等待数据稳定
		
		gpio_init_in( DataIO , DataIO ,0);					//将数据口配置输入
		bi = nrf_gpio_pin_read( DataIO );	 					// 读取数据，从最低位开始读取
		
		if(bi) byte |= (1<<j);  										//将数据写入byte变量中
		else 	 byte &= ~(1<<j);
		nrf_delay_us(50);
		
		gpio_init_out( DataIO , DataIO );
	}				
	return byte;																	//返回byte变量
}


//------------------------------------------
//DS18b20转换温度命令
//------------------------------------------
void Ds18b20TempConvertCmd(uint8_t DataIO)
{
	Ds18b20Init( DataIO );
	nrf_delay_ms(1);
	Ds18b20WriteByte(0xcc, DataIO);			// 跳过ROM操作命令		 
	Ds18b20WriteByte(0x44, DataIO);	    // 温度转换命令
	nrf_delay_ms(750);					// 等待转换成功，750ms肯定够了
}

//------------------------------------------
//DS18b20读取温度命令
//------------------------------------------
void Ds18b20TempReadCmd(uint8_t DataIO)
{	
	Ds18b20Init( DataIO );
	nrf_delay_ms(1);
	Ds18b20WriteByte(0xcc, DataIO);	 	// 跳过ROM操作命令
	Ds18b20WriteByte(0xbe, DataIO);	 	// 发送读取温度命令
}

//------------------------------------------
//DS18b20计算温度函数
//------------------------------------------
uint8_t* Ds18b20CalculateTemp(uint8_t DataIO)
{
	uint16_t temp = 0;	 				// 用来暂存12位的AD值
	uint8_t tmh = 0, tml = 0;		// 用来暂存2个8位的AD值
	uint16_t tDisp = 0;					// 用来存储乘以100倍后的温度值
	double t = 0;								// 用来存储转换后以摄氏度为单位的温度值
	uint8_t A1,A2,A3,A4;				// 用来计算温度各位的数值
	uint8_t temper[10];
	uint8_t *p;
	
	Ds18b20TempConvertCmd( DataIO );		// 先写入转换命令
	Ds18b20TempReadCmd( DataIO );			// 然后等待转换完后发送读取温度命令
	
	tml = Ds18b20ReadByte( DataIO );		// 读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte( DataIO );		// 再读高字节

	temp = tml | (tmh << 8);		// 默认是12位分辨率，前面4个S位是符号位
	
	// 正温度时符号位为0，下面代码计算没有考虑负温度情况，因为我们实验是在
	// 室温下做的，如果要考虑到负温度的情况，代码中要先判断S位，若S位为1则
	// 必须点去掉S的1再计算，计算后的值加负号即可。
	t = temp * 0.0625;
	tDisp = (uint16_t)(t * 100);			// 为方便显示将温度值乘以100后强转为u16
		
	A1 = tDisp/1000;
	A2 = (tDisp%1000)/100;
	A3 = (tDisp%100)/10;
	A4 = tDisp%10;
	sprintf(temper,"*%1d%1d.%1d%1d°C ",A1,A2,A3,A4);
	p = temper;
	
	return p;
}
