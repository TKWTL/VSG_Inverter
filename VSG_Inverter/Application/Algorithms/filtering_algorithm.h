#ifndef __FILTERING_ALGORITHM_H__
#define __FILTERING_ALGORITHM_H__

#ifdef __cplusplus
extern "C" {
#endif
    
#include "basic_type.h"

typedef struct
{
	fp32 preout;
	fp32 out;
	fp32 in;
	fp32 off_freq;
	fp32 samp_tim;
}ST_LPF;

extern void LpFilter(ST_LPF *lpf);

#include "conv_adc.h"

typedef struct {
    //用于设置，需要初始化
    float UpdateThreshold;
    int update_ticks;
    //PowerControlVar_t *pVar;
    //内部变量
    float display_value;
    float integral_error;
    int filter_ticks;
}FilterVar_t;//仪表显示用滤波算法的参数结构体

float VarFilter(FilterVar_t *pFilter, float filter_input);//

#ifdef __cplusplus
}
#endif

#endif // __FILTERING_ALGORITHM_H__
