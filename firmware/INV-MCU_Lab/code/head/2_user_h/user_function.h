
/*****************************************************************************
@Description: user_function.h 用户功能头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef  __USER_FUNCTION_H
#define  __USER_FUNCTION_H

#include "BAT32G139.h"

 int User_Divider(int Divd, int Divs);
void INV_CloseDrive(void);//关闭驱动
void User_CommonFunction(void);
void User_UART_View(void);
void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4);
#endif	

