
/*****************************************************************************
@Description: sys_state_machine.h 状态机头文件
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

#ifndef __SYS_STATE_MACHINE_H
#define __SYS_STATE_MACHINE_H




#define  CM_WAITING_DELAY_VALUE             200 // (ms)
#define CM_HardWare_Check_Delay_Value_Min   1000 // (ms)
#define CM_HardWare_Check_Delay_Value_Max   4000 // (ms)

/***************************************************************************/
/*----初始态定义-----------------------------------------------------------*/
/***************************************************************************/
struct _Initial_Deal_bits
{
    unsigned short int Initial_Ok:   1;
    unsigned int rsvd:               15;
};
union _Initial_Deal_Flag
{
    unsigned short int               all;
    struct _Initial_Deal_bits        bit;
};
typedef struct
{
    union _Initial_Deal_Flag         flag;
    void                             (*HwInit_FuncPtr)();
    void                             (*SwInit_FuncPtr)();
    void                             (*FuncPtr)();
}_Initial_Deal;

typedef _Initial_Deal *Initial_Deal_Handle;

extern _Initial_Deal  Initial_Deal;
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
#define INITIAL_DEAL_DEFAULTS {  0,                                               \
		                             (void*)0,                                        \
		                             (void*)0,                                        \
	                               (void (*)( unsigned int ))Initial_Deal_Func }
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
void Initial_Deal_Func(Initial_Deal_Handle);
/***************************************************************************/                
                                   
                                   
/***************************************************************************/
/*----等待态定义-----------------------------------------------------------*/
/***************************************************************************/                                 
struct _Waiting_Deal_bits
{
    unsigned int Waiting_Ok:         1;
    unsigned int rsvd:               15;
};

union _Waiting_Deal_Flag
{
    unsigned int                     all;
    struct _Waiting_Deal_bits        bit;
};

typedef struct
{
    union _Waiting_Deal_Flag         flag;
    unsigned int                     Delay_Count;
    unsigned int                     Delay_Value;
    void                             (*FuncPtr)();
}_Waiting_Deal;

typedef _Waiting_Deal *Waiting_Deal_Handle;
extern _Waiting_Deal  Waiting_Deal;

#define WAITING_DEAL_DEFAULTS {  0,                                               \
		                         0,                                               \
		                         CM_WAITING_DELAY_VALUE,                          \
	                             (void (*)( unsigned int ))Waiting_Deal_Func }

void Waiting_Deal_Func(Waiting_Deal_Handle);
/***************************************************************************/                

/***************************************************************************/
/*----准备态定义-----------------------------------------------------------*/
/***************************************************************************/                                 

struct _Ready_Deal_bits
{
    unsigned short int Ready_Ok:    2;
    unsigned short int rsvd:        14;
};
union _Ready_Deal_Flag
{
    unsigned short int               all;
    struct _Ready_Deal_bits          bit;
};
typedef struct
{
    union _Ready_Deal_Flag           flag;
    unsigned int                     HardWare_Check_Delay_Count;
    unsigned int                     HardWare_Check_Delay_Value_Min;
    unsigned int                     HardWare_Check_Delay_Value_Max;
    void                             (*Rdy_Check_FuncPtr)();
    void                             (*FuncPtr)();
}_Ready_Deal;

typedef _Ready_Deal *Ready_Deal_Handle;
extern _Ready_Deal  Ready_Deal;  
 
#define READY_DEAL_DEFAULTS {  0,                                               \
		                       0,                                               \
		                       CM_HardWare_Check_Delay_Value_Min,               \
		                       CM_HardWare_Check_Delay_Value_Max,               \
		                       (void (*)( unsigned int ))Ready_State_Check,     \
	                           (void (*)( unsigned int ))Ready_Deal_Func }

                                 
void Ready_Deal_Func(Ready_Deal_Handle);
void Ready_State_Check(void);
/***************************************************************************/ 

/***************************************************************************/
/*----运行态定义-----------------------------------------------------------*/
/***************************************************************************/                               
struct _Run_Deal_bits
{
    unsigned short int OpenDriver_Ok:   1;
    unsigned short int rsvd:            15;
};
union _Run_Deal_Flag
{
    unsigned short int               all;
    struct _Run_Deal_bits            bit;
};
typedef struct
{
    union _Run_Deal_Flag             flag;
    void                             (*OpenDriver_FuncPtr)();
    void                             (*FaultStateCheck_FuncPtr)();
    void                             (*FaultStateDeal_FuncPtr)();
    void                             (*FuncPtr)();
}_Run_Deal;

typedef _Run_Deal *Run_Deal_Handle;
extern _Run_Deal  Run_Deal;
 
#define RUN_DEAL_DEFAULTS {  0,                                               \
		                     (void (*)( unsigned int ))Run_State_OpenDriver,  \
		                     (void (*)( unsigned int ))Fault_State_Check,     \
	                         (void (*)( unsigned int ))Run_Deal_Func }

void Run_Deal_Func(Run_Deal_Handle);														 
void Run_State_OpenDriver(void);														 
/***************************************************************************/ 
                 
/***************************************************************************/
/*----故障态定义-----------------------------------------------------------*/
/***************************************************************************/                             
struct _Fault_Deal_bits
{
    unsigned short int rsvd:          16;
};
union _Fault_Deal_Flag
{
    unsigned short int                all;
    struct _Fault_Deal_bits           bit;
};
typedef struct
{
    union _Fault_Deal_Flag            flag;
    void                              (*FaultRelease_FuncPtr)();
    void                              (*FaultStateCheck_FuncPtr)();
    void                              (*FuncPtr)();
}_Fault_Deal;
typedef _Fault_Deal *Fault_Deal_Handle;
extern _Fault_Deal  Fault_Deal;

#define FAULT_DEAL_DEFAULTS {  0,                                          \
                               (void (*)( unsigned int ))Protect_Release,  \
                               (void (*)( unsigned int ))Fault_State_Check,\
	                           (void (*)( unsigned int ))Fault_Deal_Func }

void Fault_Deal_Func(Fault_Deal_Handle);
void Protect_Release(void);
void Fault_State_Check(void);
void Fault_State_Deal(void);
/***************************************************************************/ 
    
/***************************************************************************/
/*----停止态定义-----------------------------------------------------------*/
/***************************************************************************/                                 
struct _Stop_Deal_bits
{
	unsigned int rsvd:                 16;
};
union _Stop_Deal_Flag
{
    unsigned int                      all;
    struct _Stop_Deal_bits            bit;
};
typedef struct
{
    union _Stop_Deal_Flag             flag;
    void                             (*FuncPtr)();
}_Stop_Deal;

typedef _Stop_Deal *Stop_Deal_Handle;
extern _Stop_Deal  Stop_Deal;

#define STOP_DEAL_DEFAULTS {    0,                                               \
	                            (void (*)( unsigned long ))Stop_Deal_Func }

void Stop_Deal_Func(Stop_Deal_Handle);
/***************************************************************************/ 

                                
/***************************************************************************/
/*----状态机定义-----------------------------------------------------------*/
/***************************************************************************/
#define  CM_INITIAL_STATE        0
#define  CM_WAITING_STATE        1
#define  CM_READY_STATE          2
#define  CM_RUN_STATE            3
#define  CM_FAULT_STATE          4
#define  CM_STOP_STATE           5
/*----------------------------------------------------------------------------------*/
struct _State_Context_bits
{
	  unsigned short int Initial_Ok:         1;
	  unsigned short int Waiting_Ok:         1;
	  unsigned short int Ready_Ok:           2;
	  unsigned short int Stop_Instruction:   1;
	  unsigned short int Fault_Occur:        1;
	  unsigned short int Fault_Release:      1;
	  unsigned short int rsvd:               9;
};
union _State_Context_Flag
{
    unsigned short int                     all;
    struct _State_Context_bits             bit;
};
typedef struct
{
	  union _State_Context_Flag        flag;
	  unsigned short int               State_Identifier;
    void                             (*FuncPtr[6])();
    void                             (*ContextPtr)();
}_State_Context;

typedef _State_Context *State_Context_Handle;

extern _State_Context  State_Context;
/*----------------------------------------------------------------------------------*/
#define STATE_CONTEXT_DEFAULTS { 0,                                               \
		                             CM_INITIAL_STATE,                                \
                                 (void (*)( unsigned int ))Initial_Deal_Func,    \
                                 (void (*)( unsigned int ))Waiting_Deal_Func,    \
                                 (void (*)( unsigned int ))Ready_Deal_Func,      \
                                 (void (*)( unsigned int ))Run_Deal_Func,        \
                                 (void (*)( unsigned int ))Fault_Deal_Func,      \
                                 (void (*)( unsigned int ))Stop_Deal_Func,       \
	                               (void (*)( unsigned int ))State_Context_Func }
/*----------------------------------------------------------------------------------*/
void State_Context_Func(State_Context_Handle);                                

void Sys_StateMachine_Task(void);


#endif
