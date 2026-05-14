/*****************************************************************************
@Description: PID计算 头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __PQ_CALC_H_
#define __PQ_CALC_H_

//extern short int                 PQ_Vol_peak[];
//extern short int                 PQ_Cur_peak[];
/*------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------
 *  PQ_Calc.h.
 *------------------------------------------------------------------------------------*/

typedef struct {  unsigned short int        Calc_N;
                  unsigned short int        Calc_dot_4;
                  unsigned short int        Array_Index;

                          long   int        Vol_Peak_AD;
                           long  int        Cur_Peak_AD;

                        long     int        P_Out;
                        long     int        Q_Out;
                                void        (*Calc)();	 // Pointer to calculation function
}PQ_Calc_Var;

typedef PQ_Calc_Var *PQ_Calc_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PQ_Calc object.
-----------------------------------------------------------------------------*/
#define PQ_Calc_DEFAULTS { 0,                        \
	                         0,                        \
	                         0,                        \
	                         0,                        \
	                         0,                        \
	                         0,                        \
	                         0,                        \
	                         (void*)0 }
/*------------------------------------------------------------------------------
Prototypes for the functions in PQ_Calc.C
------------------------------------------------------------------------------*/
extern void PQ_Calc(PQ_Calc_handle);
extern void PowerCalc_Para_Init( PQ_Calc_Var *PowerCalc_Para_Init , unsigned int Switch_Dot);
extern PQ_Calc_Var PQ_Calc_Info;

//extern    long int P_Temp,Q_Temp;
#endif /* PQ_CALC_H_ */
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/
