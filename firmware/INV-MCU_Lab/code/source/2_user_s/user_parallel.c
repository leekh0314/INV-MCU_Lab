/*****************************************************************************
@Description: user_parallel.c 并机控制
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
/***************************************************************************/

volatile Parainverterslogic		paralogiccontrol;

int16_t						s16_ParaslavetomastrVal = 722;
int16_t						s16_Capture_Vol_Fre = 0;
int16_t						s16_CaptureoldVal = 0;
int16_t						s16_CapturefreVal = 0;
uint16_t					u16_CapturetimeCnt = 0;

#if InvVol_Freq == InvVol_Freq_50Hz
    //输出频率换算     50HZ   = 20.0ms    20.0ms/52us = 384   50Hz
    #define     s16_CapturetimeMaxlim  427//45Hz
    #define     s16_CapturetimeUplim   409//47Hz
    #define     s16_CapturetimeDnlim   363//53Hz
#else
    //输出频率换算     60HZ   = 16.666ms    16.666ms/52us = 320   60Hz
    #define     s16_CapturetimeMaxlim  349//55Hz     
    #define     s16_CapturetimeUplim   331//58Hz
    #define     s16_CapturetimeDnlim   310//62Hz
#endif

int8_t						s8_CaptureinCnt = 0;
int8_t						s8_CaplowlevelCnt = 0;
int16_t						s16_MasterslaveCnt = 0;
int16_t						s16_MasterslaveidVal = 483;			//
int16_t						s16_MasterslaveidDclim = 625;       //4V
int16_t						s16_MasterslaverunCnt = 0;


/*************************************************
Description: Masterslavecheck
Input      : 
Return     : 
Others     : 根据逆变器开启前外部输出电压确认该机为主机还是从机，将输出电压直流
             分量加入判据目的有二，一是防止因为外部残压导致误判，二是确认外部电
             压基本波形正常，以防故障时并机
*************************************************/
void Masterslavecheck(void)
{
    int16_t tmp;
    if (SysClockBase_ms.system_1ms == 1)
    {
        if (paralogiccontrol.masterslavebit == 2)
        {
            tmp = ABSFUN(Unlongdcvolcomponentfilsec.half.hWord);  
            if ((ADSample_Info.AC_Vol_RMS > s16_MasterslaveidVal) && (tmp < s16_MasterslaveidDclim))		//输出电压有效值大于30V，直流分量(残压)小于2V
            {
                s16_MasterslaveCnt++;
            }
            else
            {
               s16_MasterslaveCnt--;
            }
  
            if (s16_MasterslaveCnt > 500 ) 			//确认2s
            {
                paralogiccontrol.masterslavebit = 0; 		//从机
                s16_MasterslaveCnt = 0;						//计数器复位
            }

            if (s16_MasterslaveCnt < -500 )   //500ms
            {
               paralogiccontrol.masterslavebit = 1; 		//主机
               paralogiccontrol.masterslaverunbit = 1;
               s16_MasterslaveCnt = 0;				    	//计数器复位
            }
        }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
        if ((paralogiccontrol.masterslavebit == 0) && (paralogiccontrol.masterslaverunbit == 0))	//从机，等待主机完全启动后再启动从机
        {
            s16_MasterslaverunCnt++;
            if (s16_MasterslaverunCnt > 50)														//确认五个周波
            {
                s16_MasterslaverunCnt = 0;
                paralogiccontrol.slavesunprebit = 1;
            } 
            if (paralogiccontrol.slavesunprebit == 1)												//从机捕获到工频同步信号后启动
            {
                if(paralogiccontrol.synsignalokbit == 1)												//确认工频同步信号有效
                {
                    paralogiccontrol.masterslaverunbit = 1;
                }
            }
        }
    }
}

/*************************************************
Description: INV_ACOUT_FreCalc
Input      : 
Return     : 
Others     : 比较器计算输出电压频率   输出电压与 2.5V 比较输出连续方波，计算其频率
*************************************************/
uint8_t flag = 0;//采集周期标记 ，同时计数值清零
int16_t TimeCntVal = 0;//周期计数值
extern volatile uint8_t Cmp0_Interrupt_Flag_temp;
void  INV_ACOUT_FreCalc(void)
{
    //(CMP->COMPMDR) & 0X08 == 0;//表示VCIN0＜比较器0的基准电压，或者比较器0停止运行，该状态位0。  _00_COMP0_FLAG_REFERENCE_0
    //(CMP->COMPMDR) & 0X08 == 0X08;// 表示VCIN0＞比较器0的基准电压，该状态位值为1。  _08_COMP0_FLAG_REFERENCE_1

    if((((CMP->COMPMDR) & 0X08) == 0X08) && flag == 0)//比较器0中断标志状态值 	
    {
        TimeCntVal = u16_CapturetimeCnt;
        flag = 1;//采集周期标记 ，开始计数
        u16_CapturetimeCnt = 0;

    }
    else if((((CMP->COMPMDR) & 0X08) == 0) && flag == 1) // 比较器0状态值    
    {    
        flag = 0;
    }


    paralogiccontrol.synchronizbit = 0;
    u16_CapturetimeCnt++;//时间计数   表示多少个PWM周期
    if (u16_CapturetimeCnt > s16_CapturetimeMaxlim)
    {
        TimeCntVal = 0;
        u16_CapturetimeCnt =0;
    }

    
    s16_Capture_Vol_Fre = TimeCntVal;
    if (s16_Capture_Vol_Fre < 0)
    {
        s16_Capture_Vol_Fre += s16_CapturetimeMaxlim;
    }
    if((s16_Capture_Vol_Fre < s16_CapturetimeUplim) && (s16_Capture_Vol_Fre > s16_CapturetimeDnlim))  //55HZ-65Hz
    {
        paralogiccontrol.synchronizbit = 1;
        paralogiccontrol.synsignalokbit = 1;
    }
    else
    {
        paralogiccontrol.synsignalokbit = 0; 
        paralogiccontrol.synchronizbit = 0;
    }
}






