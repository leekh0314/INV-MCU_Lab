/*****************************************************************************
@Description: Repeat_Ctrl.h 重复控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __REPEAT_CTRL_H_
#define __REPEAT_CTRL_H_


typedef struct {    int       Qz_Coeff;
                    int       Err_Coeff;
                    int       Ref;
                    int       Fdb;
                    int       SPWM_Point;
                    int       Index;
                    int       Leg_Comp_Point;
                    int       Ek_Limit_Max;
                    int       Ek_Limit_Min;
                    int       Out_Limit_Max;
                    int       Out_Limit_Min;
                    int       Out;
                    void      (*Calc)();     // Pointer to calculation function
                }RepeatCtrl_Var;
/*-----------------------------------------------------------------------------
Default initalizer for the RepeatCtrl object.
-----------------------------------------------------------------------------*/
#define REPEATCTRL_DEFAULTS {   0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                                0,\
                        (void *)0 }
/*------------------------------------------------------------------------------
Prototypes for the functions in RepeatCtrl_Calc.C
------------------------------------------------------------------------------*/
extern void RepeatCtrl_Calc(RepeatCtrl_Var *RepeatCtrl);
extern void RepeatCtrl_Para_Init( RepeatCtrl_Var *RepeatCtrl_Para_Init , unsigned int Power_Level, unsigned int InvVol_Level,unsigned int InvVolFreq );

extern RepeatCtrl_Var RepeatCtrl_Info;
//extern int   Repeat_Ek_Buff[];
extern int   Repeat_Out_Buff[];

#endif /* RepeatCtrl_H_ */
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/
