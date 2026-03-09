#include "control_algorithm.h"
#include "conv_pwm.h"

float OutputA = 0;
float OutputB = 0;
float OutputC = 0;
float Uuv = 0;//UV线电压
float Uwv = 0;//WV线电压
float UuN = 0;//U相电压
float UvN = 0;//V相电压
float gain = 0.0;//电压前馈系数
float soft_k = 0;//软启动控制系数
float thph_offset;//调制中点
float sv_dmax, sv_dmin;//共用的零序分量

//三相开环逆变，带软启动功能
/*RAMFUNC void thph_open_loop(float U_ref, uint16_t index)
{
    float third_harmonic=0;
    
    U_ref /= 1.732f;
    
    OutputA = U_ref* Wave_Table[index]/ 50;
    //third_harmonic = -U_ref* Wave_Table[(index* 3)% 2049]/ 300;
    index += 683;
    if(index >= 2049) index -= 2049;
    OutputC = U_ref* Wave_Table[index]/ 50;
    OutputB = -OutputA-OutputC;
    
    third_harmonic += thph_offset;
		
//	SetDuty(0, third_harmonic+ OutputA, third_harmonic+ OutputB, third_harmonic+ OutputC);
    SetDuty_SV(0,thph_offset,OutputA,OutputB,OutputC);
}*/

/*******************************************************************
函数名称：PR_Controller
函数功能：比例谐振控制器
备    注：实际上是准PR，抗饱和算法使用香港理工大学论文《用于逆变器的比例-谐振控制器的抗饱和方案》
********************************************************************/
RAMFUNC void PR_Control(ST_PR *pstPR)
{
    float PR_Limit = pstPR->fpLim;
    
    pstPR->fpE = (pstPR->pre_is_cut) ? 
                  ((pstPR->output_fpU - pstPR->fpU) * pstPR->InvKp + pstPR->fpPreE) : 
                  (pstPR->fpDes - pstPR->fpFB);
    
	pstPR->fpU =    pstPR->n0* pstPR->fpE+
                    pstPR->n1* pstPR->fpPreE+
                    pstPR->n2* pstPR->fpPre_PreE-
                    pstPR->d1* pstPR->pre_fpU-
                    pstPR->d2* pstPR->pre_pre_fpU;
	
	pstPR->fpPre_PreE = pstPR->fpPreE;//迭代结果
	pstPR->fpPreE = pstPR->fpE; //保存本次偏差
	
    pstPR->output_fpU = fminf(fmaxf(pstPR->fpU, -PR_Limit), PR_Limit);

    pstPR->pre_is_cut = (pstPR->fpU != pstPR->output_fpU);
    
	pstPR->pre_pre_fpU = pstPR->pre_fpU;
	pstPR->pre_fpU = pstPR->fpU;
}

//计算PR算法离散参数/初始化
RAMFUNC void PR_Init(ST_PR *pstPR)
{		
    pstPR->InvKp = 1/ pstPR->Kp;
    pstPR->n0=((4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt)*pstPR->Kp+4*pstPR->Kr*pstPR->fpDt*pstPR->omiga_c)/(4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt);
    pstPR->n1=pstPR->Kp*(2*pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt-8)/(4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt);
    pstPR->n2=((4-4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt)*pstPR->Kp-4*pstPR->Kr*pstPR->fpDt*pstPR->omiga_c)/(4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt);
    pstPR->d1=(2*pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt-8)/(4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt);
    pstPR->d2=(4-4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt)/(4+4*pstPR->fpDt*pstPR->omiga_c+pstPR->omiga_0*pstPR->omiga_0*pstPR->fpDt*pstPR->fpDt);
}

//重置PR控制器状态
RAMFUNC void PR_Clean(ST_PR *pstPR)
{
	pstPR->fpE = 0;
	pstPR->fpPreE = 0;
	pstPR->fpPre_PreE = 0;
	pstPR->fpU = 0;
	pstPR->pre_fpU = 0;
	pstPR->pre_pre_fpU = 0;
    pstPR->output_fpU = 0;
    pstPR->pre_is_cut = 0;
}
	
/*******************************************************************
三相逆变PR结构体，此处调参
********************************************************************/	
RAMVAR ST_PR PR_A={.fpE=0,.fpPreE=0,.fpPre_PreE=0,.fpU=0,.pre_fpU=0,.pre_pre_fpU=0,
	.omiga_c=0.628f,.omiga_0=DEFAULT_AC_OMEGA,.fpDt=CTRL_DT,.Kp=0.1f,.Kr=500             
};		
RAMVAR ST_PR PR_B={.fpE=0,.fpPreE=0,.fpPre_PreE=0,.fpU=0,.pre_fpU=0,.pre_pre_fpU=0,
	.omiga_c=0.628f,.omiga_0=DEFAULT_AC_OMEGA,.fpDt=CTRL_DT,.Kp=0.1f,.Kr=500               
};	

/*******************************************************************
函数名称：void thph_loop
函数功能：三相逆变控制
备    注：自制
********************************************************************/
inline void thph_loop(float U_ref, float sin0, float sin240)
{
    U_ref *= soft_k;                                                            //软启动控制
    if(soft_k < 1.0f) soft_k += SOFTSTART_SLOPE;
    if(thph_offset < 2* MODULATION_MIDPOINT) thph_offset += MODULATION_SLOPE;   //*2的原因是SVPWM算法需要1左右的数作为标准偏置
    
    U_ref *= 1.4142136f/ 1.732051f;
    //参考波形生成
    PR_A.fpDes = U_ref* sin0;
    PR_B.fpDes = U_ref* sin240;
    
    Uuv = ReadControlVar(&UV_INV_t);
    Uwv = ReadControlVar(&WV_INV_t);
    UuN = (Uuv*2-Uwv)/ 3.0f;//线电压转相电压
    UvN = (Uwv+Uuv)/ -3.0f; 
    gain = 1.0f/ ReadControlVar(&VBUS_t);//获取母线电压信息用于抗饱和和标幺化

    PR_A.fpFB = UuN;
    PR_A.fpLim = ReadControlVar(&VBUS_t)* ANTI_WINDUP_LIMIT;
    PR_B.fpFB = UvN;
    PR_B.fpLim = PR_A.fpLim;
    
    PR_Control(&PR_A);
    PR_Control(&PR_B);
    
    //电流前馈与标幺化
    OutputA = (PR_A.output_fpU+ ReadControlVar(&IU_INV_t))* gain;
    OutputB = (PR_B.output_fpU+ ReadControlVar(&IV_INV_t))* gain;
    OutputC = fminf(ANTI_WINDUP_LIMIT, fmaxf(-ANTI_WINDUP_LIMIT, -OutputA- OutputB));
        
    //得到零序分量
    sv_dmax = (fmaxf(OutputA, fmaxf(OutputB, OutputC))+ thph_offset)/ 4;
    sv_dmin = (fminf(OutputA, fminf(OutputB, OutputC))+ thph_offset)/ 4;
    //叠加零序分量
    OutputA = 1- OutputA/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    OutputB = 1- OutputB/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    OutputC = 1- OutputC/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    
    SetDuty(PWM_OF_INVERTER, OutputA, OutputB, OutputC);
}

/*******************************************************************
三相整流PR结构体，此处调参
********************************************************************/
RAMVAR ST_PR PFC_PR_A={.fpE=0,.fpPreE=0,.fpPre_PreE=0,.fpU=0,.pre_fpU=0,.pre_pre_fpU=0,
	.omiga_c=0.628f,.omiga_0 = DEFAULT_AC_OMEGA,.fpDt=CTRL_DT,.Kp=3,.Kr=800              
};		
RAMVAR ST_PR PFC_PR_B={.fpE=0,.fpPreE=0,.fpPre_PreE=0,.fpU=0,.pre_fpU=0,.pre_pre_fpU=0,
	.omiga_c=0.628f,.omiga_0 = DEFAULT_AC_OMEGA,.fpDt=CTRL_DT,.Kp=3,.Kr=800              
};		
RAMVAR ST_PR PFC_PR_C={.fpE=0,.fpPreE=0,.fpPre_PreE=0,.fpU=0,.pre_fpU=0,.pre_pre_fpU=0,
	.omiga_c=0.628f,.omiga_0 = DEFAULT_AC_OMEGA,.fpDt=CTRL_DT,.Kp=3,.Kr=800              
};	

/*******************************************************************
函数名称：void tri_PFC_Loop(float I_ref)
函数功能：进行三相PFC控制
备    注：零序分量（因为直接并联）和U，V，W电压由逆变器端提供（为了节省采样电路功耗），必须在thph_loop()后执行
********************************************************************/
float I_Output_A=0;
float I_Output_B=0;
float I_Output_C=0;
float UuN_P=0;
float UvN_P=0;
float UwN_P=0;
//Ireal = Iset* 1.1f+ 0.03f
//Iset = Ireal* 0.9091f- 0.0273f
inline void tri_PFC_Loop(float I_ref, float sin0, float sin120, float sin240)
{    
    I_ref = I_ref* 0.9091f- 0.0273f;
    I_ref *= soft_k;
    I_ref *= 1.414f;
    
    PFC_PR_A.fpDes = I_ref* sin0;
    PFC_PR_C.fpDes = I_ref* sin120;
    PFC_PR_B.fpDes = I_ref* sin240;
    

    UuN_P = (Uuv* 2- Uwv)/ 3.0f; 
    UvN_P = (Uwv+ Uuv)/ -3.0f;
    UwN_P = (Uuv- 2* Uwv)/ -3.0f;
    
    PFC_PR_A.fpFB = ReadControlVar(&IU_REC_t);
    PFC_PR_A.fpLim = PR_A.fpLim;
    PFC_PR_B.fpFB = ReadControlVar(&IV_REC_t);
    PFC_PR_B.fpLim = PR_A.fpLim;
    PFC_PR_C.fpFB = ReadControlVar(&IW_REC_t);
    PFC_PR_C.fpLim = PR_A.fpLim;
    
    PR_Control(&PFC_PR_A);
    PR_Control(&PFC_PR_B);
    PR_Control(&PFC_PR_C);
    //标幺化
    I_Output_A = (UuN_P- PFC_PR_A.output_fpU)* gain; 
    I_Output_B = (UvN_P- PFC_PR_B.output_fpU)* gain; 
    I_Output_C = (UwN_P- PFC_PR_C.output_fpU)* gain;
    //SVPWM，但是使用与逆变器相等的零序分量
    I_Output_A = 1- I_Output_A/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    I_Output_B = 1- I_Output_B/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    I_Output_C = 1- I_Output_C/ 2- thph_offset+ (sv_dmax+ sv_dmin);
    
    SetDuty(PWM_OF_RECTIFIER, I_Output_A, I_Output_B, I_Output_C);
}

