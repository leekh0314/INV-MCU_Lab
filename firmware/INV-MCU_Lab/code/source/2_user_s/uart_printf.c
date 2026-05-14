/*****************************************************************************
@Description: uart_printf.c 串口打印重定向
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/
//#include <stdio.h>
//#include <rt_misc.h>
//#include <stdint.h>
//#include "BAT32G139.h"
//#include "userdefine.h"
//#include "sci.h"

//#if defined ( __CC_ARM   )
//#if (__ARMCC_VERSION < 400000)
//#else
//// Insist on keeping widthprec, to avoid X propagation by benign code in C-lib
//#pragma import _printf_widthprec
//#endif
//#endif

////
//// C library retargetting
////

//struct __FILE { int handle; /* Add whatever you need here */ };
//FILE __stdout;
//FILE __stdin;


//void _ttywrch(int ch)
//{
//  UART0_Send(ch);
//  return;
//}

//int fputc(int ch, FILE *f)
//{
//  UART0_Send(ch);
//  if(ch == '\n')
//  {
//    UART0_Send('\r');
//  }
//  return ch;
//}

//int fgetc( FILE *f)
//{
//  uint8_t ch;
//  ch = UART0_Receive();
//#if 1
//  if(ch == 0x0D) // 回车键
//  {
//    UART0_Send('\r'); // echo
//    UART0_Send('\n'); // echo
//  }
//  else {
//    UART0_Send(ch); // echo
//  }
//#endif
//  return ch;
//}

//int ferror(FILE *f) {
//  return EOF;
//}

//void _sys_exit(int return_code) {
//	if(return_code) 
//	{
//		*((volatile uint32_t*)0x40028020) = 0x00000BAD;  /* simulation NG */
//	}
//	else
//	{
//		*((volatile uint32_t*)0x40028020) = 0x00000001;  /* simulation OK */
//	}
//  while (1);    /* endless loop */
//}


// 注意：此文件与 retarget.c中内容冲突,已经注释掉retarget.c中内容
#include <stdio.h>
#include <stdint.h>
#include "BAT32G139.h"
#include "userdefine.h"
#include "sci.h"
#include "sys_mcu_header.h"


//--------------------------------------------
// 标准 C 库要求的 FILE 结构体
//--------------------------------------------
struct __FILE { int handle; };
FILE __stdout;   // printf 需要
//--------------------------------------------
// 串口单字节发送函数 (使用 UART1)
//--------------------------------------------
static void UART1_Printf_Send(uint8_t ch)
{
    SCI0->TXD1 = ch;
/*!阻塞发送*/
	while (SCI0->SSR02 & (_0040_SCI_UNDER_EXECUTE | _0020_SCI_VALID_STORED))// 用户手册19.7.1 UART发送 UART1 SCI0的通道2
    {
        ; // 等待发送完成
    }
}
//--------------------------------------------
// printf 输出底层实现
//--------------------------------------------
int fputc(int ch, FILE *f)
{
    UART1_Printf_Send((uint8_t)ch);

    // 如果遇到 '\n'，自动补 '\r'，保证终端换行正常
    if (ch == '\n')
    {
        UART1_Printf_Send('\r');
    }
    return ch;
}

//--------------------------------------------
// Keil/ARM C 库要求的调试输出函数
//--------------------------------------------
void _ttywrch(int ch)
{
    UART1_Printf_Send((uint8_t)ch);
}



///*
// *-----------------------------------------------------------------------------
// * The confidential and proprietary information contained in this file may
// * only be used by a person authorised under and to the extent permitted
// * by a subsisting licensing agreement from ARM Limited.
// *
// *            (C) COPYRIGHT 2011-2012 ARM Limited.
// *                ALL RIGHTS RESERVED
// *
// * This entire notice must be reproduced on all copies of this file
// * and copies of this file may only be made by a person if such person is
// * permitted to do so under the terms of a subsisting license agreement
// * from ARM Limited.
// *
// *      SVN Information
// *
// *      Checked In          : $Date: 2012-01-11 17:13:57 +0000 (Wed, 11 Jan 2012) $
// *
// *      Revision            : $Revision: 197600 $
// *
// *      Release Information : Cortex-M0+ AT590-r0p1-00rel0
// *-----------------------------------------------------------------------------
// */

////
//// printf retargetting functions
////

//#include <stdio.h>
////#include <rt_misc.h>
//#include <stdint.h>
//#include "BAT32G139.h"
//#include "userdefine.h"
//#include "sci.h"

//#if defined ( __CC_ARM   )
//#if (__ARMCC_VERSION < 400000)
//#else
//// Insist on keeping widthprec, to avoid X propagation by benign code in C-lib
//#pragma import _printf_widthprec
//#endif
//#endif

////
//// C library retargetting
////

//#if defined ( __CC_ARM )
//struct __FILE { int handle; /* Add whatever you need here */ };
//#endif
//FILE __stdout;
//FILE __stdin;


//#if 1
//void _ttywrch(int ch)
//{
//  UART0_Send(ch);
//  return;
//}

//int fputc(int ch, FILE *f)
//{
//  UART0_Send(ch);
//  if(ch == '\n')
//  {
//    UART0_Send('\r');
//  }
//  return ch;
//}

//#else
//extern void UART3_Sendbyte(uint8_t ch);

//void _ttywrch(int ch)
//{
//  UART3_Sendbyte(ch);
//  return;
//}

//int fputc(int ch, FILE *f)
//{
//  UART3_Sendbyte(ch);
//  if(ch == '\n')
//  {
//    UART3_Sendbyte('\r');
//  }
//  return ch;
//}
//#endif

//int fgetc( FILE *f)
//{
//  uint8_t ch;
//  ch = UART0_Receive();
//#if 1
//  if(ch == 0x0D) // 回车键
//  {
//    UART0_Send('\r'); // echo
//    UART0_Send('\n'); // echo
//  }
//  else {
//    UART0_Send(ch); // echo
//  }
//#endif
//  return ch;
//}

//int ferror(FILE *f) {
//  return EOF;
//}

//void _sys_exit(int return_code) {
//	if(return_code) 
//	{
//		*((volatile uint32_t*)0x40028020) = 0x00000BAD;  /* simulation NG */
//	}
//	else
//	{
//		*((volatile uint32_t*)0x40028020) = 0x00000001;  /* simulation OK */
//	}
//  while (1);    /* endless loop */
//}
