/*****************************************************************************
@Description: user_fault_check.c 故障保护检查
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"          // 系统MCU相关头文件（时基/外设/状态机定义等） 
#include "sys_define_parar.h"        // 系统参数与保护结构体定义（Protect_Check_Var_t等）
/***************************************************************************/

unsigned int Protect_Release_Cnt  = 0;     // 保护释放计数，用于控制一定次数/时间窗口内的解保护尝试
extern uint16_t	u16_boardtemperature;      // 外板温度（外部或板载传感器），用于过温相关逻辑

/*************************************************
Description: User_Get_Protect_Flag
Input      : 
Return     : 
Others     : 故障保护判断逻辑
*************************************************/
void Get_The_Protect_Flag( int Check_Value, Protect_Check_Var_t * Check_Info,unsigned short int* protect_code )
{
    if (( *protect_code & Check_Info->FlagBit_position ) != E_FALSE )//如果此保护标志已经置位，则退出检查
    {
        return; // 一次锁存型：保护一旦触发置位，后续不重复判定（直到显式释放）
    }

    if ( Check_Info->Limit_Type == E_MAX )//阀值为最小值
    {
        if ( Check_Value >= Check_Info->Limit_Value )           // 上阈：达到或超过触发阈值
        {
            Check_Info->LED_Sparkle_Flag &= 0xFFF0;//0xFFFC;   // 点灯闪烁样式（低4位作为模式/计数）
            Check_Info->LED_Sparkle_Flag |= 0x0001;		     // 设置闪烁模式=1（仅作为人机提示）

            Check_Info->Limit_Value_Count++;                    // 连续计数（防抖/滤除瞬态）
            if ( Check_Info->Limit_Value_Count >= 0x0FFFFFFF ) Check_Info->Limit_Value_Count = 0x0FFFFFFF; // 饱和防溢出
            if ( Check_Info->Limit_Value_Count >= Check_Info->Limit_Value_Time ) // 连续达标达指定时间
            {
                Check_Info->Limit_Value_Count = 0;              // 触发后计数清零
                *protect_code |= Check_Info->FlagBit_position;  // 设置对应保护位为1（故障锁存）
            }
        }
        else if ( Check_Value < Check_Info->Limit_Hysteretic_Value )// 低于回差阈值 => 取消预警状态/清计数
        {
            Check_Info->LED_Sparkle_Flag &= 0xFFF0;//           // 关闭该类闪烁提示
            Check_Info->Limit_Value_Count = 0;                  // 退出防抖计数
        }
        // 介于回差与上阈之间：保持计数但不触发（迟滞区避免抖动）
    }
    /*------------------------------------------------------------------------------------*/
    else if ( Check_Info->Limit_Type == E_MIN )//阀值为最大值
    {
         if ( Check_Value <= Check_Info->Limit_Value )//达到保护阀值要求（下阈）
         {
            Check_Info->LED_Sparkle_Flag = E_TRUE;              // 点灯提示（下阈类通常为常亮或另一闪烁样式）
            Check_Info->Limit_Value_Count++;                    // 连续计数（防抖）
            if ( Check_Info->Limit_Value_Count >= 0x0FFFFFFF ) Check_Info->Limit_Value_Count = 0x0FFFFFFF; // 饱和
            if ( Check_Info->Limit_Value_Count >= Check_Info->Limit_Value_Time ) // 达到持续时间，触发
            {
                Check_Info->Limit_Value_Count  = 0;             // 清计数
                *protect_code |= Check_Info->FlagBit_position;	 // 设置保护位
            }
         }
         else if ( Check_Value > Check_Info->Limit_Hysteretic_Value )// 超过回差下限 => 退出预警，清计数
         {
            Check_Info->LED_Sparkle_Flag  = E_FALSE;            // 取消提示
            Check_Info->Limit_Value_Count = 0;                  // 清防抖计数
         }
         // 介于阈值与回差之间：保持原状态（迟滞区）
    }
    else
    {
     return;//错误处理（未知阈值类型，不做动作）
    }
}

/*************************************************
Description: Fault_State_Check
Input      : 
Return     : 
Others     : 故障保护功能启用
*************************************************/
uint16_t System_ProtectFlag_Info_First_Value,Temperature_Count = 0; // 首次故障代码锁存 & 温度计次

uint8_t Inv_Out_Short_Flag_Count = 0; // 启机阶段短路屏蔽计数（外部更新处应自增）
 int8_t Inv_Out_Short_Flag = 1;       // 1=屏蔽短路保护（启机阶段/未到判断条件）；0=允许短路保护

void Fault_State_Check(void)
{          
    #if (OPERATING_MODE == DEBUG_MODE)
        return;                                       // 调试模式下直接返回，跳过保护逻辑
    #endif 

    if ((SysClockBase_ms.faultCheck_1ms != E_TRUE) || // 1ms节拍未到
        (CM_RUN_STATE != State_Context.State_Identifier) || // 非运行状态（只在RUN态检查）
        (System_ProtectFlag_Info.all != 0)) return;   // 已有故障时不再重复触发（等待故障处理流程）

    /*-------------------------过载保护-----------------------------------------------------*/
    Get_The_Protect_Flag( System_Power_Info.Active_Power_Total , &INV_P_OLP1_Info   , &System_ProtectFlag_Info.all ); // OLP1：有功功率过载一级
    Get_The_Protect_Flag( System_Power_Info.Active_Power_Total , &INV_P_OLP2_Info   , &System_ProtectFlag_Info.all ); // OLP2：有功功率过载二级
    Get_The_Protect_Flag( System_Power_Info.Active_Power_Total , &INV_P_OLP3_Info , &System_ProtectFlag_Info.all );   // OLP3：有功功率过载三级
    Get_The_Protect_Flag( System_Power_Info.Active_Power_Total , &INV_P_OLP4_Info  , &System_ProtectFlag_Info.all );  // OLP4：有功功率过载四级（紧急）

//    /*-------------------------过流保护-----------------------------------------------------*/	  
    Get_The_Protect_Flag( ADSample_Info.Inv_Cur_RMS , &INV_Induc_OCP1_Info   , &System_ProtectFlag_Info.all ); // OCP1：RMS电流过流一级
    Get_The_Protect_Flag( ADSample_Info.Inv_Cur_RMS , &INV_Induc_OCP2_Info   , &System_ProtectFlag_Info.all ); // OCP2：RMS电流过流二级
    //Get_The_Protect_Flag( ADSample_Info.Inv_Cur_RMS , &INV_Induc_OCP3_Info , &System_ProtectFlag_Info.all ); // 预留三级
    //Get_The_Protect_Flag( ADSample_Info.Inv_Cur_RMS , &INV_Induc_OCP4_Info  , &System_ProtectFlag_Info.all );// 预留四级

    /*---------------------辅助电源过压保护---------------------------------------------------------------------*/
    Get_The_Protect_Flag( ADSample_Info.Power_AuxVcc_AD , &COM_AuxPower_OVP_Info , &System_ProtectFlag_Info.all ); // 辅助电源OVP
    /*---------------------辅助电源欠压保护---------------------------------------------------------------------*/
    Get_The_Protect_Flag( ADSample_Info.Power_AuxVcc_AD , &COM_AuxPower_OLP_Info , &System_ProtectFlag_Info.all ); // 辅助电源UVP
	  
    /*------------------------------------------------------------------------------------*/				
    /*---------------------启机时屏蔽短路信号50ms或电压大于100V启用短路保护---------------*/			 
    if(State_Context.State_Identifier <= 2)
        Inv_Out_Short_Flag = 1;//不在运行状态时，不启用短路保护（屏蔽）

    if( ADSample_Info.AC_Vol_RMS > INV_ACOUT_SCP_START_VAL ||\
        (Inv_Out_Short_Flag_Count >= 100)) //输出电压大于100V << 2 或者启机时输出电压一直低于5V超过50ms  清标志启用短路保护
        {
            Inv_Out_Short_Flag = 0;//清输出电压启机标志位（允许SCP判定）
        }
    /*------------------------------------------------------------------------------------*/		
    /*-------------------------输出短路保护-----------------------------------------------*/
    if( Inv_Out_Short_Flag == 0&&\
        (ADSample_Info.Inv_Cur_Peak>200||ADSample_Info.Inv_Cur_Peak<-200) )//短路启机标志清零后启用短路保护
    {
        Get_The_Protect_Flag( ADSample_Info.AC_Vol_RMS , &INV_ACOUT_SCP_Info , &System_ProtectFlag_Info.all );  	// SCP：输出端短路保护（以电压+峰值电流触发条件）
    }
    /*-------------------------输出电压过压保护-----------------------------------------------*/	  
    Get_The_Protect_Flag( ADSample_Info.AC_Vol_RMS , &INV_ACOUT_RMS_OVP_Info , &System_ProtectFlag_Info.all ); // 交流端RMS过压 
    /*-------------------------输出电压欠压---------------------------------------------------*/ 
    Get_The_Protect_Flag( ADSample_Info.AC_Vol_RMS , &INV_ACOUT_RMS_OLP_Info , &System_ProtectFlag_Info.all ); // 交流端RMS欠压

    /*-------------------------母线过压检测保护-----------------------------------------------*/	  
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD , &COM_VBUS_OVP1_Info   , &System_ProtectFlag_Info.all ); // VBUS OVP1
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD , &COM_VBUS_OVP2_Info   , &System_ProtectFlag_Info.all ); // VBUS OVP2
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD , &COM_VBUS_OVP3_Info   , &System_ProtectFlag_Info.all ); // VBUS OVP3

    /*-------------------------母线欠压检测保护-----------------------------------------------*/
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD  , &COM_VBUS_OLP1_Info   , &System_ProtectFlag_Info.all ); // VBUS UVP1
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD  , &COM_VBUS_OLP2_Info   , &System_ProtectFlag_Info.all ); // VBUS UVP2
    Get_The_Protect_Flag( ADSample_Info.Bus_Vol_AD  , &COM_VBUS_OLP3_Info   , &System_ProtectFlag_Info.all ); // VBUS UVP3

    /*-------------------------过欠速保护-------------------------------------------------------*/
    Get_The_Protect_Flag( s16_GeneratorSpeedAvg , &COM_RPM_Over , &System_ProtectFlag_Info.all ); // 发动机/发电机转速过高
    Get_The_Protect_Flag( s16_GeneratorSpeedAvg , &COM_RPM_Low  , &System_ProtectFlag_Info.all ); // 发动机/发电机转速过低

    /*-------------------------过温保护-------------------------------------------------------*/    
    Get_The_Protect_Flag( ADSample_Info.Temp_External_AD , &COM_OTP1_Info , &System_ProtectFlag_Info.all ); // 温度告警/一级过温
    Get_The_Protect_Flag( ADSample_Info.Temp_External_AD , &COM_OTP2_Info , &System_ProtectFlag_Info.all ); // 严重过温/停机级

    /*-------------------------VREF保护-------------------------------------------------------*/
    Get_The_Protect_Flag( ADSample_Info.ref_AD_Fir>>1 , &COM_VrefOver_Info , &System_ProtectFlag_Info.all ); // 参考电压过高（右移1作为等效标定）
    Get_The_Protect_Flag( ADSample_Info.ref_AD_Fir>>1 , &COM_VrefLow_Info  , &System_ProtectFlag_Info.all ); // 参考电压过低

    if(System_ProtectFlag_Info_First_Value == 0)
        System_ProtectFlag_Info_First_Value = System_ProtectFlag_Info.all; //记录第一次故障值（用于追溯首因）

    //清故障检测1ms 时钟标记
    SysClockBase_ms.faultCheck_1ms = 0;   // 归零后等待下一次1ms节拍
}

/*************************************************
Description: Fault_State_Deal
Input      : 
Return     : 
Others     : 
*************************************************/
void Fault_State_Deal(void)
{
    // 故障处理策略由上层状态机/运行处理决定，此处占位保持原样（不改变你的工程分层）
}

/*************************************************
Description: Protect_Release
Input      : 
Return     : 
Others     : 解除保护时初始化相关变量
*************************************************/

void Protect_Release(void)
{
    int i = 0;
    if ((( Protect_Release_Cnt < 20 ) && ( Key_Pro_Reset.Key_State == KEY_OFF )) || (System_ProtectFlag_Info_First_Value == 256 && Temperature_Count > 3000))
    {
        INV_SCR_ENABLE;//可控硅开启

        System_ProtectFlag_Info_First_Value = 0;//第一次故障值清零（解锁首次故障记录）
        Temperature_Count = 0;                   // 温度计数清零（用于温控释放条件）

        // ---------------- 并机/同步相关状态复位（解保护后重新判定主从/同步） ----------------
        paralogiccontrol.masterslavebit               = 2;		//默认既非主机也非从机，检测后确认本机状态
        paralogiccontrol.synchronizbit                = 0;      // 关闭同步标志
        paralogiccontrol.slavesunprebit               = 0;      // 从机预备标志清
        paralogiccontrol.synsignalokbit               = 0;      // 同步信号OK清
        paralogiccontrol.capturecomebit               = 0;      // 捕获有效清
        paralogiccontrol.masterslaverunbit            = 0;      // 主从运行位清
	
        Inv_SineCtrl_Info.InvVol_AM_MultiCalc         = 0;      // 电压幅值多源合成清零（防止解保护后输出突变）
        paralogiccontrol.slavestartokbit              = 0;      // 从机启动OK清

        System_ProtectFlag_Info.all                   = 0;      // 清所有保护位（真正解除）
        StartCheck_Flag_Info.all                      = 0;      // 启动过程检查相关标志清零
	      
        // ---------------- 保护条目计数器清零（每个保护项的“持续时间判据”复位） ----------------
        INV_P_OLP1_Info.Limit_Value_Count      = 0;
        INV_P_OLP2_Info.Limit_Value_Count      = 0;
        INV_P_OLP3_Info.Limit_Value_Count    = 0;
        INV_P_OLP4_Info.Limit_Value_Count     = 0;
        INV_ACOUT_SCP_Info.Limit_Value_Count          = 0;
        INV_Induc_OCP1_Info.Limit_Value_Count      = 0;
        INV_Induc_OCP2_Info.Limit_Value_Count      = 0;
//        INV_Induc_OCP3_Info.Limit_Value_Count    = 0;
//        INV_Induc_OCP4_Info.Limit_Value_Count     = 0;
        INV_ACOUT_RMS_OVP_Info.Limit_Value_Count           = 0;
        INV_ACOUT_RMS_OLP_Info.Limit_Value_Count            = 0;

        COM_VBUS_OVP1_Info.Limit_Value_Count            = 0;
        COM_VBUS_OVP2_Info.Limit_Value_Count            = 0;
        COM_VBUS_OVP3_Info.Limit_Value_Count            = 0;
        COM_VBUS_OLP1_Info.Limit_Value_Count            = 0;
        COM_VBUS_OLP2_Info.Limit_Value_Count            = 0;
        COM_VBUS_OLP3_Info.Limit_Value_Count            = 0;
        COM_RPM_Over.Limit_Value_Count           = 0;
        COM_RPM_Low.Limit_Value_Count            = 0;
        COM_OTP1_Info.Limit_Value_Count  = 0;
        COM_OTP2_Info.Limit_Value_Count  = 0;
        COM_AuxPower_OVP_Info.Limit_Value_Count        = 0;
        COM_AuxPower_OLP_Info.Limit_Value_Count         = 0;
        
        COM_StartCheck_VBUS_Up_Info.Limit_Value_Count   = 0; // 启机项：母线电压上升达标计数
        COM_StartCheck_VBUS_Dn_Info.Limit_Value_Count = 0;   // 启机项：母线电压下降达标计数
        COM_StartCheck_RPM_Info.Limit_Value_Count = 0;       // 启机项：转速达标计数
        COM_StartCheck_AuxPower_Up_Info.Limit_Value_Count   = 0; // 辅助电源上电达标
        COM_StartCheck_AuxPower_Dn_Info.Limit_Value_Count = 0;   // 辅助电源掉电达标
        COM_StartCheck_Temp_Up.Limit_Value_Count    = 0;     // 温度上限达标（用于启机温度自检）

        // ---------------- PI控制器积分清零（避免解保护后因积分残留导致输出跳变） ----------------
        PID_Vol_Loop.Ui                               = 0;
        PID_Curr_Loop.Ui                              = 0;

        // ---------------- 重复控制输出缓存清零（RPT控制的历史项清零） ----------------
        for ( i = 0; i < 384 ; i++ ) 
        {
            Repeat_Out_Buff[i] = 0;
        }			

        /*----------------------故障状态---->准备状态--------------------------------------*/
        Ready_Deal.HardWare_Check_Delay_Count         = 0;     // 硬件自检计时复位
        Waiting_Deal.Delay_Count                      = 0;     // 等待状态延时清零
        Waiting_Deal.flag.bit.Waiting_Ok              = E_FALSE; // 等待完成标志清
        Ready_Deal.flag.bit.Ready_Ok                  = E_FALSE; // 准备完成标志清
        Run_Deal.flag.bit.OpenDriver_Ok               = E_FALSE; // 驱动打开成功标志清
        State_Context.flag.bit.Fault_Release          = E_TRUE;  //错误保护释放标记（供状态机转移使用）
        /*------------------------------------------------------------------------------------*/
        if ( Protect_Release_Cnt <= 50 ) Protect_Release_Cnt++; // 释放计数上限50（防止无休止快速重复释放）
    }
}
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
