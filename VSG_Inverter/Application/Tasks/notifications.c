/*
 *负责实现弹窗功能的任务
 *单独作为任务是为了可以打断其他阻塞式的任务函数
 */
#include "application.h"

void Notification_Task(void){
    while(1){
        Notification_Process();
        osDelay(pdMS_TO_TICKS(100));
    }
}
