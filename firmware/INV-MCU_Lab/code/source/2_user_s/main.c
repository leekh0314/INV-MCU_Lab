/*****************************************************************************
@Description: main.c
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/
#include "sys_mcu_header.h"
#include "sys_state_machine.h"
#include "user_function.h"
/****************************************************************************/

__IO uint8_t system_1ms_count = 0;//1ms时钟周期在Main函数需两个周期，即 u16_System1msbit_count >=1
/*************************************************
Description: main
Input      : 
Return     : 
Others     : 主调用函数
*************************************************/

int main(void)
{
    //硬件配置初始化 
    Sys_HardConfigInit();
    while(1)
    {
        //获取状态机的状态            
        State_Context.ContextPtr( &State_Context );     

        //执行状态机对应的任务
        Sys_StateMachine_Task();
                
        //用户功能任务调用（LED、KEY、updata）
        User_CommonFunction();

        if(SysClockBase_ms.system_1ms == 1)    
            system_1ms_count ++;//1ms时钟周期在Main函数存在的周期计数

        if(system_1ms_count >= 2)
        {
            SysClockBase_ms.system_1ms = 0;		//1ms 时钟标记清零
            system_1ms_count =0;//1ms时钟周期在Main函数存在的周期计数值清零
        }
    }
   
}

