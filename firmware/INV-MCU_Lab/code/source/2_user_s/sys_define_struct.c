/*****************************************************************************
@Description: sys_define_struct.c
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "BAT32G139.h"
#include "PID.h"
#include "sys_define_struct.h"
#include "sys_define_parar.h"
#include "AlgorithmLevel_Headers.h"
#include "INV_Power_Ctrl.h"
/***************************************************************************/

/*------------------------------------------------------------------------------------*/
//母线过压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_VBUS_OVP1_Info 	= { E_VBUS_OVP,E_MAX, 0,COM_VBUS_OVP1_TIME,0,\
                                                         COM_VBUS_OVP1_VAL ,COM_VBUS_OVP1_VAL_BACK };

Protect_Check_Var_t      COM_VBUS_OVP2_Info 	= { E_VBUS_OVP,E_MAX,0,COM_VBUS_OVP2_TIME,0, \
                                                         COM_VBUS_OVP2_VAL ,COM_VBUS_OVP2_VAL_BACK };

Protect_Check_Var_t      COM_VBUS_OVP3_Info     = { E_VBUS_OVP,E_MAX,0,COM_VBUS_OVP3_TIME,0, \
                                                         COM_VBUS_OVP3_VAL ,COM_VBUS_OVP3_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//母线欠压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_VBUS_OLP1_Info 	= { E_VBUS_OLP,E_MIN, 0,COM_VBUS_LVP1_TIME,0,\
                                                         COM_VBUS_LVP1_VAL ,COM_VBUS_LVP1_VAL_BACK };

Protect_Check_Var_t      COM_VBUS_OLP2_Info 	= { E_VBUS_OLP,E_MIN,0,COM_VBUS_LVP2_TIME,0, \
                                                         COM_VBUS_LVP2_VAL ,COM_VBUS_LVP2_VAL_BACK };

Protect_Check_Var_t      COM_VBUS_OLP3_Info 	= { E_VBUS_OLP,E_MIN,0,COM_VBUS_LVP3_TIME,0, \
                                                         COM_VBUS_LVP3_VAL ,COM_VBUS_LVP3_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//过载变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      INV_P_OLP1_Info 		= { E_OLP,E_MAX, 0,INV_P_OLP1_TIME,0,\
                                                         INV_P_OLP1_VAL ,INV_P_OLP1_VAL_BACK };

Protect_Check_Var_t      INV_P_OLP2_Info 		= { E_OLP,E_MAX,0,INV_P_OLP2_TIME,0, \
                                                         INV_P_OLP2_VAL ,INV_P_OLP2_VAL_BACK };

Protect_Check_Var_t      INV_P_OLP3_Info 	    = { E_OLP,E_MAX, 0,INV_P_OLP3_TIME,0,\
                                                         INV_P_OLP3_VAL ,INV_P_OLP3_VAL_BACK };

Protect_Check_Var_t      INV_P_OLP4_Info 	    = { E_OLP,E_MAX,0,INV_P_OLP4_TIME,0, \
                                                         INV_P_OLP4_VAL ,INV_P_OLP4_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//过流变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      INV_Induc_OCP1_Info 		= { E_INDUC_OCP,E_MAX,0,INV_INDUC_RMS_OCP1_TIME,0,\
                                                         INV_INDUC_RMS_OCP1_VAL,INV_INDUC_RMS_OCP1_VAL_BACK };

Protect_Check_Var_t      INV_Induc_OCP2_Info 		= { E_INDUC_OCP,E_MAX,0,INV_INDUC_RMS_OCP2_TIME,0,\
                                                         INV_INDUC_RMS_OCP2_VAL,INV_INDUC_RMS_OCP2_VAL_BACK };

//Protect_Check_Var_t      INV_Induc_OCP3_Info 	    = { E_INDUC_OCP,E_MAX,0,CM_Curr_Over_Three_Time,0,\
//                                                         CM_Curr_Over_Three_Value,CM_Curr_Over_Three_Value_Back };

//Protect_Check_Var_t      INV_Induc_OCP4_Info 	    = { E_INDUC_OCP,E_MAX,0,CM_Curr_Over_Four_Time,0,\
//                                                         CM_Curr_Over_Four_Value,CM_Curr_Over_Four_Value_Back };
/*------------------------------------------------------------------------------------*/
//输出过压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      INV_ACOUT_RMS_OVP_Info 		= { E_ACOUT_OVP,E_MAX,0,INV_ACOUT_RMS_OVP_TIME,0,\
                                                         INV_ACOUT_RMS_OVP_VAL,INV_ACOUT_RMS_OVP_VAL_BACK };
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
//输出欠压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      INV_ACOUT_RMS_OLP_Info 		= { E_ACOUT_OLP,E_MIN,0,INV_ACOUT_RMS_LVP_TIME,0,\
                                                         INV_ACOUT_RMS_LVP_VAL,INV_ACOUT_RMS_LVP_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//输出短路变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      INV_ACOUT_SCP_Info 		    = { E_ACOUT_SCP,E_MIN,0,INV_ACOUT_SCP_TIME,0,\
                                                         INV_ACOUT_SCP_VAL,INV_ACOUT_SCP_VAL_BACK };

/*------------------------------------------------------------------------------------*/
//发动机过速变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_RPM_Over 		            = { E_RPM_OVER,E_MAX,0,COM_RPM_OVER_TIME,0,\
                                                         COM_RPM_OVER_VAL,COM_RPM_OVER_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//发动机欠速变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_RPM_Low                    = { E_RPM_LOW,E_MIN,0,COM_RPM_LOW_TIME,0,\
                                                         COM_RPM_LOW_VAL,COM_RPM_LOW_VAL_BACK };
/*------------------------------------------------------------------------------------*/
//辅助电源过压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_AuxPower_OVP_Info          = { E_AUX_POWER_OVP,E_MAX,0,COM_AUX_POWER_OVP_TIME,0,\
                                                         COM_AUX_POWER_OVP_VALUE,COM_AUX_POWER_OVP_VALUE_BACK };
/*------------------------------------------------------------------------------------*/
//辅助电源欠压变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_AuxPower_OLP_Info          = { E_AUX_POWER_OLP,E_MIN,0,COM_AUX_POWER_LVP_TIME,0,\
                                                        COM_AUX_POWER_LVP_VALUE,COM_AUX_POWER_LVP_VALUE_BACK };
/*------------------------------------------------------------------------------------*/
//逆变器过温变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_OTP1_Info 	                = { E_OTP,E_MIN,0,COM_OTP1_TIME,0,\
                                                        COM_OTP1_VAL,COM_OTP1_VAL_BACK };

Protect_Check_Var_t      COM_OTP2_Info 	                = { E_OTP,E_MIN,0,COM_OTP2_TIME,0,\
                                                        COM_OTP2_VAL,COM_OTP2_VAL_BACK };

/*------------------------------------------------------------------------------------*/
//VREF参考电压异常变量定义
/*------------------------------------------------------------------------------------*/                                                            
Protect_Check_Var_t      COM_VrefOver_Info              = { E_VREF_ERROR,E_MAX,COM_VREF_OVER_TIME,0,\
                                                            COM_VREF_OVER_VALUE,COM_VREF_OVER_VALUE_BACK };

Protect_Check_Var_t      COM_VrefLow_Info               = { E_VREF_ERROR,E_MIN,COM_VREF_LOW_TIME,0,\
                                                            COM_VREF_LOW_VALUE,COM_VREF_LOW_VALUE_BACK };

/*------------------------------------------------------------------------------------*/
//开机启动条件检测变量定义
/*------------------------------------------------------------------------------------*/
Protect_Check_Var_t      COM_StartCheck_VBUS_Up_Info        = { E_START_CHECK_VBUS_UP,E_MIN,0,INV_START_CHECK_VBUS_UP_TIME,0,\
                                                            INV_START_CHECK_VBUS_UP,INV_START_CHECK_VBUS_UP_BACK };

Protect_Check_Var_t      COM_StartCheck_VBUS_Dn_Info        = { E_START_CHECK_VBUS_DN,E_MAX,0,INV_START_CHECK_VBUS_DN_TIME,0,\
                                                            INV_START_CHECK_VBUS_DN,INV_START_CHECK_VBUS_DN_BACK };

Protect_Check_Var_t      COM_StartCheck_RPM_Info            = { E_START_CHECK_RPM_UP,E_MIN,0,COM_START_CHECK_RPM_UP_TIME ,0,\
                                                             COM_START_CHECK_RPM_UP,COM_START_CHECK_RPM_UP_BACK };

Protect_Check_Var_t      COM_StartCheck_AuxPower_Up_Info    = { E_START_CHECK_AUX_POWER_UP,E_MIN,0,COM_START_CHECK_AUX_POWER_UP_TIME,0,\
                                                            COM_START_CHECK_AUX_POWER_UP,COM_START_CHECK_AUX_POWER_UP_BACK };

Protect_Check_Var_t      COM_StartCheck_AuxPower_Dn_Info    = { E_START_CHECK_AUX_POWER_DN,E_MAX,0,COM_START_CHECK_AUX_POWER_DN_TIME,0,\
                                                            COM_START_CHECK_AUX_POWER_DN,COM_START_CHECK_AUX_POWER_DN_BACK };

Protect_Check_Var_t      COM_StartCheck_Temp_Up             = { E_START_CHECK_TEMP_UP,E_MAX,COM_START_CHECK_TEMP_UP_TIME,0,\
                                                            COM_START_CHECK_TEMP_UP,COM_START_CHECK_TEMP_UP_BACK };
                                                        
/*------------------------------------------------------------------------------------*/
//功率采样变量定义
/*------------------------------------------------------------------------------------*/
Power_Info		        System_Power_Info            = {0};
ADSample_VAR            ADSample_Info                = {0};
ADSample_Store          ADSample_Store_Info          = {0};

/*------------------------------------------------------------------------------------*/
//按键变量定义
/*------------------------------------------------------------------------------------*/
KeyInfo                 Key_SW_FreqChoice		     = {0,0,0,0,0,Key_SW_FreqChoice_Compare_Dot,Key_SW_FreqChoice_Compare_Type,Key_SW_FreqChoice_Delay_MaxCount,(void (*)( unsigned int ))key_Calc};
KeyInfo	                Key_SW_ECO			         = {0,0,0,0,0,Key_SW_ECO_Compare_Dot,Key_SW_ECO_Compare_Type,Key_SW_ECO_Delay_MaxCount,(void (*)( unsigned int ))key_Calc};
KeyInfo	                Key_Pro_Reset  			     = {0,0,0,0,0,Key_Pro_Reset_Compare_Dot,Key_Pro_Reset_Compare_Type,Key_Pro_Reset_Delay_MaxCount,(void (*)( unsigned int ))key_Calc};
/*------------------------------------------------------------------------------------*/
//AD采样校正值变量定义
/*------------------------------------------------------------------------------------*/
AD_Correct_Var_t     AD_Correct_V_ACOUT             = {{0},0,0,COM_V_ACOUT_AD_CORRECT_UP,COM_V_ACOUT_AD_CORRECT_MID,\
                                                        COM_V_ACOUT_AD_CORRECT_DOWN,COM_V_ACOUT_AD_CORRECT_MID};
AD_Correct_Var_t     AD_Correct_I_Induc             = {{0},0,0,COM_I_INDUC_AD_CORRECT_UP,COM_I_INDUC_AD_CORRECT_MID,\
	                                                    COM_I_INDUC_AD_CORRECT_DOWN,COM_I_INDUC_AD_CORRECT_MID};
AD_Correct_Var_t     AD_Correct_Vref                = {0,0,0,COM_VREF_AD_CORRECT_UP,COM_VREF_AD_CORRECT_MID,\
                                                        COM_VREF_AD_CORRECT_DOWN,COM_VREF_AD_CORRECT_MID};

/*------------------------------------------------------------------------------------*/
//保护标志变量定义
/*------------------------------------------------------------------------------------*/
System_Protect_Flag_u     System_ProtectFlag_Info   = {0};
StartCheck_Flag_u         StartCheck_Flag_Info      = {0};

/*------------------------------------------------------------------------------------*/
//步进电机控制相关变量定义
/*------------------------------------------------------------------------------------*/
StepMotor_Ctrl_t    StepMotor_Ctrl                  = Step_Motor_DEFAULTS; // 步进电机PI控制
//Lookup_Table        Lookup_Table_Speed              = LOOKUP_TABLE_DEFAULTS;
Stepmotorconlogic   Stpmotorcon;
Controlboardlogic   Controllogic;

/*------------------------------------------------------------------------------------*/
//系统结构体变量定义
/*------------------------------------------------------------------------------------*/
SysClockBase_t          SysClockBase_ms         = {0};

PID_Ctrl_Var_t          INV_PID_Power           = INV_POWER_LOOP_DEFAULTS;

PID_Ctrl_Var_t          INV_PID_DCIM            = INV_DCIM_LOOP_DEFAULTS;
INV_Power_Ctrl_Var_t    INV_Power_Ctrl_Info     = {0};
INV_Ctrl_Var_t          INV_Ctrl_Info           = {0};

/*------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*  No more.                                                                           */
/*-------------------------------------------------------------------------------------*/
