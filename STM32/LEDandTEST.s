;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;LED闪烁实验;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;LED1闪烁频率1Hz，LED2闪烁频率2Hz,LED3闪烁频率4Hz;;;
;;基于状态转换图,S1->S6六种状态					  ;;;
;;2011-5-3 by 追梦                                ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;----------- 定义"位" 方便后面&|运算 --------------
BIT5   EQU 0X00000020		;BIT5=0X00000020 EQU：伪指令表示等于
BIT8   EQU 0X00000100

	
;----------- 各种定义  --------------
LED1   EQU BIT8		      	;LED1--PD.8
IOPDEN 	   EQU BIT5		   	;GPIOD使能位

;----------- 定义各寄存器的地址  --------------
GPIOD      EQU 0X40011400  	;GPIOD 地址
GPIOD_CRL  EQU 0X40011400  	;GPIOD低配置寄存器地址
GPIOD_CRH  EQU 0X40011404  	;GPIOD高配置寄存器地址
GPIOD_ODR  EQU 0X4001140C  	;输出高低电平寄存器地址
GPIOD_BSRR EQU 0X40011410  	;低置位，高清除偏移地址10h
GPIOD_BRR  EQU 0X40011414  	;清除，偏移地址14h
RCC_APB2ENR EQU 0X40021018	;时钟配置寄存器地址


;-------------- 初始化栈与数据段 -----------------------
STACK_TOP EQU 0X20002000			;定义栈顶并初始化
    AREA RESET,CODE,READONLY		;定义 RESET(系统默认入口)段 代码段 默认是只读
									;AREA：定义一个代码段或数据段指令
	DCD STACK_TOP 					;MSP主堆栈指针
	DCD START	  					;复位，PC初始值
									;DCD：伪指令用于分配一片连续的字存储单元
	ENTRY         					;指示开始执行
									;ENTRY：用于指定汇编程序的入口点

;---------------- 主函数 ----------------------------------
START								;START：伪指令相当于main
	
	LDR    R1,=RCC_APB2ENR		; =：把label的值放入R1
	LDR    R0,[R1]	    		;LDR：把R1的值放入R0
	LDR    R2,=IOPDEN
	ORR    R0,R2				;改R0
								;ORR：R2与R0 或运算 放到R0
	STR    R0,[R1]				;写，使能GPIOD时钟
								;STR：将r0的值，传送到地址值为r1的RCC_APB2ENR
	
	;MOV(或LDR)+LDR+STR 这种结构就是把0x333这个值放进GPIOD_CRH寄存器当中
	;不过是汇编 要写很多段代码 C就一句GPIOD_CRH=0x333
	MOV    R0,#0x333			
								;MOV：把0x333这个立即数放进R0
								;实验得出MOV指令并没有立即数只能为8位这个限制
	LDR    R1,=GPIOD_CRH 		;PD.8\9\10均在高寄存器
	STR    R0,[R1]				;LED1--PD.8  推挽输出，50MHz

	LDR    R1,=GPIOD_ODR		;R1暂时放GPIOD_ODR寄存器
	LDR    R2,=0x00000F00		;R2暂时放0x00000F00
	MOV    R3,#0				;R3=1 为下面循环做准备 每循环一次+1 来计算LED量和灭
	
LOOP							;LOOP：循环伪指令
	STR    R2,[R1]				;GPIOD_ODR=R2 这是反转的关键
	MOV    R0,#125				;R0 延迟125ms
	;BL     DELAY_NMS			;跳转到DELAY_NMS函数
								;BL：带链接跳转指令
								;bl将下一条指令先存到r14寄存器 然后再执行跳转 用于调用子程序
	ADD    R3,#1	 			;R3开始计数
								;ADD：R3计数加1

	CMP    R3,#1				;CMP：比较指令(R3-1) 不保存结果，只是影响相应的标志位
								;Z=1表示运算结果为零；Z=0表示运算结果不为零
	BEQ    STATE1				;当R3=1 跳转到状态1
								;BEQ: 数据跳转指令，标志寄存器中Z标志位等于零时, 跳转到BEQ后标签处
	CMP    R3,#2				;当R3=2 跳转到状态2
	BEQ    STATE2
	CMP    R3,#3				;当R3=3 跳转到状态3
	BEQ    STATE3
	CMP    R3,#4				;当R3=4 跳转到状态4
	BEQ    STATE4
	B      GOON    







;------------- 子函数 ------------------


;----------- 3中状态函数 ----------------

;************************** TEST **************************************
STATE1	   						;状态1 /测试部分 用R4,R5/
	MOV	   R4,#0xFF
	;LDR    R5,[R4,#4]!			;!：先更改[]内容 将R4的值+4(R4变了) 然后寻址R4+4这个地址 然后把这个地址的数放入R5
	;MOV    R5,R4,LSL #2		;LSL：先把R4逻辑左移2bit 然后放入R5 期间R4不变
	;MOV    R5,R4,LSR #1		;LSL：先把R4逻辑右移1bit 然后放入R5 期间R4不变
	;MOV    R5,R4,ROR #2		;ROR：R4循环右移2位0b1111 - 0b1111  然后放入R5 期间R4不变 而LSL是ob1111 - ob0011 把R4=0xFFFFFFFF可见效果
	;MRS    R5,PSR				;MRS：读CPSR 寄存器的值到R5 这里CPSR为PSR       MSR为写	
	;MOV    R5,#2
	;ADC	R6,R5,R4			;ADC：带进位的加法 当CPSR C位=1 就会进位 因为前面cmp的时候C置1了
	;SBC    R6,R5,R4			;SBC：带进位的减法 R5-R4 与ADC不同 他是C位=0 会减多一位
	;BIC    R5,R4,#0xF			;BIC：清零指令 把R4的第0,1,2,3位清0 放入R5 期间R4不变
	
	B      GOON
;**********************************************************************

STATE2	   						;状态2
	EOR    R2,#LED1				;EOR：R2与#LED1异或后 放进R2
	B      GOON
STATE3	   						;状态3
	EOR    R2,#LED1
	B      GOON
STATE4
	MOV    R3,#0				;R3 = 0 重新来一次
;----------- 继续循环函数 ----------------
GOON	   
	B      LOOP	   ;继续循环

 
; ----------- 延迟毫秒级函数 -------------
;延时R0（ms），误差((R0-1)*4+12)/8us ，延时较长时，误差小于0.1%
DELAY_NMS
	PUSH   {R1}	  		;2个周期
						;PUSH：将R1的数据入栈
						
DELAY_NMSLOOP			; 延迟1毫秒循环
	SUB    R0,#1
	MOV    R1,#1

DELAY_ONEUS				; 延迟1微秒 
    SUB    R1,#1
	NOP
	NOP
	NOP
	CMP    R1,#0
	BNE    DELAY_ONEUS		;若R1 != 0 跳转到本函数头 继续循环到1000次
	CMP    R0,#0			
	BNE    DELAY_NMSLOOP	;若R0 != 0 跳转到DELAY_NMSLOOP 继续新的1000次
	POP    {R1}				;POP：将栈顶数据弹出到R1
	BX     LR				;BX reg：跳转到为寄存器reg值的地址哪里 并且把现在的地址保存在LR中 （其他处理器有切换状态的效果）
							;与B的区别 -- B label 是单纯的跳转到label处
							;与BL的区别 -- 与BL label一样 但是BL reg就只有 单纯跳转到寄存器reg值的地址
							; https://www.cnblogs.com/zhugeanran/p/8493058.html
	NOP	
	END
