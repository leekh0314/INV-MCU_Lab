/*****************************************************************************
@Description: sys_define_parar.h 系统参数定义头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __SYS_DEFINE_PARAR_H
#define __SYS_DEFINE_PARAR_H
#include "sys_define_config.h"




/***************************************************************************/
//虚拟阻抗系数
/***************************************************************************/
#define INV_VIRTUAL_RES                                     ( 0 )


/***************************************************************************/
/*VREF_AD校正值*/
/***************************************************************************/
#define COM_VREF_AD_CORRECT_UP                              (4296)
#define COM_VREF_AD_CORRECT_MID                             (4096)
#define COM_VREF_AD_CORRECT_DOWN                            (3896)

/***************************************************************************/
//电压开机校正值
/***************************************************************************/
#define COM_V_ACOUT_AD_CORRECT_UP                           ( 4296 )
#define COM_V_ACOUT_AD_CORRECT_MID                          ( 4096 )
#define COM_V_ACOUT_AD_CORRECT_DOWN                         ( 3896 )

/***************************************************************************/
//电感电流开机校正值
/***************************************************************************/    
#define COM_I_INDUC_AD_CORRECT_UP                           ( 4296 )
#define COM_I_INDUC_AD_CORRECT_MID                          ( 4096 )
#define COM_I_INDUC_AD_CORRECT_DOWN                         ( 3896 )
    

/***************************************************************************/
//按键监测参数设置
/***************************************************************************/
#define Key_Pro_Reset_Compare_Dot                           (0)
#define Key_Pro_Reset_Compare_Type                          E_EQUAL
#define Key_Pro_Reset_Delay_MaxCount                        (200)

#define Key_SW_ECO_Compare_Dot                              (0)
#define Key_SW_ECO_Compare_Type                             E_EQUAL
#define Key_SW_ECO_Delay_MaxCount                           (200)

#define Key_SW_FreqChoice_Compare_Dot                       (0)
#define Key_SW_FreqChoice_Compare_Type                      E_EQUAL
#define Key_SW_FreqChoice_Delay_MaxCount                    (200)
    
/***************************************************************************/
//开机检测温度参数设置
/***************************************************************************/
#define COM_START_CHECK_TEMP_UP_RES                         (2.240)//设定温度70℃对应NTC电阻值(KΩ)
#define COM_START_CHECK_TEMP_UP_BACK_RES                    (1.678)//设定温度80℃对应NTC电阻值(KΩ)
#define COM_START_CHECK_TEMP_UP                             (int32_t)((COM_START_CHECK_TEMP_UP_RES/(COM_START_CHECK_TEMP_UP_RES+HW_TEMP_DIVIDE_RES)*4096))  
#define COM_START_CHECK_TEMP_UP_BACK                        (int32_t)((COM_START_CHECK_TEMP_UP_BACK_RES/(COM_START_CHECK_TEMP_UP_BACK_RES+HW_TEMP_DIVIDE_RES)*4096)) 
#define COM_START_CHECK_TEMP_UP_TIME		                (700)//延时时间(ms)


/*------------------------------------------------------------------------------------*/
//辅助电源上下限参数设置
/*------------------------------------------------------------------------------------*/

#define COM_AUX_POWER_MAX                                   (15.0)//(V)
//#define COM_AUX_POWER_MIN                                   (9.9)
//#define COM_AUX_POWER_VALBACK                               (0.3)//保护使用的回差值
#define COM_AUX_POWER_MIN                                   (8.5) // 20251015
#define COM_AUX_POWER_VALBACK                               (0.5) // 20251015 保护使用的回差值
/***************************************************************************/
//开机检测辅助电源参数设置
/***************************************************************************/
#define COM_START_CHECK_AUX_POWER_UP                        (int32_t)( COM_AUX_POWER_MAX * 4096/COM_AUX_POWER_BASE)// 2048 辅助电源真是电压(V)*COM_REAL_AUXPOWER_SCAL   
#define COM_START_CHECK_AUX_POWER_UP_BACK                   (int32_t)((COM_AUX_POWER_MAX + COM_AUX_POWER_VALBACK) * 4096/COM_AUX_POWER_BASE)// 2116.3
#define COM_START_CHECK_AUX_POWER_UP_TIME		            (700)//延时时间(ms)

#define COM_START_CHECK_AUX_POWER_DN                        (int32_t)( COM_AUX_POWER_MIN * 4096/COM_AUX_POWER_BASE)	// 1228.8
#define COM_START_CHECK_AUX_POWER_DN_BACK                   (int32_t)((COM_AUX_POWER_MIN - COM_AUX_POWER_VALBACK) * 4096/COM_AUX_POWER_BASE)// 1160.5
#define COM_START_CHECK_AUX_POWER_DN_TIME			        (700)//延时时间(ms)

/***************************************************************************/
//辅助电源过压/欠压设置
/***************************************************************************/
#define COM_AUX_POWER_OVP_TIME                              (100)//保时时间(ms))
#define COM_AUX_POWER_OVP_VALUE                             (int32_t)( COM_AUX_POWER_MAX * 4096/COM_AUX_POWER_BASE)//辅助电源真是电压(V)*COM_REAL_AUXPOWER_SCAL   
#define COM_AUX_POWER_OVP_VALUE_BACK                        (int32_t)((COM_AUX_POWER_MAX - COM_AUX_POWER_VALBACK)* 4096/COM_AUX_POWER_BASE)   

#define COM_AUX_POWER_LVP_TIME                              (200)//延时时间(ms) 
#define COM_AUX_POWER_LVP_VALUE                             (int32_t)( COM_AUX_POWER_MIN * 4096/COM_AUX_POWER_BASE)    
#define COM_AUX_POWER_LVP_VALUE_BACK                        (int32_t)((COM_AUX_POWER_MIN + COM_AUX_POWER_VALBACK) * 4096/COM_AUX_POWER_BASE)


/***************************************************************************/
//过温设置
//过温保护：OTP(Over-temperature protection)
/***************************************************************************/	
#define COM_OTP1_TIME                                       (100)//延时时间(ms)
#define COM_OTP2_TIME                                       (100)//延时时间(ms)
#define COM_OTP1_RES                                        (1.678)//设定温度80℃对应NTC电阻值(KΩ)
#define COM_OTP2_RES                                        (1.275)//设定温度90℃对应NTC电阻值(KΩ)
#define COM_OTP1_BACK_RES                                   (2.240)//设定温度70℃对应NTC电阻值(KΩ)
#define COM_OTP2_BACK_RES                                   (2.240)//设定温度70℃对应NTC电阻值(KΩ)                          
#define COM_OTP1_VAL                                        (int32_t)((COM_OTP1_RES/(COM_OTP1_RES+HW_TEMP_DIVIDE_RES)*4096))// ADC阈值588
#define COM_OTP2_VAL                                        (int32_t)((COM_OTP2_RES/(COM_OTP2_RES+HW_TEMP_DIVIDE_RES)*4096))// ADC阈值463
#define COM_OTP1_VAL_BACK                                   (int32_t)((COM_OTP1_BACK_RES/(COM_OTP1_BACK_RES+HW_TEMP_DIVIDE_RES)*4096))// ADC阈值749 
#define COM_OTP2_VAL_BACK                                   (int32_t)((COM_OTP2_BACK_RES/(COM_OTP2_BACK_RES+HW_TEMP_DIVIDE_RES)*4096))	// ADC阈值749 
/***************************************************************************/
/***************************************************************************/

/***************************************************************************/
//VREF参考电压保护点设置
/***************************************************************************/	
#define COM_VREF_MAX                                        (2248)
#define COM_VREF_MIN                                        (1848)
#define COM_VREF_VALBACK                                    ( 80)//保护使用的回差值

#define COM_VREF_OVER_TIME                                  (100)//延时时间(ms)
#define COM_VREF_OVER_VALUE                                 (COM_VREF_MAX)//
#define COM_VREF_OVER_VALUE_BACK                            (COM_VREF_MAX - COM_VREF_VALBACK)

#define COM_VREF_LOW_TIME                                   (100)//延时时间(ms)
#define COM_VREF_LOW_VALUE                                  (COM_VREF_MIN)//
#define COM_VREF_LOW_VALUE_BACK                             (COM_VREF_MIN + COM_VREF_VALBACK)	
/***************************************************************************/


/***************************************************************************/
//输出过压/欠压、短路参数设置
/***************************************************************************/
#if( InvVol_Type == Out_Vol_120 )
    /*------------------------------------------------------------------------------------*/
    //输出过压/欠压参数设置
    //有效值：RMS
    //过流保护：OVP
    /*------------------------------------------------------------------------------------*/
    #define VACOUT_RMS_OVP                                  (255)//(V)
    #define VACOUT_RMS_LVP                                  (160)//(V)
//    #define VACOUT_RMS_OVP                                  (133)//(V) 120VAC OUT 20250830
//    #define VACOUT_RMS_LVP                                  (105)//(V)  20250830
		
    #define VACOUT_RMS_VP_VALBACK                           ( 5)//保护使用的回差值(V)
    
    #define INV_ACOUT_RMS_OVP_TIME                          ( 300)//延时时间(ms)        
    #define INV_ACOUT_RMS_OVP_VAL                           ( VACOUT_RMS_OVP * 4)//真实有效值(V)*COM_REAL_VACOUT_RMS_SCAL  
    #define INV_ACOUT_RMS_OVP_VAL_BACK                      ((VACOUT_RMS_OVP - VACOUT_RMS_VP_VALBACK) * 4)  

    #define INV_ACOUT_RMS_LVP_TIME                          ( 1500)//延时时间(ms) 
    #define INV_ACOUT_RMS_LVP_VAL                           ( VACOUT_RMS_LVP * 4) 
    #define INV_ACOUT_RMS_LVP_VAL_BACK                      ((VACOUT_RMS_LVP + VACOUT_RMS_VP_VALBACK) * 4)      

    /***************************************************************************/
    //输出短路变量设置
    //短路保护：SCP(short circuit protect)
    /***************************************************************************/
    #define VACOUT_RMS_SCP                                  ( 20)//(V)
    #define VACOUT_RMS_SCP_VALBACK                          ( 15)//保护使用的回差值(V)
        
    #define INV_ACOUT_SCP_TIME                              ( 55)//延时时间(ms)      	
    #define INV_ACOUT_SCP_VAL                               ( VACOUT_RMS_SCP * 4)//真实有效值(V)*4
    #define INV_ACOUT_SCP_VAL_BACK                          ((VACOUT_RMS_SCP + VACOUT_RMS_SCP_VALBACK) * 4)  

    //启用短路功能相关设定值
    #define INV_ACOUT_SCP_START_FLAG                        ( 1)//1：初始态不启用短路功能；0：初始态启用短路功能
    #define INV_ACOUT_SCP_START_TIME                        (60)//延时时间(ms)  
    #define INV_ACOUT_SCP_START_VAL                         (100 * 4)//真实有效值(V)*4   

#elif ( InvVol_Type == Out_Vol_220 ||InvVol_Type == Out_Vol_230 ) 
    /*------------------------------------------------------------------------------------*/
    //输出过压/欠压参数设置
    //有效值：RMS
    //过流保护：OVP
    /*------------------------------------------------------------------------------------*/
    #define VACOUT_RMS_OVP                                  (255)//(V)
    #define VACOUT_RMS_LVP                                  (130)//(V)
    #define VACOUT_RMS_VP_VALBACK                           ( 5)//保护使用的回差值(V)
		
    #define INV_ACOUT_RMS_OVP_TIME                          ( 300)//延时时间(ms)        
    #define INV_ACOUT_RMS_OVP_VAL                           ( VACOUT_RMS_OVP * 4)//  真实有效值(V)*COM_REAL_VACOUT_RMS_SCAL
    #define INV_ACOUT_RMS_OVP_VAL_BACK                      ((VACOUT_RMS_OVP - VACOUT_RMS_VP_VALBACK) * 4)  //   转换成内部定点值便于直接比较 1020 = 255*4

    #define INV_ACOUT_RMS_LVP_TIME                          ( 1500)//延时时间(ms) 
    #define INV_ACOUT_RMS_LVP_VAL                           ( VACOUT_RMS_LVP * 4) 
    #define INV_ACOUT_RMS_LVP_VAL_BACK                      ((VACOUT_RMS_LVP + VACOUT_RMS_VP_VALBACK) * 4)      
    
    /***************************************************************************/
    //输出短路变量设置
    //短路保护：SCP(short circuit protect)
    /***************************************************************************/
    #define VACOUT_RMS_SCP                                  ( 20)//(V)
    #define VACOUT_RMS_SCP_VALBACK                          ( 15)//保护使用的回差值(V)
        
    #define INV_ACOUT_SCP_TIME                              ( 55)//延时时间(ms)      	
    #define INV_ACOUT_SCP_VAL                               ( VACOUT_RMS_SCP * 4)//真实有效值(V)*4  
    #define INV_ACOUT_SCP_VAL_BACK                          ((VACOUT_RMS_SCP + VACOUT_RMS_SCP_VALBACK) * 4)  

    //启用短路功能相关设定值
    #define INV_ACOUT_SCP_START_FLAG                        ( 1)//1：初始态不启用短路功能；0：初始态启用短路功能
    #define INV_ACOUT_SCP_START_TIME                        (60)//延时时间(ms)  
    #define INV_ACOUT_SCP_START_VAL                         (100 * 4)//真实有效值(V)*4   
#endif


/***************************************************************************/
//400V母线过压/欠压设置
//过压保护：OVP
//欠压保护：LVP
/***************************************************************************/
#if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
    /*------------------------------------------------------------------------------------*/
    //开机检测母线参数设置
    /*------------------------------------------------------------------------------------*/
    #define INV_START_CHECK_VBUS_MAX                        (430)//(V)
    #define INV_START_CHECK_VBUS_MIN                        (340)//(V)
    #define INV_START_CHECK_VBUS_VALBACK                    ( 5)//保护使用的回差值(V)
    
    #define INV_START_CHECK_VBUS_UP                         ( INV_START_CHECK_VBUS_MAX * 4096 / COM_VBUS_BASE )//母线电压真实值(V)*  
    #define INV_START_CHECK_VBUS_UP_BACK                    ((INV_START_CHECK_VBUS_MAX + INV_START_CHECK_VBUS_VALBACK) * 4096 / COM_VBUS_BASE )
    #define INV_START_CHECK_VBUS_UP_TIME		            (10)//延时时间(ms)

    #define INV_START_CHECK_VBUS_DN                         ( INV_START_CHECK_VBUS_MIN * 4096 / COM_VBUS_BASE )	
    #define INV_START_CHECK_VBUS_DN_BACK                    ((INV_START_CHECK_VBUS_MIN - INV_START_CHECK_VBUS_VALBACK) * 4096 / COM_VBUS_BASE )
    #define INV_START_CHECK_VBUS_DN_TIME			        (10)//延时时间(ms)

///******************************************************************************************************************************/
//// 20250830 测试 210VDC/120VAC NOMORL模式
//    #define INV_START_CHECK_VBUS_UP                         (( 240 << 12 ) / COM_VBUS_BASE )//转换为与AD采样值统一单位进行比较  20250827
//    #define INV_START_CHECK_VBUS_UP_BACK                    (( 235 << 12 ) / COM_VBUS_BASE ) // 20250827
//    #define INV_START_CHECK_VBUS_UP_TIME		            (500)//延时时间(ms)
//    #define INV_START_CHECK_VBUS_DN                         (( 180 << 12 ) / COM_VBUS_BASE )//转换为与AD采样值统一单位进行比较	
//    #define INV_START_CHECK_VBUS_DN_BACK                    (( 175 << 12 ) / COM_VBUS_BASE )
//    #define INV_START_CHECK_VBUS_DN_TIME			        (1000)//延时时间(ms)
//		
//    #define COM_VBUS_OVP1_TIME                              (100)//保时时间(ms))
//    #define COM_VBUS_OVP1_VAL                               (int32_t)((280 * 4096)/COM_VBUS_BASE)//转换为与AD采样值统一单位进行比较   
//    #define COM_VBUS_OVP1_VAL_BACK                          (int32_t)((275 * 4096)/COM_VBUS_BASE)   

//    #define COM_VBUS_OVP2_TIME                              (50)//保时时间(ms))
//    #define COM_VBUS_OVP2_VAL                               (int32_t)((280 * 4096)/COM_VBUS_BASE)// COM_VBUS_BASE = 5*80.5   2560 转换为与AD采样值统一单位进行比较 20250823  
//    #define COM_VBUS_OVP2_VAL_BACK                          (int32_t)((275 * 4096)/COM_VBUS_BASE)   

//    #define COM_VBUS_OVP3_TIME                              (15)//保时时间(ms))
//    #define COM_VBUS_OVP3_VAL                               (int32_t)((290 * 4096)/COM_VBUS_BASE)//转换为与AD采样值统一单位进行比较   
//    #define COM_VBUS_OVP3_VAL_BACK                          (int32_t)((285 * 4096)/COM_VBUS_BASE) 
//		
//    /*------------------------------------------------------------------------------------*/
//    //母线欠压参数设置
//    /*------------------------------------------------------------------------------------*/
//    #define COM_VBUS_LVP1_TIME                              (5000)//延时时间(ms) 
//    #define COM_VBUS_LVP1_VAL                               (int32_t)((160 * 4096)/COM_VBUS_BASE)//转换为与AD采样值统一单位进行比较    
//    #define COM_VBUS_LVP1_VAL_BACK                          (int32_t)((165 * 4096)/COM_VBUS_BASE)

//    #define COM_VBUS_LVP2_TIME                              (3000)//延时时间(ms) 
//    #define COM_VBUS_LVP2_VAL                               (int32_t)((130 * 4096)/COM_VBUS_BASE)//转换为与AD采样值统一单位进行比较    
//    #define COM_VBUS_LVP2_VAL_BACK                          (int32_t)((135 * 4096)/COM_VBUS_BASE)

//    #define COM_VBUS_LVP3_TIME                              (1000)//延时时间(ms) 
//    #define COM_VBUS_LVP3_VAL                               (int32_t)((110 * 4096)/COM_VBUS_BASE)// 转换为与AD采样值统一单位进行比较    
//    #define COM_VBUS_LVP3_VAL_BACK                          (int32_t)((115 * 4096)/COM_VBUS_BASE)
///******************************************************************************************************************************/
    #define COM_VBUS_OVP1                                   (420)//(V)
    #define COM_VBUS_OVP2                                   (425)//(V)
    #define COM_VBUS_OVP3                                   (440)//(V)
    #define COM_VBUS_LVP1                                   (330)//(V)
    #define COM_VBUS_LVP2                                   (320)//(V)
    #define COM_VBUS_LVP3                                   (300)//(V)    
    #define COM_VBUS_VP_VALBACK                             ( 5)//保护使用的回差值(V)
    
    #define COM_VBUS_OVP1_TIME		                        (30)//延时时间(ms)
    #define COM_VBUS_OVP2_TIME			                    (30)//延时时间(ms))
    #define COM_VBUS_OVP3_TIME			                    (30)//延时时间(ms)
		
    #define COM_VBUS_OVP1_VAL			                    ( COM_VBUS_OVP1 * 4096 / COM_VBUS_BASE  )//母线电压真实值(V)*COM_REAL_VBUS_SCAL
    #define COM_VBUS_OVP2_VAL			                    ( COM_VBUS_OVP2 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP3_VAL			                    ( COM_VBUS_OVP3 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP1_VAL_BACK                          ((COM_VBUS_OVP1 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP2_VAL_BACK			                ((COM_VBUS_OVP2 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP3_VAL_BACK			                ((COM_VBUS_OVP3 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )

    #define COM_VBUS_LVP1_TIME			                    (5000)//延时时间(ms)
    #define COM_VBUS_LVP2_TIME			                    (3000)//延时时间(ms)
    #define COM_VBUS_LVP3_TIME			                    (210)//延时时间(ms)
		
    #define COM_VBUS_LVP1_VAL			                    ( COM_VBUS_LVP1 * 4096 / COM_VBUS_BASE  )//母线电压真实值(V)*COM_REAL_VBUS_SCAL
    #define COM_VBUS_LVP2_VAL			                    ( COM_VBUS_LVP2 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP3_VAL		                        ( COM_VBUS_LVP3 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP1_VAL_BACK                          ((COM_VBUS_LVP1 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP2_VAL_BACK			                ((COM_VBUS_LVP2 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP3_VAL_BACK			                ((COM_VBUS_LVP3 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )		 
#else
    /*------------------------------------------------------------------------------------*/
    //开机检测母线参数设置
    /*------------------------------------------------------------------------------------*/
    #define INV_START_CHECK_VBUS_MAX                        (430)//(V)
    #define INV_START_CHECK_VBUS_MIN                        (340)//(V)
    #define INV_START_CHECK_VBUS_VALBACK                    ( 5)//保护使用的回差值(V)
    
    #define INV_START_CHECK_VBUS_UP                         ( INV_START_CHECK_VBUS_MAX * 4096 / COM_VBUS_BASE )//母线电压真实值(V)*  
    #define INV_START_CHECK_VBUS_UP_BACK                    ((INV_START_CHECK_VBUS_MAX + INV_START_CHECK_VBUS_VALBACK) * 4096 / COM_VBUS_BASE )
    #define INV_START_CHECK_VBUS_UP_TIME		            (210)//延时时间(ms)

    #define INV_START_CHECK_VBUS_DN                         ( INV_START_CHECK_VBUS_MIN * 4096 / COM_VBUS_BASE )	
    #define INV_START_CHECK_VBUS_DN_BACK                    ((INV_START_CHECK_VBUS_MIN - INV_START_CHECK_VBUS_VALBACK) * 4096 / COM_VBUS_BASE )
    #define INV_START_CHECK_VBUS_DN_TIME			        (210)//延时时间(ms)

    #define COM_VBUS_OVP1                                   (430)//(V)
    #define COM_VBUS_OVP2                                   (435)//(V)
    #define COM_VBUS_OVP3                                   (445)//(V)
    #define COM_VBUS_LVP1                                   (330)//(V)
    #define COM_VBUS_LVP2                                   (320)//(V)
    #define COM_VBUS_LVP3                                   (300)//(V)    
    #define COM_VBUS_VP_VALBACK                             ( 5)//保护使用的回差值(V)
    
    #define COM_VBUS_OVP1_TIME		                        (30)//延时时间(ms)
    #define COM_VBUS_OVP2_TIME			                    (20)//延时时间(ms))
    #define COM_VBUS_OVP3_TIME			                    (10)//延时时间(ms)
    #define COM_VBUS_OVP1_VAL			                    ( COM_VBUS_OVP1 * 4096 / COM_VBUS_BASE  )//母线电压真实值(V)*COM_REAL_VBUS_SCAL
    #define COM_VBUS_OVP2_VAL			                    ( COM_VBUS_OVP2 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP3_VAL			                    ( COM_VBUS_OVP3 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP1_VAL_BACK                          ((COM_VBUS_OVP1 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP2_VAL_BACK			                ((COM_VBUS_OVP2 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_OVP3_VAL_BACK			                ((COM_VBUS_OVP3 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )

    #define COM_VBUS_LVP1_TIME			                    (5000)//延时时间(ms)
    #define COM_VBUS_LVP2_TIME			                    (3000)//延时时间(ms)
    #define COM_VBUS_LVP3_TIME			                    (210)//延时时间(ms)
    #define COM_VBUS_LVP1_VAL			                    ( COM_VBUS_LVP1 * 4096 / COM_VBUS_BASE  )//母线电压真实值(V)*COM_REAL_VBUS_SCAL
    #define COM_VBUS_LVP2_VAL			                    ( COM_VBUS_LVP2 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP3_VAL		                        ( COM_VBUS_LVP3 * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP1_VAL_BACK                          ((COM_VBUS_LVP1 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP2_VAL_BACK			                ((COM_VBUS_LVP2 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
    #define COM_VBUS_LVP3_VAL_BACK			                ((COM_VBUS_LVP3 - COM_VBUS_VP_VALBACK) * 4096 / COM_VBUS_BASE  )
#endif


/***************************************************************************/
/***************************************************************************/
#if ( Inv_Power_Model == Inv_Power_1000W )
    /*------------------------------------------------------------------------------------*/
    //开机检测转速参数设置
    /*------------------------------------------------------------------------------------*/
    #define COM_START_CHECK_RPM_UP                          (( 5100UL * CM_RPM_BASE ) >> R_GQ)//5688     //5000rpm
    #define COM_START_CHECK_RPM_UP_BACK                     (( 5000UL * CM_RPM_BASE ) >> R_GQ)//5800	    //5100rpm
    #define COM_START_CHECK_RPM_UP_TIME			            ( 500)		  //0.5Sec 

    /*------------------------------------------------------------------------------------*/
    //过速、欠速设置
    /*------------------------------------------------------------------------------------*/
    #define COM_RPM_OVER_TIME                               (10000)       //10sec 		 
    #define COM_RPM_OVER_VAL                                (( 5800UL * CM_RPM_BASE ) >> R_GQ)
    #define COM_RPM_OVER_VAL_BACK                           (( 5600UL * CM_RPM_BASE ) >> R_GQ)

    #define COM_RPM_LOW_TIME                                (2500)       //3sec 		 
    #define COM_RPM_LOW_VAL                                 (( 3000UL * CM_RPM_BASE ) >> R_GQ)
    #define COM_RPM_LOW_VAL_BACK                            (( 3500UL * CM_RPM_BASE ) >> R_GQ)	    
    
    /*------------------------------------------------------------------------------------*/
    //有功功率P(W)过载参数设置
    //过载保护：OLP(over load protect)
    /*------------------------------------------------------------------------------------*/
    #define INV_P_OLP1                                      (2900)//(W)
    #define INV_P_OLP2                                      (2950)//(W)
    #define INV_P_OLP3                                      (3000)//(W)
    #define INV_P_OLP4                                      (3050)//(W)
    #define INV_P_OLP_VALBACK                               ( 30)//保护使用的回差值(W)

    #define INV_P_OLP1_TIME                                 (40000)//延时时间(ms)   
    #define INV_P_OLP2_TIME                                 (8000)//延时时间(ms)    
    #define INV_P_OLP3_TIME	                                (6000)//延时时间(ms)     
    #define INV_P_OLP4_TIME	                                (6000)//延时时间(ms)     

    #define INV_P_OLP1_VAL                                  (INV_P_OLP1) //设定功率值(W)     
    #define INV_P_OLP2_VAL                                  (INV_P_OLP2)      
    #define INV_P_OLP3_VAL	                                (INV_P_OLP3)      
    #define INV_P_OLP4_VAL	                                (INV_P_OLP4) 

    #define INV_P_OLP1_VAL_BACK                             (INV_P_OLP1 - INV_P_OLP_VALBACK)
    #define INV_P_OLP2_VAL_BACK                             (INV_P_OLP2 - INV_P_OLP_VALBACK)
    #define INV_P_OLP3_VAL_BACK	                            (INV_P_OLP3 - INV_P_OLP_VALBACK)
    #define INV_P_OLP4_VAL_BACK	                            (INV_P_OLP4 - INV_P_OLP_VALBACK)

    /*------------------------------------------------------------------------------------*/
    //视在功率-S(VA)过载参数设置
    /*------------------------------------------------------------------------------------*/
    #define INV_S_OLP1                                      (3000)//(VA)
    #define INV_S_OLP2                                      (3100)//(VA)
    #define INV_S_OLP3                                      (3200)//(VA)
    #define INV_S_OLP4                                      (3200)//(VA)
    #define INV_S_OLP_VALBACK                               ( 30)//保护使用的回差值(VA)

    #define INV_S_OLP1_TIME                                 (120000)//延时时间(ms)   
    #define INV_S_OLP2_TIME                                 (20000)//延时时间(ms)    
    #define INV_S_OLP3_TIME	                                (2000)//延时时间(ms)     
    #define INV_S_OLP4_TIME	                                (1000)//延时时间(ms)     

    #define INV_S_OLP1_VAL                                  (INV_S_OLP1)    
    #define INV_S_OLP2_VAL                                  (INV_S_OLP2)      
    #define INV_S_OLP3_VAL	                                (INV_S_OLP3)      
    #define INV_S_OLP4_VAL	                                (INV_S_OLP4) 

    #define INV_S_OLP1_VAL_BACK                             (INV_S_OLP1 - INV_S_OLP_VALBACK)
    #define INV_S_OLP2_VAL_BACK                             (INV_S_OLP2 - INV_S_OLP_VALBACK)
    #define INV_S_OLP3_VAL_BACK	                            (INV_S_OLP3 - INV_S_OLP_VALBACK)
    #define INV_S_OLP4_VAL_BACK	                            (INV_S_OLP4 - INV_S_OLP_VALBACK)

    /*------------------------------------------------------------------------------------*/
    //电感过流参数有效值保护设置
    //有效值：RMS
    //过流保护：OCP
    /*------------------------------------------------------------------------------------*/
     #if ( InvVol_Type == Out_Vol_110 ||InvVol_Type == Out_Vol_120) 
        #define INV_INDUC_RMS_OCP1                          (14.5)//(A)
        #define INV_INDUC_RMS_OCP2                          (15.0)//(A)
        #define INV_INDUC_RMS_OCP_VALBACK                   ( 0.4)//保护使用的回差值(A)

        #define INV_INDUC_RMS_OCP1_TIME                     (40000)//延时时间(ms)
        #define INV_INDUC_RMS_OCP2_TIME                     (8000)//延时时间(ms)

        #define INV_INDUC_RMS_OCP1_VAL                      (int32_t)(INV_INDUC_RMS_OCP1 * 256)//真实电流有效值(A)*COM_REAL_IINDUC_RMS_SCAL
        #define INV_INDUC_RMS_OCP2_VAL                      (int32_t)(INV_INDUC_RMS_OCP2 * 256)   

        #define INV_INDUC_RMS_OCP1_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)
        #define INV_INDUC_RMS_OCP2_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)  
        
     #elif ( InvVol_Type == Out_Vol_220||InvVol_Type == Out_Vol_230 )
        #define INV_INDUC_RMS_OCP1                          (14.5)//(A)
        #define INV_INDUC_RMS_OCP2                          (15.0)//(A)
        #define INV_INDUC_RMS_OCP_VALBACK                   ( 0.4)//保护使用的回差值(A)

        #define INV_INDUC_RMS_OCP1_TIME                     (40000)//延时时间(ms)
        #define INV_INDUC_RMS_OCP2_TIME                     (8000)//延时时间(ms)

        #define INV_INDUC_RMS_OCP1_VAL                      (int32_t)(INV_INDUC_RMS_OCP1 * 256)//真实电流有效值(A)*COM_REAL_IINDUC_RMS_SCAL
        #define INV_INDUC_RMS_OCP2_VAL                      (int32_t)(INV_INDUC_RMS_OCP2 * 256)   

        #define INV_INDUC_RMS_OCP1_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)
        #define INV_INDUC_RMS_OCP2_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)  

     #endif
/*-----------------------------------------------------------------------------*/
#elif ( Inv_Power_Model == Inv_Power_2000W )
    /*------------------------------------------------------------------------------------*/
    //开机检测转速参数设置
    /*------------------------------------------------------------------------------------*/
    #define COM_START_CHECK_RPM_UP                          (( 4100UL * CM_RPM_BASE ) >> R_GQ)//5688     //5000rpm
    #define COM_START_CHECK_RPM_UP_BACK                     (( 4000UL * CM_RPM_BASE ) >> R_GQ)//5800	    //5100rpm
    #define COM_START_CHECK_RPM_UP_TIME			            ( 500)		  //(ms) 

    /*------------------------------------------------------------------------------------*/
    //过速、欠速设置
    /*------------------------------------------------------------------------------------*/
    #define COM_RPM_OVER_TIME                               (2000)       //(ms) 		 
    #define COM_RPM_OVER_VAL                                (( 4000UL * CM_RPM_BASE ) >> R_GQ)
    #define COM_RPM_OVER_VAL_BACK                           (( 3950UL * CM_RPM_BASE ) >> R_GQ)

    #define COM_RPM_LOW_TIME                                (2000)       //(ms)  		 
    #define COM_RPM_LOW_VAL                                 (( 2550UL * CM_RPM_BASE ) >> R_GQ)
    #define COM_RPM_LOW_VAL_BACK                            (( 2600UL * CM_RPM_BASE ) >> R_GQ)	    
      
    /*------------------------------------------------------------------------------------*/
    //有功功率P(W)过载参数设置
    //过载保护：OLP(over load protect)
    /*------------------------------------------------------------------------------------*/
    #define INV_P_OLP1                                      (3150)//(W)
    #define INV_P_OLP2                                      (3170)//(W)
    #define INV_P_OLP3                                      (3210)//(W)
    #define INV_P_OLP4                                      (3210)//(W)
    #define INV_P_OLP_VALBACK                               ( 20)//保护使用的回差值(W)

    #define INV_P_OLP1_TIME                                 (40000)//延时时间(ms)   
    #define INV_P_OLP2_TIME                                 (19000)//延时时间(ms)    
    #define INV_P_OLP3_TIME	                                (9000)//延时时间(ms)     
    #define INV_P_OLP4_TIME	                                (9000)//延时时间(ms)     

    #define INV_P_OLP1_VAL                                  (INV_P_OLP1) //设定功率值(W)     
    #define INV_P_OLP2_VAL                                  (INV_P_OLP2)      
    #define INV_P_OLP3_VAL	                                (INV_P_OLP3)      
    #define INV_P_OLP4_VAL	                                (INV_P_OLP4) 

    #define INV_P_OLP1_VAL_BACK                             (INV_P_OLP1 - INV_P_OLP_VALBACK)
    #define INV_P_OLP2_VAL_BACK                             (INV_P_OLP2 - INV_P_OLP_VALBACK)
    #define INV_P_OLP3_VAL_BACK	                            (INV_P_OLP3 - INV_P_OLP_VALBACK)
    #define INV_P_OLP4_VAL_BACK	                            (INV_P_OLP4 - INV_P_OLP_VALBACK)

    /*------------------------------------------------------------------------------------*/
    //视在功率-S(VA)过载参数设置
    /*------------------------------------------------------------------------------------*/
    #define INV_S_OLP1                                      (3140)//(VA)
    #define INV_S_OLP2                                      (3180)//(VA)
    #define INV_S_OLP3                                      (3200)//(VA)
    #define INV_S_OLP4                                      (3200)//(VA)
    #define INV_S_OLP_VALBACK                               ( 20)//保护使用的回差值(VA)

    #define INV_S_OLP1_TIME                                 (120000)//延时时间(ms)   
    #define INV_S_OLP2_TIME                                 (20000)//延时时间(ms)    
    #define INV_S_OLP3_TIME	                                (2000)//延时时间(ms)     
    #define INV_S_OLP4_TIME	                                (1000)//延时时间(ms)     

    #define INV_S_OLP1_VAL                                  (INV_S_OLP1)    
    #define INV_S_OLP2_VAL                                  (INV_S_OLP2)      
    #define INV_S_OLP3_VAL	                                (INV_S_OLP3)      
    #define INV_S_OLP4_VAL	                                (INV_S_OLP4) 

    #define INV_S_OLP1_VAL_BACK                             (INV_S_OLP1 - INV_S_OLP_VALBACK)
    #define INV_S_OLP2_VAL_BACK                             (INV_S_OLP2 - INV_S_OLP_VALBACK)
    #define INV_S_OLP3_VAL_BACK	                            (INV_S_OLP3 - INV_S_OLP_VALBACK)
    #define INV_S_OLP4_VAL_BACK	                            (INV_S_OLP4 - INV_S_OLP_VALBACK)

    /*------------------------------------------------------------------------------------*/
    //电感过流参数有效值保护设置
    //有效值：RMS
    //过流保护：OCP
    /*------------------------------------------------------------------------------------*/
     #if ( InvVol_Type == Out_Vol_110 ||InvVol_Type == Out_Vol_120) 
        #define INV_INDUC_RMS_OCP1                          (14.5)//(A)
        #define INV_INDUC_RMS_OCP2                          (15.0)//(A)
        #define INV_INDUC_RMS_OCP_VALBACK                   ( 0.4)//保护使用的回差值(A)

        #define INV_INDUC_RMS_OCP1_TIME                     (40000)//延时时间(ms)
        #define INV_INDUC_RMS_OCP2_TIME                     (8000)//延时时间(ms)

        #define INV_INDUC_RMS_OCP1_VAL                      (int32_t)(INV_INDUC_RMS_OCP1 * 256)//真实电流有效值(A)*COM_REAL_IINDUC_RMS_SCAL
        #define INV_INDUC_RMS_OCP2_VAL                      (int32_t)(INV_INDUC_RMS_OCP2 * 256)   

        #define INV_INDUC_RMS_OCP1_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)
        #define INV_INDUC_RMS_OCP2_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)  
        
     #elif ( InvVol_Type == Out_Vol_220||InvVol_Type == Out_Vol_230 )
        #define INV_INDUC_RMS_OCP1                          (14.5)//(A)
        #define INV_INDUC_RMS_OCP2                          (14.5)//(A)
        #define INV_INDUC_RMS_OCP_VALBACK                   ( 0.4)//保护使用的回差值(A)

        #define INV_INDUC_RMS_OCP1_TIME                     (80000)//延时时间(ms)
        #define INV_INDUC_RMS_OCP2_TIME                     (8000)//延时时间(ms)
        #define INV_INDUC_RMS_OCP1_VAL                      (int32_t)(INV_INDUC_RMS_OCP1 * 256)//真实电流有效值(A)*COM_REAL_IINDUC_RMS_SCAL
        #define INV_INDUC_RMS_OCP2_VAL                      (int32_t)(INV_INDUC_RMS_OCP2 * 256)   

        #define INV_INDUC_RMS_OCP1_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)
        #define INV_INDUC_RMS_OCP2_VAL_BACK                 (int32_t)((INV_INDUC_RMS_OCP1 - INV_INDUC_RMS_OCP_VALBACK) * 256)  

     #endif
/*-----------------------------------------------------------------------------*/
#endif		


  
/***************************************************************************/
/***************************************************************************/

#if ( Inv_Power_Model == Inv_Power_1000W )
   #if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011)
   #else
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011)
   #endif
#elif ( Inv_Power_Model == Inv_Power_1600W )
   #if ( (InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011)
   #else
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011) 
   #endif 
#elif ( Inv_Power_Model == Inv_Power_2000W )
   #if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011)
   #else
        #define 	 CM_ACVol_Factor                        (265)
        #define 	 CM_LiCur_Factor                        (7779)
        #define 	 CM_LdCur_Factor                        (7779)
        #define 	 CM_P_Factor                            (1011)
        #define 	 CM_Q_Factor                            (1011)
   #endif 
#endif 	 
  
/***************************************************************************/
/***************************************************************************/

#if ( Inv_Power_Model == Inv_Power_1000W )
   #if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)				
	 #else
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)			
	 #endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#elif ( Inv_Power_Model == Inv_Power_1600W )
    #if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)		
	 #else
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)			
	 #endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#elif ( Inv_Power_Model == Inv_Power_2000W )		 
   #if ((InvVol_Type == Out_Vol_110 )|| (InvVol_Type == Out_Vol_120 ))
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)
      #else
        //--------------------------------------------------------------------
        /*系统标幺值*/
        //--------------------------------------------------------------------
        #define CM_InvVol_ScaleToPU                         (4236)
        #define CM_InvCur_ScaleToPU                         (2604)
        #define CM_LoadCur_ScaleToPU                        (756)
        #define CM_BusVol_ScaleToPU                         (5256)
        #define CM_DCIM_Vol_ScaleToPU                       (3000)
        #define CM_AD_Ref_ScaleToPU                         (1973)
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_Cur_KpToKi_Factor                  (1064)
        #define AlgCurve_Cur_Kp_Factor_B                    (8000)
        #define AlgCurve_Cur_Kp_Factor_K                    (4533)
        #define AlgCurve_Cur_Kp_Value_Down_Limit            (1500)	
        //--------------------------------------------------------------------
        //--------------------------------------------------------------------
        #define AlgCurve_BusToInVolAM_Factor_B                  (-8294)
        #define AlgCurve_BusToInVolAM_Factor_K                  (3840)
        #define AlgCurve_BusToInVolAM_Factor_Value_Up_Limit     (1024)
        #define AlgCurve_BusToInVolAM_Factor_Value_Down_Limit   (200)
        //--------------------------------------------------------------------
        /*重复控制根据负载电流大小变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_IrmsToRepCtrl_Factor_B             (170)
        #define AlgCurve_IrmsToRepCtrl_Factor_K             (2523)
        #define AlgCurve_IrmsToRepCtrl_Value_Up_Limit       (450)            
        #define AlgCurve_IrmsToRepCtrl_Value_Down_Limit     (180)
        //--------------------------------------------------------------------
        /*虚拟阻抗变系数*/
        //--------------------------------------------------------------------
        #define AlgCurve_BusToVirRes_Factor_B               (22660)
        #define AlgCurve_BusToVirRes_Factor_K               (-4387)
        #define AlgCurve_IBusToVirRes_Value_Up_Limit        (4096)            
        #define AlgCurve_BusToVirRes_Value_Down_Limit       (1024)
	 #endif	 

#endif
/***************************************************************************/


#endif
