/*****************************************************************************
@Description: stepmotor_ctrl.c 步进电机控制
@Author     : naiva
@Version    : 2025.09.17 
@History    : 2025.09.17  naiva  V2.0.1 创建
@Attention  ：
*****************************************************************************/

/****************************************************************************/
/*----include files---------------------------------------------------------*/
#include "AlgorithmLevel_Headers.h"
#include "user_stepmotor.h"
/***************************************************************************/

/*************************************************
Description: Step_Motor_PI
Input      : 
Return     : 
Others     : 步进电机PI参数调节;入口:比例、积分系数、参考值、反馈值，、限幅值;出口:PI结果

*************************************************/
int32_t Kp_A,Ki_B= 0;
void StepMotor_PI_Ctrl(StepMotor_Ctrl_t *StepMotor_handle)
{

		
	StepMotor_handle->err_n = StepMotor_handle->err;
	StepMotor_handle->err = StepMotor_handle->ref - StepMotor_handle->fdb;
    
//    if(( StepMotor_handle->err > 450))
//            StepMotor_handle->err_n = StepMotor_handle->err_n >> 2;  
    
	if ( StepMotor_handle->err > StepMotor_handle->errMax ) StepMotor_handle->err = StepMotor_handle->errMax;
	if ( StepMotor_handle->err < StepMotor_handle->errMin ) StepMotor_handle->err = StepMotor_handle->errMin;

    Kp_A = ((int32_t) StepMotor_handle->kp  * ( StepMotor_handle->err - StepMotor_handle->err_n ));//比例部分作用
    Ki_B = ((int32_t) StepMotor_handle->ki  * StepMotor_handle->err );//积分部分作用  
                        
    
    if( (Kp_A + Ki_B ) >= 0 )
        StepMotor_handle->out = (Kp_A + Ki_B ) >> 9;       //     (Kp_A + Ki_B )/512             
    else 
        StepMotor_handle->out = -(-(Kp_A + Ki_B) >> 9);                        
    
    /*------------------------------------------------------------------------------------*/
    if ( StepMotor_handle->out > StepMotor_handle->outMax ) StepMotor_handle->out = StepMotor_handle->outMax;
    if ( StepMotor_handle->out < StepMotor_handle->outMin ) StepMotor_handle->out = StepMotor_handle->outMin;
		
}


/*------------------------------------------------------------------------------------*/
/*  No more.                                                                           */
/*------------------------------------------------------------------------------------*/
