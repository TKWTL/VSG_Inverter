/*
 *用户应用文件夹下底层算法统一头文件
 */
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#ifdef __cplusplus
extern "C" {
#endif

//系统
#include "main.h"

//RTOS接口
#include "cmsis_os2.h"

//U8G2库
#include "u8g2.h"
    
//外设驱动库
#include "oled.h"       //OLED(CH1116)驱动库

//按键驱动库
#include "buttons.h"
    
//MiaoUI库
#include "core/ui.h"
#include "display/dispDriver.h"
#include "indev/indevDriver.h"
    
//该层次各应用函数头文件
#include "time.h"
#include "system_operation.h"
#include "system_statistic.h"
#include "display_effects.h"

//开关电源软件组件
#include "conv_adc.h"
#include "conv_algorithm.h"
#include "conv_pwm.h"
#include "differential_algorithm.h"
#include "math_algorithm.h"
#include "control_algorithm.h"
#include "application.h"
#include "filtering_algorithm.h"
#include "conv_protection.h"

#ifdef __cplusplus
}
#endif

#endif
 