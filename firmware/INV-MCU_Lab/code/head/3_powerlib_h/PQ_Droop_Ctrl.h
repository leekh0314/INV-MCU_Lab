/*****************************************************************************
@Description: PQ_Droop_Ctrl.h 有功无功下垂控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __PQ_DROOP_CTRL_H_
#define __PQ_DROOP_CTRL_H_


/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*  PQ_Droop_Ctrl.h.                                                           */
/*-----------------------------------------------------------------------------*/
typedef struct{   int  P_n_In;
                  int  Q_n_In;
	              int  BusVol_In;
	              int  Power_P_OmigaCoeff;
                  int  Power_Q_OmigaCoeff;
	              int  Power_P_AMPCoeff;
                  int  Power_Q_AMPCoeff;
                  int  Omiga_Out_Limit_Max;
                  int  Omiga_Out_Limit_Min;
                  int  AMP_Out_Limit_Max;
                  int  AMP_Out_Limit_Min;
                  int  Omiga_Out;
                  int  AMP_Out;
                  void (*Calc)();
}PQ_Droop_Ctrl_Var;
/*-----------------------------------------------------------------------------*/
/*Default initalizer for the PQ_Droop_Ctrl object.                             */
/*-----------------------------------------------------------------------------*/
#define PQ_Droop_Ctrl_DEFAULTS { 0,\
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
	                               0,\
	                               PQ_Droop_Ctrl }		
/*-----------------------------------------------------------------------------*/
/*Prototypes for the functions in PQ_Droop_Ctrl.C                              */
/*-----------------------------------------------------------------------------*/
extern void PQ_Droop_Ctrl( PQ_Droop_Ctrl_Var *PQ_Droop_Ctrl );
extern void Parallel_Para_Init( PQ_Droop_Ctrl_Var *Parallel_Para_Init , unsigned int Power_Level, unsigned int InvVol_Level,unsigned int InvVolFreq ,unsigned int SelfOther_Parallel);
																 
extern PQ_Droop_Ctrl_Var PQ_Droop_Ctrl_Info;                                                                                                                                      
                                   
#endif /* PQ_DROOP_CTRL_H_ */
/*----------------------------------------------------------------------------*/
/*  No more.                                                                  */
/*----------------------------------------------------------------------------*/
