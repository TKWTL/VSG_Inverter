#include "application.h"

//弹窗显示时间（单位：毫秒）
int notification_showtime = 1500;

//向弹窗队列发送一条消息
void Notification_Send(const char* p){
    Beep_Send(1);
    osMessageQueuePut(Notification_QueueHandle, &p, NULL, 0);
}

/*
 *弹窗功能实现任务函数
 *循环检查队列，无内容则阻塞线程
 *有内容时，阻塞Menu_Task，内容的值作为索引值查询notifications_draw.h中定义的字符串数组，并通过动画效果显示出来
 *在用户按键或者到达了系统设置的展示时间后退出
 */
void Notification_Process(void){
    osStatus_t status;
    const char *p_notification;
    uint8_t bgC = ui.bgColor;
    int notification_timer;
    
    status = osMessageQueueGet(Notification_QueueHandle, &p_notification, NULL, 0);
    if(status == osOK){
        Disp_SendBuffer();
        vTaskSuspend(MenuHandle);
        //显示
        PopUp_Show(2, 10, 122, 42);
        Disp_SetFont(font_menu_main_h12w6);
        Disp_DrawStr(8, 36, p_notification);
        Disp_SendBuffer();
        
        //延时展示，有按键时退出
        for(notification_timer = notification_showtime; notification_timer > 0; notification_timer -= 100){
            if(indevScan() != UI_ACTION_NONE) break;
            osDelay(pdMS_TO_TICKS(100));
        }
        
        vTaskResume(MenuHandle);
    }
}
    