/*****************************************************************************
@Description: user_sample.h 采样头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef  __USER_SAMPLE_H
#define  __USER_SAMPLE_H

#include "BAT32G139.h"
#include "sys_define_struct.h"
#include "sys_hardware_init.h"
#include "user_parallel.h"
#include "user_stepmotor.h"


extern		int16_t					s16_GeneratorfreVal;
extern		int16_t					s16_OutputcurscaletoPu;
extern		int16_t					s16_UoutcquireVal;
 
void COM_CurSample(void);
void COM_VolSample(void);
void COM_Altern_Sample(void);
void Update_AC_Data(void);//电压电流功率数据更新
void User_EffectiveValue_Calc( void );
void Get_The_Protect_Flag( int Check_Value, Protect_Check_Var_t * Check_Info,unsigned short int* protect_code );
#endif


