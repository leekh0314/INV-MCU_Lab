/*****************************************************************************
@Description: sys_hardware_init.c
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "sys_hardware_init.h"
#include "clk.h"
#include "gpio.h"
/***************************************************************************/

/*************************************************
Description: SystemClock_Init
Input      : 
Return     : 
Others     : EPWM配置 64M
*************************************************/
void SystemClock_Init(void)
{
	#if 1
    //0x40:选择X1振荡模式
	CLK_Osc_Setting(OSC_OSCILLATOR, OSC_OSCILLATOR);
    //0x40:选择X1振荡模式； 0x00:外部晶振范围0 - 10MHZ；  振荡稳定时间选择默认值， 振荡稳定时间计数值判断
	CLK_MainOsc_Setting(OSC_OSCILLATOR, OSC_UNDER_10M);

    //-----------------------------------------------------------------------
    // Select PLL as system clock
    //-----------------------------------------------------------------------
    //0x80:选择FMAX外部晶振作为PLL的时钟源；0X20 PLL倍频选择16倍  ；0X04  PLL分频选择2分频
    CLK_PLL_Setting(PLL_SR_fMX,PLL_DIV_2,PLL_MUL_16);
    CLK_PLL_Start(); /* PLLON = 1 */
	CLK_Fclk_Select(MAINCLK_FPLL);//高速内部振荡器时钟和PLL时钟的选择:1表示配置为PLL时钟
	while((CGC->MCKC & CGC_MCKC_CKSTR_Msk) == 0)//CKSTR高速内部振荡器时钟和PLL时钟的选择的状态:为1时表示当前时钟状态选择的是PLL时钟
       __NOP();
    SystemCoreClock = 64000000; // 8M*16/2 =  64MHz
	#else
		//内部晶振振荡模式   需把字节选项更改为64M模式
			
		////	CGC->CSC  = 0x00; //高速内部振荡器运行	
		////	while(CGC->OSTC < 0xf0); // 振荡稳定时间计数值判断
		////	CGC->CKC = 0x00;		//选择高速内部振荡器时钟（f IH ）作为主系统时钟（f MAIN ）
		////	CGC->PLLCR = 0x00;  //选择高速内部振荡器时钟F IH 作为PLL的输入时钟源
		////	CGC->MCKC = 0x01; /* CKSELR = 1, PDIV = 0 */  //高速内部振荡器时钟和PLL时钟的选择:1表示配置为PLL时钟
		////	while((CGC->MCKC & CGC_MCKC_CKSTR_Msk) == 0);//高速内部振荡器时钟和PLL时钟的选择的状态:为1时表示当前时钟状态选择的是PLL时钟	

	#endif

}

/*************************************************
Description: WDT_ConfigInit
Input      : 
Return     : 
Others     : 看门狗配置
*************************************************/
void WDT_ConfigInit(void)
{	
	
}

/*************************************************
Description: SysTick_ConfigInit
Input      : 
Return     : 
Others     : 系统定时器配置
*************************************************/
void SysTick_ConfigInit(void)
{
    uint32_t msCnt; 	// count value of 1ms
    //SystemCoreClockUpdate();	//更新时钟状态，获取系统时钟
    msCnt = SystemCoreClock / 1000; 
    SysTick_Config(msCnt);//滴答定时器初始化				
}

/*************************************************
Description: GPIO_Init
Input      : 
Return     : 
Others     : GPIO初始化配置
*************************************************/
void GPIO_Init(void)
{

    /*  SWCLK 配置成输入:P137*/ 
    PORT_Init(PORT13,PIN7,INPUT);// P137  配置为输入模式
    DBG -> DBGSTOPCR &= (~DBG_DBGSTOPCR_SWDIS_Msk);//配置为SW 模式
               
	    /* LED blinky on target board  LED绿灯控制配置 ，“1“表示点亮*/ 
    PORT_Init(PORT6,PIN0,OPENDRAIN_OUTPUT); // P60/led  配置为输出模式
    PORT_ClrBit(PORT6,PIN0);

    /* LED blinky on target board  LED红灯控制配置 ，“1“表示点亮*/ 
    PORT_Init(PORT6,PIN1,OPENDRAIN_OUTPUT); // P61/led  配置为输出模式
    PORT_ClrBit(PORT6,PIN1);	
    
	
    /* 逆变输出RY控制配置，“1“为ON*/ 
    PORT_Init(PORT7,PIN0,OUTPUT); // P70/逆变输出RY控制信号,配置为输出模式
    PORT_SetBit(PORT7,PIN0);

    /* 整流器输出EN_REC控制配置，“1“为ON*/ 
    PORT_Init(PORT3,PIN0,OUTPUT); // P30/整流器输出EN_REC控制信号,配置为输出模式
    PORT_ClrBit(PORT3,PIN0);


    /* 逆变器保护切除P_CUT，默认上拉输入(低电平有效)  */	
    PORT_Init(PORT3,PIN1,PULLUP_INPUT);   // P31设置为上拉输入端口
    /* 逆变器节能模式选择ECO，默认上拉输入(低电平有效)  */	
    PORT_Init(PORT7,PIN4,PULLUP_INPUT);   // P74设置为上拉输入端口
    
    
    // 输入电流过流信号配置，“0“过流,INTP0(GPIO_P136)触发 PWM 关断
    PORT_Init(PORT13,PIN6,PULLUP_INPUT);   // P136设置为上拉输入端口

    //STEP步进电机控制引脚初始化配置
    PORT_Init(PORT2,PIN4,OUTPUT); // P24/led  配置为输出模式
    PORT_ClrBit(PORT2,PIN4);
    PORT_Init(PORT2,PIN5,OUTPUT); // P25/led  配置为输出模式
    PORT_ClrBit(PORT2,PIN5);
    PORT_Init(PORT2,PIN6,OUTPUT); // P26/led  配置为输出模式
    PORT_ClrBit(PORT2,PIN6);
    PORT_Init(PORT2,PIN7,OUTPUT); // P27/led  配置为输出模式
    PORT_ClrBit(PORT2,PIN7);
}



/*************************************************
Description: EPWM_ConfigInit
Input      : 
Return     : 
Others     : EPWM配置 - M定时器
*************************************************/
void EPWM_ConfigInit(void)
{
	/* 
	(1)设置EPWM时钟
	*/
    CGC->PER1 |= CGC_PER1_TMMEN_Msk;     /* enables input clock supply */
    CGC->PER1 |= 0x04U;

	/*
	(2)设置EPWM工作模式
	*/
    TMM->TMSTR |= _04_TMM_TM0_COUNT_CONTINUES | _08_TMM_TM1_COUNT_CONTINUES;//在和TMGRA1、TMGRA0寄存器比较匹配后还继续计数。
    TMM->TMSTR &= (uint8_t)~(_02_TMM_TM1_COUNT_START | _01_TMM_TM0_COUNT_START);//TM0、TM1停止计数。
	
    /* the TMGRD0/TMGRC1/GMGRD1 is used as buffer register of TMGRB0/TMGRA1/TMGRB1 */
	//TMGRD0/TMGRC1/GMGRD1寄存器做为TMGRB0/TMGRA1/TMGRB1寄存器的缓冲寄存器  
    TMM->TMMR = _80_TMM_TMGRD1_BUFFER | _40_TMM_TMGRC1_BUFFER | _20_TMM_TMGRD0_BUFFER;
	
    /* the counter is free running */
		//TM0，TM1计数器的清除选择不使能，   clk为定时器计数源的选择
    #if 0
    TMM->TMCR0 =   _20_TMM_COUNTER_CLEAR_TMGRA | TMM_CLOCK_FCLK; //复位同步模式必须置“001”（在和TMGRA0寄存器比较匹配时清除TM0寄存器）  _00_TMM_COUNTER_CLEAR_DISABLE | clk;//互补  
    TMM->TMCR1 = _00_TMM_COUNTER_CLEAR_DISABLE | TMM_CLOCK_FCLK;
    #else
    //TM0，TM1计数器的清除选择不使能（自由运行），   clk为定时器计数源的选择
    TMM->TMCR0 = _00_TMM_COUNTER_CLEAR_DISABLE | _00_TMM_INETNAL_CLOCK_FCLK_FHOCO;
    TMM->TMCR1 = _00_TMM_COUNTER_CLEAR_DISABLE | _00_TMM_INETNAL_CLOCK_FCLK_FHOCO;  
    #endif   
    
	/*
	(3)设置EPWM反向输出、数据加载方式
	*/
	/* Set the initial level and active level: 
	   OLS1(反相-COUNTER), OLS0(正向-NORMAL): 0：初始输出“H”电平，“L”电平有效。1：初始输出“L”电平，“H”电平有效。
	   CMD1, CMD0 :  0b10:互补PWM模式（在TM1发生下溢时，将数据从缓冲寄存器传送到通用寄存器）*/  
   
	TMM->TMFCR = _02_TMM_TRANSFER_TM1_UNDERFLOW ; //互补PWM模式（在TM1发生下溢时，将数据从缓冲寄存器传送到通用寄存器，即更新占空比） 且为周期点更新占空比
    //TMM->TMFCR = _01_TMM_TRANSFER_RESET_SYNCHRONOUS ;//复位同步PWM模式   锯齿波

	#if 0	
		TMM->TMFCR |= _00_TMM_NORMAL_PHASE_LEVEl_HL  | //OLS0(正向-NORMAL): 0：初始输出“H”电平，“L”电平有效。1：初始输出“L”电平，“H”电平有效。
					  _00_TMM_COUNTER_PHASE_LEVEl_HL;  //OLS1(反相-COUNTER): 0：初始输出“H”电平，“L”电平有效。1：初始输出“L”电平，“H”电平有效。
	#else	
		TMM->TMFCR |= _04_TMM_NORMAL_PHASE_LEVEl_LH  | //OLS0(正向-NORMAL): 0：初始输出“H”电平，“L”电平有效。1：初始输出“L”电平，“H”电平有效。
					  _08_TMM_COUNTER_PHASE_LEVEl_LH;  //OLS1(反相-COUNTER): 0：初始输出“H”电平，“L”电平有效。1：初始输出“L”电平，“H”电平有效。	
	#endif
    
	/*
	(4)设置EPWM周期、EPWM死区
	*/
    TMM->TM0    = DEADTIME;//死区时间设置
    TMM->TM1    = 0x0000;
    TMM->TMGRA0 = PWM_PERIOD;   //计数到对称中心的顶点值
    TMM->TMGRC1 = PWM_PERIOD_HALF;//PWM_PERIOD_HALF;//PWM2的输出占空比设置   TMGRC1寄存器为TMGRA1寄存器的缓冲寄存器  
    TMM->TMGRA1 = 0;

    TMM->TMGRD0 = PWM_PERIOD_HALF;//PWM_PERIOD_HALF;//PWM1的输出占空比设置   TMGRD0寄存器为TMGRB0寄存器的缓冲寄存器  
    TMM->TMGRB0 = 0;

    TMM->TMGRD1 = PWM_PERIOD_HALF;//PWM3的输出占空比设置   TMGRD1寄存器为TMGRB1寄存器的缓冲寄存器  
    TMM->TMGRB1 = 0;		


	/*
	(5)设置EPWM输出使能
	*/
    //初始化时输出全部关断
    TMM->TMOER1 = _01_TMM_TMIOA0_OUTPUT_DISABLE | _02_TMM_TMIOB0_OUTPUT_DISABLE | _04_TMM_TMIOC0_OUTPUT_DISABLE | _08_TMM_TMIOD0_OUTPUT_DISABLE |
                  _10_TMM_TMIOA1_OUTPUT_DISABLE | _20_TMM_TMIOB1_OUTPUT_DISABLE | _40_TMM_TMIOC1_OUTPUT_DISABLE | _80_TMM_TMIOD1_OUTPUT_DISABLE;


	/*
	(6)设置比较匹配中断  使能TMM0、TMM1中断
	*/
    /* Clear the interrupt flag register   清除中断标志*/
    TMM->TMSR0 =  0;
    TMM->TMSR1 =  0;		
    /* Set the interrupt enable register */
    TMM->TMIER0 = _10_TMM_OVIE_ENABLE | _00_TMM_IMID_DISABLE | _00_TMM_IMIC_DISABLE | _00_TMM_IMIB_DISABLE | _00_TMM_IMIA_DISABLE;
    TMM->TMIER1 = _10_TMM_OVIE_ENABLE | _00_TMM_IMID_DISABLE | _00_TMM_IMIC_DISABLE | _00_TMM_IMIB_DISABLE | _00_TMM_IMIA_DISABLE;
    			
    //使能TMM0、TMM1中断 :中断优先级设置  0最高   3最低
    //INTC_EnableIRQ(TMM0_IRQn);/* enable INTTMM0 interrupt */
    INTC_EnableIRQ(TMM1_IRQn);/* enable INTTMM0 interrupt */

    //NVIC_SetPriority(TMM0_IRQn, 1); /* Set higher priority to execute slave ISR firstly */
    NVIC_SetPriority(TMM1_IRQn, 0); /* 更新占空比   优先级高  Set higher priority to execute slave ISR firstly */
	         
            
	/*
	(7)设置EPWM刹车
	*/
    //强制截止输出配置  输入源为INTP0 输入,即使用 INTP0(GPIO_P136)触发 PWM 关断。
    TMM->OPCTL0 =   _00_TMM_PWMOP_HAZARD_DIASBLE | 
                    _20_TMM_PWMOP_RISING_RELEASE |  //上升沿：输出强制截止解除;下降沿：输出强制截止
                    _10_TMM_PWMOP_INTP_CUTOFF |    //截止源的选择：INTP0 输入 作为输入源
                    _04_TMM_PWMOP_SOFT_CONDITION_RELEASE | //定时器M互补PWM模式：根据 OPEDGE 选择的 TMIOC0 的沿解除强制截止，脉冲输出恢复
                    _00_TMM_PWMOP_HARDWARE_RELEAS ;//选择硬件解除模式：定时器 M 互补 PWM 模式：监测到解除源之后，根据 OPEDGE 选择的 TMIOC0 的沿解除强制截止。            
                //	_00_TMM_PWMOP_SOFT_RELEASE_REMAIN; 
    TMM->OPDF0  =  _80_TMM_PWMOP_TMIODm_LOW | _08_TMM_PWMOP_TMIOBm_LOW ; //强制输出为低电平   B0、D0引脚禁止强制输出为低电平
    TMM->OPDF1  =  _80_TMM_PWMOP_TMIODm_LOW | _20_TMM_PWMOP_TMIOCm_LOW | _08_TMM_PWMOP_TMIOBm_LOW | _02_TMM_PWMOP_TMIOAm_LOW ; 		//A1、C1  B1、D1强制输出为低电平		                  
    TMM->OPEDGE =  _01_TMM_PWMOP_RELEASE_TMIOC0_F;//解除边沿选择：在 TMIOC0 的下降沿解除，即在周期点解除                     


	/*
	(8)设置IO口输出
	*/	
    /* Set TMIOC0 pin */
    TMOC0_PORT_SETTING();

    /* Set PWM1 TMIOB0 PWM2H P14 pin */
    TMOB0_PORT_SETTING(); 
    /* Set PWM1 TMIOD0 PWM2L P15 pin */
    TMOD0_PORT_SETTING();
		
    /* Set PWM2 TMIOA1 PWM1H P12 pin */
    TMOA1_PORT_SETTING();
    /* Set PWM2 TMIOC1 PWM1L P13 pin */
    TMOC1_PORT_SETTING();
		
    /* Set TMIOB1/W+ pin */
//    TMOB1_PORT_SETTING();
    /* Set TMIOD1/W- pin */
//    TMOD1_PORT_SETTING();
                      
	/*
	(9)重映射,客户根据实际应用配置 
	*/	

	
	/*
	(10)在zero点、周期点加载占空比
	*/	

	
	/*
	(11)开启EPWM
	*/	
    TMM->TMSTR |= _02_TMM_TM1_COUNT_START | _01_TMM_TM0_COUNT_START;//启动定时器	

}


/*************************************************
Description: User_PWM_Enable
Input      : 
Return     : 
Others     : 开启PWM输出
*************************************************/
void User_PWM_Enable(void)
{
    TMM->TMOER1 = _01_TMM_TMIOA0_OUTPUT_DISABLE | _00_TMM_TMIOB0_OUTPUT_ENABLE | _00_TMM_TMIOC0_OUTPUT_ENABLE | _00_TMM_TMIOD0_OUTPUT_ENABLE |
                  _00_TMM_TMIOA1_OUTPUT_ENABLE | _20_TMM_TMIOB1_OUTPUT_DISABLE | _00_TMM_TMIOC1_OUTPUT_ENABLE | _80_TMM_TMIOD1_OUTPUT_DISABLE;
}

/*************************************************
Description: User_PWM_Disable
Input      : 
Return     : 
Others     : 关闭PWM输出
*************************************************/
void User_PWM_Disable(void)
{
    TMM->TMOER1 =   _01_TMM_TMIOA0_OUTPUT_DISABLE | _02_TMM_TMIOB0_OUTPUT_DISABLE | _00_TMM_TMIOC0_OUTPUT_ENABLE | _08_TMM_TMIOD0_OUTPUT_DISABLE |
                    _10_TMM_TMIOA1_OUTPUT_DISABLE | _20_TMM_TMIOB1_OUTPUT_DISABLE | _40_TMM_TMIOC1_OUTPUT_DISABLE | _80_TMM_TMIOD1_OUTPUT_DISABLE;

    PORT->P1 &= ~(15<<2) ; //PWM口拉低	
}


/*************************************************
Description: ADC_ConfigInit
Input      : 
Return     : 
Others     : 高速ADC配置
*************************************************/
void ADC_ConfigInit(void)
{
	/*
	(1)设置ADC时钟
	*/
    CGC->PER0 |= CGC_PER0_ADCEN_Msk;    /* enables input clock supply */
    ADC->ADM0  = 0x00U;                 /* disable AD conversion and clear ADM0 register */
	
	/*
	(2)设置ADC通道使能
	*/	
    #if 1	
        PORT->PMC2 |= (1 << 0);  // /* Set ANI0(P20) pin: 散热器温度 */ 
        PORT->PMC2 |= (1 << 3);  // /* Set ANI3(P23) pin：交流电压*/ 
        PORT->PMC1 |= (1 << 0); //  /* Set ANI9(P10) pin： 基准电压 */ 
//        PORT->PMC0 |= (1 << 3); //  /* Set ANI10(P03) pin：母线电压 */ 
//        PORT->PMC0 |= (1 << 2); //  /* Set ANI11(P02) pin：电源工作电压 */ 
        PORT->PMC0 |= (1 << 1); //  /* Set ANI10(P01) pin：母线电压 20250829*/ 
        PORT->PMC0 |= (1 << 0); //  /* Set ANI11(P00) pin：电源工作电压 20250829 */ 
        PORT->PMC14|= (1 << 6);  // /* Set ANI15(P146) pin 电感电流*/ 
    #else 
            ADC_PORT_SETTING();//配置ANI00 - ANI15通道
    #endif
	/*
	(3)设置ADC工作模式
	*/	
    /* AD operation mode: select or scan mode  */
    //ADC->ADM0 = _10_AD_CONVERSION_CLOCK_8 | _00_AD_COMPARATOR_DISABLE;
    ADC->ADM0 = _28_AD_CONVERSION_CLOCK_1 | _00_AD_COMPARATOR_DISABLE;    //Fclk /1  转换时钟频率 ;转换时间为45/Fclk

    /* AD conversion mode setting */
    ADC->ADM1 = _00_AD_HISPEED;  //高速变换模式
		
    //采样时钟数设置为13.5 * Fclk
//    ADC->ADNSMP = _0F_AD_CHARGE_DISCHARGE_15CLK;   //采样时钟数设置为13.5 * Fclk
    ADC->ADNSMP = _0D_AD_CHARGE_DISCHARGE_13CLK;   //采样时钟数设置为13.5 * Fclk 20250830 

    //AD转换通道模式设置		
    #if 1     
        ADC->ADM1 |= _00_AD_OPERMODE_SELECT;  //AD转换通道设置为选择模式
    #else     
        ADC->ADM1 |= _80_AD_OPERMODE_SCAN; //AD转换通道设置为扫描模式
    #endif
    
    // AD转换模式设置
    #if 0
        ADC->ADM1 |= _00_AD_CONVMODE_SEQUENTIAL; // AD转换模式设置为单次转换模式
    #else 
        ADC->ADM1 |= _08_AD_CONVMODE_ONESHOT;  // AD转换模式设置为连续转换模式
    #endif

    // AD参考电压设置
    /* AD reference voltage setting */
    ADC->ADM2 = _00_AD_POSITIVE_VDD | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 ;

//    ADC->ADM2 = _40_AD_POSITIVE_AVREFP | _20_AD_NEGATIVE_AVREFM | _00_AD_AREA_MODE_1 ;

//    ADC->ADM2 = _40_AD_POSITIVE_AVREFP | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 ;


    // AD参考电压设置
    /* AD trigger selection */
    ADC->ADTRG = _00_AD_TRIGGER_SOFTWARE;   //软件触发模式

//    ADC->ADTRG = _80_AD_TRIGGER_HARDWARE_NOWAIT;//硬件无等待触发模式

//    ADC->ADTRG = _C0_AD_TRIGGER_HARDWARE_WAIT;//硬件等待触发模式


    // AD上下限值限制	
    /* AD comversion result comprision upper limit setting */
    ADC->ADUL = _FF_AD_ADUL_VALUE;   //比较AD采集的12位数据的高八位
    /* AD comversion result comprision lower limit setting */
    ADC->ADLL = _00_AD_ADLL_VALUE;


	/*
	(4)设置ADC中断
	*/	
	  //清除中断标志
    INTC_ClearPendingIRQ(ADC_IRQn);     /* clear INTAD interrupt flag */
    NVIC_ClearPendingIRQ(ADC_IRQn);     /* clear INTAD interrupt flag */ 
    #if 0
        INTC_EnableIRQ(ADC_IRQn);       /* enable INTAD interrupt */
    #else
        INTC_DisableIRQ(ADC_IRQn);      /* disable INTAD interrupt */     
    #endif
    
	/*
	(5)设置优先级
	*/	
//    NVIC_SetPriority(ADC_IRQn, 2); /* Set higher priority to execute slave ISR firstly */

	/*
	(6)开启ADC
	*/		

    ADC->ADM0 |= ADCE;          // 使能A/D电压比较器的运行。
    ADC->ADS = ADC_CHANNEL_3;   // 指定模拟输入通道号
    ADC->ADM0 |= ADCS;          // 允许转换运行		
}

/*************************************************
Description: TMB_InputCapture_Init
Input      : 
Return     : 
Others     : 用于发电机转速信号捕获
*************************************************/
void TMB_InputCapture_Init(void)
{
	/*
	(1)设置TMB_InputCapture的时钟
	*/
    CGC->PER1 |= CGC_PER1_TMBEN_Msk;     /* enables input clock supply */
    TMB->TBMR &= (uint8_t)~_80_TMB_COUNT_START;

    TMB->TBCR = _00_TMB_CLEAR_DISABLEED  | _00_TMB_COUNT_RISING | _03_TMB_fCLK_8_SELECTED;  //时钟8分频，上升沿计数  64M/8 = 8M
    TMB->TBMR = _00_TMB_SIGNALB_SELECTED | //选择外部输入信号1/数字滤波器通过信号1。
								_00_TMB_INCREMENT_MODE   |  //递增计数
								_00_TMB_TBIOA_FILTER_USED| //	TBIO0引脚的数字滤波器功能的选择
								_10_TMB_fCLK_8_SELECTED  | //数字滤波器功能所用时钟的选择
								_00_TMB_TIMER_MODE;        //定时器模式  


	/*
	(2)设置TMB_InputCapture捕获配置模式
	*/
//    TMB->TBIOR |= _80_TMB_TBGRB_REGISTER_USED | _40_TMB_TBGRB_INPUT_CAPTURE ;  //TBGRB配置为输入捕捉功能  ，使用缓冲寄存器，TBIO1的上升沿
    TMB->TBIOR = _08_TMB_TBGRA_REGISTER_USED | _04_TMB_TBGRA_INPUT_CAPTURE; //TBGRA配置为输入捕捉功能  ，使用缓冲寄存器，TBIO0的上升沿
	
	/*
	(4)设置TMB_InputCapture捕获中断
	*/	
		INTC_DisableIRQ(TMB_IRQn);/* disable INTTMB interrupt */
    INTC_ClearPendingIRQ(TMB_IRQn);/* clear INTTMB interrupt flag */
		TMB->TBIER = _08_TMB_INTERRUPT_TBOVF_ENABLE  | _01_TMB_INTERRUPT_TBIMFA_ENABLE;//配置中断允许寄存器	
	/*
	(5)设置TMB_InputCapture中断优先级
	*/	
		NVIC_SetPriority(TMB_IRQn,3);

	/*
	(6)配置输入源IO端口模式
	*/	
		/* Set TBIO0 pin */
		PORT->PM5 |= 0x01U; // P50   输入模式
		
//		/* Set TBIO1 pin */
//		PORT->PM5 |= 0x02U; // P51   输入模式
		
	/*
	(7)开启TMB_InputCapture模块
	*/	
		TMB0_Start();	 	 		 
}

/*************************************************
Description: ACMP0_ConfigInit
Input      : 
Return     : 
Others     : ACMP0配置：用于输出电压频率检测
*************************************************/
void ACMP0_ConfigInit(void)
{			
	
	/*
	(1)设置比较器的输入时钟源
	 */
    CGC->PER1 |= CGC_PER1_PGACMPEN_Msk;    /* enables input clock supply */
 
    
	/*
	 (2)设置ACMP0 基准电压值 
	*/	
    CMP_VREF_Set_Value(CMP_CHANNEL_0, 128);//设置内置基准电压值   VDD（5.0V） / 256 * 128 = 2.5V 
    CMP_VREF_Init     (CMP_CHANNEL_0, CMP_VDD_VSS_REFERENCE_VOLTAGE);//比较器内置基准电压控制寄存器配置(CVRCTL)

	
	/*
	(3)设置ACMP0 正相端和反相端工作模式
	*/	
    //比较器0的输入信号选择控制寄存器（CMPSEL0）:
    CMP_Input_Select  (CMP_CHANNEL_0, CMP_PSIDE_VCIN0,CMP_NSIDE_IVREF);//比较器0：正端选择外部引脚输入(VCIN0引脚)，负端选择选择内置基准电压VREF0

    
	/*
	(4)开启滤波输出
	*/
    //比较器滤波控制寄存器（COMPFIR）
    CMP_Filter_Setting(CMP_CHANNEL_0, CMP_FILTER_FCLK, CMP_EDGE_RISING);//比较器0：滤波频率选择Fclk/8，双边沿检测产生中断请求


	/*
	(5)比较器中断
	*/
    #if 0
        CMP->COMPOCR =  _01_COMP0_INTERRPUT_ENABLE;		 //允许比较器0的中断请求	
        INTC_EnableIRQ(CMP0_IRQn);          /* enable INTCMP0 interrupt */    
        NVIC_SetPriority(CMP0_IRQn, 3); /* Set higher priority to execute slave ISR firstly */
    #else
        INTC_ClearPendingIRQ(CMP0_IRQn);    /* clear INTCMP0 interrupt flag */       
    #endif


	/*
	(6)设置迟滞
	*/	 	 
//	*( uint8_t*)(0x4004384E) = 0x13;//迟滞
	
    
	/*
	(7)配置ACMP IO口
	*/
    /* Set VCIN0(P22) pin   比较器正向端输入配置*/				
    PORT->PMC2  |= (1 << 2);  
          

	/*
	(8)配置ACMP 反相输出
	*/
    CMP->COMPOCR &= 0xfb;//将比较器0反相使能位清零
    #if 1
        CMP->COMPOCR |=  _00_COMP0_OUTPUT_NORMAL_TO_VCOUT0;		    //VCOUT0进行比较器0的输出
    #else
        CMP->COMPOCR |=  _04_COMP0_OUTPUT_INVERTED_TO_VCOUT0;		 //VCOUT0进行比较器0的反相输出   
    #endif    
	/*
	(9)开启ACMP0
	*/
    CMP->COMPMDR = _01_COMP0_OPERATION_ENABLED;//使能比较器0的运行

}


/*************************************************
Description: UART0_ConfigInit
Input      : 
Return     : 
Others     : UART0配置：
*************************************************/
void UART0_ConfigInit(void)
{
	
	/*
	(1)开启UARTx时钟
	*/
    CGC->PER0 |= CGC_PER0_SCI0EN_Msk;
    SCI0->SPS0 = (0 << SCI0_SPS0_PRS00_Pos) | (3 << SCI0_SPS0_PRS00_Pos);

    
	/*
	(2)设置UARTx模式
	*/        
    /* transmission channel */
    SCI0->SMR00 = _0020_SMRMN_DEFAULT_VALUE | _0000_SCI_CLOCK_SELECT_CK00 | _0000_SCI_CLOCK_MODE_CKS |
                  _0002_SCI_MODE_UART | _0000_SCI_TRANSFER_END;
    SCI0->SCR00 = _0004_SCRMN_DEFAULT_VALUE | _8000_SCI_TRANSMISSION | _0000_SCI_TIMING_1 | _0000_SCI_INTSRE_MASK |
                  _0000_SCI_PARITY_NONE | _0080_SCI_LSB | _0010_SCI_STOP_1 | _0003_SCI_LENGTH_8;
    SCI0->SDR00 = _CE00_SCI_BAUDRATE_DIVISOR;
    /* reception channel */
    MISC->NFEN0 |= _01_SCI_RXD0_FILTER_ON;
    SCI0->SIR01 = _0004_SCI_SIRMN_FECTMN | _0002_SCI_SIRMN_PECTMN | _0001_SCI_SIRMN_OVCTMN;
    SCI0->SMR01 = _0020_SMRMN_DEFAULT_VALUE | _0000_SCI_CLOCK_SELECT_CK00 | _0000_SCI_CLOCK_MODE_CKS |
                  _0100_SCI_TRIGGER_RXD | _0000_SCI_EDGE_FALL | _0002_SCI_MODE_UART | _0000_SCI_TRANSFER_END;
    SCI0->SCR01 = _0004_SCRMN_DEFAULT_VALUE | _4000_SCI_RECEPTION | _0000_SCI_TIMING_1 | _0000_SCI_INTSRE_MASK |
                  _0000_SCI_PARITY_NONE | _0080_SCI_LSB | _0010_SCI_STOP_1 | _0003_SCI_LENGTH_8;
    SCI0->SDR01 = _CE00_SCI_BAUDRATE_DIVISOR;
    

	/*
	(3)UARTx引脚配置
	*/	 
    /* Set TxD0 pin */
    PORT->PIOR0 &= ~(1 << 1);    /* allocate TXD0 to P51 */ 
    PORT->P5    |=  (1 << 1);    /* P51 output high level */ 
    PORT->PM5   &= ~(1 << 1);    /* P51 is used as TXD0 output */ 
    PORT->POM5  &= ~(1 << 1);    /* P51 is push-pull output mode */ 
    /* Set RxD0 pin */
    PORT->PIOR0 &= ~(1 << 1);    /* allocate RXD0 to P50 */ 
    PORT->PM5   |=  (1 << 0);    /* P50 is used as RXD0 input */ 


	/*
	(4)UARTx波特率配置
	*/
    /* UART0 Start, Setting baud rate */
    //波特率计算：64M/(2^5)/((0+1)*2) = 2M/2 = 1M 波特率    
    SCI0->ST0 = _0002_SCI_CH1_STOP_TRG_ON | _0001_SCI_CH0_STOP_TRG_ON;
    SCI0->SPS0 &= ~SCI0_SPS0_PRS00_Msk;
    SCI0->SPS0 |=  _0000_SCI_CK01_fCLK_0 | 5;   //注意 UART0 和 UART1 共用 SCI0->SPS0 寄存器
    SCI0->SDR00 = 0 << 9;
    SCI0->SDR01 = 0 << 9;


	/*
	(5)开启UARTx输出
	*/	    
    /* output enable */
    SCI0->SO0 |= _0001_SCI_CH0_DATA_OUTPUT_1;
    SCI0->SOL0 &= (uint16_t)~_0001_SCI_CHANNEL0_INVERTED;
    SCI0->SOE0 |= _0001_SCI_CH0_OUTPUT_ENABLE;
    SCI0->SS0 |= _0002_SCI_CH1_START_TRG_ON | _0001_SCI_CH0_START_TRG_ON;
 
	/*
	(6)开启中断
	*/    
    INTC_ClearPendingIRQ(ST0_IRQn); /* clear INTST0 interrupt flag */
    INTC_ClearPendingIRQ(SR0_IRQn); /* clear INTSR0 interrupt flag */
    NVIC_ClearPendingIRQ(ST0_IRQn); /* clear INTST0 interrupt flag */
    NVIC_ClearPendingIRQ(SR0_IRQn); /* clear INTSR0 interrupt flag */
    #if 0
        INTC_EnableIRQ(ST0_IRQn);       /* enable INTST0 interrupt */
        INTC_EnableIRQ(SR0_IRQn);       /* enable INTSR0 interrupt */
    #else
        INTC_DisableIRQ(ST0_IRQn);      /* disable INTST0 interrupt */
        INTC_DisableIRQ(SR0_IRQn);      /* disable INTSR0 interrupt */
    #endif
}


/*************************************************
Description: UART1_ConfigInit
Input      : 
Return     : 
Others     : UART1配置：逆变侧数据发送 （波特率9600）
*************************************************/
void UART1_ConfigInit(void)
{
	/*
	(1)开启UARTx时钟
	*/	    
    CGC->PER0 |= CGC_PER0_SCI0EN_Msk;
    //（STmn）写“1”，就将串行通道允许状态寄存器m（SEm）的对应位（SEmn）清“0”（运行停止状态）。
    SCI0->ST0 |= _0008_SCI_CH3_STOP_TRG_ON | _0004_SCI_CH2_STOP_TRG_ON;


	/*
	(2)设置UARTx模式
	*/    
    /* transmission channel */
    SCI0->SMR02 = _0020_SMRMN_DEFAULT_VALUE | _0000_SCI_CLOCK_SELECT_CK00 | _0000_SCI_CLOCK_MODE_CKS |
                  _0002_SCI_MODE_UART | _0000_SCI_TRANSFER_END;
    SCI0->SCR02 = _0004_SCRMN_DEFAULT_VALUE | _8000_SCI_TRANSMISSION | _0000_SCI_TIMING_1 | _0000_SCI_INTSRE_MASK |
                  _0000_SCI_PARITY_NONE | _0080_SCI_LSB | _0010_SCI_STOP_1 | _0003_SCI_LENGTH_8;
    SCI0->SDR02 = _CE00_SCI_BAUDRATE_DIVISOR;
    
    /* reception channel */
    MISC->NFEN0 |= _04_SCI_RXD1_FILTER_ON;
    SCI0->SIR03 = _0004_SCI_SIRMN_FECTMN | _0002_SCI_SIRMN_PECTMN | _0001_SCI_SIRMN_OVCTMN;
    SCI0->SMR03 = _0020_SMRMN_DEFAULT_VALUE | _0000_SCI_CLOCK_SELECT_CK00 | _0000_SCI_CLOCK_MODE_CKS |
                  _0100_SCI_TRIGGER_RXD | _0000_SCI_EDGE_FALL | _0002_SCI_MODE_UART | _0000_SCI_TRANSFER_END;
    SCI0->SCR03 = _0004_SCRMN_DEFAULT_VALUE | _4000_SCI_RECEPTION | _0000_SCI_TIMING_1 | _0000_SCI_INTSRE_MASK |
                  _0000_SCI_PARITY_NONE | _0080_SCI_LSB | _0010_SCI_STOP_1 | _0003_SCI_LENGTH_8;									
    SCI0->SDR03 = _CE00_SCI_BAUDRATE_DIVISOR;
    
    
	/*
	(3)UARTx引脚配置
	*/	    
    /* Set TxD1 pin  P72 重定向成TXD1*/
    PORT->PIOR0 |=  (1 << 5);    /* P72 重定向成TXD1模式 */ \
    PORT->P7    |=  (1 << 2);    /* P72 output high level */ 
    PORT->PM7   &= ~(1 << 2);    /* P72 is used as TXD1 output */ 
    PORT->POM7  &= ~(1 << 2);    /* P72 is push-pull output mode */ 
    
    /* Set RxD1 pin   P73 重定向成RXD1*/
    //PORT->PIOR0 |=  (1 << 5);    /* P73 重定向成RXD1模式 */ \
    //PORT->PM7   |=  (1 << 3);    /* P73 is used as RXD1 input */ 

	/*
	(4)UARTx波特率配置
	*/	
    /* UART1 Start, Setting baud rate */
    #if 0
    //波特率计算：64M/(2^5)/((103+1)*2) = 2M/208 = 9615 ≈ 9600 波特率
    SCI0->SPS0 = _0000_SCI_CK01_fCLK_0 | 5;     //注意 UART0 和 UART1 共用 SCI0->SPS0 寄存器
    SCI0->SDR02 = 103 << 9;
    SCI0->SDR03 = 103 << 9;
    #else
    // 波特率计算：64M/(2^5)/((0+1)*2) = 2M/2 = 1M 波特率    
    SCI0->SPS0 |=  _0000_SCI_CK01_fCLK_0 | 5;   //注意 UART0 和 UART1 共用 SCI0->SPS0 寄存器
    SCI0->SDR02 = 0 << 9;
    SCI0->SDR03 = 0 << 9;

//		// 波特率计算： 64M/(2^2)/((13+1)*2) = 16M/28 = 571427.5 ≈ 57600
//		SCI0->SPS0 = _0000_SCI_CK01_fCLK_0 | 2; // 20250906
//		SCI0->SDR02 = (13 << 9);// 实际 ≈ 571428.6 bps
//		SCI0->SDR03 = (13 << 9);


    
    
    #endif
	/*
	(5)开启UARTx输出
	*/	    
    /* output enable */
    SCI0->SO0 |= _0004_SCI_CH2_DATA_OUTPUT_1;
    SCI0->SOL0 &= (uint16_t)~_0004_SCI_CHANNEL2_INVERTED;
    SCI0->SOE0 |= _0004_SCI_CH2_OUTPUT_ENABLE;//设定允许或者停止各通道的串行通信的输出。 发送   
    SCI0->SS0 |= _0008_SCI_CH3_START_TRG_ON | _0004_SCI_CH2_START_TRG_ON;//允许各通道的通信/开始计数的触发寄存器
    
	/*
	(6)开启中断
	*/    
    INTC_ClearPendingIRQ(ST1_IRQn); /* clear INTST1 interrupt flag */
    INTC_ClearPendingIRQ(SR1_IRQn); /* clear INTSR1 interrupt flag */
    NVIC_ClearPendingIRQ(ST1_IRQn); /* clear INTST1 interrupt flag */
    NVIC_ClearPendingIRQ(SR1_IRQn); /* clear INTSR1 interrupt flag */
    #if 0 
        INTC_EnableIRQ(ST1_IRQn);       /* enable INTST1 interrupt */
        INTC_EnableIRQ(SR1_IRQn);       /* enable INTSR1 interrupt */
    #else
        INTC_DisableIRQ(ST1_IRQn);      /* disable INTST1 interrupt */
        INTC_DisableIRQ(SR1_IRQn);      /* disable INTSR1 interrupt */
    #endif
}


/*************************************************
Description: Sys_HardConfigInit
Input      : 
Return     : 
Others     : 系统硬件配置初始化
*************************************************/
extern uint8_t u8_senddataarraydebug[9];
extern uint8_t txBuf[20] ;
void Sys_HardConfigInit(void)
{
	
    //系统时钟
	SystemClock_Init();
    
    //滴答定时器初始化
    SysTick_ConfigInit();
	
    // IO端口初始化配置
    GPIO_Init();

    // DMA初始化配置
    DMA_Start(DMA_VECTOR_ST1, 0, DMA_MODE_NORMAL, DMA_SIZE_BYTE, 19, (void *)&txBuf[1], (void *)&SCI0->TXD1);// vofa+ 20Byte
//	  DMA_Start(DMA_VECTOR_ST1, 0, DMA_MODE_NORMAL, DMA_SIZE_BYTE, 9, (void *)&txBuf[1], (void *)&SCI0->TXD1); // VisualScope 10Byte/57600波特率 

    // ADC初始化配置
    ADC_ConfigInit();

    //比较器0初始化    
    ACMP0_ConfigInit();

    // 发电机转速捕获
    TMB_InputCapture_Init();
	
    //UART0初始化 
    //UART0_ConfigInit();
    
    //UART1初始化 
    UART1_ConfigInit();

    //EPWM初始化    
    EPWM_ConfigInit();

}



