/*****************************************************************************
@Description: InvVol_AM.h 逆变器电压控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：目前版本中没有模块直接使用 CM_Vpeak_AM 或 CM_VPeak_Bus_AM（全工程搜索不到引用）,
             说明幅值基准仍然由 sys_define_config.h 里的 INV_AC_VOL_AMP_VAL_* 等常量主导;
             InvVol_AM.h 这部分更多扮演“配置模板/占位符”角色,供未来机型拓展。
*****************************************************************************/

#ifndef __INVVOL_AM_H_
#define __INVVOL_AM_H_

#include "sys_define_config.h"


#define CM_Vpeak_1KW_110Vac_50Hz_AM                             (2740)
#define CM_Vpeak_1KW_110Vac_60Hz_AM                             (2740)
#define CM_Vpeak_1KW_120Vac_50Hz_AM                             (2740)
#define CM_Vpeak_1KW_120Vac_60Hz_AM                             (2740)

#define CM_Vpeak_1KW_220Vac_50Hz_AM                             (2740)
#define CM_Vpeak_1KW_220Vac_60Hz_AM                             (2740)
#define CM_Vpeak_1KW_230Vac_50Hz_AM                             (2740)
#define CM_Vpeak_1KW_230Vac_60Hz_AM                             (2740)



#define CM_Vpeak_2KW_110Vac_50Hz_AM                             (2740) 
#define CM_Vpeak_2KW_110Vac_60Hz_AM                             (2740)
#define CM_Vpeak_2KW_120Vac_50Hz_AM                             (2740)
#define CM_Vpeak_2KW_120Vac_60Hz_AM                             (2740)

#define CM_Vpeak_2KW_220Vac_50Hz_AM                             (2740)
#define CM_Vpeak_2KW_220Vac_60Hz_AM                             (2740)
#define CM_Vpeak_2KW_230Vac_50Hz_AM                             (2740)// 230Vac、50Hz 条件下，2kW 系统的峰值电压，数值为 2740
#define CM_Vpeak_2KW_230Vac_60Hz_AM                             (2740)

  
//-----------------------------------------------------------------------------
#if   Inv_Power_Model == Inv_Power_1000W
    //-----------------------------------------------------------------------------
    #if ( InvVol_Type == Out_Vol_110 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_110Vac_50Hz_AM//_IQ(0.68)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_110Vac_60Hz_AM//_IQ(0.68)
        #endif
        #define CM_VPeak_Bus_AM                                 (23500)
    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_120 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_120Vac_50Hz_AM//_IQ(0.74)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_120Vac_60Hz_AM
        #endif
        #define CM_VPeak_Bus_AM                                 (2450)//180

    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_220 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_220Vac_50Hz_AM//_IQ(0.675)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_220Vac_60Hz_AM//_IQ(0.67)
        #endif
        #define CM_VPeak_Bus_AM                                 (23500)//24200//360/530
    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_230 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_230Vac_50Hz_AM//_IQ(0.702)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_1KW_230Vac_60Hz_AM//_IQ(0.7)
        #endif
        #define CM_VPeak_Bus_AM                                 (24000)//23600//24200//370/530
    #endif

#elif ( Inv_Power_Model  == Inv_Power_1600W ) || ( Inv_Power_Model  == Inv_Power_2000W )
    //-----------------------------------------------------------------------------
    #if( InvVol_Type == Out_Vol_110 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_110Vac_50Hz_AM//_IQ(0.68)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_110Vac_60Hz_AM//_IQ(0.68)
        #endif
        #define CM_VPeak_Bus_AM                                 (23500)//185/263
    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_120 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_120Vac_50Hz_AM//_IQ(0.74)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_120Vac_60Hz_AM
        #endif
        #define CM_VPeak_Bus_AM                                 (2450)//180
    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_220 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_220Vac_50Hz_AM//_IQ(0.675)
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_220Vac_60Hz_AM//_IQ(0.67)
        #endif
        #define CM_VPeak_Bus_AM                                 (23500)//24200//360/530
    //-----------------------------------------------------------------------------
    #elif ( InvVol_Type == Out_Vol_230 )
        #if InvVol_Freq  == InvVol_Freq_50Hz
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_230Vac_50Hz_AM//_IQ(0.702) 
        #else
            #define CM_Vpeak_AM                                 CM_Vpeak_2KW_230Vac_60Hz_AM//_IQ(0.7)
        #endif
        #define CM_VPeak_Bus_AM                                 (24000)
    #endif
#endif

/*------------------------------------------------------------------------------------*/
#if   ( Inv_Power_Model == Inv_Power_1000W )
    #if ((InvVol_Type == Out_Vol_110 )||(InvVol_Type == Out_Vol_120 ))
        #define CM_InvCurr_Up_Limit                             (2000)
        #define CM_InvCurr_Down_Limit                           (-2000)
    #else
        #define CM_InvCurr_Up_Limit                             (2)
        #define CM_InvCurr_Down_Limit                           (-1250)
    #endif
/*------------------------------------------------------------------------------------*/
#elif ( Inv_Power_Model == Inv_Power_1600W ) || ( Inv_Power_Model  == Inv_Power_2000W )
    #if ((InvVol_Type == Out_Vol_110 )||(InvVol_Type == Out_Vol_120 ))
        #define CM_InvCurr_Up_Limit                             (3500)
        #define CM_InvCurr_Down_Limit                           (-3500)
    #else
        #define CM_InvCurr_Up_Limit                             (3400)
        #define CM_InvCurr_Down_Limit                           (-3400)
    #endif
#endif
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
#if ((InvVol_Type == Out_Vol_110 )||(InvVol_Type == Out_Vol_120 ))
    #define    INVCURR_DECOUPLING_COEFF                         (1200)
#else
    #define    INVCURR_DECOUPLING_COEFF                         (1200)
#endif


/*------------------------------------------------------------------------------------*/
#endif
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/
