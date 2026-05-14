/*****************************************************************************
@Description: LPF.h 低通滤波头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __LPF_H_
#define __LPF_H_

#define	CM_NOTH_X1_Coeff       _IQ( -2      )
#define	CM_NOTH_X2_Coeff       _IQ(  1      )
#define	CM_NOTH_Y1_Coeff       _IQ(  1.98   )
#define	CM_NOTH_Y2_Coeff       _IQ( -0.9806 )
/*----------------------------------------------------------------------------------*/

typedef struct {  long  Input;     // Input:  filiter input k
				  long  Out;       // Output: filiter output k
				  long  Out_k;     // Output: filiter output k-1
				  long  Coeff;       // Input:  switch frequency
		 	 	  void  (*Calc)();	 // Pointer to calculation function
} LPF_FirstOrder_VAR;

typedef LPF_FirstOrder_VAR *LPF_FirstOrder_handle;
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

typedef struct {  long  Input;     // Input:  filiter input k
				  long  Out;       // Output: filiter output k
				  long  x_k_1;
				  long  x_k_2;
				  long  y_k_1;
				  long  y_k_2;
				  long  X1_Coeff;
				  long  X2_Coeff;
				  long  y1_Coeff;
				  long  y2_Coeff;
		 	 	  void  (*Calc)();	 // Pointer to calculation function
} _Noth;

typedef _Noth *Noth_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PARK object.
-----------------------------------------------------------------------------*/                     
/*-----------------------------------------------------------------------------*/

#define LPF_FIRSTORDER_DEFAULTS { 0, \
	                              0, \
	                              0, \
	                              0, \
	                              (void (*)( unsigned long ))LPF_FirstOrder_Calc }

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
#define NOTH_DEFAULTS {           0, \
	                              0, \
	                              0, \
	                              0, \
	                              0, \
	                              0, \
	                              CM_NOTH_X1_Coeff, \
	                              CM_NOTH_X2_Coeff, \
	                              CM_NOTH_Y1_Coeff, \
	                              CM_NOTH_Y2_Coeff, \
	                              (void (*)( unsigned long ))Noth_Calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PARK.C
------------------------------------------------------------------------------*/
void Noth_Calc(Noth_handle);
void LPF_FirstOrder_Calc(LPF_FirstOrder_handle);

extern LPF_FirstOrder_VAR LPF_FirstOrder_AC_Voltage_PeakA;
extern LPF_FirstOrder_VAR LPF_FirstOrder_Inv_Current_PeakA;
extern LPF_FirstOrder_VAR LPF_FirstOrder_Load_Current_PeakA;

extern LPF_FirstOrder_VAR LPF_FirstOrder_Power_15V;
extern LPF_FirstOrder_VAR LPF_FirstOrder_Bus_Vol_A;
extern LPF_FirstOrder_VAR LPF_FirstOrder_External_Temp;
extern LPF_FirstOrder_VAR LPF_FirstOrder_Internal_Temp;
extern LPF_FirstOrder_VAR LPF_FirstOrder_ADKey_FreqSelect;
extern LPF_FirstOrder_VAR LPF_FirstOrder_ADKey_ECO;
extern LPF_FirstOrder_VAR LPF_FirstOrder_AD_Reference;

extern LPF_FirstOrder_VAR LPF_FirstOrder_Eng_Freq;
extern LPF_FirstOrder_VAR LPF_FirstOrder_Pro_Release;

//extern _Noth  Noth_BusVol_AD;

#endif /*LPF_H_*/
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/


