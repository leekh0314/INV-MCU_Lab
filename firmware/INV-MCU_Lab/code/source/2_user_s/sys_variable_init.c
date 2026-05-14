/*****************************************************************************
@Description: sys_variable_init.c 	变量初始化文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "sys_define_parar.h"
#include "sys_define_config.h"
/***************************************************************************/

extern const short int Sine_Table_50Hz[];
extern const short int Sine_Table_60Hz[];

/*************************************************
Description: Sys_Variableinit
Input      : 
Return     : 
Others     : 变量初始化
*************************************************/
void Sys_Variableinit(void)
{	
    INV_Info.mode_AC_Software_Ctrl  = INV_SOFTWARE_CTRL_SELECT;    
    INV_Info.SS_Step_Value          = INV_SOFTWARE_STEP_VALUE;//AC缓启动时，幅值系数每次步进值        
    INV_Info.SS_Shift_Value         = INV_SOFTWARE_SHIFT_VALUE;//AC缓启动时，幅值系数缩放移位值       
    INV_Info.SS_AMP_Target_Hold     = INV_SOFTWARE_HOLD_INIT;//放大后的幅值系数初始值 
    

    
    paralogiccontrol.masterslavebit = 2;		//默认既非主机也非从机，检测后确认本机状态    
    paralogiccontrol.synchronizbit = 0;    
    paralogiccontrol.slavesunprebit = 0;    
    paralogiccontrol.synsignalokbit = 0;   
    paralogiccontrol.capturecomebit = 0;   
    paralogiccontrol.masterslaverunbit = 0;
    Controllogic.ecomodeidbit = 0;
                                	  
    AD_Correct_V_ACOUT.Flag.all          = 0;
    AD_Correct_I_Induc.Flag.all          = 0;
    System_ProtectFlag_Info.all                  = 0;
    StartCheck_Flag_Info.all                     = 0;
    Lookup_Table_Speed.Table_Length              = Len_Power_Rpm;
    //Lookup_Table_Speed.Calc                      = &Lookup_Table_Calc; // 绑定 Lookup_Table_Calc() 速度查表 步进电机 / 功率曲线用的查表模块 (未启用)
		//--------------------------------------------------------------------------------------
    #if InvVol_Freq == InvVol_Freq_50Hz
		    Inv_SineCtrl_Info.SineTab_Ptr            = &Sine_Table_50Hz[0]; // 绑定 50Hz 正弦表_Sine_Ctrl_Info 结构里的指针指向正弦波数组,在 Inverter_AlgCtrl() 中 生成正弦参考
		    Inv_SineCtrl_Info.SineTab_PeriodPoint    = SPWMWAVE_DOT_50Hz;
		    Inv_SineCtrl_Info.InvVol_Dot_Reciprocal  = SPWMWAVE_DOT_RECIPROCAL_50Hz;
		    RepeatCtrl_Info.SPWM_Point               = SPWMWAVE_DOT_50Hz;
	  #else
		    Inv_SineCtrl_Info.SineTab_Ptr            = &Sine_Table_60Hz[0];
		    Inv_SineCtrl_Info.SineTab_PeriodPoint    = SPWMWAVE_DOT_60Hz;
		    Inv_SineCtrl_Info.InvVol_Dot_Reciprocal  = SPWMWAVE_DOT_RECIPROCAL_60Hz;
            RepeatCtrl_Info.SPWM_Point               = SPWMWAVE_DOT_60Hz;
	  #endif
		Inv_SineCtrl_Info.CurrLoop_Up_Limit          = 800;//CM_InvCurr_Up_Limit;
		Inv_SineCtrl_Info.CurrLoop_Dn_Limit          = -800;//CM_InvCurr_Down_Limit;
		Inv_SineCtrl_Info.InvVol_Omiga_UpLimit       = 4140;
		Inv_SineCtrl_Info.InvVol_Omiga_DownLimit     = 4050;
		Inv_SineCtrl_Info.InvVol_Omiga_MultiCalc     = 4096;
		Inv_SineCtrl_Info.Switch_Period              = PWM_PERIOD;
		Inv_SineCtrl_Info.Virtual_impedance_Coeff    = INV_VIRTUAL_RES;
		Inv_SineCtrl_Info.CloseOpen_Loop_Flag        = OPERATING_MODE;
		Inv_SineCtrl_Info.Single_Double_Flag         = CM_Inverter_Loop_Ctrl;
		Inv_SineCtrl_Info.Calc                       = &Inverter_AlgCtrl; // 绑定 Inverter_AlgCtrl()逆变调制计算入口 逆变器调制与控制计算
		//--------------------------------------------------------------------------------------
		#if ( OPERATING_MODE == NORMAL_MODE )		
            Inv_SineCtrl_Info.InvVol_AM_Target = 0;//INV_AC_VOL_AMP_VAL_NORMAL_REF;//
        #else
            Inv_SineCtrl_Info.InvVol_AM_Target = INV_AC_VOL_AMP_VAL_DEBUG_REF; 

        #endif	

		PID_Para_Init( &PID_Vol_Loop , &PID_Curr_Loop , Inv_Power_Model , InvVol_Type , CM_Inverter_Loop_Ctrl ); // 电压环 PID_Vol_Loop、电流环 PID_Curr_Loop 的 Calc 都指向 PID_Calc()
		Parallel_Para_Init( &PQ_Droop_Ctrl_Info , Inv_Power_Model , InvVol_Type , InvVol_Freq ,CM_Inverter_SelfOther_Parallel);
		RepeatCtrl_Para_Init( &RepeatCtrl_Info  , Inv_Power_Model , InvVol_Type , InvVol_Freq );
		#if  InvVol_Freq == InvVol_Freq_50Hz
            PowerCalc_Para_Init( &PQ_Calc_Info , SPWMWAVE_DOT_50Hz  );
		#else 
            PowerCalc_Para_Init( &PQ_Calc_Info , SPWMWAVE_DOT_60Hz );
		#endif
        
    INV_Power_Ctrl_Info.u8Mode_Select = INV_POWER_CTRL_SELECT;//限功率控制使能/失能模式选择
    INV_PID_Power.ctrl_Period_Val = Inv_SineCtrl_Info.SineTab_PeriodPoint;
}

/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/


