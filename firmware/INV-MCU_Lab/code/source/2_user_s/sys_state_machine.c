/*****************************************************************************
@Description: sys_state_machine.c
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "user_function.h"
#include "user_sample.h"
/***************************************************************************/

_State_Context       State_Context = STATE_CONTEXT_DEFAULTS;
_Initial_Deal        Initial_Deal  = INITIAL_DEAL_DEFAULTS;
_Waiting_Deal        Waiting_Deal  = WAITING_DEAL_DEFAULTS;
_Ready_Deal          Ready_Deal    = READY_DEAL_DEFAULTS;
_Run_Deal            Run_Deal      = RUN_DEAL_DEFAULTS;
_Fault_Deal          Fault_Deal    = FAULT_DEAL_DEFAULTS;
_Stop_Deal           Stop_Deal     = STOP_DEAL_DEFAULTS;


/*************************************************
Description: Initial_Deal_Func
Input      : 
Return     : 
Others     : 初始态功能函数
*************************************************/
void Initial_Deal_Func(_Initial_Deal *Initial_Deal_Handle)
{
	//Initial_Deal_Handle->HwInit_FuncPtr(&Initial_Deal_Handle);
	Initial_Deal_Handle->SwInit_FuncPtr(&Initial_Deal_Handle);
	Waiting_Deal.Delay_Count              = 0;
	Ready_Deal.HardWare_Check_Delay_Count = 0;
	Initial_Deal_Handle->flag.bit.Initial_Ok = E_TRUE;
}

/*************************************************
Description: Waiting_Deal_Func
Input      : 
Return     : 
Others     : 等待态功能函数
*************************************************/
void Waiting_Deal_Func( _Waiting_Deal *Waiting_Deal_Handle )
{
    if ( 1 == SysClockBase_ms.system_1ms )	 
    {
        Waiting_Deal_Handle->Delay_Count++;
        if ( Waiting_Deal_Handle->Delay_Count > Waiting_Deal_Handle->Delay_Value )
            Waiting_Deal_Handle->Delay_Count = Waiting_Deal_Handle->Delay_Value;
    }

    if ( Waiting_Deal_Handle->Delay_Count >= Waiting_Deal_Handle->Delay_Value )
    {
        Ready_Deal.HardWare_Check_Delay_Count = 0;
        Waiting_Deal_Handle->flag.bit.Waiting_Ok = E_TRUE;
    }
}

/*************************************************
Description: Ready_Deal_Func
Input      : 
Return     : 
Others     : 准备态功能函数
*************************************************/
void Ready_Deal_Func(_Ready_Deal *Ready_Deal_Handle)
{
    Ready_Deal_Handle->Rdy_Check_FuncPtr(&Ready_Deal_Handle);
}

/*************************************************
Description: Ready_State_Check
Input      : 
Return     : 
Others     : 准备态自检功能函数
*************************************************/
void Ready_State_Check(void)
{
    Masterslavecheck();							   //主从机识别
    if ( 1 == SysClockBase_ms.system_1ms )
    {
        Ready_Deal.HardWare_Check_Delay_Count++;
        if ( Ready_Deal.HardWare_Check_Delay_Count > Ready_Deal.HardWare_Check_Delay_Value_Max )
        Ready_Deal.HardWare_Check_Delay_Count = Ready_Deal.HardWare_Check_Delay_Value_Max;
        
        Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD , &COM_StartCheck_VBUS_Up_Info   , &StartCheck_Flag_Info.all );
        Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD , &COM_StartCheck_VBUS_Dn_Info , &StartCheck_Flag_Info.all );
        Get_The_Protect_Flag( s16_GeneratorSpeedAvg            , &COM_StartCheck_RPM_Info , &StartCheck_Flag_Info.all );
        Get_The_Protect_Flag( ADSample_Info.Power_AuxVcc_AD    , &COM_StartCheck_AuxPower_Up_Info   , &StartCheck_Flag_Info.all );
        Get_The_Protect_Flag( ADSample_Info.Power_AuxVcc_AD    , &COM_StartCheck_AuxPower_Dn_Info , &StartCheck_Flag_Info.all );
        Get_The_Protect_Flag( ADSample_Info.Temp_External_AD    , &COM_StartCheck_Temp_Up , &StartCheck_Flag_Info.all );

    }

    paralogiccontrol.masterslaverunbit = 1;
    paralogiccontrol.masterslavebit = 1;//PASS
    //---------------------------------------------------------------------------------------------
    if (( E_TRUE == paralogiccontrol.masterslaverunbit               ) && \
        ( E_TRUE == StartCheck_Flag_Info.bit.RPM_Up_OK      ) && \
        ( E_TRUE == StartCheck_Flag_Info.bit.auxPower_Up_OK   ) && \
        ( E_TRUE == StartCheck_Flag_Info.bit.auxPower_Dn_OK ) && \
        ( E_TRUE == StartCheck_Flag_Info.bit.temp_Up_OK ) && \
        ( E_TRUE == AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok ) && \
        ( E_TRUE == AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok ) && \
        ( E_TRUE == AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok ) )
    {
        if(( E_TRUE == StartCheck_Flag_Info.bit.vBus_Up_OK   ) && \
           ( E_TRUE == StartCheck_Flag_Info.bit.vBus_Dn_OK ))
        {
            if ( 1 == paralogiccontrol.masterslavebit )      //主机
            {
                Inv_SineCtrl_Info.SineTab_Index              = 0;
                Ready_Deal.flag.bit.Ready_Ok                 = E_TRUE;
                Ready_Deal.HardWare_Check_Delay_Count        = 0;
                System_ProtectFlag_Info.bit.System_Init_Fail = E_FALSE;
            }
            //------------------------------------------------------------------
            else if ( 0 == paralogiccontrol.masterslavebit )  //从机
            {
                if (( 1 == paralogiccontrol.synchronizbit ) && ( 0 == paralogiccontrol.slavestartokbit ))
                {
                    if ( ADSample_Info.AC_Vol_RMS > s16_ParaslavetomastrVal)		 // 从机当外部输出电压有效值大于110V时启动
                    {
                        Inv_SineCtrl_Info.SineTab_Index               =  2;
                        paralogiccontrol.slavestartokbit              =  1;				//从机启动完成
                        Ready_Deal.flag.bit.Ready_Ok                  =  E_TRUE;
                        System_ProtectFlag_Info.all                  &= ~E_Sys_Init_Fail;
                        Ready_Deal.HardWare_Check_Delay_Count         =  0;
                    }
                }
            }
        }
    }
    else
    {
        if ( Ready_Deal.HardWare_Check_Delay_Count >= Ready_Deal.HardWare_Check_Delay_Value_Max )
        {
            Ready_Deal.flag.bit.Ready_Ok                  = E_FAIL;
            Ready_Deal.HardWare_Check_Delay_Count         = 0;
            System_ProtectFlag_Info.all                  |= E_Sys_Init_Fail;
        }
    }
}

/*************************************************
Description: Run_State_OpenDriver
Input      : 
Return     : 
Others     : 运行态开驱动功能函数
*************************************************/
void Run_State_OpenDriver(void)
{
    PID_Vol_Loop.Ui                         = 0;
    PID_Curr_Loop.Ui                        = 0;
    PQ_Calc_Info.Array_Index                = Inv_SineCtrl_Info.SineTab_Index;
    User_PWM_Enable();//打开PWM
}

/*************************************************
Description: Run_Deal_Func
Input      : 
Return     : 
Others     : 运行态功能函数
*************************************************/
void Run_Deal_Func( _Run_Deal *Run_Deal_Handle )
{
    if ( E_FALSE == Run_Deal_Handle->flag.bit.OpenDriver_Ok )
    {
        Run_Deal_Handle->OpenDriver_FuncPtr(&Run_Deal_Handle);
        Run_Deal_Handle->flag.bit.OpenDriver_Ok = E_TRUE;
    }
    Run_Deal_Handle->FaultStateCheck_FuncPtr(&Run_Deal_Handle);
    Run_Deal_Handle->FaultStateDeal_FuncPtr(&Run_Deal_Handle);
}

/*************************************************
Description: Fault_Deal_Func
Input      : 
Return     : 
Others     : 故障态功能函数
*************************************************/
void Fault_Deal_Func(_Fault_Deal *Fault_Deal_Handle)
{
	Fault_Deal_Handle->FaultStateCheck_FuncPtr(&Fault_Deal_Handle);
	Fault_Deal_Handle->FaultRelease_FuncPtr(&Fault_Deal_Handle);
}

/*************************************************
Description: Stop_Deal_Func
Input      : 
Return     : 
Others     : 停止态功能函数
*************************************************/
void Stop_Deal_Func(_Stop_Deal *Stop_Deal_Handle)
{
}

/*************************************************
Description: State_Context_Func
Input      : 
Return     : 
Others     : 状态机逻辑判断函数
*************************************************/
int16_t temp_test = 0;
void State_Context_Func( _State_Context *State_Context_Handle )
{
    switch( State_Context_Handle->State_Identifier )
    {
       case CM_INITIAL_STATE:

    	   if ( E_TRUE == State_Context_Handle->flag.bit.Initial_Ok )
       	   {
       	      State_Context_Handle->State_Identifier = CM_WAITING_STATE;
       	   }
       break;
    /*----------------------------------------------------------------------------------*/
       case CM_WAITING_STATE:
           
           if ( E_TRUE == State_Context_Handle->flag.bit.Waiting_Ok )
       	   {
           	  State_Context_Handle->State_Identifier = CM_READY_STATE;
           	  State_Context_Handle->flag.bit.Waiting_Ok = E_FALSE;
       	   }
           
       break;
    /*----------------------------------------------------------------------------------*/
       case CM_READY_STATE:
           if ( E_TRUE == State_Context_Handle->flag.bit.Stop_Instruction )
           {
              State_Context_Handle->State_Identifier = CM_STOP_STATE;
           }
       	   else if ( E_TRUE == State_Context_Handle->flag.bit.Ready_Ok )
       	   {
       	      State_Context_Handle->State_Identifier = CM_RUN_STATE;
       	   }
       	   else  if ( E_FAIL == State_Context_Handle->flag.bit.Ready_Ok )
       	   {
       	      State_Context_Handle->State_Identifier = CM_FAULT_STATE;
       	   }
       break;
    /*----------------------------------------------------------------------------------*/
       case CM_RUN_STATE:
           
    	   State_Context_Handle->flag.bit.Ready_Ok = E_FALSE;
    	   if ( E_TRUE == State_Context_Handle->flag.bit.Stop_Instruction )
           {
              State_Context_Handle->State_Identifier = CM_STOP_STATE;
           }
     	   else if ( E_TRUE == State_Context_Handle->flag.bit.Fault_Occur )
     	   {
     	      State_Context_Handle->State_Identifier = CM_FAULT_STATE;
     	   }
       break;
    /*----------------------------------------------------------------------------------*/
       case CM_FAULT_STATE:   
            
       
    	   State_Context_Handle->flag.bit.Ready_Ok = E_FALSE;
    	   if ( E_TRUE == State_Context_Handle->flag.bit.Stop_Instruction )
           {
              State_Context_Handle->State_Identifier = CM_STOP_STATE;
           }
   	       else if ( E_TRUE == State_Context_Handle->flag.bit.Fault_Release )
   	       {
   	          State_Context_Handle->State_Identifier = CM_WAITING_STATE;
   	          State_Context_Handle->flag.bit.Fault_Occur = E_FALSE;
   	          State_Context_Handle->flag.bit.Fault_Release = E_FALSE;
   	       }
           
       break;
    /*----------------------------------------------------------------------------------*/
       case CM_STOP_STATE:          
    	   State_Context_Handle->flag.bit.Ready_Ok = E_FALSE;
    	   State_Context_Handle->flag.bit.Waiting_Ok = E_FALSE;
       break;
    /*----------------------------------------------------------------------------------*/
       default:
       break;
    }
}

/*************************************************
Description: Sys_StateMachine_Task
Input      : 
Return     : 
Others     : 执行系统状态机对应的任务
*************************************************/
void Sys_StateMachine_Task(void)
{
    switch( State_Context.State_Identifier )
    {

        case CM_INITIAL_STATE:
            //Initial_Deal.HwInit_FuncPtr = &Sys_HardConfigInit;//外设模块初始化               
            Initial_Deal.SwInit_FuncPtr = &Sys_Variableinit;

            State_Context.FuncPtr[CM_INITIAL_STATE]( &Initial_Deal );
            State_Context.flag.bit.Initial_Ok = Initial_Deal.flag.bit.Initial_Ok;						
        break;
        
        case CM_WAITING_STATE:
            INV_SCR_ENABLE;//使能可控硅
            State_Context.FuncPtr[CM_WAITING_STATE]( &Waiting_Deal );
            State_Context.flag.bit.Waiting_Ok = Waiting_Deal.flag.bit.Waiting_Ok;
        break;
        
        case CM_READY_STATE:
            Waiting_Deal.flag.bit.Waiting_Ok = E_FALSE;
            #if OPERATING_MODE == NORMAL_MODE
                State_Context.FuncPtr[CM_READY_STATE]( &Ready_Deal );
                State_Context.flag.bit.Ready_Ok = Ready_Deal.flag.bit.Ready_Ok;						        
            #else
                State_Context.flag.bit.Ready_Ok = E_TRUE;
            #endif
        break;
        
        case CM_RUN_STATE:
            State_Context.FuncPtr[CM_RUN_STATE]( &Run_Deal );
        break;
        
        case CM_FAULT_STATE:

            INV_SCR_DISABLE;//关闭可控硅
            State_Context.FuncPtr[CM_FAULT_STATE]( &Fault_Deal );
            Run_Deal.flag.bit.OpenDriver_Ok    = E_FALSE;
        break;
        
        case CM_STOP_STATE:
            State_Context.FuncPtr[CM_STOP_STATE]( &Stop_Deal );
        break;
        
        default:
        break;
    }

}
