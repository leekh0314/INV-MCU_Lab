/*****************************************************************************
@Description: user_function.c 用户功能函数
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "sys_mcu_header.h"
#include "string.h"
/***************************************************************************/


/*************************************************
 * Function   : User_LED_Deal
 * Description: 系统 LED 指示灯逻辑处理函数
 * Input      : None
 * Return     : None
 * Others     : 
 * Date       : 20250904
 *   - 红灯：运行/故障状态指示
 *   - 绿灯：故障类型指示（通过闪烁次数区分）
 * Note       : User_UART_View() 在 SysTick_Handler() 里被调用，它执行时间太长（阻塞或耗时函数），把本来 1ms 的节拍“拉长”了
 *************************************************/
void User_LED_Deal(void)
{
    /********** 可调参数 **********/
    static const uint16_t BLINK_ON_TIME   = 500;   // 单次亮灯时长 (ms)
    static const uint16_t BLINK_OFF_TIME  = 500;   // 单次灭灯时长 (ms)
    static const uint16_t BLINK_INTERVAL  = 6000;  // 一轮闪烁结束后的停顿间隔 (ms)

    static uint16_t timer_ms      = 0;    // ms 计数器
    static uint8_t  blink_count   = 0;    // 当前已闪烁次数
    static uint8_t  target_blinks = 0;    // 目标闪烁次数（取决于故障类型）
    static uint8_t  phase         = 0;    // 0=空闲，1=亮，2=灭，3=间隔

    /********** 初始 / 等待 / 就绪状态 **********/
    if ((State_Context.State_Identifier == CM_INITIAL_STATE) ||
        (State_Context.State_Identifier == CM_WAITING_STATE) ||
        (State_Context.State_Identifier == CM_READY_STATE))
    {
        LED_RED_OFF;
        LED_GREEN_OFF;
        phase = 0; // 重置状态机
        return;
    }

    /********** 运行状态 **********/
    if (State_Context.State_Identifier == CM_RUN_STATE)
    {
        LED_GREEN_ON;  // 绿灯常亮（表示运行）

        if ((System_Power_Info.Active_Power_Total > 1650) &&
            (System_Power_Info.Active_Power_Total <= 1850))
        {
            // 红灯 2Hz 闪烁
            if (SysClockBase_ms.system_1ms)
            {
                timer_ms++;
                if (timer_ms < BLINK_ON_TIME) {
                    LED_RED_ON;
                } else if (timer_ms < (BLINK_OFF_TIME+BLINK_OFF_TIME)) {
                    LED_RED_OFF;
                } else {
                    timer_ms = 0;
                }
            }
        }
        else if (System_Power_Info.Active_Power_Total > 1850) {
            LED_RED_ON;   // 功率过高：红灯常亮
        }
        else {
            LED_RED_OFF;  // 功率正常：红灯灭
        }
        return;
    }

    /********** 故障/停止状态 **********/
    if ((State_Context.State_Identifier == CM_FAULT_STATE) ||
        (State_Context.State_Identifier == CM_STOP_STATE))
    {
        LED_RED_ON; // 故障时红灯常亮

//struct System_Protect_Flag_BITS
//{
//	unsigned short int VBUS_OVP:        1;//0位   过压√
//	unsigned short int VBUS_OLP:        1;//2     欠压√
//	unsigned short int INDUC_OCP:       1;//4     过流√
//	unsigned short int OLP:             1;//8     过载√
//	unsigned short int ACOUT_OVP:       1;//16    过压√
//	unsigned short int ACOUT_OLP:       1;//32    欠压√
//	unsigned short int ACOUT_SCP:       1;//64    短路√
//	unsigned short int ACOUT_DCIM:      1;//128
//	unsigned short int OTP:             1;//256   过温√
//	unsigned short int RPM_OVER:        1;//512   超速√
//	unsigned short int AUX_POWER_OVP:   1;//1024
//	unsigned short int AUX_POWER_OLP:   1;
//	unsigned short int System_Init_Fail:1;
//	unsigned short int RPM_LOW:         1;
//	unsigned short int Vref_Error:      1;
//  unsigned short int rsvd:            1;

//};
				/***** 确定故障类型优先级（只显示一个最高优先级）*****/
				if      (System_ProtectFlag_Info.bit.VBUS_OVP)           target_blinks = 1;  // bit:1   母线过压
				else if (System_ProtectFlag_Info.bit.VBUS_OLP)           target_blinks = 2;  // bit:2   母线欠压
				else if (System_ProtectFlag_Info.bit.INDUC_OCP)          target_blinks = 3;  // bit:4   输出过流
				else if (System_ProtectFlag_Info.bit.OLP)                target_blinks = 4;  // bit:8   过载
				else if (System_ProtectFlag_Info.bit.ACOUT_OVP)          target_blinks = 5;  // bit:16  输出过压
				else if (System_ProtectFlag_Info.bit.ACOUT_OLP)          target_blinks = 6;  // bit:32  输出欠压
				else if (System_ProtectFlag_Info.bit.ACOUT_SCP)          target_blinks = 7;  // bit:64  短路
				else if (System_ProtectFlag_Info.bit.OTP)                target_blinks = 8;  // bit:256 过温
				else if (System_ProtectFlag_Info.bit.RPM_OVER)           target_blinks = 9;  // bit:512 超速
				else if (System_ProtectFlag_Info.bit.AUX_POWER_OVP)      target_blinks = 10; // bit:    辅助电源过压
				else if (System_ProtectFlag_Info.bit.AUX_POWER_OLP)      target_blinks = 11; // bit:    辅助电源欠压
				else if (System_ProtectFlag_Info.bit.System_Init_Fail)   target_blinks = 12; // bit:    系统初始化失败
				else if (System_ProtectFlag_Info.bit.RPM_LOW)            target_blinks = 13; // bit:    欠速
				else if (System_ProtectFlag_Info.bit.Vref_Error)         target_blinks = 14; // bit:    参考基准电压异常
				else                                                     target_blinks = 15; // bit:x   默认(ACOUT_DCIM/AUX_POWER_OVP/AUX_POWER_OLP/System_Init_Fail/RPM_LOW/Vref_Error)


        /***** 闪烁状态机处理 *****/
        if (SysClockBase_ms.system_1ms)
        {
            timer_ms++;

            switch (phase)
            {
                case 0: // 空闲 → 进入闪烁循环
                    blink_count = 0;
                    timer_ms = 0;
                    phase = 1;
                    break;

                case 1: // 亮灯阶段
                    LED_GREEN_ON;
                    if (timer_ms >= BLINK_ON_TIME) {
                        timer_ms = 0;
                        phase = 2; // 切换到灭灯阶段
                    }
                    break;

                case 2: // 灭灯阶段
                    LED_GREEN_OFF;
                    if (timer_ms >= BLINK_OFF_TIME) {
                        timer_ms = 0;
                        blink_count++;
                        if (blink_count >= target_blinks) {
                            phase = 3; // 进入间隔阶段
                        } else {
                            phase = 1; // 继续下一次闪烁
                        }
                    }
                    break;

                case 3: // 间隔阶段
                    LED_GREEN_OFF;
                    if (timer_ms >= BLINK_INTERVAL) {
                        timer_ms = 0;
                        phase = 0; // 周期结束，重新开始
                    }
                    break;

                default:
                    phase = 0;
                    break;
            }
        }
    }
}


/**
 * @brief  计算CRC16校验码（Modbus标准，多项式 0xA001）,用于VisualScope上位机校验代码。
 *
 * 本函数用于对输入的字节数组进行CRC16校验，结果存入Rcvbuf中。
 * 算法采用常用的CRC16(Modbus)标准：初始值0xFFFF，多项式0xA001。
 * 注意：输出的低字节在前，高字节在后。
 *
 * @param Array   输入数据数组（待校验的数据）,存放需要发送的4个通道数据
 * @param Rcvbuf  存放CRC校验码，长度为2字节
 *                 - Rcvbuf[0]：CRC高字节
 *                 - Rcvbuf[1]：CRC低字节
 * @param Len     输入数据的长度（字节数）固定8
 *
 * @note  如果需要符合Modbus RTU发送顺序，应交换Rcvbuf[0]与Rcvbuf[1]。
 * @note  示例：CRC16(Array,Rcvbuf,8)
 */
void CRC16(unsigned char *Array, unsigned char *Rcvbuf, unsigned int Len)
{
    unsigned int IX, IY, CRC_Value;
    CRC_Value = 0xFFFF; // 初始值，全1

    if (Len <= 0)
    {
        CRC_Value = 0;  // 长度非法时返回0
    }        
    else
    {
        Len--;  // 因为下方循环使用 <=，所以这里先减1，避免越界

        // 遍历每个字节
        for (IX = 0; IX <= Len; IX++)
        {
            // 将当前字节与CRC寄存器低8位异或
            CRC_Value = CRC_Value ^ (unsigned int)(Array[IX]);
            
            // 遍历该字节中的8位
            for (IY = 0; IY <= 7; IY++)
            {
                // 如果最低位为1，则右移并与多项式0xA001异或
                if ((CRC_Value & 1) != 0)
                {
                    CRC_Value = (CRC_Value >> 1) ^ 0xA001;// 多项式 0xA001
                }
                // 否则仅右移
                else
                {
                    CRC_Value = CRC_Value >> 1;
                }
            }
        }
    }

    // 将最终结果拆分为低字节在前
    Rcvbuf[0] = (CRC_Value & 0x00ff);       // 低字节
    Rcvbuf[1] = (CRC_Value & 0xff00) >> 8;  // 高字节

}
/*************************************************
Description: Function_TxSendDebug
Input      : 
Return     : 
Others     : 调试串口数据处理
*************************************************/
#if 0
uint8_t u8_senddataarraydebug[9] = {0};
void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4)
{
    uint8_t  u8_senddataindexdebug = 0;

    u8_senddataarraydebug[0] = (uint8_t)(data1);  u8_senddataarraydebug[1] = (uint8_t)(data1>>8);
    u8_senddataarraydebug[2] = (uint8_t)(data2);  u8_senddataarraydebug[3] = (uint8_t)(data2>>8);
    u8_senddataarraydebug[4] = (uint8_t)(data3);  u8_senddataarraydebug[5] = (uint8_t)(data3>>8);
    u8_senddataarraydebug[6] = (uint8_t)(data4);  u8_senddataarraydebug[7] = (uint8_t)(data4>>8);

    u8_senddataarraydebug[8] = 0;                            //校验字节清零 
    for(u8_senddataindexdebug = 0; u8_senddataindexdebug <= 7; u8_senddataindexdebug++)
    {
        u8_senddataarraydebug[8] += u8_senddataarraydebug[u8_senddataindexdebug];
    }
		
    DMAVEC->CTRL[0].DMSAR = (uint32_t)(u8_senddataarraydebug+1);
    DMAVEC->CTRL[0].DMACT = 8;                                      //传输8个数据
    DMA->DMAEN1 |= 1<<6;   //uart1                                  //使能传输(UART1)
//      DMA->DMAEN1 |= 1<<4;   //uart0  
    SCI0->TXD1 = u8_senddataarraydebug[0]; 		
}
#else

//uint8_t txBuf[20] = {0};
//float num[4] = {0};
//uint8_t u8_senddebug = 0;
//void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4)
//{
//    num[0] = data1;
//    num[1] = data2;
//    num[2] = data3;
//    num[3] = data4; 

//    memcpy(txBuf,(uint8_t *)num,16);
//	
//    txBuf[16] = 0x00;
//    txBuf[17] = 0x00;
//    txBuf[18] = 0x80;
//    txBuf[19] = 0x7f;
//	
//		
////    u8_senddebug++;
////    if(u8_senddebug >=20) u8_senddebug = 0;
////    SCI0->TXD1 = txBuf[u8_senddebug]; 

//    DMAVEC->CTRL[0].DMSAR = (uint32_t)(txBuf+1); // 从 txBuf[1] 开始传输
//    DMAVEC->CTRL[0].DMACT = 19; // 设置传输长度为19字节  //传输8个数据
//    DMA->DMAEN1 |= 1<<6;       // 启动 DMA 通道，触发 UART1 发送
////      DMA->DMAEN1 |= 1<<4;  // 如果是 UART0，改为第4位
//    SCI0->TXD1 = txBuf[0]; 	  // 先写第1个字节触发UART发送，DMA随后接管
//}

/* ---------------- uart1 + DMA数据发送函数-(vofa+ 上位机)-1M波特率 ----------------*/
uint8_t txBuf[20] = {0};
float num[4] = {0};
void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4)
{
    num[0] = data1;
    num[1] = data2;
    num[2] = data3;
    num[3] = data4; 

    memcpy(txBuf,(uint8_t *)num,16);
	
    txBuf[16] = 0x00; // 帧尾 tail 00 00 80 7f
    txBuf[17] = 0x00;
    txBuf[18] = 0x80;
    txBuf[19] = 0x7f;
		
    DMAVEC->CTRL[0].DMSAR = (uint32_t)(txBuf+1); // 从 txBuf[1] 开始传输
    DMAVEC->CTRL[0].DMACT = 19; // 设置传输长度为19字节  //传输8个数据
    DMA->DMAEN1 |= 1<<6;       // 启动 DMA 通道，触发 UART1 发送
//      DMA->DMAEN1 |= 1<<4;  // 如果是 UART0，改为第4位
    SCI0->TXD1 = txBuf[0]; 	  // 先写第1个字节触发UART发送，DMA随后接管
}


///* ---------------- uart1数据发送函数-CRC16(VisualScope)-传统逐字节发送-9600波特率 ----------------*/
//void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4) {
//	static uint8_t u8_senddataindexdebug = 0;
//	uint8_t CRCbuf[2] = {0};

//	static uint8_t u8_senddataarraydebug[10] = {0};// 不要帧头

//    // 拆分通道数据（小端模式:低位在前）
//    u8_senddataarraydebug[0] = (uint8_t)(data1 & 0xFF);// 低位
//    u8_senddataarraydebug[1] = (uint8_t)((data1 >> 8) & 0xFF);// 高位
//    u8_senddataarraydebug[2] = (uint8_t)(data2 & 0xFF);
//    u8_senddataarraydebug[3] = (uint8_t)((data2 >> 8) & 0xFF);
//    u8_senddataarraydebug[4] = (uint8_t)(data3 & 0xFF);
//    u8_senddataarraydebug[5] = (uint8_t)((data3 >> 8) & 0xFF);
//    u8_senddataarraydebug[6] = (uint8_t)(data4 & 0xFF);
//    u8_senddataarraydebug[7] = (uint8_t)((data4 >> 8) & 0xFF);
//	// 计算CRC16（对4通道数据 8个unsigned char数据做校验）
//	CRC16(u8_senddataarraydebug, CRCbuf, 8);

//	// 附加 CRC 校验码
//	u8_senddataarraydebug[8]  = CRCbuf[0]; 
//	u8_senddataarraydebug[9]  = CRCbuf[1]; 

//	/*----方法1---- 逐字节发送-阻塞 9600波特率-------*/ 
////	for (int i = 0; i < sizeof(u8_senddataarraydebug); i++) {
////		
////		SCI0->TXD1 = u8_senddataarraydebug[i];
////		// 用户手册19.7.1 UART发送 UART1 SCI0的通道2
////		// 等待发送完成
////		while (SCI0->SSR02 & (_0040_SCI_UNDER_EXECUTE | _0020_SCI_VALID_STORED)) { ; }
////	}
//  
//	/*----方法2---- 非阻塞 -需1M 波特率-------20250904*/ 
//	SCI0->TXD1 = u8_senddataarraydebug[u8_senddataindexdebug]; //非阻塞, 必须设置1M 波特率 
//	// 发送索引递增
//	if (++u8_senddataindexdebug >= sizeof(u8_senddataarraydebug)) {
//		u8_senddataindexdebug = 0; // 发完一帧，回到开头
//	}
//}

///* ---------------- uart1 + DMA数据发送函数-CRC16(VisualScope_Crack 上位机)-9600波特率-----test_cnt>=200------------*/
//uint8_t txBuf[10] = {0};
//void Function_TxSendDebug(int32_t data1,int32_t data2,int32_t data3,int32_t data4) {
//	uint8_t CRCbuf[2] = {0};
//	// 拆分通道数据（小端模式:低位在前）
//	txBuf[0] = (uint8_t)(data1 & 0xFF);// 低位
//	txBuf[1] = (uint8_t)((data1 >> 8) & 0xFF);// 高位
//	txBuf[2] = (uint8_t)(data2 & 0xFF);
//	txBuf[3] = (uint8_t)((data2 >> 8) & 0xFF);
//	txBuf[4] = (uint8_t)(data3 & 0xFF);
//	txBuf[5] = (uint8_t)((data3 >> 8) & 0xFF);
//	txBuf[6] = (uint8_t)(data4 & 0xFF);
//	txBuf[7] = (uint8_t)((data4 >> 8) & 0xFF);

//	// 计算CRC16（对4通道数据 8个unsigned char数据做校验）
//	CRC16(txBuf, CRCbuf, 8);

//	// 附加 CRC 校验码
//	txBuf[8]  = CRCbuf[0]; 
//	txBuf[9]  = CRCbuf[1]; 

//	DMAVEC->CTRL[0].DMSAR = (uint32_t)(txBuf+1); // 从 txBuf[1] 开始传输
//	DMAVEC->CTRL[0].DMACT = 9; // 设置传输长度为9字节 
//	DMA->DMAEN1 |= 1<<6;       // 启动 DMA 通道，触发 UART1 发送
//	//DMA->DMAEN1 |= 1<<4;  // 如果是 UART0，改为第4位
//	SCI0->TXD1 = txBuf[0]; 	  // 先写第1个字节触发UART发送，DMA随后接管
//}
#endif



// 20250825 系统保护位名字数组（和位定义顺序对应，bit0 → bit15） 
const char *ProtectFlagNames[16] = {
    "VBUS_OVP",         // bit0 母线过压
    "VBUS_OLP",         // bit1 母线过流/过载保护
    "INDUC_OCP",        // bit2 电感过流保护
    "OLP",              // bit3 过载保护
    "ACOUT_OVP",        // bit4 交流输出过压保护
    "ACOUT_OLP",        // bit5 交流输出欠压保护
    "ACOUT_SCP",        // bit6 交流输出短路保护
    "ACOUT_DCIM",       // bit7 直流偏置保护
    "OTP",              // bit8 过温保护
    "RPM_OVER",         // bit9 转速过高
    "AUX_POWER_OVP",    // bit10 辅助电源过压
    "AUX_POWER_OLP",    // bit11 辅助电源过流/过载
    "System_Init_Fail", // bit12 系统初始化失败
    "RPM_LOW",          // bit13 转速过低
    "Vref_Error",       // bit14 基准电压异常
    "rsvd"              // bit15 保留
};

void Print_SystemProtectFlags(void)
{
    printf("系统保护标志 (bit[15..0]): \n");
    for(int i = 15; i >= 0; i--) {
        printf("位%-2d (%-16s): %d\n",
               i,
               ProtectFlagNames[i],
               (System_ProtectFlag_Info.all >> i) & 1);
    }
		
//System_Protect_Flag_u flag;
//flag.all = 0x0001;   // 只置最低位
//printf("VBUS_OVP = %d\n", flag.bit.VBUS_OVP); // VBUS_OVP = 1 说明环境里位域是从低位开始排
//printf("rsvd    = %d\n", flag.bit.rsvd);

}
const char *StartCheckFlagNames[16] = {
    "vBus_Up_OK",       // bit0 母线电压上升正常
    "vBus_Dn_OK",       // bit1 母线电压下降正常
    "RPM_Up_OK",        // bit2 转速上升正常
    "auxPower_Up_OK",   // bit3 辅助电源上升正常
    "auxPower_Dn_OK",   // bit4 辅助电源下降正常
    "temp_Up_OK",       // bit5 温度检测正常
    "rsvd6",            // bit6 保留
    "rsvd7",            // bit7
    "rsvd8",            // bit8
    "rsvd9",            // bit9
    "rsvd10",           // bit10
    "rsvd11",           // bit11
    "rsvd12",           // bit12
    "rsvd13",           // bit13
    "rsvd14",           // bit14
    "rsvd15"            // bit15
};
// 系统启动检测
void Print_StartCheckFlags(void)
{
    printf("系统启动检查标志 (bit[15..0]): \n");
    for(int i = 15; i >= 0; i--) {
        printf("位%-2d (%-16s): %d\n",
               i,
               StartCheckFlagNames[i],
               (StartCheck_Flag_Info.all >> i) & 1);
    }
}

//// 四个测试变量
//static int32_t var1 = 0;      // 递增
//static int32_t var2 = 1000;   // 递减
//static int32_t var3 = 0;      // 正弦变化
//static int32_t var4 = 0;      // 锯齿波

//void User_UART_View(void)
//{
//    // 1. 递增变量
//    var1 += 10;
//    if(var1 > 1000) var1 = 0;

//    // 2. 递减变量
//    var2 -= 20;
//    if(var2 < 0) var2 = 1000;

//    // 3. 正弦波模拟（近似）
//    static float angle = 0.0f;
//    var3 = (int32_t)(5000.0f * sinf(angle));// var3 sin
//	  var4 = (int32_t)(5000.0f * sinf(angle));// var4 sin
//    angle += 0.1f;
//    if(angle > 6.28f) angle = 0.0f; // 2π

////    // 4. 锯齿波 
//	  
////    var4 += 50;
////    if(var4 > 1000) var4 = -1000;

//    // 发送数据到 Vofa+
//    Function_TxSendDebug(var1, var2, var3, var4);
//}

/*************************************************
Description: User_UART_View
Input      : 
Return     : 
Others     : 用于串口调试
*************************************************/
extern const  short int Sine_Table_50Hz[];
extern const  short int Sine_Table_60Hz[];
extern int16_t correct_DCIM_value;
extern int32_t		s32_GeneratorcapvalCal_OLd ;
extern int stepmotor_temp,duty_test;
extern uint8_t stepmotor_speed_const;
extern int32_t s32_GeneratorcapvalCaltemp ,AM_COUNT ;

void User_UART_View(void)
{					
    /*------------------------------------------------------------------------------------*/			
    /*---------------------UART0串口调试--------------------------------------------------*/				
    //Function_TxSendDebug( PID_Vol_Loop.Ref, PID_Vol_Loop.Fdb,PID_Vol_Loop.Out,PID_Vol_Loop.Ui>> 15 );//    
    //Function_TxSendDebug( 0, System_Power_Info.Active_Power_Total, ADSample_Info.AC_Vol_RMS,ADSample_Info.Inv_Cur_RMS );//
    //Function_TxSendDebug( System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,State_Context.State_Identifier*1000,ADSample_Info.Bus_Vol_AD);//
    //Function_TxSendDebug( PID_Vol_Loop.Ref, PID_Vol_Loop.Fdb,PID_Curr_Loop.Ref,PID_Curr_Loop.Fdb);//
    
//    Function_TxSendDebug( Inv_SineCtrl_Info.InvVol_AM_Target, ADSample_Info.Bus_Vol_AD,System_Power_Info.Active_Power_Total,PID_Curr_Loop.Fdb);//
////    Function_TxSendDebug( PID_Vol_Loop.Ref, PID_Vol_Loop.Fdb,PID_Curr_Loop.Ref,PID_Curr_Loop.Fdb);//
//    Function_TxSendDebug( System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,State_Context.State_Identifier*1000+INV_SCR_STATUS,ADSample_Info.Bus_Vol_AD);//
//      Function_TxSendDebug( AD_Correct_V_ACOUT.ADRef_Correct_Value, AD_Correct_I_Induc.ADRef_Correct_Value,PID_Curr_Loop.Ref,PID_Curr_Loop.Fdb);//

//    Function_TxSendDebug( ADSample_Info.AC_Vol_RMS ,ADSample_Info.Inv_Cur_RMS,System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total);//
//    Function_TxSendDebug( ADSample_Info.AC_Vol_RMS ,ADSample_Info.Inv_Cur_RMS,System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total);//
////    Function_TxSendDebug( ADSample_Info.AC_Vol_RMS ,ADSample_Info.Inv_Cur_RMS,System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total);//
    
//    Function_TxSendDebug( ADSample_Info.Inv_Cur_Peak ,PID_Curr_Loop.Out,State_Context.State_Identifier*1000,INV_PID_DCIM.out);//
//    Function_TxSendDebug( ADSample_Info.AC_Vol_Peak ,ADSample_Info.Inv_Cur_Peak,State_Context.State_Identifier*1000,ADSample_Info.ref_AD_Fir);//
//   Function_TxSendDebug( System_Power_Info.Active_Power_Total,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,stepmotor_temp*100);//
    
//   Function_TxSendDebug( System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total,ADSample_Info.AC_Vol_RMS,(((uint32_t)ADSample_Info.AC_Vol_RMS * ADSample_Info.Load_Cur_RMS ) >> 2) >> 8);//
   
//      Function_TxSendDebug(  State_Context.State_Identifier*1000 ,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,System_ProtectFlag_Info.all);//
//     Function_TxSendDebug( StepMotor_Ctrl.err+stepmotor_speed_const*1000,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,stepmotor_temp*100);//
//     Function_TxSendDebug( StepMotor_Ctrl.err+stepmotor_speed_const*1000,PID_Vol_Loop.Fdb,ADSample_Info.Bus_Vol_Filiter_AD,INV_Info.AC_Vol_AMP_Target);//

//     Function_TxSendDebug( ADSample_Info.Inv_Cur_Peak,ADSample_Info.AC_Vol_Peak,AD_Correct_I_Induc.ADRef_Correct_Value,ADSample_Info.Bus_Vol_Filiter_AD);//


/**************************************************UART1-串口调试代码*********************************************************/
#if 0
/****************************uart1_printf/DEBUG_MODE/波特率9600/test_cnt >= 200***********************************/
//     Print_SystemProtectFlags(); //系统保护位
//		 Print_StartCheckFlags();// 系统启动检测
//		 
		 printf("State:%d\n",State_Context.State_Identifier);// 状态机编号
		 printf("Speed:%d\n", s16_GeneratorSpeedAvg);//发电机平均转速
		 printf("Bus_Vol_Filiter_AD:%d\n",ADSample_Info.Bus_Vol_Filiter_AD);//直流母线电压_滤波
     printf("ref_AD:%d\n",ADSample_Info.ref_AD>>1);//基准电压ADC
		 
//		 printf("NTC温度采样ADC：%d\n",ADSample_Info.Temp_External_AD);		 // ADSample_Info.Temp_External_AD
////     printf("PID_Vol_Loop.Ref:%d \n",PID_Vol_Loop.Ref);		
////     printf("PID_Vol_Loop.Fdb:%d \n",PID_Vol_Loop.Fdb);	
////     printf("PID_Curr_Loop.Ref:%d \n",PID_Curr_Loop.Ref);	
////     printf("PID_Curr_Loop.Fdb:%d \n",PID_Curr_Loop.Fdb);	
//		 
     printf("InvVol_Store_One:%d\n",ADSample_Store_Info.InvVol_Store_One>>1); // 交流电压ADC ADSample_Store_Info.InvVol_Store_One
     printf("InvCur_Store_One:%d\n",ADSample_Store_Info.InvCur_Store_One>>1);// 电感电流ADC ADSample_Store_Info.InvCur_Store_One
     printf("AC_Vol_RMS:%.2f\n",(float)(ADSample_Info.AC_Vol_RMS/4.0)); // 交流电压有效值
		 printf("Inv_Cur_RMS:%.2f\n",(float)(ADSample_Info.Inv_Cur_RMS/256.0));//交流电流有效值
//		 printf("Load_Cur_RMS：%d\n",ADSample_Info.Load_Cur_RMS);//负载电流有效值
#else
       // 0xFFFF/系统保护位/系统启动检测/转速
//     Function_TxSendDebug(0xFFFF,System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,s16_GeneratorSpeedAvg); 
     
//     Function_TxSendDebug(System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,PID_Vol_Loop.Ref,PID_Vol_Loop.Fdb); // 系统保护位/系统启动检测/输出电压给定/输出电压反馈  
//		 Function_TxSendDebug(System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,ADSample_Info.Power_AuxVcc_AD,(int32_t)(s16_GeneratorSpeedAvg)); // 系统保护位+系统启动检测+辅助电源ad值+转速  
//		 Function_TxSendDebug(System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,ADSample_Info.Power_AuxVcc_AD,ADSample_Info.AC_Vol_RMS); // 系统保护位+系统启动检测+辅助电源ad值+交流RMS  
			
		 // 20251018 开环(DEBUG_MODE)交流输出AD&RMS 排查
		 //Function_TxSendDebug(System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,ADSample_Store_Info.InvVol_Store_One>>1,(ADSample_Info.AC_Vol_RMS/4.0)); // 系统保护位+系统启动检测+交流输出ad值+交流RMS  
//     Function_TxSendDebug(ADSample_Store_Info.InvVol_Store_One>>1,(ADSample_Info.AC_Vol_RMS/4.0),ADSample_Store_Info.InvVol_Store_One>>1,(ADSample_Info.AC_Vol_RMS/4.0)); // 交流电压输出ad值+交流电压RMS+交流电压输出ad值+交流电压RMS  
     Function_TxSendDebug(ADSample_Store_Info.InvVol_Store_One>>1,(ADSample_Info.AC_Vol_RMS/4.0),ADSample_Store_Info.InvCur_Store_One>>1,(ADSample_Info.Inv_Cur_RMS/256.0)); // 交流输出ad值+交流RMS+交流输出电流ad值+交流电流RMS  

//		Function_TxSendDebug(State_Context.State_Identifier,(int32_t)(s16_GeneratorSpeedAvg), (int32_t)(ADSample_Info.Temp_External_AD),(int32_t)(ADSample_Info.ref_AD)); //  
//		Function_TxSendDebug(State_Context.State_Identifier,(int32_t)(s16_GeneratorSpeedAvg), (int32_t)(PID_Vol_Loop.Ref),(int32_t)(PID_Vol_Loop.Fdb)); // PID_Vol_Loop.Ref, PID_Vol_Loop.Fdb  
//		Function_TxSendDebug(State_Context.State_Identifier,(int32_t)(s16_GeneratorSpeedAvg), (int32_t)(PID_Curr_Loop.Ref),(int32_t)(PID_Curr_Loop.Fdb)); // PID_Curr_Loop.Ref, PID_Curr_Loop.Fdb  
//		Function_TxSendDebug((int32_t)(PID_Vol_Loop.Ref),(int32_t)(PID_Vol_Loop.Fdb), (int32_t)(PID_Curr_Loop.Ref),(int32_t)(PID_Curr_Loop.Fdb)); // PID_Curr_Loop.Ref, PID_Curr_Loop.Fdb  20250829
//    Function_TxSendDebug(0x1234,0x5678,0x9abc,0xdef0);// 测试
#endif
/***********************************************************************************************************/

		 
		 
//    Function_TxSendDebug( System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total,System_ProtectFlag_Info.all,ADSample_Info.Bus_Vol_Filiter_AD);//
//    Function_TxSendDebug( StepMotor_Ctrl.err+stepmotor_speed_const*1000, StepMotor_Ctrl.ref,stepmotor_temp*100,StepMotor_Ctrl.fdb);//

//    Function_TxSendDebug( StepMotor_Ctrl.err+stepmotor_speed_const*1000, ADSample_Info.Bus_Vol_Filiter_AD,PID_Curr_Loop.Ref,PID_Curr_Loop.Fdb);//
//    Function_TxSendDebug( StepMotor_Ctrl.err+stepmotor_speed_const*1000, Inv_SineCtrl_Info.CurrLoop_Up_Limit,stepmotor_temp*100,StepMotor_Ctrl.fdb);//

//    Function_TxSendDebug( System_Power_Info.Active_Power_Total,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,stepmotor_temp*100);//
//    Function_TxSendDebug( s32_GeneratorcapvalCal_OLd>>2,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,stepmotor_temp*100);//
//    Function_TxSendDebug( System_ProtectFlag_Info.all,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,ADSample_Info.Bus_Vol_Filiter_AD);//
//    Function_TxSendDebug( System_ProtectFlag_Info.all,StepMotor_Ctrl.fdb,StepMotor_Ctrl.ref,ADSample_Info.ref_AD_Fir);//

//    Function_TxSendDebug( System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,\
//        AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok*1000+AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok*100+AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok*10+paralogiccontrol.masterslavebit,State_Context.State_Identifier*1000);//
////    Function_TxSendDebug( ADSample_Info.Bus_Vol_AD ,Inv_SineCtrl_Info.InvVol_AM_Target,ADSample_Info.AC_Vol_Peak,ADSample_Info.Bus_Vol_Filiter_AD);//
//    Function_TxSendDebug( Inv_SineCtrl_Info.InvVol_AM_MultiCalc ,PQ_Droop_Ctrl_Info.AMP_Out ,Inv_SineCtrl_Info.InvVol_AM_Target,PID_Vol_Loop.Out);//
//    Function_TxSendDebug( PID_Curr_Loop.Ref ,PID_Curr_Loop.Fdb ,PID_Vol_Loop.Ref ,PID_Vol_Loop.Fdb);//
//    Function_TxSendDebug( Inv_SineCtrl_Info.InvVol_Peak_AD ,Inv_SineCtrl_Info.InvVol_AM_BusCalc ,PQ_Droop_Ctrl_Info.BusVol_In ,PQ_Droop_Ctrl_Info.AMP_Out);//
//    Function_TxSendDebug( Inv_SineCtrl_Info.InvVol_Peak_AD ,Inv_SineCtrl_Info.BusVol_AD ,Inv_SineCtrl_Info.InvVol_AM_Target ,AM_COUNT>>2);//

//    Function_TxSendDebug( PID_Curr_Loop.Ref ,PID_Curr_Loop.Fdb ,PID_Vol_Loop.Ref ,System_ProtectFlag_Info.all );//
//    Function_TxSendDebug( PID_Curr_Loop.Ref ,PID_Curr_Loop.Fdb ,Inv_SineCtrl_Info.CurrLoop_Up_Limit,System_Power_Info.Active_Power_Total);//
//    Function_TxSendDebug(Inv_SineCtrl_Info.InvVol_SPWM_Period,INV_Info.AC_Vol_AMP_Target,PID_Vol_Loop.Ref,Inv_SineCtrl_Info.InvVol_SPWM_Compare_A);//
//    Function_TxSendDebug(Inv_SineCtrl_Info.InvVol_SPWM_Period,INV_Info.AC_Vol_AMP_Target,Inv_SineCtrl_Info.BusVol_AD,Inv_SineCtrl_Info.InvVol_SPWM_Compare_A);//

//    Function_TxSendDebug(AD_Correct_Vref.ADRef_Correct_Value,AD_Correct_I_Induc.ADRef_Correct_Value,ADSample_Info.Bus_Vol_Filiter_AD ,AD_Correct_V_ACOUT.ADRef_Correct_Value  );//

//    Function_TxSendDebug(AD_Correct_Vref.ADRef_Correct_Value,AD_Correct_I_Induc.ADRef_Correct_Value,StartCheck_Flag_Info.all,AD_Correct_V_ACOUT.ADRef_Correct_Value  );//

//    Function_TxSendDebug( System_Power_Info.Active_Power_Total,System_Power_Info.Apparent_Power_Total ,Inv_SineCtrl_Info.CurrLoop_Up_Limit ,System_ProtectFlag_Info.all);//
	     
//    Function_TxSendDebug( ADSample_Info.AC_Vol_RMS*10>>2,ADSample_Info.Inv_Cur_RMS*10>>8,System_Power_Info.Apparent_Power_Total ,System_Power_Info.Active_Power_Total);//
   
////    Function_TxSendDebug( AD_Correct_V_ACOUT.ADRef_Correct_Value ,AD_Correct_Vref.ADRef_Correct_Value, Inv_SineCtrl_Info.InvVol_Peak_AD ,AD_Correct_I_Induc.ADRef_Correct_Value);//
//      Function_TxSendDebug( PID_Curr_Loop.Ref ,PID_Curr_Loop.Fdb ,PID_Vol_Loop.Ref ,PID_Vol_Loop.Fdb);//

//    Function_TxSendDebug( ADSample_Info.AC_Vol_RMS*10>>2,ADSample_Info.Inv_Cur_RMS*10>>8,System_Power_Info.Apparent_Power_Total ,System_Power_Info.Active_Power_Total);//
//    Function_TxSendDebug( AD_Correct_V_ACOUT.ADRef_Correct_Value ,AD_Correct_Vref.ADRef_Correct_Value,ADSample_Store_Info.InvCur_Store_One,AD_Correct_I_Induc.ADRef_Correct_Value);//
            
//    Function_TxSendDebug( AD_Correct_V_ACOUT.ADRef_Correct_Value ,AD_Correct_Vref.ADRef_Correct_Value,ADSample_Info.AC_Vol_Peak,AD_Correct_I_Induc.ADRef_Correct_Value);//
//        Function_TxSendDebug( System_ProtectFlag_Info.all ,StepMotor_Ctrl.fdb,\
//    AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok*1000+AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok*100+AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok*10,StartCheck_Flag_Info.all);//
//        Function_TxSendDebug( System_ProtectFlag_Info.all ,State_Context.State_Identifier*1000,\
//    AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok*1000+AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok*100+AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok*10,ADSample_Info.ref_AD_Fir);//

//        Function_TxSendDebug( INV_Ctrl_Info.DCIM_Val ,ADSample_Info.Inv_Cur_Peak,ADSample_Info.AC_Vol_Peak,INV_PID_DCIM.out);//
//        Function_TxSendDebug( System_Power_Info.Active_Power_Total ,ADSample_Info.Inv_Cur_Peak,ADSample_Info.AC_Vol_Peak,Inv_SineCtrl_Info.CurrLoop_Up_Limit );//

//        Function_TxSendDebug( ADSample_Store_Info.InvVol_Store_One ,ADSample_Info.Inv_Cur_Peak,ADSample_Info.AC_Vol_Peak,System_ProtectFlag_Info.all);//

//        Function_TxSendDebug( correct_DCIM_value ,ADSample_Info.Inv_Cur_Peak,ADSample_Info.AC_Vol_Peak,System_ProtectFlag_Info.all);//
    
//     Function_TxSendDebug( Inv_SineCtrl_Info.InvVol_SPWM_Period ,Inv_SineCtrl_Info.InvVol_SPWM_Compare_A ,PID_Curr_Loop.Out,INV_Power_Ctrl_Info.SPWM_Compare_A);//

//     Function_TxSendDebug( PID_Vol_Loop.Ref ,PID_Curr_Loop.Ui>>12 ,PID_Curr_Loop.Out,ADSample_Info.Bus_Vol_AD);//

//     Function_TxSendDebug( ADSample_Info.Inv_Cur_Peak ,ADSample_Info.Inv_Cur_RMS ,ADSample_Info.AC_Vol_Peak,ADSample_Info.AC_Vol_RMS);//
//     Function_TxSendDebug( System_Power_Info.Apparent_Power_Total ,System_Power_Info.Active_Power_Total ,ADSample_Info.Inv_Cur_RMS,ADSample_Info.AC_Vol_RMS);//
//    Function_TxSendDebug( ADSample_Info.Inv_Cur_Peak ,INV_PID_Power.fdb,INV_PID_Power.err_Integral>>6,INV_PID_Power.out);//

    //Function_TxSendDebug( ADSample_Info.ref_AD_Fir ,AD_Correct_Vref.ADRef_Correct_Value,ADSample_Info.AC_Vol_Peak,System_ProtectFlag_Info.all);//
//    Function_TxSendDebug( System_ProtectFlag_Info.all ,StartCheck_Flag_Info.all,\
//            State_Context.State_Identifier*1000,\
//            AD_Correct_V_ACOUT.Flag.bit.ADRef_Correct_Ok+\
//            AD_Correct_I_Induc.Flag.bit.ADRef_Correct_Ok*10+\
//            AD_Correct_Vref.Flag.bit.ADRef_Correct_Ok*100);//
    //Function_TxSendDebug( ADSample_Info.AC_Vol_Sum_Back>>12, System_Power_Info.Active_Power_Total, ADSample_Info.AC_Vol_RMS,ADSample_Info.Inv_Cur_RMS );//
}

/*************************************************
Description: INV_CloseDrive
Input      : 
Return     : 
Others     : 保护时关闭驱动
*************************************************/
void INV_CloseDrive(void)
{	  
	 if (( CM_INITIAL_STATE != State_Context.State_Identifier ) &&
	     ( CM_WAITING_STATE != State_Context.State_Identifier ) &&
	     ( CM_READY_STATE   != State_Context.State_Identifier ) &&
	      (         E_FALSE != System_ProtectFlag_Info.all   ))
	 {
       if ( ( Inv_SineCtrl_Info.SineTab_Index <=   5 ) || ( Inv_SineCtrl_Info.SineTab_Index >= ( Inv_SineCtrl_Info.SineTab_PeriodPoint - 5 )) )  //在接近过零点的时候关闭驱动
        {
            #if OPERATING_MODE == NORMAL_MODE
                INV_SCR_DISABLE;
                User_PWM_Disable();	//关闭PWM输出 
                State_Context.flag.bit.Fault_Occur = E_TRUE;
            #endif
        }
	 }
	 else State_Context.flag.bit.Fault_Occur = E_FALSE;				
}


/*************************************************
Description: User_Divider
Input      : 
Return     : 
Others     : 硬件除法器 只能在中断中使用
*************************************************/
int User_Divider(int Divd, int Divs)
{
	uint32_t Quot,Remain = 0;
    
	DIV_Operation(Divd, Divs, &Quot, &Remain);  
	
	return Quot;   
}


/*************************************************
Description: Key_Calc
Input      : 
Return     : 
Others     : 按键功能程序逻辑处理
*************************************************/
void key_Calc( KeyInfo *key )
{
	  if (key->Compare_Type == E_MAX)
	  {
        if   ( key->In > key->Compare_Dot )  key->Compare_Logic = 1;
        else                                 key->Compare_Logic = 2;
	  }
	  else if (key->Compare_Type == E_EQUAL)
	  {
        if   ( key->In == key->Compare_Dot ) key->Compare_Logic = 1;
        else                                 key->Compare_Logic = 2;
	  }
	  else
	  {
        if   ( key->In < key->Compare_Dot )  key->Compare_Logic = 1;
        else                                 key->Compare_Logic = 2;
	  }

	 /*------------------------------------------------------------------------------------*/
	  if ( key->Compare_Logic == 1 )	//低有效，上位
	  {
		    if ( key->IO_L_count > 0 )                    key->IO_L_count--;			  
		    if ( key->IO_H_count < key->Delay_MaxCount )  key->IO_H_count++;
		    else
		    {
			      key->IO_H_count = 0;
			      key->Key_State = KEY_OFF;  
		    }
	  }
	  else
	  {
		    if ( key->IO_H_count > 0 )                    key->IO_H_count--;			
		    if ( key->IO_L_count < key->Delay_MaxCount )  key->IO_L_count++;
		    else
		    {
			      key->IO_L_count = 0;
			      key->Key_State = KEY_ON;   // 
		    }
	  }
}


/*************************************************
Description: User_Key_Deal
Input      : 
Return     : 
Others     : 按键功能程序逻辑处理
*************************************************/
void User_Key_Deal(void)
{
    if (SysClockBase_ms.system_1ms == 1 )
    {
        //---------------------------------ECO模式选择开关
        Key_SW_ECO.In = ECOMODE; // 获取IO口状态值
        Key_SW_ECO.Calc( &Key_SW_ECO );

        if  ( Key_SW_ECO.Key_State == KEY_ON )  Controllogic.ecomodeidbit = Full_Speed_Mode;//Save_Energy_Mode;
        else                                    Controllogic.ecomodeidbit = Save_Energy_Mode;//Full_Speed_Mode;
        
        //--------------------------------输出频率选择开关
        Key_SW_FreqChoice.In = FRESWITCH;// 获取IO口状态值
        Key_SW_FreqChoice.Calc( &Key_SW_FreqChoice );
        
        //--------------------------------保护切除选择开关
        Key_Pro_Reset.In = FAULT_RST;// 获取IO口状态值
        Key_Pro_Reset.Calc( &Key_Pro_Reset );
    }
}

/*************************************************
Description: User_CommonFunction
Input      : 
Return     : 
Others     : 用户任务执行
*************************************************/
void User_CommonFunction(void)
{
    Update_AC_Data();//更新电压、电流、功率有效值
    
    StepmotormoveCheck();//步进电机运行
    
    User_LED_Deal();  //LED显示功能
    
    User_Key_Deal(); //按键功能
    
}
