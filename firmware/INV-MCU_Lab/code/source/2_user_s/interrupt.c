/*****************************************************************************
@Description: interrupt.c 中断服务程序
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "user_function.h"
#include "sys_define_parar.h"


/****************************************************************************/

extern int16_t      s16_Capture_Vol_Fre;
extern int16_t		s16_GeneratorSpeedAvg;
extern int16_t	    s16_MasterslaverunCnt;
extern int16_t		s16_IuacquireVal;
extern int16_t		s16_GeneratorfreVal;
extern int16_t		s16_CapturefreVal;
extern int16_t		s16_OutcapcurValfil;
extern int16_t		s16_OutvoldcabsVal;
extern volatile uint8_t Cmp0_Interrupt_Flag_temp;

/* 中断暂存变量 */
uint8_t  tmsr1_temp;
uint8_t  tmier1_temp;
uint32_t temp_A = 0;

 
/*--------------发电机转速捕获-------------*/
int16_t		s16_GeneratorfreVal = 0;
int32_t		s32_GeneratorcapvalCal = 0;
int32_t		s32_GeneratorcapvalCal_OLd = 0;
int16_t     s16_GeneratorfreVal_Old;
int8_t      SpeedSampleCount = 10;
volatile int count_flag = 0;
int16_t     s16_GeneratorfreVal_Array[10] = {0};
extern uint16_t	u16_CapturetimeCnt;
extern int16_t TimeCntVal;//周期计数值
int32_t s32_GeneratorcapvalCaltemp = 0;
extern void  CloseDrive_Function(void);
extern const  short int Sine_Table_50Hz[];
extern const  short int Sine_Table_60Hz[];  
extern int32_t Kp_A,Ki_B;
extern int16_t P_table[384];
extern int16_t Q_table[384];
void IRQ28_Handler(void) __attribute__((alias("tmm1_interrupt")));
void IRQ29_Handler(void) __attribute__((alias("tmb0_interrupt")));




/*************************************************
Description: tmm0_interrupt
Input      : 
Return     : 
Others     : 
*************************************************/
//目前在代码中没有被引用，应该是早期功率-幅值线性补偿方案留下的宏
#define  Vol_AM_Target_k   178  
#define  Vol_AM_Target_b   2000
/*****************************************************************************************/
// Vol_AM_Target_k1、Vol_AM_Target_b1根据母线电压实时调整交流电压幅值目标；
// 本组常量定义了一条“母线电压 → 目标交流幅值”的线性补偿曲线，用于在 DC 母线电压下降时同步降低 AC 调制深度；
// 采用 16384 这一 2 的幂次形式，便于右移 12 位实现整数斜率（即定点计算效率更高）；
// 当右移 12 位时，相当于斜率为 4，表示母线电压每下降 1 个 ADC 计数，交流幅值按 4 个计数的比例降低。
/*****************************************************************************************/
#define  Vol_AM_Target_k1   16384  // 16384 = 4 << 12 
#define  Vol_AM_Target_b1   -3850

extern int16_t correct_DCIM_value;
int test_cnt,duty_test = 0;
int AM_COUNT = 0;
int InvVol_AM_Target_Temp,InvVol_AM_Target_TempLPF = 0;

INV_Var_t INV_Info; 


void tmm1_interrupt(void)
{
    /*------------------------------------------------------------------------------------*/
    /*---------------------PWM寄存器值暂存------------------------------------------------*/	
    tmsr1_temp = TMM->TMSR1;
    tmier1_temp = TMM->TMIER1;
    
    /*------------------------------------------------------------------------------------*/
    /*---------------------清除TMM0中断挂起-----------------------------------------------*/		 
    INTC_ClearPendingIRQ(TMM1_IRQn); /* clear INTTMM0 interrupt flag */	
    TMM->TMIER1 = 0x00U;                 
    
    INV_CloseDrive();//故障关驱动   
    INV_ACOUT_FreCalc();//输出频率检测  

    /*------------------------------------------------------------------------------------*/
    /*---------------------AD采样及数据处理-----------------------------------------------*/		   
    COM_VolSample();//输出电压采样获取   
    COM_CurSample();//逆变电流采样获取		
    COM_Altern_Sample();//辅助电源、温度、VREF采样值获取			
     
    User_EffectiveValue_Calc();//有效值计算   		
 

    /*------------------------------------------------------------------------------------*/				
    /*---------------------电压电流环PI参数设定-------------------------------------------*/	
    #if CM_Inverter_Loop_Ctrl == CM_Inverter_Single_Loop
        PID_Vol_Loop.Kp = 3700;
        PID_Vol_Loop.Ki = 2700;
    #else
//        PID_Vol_Loop.Kp = 9500;
//        PID_Vol_Loop.Ki = 9000; 
        PID_Vol_Loop.Kp = 7500;
        PID_Vol_Loop.Ki = 7000;
        PID_Curr_Loop.Kp = 1510;
        PID_Curr_Loop.Ki = 2510; 

    #endif  

    
#if OPERATING_MODE == Normal_Mode    

    /*------------------------------------------------------------------------------------*/		
    /*-------------------------根据功率值线性变化电压参考值-------------------------------*/  
    /*-------------------------负载越大电压越高-------------------------------------------*/          
        if(Inv_SineCtrl_Info.BusVol_AD > 1540 && (INV_Info.AC_Vol_AMP_Target >= INV_AC_VOL_AMP_VAL_NORMAL_REF))
        {                   
            //电压幅值补偿处理
            // InvVol_AM_Target_Temp =  (106 * System_Power_Info.Active_Power_Total>>12) + INV_AC_VOL_AMP_VAL_NORMAL_REF; //  斜率 106/4096 ≈ 0.0259 count/W，使得功率每增加 1 W 幅值参考就上升约 0.0259。           InvVol_AM_Target_Temp =  (INV_LOAD_PWR_SLOPE_NUM * System_Power_Info.Active_Power_Total >> 12) + INV_AC_VOL_AMP_VAL_NORMAL_REF;// 20251022 斜率修改
            InvVol_AM_Target_Temp = (INV_LOAD_PWR_SLOPE_NUM * System_Power_Info.Active_Power_Total >> 12) + INV_AC_VOL_AMP_VAL_NORMAL_REF; // INV_LOAD_PWR_SLOPE_NUM 功率补偿斜率的分子

            InvVol_AM_Target_TempLPF = (InvVol_AM_Target_Temp*596 + InvVol_AM_Target_TempLPF*(4096 - 596))>>12;

            INV_Info.AC_Vol_AMP_Target = InvVol_AM_Target_TempLPF;
            
            if(INV_Info.AC_Vol_AMP_Target < INV_AC_VOL_AMP_VAL_NORMAL_REF)
            {
               INV_Info.AC_Vol_AMP_Target = INV_AC_VOL_AMP_VAL_NORMAL_REF;
            }
            if(INV_Info.AC_Vol_AMP_Target > (INV_AC_VOL_AMP_VAL_NORMAL_REF + 100))
            {
               INV_Info.AC_Vol_AMP_Target = (INV_AC_VOL_AMP_VAL_NORMAL_REF + 100);
            }             
        }
        else
        {          
    /*------------------------------------------------------------------------------------*/		
    /*---------------------------根据母线电压线性变化电压参考值---------------------------*/               

//            //突加载油门动态匹配
            INV_Info.AC_Vol_AMP_Target = ( Vol_AM_Target_k1 * ADSample_Info.Bus_Vol_AD>>12 )+ Vol_AM_Target_b1  ;                     
            if(INV_Info.AC_Vol_AMP_Target > INV_AC_VOL_AMP_VAL_NORMAL_REF)
            {
                INV_Info.AC_Vol_AMP_Target = INV_AC_VOL_AMP_VAL_NORMAL_REF;
            }     
            if(INV_Info.AC_Vol_AMP_Target < 700)
            {
                INV_Info.AC_Vol_AMP_Target = 700;
            }   
        }                
#else
       INV_Info.AC_Vol_AMP_Target = INV_AC_VOL_AMP_VAL_DEBUG_REF; 

#endif  
  
    /*------------------------------------------------------------------------------------*/				
    /*---------------------控制算法执行及占空比更新---------------------------------------*/	
    if ( CM_RUN_STATE == State_Context.State_Identifier )
    {        
        Inv_SineCtrl_Info.BusVol_AD            = ADSample_Info.Bus_Vol_Filiter_AD;
        Inv_SineCtrl_Info.InvVol_Peak_AD       = ADSample_Info.AC_Vol_Peak;
        Inv_SineCtrl_Info.InvCur_Peak_AD       = ADSample_Info.Inv_Cur_Peak;
        Inv_SineCtrl_Info.InvLoad_Peak_AD      = ADSample_Info.Load_Cur_Peak;
        Inv_SineCtrl_Info.InvVol_rms           = ADSample_Info.AC_Vol_RMS;
        Inv_SineCtrl_Info.InvLoad_Irms         = ADSample_Info.Load_Cur_RMS;
                
        //直流分量校正
        INV_DCIM_Ctrl();
        //限功率
        INV_Power_Ctrl();
        
        if(ADSample_Info.Inv_Cur_RMS>259)
        {
            //限流环上下限补偿
            if(System_Power_Info.Active_Power_Total < 1500)
            {
               Inv_SineCtrl_Info.CurrLoop_Up_Limit          = 1600 +(System_Power_Info.Active_Power_Total*2>>3);
               Inv_SineCtrl_Info.CurrLoop_Dn_Limit          = -Inv_SineCtrl_Info.CurrLoop_Up_Limit;   
            }
            else if(Inv_SineCtrl_Info.CurrLoop_Up_Limit < CM_InvCurr_Up_Limit&&Inv_SineCtrl_Info.SineTab_Index==0)// 电流环的保护阈值
            {
               Inv_SineCtrl_Info.CurrLoop_Up_Limit          += 10;
               Inv_SineCtrl_Info.CurrLoop_Dn_Limit          = -Inv_SineCtrl_Info.CurrLoop_Up_Limit;
            }        
        }
        else
        {
           Inv_SineCtrl_Info.CurrLoop_Up_Limit          = 1600 ;
           Inv_SineCtrl_Info.CurrLoop_Dn_Limit          = -Inv_SineCtrl_Info.CurrLoop_Up_Limit;              
        }            
    
       Inv_SineCtrl_Info.CurrLoop_Up_Limit          = 6000 ;
       Inv_SineCtrl_Info.CurrLoop_Dn_Limit          = -Inv_SineCtrl_Info.CurrLoop_Up_Limit;          
           
        
        Inv_SineCtrl_Info.Calc( &Inv_SineCtrl_Info ); // Calculate PWM duty cycle  Inverter_AlgCtrl() // 逆变调制计算入口 逆变器调制与控制计算         
        TMM->TMGRA0 = Inv_SineCtrl_Info.InvVol_SPWM_Period;	//Set pwm period
        TMM->TMGRD0 = Inv_SineCtrl_Info.InvVol_SPWM_Compare_A;//PWM1的输出占空比设置   TMGRC1寄存器为TMGRA1寄存器的缓冲寄存器  
        TMM->TMGRC1 = Inv_SineCtrl_Info.InvVol_SPWM_Compare_B;//PWM2的输出占空比设置   TMGRC1寄存器为TMGRA1寄存器的缓冲寄存器 			
    }


    /*------------------------------------------------------------------------------------*/			
    /*------------------------正弦表查表计数值更新----------------------------------------*/    
    Inv_SineCtrl_Info.SineTab_Index++;
    if ( Inv_SineCtrl_Info.SineTab_Index >= Inv_SineCtrl_Info.SineTab_PeriodPoint )
    {
        Inv_SineCtrl_Info.SineTab_Index =0;

        ADSample_Info.AC_Vol_Sum_Back       = ADSample_Info.AC_Vol_Sum;    //0.2us
        ADSample_Info.Inv_Cur_Sum_Back      = ADSample_Info.Inv_Cur_Sum;
        ADSample_Info.Load_Cur_Sum_Back     = ADSample_Info.Load_Cur_Sum;
        ADSample_Info.AC_Active_Power_Back  = ADSample_Info.AC_Active_Power_Sum;

        ADSample_Info.AC_Vol_Sum            = 0;
        ADSample_Info.Inv_Cur_Sum           = 0;
        ADSample_Info.Load_Cur_Sum          = 0;
        ADSample_Info.AC_Active_Power_Sum   = 0;	

    }	
    test_cnt++;
    if( test_cnt >= 10 )
    {
        test_cnt = 0;
        User_UART_View(); // 1M波特率 启用串口1调试 
    }     
    /*------------------------------------------------------------------------------------*/		
    /*-------------------------清除对应标志位与使能中断-----------------------------------*/				
    TMM->TMSR1 = tmsr1_temp & (uint8_t)~_20_TMM1_INTUN_GENERATE_FLAG;//清除TM1下溢标志位
    TMM->TMSR1 = 0;
    TMM->TMIER1 = tmier1_temp;//中断使能

}

/*************************************************
Description: tmb0_interrupt
Input      : 
Return     : 
Others     : IC2捕获中断：获取发电机转速
*************************************************/
static void tmb0_interrupt(void)
{
    uint32_t Temp_AA;
    uint32_t Temp_BB;
    uint8_t tbsr_temp,tbier_temp; 
    
    INTC_ClearPendingIRQ(TMB_IRQn); /* clear INTTMB interrupt flag */

    tbsr_temp = TMB->TBSR;
    tbier_temp = TMB->TBIER;
    TMB->TBIER = 0x00U;

    if ((TMB->TBSR & _08_TMB_REGISTER_OVERFLOW) == _08_TMB_REGISTER_OVERFLOW)
    {
        TMB->TBSR = tbsr_temp & (uint8_t)~_08_TMB_REGISTER_OVERFLOW;
        g_tmb_overflow_count_a += 1UL;
    }

#if 1
    if ((TMB->TBSR & _01_TMB_INTERRUPT_TBIMFA_ENABLE) == _01_TMB_INTERRUPT_TBIMFA_ENABLE)
    {
        TMB->TBSR = tbsr_temp & ~_01_TMB_INTERRUPT_TBIMFA_ENABLE;
        g_tmb_intTaken_a++;
					
        if (g_tmb_overflow_count_a == 0UL)
        {
            g_tmb_active_width_a = (TMB->TBGRA - TMB->TBGRC);					
        }
        else
        {
            g_tmb_active_width_a = (TMB->TBGRA - TMB->TBGRC) + (0x10000UL * g_tmb_overflow_count_a);//TMB->TBGRC为上一下的值
            g_tmb_overflow_count_a = 0UL;					
        }
    }
		

    s32_GeneratorcapvalCal     = g_tmb_active_width_a;//捕获A
    s32_GeneratorcapvalCaltemp = s32_GeneratorcapvalCal;

    if ( s32_GeneratorcapvalCal >   ECAP_VALUE_FREQ_LIMIT_UP )  
        s32_GeneratorcapvalCal  = ECAP_VALUE_FREQ_LIMIT_UP;
    if ( s32_GeneratorcapvalCal < ECAP_VALUE_FREQ_LIMIT_DOWN )  
        s32_GeneratorcapvalCal  = s32_GeneratorcapvalCal_OLd;

    s32_GeneratorcapvalCal_OLd  = s32_GeneratorcapvalCal;

    
    Temp_AA = ( ( 480000000L/CM_Engine_Pole_Pairs )) >> R_GQ;	//   (8MHz)*60=480 000 000       例：5000RPM * 114 >> 7  5000RPM * CM_RPM_BASE >> R_GQ
    Temp_BB = ( Temp_AA	* CM_RPM_BASE );// >> R_GQ;

    Temp_AA = s32_GeneratorcapvalCal ;   
    s16_GeneratorfreVal = ( Temp_BB / Temp_AA );

    if(s16_GeneratorfreVal_Old > 2262)//不等于最低转速时启用
    { 
        if(((s16_GeneratorfreVal - s16_GeneratorfreVal_Old) > 350 ||\
            (s16_GeneratorfreVal - s16_GeneratorfreVal_Old) < -350) && ADSample_Info.Bus_Vol_Filiter_AD > 1430)
            s16_GeneratorfreVal = s16_GeneratorfreVal_Old;
    }
       
    s16_GeneratorfreVal_Old = s16_GeneratorfreVal;// 滤波前的转速
    
    GeneratorfreFilter.half.hWord = (((uint32_t)GeneratorfreFilter.half.hWord * (4096-1500))  + ((uint32_t)s16_GeneratorfreVal * 1500) ) >> 12;//采集的转速一阶低通滤波    
    
    if(count_flag >= SpeedSampleCount)    count_flag = 0;
        
    s16_GeneratorfreVal_Array[count_flag] = GeneratorfreFilter.half.hWord; // 缓存滤波后转速值
    count_flag ++;	
		
		
#endif
    TMB->TBIER = tbier_temp;

}


/*************************************************
Description: SysTick_Handler
Input      : 
Return     : 
Others     : 系统定时器中断服务函数
*************************************************/
uint8_t Step_Motor_ms = 0;
extern uint16_t System_ProtectFlag_Info_First_Value,Temperature_Count;
extern uint8_t Inv_Out_Short_Flag_Count;
void SysTick_Handler(void)
{
    SysTick->CTRL|=SysTick_CTRL_COUNTFLAG_Msk;
	
    Step_Motor_ms ++;                       //步进电机执行周期计数
    StepMotor_Ctrl.periodCount++;         //步进电机控制周期计数
    SysClockBase_ms.system_1ms = 1;		//系统1ms 时钟标记
    SysClockBase_ms.stepMotor_1ms = 1;	//步进电机1ms 时钟标记
    SysClockBase_ms.faultCheck_1ms = 1;	    //故障检测1ms 时钟标记
	
/******************************************************************************/
//		__IO static uint8_t cnt_100ms = 0; 
//		// 串口调试 20250822
//		if(++cnt_100ms >= 100)
//    {
//			cnt_100ms = 0;
//			// 串口调试 20250822 
////      User_UART_View(); /*! 串口调试函数会“拉长”1ms节拍*/
//		}

/******************************************************************************/
    
    //System_ProtectFlag_Info_First_Value == 256 表示过温保护
    if(System_ProtectFlag_Info_First_Value == 256 && ADSample_Info.Temp_External_AD > COM_OTP1_VAL_BACK ) //因温度引起逆变器保护并且温度降至设定后开始计数
        Temperature_Count ++;
    

    if (Step_Motor_ms >= 3 && StepMotor_Ctrl.stepDirection != 0)//步进电机执行
    {
        Step_Motor_ms = 0;
        Stepmotordrive();//步进电机执行动作

//    *Step_Driver_Hardware_Buff[0] = (PORT->P2 & 0x0f)| Step_Driver_Signal_Buff[3];//PORTC端口先与上0xfc3f表示把RC6，7，8，9端口清零，再给其端口PORTC端口寄存器控制值
			
        StepMotor_Ctrl.stepActiveNum++;           //步进电机执行次数计数
        if ( StepMotor_Ctrl.stepActiveNum >= ( ABSFUN( StepMotor_Ctrl.stepNum )))  //步进电机执行次数大于控制次数后清零
        {
            StepMotor_Ctrl.stepNum = 0;
            StepMotor_Ctrl.stepActiveNum     = 0;
            StepMotor_Ctrl.stepDirection = 0; //步进电机方向清零
        }
    }
    
    
    //步进电机PI控制周期标志生成
    if (StepMotor_Ctrl.periodCount >= StepMotor_Ctrl.periodValue)	//控制周期标志位
    {          
        Stpmotorcon.motorconbit = 1;           
        StepMotor_Ctrl.periodCount = 0;
    }
    
    //1ms调用一次故障检测函数
    Fault_State_Check();

		//短路计数值 
	if( CM_RUN_STATE == State_Context.State_Identifier &&\
        Run_Deal.flag.bit.OpenDriver_Ok == E_TRUE   &&\
        Inv_Out_Short_Flag_Count <= INV_ACOUT_SCP_START_TIME) 
		{
            Inv_Out_Short_Flag_Count ++;
        }
}



