#ifndef __CONTROL_ALGORITHM_H__
#define __CONTROL_ALGORITHM_H__

#include "basic_type.h"
#include "differential_algorithm.h"
#include "math_algorithm.h"

//PR控制器结构体
typedef struct
{
	fp32 fpDes;                 //控制变量目标值
	fp32 fpFB;	                //控制变量反馈值
	fp32 fpLim;	            //抗饱和时外界输入的增益值，用于标幺化

	fp32 fpE;	                //本次偏差
	fp32 fpPreE;                //上次偏差
	fp32 fpPre_PreE;            //上上次偏差
	
	fp32 fpU;	                //本次PID运算结果
    fp32 pre_fpU;               //上次的输出
	fp32 pre_pre_fpU;           //上上次的输出
    
    fp32 output_fpU;            //经过限幅后的输出
    int pre_is_cut;             //上一次限幅情况

	fp32 omiga_0;               //中心频率
    fp32 omiga_c;               //频带宽度    
	fp32 Kp;                    //比例系数
	fp32 Kr;                    //谐振系数
	fp32 fpDt;                  //控制周期
	
    fp32 InvKp;                 //比例系数的倒数
    fp32 n0;
	fp32 n1;
	fp32 n2;
	fp32 d1;
	fp32 d2;
} ST_PR;

void PR_Control(ST_PR *pstPid); //QPR控制器，带抗饱和
void PR_Init(ST_PR *PR);        //计算PR算法离散参数
void PR_Clean(ST_PR *PR);       //重置PR控制器状态

void thph_open_loop(float U_ref,uint16_t index);
void thph_loop(float U_ref, float sin0, float sin240);
void tri_PFC_Loop(float I_ref, float sin0, float sin120, float sin240);

#endif //__CONTROL_ALGORITHM_H__
