/*****************************************************************************
@Description: user_stepmotor.c 步进电机控制
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.10.28  naiva  V2.0.1 创建
@Attention  ：
  - 本文件实现发动机节气门步进电机的开环/闭环驱动与转速给定逻辑。
  - 核心包含三部分：
    1) 功率→转速查表（ECO/全速两套曲线，按机型与输出电压切换）；
    2) 步进序列输出（四相八拍 / 两相四拍，方向可选）；
    3) 采样转速滤波与 PI 调节，得到步进电机的步数与方向。
  - 统一注释风格：模块级（横线块）、函数级（Description/IO/Notes）、语句/变量级（行尾或行前简注）。
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "user_stepmotor.h"
#include "sys_define_struct.h"
#include "user_function.h"
#include "AlgorithmLevel_Headers.h"
/***************************************************************************/

/*-------------------------------------------------------------------------------------------------*/
/* 功率-转速分段查表（按机型/电压档选择不同的功率点与对应目标转速）                                  */
/* 说明：                                                                                           */
/*  1) x 轴为功率（单位等效于“W×缩放系数 CM_Pcv_BASE 后右移 P_GQ”）；                               */
/*  2) y 轴为目标转速（单位等效于“rpm×缩放系数 CM_RPM_BASE 后右移 R_GQ”）；                         */
/*  3) ECO：节能模式（低负载/声噪优先）；Full：全速模式（响应优先或锁速需求）。                    */
/*  4) Protect_Idle_Rpm：保护怠速（启动/故障复位等保底转速）。                                      */
/*-------------------------------------------------------------------------------------------------*/
#if Inv_Power_Model  == Inv_Power_1000W
    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式（x 轴：功率刻度，低→高）
       {          0,               ((233L*CM_Pcv_BASE)>>P_GQ),((393L*CM_Pcv_BASE)>>P_GQ),((697L*CM_Pcv_BASE)>>P_GQ),((893L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式（y 轴：目标转速刻度，怠速→高转）
       {Protect_Idle_Rpm,          ((4130L*CM_RPM_BASE)>>R_GQ),((4218L*CM_RPM_BASE)>>R_GQ),((4750L*CM_RPM_BASE)>>R_GQ ),((5312L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ) };

       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式（x 轴）
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((60L*CM_Pcv_BASE)>>P_GQ),((700L*CM_Pcv_BASE)>>P_GQ),((850L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式（y 轴：更倾向锁定较高转速）
       {(( 5600L*CM_RPM_BASE )>>R_GQ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ), ((5600L*CM_RPM_BASE)>>R_GQ) };
    /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    #else
		   int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
       {          0,               ((233L*CM_Pcv_BASE)>>P_GQ),((393L*CM_Pcv_BASE)>>P_GQ),((697L*CM_Pcv_BASE)>>P_GQ),((893L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((4130L*CM_RPM_BASE)>>R_GQ),((4218L*CM_RPM_BASE)>>R_GQ),((4750L*CM_RPM_BASE)>>R_GQ ),((5312L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ) };

       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((60L*CM_Pcv_BASE)>>P_GQ),((700L*CM_Pcv_BASE)>>P_GQ),((850L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 5600L*CM_RPM_BASE )>>R_GQ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ), ((5600L*CM_RPM_BASE)>>R_GQ) };
	  #endif
 /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
 /*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#elif Inv_Power_Model  == Inv_Power_1600W
    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式（负载区间更高）
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((950L*CM_Pcv_BASE)>>P_GQ),((1250L*CM_Pcv_BASE)>>P_GQ),((1400L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式（随功率平滑上升）
       {Protect_Idle_Rpm,          ((3900L*CM_RPM_BASE)>>R_GQ),((4000L*CM_RPM_BASE)>>R_GQ),((4200L*CM_RPM_BASE)>>R_GQ ),((4400L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ) };
   /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式（高负载段密集）
       {          0,               ((1400L*CM_Pcv_BASE)>>P_GQ),((1500L*CM_Pcv_BASE)>>P_GQ),((1550L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式（锁速 4.25~4.75k 区间）
       {(( 4250L*CM_RPM_BASE )>>R_GQ),((4550L*CM_RPM_BASE)>>R_GQ ),((4600L*CM_RPM_BASE)>>R_GQ ),((4650L*CM_RPM_BASE)>>R_GQ ),((4700L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ) };
       /*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    #else
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
       {          0,               ((320L*CM_Pcv_BASE)>>P_GQ),((700L*CM_Pcv_BASE)>>P_GQ),((1050L*CM_Pcv_BASE)>>P_GQ),((1350L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((3750L*CM_RPM_BASE)>>R_GQ),((3950L*CM_RPM_BASE)>>R_GQ),((4200L*CM_RPM_BASE)>>R_GQ ),((4520L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ) };
       /*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((1400L*CM_Pcv_BASE)>>P_GQ),((1450L*CM_Pcv_BASE)>>P_GQ),((1500L*CM_Pcv_BASE)>>P_GQ),((1550L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 4750L*CM_RPM_BASE )>>R_GQ),((4750L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ ),((4750L*CM_RPM_BASE)>>R_GQ) };
		#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#elif Inv_Power_Model  == Inv_Power_2000W
//    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
//      int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
//       {          0,               ((400L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((1800L*CM_Pcv_BASE)>>P_GQ),((2000L*CM_Pcv_BASE)>>P_GQ)};
//
//       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
//       {Protect_Idle_Rpm,          ((4050L*CM_RPM_BASE)>>R_GQ),((4500L*CM_RPM_BASE)>>R_GQ),((5000L*CM_RPM_BASE)>>R_GQ ),((5350L*CM_RPM_BASE)>>R_GQ),((5600L*CM_RPM_BASE)>>R_GQ) };
//       /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//      int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
//       {          0,               ((300L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((1800L*CM_Pcv_BASE)>>P_GQ),((1900L*CM_Pcv_BASE)>>P_GQ),((2000L*CM_Pcv_BASE)>>P_GQ)};
//
//       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
//       {(( 4800L*CM_RPM_BASE )>>R_GQ),((4850L*CM_RPM_BASE)>>R_GQ),((5000L*CM_RPM_BASE)>>R_GQ ),((5350L*CM_RPM_BASE)>>R_GQ ),((5430L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ) };
//    #else         //2KW为基值   4096/2000 = 2.048
//    /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//      int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
//       {          0,               ((400L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((1800L*CM_Pcv_BASE)>>P_GQ),((2000L*CM_Pcv_BASE)>>P_GQ)};
//
//       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
//       {Protect_Idle_Rpm,          ((4050L*CM_RPM_BASE)>>R_GQ),((4500L*CM_RPM_BASE)>>R_GQ),((5000L*CM_RPM_BASE)>>R_GQ ),((5350L*CM_RPM_BASE)>>R_GQ),((5600L*CM_RPM_BASE)>>R_GQ) };
//       /*-------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//      int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
//       {          0,               ((300L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((1800L*CM_Pcv_BASE)>>P_GQ),((1900L*CM_Pcv_BASE)>>P_GQ),((2000L*CM_Pcv_BASE)>>P_GQ)};
//
//       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
//       {(( 4800L*CM_RPM_BASE )>>R_GQ),((4850L*CM_RPM_BASE)>>R_GQ),((5000L*CM_RPM_BASE)>>R_GQ ),((5350L*CM_RPM_BASE)>>R_GQ ),((5430L*CM_RPM_BASE)>>R_GQ ),((5600L*CM_RPM_BASE)>>R_GQ) };
//    #endif
       
    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
    //    int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
    //    {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((2400L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};
    //
    //    int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
    //    {Protect_Idle_Rpm,          ((3050L*CM_RPM_BASE)>>R_GQ),((3200L*CM_RPM_BASE)>>R_GQ ),((3350L*CM_RPM_BASE)>>R_GQ ),((3500L*CM_RPM_BASE)>>R_GQ),((3750L*CM_RPM_BASE)>>R_GQ) };
    //  /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    //    int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
    //    {          0,               ((2100L*CM_Pcv_BASE)>>P_GQ),((2400L*CM_Pcv_BASE)>>P_GQ),((2550L*CM_Pcv_BASE)>>P_GQ),((2750L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};
    //
    //    int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
    //    {(( 3450L*CM_RPM_BASE )>>R_GQ),((3550L*CM_RPM_BASE )>>R_GQ),((3600L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ)};
    /*--------------------------------------------20251024 逆变器实际功率调整到3KW-注释以上代码，新增以下代码------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = // 功率_节能模式（覆盖到 3kW）
           {0, ((500L * CM_Pcv_BASE) >> P_GQ), ((1000L * CM_Pcv_BASE) >> P_GQ), ((1600L * CM_Pcv_BASE) >> P_GQ), ((2400L * CM_Pcv_BASE) >> P_GQ), ((3000L * CM_Pcv_BASE) >> P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] = // 转速_节能模式（对应较低锁速带）
           {Protect_Idle_Rpm, ((3000L * CM_RPM_BASE) >> R_GQ), ((3200L * CM_RPM_BASE) >> R_GQ), ((3350L * CM_RPM_BASE) >> R_GQ), ((3600L * CM_RPM_BASE) >> R_GQ), ((3750L * CM_RPM_BASE) >> R_GQ)};
       /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = // 功率_全速模式（3kW 段）
           {0, ((2100L * CM_Pcv_BASE) >> P_GQ), ((2400L * CM_Pcv_BASE) >> P_GQ), ((2550L * CM_Pcv_BASE) >> P_GQ), ((2750L * CM_Pcv_BASE) >> P_GQ), ((3000L * CM_Pcv_BASE) >> P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] = // 转速_全速模式（逐级小幅抬升）
           {((3450L * CM_RPM_BASE) >> R_GQ), ((3550L * CM_RPM_BASE) >> R_GQ), ((3600L * CM_RPM_BASE) >> R_GQ), ((3650L * CM_RPM_BASE) >> R_GQ), ((3700L * CM_RPM_BASE) >> R_GQ), ((3750L * CM_RPM_BASE) >> R_GQ)};

#else
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式（另一个电压档）
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1900L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((2950L*CM_RPM_BASE)>>R_GQ),((3050L*CM_RPM_BASE)>>R_GQ ),((3350L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ) };
       /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((2000L*CM_Pcv_BASE)>>P_GQ),((2300L*CM_Pcv_BASE)>>P_GQ),((2500L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 3450L*CM_RPM_BASE )>>R_GQ),((3500L*CM_RPM_BASE )>>R_GQ),((3550L*CM_RPM_BASE)>>R_GQ ),((3600L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ),((3750L*CM_RPM_BASE)>>R_GQ)};
#endif       
       
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#elif Inv_Power_Model == Inv_Power_3000W
    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1600L*CM_Pcv_BASE)>>P_GQ),((2400L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((3000L*CM_RPM_BASE)>>R_GQ),((3200L*CM_RPM_BASE)>>R_GQ ),((3350L*CM_RPM_BASE)>>R_GQ ),((3600L*CM_RPM_BASE)>>R_GQ),((3750L*CM_RPM_BASE)>>R_GQ) };
     /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((2100L*CM_Pcv_BASE)>>P_GQ),((2400L*CM_Pcv_BASE)>>P_GQ),((2550L*CM_Pcv_BASE)>>P_GQ),((2750L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 3450L*CM_RPM_BASE )>>R_GQ),((3550L*CM_RPM_BASE )>>R_GQ),((3600L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3700L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ)};
    #else
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
       {          0,               ((500L*CM_Pcv_BASE)>>P_GQ),((1000L*CM_Pcv_BASE)>>P_GQ),((1900L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((2950L*CM_RPM_BASE)>>R_GQ),((3050L*CM_RPM_BASE)>>R_GQ ),((3350L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ) };
       /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((2000L*CM_Pcv_BASE)>>P_GQ),((2300L*CM_Pcv_BASE)>>P_GQ),((2500L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 3450L*CM_RPM_BASE )>>R_GQ),((3500L*CM_RPM_BASE )>>R_GQ),((3550L*CM_RPM_BASE)>>R_GQ ),((3600L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ)};
    #endif
 /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
 /*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#elif Inv_Power_Model == Inv_Power_3600W
    #if ((InvVol_Type == Out_Vol_220 )||(InvVol_Type == Out_Vol_230 )||(InvVol_Type == Out_Vol_240 ))
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式（覆盖至 3.6kW）
       {          0,               ((800L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3200L*CM_Pcv_BASE)>>P_GQ),((3400L*CM_Pcv_BASE)>>P_GQ),((3600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((2950L*CM_RPM_BASE)>>R_GQ),((3450L*CM_RPM_BASE)>>R_GQ ),((3550L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ) };
       /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式（3.6kW 尾段）
       {          0,               ((2500L*CM_Pcv_BASE)>>P_GQ),((2800L*CM_Pcv_BASE)>>P_GQ),((3200L*CM_Pcv_BASE)>>P_GQ),((3400L*CM_Pcv_BASE)>>P_GQ),((3600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
           {(( 3250L*CM_RPM_BASE )>>R_GQ),((3350L*CM_RPM_BASE )>>R_GQ),((3450L*CM_RPM_BASE)>>R_GQ ),((3550L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ)};
    #else
       int g_Engine_Power_P_x_ECO[Len_Power_Rpm] = //功率_节能模式
       {          0,               ((800L*CM_Pcv_BASE)>>P_GQ),((2200L*CM_Pcv_BASE)>>P_GQ),((3000L*CM_Pcv_BASE)>>P_GQ),((3400L*CM_Pcv_BASE)>>P_GQ),((3600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_ECO[Len_Power_Rpm] =//转速_节能模式
       {Protect_Idle_Rpm,          ((2950L*CM_RPM_BASE)>>R_GQ),((3250L*CM_RPM_BASE)>>R_GQ ),((3350L*CM_RPM_BASE)>>R_GQ ),((3550L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ) };
       /*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

       int g_Engine_Power_P_x_Full[Len_Power_Rpm] = //功率_全速模式
       {          0,               ((3000L*CM_Pcv_BASE)>>P_GQ),((3300L*CM_Pcv_BASE)>>P_GQ),((3400L*CM_Pcv_BASE)>>P_GQ),((3500L*CM_Pcv_BASE)>>P_GQ),((3600L*CM_Pcv_BASE)>>P_GQ)};

       int g_Engine_Power_Rpm_y_Full[Len_Power_Rpm] =//转速_全速模式
       {(( 3250L*CM_RPM_BASE )>>R_GQ),((3350L*CM_RPM_BASE )>>R_GQ),((3450L*CM_RPM_BASE)>>R_GQ ),((3550L*CM_RPM_BASE)>>R_GQ ),((3650L*CM_RPM_BASE)>>R_GQ ),((3750L*CM_RPM_BASE)>>R_GQ)};
    #endif
#endif


/*------------------------------------------------------------------------------------*/
/* 步进电机端口映射缓存：根据编译期方向配置选择具体的硬件寄存器地址                     */
/* 提示：此处仅缓存 PORT->P2 的地址，便于快速写端口位（降低开销）。                     */
/*------------------------------------------------------------------------------------*/
#if  Motor_Step_Driection == Forward_Driver_Motor				 
   volatile uint8_t* Step_Driver_Hardware_Buff[1]={&(PORT->P2)};
#endif
/*------------------------------------------------------------------------------------*/
/* 步进序列表：四相八拍/两相四拍                                                       */
/*  - 四相八拍表按 A→DA→D→CD→C→BC→B→AB→A（反向）或其相反次序（正向）输出；           */
/*  - 每个元素是端口位图（例如：0x20/0xA0...），与硬件相连引脚保持一致。              */
/*------------------------------------------------------------------------------------*/
#if CM_Step_Driver_Phase == CM_Step_Four_Phase	 
    //unsigned short int Step_Driver_Signal_Buff[9] ={0x20,0x30,0x10,0x50,0x40,0xC0,0x80,0xA0,0x0000};//正向：A-AB-B-BC-C-CD-D-DA-A  四相八拍
    unsigned short int Step_Driver_Signal_Buff[9] ={0x20,0xA0,0x80,0xC0,0x40,0x50,0x10,0x30,0x0000};//反向：A-DA-D-CD-C-BC-B-AB-A  四相八拍
#else
    unsigned  short int Step_Driver_Signal_Buff[9] ={0x30,0x60,0xC0,0x90,0x30,0x60,0xC0,0x90,0x00};//AB-BC-CD-DA  2相4拍
#endif	
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/

/*************************************************
Description: Stepmotordrive
Input      : 无
Return     : 无
Others     : 步进电机执行逻辑
  - 每次调用推进一步（按当前步索引与方向从序列表取码并写端口）。
  - stepIndex 0..7 循环；stepDirection <0 为“正向”（与注释对应），≥0 为“反向”。
  - 注意：这里“正/反”的语义与硬件接线/表项顺序有关，保持与项目对齐即可。
*************************************************/
void Stepmotordrive(void)
{
    short int Stepmotor_index_T;
    StepMotor_Ctrl.stepIndex++;                       /** 步索引自增（0..7） */
    if ( StepMotor_Ctrl.stepIndex >= 8 ) StepMotor_Ctrl.stepIndex = 0;

    if   (StepMotor_Ctrl.stepDirection < 0)  Stepmotor_index_T = StepMotor_Ctrl.stepIndex;   // 步进电机正向
    else                                       Stepmotor_index_T = 7 - StepMotor_Ctrl.stepIndex;// 步进电机反向
//    Stepmotor_index_T = 6;

    *Step_Driver_Hardware_Buff[0] = (PORT->P2 & 0x0f)| Step_Driver_Signal_Buff[Stepmotor_index_T];//PORTC端口先与上0xfc3f表示把RC6，7，8，9端口清零，再给其端口PORTC端口寄存器控制值
    /** 上式含义：保留低 4 位，清步进相关高位，再 OR 上当前步所需的相位位图。 */
}


/*************************************************
Description: Lookup_Table_Calc1
Input      : Lookup_Table_Handle1 —— 传入查表对象（含 x 输入 / x、y 数组 / 表长）
Return     : 无（结果写回 y_Axis_Out）
Others     : 根据功率值查表生成参考转速（线性插值，Q10 近似）
  - 遍历定位区间 [i-1, i]，以左闭右开方式计算线性插值。
  - 注意：ref_k = (RPM_Diff << 10/ Power_Diff) 的运算符优先级可能不符合预期；
          “<<” 的优先级低于 “/”，当前表达式等价于 (RPM_Diff << (10/Power_Diff))。
          若期望 (RPM_Diff << 10) / Power_Diff，应增加括号，但本实现不改动源逻辑，仅提示。
*************************************************/
volatile uint8_t flag_count_PIOut,flag_count_PIOut_temp,flag_count_PIOut_temp4,flag_count_PIOut_temp5 = 0;
volatile uint8_t Start_Flag = 1;                 /** 启动阶段标志（节能曲线预热阶段） */
uint8_t Active_Power_Total_Count = 0;            /** 负载突变计数，用于快关逻辑 */
int16_t Active_Power_Total_Old,Start_Count,Start_Count_Full = 0;
int16_t  s16_GeneratorSpeed_Max,s16_GeneratorSpeed_Min=0;
int16_t		s16_GeneratorSpeedAvg = 0;            /** 转速平均（去极值后平均） */
int32_t		s32_GeneratorSpeedSum = 0;            /** 采样累加和（便于计算均值） */
extern int16_t  s16_GeneratorfreVal_Array[10];
extern int8_t   SpeedSampleCount;
UNLONG_u  GeneratorfreFilter;

void Lookup_Table_Calc1(Lookup_Table *Lookup_Table_Handle1)
{
	int32_t ref_k,ref_b = 0;                       /** 线性插值斜率/截距（Q10 架构） */
	int32_t Power_Diff,RPM_Diff = 0;
	int32_t TableLength = Lookup_Table_Handle1->Table_Length;
	int32_t Power_In = Lookup_Table_Handle1->x_Axis_In;
	
	for(int i = 1;i < TableLength;i++) 
	{
		if( Power_In <= Lookup_Table_Handle1->x_Axis_Ptr[i])
		{
			Power_Diff  = Lookup_Table_Handle1->x_Axis_Ptr[i] - Lookup_Table_Handle1->x_Axis_Ptr[i - 1];
			RPM_Diff    = Lookup_Table_Handle1->y_Axis_Ptr[i] - Lookup_Table_Handle1->y_Axis_Ptr[i - 1];
            ref_k = (RPM_Diff << 10/ Power_Diff);                                 /** 注意：运算符优先级，见函数注释 */
            ref_b = (((uint32_t)Lookup_Table_Handle1->y_Axis_Ptr[i])<<10) - ref_k * Lookup_Table_Handle1->x_Axis_Ptr[i];
            
			Lookup_Table_Handle1->y_Axis_Out = (ref_k * Power_In +  ref_b)>>10;   /** y = kx + b（回写输出） */
            return ;
		}	
	}      
}

/*************************************************
Description: StepmotormoveCheck
Input      : 无
Return     : 无
Others     : 步进电机相关数据处理及算法调用
  主流程（1ms 节拍与电机调度周期配合）：
  1) 启动阶段延时与 ECO 预热（Start_Flag/Start_Count）；
  2) 按 ECO/Full 选择功率→转速查表，得到 StepMotor_Ctrl.ref；
  3) 采样转速数组，去极值后求平均作为反馈 fdb；
  4) 设置 PI 参数并执行 StepMotor_Ctrl.PI_Calc()，得到 out；
  5) 折算为 stepNum 与 stepDirection，并执行一系列限幅/快关/抖动抑制；
  6) 最终由调度器调用 Stepmotordrive() 输出相位序列。
*************************************************/
int stepmotor_temp =0;
uint8_t  stepmotor_speed_const,stepmotor_speed_constcnt = 0;
void  StepmotormoveCheck(void)
{   
    if (SysClockBase_ms.stepMotor_1ms == 1)       /** 1ms 周期：更新 ref（查表） */
    {   // 先强制 ECO，直到转速 ≥3100（约 700 ms）再放开
        if(Start_Flag == 1 && StepMotor_Ctrl.fdb < 3100)   //启机为全速模式启动时，先在节能模式稳定700ms后再切换到全速模式
            Start_Count ++;
        if(Start_Count > 700)
            Start_Flag = 0;

        //Controllogic.ecomodeidbit = 0; 
        //根据功率查表得到对应的参考转速值
        if (Controllogic.ecomodeidbit == 1 || Start_Flag ==1)							//ECO MODE
        {
            Lookup_Table_Speed.x_Axis_In  = (((uint32_t)System_Power_Info.Apparent_Power_Total * CM_Pcv_BASE)>>P_GQ) ;
            Lookup_Table_Speed.x_Axis_Ptr = &g_Engine_Power_P_x_ECO[0];
            Lookup_Table_Speed.y_Axis_Ptr = &g_Engine_Power_Rpm_y_ECO[0];
            //Lookup_Table_Speed.Calc(&Lookup_Table_Speed);     
            Lookup_Table_Calc1(&Lookup_Table_Speed); 
        }        
        else
        {
            Lookup_Table_Speed.x_Axis_In  = (((uint32_t)System_Power_Info.Apparent_Power_Total * CM_Pcv_BASE)>>P_GQ);
            Lookup_Table_Speed.x_Axis_Ptr = &g_Engine_Power_P_x_Full[0];
            Lookup_Table_Speed.y_Axis_Ptr = &g_Engine_Power_Rpm_y_Full[0];
            //Lookup_Table_Speed.Calc(&Lookup_Table_Speed);
            Lookup_Table_Calc1(&Lookup_Table_Speed);
        }
        /*----------------------PID转速参考值--------------------------------------*/
        StepMotor_Ctrl.ref = Lookup_Table_Speed.y_Axis_Out + 0;// 查表得到的转速赋给参考转速（+0 占位，保留修改钩子）
        
        if( System_Power_Info.Active_Power_Total >= 1400)
            StepMotor_Ctrl.ref += 90;      /** 大功率段目标转速微抬（改善响应） */

        SysClockBase_ms.stepMotor_1ms = 0; /** 清 1ms 标记 */
    }

    /*----------------------转速参考值最大限制----------------------------------*/
    if ( StepMotor_Ctrl.ref > CM_Engine_Speed_RefMAX ) 
        StepMotor_Ctrl.ref = CM_Engine_Speed_RefMAX;   //给定速度不能超过设定值       

    /*----------------------步进电机控制执行------------------------------------*/
    if (Stpmotorcon.motorconbit == 1 )//步进电机控制周期标志，处理步进电机逻辑
    {
        int i = 0;
        Stpmotorcon.motorconbit = 0;          

        /*-------------------采样转速值数据处理---------------------------------*/
        s16_GeneratorSpeed_Max = s16_GeneratorfreVal_Array[0];
        s16_GeneratorSpeed_Min = s16_GeneratorfreVal_Array[0];
        for(i=0;i < SpeedSampleCount;i++)//筛选最小最大值
        {
            s32_GeneratorSpeedSum    += s16_GeneratorfreVal_Array[i];
            if(s16_GeneratorfreVal_Array[i] > s16_GeneratorSpeed_Max)    s16_GeneratorSpeed_Max = s16_GeneratorfreVal_Array[i];
            if(s16_GeneratorfreVal_Array[i] < s16_GeneratorSpeed_Min)    s16_GeneratorSpeed_Min = s16_GeneratorfreVal_Array[i];
        }
        s16_GeneratorSpeedAvg = (s32_GeneratorSpeedSum - s16_GeneratorSpeed_Max - s16_GeneratorSpeed_Min) >> 3;//去掉最低最大值后求平均值

        /*----------------------数据变量清零------------------------------------*/            
        s32_GeneratorSpeedSum = 0;
        s16_GeneratorSpeed_Max = 0;
        s16_GeneratorSpeed_Min = 0;
       
         if(s16_GeneratorSpeedAvg < 0)//转速小于零直接赋值上个转速值
            s16_GeneratorSpeedAvg = StepMotor_Ctrl.fdb;
        
        /*----------------------PID转速反馈值--------------------------------------*/ 
        StepMotor_Ctrl.fdb = s16_GeneratorSpeedAvg;//((uint32_t)Step_Motor.Eng_Speed_fdb * 1593  + (uint32_t)s16_GeneratorSpeedAvg_test * 2503 ) >> 12;//一阶低通滤波           
//        StepMotor_Ctrl.fdb = ((uint32_t)StepMotor_Ctrl.fdb * 2593  + (uint32_t)s16_GeneratorSpeedAvg * 1503 ) >> 12;//一阶低通滤波           
        
        /*-------------------步进电机PID控制参数-----------------------------------*/
        StepMotor_Ctrl.kp = 280;
        StepMotor_Ctrl.ki = 55;
        StepMotor_Ctrl.periodValue = 130;  /** PI 调度周期（内部分频值），与 PI_Calc 内部实现关联 */
            
        /*----------------------步进电机PID执行------------------------------------*/     
        StepMotor_Ctrl.PI_Calc( &StepMotor_Ctrl );

        /*----------------------获取步进电机执行步数及方向-------------------------*/
        if(StepMotor_Ctrl.out >= 0)
            StepMotor_Ctrl.stepNum =   ((int32_t)StepMotor_Ctrl.out * 110) >> 10; /** 输出放大与 Q 进制换算 */
        else 
            StepMotor_Ctrl.stepNum = - ((int32_t)StepMotor_Ctrl.out *(-110)) >> 10;

        UPDNLMTFUN(StepMotor_Ctrl.stepNum,StepMotor_Ctrl.outMax,StepMotor_Ctrl.outMin);    //-100  100之间（统一限幅）

        /*------------------------------------------------------------------------*/
        if(StepMotor_Ctrl.err > 250)   //转速偏差大于250
            flag_count_PIOut ++;
        else                                
        {
            flag_count_PIOut_temp = 0;
            flag_count_PIOut = 0;
        }
         
       if(flag_count_PIOut >= 8 && StepMotor_Ctrl.stepNum > 0) //连续输出正向十次后，强制正向输出为一步，防止一直开油门顶死
       {
            StepMotor_Ctrl.stepNum = 2;
            flag_count_PIOut_temp ++;
       }
        
        
//        if(flag_count_PIOut_temp >= 2  )  
//        {
//            StepMotor_Ctrl.stepNum = 1;
//            flag_count_PIOut_temp = 0;
//        }
  
        
//        if(System_Power_Info.Active_Power_Total < 3400)    
        {            
            if(StepMotor_Ctrl.err <= 300 && StepMotor_Ctrl.err >= 0 && StepMotor_Ctrl.stepNum  > 0)  //误差在+350以内时，正向输出步数强制为一步
            {
                flag_count_PIOut_temp4 ++;
                StepMotor_Ctrl.stepNum = 1;
            }
            else  flag_count_PIOut_temp4=0;


//            if(flag_count_PIOut_temp4 >= 1)    //误差在350以内时 ，输出步数强制为一步 ，且3个PID周期执行一步
//            {
//                StepMotor_Ctrl.stepNum = 1;
//                flag_count_PIOut_temp4 = 0;
//            }
//            
            
            if(StepMotor_Ctrl.err >= -300 && StepMotor_Ctrl.err < 0 && StepMotor_Ctrl.stepNum  < 0) //误差在-350以内时 ，反向输出步数强制为一步
            {
                flag_count_PIOut_temp5 ++;
                StepMotor_Ctrl.stepNum = 0;
            }
            else flag_count_PIOut_temp5=0;

            if(flag_count_PIOut_temp5 >= 1)    //误差在350以内时 ，输出步数强制为一步 ，且3个PID周期执行一步
            {
                StepMotor_Ctrl.stepNum = -2;
                flag_count_PIOut_temp5 = 0;
            }
        }
        
        
        if(StepMotor_Ctrl.err >= 0 && StepMotor_Ctrl.stepNum  < 0) 
        {
            StepMotor_Ctrl.stepNum = StepMotor_Ctrl.stepNum >> 2; /** 误差与步数符号相斥时，衰减步数（避免过冲） */
        }
        if(StepMotor_Ctrl.err <= 0 && StepMotor_Ctrl.stepNum  > 0) 
        {
            StepMotor_Ctrl.stepNum = StepMotor_Ctrl.stepNum >> 1; /** 同上，另一侧衰减 */
        }            
        

//        //突加载步进电机动作补偿
//        if(StepMotor_Ctrl.stepNum >= 8 && StepMotor_Ctrl.err > 450 && flag_count_PIOut <= 5)
//        {
//            if(System_Power_Info.Active_Power_Total > 250 && flag_count_PIOut_temp < 1)     
//            {                
//                StepMotor_Ctrl.stepNum = 12;
//                flag_count_PIOut_temp ++;
//            }
//            else    
//            {
//                StepMotor_Ctrl.stepNum = 12;                
//            }
//        }            

        /*----------------------步进电机快关逻辑处理-------------------------*/
        // 突降载检测：功率差超 1100/1500/2300 W 时，排队 -20 步快关油门，确保调速不过冲
        if( (Active_Power_Total_Old - System_Power_Info.Active_Power_Total) > 2300 )//
            Active_Power_Total_Count = 11;
        else if( (Active_Power_Total_Old - System_Power_Info.Active_Power_Total) > 1500 )
            Active_Power_Total_Count = 8;
        else if( (Active_Power_Total_Old - System_Power_Info.Active_Power_Total) > 1100 )
            Active_Power_Total_Count = 4;
        else if( StepMotor_Ctrl.stepNum > 0)
            Active_Power_Total_Count = 0;    
        if(Active_Power_Total_Count > 0 && StepMotor_Ctrl.err < 0)
        {
            StepMotor_Ctrl.stepNum = -20; //反向输出步数（快关油门以快速回落）
            Active_Power_Total_Count --;
        }
        //----------------------超速保护逻辑处理-------------------------*/
        // 一旦反馈 ≥ CM_Engine_Speed_RefMAX 或超调 >600，直接发出 -26 步关闭
        if(StepMotor_Ctrl.fdb >= CM_Engine_Speed_RefMAX ||\
            (StepMotor_Ctrl.fdb-StepMotor_Ctrl.ref) > 600)
        {
           StepMotor_Ctrl.stepNum = -26;  /** 过速/偏差过大时强退一步数 */
        }

        if      ( StepMotor_Ctrl.stepNum > 0 )  StepMotor_Ctrl.stepDirection =  1; /** 正向（开油） */
        else if ( StepMotor_Ctrl.stepNum < 0 )  StepMotor_Ctrl.stepDirection = -1; /** 反向（关油） */
        else                                      StepMotor_Ctrl.stepDirection =  0; /** 不动 */

        // 恒速小窗逻辑：误差较小且持续时，抑制步进动作以降低噪声/抖动
        if( (StepMotor_Ctrl.err <= 70 && StepMotor_Ctrl.err >= -70) )  //误差在+350以内时，正向输出步数强制为一步
        {
            stepmotor_speed_constcnt++;
            if(stepmotor_speed_constcnt>5)
                stepmotor_speed_const = 1;   /** 误差较小且持续，启用“恒速小窗”抑制抖动 */
        }   
       if( Inv_SineCtrl_Info.BusVol_AD < 1450||\
           StepMotor_Ctrl.err >= 400 ||\
           StepMotor_Ctrl.err <= -400 ||\
           (Active_Power_Total_Old - System_Power_Info.Active_Power_Total) <= -20 ||\
           (Active_Power_Total_Old - System_Power_Info.Active_Power_Total) >= 20)   
       {
           stepmotor_speed_const = 0;        /** 条件破坏恒速小窗，立刻退出 */
           stepmotor_speed_constcnt = 0;
       }
       if(stepmotor_speed_const == 1)
       {
           StepMotor_Ctrl.stepNum = 0;       /** 在恒速小窗中，强制不动作（降低噪声/抖动） */
       }
       
        stepmotor_temp = StepMotor_Ctrl.stepNum;             /** 便于调试观察 */
        Active_Power_Total_Old = System_Power_Info.Active_Power_Total;  /** 记录本次功率 */
    }
    
}
