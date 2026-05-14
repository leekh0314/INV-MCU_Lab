/*****************************************************************************
@Description: stepmotor_ctrl.c 步进电机控制
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/
#ifndef __STEPMOTOR_CTRL_H_
#define __STEPMOTOR_CTRL_H_

#include "stdint.h"
#include "sys_define_config.h"

extern  unsigned short int Step_Driver_Signal_Buff[9];

#define Len_Power_Rpm		                                    (6)
#define CM_Pcv_BASE                                         (131)
#define P_GQ                                                (6)
//步进电机控制输出IO宏定义变量
#define StepA  PORTCbits.RC6   //宏定义输出控制 RC6变量（步进电机驱动A）
#define StepB  PORTCbits.RC7   //宏定义输出控制 RC7变量（步进电机驱动B）
#define StepC  PORTCbits.RC8   //宏定义输出控制 RC8变量（步进电机驱动C）
#define StepD  PORTCbits.RC9   //宏定义输出控制 RC9变量（步进电机驱动D）

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
#define  Motor_Step_Driver_Freq                             (2)
#define  CM_Engine_Pole_Pairs                               (9)
#define  CM_RPM_BASE_Value                                  (4600)
#define  CM_RPM_BASE                                        (114)  //4600   (4096 / 4600 * 128 = 114)
#define  R_GQ                                               (7)
#define  Protect_Idle_Rpm                                   ((2850L*CM_RPM_BASE)>>R_GQ)   //3720
#define  CM_Engine_Speed_RefMAX                             (( 4000UL * CM_RPM_BASE ) >> R_GQ)	//参考转速最大值限制
//13333
//   RPM =  (60 * f) / 9极对数
#define ECAP_VALUE_FREQ_LIMIT_UP                            (22200)//300hz   2571RPM     8M  1/8us      
#define ECAP_VALUE_FREQ_LIMIT_DOWN                          (12428)  // 	(1000000/(12428/8))*60/9  700Hz时的周期数       6000rpm      
/*-----------------------------------------------------------------------------*/
//#define CM_Engine_Speed_RefMAX  ((4000UL * CM_RPM_BASE) >> R_GQ) // 转速参考最大值

#define  Motor_Step_kp                                      (280)
#define  Motor_Step_ki                                      (55)
#define  Motor_Step_kd                                      (5)
#define  Motor_Period_Value                                 (170)
//#define Motor_Step_kp           (280)       // 比例系数
//#define Motor_Step_ki           (55)        // 积分系数
//#define Motor_Step_kd           (5)         // 微分系数
//#define Motor_Period_Value      (170)       // 控制周期值
/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
#define  Motor_Step_ErrMax                                  (1500)
#define  Motor_Step_ErrMin                                  (-1500)
#define  Motor_Step_OutMax                                  (100)//60
#define  Motor_Step_OutMin                                  (-100)//-60
#define  Motor_Steady_State_Dot                             (4000)

//#define Motor_Step_ErrMax       (1500)      // 误差最大值
//#define Motor_Step_ErrMin       (-1500)     // 误差最小值
//#define Motor_Step_OutMax       (100)       // 输出最大值
//#define Motor_Step_OutMin       (-100)      // 输出最小值

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
#define  Forward_Driver_Motor                               (0) // 正转
#define  Inverse_Driver_Motor                               (1) // 反转
/*------------------------------------------------------------------------------*/
#define  Motor_Step_Static_MidNum                           (5000)
#define  Motor_Step_Num                                     (8)
#define  Motor_Step_Driection                               Forward_Driver_Motor    // 正转
/*------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
typedef struct { 
                    int  ref;               
                    int  fdb;               
                    int  err;                   
                    int  err_n;                  
                    int  err_n2;
                    int  kp;                        
                    int  ki;                       
                    int  kd;                                        
                    int  errMax;                   
                    int  errMin;                      
                    int  outMax;               
                    int  outMin;              
                    int  out;                
              short int  stepNum;
              short int  stepDirection;
              short int  stepIndex;
              short int  stepStaticMidNum;
              short int  stepActiveNum;
     unsigned short int  periodCount;
     unsigned short int  periodValue;

     unsigned short int *Motor_Driver_Signal_Ptr;
  volatile unsigned int *Motor_Driver_Hard_Set_Ptr[1];
  volatile unsigned int *Motor_Driver_Hard_Clear_Ptr;
                void   (*PI_Calc)();	                // Pointer to calculation function
} StepMotor_Ctrl_t;

//typedef struct { 
//    int  ref;               // 转速参考值（给定值）
//    int  fdb;               // 转速反馈值（实际测量值）
//    int  err;               // 当前误差（ref - fdb）
//    int  err_n;             // 上次误差
//    int  err_n2;            // 上上次误差
//    int  kp;                // 比例系数
//    int  ki;                // 积分系数
//    int  kd;                // 微分系数
//    int  errMax;            // 误差上限
//    int  errMin;            // 误差下限
//    int  outMax;            // 输出上限
//    int  outMin;            // 输出下限
//    int  out;               // PID输出结果
//    short int  stepNum;     // 步进电机步数
//    short int  stepDirection; // 步进电机方向
//    short int  stepIndex;   // 步进电机索引
//    short int  stepStaticMidNum; // 静态中值
//    short int  stepActiveNum; // 活跃步数
//    unsigned short int  periodCount; // 周期计数
//    unsigned short int  periodValue; // 周期值
//    unsigned short int *Motor_Driver_Signal_Ptr; // 电机驱动信号指针
//    volatile unsigned int *Motor_Driver_Hard_Set_Ptr[1]; // 硬件设置指针
//    volatile unsigned int *Motor_Driver_Hard_Clear_Ptr; // 硬件清除指针
//    void   (*PI_Calc)();    // PID计算函数指针
//} StepMotor_Ctrl_t;

typedef StepMotor_Ctrl_t *StepMotor_handle;


/*-----------------------------------------------------------------------------
Default initalizer for the PARK object.
-----------------------------------------------------------------------------*/

#define Step_Motor_DEFAULTS { 0,                        \
                              0,                        \
                              0,                        \
                              0,                        \
                              0,                        \
                              Motor_Step_kp,         \
                              Motor_Step_ki,         \
                              Motor_Step_kd,         \
                              Motor_Step_ErrMax,     \
                              Motor_Step_ErrMin,     \
                              Motor_Step_OutMax,     \
                              Motor_Step_OutMin,     \
                              0,                        \
                              Motor_Step_Num,           \
                              Motor_Step_Driection,     \
                              0,                        \
                              Motor_Step_Static_MidNum, \
                              0,                        \
                              Motor_Period_Value,    \
                              0,                        \
                              &Step_Driver_Signal_Buff[0], \
                              {(void *)0},                \
                              (void *)0,                \
					(void (*)( unsigned int ))StepMotor_PI_Ctrl} // 绑定步进电机PI调节控制函数
                              

/*------------------------------------------------------------------------------
Prototypes for the functions in PARK.C
------------------------------------------------------------------------------*/
void StepMotor_PI_Ctrl(StepMotor_handle);


extern void Engine_Rpm_Ctl(void); //发动机转速控制函数Engine_Rpm_Ctl 仅在头文件声明，当前版本未实现，遗留接口
extern StepMotor_Ctrl_t StepMotor_Ctrl;
extern volatile uint8_t* Step_Driver_Hardware_Buff[];

#endif /* SETP_CTRL_H_ */
/*-------------------------------------------------------------------------------------
 *  No more.
 *------------------------------------------------------------------------------------*/
