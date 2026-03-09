/*******************************************************************************
负责数字电源环路及外设的进一步配置和初始化
之后循环进行时间更新和低速ADC采样，周期400ms
*******************************************************************************/
#include "application.h"

extern ST_PR PR_A, PR_B, PFC_PR_A, PFC_PR_B, PFC_PR_C;

//在调度器启动后才开始初始化开关电源相关步骤
void ADC_Task(void *argument)
{
    Protection_Init1(&ui);
    Protection_Init2(&ui);
    
    //ADC_Init();
    
    //初始化PR差分方程系数，必须在环路开始前执行 
    PR_Init(&PR_A);
    PR_Init(&PR_B);
    PR_Init(&PFC_PR_A);
    PR_Init(&PFC_PR_B);
    PR_Init(&PFC_PR_C);
    
    //Timer_Init(&ui);
    
    for(;;)
    {
        ADC_SampleandFilter();
        RTC_UpdateTime();//时间也放在此处更新
    }
}