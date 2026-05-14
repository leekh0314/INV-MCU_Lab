/*****************************************************************************
@Description: user_sample.c 采样处理
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "sys_define_parar.h"
/***************************************************************************/


int16_t		s16_IuacquireVal;
int16_t		s16_UoutcquireVal = 0;
int16_t		s16_OutcapcurVal = 0;
int16_t		s16_OutvoldiffVal = 0;
int16_t		s16_OutcapcurValfil = 0;
int16_t		s16_UoutcquireValbak = 0;
int32_t		s32_UbuscquireVal = 0;

UNLONG_u				Unlongdcvolcomponentfilter;
UNLONG_u				Unlongdcvolcomponentfilsec;

/*************************************************
Description: COM_AuxPowerSample
Input      : 
Return     : 
Others     : 辅助电源电压采样
*************************************************/
void COM_AuxPowerSample(void)
{	
    //ANI11 电源电压采样 获取ANI11通道值
    ADC->ADS = ADC_CHANNEL_11; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Info.Power_AuxVcc_AD = ADC->ADCR;  // 读取12位A/D转换结果
}


/*************************************************
Description: COM_TempNTC_Sample
Input      : 
Return     : 
Others     : NTC温度采样
*************************************************/
void COM_TempNTC_Sample(void)
{
    //ANI0 散热器温度采样
    ADC->ADS = ADC_CHANNEL_0; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Info.Temp_External_AD = ADC->ADCR;  // 读取12位A/D转换结果	
}

/*************************************************
Description: COM_Vref_Sample
Input      : 
Return     : 
Others     : 基准电压2.5V采样
*************************************************/
void COM_Vref_Sample(void)
{
    //ANI9 基准电压采样
    ADC->ADS = ADC_CHANNEL_9; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Info.ref_AD = ADC->ADCR << 1;  // 读取12位A/D转换结果	
    ADSample_Info.ref_AD_Fir = (ADSample_Info.ref_AD_Fir * 1593 >> 12) + (ADSample_Info.ref_AD* 2503 >> 12);

    if( E_FALSE == AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok  && \
        ADSample_Info.Power_AuxVcc_AD>COM_AUX_POWER_LVP_VALUE_BACK)
    {
        AD_Correct_Vref.ADRef_Correct_Sum_Cnt++;
        AD_Correct_Vref.ADRef_Correct_Sum += ADSample_Info.ref_AD;
        if ( AD_Correct_Vref.ADRef_Correct_Sum_Cnt >= 512 )
        {
            AD_Correct_Vref.ADRef_Correct_Value = AD_Correct_Vref.ADRef_Correct_Sum >> 9;
            AD_Correct_Vref.ADRef_Correct_Sum_Cnt = 0;
            AD_Correct_Vref.ADRef_Correct_Sum     = 0;
            
            if (( AD_Correct_Vref.ADRef_Correct_Value <= AD_Correct_Vref.ADRef_Correct_Up_Limit   ) &&\
            ( AD_Correct_Vref.ADRef_Correct_Value >= AD_Correct_Vref.ADRef_Correct_Down_Limit ))
            {
                AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok = E_TRUE;
            }
        }
    } 
}

/*************************************************
Description: COM_Altern_Sample
Input      : 
Return     : 
Others     : 轮询采样温度和辅助电源
*************************************************/
void COM_Altern_Sample(void)
{
    static int8_t Altern_Cnt1,Altern_Cnt2 = 0;

    if(Altern_Cnt1 == 0 )
    {
        if(Altern_Cnt2 == 0 )
        {
            COM_TempNTC_Sample();//温度采样
            Altern_Cnt2 = 1;
        }
        else
        {
            COM_AuxPowerSample();//辅助电源电压采样   
            Altern_Cnt2 = 0;
        } 
        Altern_Cnt1 = 1;
    }
    else 
    {
        COM_Vref_Sample();//VREF电压采样   
        Altern_Cnt1 = 0;
    }       
}

/*************************************************
Description: COM_CurSample
Input      : 
Return     : 
Others     : 该处使用了电容电流重构技术
                     u(k) - u(k-1)
            ic = C * --------------
                          Ts
			 考虑到输出电压采样可能的干扰，重构后使用低通滤波器进行滤波处理
*************************************************/
#define CAP_VALUE   4700 //nF
#define CAP_dt      (CAP_VALUE*PWM_FREQ/1000*4096/1000000) //nF
void COM_CurSample(void)
{	
	
	//ANI15 输出电感电流采样  获取ANI15通道值
    ADC->ADS = ADC_CHANNEL_15; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Store_Info.InvCur_Store_One = ADC->ADCR << 1;  // 读取12位A/D转换结果

	//偏置值校正
    if( E_FALSE == AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok && \
        ADSample_Info.Power_AuxVcc_AD>COM_AUX_POWER_LVP_VALUE_BACK )
    {
        AD_Correct_I_Induc.ADRef_Correct_Sum_Cnt++;
        AD_Correct_I_Induc.ADRef_Correct_Sum += ADSample_Store_Info.InvCur_Store_One;				
        if ( AD_Correct_I_Induc.ADRef_Correct_Sum_Cnt >= 1024 )
        {
            AD_Correct_I_Induc.ADRef_Correct_Value   = AD_Correct_I_Induc.ADRef_Correct_Sum >> 10;
            AD_Correct_I_Induc.ADRef_Correct_Sum_Cnt = 0;
            AD_Correct_I_Induc.ADRef_Correct_Sum     = 0;
                  
            if (( AD_Correct_I_Induc.ADRef_Correct_Value <= AD_Correct_I_Induc.ADRef_Correct_Up_Limit   ) && \
                ( AD_Correct_I_Induc.ADRef_Correct_Value >= AD_Correct_I_Induc.ADRef_Correct_Down_Limit ))
            {
                AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok = E_TRUE;
            }	
        }
    }

    //电感电流数据处理
    s16_IuacquireVal = ADSample_Store_Info.InvCur_Store_One - AD_Correct_I_Induc.ADRef_Correct_Value;//AD_Correct_Vref.ADRef_Correct_Value;   
//    s16_IuacquireVal = ADSample_Store_Info.InvCur_Store_One - AD_Correct_Vref.ADRef_Correct_Value;                         
    
//    ADSample_Info.Inv_Cur_Peak = (ADSample_Info.Inv_Cur_Peak * 593 >> 12) + ((int32_t)s16_IuacquireVal * 3503 >> 12);	  //0.7us
    ADSample_Info.Inv_Cur_Peak = s16_IuacquireVal;	  //0.7us

    //负载电流数据处理
    s16_OutvoldiffVal = ADSample_Info.AC_Vol_Peak - s16_UoutcquireValbak;   
    s16_OutcapcurVal = ((int32_t)s16_OutvoldiffVal * CAP_dt) >> 12;// I = C*(dv/dt)
    s16_OutcapcurValfil = ((int32_t)s16_OutcapcurValfil * 1815 >> 12) + ((int32_t)s16_OutcapcurVal * 2281 >> 12);  //0.7us
    ADSample_Info.Load_Cur_Peak = ADSample_Info.Inv_Cur_Peak - s16_OutcapcurValfil;  //0.3us
    s16_UoutcquireValbak = ADSample_Info.AC_Vol_Peak;			
 

}  

/*************************************************
Description: COM_VolSample
Input      : 
Return     : 
Others     : 母线电压、输出电压采样及数据处理
*************************************************/
void COM_VolSample(void)
{

    //ANI3 逆变器输出交流电压采样
    ADC->ADS = ADC_CHANNEL_3; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Store_Info.InvVol_Store_One = ADC->ADCR << 1;  // 读取12位A/D转换结果
    
    if( E_FALSE == AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok &&\
        ADSample_Info.Power_AuxVcc_AD>COM_AUX_POWER_LVP_VALUE_BACK)
    {
        AD_Correct_V_ACOUT.ADRef_Correct_Sum_Cnt++;
        AD_Correct_V_ACOUT.ADRef_Correct_Sum += ADSample_Store_Info.InvVol_Store_One;
        if ( AD_Correct_V_ACOUT.ADRef_Correct_Sum_Cnt >= 1024 )
        {
            AD_Correct_V_ACOUT.ADRef_Correct_Value = AD_Correct_V_ACOUT.ADRef_Correct_Sum >> 10;
            AD_Correct_V_ACOUT.ADRef_Correct_Sum_Cnt = 0;
            AD_Correct_V_ACOUT.ADRef_Correct_Sum     = 0;
            
            if (( AD_Correct_V_ACOUT.ADRef_Correct_Value <= AD_Correct_V_ACOUT.ADRef_Correct_Up_Limit   ) &&\
                ( AD_Correct_V_ACOUT.ADRef_Correct_Value >= AD_Correct_V_ACOUT.ADRef_Correct_Down_Limit ))
            {
                AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok = E_TRUE;
            }
        }
    }
	//输出电压数据处理		
//    ADSample_Info.AC_Vol_Peak = ( ADSample_Info.AC_Vol_Peak * 596 >> 12) + (( ADSample_Store_Info.InvVol_Store_One - AD_Correct_Vref.ADRef_Correct_Value ) * 3500 >> 12);	
    ADSample_Info.AC_Vol_Peak =   ADSample_Store_Info.InvVol_Store_One - AD_Correct_V_ACOUT.ADRef_Correct_Value;//扣除零点偏置 消除直流偏置 电压采样是围绕 2.5 V 的中心（ADC 中点）进行的，真正的交流信号是 ±Δ 的摆动 20250923 注释
//    ADSample_Info.AC_Vol_Peak =   ADSample_Store_Info.InvVol_Store_One - AD_Correct_Vref.ADRef_Correct_Value;	

    //ANI10 BUS电压采样
    ADC->ADS = ADC_CHANNEL_10; //指定模拟输入通道号
    ADC->ADM0 |= ADCS; //允许转换运行
    while((ADC->ADM0 & ADCS) != 0);  //等待AD中断标记为1
    ADSample_Store_Info.BusVol_Store_One = ADC->ADCR;  // 读取12位A/D转换结果
    
	//母线电压数据处理		
	ADSample_Info.Bus_Vol_AD      =((uint32_t)ADSample_Store_Info.BusVol_Store_One); 
	s32_UbuscquireVal             = DFILTER(6,ADSample_Info.Bus_Vol_AD,s32_UbuscquireVal);
	ADSample_Info.Bus_Vol_Filiter_AD     =(s32_UbuscquireVal >> 16) ;    
}


/*************************************************
Description: User_EffectiveValue_Calc
Input      :  使用定义法计算输出电压有效值的平方    先平方   求和后求平均  最后开方
Return     : 
Others     : 没有进行开方运行是为了在满足性能与功能的基础上尽量减小时间开销
*************************************************/
void User_EffectiveValue_Calc( void )
{
    ADSample_Info.AC_Vol_Sum          += ( ADSample_Info.AC_Vol_Peak   * ADSample_Info.AC_Vol_Peak   ) >> 12;
    ADSample_Info.Inv_Cur_Sum         += ( ADSample_Info.Inv_Cur_Peak  * ADSample_Info.Inv_Cur_Peak  ) >> 6;
    ADSample_Info.Load_Cur_Sum        += ( ADSample_Info.Load_Cur_Peak * ADSample_Info.Load_Cur_Peak ) >> 6;	
    ADSample_Info.AC_Active_Power_Sum += ( ADSample_Info.AC_Vol_Peak   * ADSample_Info.Load_Cur_Peak ) >> 12;
}

/*************************************************
Description: Update_AC_Data
Input      : 
Return     : 
Others     : 有效值、功率数据处理
*************************************************/
int32_t s32_ActivePowerTotal_temp,s32_Apparent_Power_Total_temp; //有功功率和视在功率临时变量
void Update_AC_Data(void)
{
    uint32_t AC_Vol        = 0;
    uint32_t Inv_Cur       = 0;
    uint32_t Load_Curr 	   = 0;
    uint32_t AC_Vol_temp   = 0;
    uint32_t Inv_Cur_temp  = 0;
    uint32_t Load_Cur_temp = 0;
    
    //1ms更新一次电压、电流、功率有效值
	  // if ( 1 == SysClockBase_ms.system_1ms ) 
    if ( 1 == SysClockBase_ms.system_1ms ) // 注意:这里if判断之前是注释掉的 20250830
    {
        AC_Vol               = ( ADSample_Info.AC_Vol_Sum_Back   * Inv_SineCtrl_Info.InvVol_Dot_Reciprocal ) >> 15>>1;
        Inv_Cur              = ( ADSample_Info.Inv_Cur_Sum_Back  * Inv_SineCtrl_Info.InvVol_Dot_Reciprocal ) >> 11>>1;
        Load_Curr            = ( ADSample_Info.Load_Cur_Sum_Back * Inv_SineCtrl_Info.InvVol_Dot_Reciprocal ) >> 11>>1;
        System_Power_Info.Active_Power_Rpm      = ( ADSample_Info.AC_Active_Power_Back  * Inv_SineCtrl_Info.InvVol_Dot_Reciprocal ) >> 15>>1;// Inv_SineCtrl_Info.InvVol_Dot_Reciprocal频率倒数点（用于调节采样/正弦表速率）
                    
        AC_Vol_temp                = ( sqrt((AC_Vol * (COM_V_ACOUT_BASE * COM_V_ACOUT_BASE >>2)) ));//右移两位是读取AD值放大了2倍
        Inv_Cur_temp               = ( sqrt(Inv_Cur * (COM_I_INDUC_BASE * COM_I_INDUC_BASE >>8) ));
        Load_Cur_temp              = ( sqrt(Load_Curr * (COM_I_INDUC_BASE * COM_I_INDUC_BASE >>8) ));
        // 临时有功功率计算: P
        //267.3 * 5 / 2 * 20*5/2 = 33412
        s32_ActivePowerTotal_temp  = (( uint32_t)System_Power_Info.Active_Power_Rpm * (COM_V_ACOUT_BASE * COM_I_INDUC_BASE>>2) >> 12);              
        
        if(CM_RUN_STATE == State_Context.State_Identifier && ADSample_Info.Inv_Cur_RMS>259)
        {
            ADSample_Info.AC_Vol_RMS   = (AC_Vol_temp>>4)+0;//实际电压  >> 2 位 ;       
            ADSample_Info.Inv_Cur_RMS  = Inv_Cur_temp+0;//实际电流  >> 8 位  0.3A * 256 = 
            ADSample_Info.Load_Cur_RMS = Load_Cur_temp+0;//实际电流  >> 8 位     
            //有功功率一阶低通滤波
            System_Power_Info.Active_Power_Total    =10+  ((System_Power_Info.Active_Power_Total * 1293 + s32_ActivePowerTotal_temp * 2803 ) >> 12);//有功功率一阶低通滤波
            // 视在功率计算 P = U * I
            s32_Apparent_Power_Total_temp  = (((uint32_t)ADSample_Info.AC_Vol_RMS * ADSample_Info.Inv_Cur_RMS ) >> 2) >> 8;
            System_Power_Info.Apparent_Power_Total    =  0 +   ((System_Power_Info.Apparent_Power_Total * 2593 + s32_Apparent_Power_Total_temp * 1503 ) >> 12);//有功功率一阶低通滤波
          
        }
        else
        {
            ADSample_Info.AC_Vol_RMS   = (AC_Vol_temp>>4)+0;//实际电压  >> 2 位 ;       
            ADSample_Info.Inv_Cur_RMS  =  Inv_Cur_temp;//实际电流  >> 8 位  0.3A * 256 = 
            ADSample_Info.Load_Cur_RMS = Load_Cur_temp;//实际电流  >> 8 位    
            //有功功率一阶低通滤波
            System_Power_Info.Active_Power_Total    =     ((System_Power_Info.Active_Power_Total * 1293 + s32_ActivePowerTotal_temp * 2803 ) >> 12);//有功功率一阶低通滤波
            // 视在功率计算 P = U * I
            s32_Apparent_Power_Total_temp  = (((uint32_t)ADSample_Info.AC_Vol_RMS * ADSample_Info.Inv_Cur_RMS ) >> 2) >> 8;
            System_Power_Info.Apparent_Power_Total    =    ((System_Power_Info.Apparent_Power_Total * 2593 + s32_Apparent_Power_Total_temp * 1503 ) >> 12);//有功功率一阶低通滤波            
        }
        
        if(System_Power_Info.Active_Power_Total < 0)//有功功率不能为0
        {
            System_Power_Info.Active_Power_Total = 0;
        }
        
    }
}

/*------------------------------------------------------------------------------------*/
/*
函数名称: Update_AC_Data
关于 s32_ActivePowerTotal_temp 的系数由来与数据链路：
User_EffectiveValue_Calc() 将每个采样点的电压、电流峰值平方及瞬时有功（V_peak * I_peak）累加（user_sample.c:248-253）。
每个 SPWM 周期结束时，中断把累加和转存到 _Back 变量（interrupt.c:214-225），完成一个周期内的统计。
Update_AC_Data() 用正弦表点数的倒数 Inv_SineCtrl_Info.InvVol_Dot_Reciprocal（来自配置宏 SPWMWAVE_DOT_RECIPROCAL_*，见 sys_define_config.h:214-236）对累加值做平均，得到仍处于“ADC 数值 × 定点系数”的 System_Power_Info.Active_Power_Rpm（user_sample.c:266-271）。
COM_V_ACOUT_BASE = HW_ADC_REF × HW_V_ACOUT_GAIN 与 COM_I_INDUC_BASE = HW_ADC_REF × HW_I_INDUC_GAIN（sys_define_config.h:169-181）分别是将 ADC 码值换算到实际电压、电流所需的比例。式子s32_ActivePowerTotal_temp = ((uint32_t)System_Power_Info.Active_Power_Rpm * (COM_V_ACOUT_BASE * COM_I_INDUC_BASE >> 2)) >> 12;
就是在把“电压×电流”的无量纲平均值乘上电压/电流基值的乘积，再用 >>2 抵消前面电压采样左移 1 位造成的 4 倍放大，>>12 抵消在累加阶段（>>12）和平均阶段（>>15）引入的定点缩放，最终得到以瓦为单位的临时值。
System_Power_Info.Active_Power_Total 随后对 s32_ActivePowerTotal_temp 做一阶低通滤波，作为对外使用的有功功率；而 Active_Power_Rpm 本身作为中间量在下一次周期重新回写。
*/                                                                        
/*------------------------------------------------------------------------------------*/
