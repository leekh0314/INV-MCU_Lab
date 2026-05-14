/*****************************************************************************
@Description: user_parallel.h 并网控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef  __USER_PARALLEL_H
#define  __USER_PARALLEL_H

#include "BAT32G139.h"
#include "sys_define_struct.h"


typedef struct 
{
    uint16_t		masterslavebit:2;					//主从机标志位 1:					bit0 bit1
    uint16_t		synchronizbit:1;					//检测到有效的工频同步信号			bit2
    uint16_t		masterslaverunbit:1;			    //主从机启动标志位					bit3
    uint16_t		slavesunprebit:1;					//从机预启动标志位					bit4
    uint16_t		synsignalokbit:1;					//工频同步信号有效标志				bit5
    uint16_t		slavestartokbit:1;					//从机开机标志位					bit6
    uint16_t		softstartokbit:1;					//软启动完成标志位					bit7
    uint16_t		capturecomebit:1;					//捕获标志位，用于滤波				bit8
    uint16_t        Resbits:7;							//预留位							bit9 - bit15
}Parainverterslogic;


extern UNLONG_u					    Unlongdcvolcomponentfilsec;
extern int16_t						s16_CapturetimeUplim;
extern int16_t						s16_CapturetimeDnlim;
extern int16_t						s16_CapturetimeMaxlim;
extern int16_t						s16_MasterslaveidVal;
extern int16_t						s16_MasterslaveidDclim;
extern int16_t						s16_ParaslavetomastrVal;
extern int16_t	                    s16_GeneratorSpeedAvg;


extern void  Masterslavecheck(void);
extern void  INV_ACOUT_FreCalc(void);
extern volatile Parainverterslogic		paralogiccontrol;

#endif

