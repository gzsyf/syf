#include "project_nrf51822.c"//包含51822工程需要用到的头文件（晶振，延时，io脚什么的
#include "pca1000x.h"
#include "key_scan_1.h"
#include <stdbool.h>//调用bool
#include "stdio.h"//标准输入输出
#include <stdint.h>//声明已知大小的整数或显示特征的整数



static uint8_t btn0_nstate;     // Button0 的 new state 标志位
static uint8_t btn1_nstate;     // Button1 的 new state 标志位

static uint8_t btn0_ostate;			// Button0 的 old state 标志位
static uint8_t btn1_ostate;			// Button1 的 old state 标志位

//----------------------------------
//key初始函数
//----------------------------------
void KEY_Init(void)
{
			gpio_init_in(16,17,3);//设置16~17脚配置为输入3代表上拉电阻（开发板为按钮key0，key1）
			btn0_ostate = nrf_gpio_pin_read(BUTTON_0); // 初始 Button0 old state
			btn1_ostate = nrf_gpio_pin_read(BUTTON_1); // 初始 Button1 old state

}

//----------------------------------
//key扫描函数
//----------------------------------
uint8_t Key_Scan(void)
{
    btn0_nstate = nrf_gpio_pin_read(BUTTON_0);  // 初始 Button0 new state
    btn1_nstate = nrf_gpio_pin_read(BUTTON_1);	// 初始 Button1 new state
		
    if ((btn0_ostate == 1) && (btn0_nstate == 0))
    {
      return 1;
    }
     
    if ((btn1_ostate == 1) && (btn1_nstate == 0))
    {
      return 2;
    }
		else
		return 3 ;
    
    btn0_ostate = btn0_nstate;
    btn1_ostate = btn1_nstate;
}

