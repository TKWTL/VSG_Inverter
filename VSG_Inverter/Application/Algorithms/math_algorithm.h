#ifndef __MATH_ALGORITHM_H__
#define __MATH_ALGORITHM_H__

#include <math.h>
#include "basic_type.h"

extern fp32 SatFunc(fp32 in, fp32 d);
extern fp32 fast_atan2(fp32 y, fp32 x);
extern fp32 invSqrt(fp32 x);
extern void RampSignal(fp32 *p_Output, fp32 DesValue, fp32 Step);
extern fp32 RampSign(fp32 in, fp32 dead);
extern fp32 SineSignal_Output(fp32 A, fp32 f, fp32 acc_tim, fp32 dt);
extern fp32 abs_fl(fp32 in);
extern fp32 Linear_LeastSquare(fp32 *px, fp32 *py, unsigned char order);
extern fp32 my_clip(fp32 fpValue, fp32 fpMin, fp32 fpMax);
extern uint32_t Clip_Uint(uint32_t value, uint32_t fpMin, uint32_t fpMax);
extern int32_t Round(fp32 fp_Round_Number);
extern int32_t Sgn(fp32 in);

#endif //__MATH_ALGORITHM_H__
