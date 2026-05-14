/*****************************************************************************
@Description: sys_define_struct.h 系统结构体定义
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __SYS_STRUCT_H
#define __SYS_STRUCT_H

#include "stdint.h"

/***************************************************************************/
/*-----------------------------常用数学工具宏定义--------------------------*/
/***************************************************************************/
#define ABSFUN(Value)			(Value)>=0?(Value):-(Value)

#define MAXFUN(varone,vartwo)    (varone)>(vartwo)?(varone):(vartwo)

#define UPDNLMTFUN(Var,Max,Min)	{(Var)=((Var)>=(Max))?(Max):(Var);(Var)=((Var)<=(Min))?(Min):(Var);}

//中断中调用
#define DFILTER(filnum,newinputval,oldoutputval)	(((((int32_t)(newinputval)<<16)-(oldoutputval))>>((filnum)+1))+(oldoutputval)) 

//非中断中调用
#define DFILTER_N(filnum1,newinputval1,oldoutputval1)	(((((int32_t)(newinputval1)<<16)-(oldoutputval1))>>((filnum1)+1))+(oldoutputval1)) 



//******************系统中用到的枚举常量**********************************
typedef enum
{
  	E_ACOUT_FRE_60HZ,
	E_ACOUT_FRE_50HZ	
}
V_ACOUT_Fre_e;//系统输出电压频率制式枚举

typedef enum
{
  	E_FALSE = 0,
	E_TRUE  = 1,
	E_FAIL  = 2
}
Logic_Value_e;

typedef enum
{
    E_DISABLE,	
    E_ENABLE
}
Sys_Enable_e;


typedef enum
{
    E_MIN = 0,	   //进入保护时极限值为最小值，大于此值时有效
    E_MAX = 1,	   //进入保护时极限值为最大值，小于此值时有效
    E_EQUAL
}
Protect_Limit_Type_e;

typedef union
{
    int32_t       dWord;
    struct
    {
        uint16_t    lWord;
        int16_t     hWord;
    }half;
}UNLONG_u;  


typedef struct SysClockBase
{   uint8_t sys_PWM_Base;
    uint8_t system_1ms:1;					//系统1ms时基有效位
    uint8_t stepMotor_1ms:1;					//步进电机1ms时基有效位
    uint8_t faultCheck_1ms:1;					//步进电机1ms时基有效位
    uint8_t Resbits:5;						//预留位
}SysClockBase_t;
extern SysClockBase_t  SysClockBase_ms;


/***************************************************************************/
/***************************************************************************/
typedef enum
{
    E_VBUS_OVP                  = ( 1 << 0  ),//0位
    E_VBUS_OLP                  = ( 1 << 1  ),//1位
    E_INDUC_OCP                 = ( 1 << 2  ),//2位
    E_OLP                       = ( 1 << 3  ),//3位
    E_ACOUT_OVP                 = ( 1 << 4  ),//4位
    E_ACOUT_OLP                 = ( 1 << 5  ),//5位
    E_ACOUT_SCP                 = ( 1 << 6  ),//6位
    E_ACOUT_DCIM                = ( 1 << 7  ),//7位
    E_OTP                       = ( 1 << 8  ),//8位
    E_RPM_OVER                  = ( 1 << 9  ),//9位
    E_AUX_POWER_OVP             = ( 1 << 10 ),//10位
    E_AUX_POWER_OLP             = ( 1 << 11 ),//11位
    E_Sys_Init_Fail             = ( 1 << 12 ),//12位
    E_RPM_LOW                   = ( 1 << 13 ),//13位
    E_VREF_ERROR                = ( 1 << 14 ) //14位
}
ProtectFlag_Position_e;

/***************************************************************************/
/*----故障保护结构体数据定义-----------------------------------------------*/
/***************************************************************************/
struct System_Protect_Flag_BITS
{
	unsigned short int VBUS_OVP:        1;//0位  
	unsigned short int VBUS_OLP:        1;//2
	unsigned short int INDUC_OCP:       1;//4
	unsigned short int OLP:             1;//8
	unsigned short int ACOUT_OVP:       1;//16
	unsigned short int ACOUT_OLP:       1;//32
	unsigned short int ACOUT_SCP:       1;//64
	unsigned short int ACOUT_DCIM:      1;//128
	unsigned short int OTP:             1;//256
	unsigned short int RPM_OVER:        1;//512
	unsigned short int AUX_POWER_OVP:   1;//1024
	unsigned short int AUX_POWER_OLP:   1;
	unsigned short int System_Init_Fail:1;
	unsigned short int RPM_LOW:         1;
	unsigned short int Vref_Error:      1;
  unsigned short int rsvd:            1;

};
typedef union _System_Protect_Flag
{
	unsigned short int                   all;
	struct System_Protect_Flag_BITS      bit;
}System_Protect_Flag_u;
extern System_Protect_Flag_u System_ProtectFlag_Info;

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
typedef enum
{
	  E_START_CHECK_VBUS_UP         = ( 1 << 0  ),//0位
	  E_START_CHECK_VBUS_DN         = ( 1 << 1  ),
	  E_START_CHECK_RPM_UP          = ( 1 << 2  ),
	  E_START_CHECK_AUX_POWER_UP    = ( 1 << 3  ),
	  E_START_CHECK_AUX_POWER_DN    = ( 1 << 4  ),
      E_START_CHECK_TEMP_UP         = ( 1 << 5  )
}
StartCheckFlag_Position_e;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
struct StartCheck_Flag_BITS
{
	unsigned short int vBus_Up_OK:          1;//0位 1
	unsigned short int vBus_Dn_OK:          1; //2
	unsigned short int RPM_Up_OK:           1; //4
	unsigned short int auxPower_Up_OK:      1; //8
	unsigned short int auxPower_Dn_OK:      1; //16
	unsigned short int temp_Up_OK:          1; //32
	unsigned short int rsvd:                10;
};
typedef union _StartCheck_Flag
{
	unsigned short int                   all;
	struct StartCheck_Flag_BITS          bit;
}StartCheck_Flag_u;
extern StartCheck_Flag_u StartCheck_Flag_Info;
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
typedef enum
{
    KEY_OFF = 0  ,
    KEY_ON	= 1
}
KEY_STATE_e;//开/关两种状态

typedef enum
{
	Save_Energy_Mode=0,//默认是节能模式
	Full_Speed_Mode  //全速模式
}
Sys_Run_Mode_e;

/***************************************************************************/
/*--Protect flag information-----------------------------------------------*/
/***************************************************************************/
typedef struct _Protect_Check_Info//保护的进入/处理、和恢复的数据结构
{
    unsigned int short  FlagBit_position;
    unsigned int short  Limit_Type;
    unsigned int short  LED_Sparkle_Flag;
    unsigned int        Limit_Value_Time;//以 1MS为单位
    unsigned int        Limit_Value_Count;//计数值，唯一的一个变量，其它的都是初始化后不再改变
             int        Limit_Value;
             int        Limit_Hysteretic_Value;
}
Protect_Check_Var_t;

extern Protect_Check_Var_t      INV_P_OLP1_Info;
extern Protect_Check_Var_t      INV_P_OLP2_Info;
extern Protect_Check_Var_t      INV_P_OLP3_Info;
extern Protect_Check_Var_t      INV_P_OLP4_Info;

extern Protect_Check_Var_t      INV_ACOUT_SCP_Info;

extern Protect_Check_Var_t      COM_VBUS_OVP1_Info;
extern Protect_Check_Var_t      COM_VBUS_OVP2_Info;
extern Protect_Check_Var_t      COM_VBUS_OVP3_Info;

extern Protect_Check_Var_t      COM_VBUS_OLP1_Info;
extern Protect_Check_Var_t      COM_VBUS_OLP2_Info;
extern Protect_Check_Var_t      COM_VBUS_OLP3_Info;

extern Protect_Check_Var_t      INV_Induc_OCP1_Info;
extern Protect_Check_Var_t      INV_Induc_OCP2_Info;
extern Protect_Check_Var_t      INV_Induc_OCP3_Info;
extern Protect_Check_Var_t      INV_Induc_OCP4_Info;

extern Protect_Check_Var_t      INV_ACOUT_RMS_OVP_Info;
extern Protect_Check_Var_t      INV_ACOUT_RMS_OLP_Info;

extern Protect_Check_Var_t      COM_AuxPower_OVP_Info;
extern Protect_Check_Var_t      COM_AuxPower_OLP_Info;

extern Protect_Check_Var_t      COM_OTP1_Info;
extern Protect_Check_Var_t      COM_OTP2_Info;

extern Protect_Check_Var_t      COM_RPM_Over;
extern Protect_Check_Var_t      COM_RPM_Low;

extern Protect_Check_Var_t      COM_VrefOver_Info;
extern Protect_Check_Var_t      COM_VrefLow_Info;

extern Protect_Check_Var_t      COM_StartCheck_VBUS_Up_Info;
extern Protect_Check_Var_t      COM_StartCheck_VBUS_Dn_Info;
extern Protect_Check_Var_t      COM_StartCheck_RPM_Info;
extern Protect_Check_Var_t      COM_StartCheck_AuxPower_Up_Info;
extern Protect_Check_Var_t      COM_StartCheck_AuxPower_Dn_Info;
extern Protect_Check_Var_t      COM_StartCheck_Temp_Up ;
/***************************************************************************/
/*---Ctrl_Para_Set---------------------------------------------------------*/
/***************************************************************************/
typedef struct _Ctrl_Para_Set
{	
    int   Ref;
    int   Kp;
    int   Ki;
    int   Ctrl_PRD;	
}
Ctrl_Para_Set;
extern Ctrl_Para_Set InvVol_CtrlPara_Set;
extern Ctrl_Para_Set InvCur_CtrlPara_Set;
extern Ctrl_Para_Set EngSpeed_CtrlPara_Set;
/***************************************************************************/
/*---Inverter Protect flag-----------------------------------------------*/
/***************************************************************************/
struct MotorConverter_Protect_Flag_BITS
{
    unsigned short int  Converter_BusVol_Over:     1;
    unsigned short int  Converter_BusVol_Low:      1;
    unsigned short int  Converter_Curr_Over:       1;
    unsigned short int  Converter_NTC_Temp_Over:   1;	  
    unsigned short int  rsvd:                      12;
};
union MotorConverter_Protect_Flag
{
    unsigned short int                        All;
    struct MotorConverter_Protect_Flag_BITS   Bit;
};
extern union MotorConverter_Protect_Flag MotorConverter_ProtectFlag;


/***************************************************************************/
/*---SystemStatus flag-----------------------------------------------------*/
/***************************************************************************/
struct SystemStatus_Flag_BITS
{
    unsigned short int  HardWare_Check:                  1;//0位
    unsigned short int  Offset_Adj_Time:                 1;
    unsigned short int  Offset_Adj_End:                  1;
    unsigned short int  SCI_Bits_End:                    1;
    unsigned short int  SCI_Bits_Ready:                  1;
    unsigned short int  AD_Mutex_Channel_GroupB:         3;
    unsigned short int  AD_Reference_Complect:           1;
    unsigned short int  RESERVED:                        7;
};
union SystemStatus_Flag
{
    unsigned short int              all;
    struct SystemStatus_Flag_BITS   bit;
};
extern union SystemStatus_Flag SystemStatusFlag_Info;
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
typedef struct _Power_Info
{
    int Apparent_Power_Total; // 视在功率,单位VA
    int Active_Power_Total;   // 有功功率,单位W
    int Active_Power_Rpm;     // 有功功率（转速相关）
    int Reavtive_Power_Total; // 无功功率(Reactive),单位Var
} Power_Info;
extern Power_Info System_Power_Info;

/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
struct _ADSample_RefCorrect_Flag
{
    unsigned short int ADRef_Correct_Ok:  1;
    unsigned short int Rsvd:              15;
};
union ADSample_RefCorrect_Flag
{
    unsigned short int                    all;
    struct _ADSample_RefCorrect_Flag      bit; 
};
typedef struct _ADSample_RefCorrect
{
    union ADSample_RefCorrect_Flag        Flag; 
    int                                   ADRef_Correct_Sum;
    int                                   ADRef_Correct_Sum_Cnt;
    int                                   ADRef_Correct_Up_Limit;
    int                                   ADRef_Correct_Mid_Limit;
    int                                   ADRef_Correct_Down_Limit;
    int                                   ADRef_Correct_Value;
}AD_Correct_Var_t;
extern AD_Correct_Var_t AD_Correct_V_ACOUT;
extern AD_Correct_Var_t AD_Correct_I_Induc;
extern AD_Correct_Var_t AD_Correct_Vref;
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
typedef struct  _ADSample_BaseValuePU
{
    short int InvVol_ScaleToPU; 
    short int InvCur_ScaleToPU;
    short int LoadCur_ScaleToPU;
    short int BusVol_ScaleToPU;
    short int Power_ScaleToPU;
    short int DCIM_Vol_ScaleToPU;
    short int AD_Ref_ScaleToPU;
}ADSample_ScaleToPU;
extern ADSample_ScaleToPU ADSample_ScaleToPU_Info;
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
typedef struct _AlgCurve_Calc
{
    int KpToKi_Factor;
    int Factor_B;
    int Factor_K;
    int AlgCurve_Value;
    int AlgCurve_Value_Up_Limit;
    int AlgCurve_Value_Down_Limit;
}AlgCurve_Calc;
extern AlgCurve_Calc  AlgCurve_Calc_CurLoop_Kp;
extern AlgCurve_Calc  AlgCurve_Calc_BusToInVolAM_Factor;
extern AlgCurve_Calc  AlgCurve_Calc_RepCtrl_Factor;
extern AlgCurve_Calc  AlgCurve_Calc_BusToVirRes_Factor;
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
typedef struct _ADSample_Store
{
    int  InvVol_Store_One;
    int  InvVol_Store_Two;
    int  InvCur_Store_One;
    int  InvCur_Store_Two;
    int  LoadCur_Store_One;
    int  LoadCur_Store_Two;
    int  BusVol_Store_One;
    int  BusVol_Store_Two;
    int  DCIM_Vol_Store;
    int  AD_Store_Ref;  
}ADSample_Store;
extern ADSample_Store   ADSample_Store_Info;
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
typedef struct 
{
    int  AC_Vol_Peak;
    int  Inv_Cur_Peak;
    int  Load_Cur_Peak;
    int  AC_Vol_Sum;
    int  Inv_Cur_Sum;
    int  Load_Cur_Sum;
    int  AC_Vol_Sum_Back;
    int  Inv_Cur_Sum_Back;
    int  Load_Cur_Sum_Back;
    int  AC_Vol_RMS;
    int  Inv_Cur_RMS;
    int  Load_Cur_RMS;
    int  AC_Vol_DCIM;
    int  AC_Vol_DCIM_Sum;
    int  AC_Vol_DCIM_Sum_Back;
    int  AC_Vol_DCIM_RMS;
    int  Bus_Vol_AD;
    int  Bus_Vol_Sum;
    int  Bus_Vol_Filiter_AD;
    int  Bus_Vol_Avg_Sum;
    int  Bus_Vol_Avg_AD_AM;
    int  AC_Active_Power_Sum;
    int  AC_Active_Power_Back;
    int  Power_AuxVcc_AD;
    int  Temp_External_AD;
    int  ref_AD;
    int  ref_AD_Fir;
} ADSample_VAR;
extern ADSample_VAR    ADSample_Info;
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/				 
typedef struct {
    unsigned short int In;
    unsigned short int IO_H_count;
    unsigned short int IO_L_count;
    unsigned short int Key_State;
    unsigned short int Compare_Logic;
    unsigned short int Compare_Dot;
    unsigned short int Compare_Type;
    unsigned short int Delay_MaxCount;
    void  (*Calc)();
} KeyInfo;
typedef KeyInfo *KeyInfo_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PARK object.
-----------------------------------------------------------------------------*/

#define KeyInfo_DEFAULTS {  0, \
	                         0, \
	                         0, \
	                         0, \
	                         0, \
	                         0, \
	                         0, \
	                         0, \
	                         (void (*)( unsigned int ))key_Calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PARK.C
------------------------------------------------------------------------------*/
void key_Calc(KeyInfo_handle);


extern KeyInfo	            Key_SW_FreqChoice	;
extern KeyInfo	            Key_SW_ECO;
extern KeyInfo	            Key_Pro_Reset;
  

typedef struct 
{
    uint8_t	motorconbit:1;		//步进电机控制周期标志位		bit0
    uint8_t reserbits:7;			//预留位			bit1~bit7
}Stepmotorconlogic;
extern Stepmotorconlogic Stpmotorcon;


typedef struct 
{
    uint16_t    systemabnormalbit:1;				//系统故障标志位			bit0
    uint16_t	ecomodeidbit:1;						//ECO模式确认				bit1
    uint16_t	powercunbit:1;						//power cut					bit2
    uint16_t	freswitchbit:1;						//频率切换					bit3
    uint16_t	ecochangebit:1;						//ECO按键状态更改标志位		bit4
    uint16_t    powercutchangebit:1;				//power cut按键状态更改		bit5
    uint16_t	freswitchchangebit:1;				//Fre状态改变标志位			bit6
    uint16_t	powercutovertimebit:1;				//Powercut次数超限			bit7
    uint16_t	sysoverloadbit:1;					//输出过载标志位            bit8 
    uint16_t	sysoverloadclosebit:1;				//输出过载准备关机标志位    bit9
    uint16_t 	Resbits:	6;						//预留位					bit10 ~ bit15
}Controlboardlogic;
extern Controlboardlogic    Controllogic;



#endif
/***************************************************************************/

