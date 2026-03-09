#include "application.h"

#define DUTY_MAX    0.998f
#define DUTY_MIN    0.002f
#define HRTIM_CMP_FLOOR_LIMIT   0x30U                                           //根据手册RM0440来确定指定倍频/分频数下的值

void Timer_Init(ui_t *ui){
    SetFrequency(ui);
	LL_HRTIM_TIM_CounterEnable(HRTIM1,  LL_HRTIM_TIMER_MASTER|
                                        LL_HRTIM_TIMER_A|
                                        LL_HRTIM_TIMER_B|
                                        LL_HRTIM_TIMER_C|
                                        LL_HRTIM_TIMER_D|
                                        LL_HRTIM_TIMER_E|
                                        LL_HRTIM_TIMER_F
    );
    LL_HRTIM_EnableIT_REP(HRTIM1, LL_HRTIM_TIMER_D); //注意使能中断
}

//重装载值96~65503
//比较值0~65503
RAMVAR uint32_t g_Timer_Period;//周期值（未减去1）
RAMVAR uint32_t g_Timer_CompareMax;//比较器限幅值，由HRTIM的缺陷而产生

int SwitchingFrequency = DEFAULT_FSW_KHZ;

//设置开关频率，只保证两个很近频率间切换的稳定性
//必须打开定时器预装载功能和repetition counter，否则中心对齐的PWM将出现相位反转！！！
void SetFrequency(ui_t *ui){
    g_Timer_Period = 170000UL* 8UL/ SwitchingFrequency;
    if(g_Timer_Period > 65536U- HRTIM_CMP_FLOOR_LIMIT) g_Timer_Period = 65536U- HRTIM_CMP_FLOOR_LIMIT;
    else if(g_Timer_Period < HRTIM_CMP_FLOOR_LIMIT) g_Timer_Period = HRTIM_CMP_FLOOR_LIMIT;
    //更改重装载值
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_A, g_Timer_Period);
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_B, g_Timer_Period);
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_C, g_Timer_Period);
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_D, g_Timer_Period);
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_E, g_Timer_Period);
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_F, g_Timer_Period);
    //更改同步触发
    LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMER_MASTER, g_Timer_Period - 1);
    LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_MASTER, (g_Timer_Period / 2) - 1);//逆变器组定时器复位设置
    LL_HRTIM_TIM_SetCompare2(HRTIM1, LL_HRTIM_TIMER_MASTER, (g_Timer_Period / 2) - 1);//整流器组定时器复位设置
    //更改比较器限幅值
    g_Timer_CompareMax = g_Timer_Period - HRTIM_CMP_FLOOR_LIMIT;
}

//设置占空比，根据周期和输入的百分比值，带占空比限幅
RAMFUNC void SetDuty(uint8_t Timer_Set, float Du, float Dv, float Dw){
    uint32_t compare_u, compare_v, compare_w, compare_max = g_Timer_CompareMax;
    float Period = g_Timer_Period;
    
    compare_u = (uint32_t)(Period* Du);
    compare_v = (uint32_t)(Period* Dv);
    compare_w = (uint32_t)(Period* Dw);
    //由于HRTIM的缺陷，需要进行限幅
    if(compare_u < HRTIM_CMP_FLOOR_LIMIT) compare_u = HRTIM_CMP_FLOOR_LIMIT;
    if(compare_u > compare_max) compare_u = compare_max;
    if(compare_v < HRTIM_CMP_FLOOR_LIMIT) compare_v = HRTIM_CMP_FLOOR_LIMIT;
    if(compare_v > compare_max) compare_v = compare_max;
    if(compare_w < HRTIM_CMP_FLOOR_LIMIT) compare_w = HRTIM_CMP_FLOOR_LIMIT;
    if(compare_w > compare_max) compare_w = compare_max;
    //更改比较值	
    if(Timer_Set == PWM_OF_INVERTER){//逆变器组
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_D, compare_u);
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_E, compare_v);
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_F, compare_w);
    }
    else{//整流器组
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_A, compare_u);
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, compare_v);
        LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_C, compare_w);
    }
}

//SVPWM，已弃用，仅作原理展示
void SetDuty_SV(uint8_t Timer_Set,float offset,float Du,float Dv,float Dw){
    float sv_dmax, sv_dmin;
    
    Du = (Du+ offset)/ 2;
    Dv = (Dv+ offset)/ 2;
    Dw = (Dw+ offset)/ 2;
    
    sv_dmax = fmaxf(Du, fmaxf(Dv, Dw));
    sv_dmin = fminf(Du, fminf(Dv, Dw));
    
    Du = Du+ (offset- sv_dmax- sv_dmin)/ 2;
    Dv = Dv+ (offset- sv_dmax- sv_dmin)/ 2;
    Dw = Dw+ (offset- sv_dmax- sv_dmin)/ 2;
    
    SetDuty(Timer_Set,Du,Dv,Dw);
}


//打开PWM
inline void PWM_Start_Inv(void){
    LL_HRTIM_EnableOutput(HRTIM1, LL_HRTIM_OUTPUT_TD1 | LL_HRTIM_OUTPUT_TD2 |
                                  LL_HRTIM_OUTPUT_TE1 | LL_HRTIM_OUTPUT_TE2 |
                                  LL_HRTIM_OUTPUT_TF1 | LL_HRTIM_OUTPUT_TF2);
}
inline void PWM_Start_Rec(void){
    LL_HRTIM_EnableOutput(HRTIM1, LL_HRTIM_OUTPUT_TA1 | LL_HRTIM_OUTPUT_TA2 |
                                  LL_HRTIM_OUTPUT_TB1 | LL_HRTIM_OUTPUT_TB2 |
                                  LL_HRTIM_OUTPUT_TC1 | LL_HRTIM_OUTPUT_TC2);
}
//关闭PWM
inline void PWM_Stop(void){
    LL_HRTIM_DisableOutput(HRTIM1, LL_HRTIM_OUTPUT_TA1 | LL_HRTIM_OUTPUT_TA2 |
                                   LL_HRTIM_OUTPUT_TB1 | LL_HRTIM_OUTPUT_TB2 |
                                   LL_HRTIM_OUTPUT_TC1 | LL_HRTIM_OUTPUT_TC2 |
                                   LL_HRTIM_OUTPUT_TD1 | LL_HRTIM_OUTPUT_TD2 |
                                   LL_HRTIM_OUTPUT_TE1 | LL_HRTIM_OUTPUT_TE2 |
                                   LL_HRTIM_OUTPUT_TF1 | LL_HRTIM_OUTPUT_TF2);
}
    
/***********************以下函数须在ui_conf.c中调用****************************/
ui_item_t Frequency_Item;
//创建PWM设置模块所需的参数
void Create_PWM_Parameters(ui_t *ui){
    static ui_data_t Frequency_data;
    Frequency_data.name = "Switch Frequency";
    Frequency_data.ptr = &SwitchingFrequency;
    Frequency_data.function = SetFrequency;
    Frequency_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Frequency_data.dataType = UI_DATA_INT;
    Frequency_data.actionType = UI_DATA_ACTION_RW;
    Frequency_data.max = 150;
    Frequency_data.min = 20;
    Frequency_data.step = 5;
    static ui_element_t Frequency_element;
    Frequency_element.data = &Frequency_data;
    Create_element(&Frequency_Item, &Frequency_element);
}
//将PWM设置模块的对象添加到菜单中
void Add_PWM_Items(ui_page_t *ParentPage){
    AddItem(" Switch Frequency", UI_ITEM_DATA, NULL, &Frequency_Item, ParentPage, NULL, NULL);
}
