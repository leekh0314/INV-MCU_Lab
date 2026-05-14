
/*****************************************************************************
@Description: sys_mcu_header.h MCU头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __SYS_MCU_HEADER_H
#define	__SYS_MCU_HEADER_H


#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "BAT32G139.h"
#include "userdefine.h"

/************driver***************/
#include "adc.h"
#include "cmp.h"
#include "gpio.h"
#include "pga.h"
#include "tim4.h"
#include "timm.h"
#include "sci.h"
#include "div.h"
#include "wdt.h"
#include "clk.h"
#include "rtc.h"
#include "DMA.h"
#include "timb.h"
#include "div.h"

/***************app****************/
#include "PQ_Calc.h"
#include "user_sample.h"
#include "PQ_Droop_Ctrl.h"
#include "user_parallel.h"
#include "sys_define_struct.h"
#include "sys_hardware_init.h"
#include "sys_state_machine.h"
#include "sys_define_config.h"
#include "INV_Power_Ctrl.h"
#include "AlgorithmLevel_Headers.h"

#endif 

