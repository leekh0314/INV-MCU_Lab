/*****************************************************************************
@Description: INV_Power_Ctrl.h 逆变器功率控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __INV_POWER_CTRL__H__
#define __INV_POWER_CTRL__H__
#include "stdint.h"
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/
typedef struct
{
        int32_t     AC_Vol_AMP_Target_Ref;//软启动参考值
        int32_t     AC_Vol_AMP_Target;//幅值
        int16_t     SS_Step_Value;        
        int8_t      SS_Shift_Value;       
        int32_t     SS_AMP_Target_Hold;             

        int8_t      mode_AC_Software_Ctrl;//AC输出软启动控制

}INV_Var_t; 

extern INV_Var_t INV_Info;    
    
typedef struct { 
    uint8_t  u8Mode_Select; //限功率使能
    uint16_t SPWM_Compare_A;
    uint16_t SPWM_Compare_B;
} INV_Power_Ctrl_Var_t;
void INV_Power_Ctrl(void);
extern INV_Power_Ctrl_Var_t INV_Power_Ctrl_Info;

typedef struct
{  
        int32_t     DCIM_Val;    
        int32_t     DCIM_Val_Fir;
        int32_t     DCIM_Sum;

}INV_Ctrl_Var_t;
extern INV_Ctrl_Var_t INV_Ctrl_Info;

typedef struct { 
    int32_t  ref;                  // 输入: 给定输入
    int16_t  fdb;                  // 输入: 反馈输入
    int32_t  err;                  // 变量: 当前误差
    int32_t  err_n;                // 变量: 上次误差
    int32_t  err_n2;  					   // 变量: 上上次误差
    int32_t  err_Integral;				 // 变量: 积分累计误差	
    int32_t  kp;                   // 参数: 比例系数          
    int32_t  ki;                   // 参数: 积分系数        
    int32_t  kd; 	                 // 参数: 微分系数
    int32_t  err_Max;              // 参数: 当前误差上限幅          
    int32_t  err_Min;              // 参数: 当前误差下限幅 
    int32_t  err_Integral_Max;           // 参数: 积分输出上限幅
    int32_t  err_Integral_Min;           // 参数: 积分输出下限幅
    int32_t  out_Max;          // 参数: PID输出上限幅          
    int32_t  out_Min;     	   // 参数: PID输出下限幅 
    int32_t  up;                   // 变量: 比例输出
    int32_t  ui;                   // 变量: 积分输出	
    int32_t  ud;                   // 变量: 积分输出		
    int32_t  out;                  // 变量: PID输出             
    uint16_t  ctrl_Period_Cnt;
    uint16_t  ctrl_Period_Val;    // 参数: 控制周期   
   void  (*Calc)();	/* Pointer to calculation function */                          
} PID_Ctrl_Var_t;

/*-------------------INV功率环参数---------------------------------------*/
#define     INV_POWER_REF                               (3290)//(W)
#define     INV_POWER_KP                                (4500)
#define     INV_POWER_KI                                (4000)
#define     INV_POWER_KD                                (0)
#define     INV_POWER_ERR_MAX				(400)
#define     INV_POWER_ERR_MIN				(-400)

#define     INV_POWER_OUT_MAX                           (0)    
#define     INV_POWER_OUT_MIN                           (-700)
#define     INV_POWER_ERR_INTEGRAL_MAX                  (0)    
#define     INV_POWER_ERR_INTEGRAL_MIN                  (INV_POWER_OUT_MIN*32767/INV_POWER_KI)
#define     INV_POWER_PERIOD_VALUE                      (410)//控制周期低于功率计算周期

/*-------------------DCIM参数------------------------------------------*/
#define     INV_DCIM_KP                                 (19900)
#define     INV_DCIM_KI                                 (1500)
#define     INV_DCIM_KD                                 (0)
#define     INV_DCIM_ERR_MAX				(32767)
#define     INV_DCIM_ERR_MIN			  (-32767)

#define     INV_DCIM_OUT_MAX                            (900)    
#define     INV_DCIM_OUT_MIN                            (-900)
#define     INV_DCIM_ERR_INTEGRAL_MAX                   (INV_DCIM_OUT_MAX*32767/INV_DCIM_KI)    
#define     INV_DCIM_ERR_INTEGRAL_MIN                   (INV_DCIM_OUT_MIN*32767/INV_DCIM_KI)
#define     INV_DCIM_PERIOD_VALUE                       (0)

/*-----------------------------------------------------------------------------*/
#define INV_POWER_LOOP_DEFAULTS {INV_POWER_REF,                 \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                INV_POWER_KP,                   \
                                INV_POWER_KI,                   \
                                INV_POWER_KD,                   \
                                INV_POWER_ERR_MAX,              \
                                INV_POWER_ERR_MIN,              \
                                INV_POWER_ERR_INTEGRAL_MAX,     \
                                INV_POWER_ERR_INTEGRAL_MIN,     \
                                INV_POWER_OUT_MAX,              \
                                INV_POWER_OUT_MIN,              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                INV_POWER_PERIOD_VALUE,         \
                                (void (*)( unsigned int ))PID_Ctrl}        

                                
#define INV_DCIM_LOOP_DEFAULTS { 0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                INV_DCIM_KP,                     \
                                INV_DCIM_KI,                     \
                                INV_DCIM_KD,                     \
                                INV_DCIM_ERR_MAX,                \
                                INV_DCIM_ERR_MIN,                \
                                INV_DCIM_ERR_INTEGRAL_MAX,       \
                                INV_DCIM_ERR_INTEGRAL_MIN,       \
                                INV_DCIM_OUT_MAX,                \
                                INV_DCIM_OUT_MIN,                \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                0,                              \
                                INV_DCIM_PERIOD_VALUE,           \
                                (void (*)( unsigned int ))PID_Ctrl}
/*------------------------------------------------------------------------------*/

void INV_DCIM_Ctrl(void);
void INV_Power_Ctrl(void);                                
void PID_Ctrl(PID_Ctrl_Var_t *PID_Info);
                                
extern PID_Ctrl_Var_t INV_PID_Power;
extern PID_Ctrl_Var_t INV_PID_DCIM;                
               
#endif /*PID_H_*/
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/

