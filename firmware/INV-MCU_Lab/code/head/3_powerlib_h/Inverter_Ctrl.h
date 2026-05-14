/*****************************************************************************
@Description: Inverter_Ctrl.h 逆变器控制头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __INVERTER__CTRL_H__
#define __INVERTER__CTRL_H__


#define    SPWMDUTY_MAX_LIMIT                           (4000) //0.9716
#define    SPWMDUTY_MIN_LIMIT                           (-4000)  //-0.9716
//----------------------------------------------------------------------------------------------
// typedef struct
// {
//     unsigned short int  SineTab_Index;          // 正弦表当前索引，用于查表调制（步进位置）
//     unsigned short int  SineTab_PeriodPoint;    // 正弦表总点数（决定一周波长）
//     unsigned short int  InvVol_Out_Freq;        // 输出电压频率（Hz），根据正弦表索引增量计算
//                    int  InvVol_Peak_Ref;        // 电压环参考峰值（调制目标）
//                    int  InvVol_AM_Target;       // 电压幅值目标（目标电压幅值设定）
//                    int  InvVol_AM_BusCalc;      // 母线电压补偿后的电压幅值（Bus 电压影响修正）
//                    int  InvVol_AM_MultiCalc;    // 多环控制叠加后的电压幅值（考虑 Droop、PID 等）
//                    int  InvVol_Omiga_MultiCalc; // 多环控制叠加后的角频率（Ω = 2πf）
//                    int  InvVol_Omiga_UpLimit;   // 角频率上限（避免过高）
//                    int  InvVol_Omiga_DownLimit; // 角频率下限（避免过低）
//                    int  InvVol_SPWM_Period;     // SPWM 周期（PWM 计数周期）
//                    int  InvVol_SPWM_Compare_A;  // SPWM 通道 A 比较值（决定占空比）
//                    int  InvVol_SPWM_Compare_B;  // SPWM 通道 B 比较值（决定占空比）
//                    int  InvVol_SPWM_Duty;       // SPWM 调制输出占空比（双极性控制）
//                    int  InvVol_Dot_Reciprocal;  // 频率倒数点（用于调节采样/正弦表速率）
//                    int  Switch_Period;          // 开关周期（PWM 基准周期）
//                    int  BusVol_AD;              // 母线电压采样值（AD 转换结果）
//                    int  InvVol_Peak_AD;         // 输出电压峰值采样（AD 转换结果）
//                    int  InvCur_Peak_AD;         // 输出电流峰值采样（AD 转换结果）
//                    int  InvLoad_Peak_AD;        // 负载电流峰值采样（AD 转换结果）
//                    int  InvVol_rms;             // 输出电压有效值（RMS 计算结果）
//                    int  InvLoad_Irms;           // 输出电流有效值（RMS 计算结果）
//                    int  Virtual_impedance_Coeff;// 虚拟阻抗系数（改善动态特性）
//              short int  CloseOpen_Loop_Flag;    // 开环/闭环控制标志（0=闭环，1=开环）
//              short int  Single_Double_Flag;     // 单机/并机标志（0=单机，1=并机）
//                    int  CurrLoop_Up_Limit;      // 电流环上限（保护）
//                    int  CurrLoop_Dn_Limit;      // 电流环下限（保护）
//      const short int  *SineTab_Ptr;             // 指向正弦波查表数组（存放一周正弦波数据）
//                     void (*Calc)();             // 函数指针：指向计算函数（如 Inverter_AlgCtrl）
// }_Sine_Ctrl_Info;
typedef struct
{
  
	  unsigned short int  SineTab_Index;
	  unsigned short int  SineTab_PeriodPoint;
	  unsigned short int  InvVol_Out_Freq;
	                 int  InvVol_Peak_Ref;
	                 int  InvVol_AM_Target;
	                 int  InvVol_AM_BusCalc;
	                 int  InvVol_AM_MultiCalc;
	                 int  InvVol_Omiga_MultiCalc;
	                 int  InvVol_Omiga_UpLimit;
	                 int  InvVol_Omiga_DownLimit;
	                 int  InvVol_SPWM_Period;
	                 int  InvVol_SPWM_Compare_A;
		             int  InvVol_SPWM_Compare_B;
	                 int  InvVol_SPWM_Duty;
	                 int  InvVol_Dot_Reciprocal;
	                 int  Switch_Period;
	                 int  BusVol_AD;
	                 int  InvVol_Peak_AD;
	                 int  InvCur_Peak_AD;
	                 int  InvLoad_Peak_AD;
	                 int  InvVol_rms;
	                 int  InvLoad_Irms;
	                 int  Virtual_impedance_Coeff;
               short int  CloseOpen_Loop_Flag;
               short int  Single_Double_Flag;
                     int  CurrLoop_Up_Limit;
                     int  CurrLoop_Dn_Limit;								 
	     const short int  *SineTab_Ptr;
			        void      (*Calc)();          /* Pointer to calculation function */
}_Sine_Ctrl_Info;


extern _Sine_Ctrl_Info Inv_SineCtrl_Info;

extern void Inverter_AlgCtrl(_Sine_Ctrl_Info *InvAlg_Ctrl);


#endif
/*-------------------------------------------------------------------------------------
*  No more.
*------------------------------------------------------------------------------------*/
