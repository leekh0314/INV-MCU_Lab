/*****************************************************************************
@Description: sys_define_config.h 系统配置头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/


#ifndef __SYS_DEFINE_CONFIG_H
#define	__SYS_DEFINE_CONFIG_H

/***************************************************************************/
/*-------------------EPWM开关周期设置--------------------------------------*/
/***************************************************************************/
#define MCU_CLK 	                            (64000000L)			/* PWM timer frequency[Hz] */
//#define PWM_FREQ	                            (12000L)				/* frequency[Hz] 12000hz*/
#define PWM_FREQ	                            (19200L)		  /* frequency[Hz]*/

#define DEADTIME			                    ((int32_t)((1.8 * MCU_CLK)) / 1000000L)
#if 1
    //中心互补对称周期值
    #define PWM_PERIOD	                        ((int32_t)((int32_t)MCU_CLK  / (PWM_FREQ * 2)) + DEADTIME - 2)   //1/fk* (m+2–p)* 2
    #define PWM_PERIOD_HALF 	                (PWM_PERIOD >> 1)
#else
    //锯齿波周期值
    #define PWM_PERIOD	                        ((int32_t)((int32_t)MCU_CLK  / (PWM_FREQ )) -1)  //1/fk * (m+1) = period = 1/PWM_FREQ   =>  m + 1 = fk/PWM_FREQ  锯齿波   
    #define PWM_PERIOD_HALF 	                (PWM_PERIOD >> 1)
#endif

/***************************************************************************/

/***************************************************************************/
/*-------------------串口波特率设置----------------------------------------*/
/***************************************************************************/
#define UART0_BAUDRATE                          (250000)        //UART0波特率设定值   
#define UART1_BAUDRATE                          (250000)        //UART1波特率设定值 
/***************************************************************************/

/***************************************************************************/
/*-------------------工作模式设置------------------------------------------*/
/***************************************************************************/
#define  NORMAL_MODE                            (0)
#define  DEBUG_MODE                             (1)
#define  OPERATING_MODE                         (NORMAL_MODE)
/***************************************************************************/

/*-------------------VACOUT输出电压缓启使能--------------------------------*/
#define INV_SOFTWARE_DISABLE                        (0)
#define INV_SOFTWARE_ENABLE                         (1)              
#define INV_SOFTWARE_CTRL_SELECT                    (INV_SOFTWARE_DISABLE)//是否启用输出电压缓启功能 
/*------------------------------------------------------------------------------------*/
//逆变输出电压缓启动相关参数
/*------------------------------------------------------------------------------------*/
#define INV_SOFTWARE_STEP_VALUE                         (50)//AC缓启动时，幅值系数每次步进值
#define INV_SOFTWARE_SHIFT_VALUE                        ( 6)//AC缓启动时，幅值系数缩放移位值
#define INV_SOFTWARE_HOLD_INIT                          (450 << INV_SOFTWARE_SHIFT_VALUE)//放大后的幅值系数初始值

/***************************************************************************/
/*-------------------IO端口宏定义------------------------------------------*/
/***************************************************************************/
#define LED_RED_ON                              ( PORT->PSET6 = (1<<1) )
#define LED_RED_OFF                             ( PORT->PCLR6 = (1<<1) )

#define LED_GREEN_ON                            ( PORT->PSET6 = (1<<0) )
#define LED_GREEN_OFF                           ( PORT->PCLR6 = (1<<0) )

//SCR：可控硅整流器（Silicon Controlled Rectifier）
#define INV_SCR_ENABLE                          ( PORT->PSET3 = (1<<0) )//宏定义输出控制 变量（整流器控制，“1”开启整流器）
#define INV_SCR_DISABLE                         ( PORT->PCLR3 = (1<<0) )//宏定义输出控制 变量（整流器控制，“0”关闭整流器）
#define INV_SCR_STATUS	                        (PORT_GetBit(PORT3,PIN0) >> 0)//


#define ECOMODE	 	                            (PORT_GetBit(PORT7,PIN4) >> 4) //节能模式
#define FAULT_RST	                            (PORT_GetBit(PORT3,PIN1) >> 1) //故障复位
#define FRESWITCH	                            (PORT_GetBit(PORT7,PIN1) >> 1)//逆变器频率选择，默认上拉输入(低电平有效)base_freq=1->50hz/0->60hz
/***************************************************************************/



/***************************************************************************/
/*-------------------逆变器参数配置----------------------------------------*/
/***************************************************************************/
/*------------------------------------------------------------------------------------*/
//输出电压幅值系数
/*------------------------------------------------------------------------------------*/
//#define INV_AC_VOL_AMP_VAL_NORMAL_REF           (1950)//(0 ~ 4096) 
//#define INV_AC_VOL_AMP_VAL_NORMAL_REF           (2100)//(0 ~ 4096) 调整输出电压幅值
//#define INV_AC_VOL_AMP_VAL_NORMAL_REF           (995)//(0 ~ 4096) 
#define INV_AC_VOL_AMP_VAL_NORMAL_REF           (2000)//(0 ~ 4096)

#define INV_AC_VOL_AMP_VAL_DEBUG_REF            (3300)//(0 ~ 4096) 

/*-------------------负载功率对输出电压幅值的影响斜率参数 20251022------------------- */
//负载越大电压越高 106/4096 ≈ 0.0259,功率每增加 1 W 幅值参考就上升约0.0259, count * W = 0.0259 * 2500W ≈ 64 赋值系数 , 幅值系数995 + 64 = 1059,输出电压约245VAC
#define INV_LOAD_PWR_SLOPE_NUM           (30)     /* 扩展：可替换成 106、85、75、10、45等 */  
/*-------------------输出电压类型------------------------------------------*/
#define  Out_Vol_110                            (110)
#define  Out_Vol_120                            (120)             
#define  Out_Vol_220                            (220)           
#define  Out_Vol_230                            (230)
#define  InvVol_Type                            (Out_Vol_230)  

/*-------------------输出电压频率------------------------------------------*/
#define  InvVol_Freq_50Hz                       (50)
#define  InvVol_Freq_60Hz                       (60)    
#define  InvVol_Freq                            (InvVol_Freq_50Hz)

/*-------------------频率切换开关------------------------------------------*/
#define  Freq_Choice_Disable                    (0)             
#define  Freq_Choice_Enable                     (1)               
#define  InvVol_Freq_Choice                     (Freq_Choice_Disable)

/*-------------------逆变器功率等级------------------------------------------*/
#define  Inv_Power_1000W                        (1000)                
#define  Inv_Power_1600W                        (1600)                
#define  Inv_Power_2000W                        (2000)              
#define  Inv_Power_Model                        (Inv_Power_2000W)

/*-------------------限功率控制使能----------------------------------------*/
#define INV_POWER_DISABLE                       (0)
#define INV_POWER_ENABLE                        (1)
#define INV_POWER_CTRL_SELECT                   (INV_POWER_ENABLE)//1：开启限功率控制；0：关闭限功率控制  

/*-------------------逆变器控制环路选择------------------------------------*/
#define CM_Inverter_Single_Loop                 (0)
#define CM_Inverter_Double_Loop                 (1)              
#define CM_Inverter_Loop_Ctrl                   (CM_Inverter_Double_Loop)

/*-------------------逆变器并联模式选择------------------------------------*/
#define CM_Inverter_Cross_Parallel              (0)
#define CM_Inverter_Equal_Parallel              (1)

#define CM_Inverter_Self_Parallel               (0)
#define CM_Inverter_Other_Parallel              (1)

#define CM_Inverter_Parallel_Type               (CM_Inverter_Cross_Parallel)
#define CM_Inverter_SelfOther_Parallel          (CM_Inverter_Other_Parallel)
/***************************************************************************/

/***************************************************************************/
/*-------------------步进电机类型选择--------------------------------------*/
/***************************************************************************/
//五线四相；四线两相
#define CM_Step_Four_Line                       (0)
#define CM_Step_Five_Line                       (1)

#define CM_Step_Two_Phase                       (0)
#define CM_Step_Four_Phase                      (1)

#define CM_Step_Type                            (CM_Step_Four_Line)
#define CM_Step_Driver_Phase                    (CM_Step_Two_Phase)
/***************************************************************************/

/***************************************************************************/
/*-------------------硬件参数配置------------------------------------------*/
/***************************************************************************/    
#define HW_ADC_REF                              ( 5.0)// AD参考工作电压                                                                                                                            
//#define HW_VBUS_GAIN                            ( 200.0)// 母线电压倍数    
//#define HW_VBUS_GAIN                            ( 80.5)// 母线电压倍数           
//#define HW_VBUS_GAIN                            ( 146.2)// 母线电压倍数              
#define HW_VBUS_GAIN                            ( 144.4)// 母线电压倍数   

#define HW_AUX_POWER_GAIN                       ( 6.0)//辅助电源电压倍数   
//#define HW_V_ACOUT_GAIN                         ( 267.3)// 输出电压放大倍数 
//#define HW_V_ACOUT_GAIN                         ( 133.5)// 输出电压放大倍数 
//#define HW_V_ACOUT_GAIN                         ( 256.0)// 输出电压放大倍数 
//#define HW_V_ACOUT_GAIN                         ( 744.5)// 输出电压放大倍数 
#define HW_V_ACOUT_GAIN                         ( 378.0)// 输出电压放大倍数 



//#define HW_I_INDUC_GAIN                         ( 20.0)// 电流放大倍数 
//#define HW_I_INDUC_GAIN                         ( 33.3)// 电流放大倍数 电路33.3:1 20250829
#define HW_I_INDUC_GAIN                         ( 49.7)// 电流放大倍数 

#define HW_TEMP_DIVIDE_RES                      ( 10.00)// 温度采样分压电阻值(KΩ)

#define COM_I_INDUC_BASE                        ((int32_t)(HW_ADC_REF * HW_I_INDUC_GAIN))
#define COM_V_ACOUT_BASE                        ((int32_t)(HW_ADC_REF * HW_V_ACOUT_GAIN))//放大倍数 * AD参考电压
#define COM_VBUS_BASE                           ((int32_t)(HW_ADC_REF * HW_VBUS_GAIN))
#define COM_AUX_POWER_BASE                      ((int32_t)(HW_ADC_REF * HW_AUX_POWER_GAIN))


/***************************************************************************/
/*-------------------正弦表点数设置----------------------------------------*/
/***************************************************************************/
#if(PWM_FREQ == 9600)
    #define SPWMWAVE_DOT_50Hz                   (192)                   
    #define SPWMWAVE_DOT_50Hz_2                 (96)                  
    #define SPWMWAVE_DOT_50Hz_4                 (48)                  
    #define SPWMWAVE_DOT_RECIPROCAL_50Hz        (171)                  //_IQ(1/192) * 32768
    #define SINE_FREQ_VALUE_50Hz                (50)                    //9.6khz

    #define SPWMWAVE_DOT_60Hz                   (160)
    #define SPWMWAVE_DOT_60Hz_2                 (80)
    #define SPWMWAVE_DOT_60Hz_4                 (40)
    #define SPWMWAVE_DOT_RECIPROCAL_60Hz        (205)  //_IQ(1/160*32768)  9.6K
    #define SINE_FREQ_VALUE_60Hz                (60)//hz
#elif(PWM_FREQ == 12000)
    #define SPWMWAVE_DOT_50Hz                   (240)                   
    #define SPWMWAVE_DOT_50Hz_2                 (120)                  
    #define SPWMWAVE_DOT_50Hz_4                 (60)                  
    #define SPWMWAVE_DOT_RECIPROCAL_50Hz        (32768/SPWMWAVE_DOT_50Hz*2)                  //_IQ(1/240) * 32768
    #define SINE_FREQ_VALUE_50Hz                (50)                    //12khz
    

    #define SPWMWAVE_DOT_60Hz                   (200)
    #define SPWMWAVE_DOT_60Hz_2                 (100)
    #define SPWMWAVE_DOT_60Hz_4                 (50)
    #define SPWMWAVE_DOT_RECIPROCAL_60Hz        (164)  //_IQ(1/320)  12K
    #define SINE_FREQ_VALUE_60Hz                (60)//hz
#elif(PWM_FREQ == 19200)
    #define SPWMWAVE_DOT_50Hz                   (384)              
    #define SPWMWAVE_DOT_50Hz_2                 (192)               
    #define SPWMWAVE_DOT_50Hz_4                 (96)              
    #define SPWMWAVE_DOT_RECIPROCAL_50Hz        (85)                //_IQ(1/192) * 32768
    #define SINE_FREQ_VALUE_50Hz                (50)               //19.2khz

    #define SPWMWAVE_DOT_60Hz                   (320)
    #define SPWMWAVE_DOT_60Hz_2                 (160)
    #define SPWMWAVE_DOT_60Hz_4                 (80)
    #define SPWMWAVE_DOT_RECIPROCAL_60Hz        (102)  //_IQ(1/320*32768)  19.2K
    #define SINE_FREQ_VALUE_60Hz                (60)//hz
#elif(PWM_FREQ == 21000)    
    #define SPWMWAVE_DOT_50Hz                           (420)              
    #define SPWMWAVE_DOT_50Hz_2                         (210)               
    #define SPWMWAVE_DOT_50Hz_4                         (105)              
    #define SPWMWAVE_DOT_RECIPROCAL_50Hz                (78)               
    #define SINE_FREQ_VALUE_50Hz                        (50)   

    #define SPWMWAVE_DOT_60Hz                           (350)
    #define SPWMWAVE_DOT_60Hz_2                         (175)
    #define SPWMWAVE_DOT_60Hz_4                         (87)
    #define SPWMWAVE_DOT_RECIPROCAL_60Hz                (94)//_IQ(1/350) * 32768  
    #define SINE_FREQ_VALUE_60Hz                        (60)//hz    
    
#elif(PWM_FREQ == 24000)    
    #define SPWMWAVE_DOT_50Hz                   (480)              
    #define SPWMWAVE_DOT_50Hz_2                 (240)               
    #define SPWMWAVE_DOT_50Hz_4                 (120)              
    #define SPWMWAVE_DOT_RECIPROCAL_50Hz        (68)                //_IQ(1/480) * 32768  24Khz
    #define SINE_FREQ_VALUE_50Hz                (50)               //    
    
    #define SPWMWAVE_DOT_60Hz                   (400)
    #define SPWMWAVE_DOT_60Hz_2                 (200)
    #define SPWMWAVE_DOT_60Hz_4                 (100)
    #define SPWMWAVE_DOT_RECIPROCAL_60Hz        (82)  //_IQ(1/400) * 32768  24K
    #define SINE_FREQ_VALUE_60Hz                (60)//hz    
#endif


/***************************************************************************/



/***************************************************************************/
#endif 
