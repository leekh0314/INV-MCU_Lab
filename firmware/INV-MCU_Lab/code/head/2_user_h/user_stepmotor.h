/*****************************************************************************
@Description: user_stepmotor.h 步进电机控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef  __USER_STEPMOTOR_H
#define  __USER_STEPMOTOR_H

#include "BAT32G139.h"
#include "sys_define_struct.h"
#include "Lookup_Table.h"


extern   int16_t	s16_GeneratorfreVal;
extern   UNLONG_u GeneratorfreFilter;

void     Stepmotordrive(void);
void     StepmotormoveCheck(void);

#endif


