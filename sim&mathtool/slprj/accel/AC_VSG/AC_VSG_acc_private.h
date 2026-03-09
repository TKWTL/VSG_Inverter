#ifndef RTW_HEADER_AC_VSG_acc_private_h_
#define RTW_HEADER_AC_VSG_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "AC_VSG_acc.h"
#include "AC_VSG_acc_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)     if(!(ptr)) {\
    ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
    }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((ptr));\
    (ptr) = (NULL);\
    }
#else
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((void *)(ptr));\
    (ptr) = (NULL);\
    }
#endif
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr , int_T
* tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay , real_T *
* uBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay , int_T *
maxNewBufSzPtr ) ; real_T AC_VSG_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx ,
int_T oldestIdx , int_T newIdx , real_T initOutput , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput ) ; extern real_T look1_binlxpw (
real_T u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex
) ; void AC_VSG_PR_Init ( SimStruct * S , X_PR_AC_VSG_T * localX ) ; void
AC_VSG_PR_Deriv ( SimStruct * S , B_PR_AC_VSG_T * localB , P_PR_AC_VSG_T *
localP , X_PR_AC_VSG_T * localX , XDot_PR_AC_VSG_T * localXdot ) ; void
AC_VSG_PR ( SimStruct * S , real_T rtu_err , B_PR_AC_VSG_T * localB ,
P_PR_AC_VSG_T * localP , X_PR_AC_VSG_T * localX ) ; void AC_VSG_RMS_Init (
SimStruct * S , B_RMS_AC_VSG_T * localB , DW_RMS_AC_VSG_T * localDW ,
P_RMS_AC_VSG_T * localP , X_RMS_AC_VSG_T * localX ) ; void AC_VSG_RMS_Deriv (
SimStruct * S , B_RMS_AC_VSG_T * localB , DW_RMS_AC_VSG_T * localDW ,
XDot_RMS_AC_VSG_T * localXdot ) ; void AC_VSG_RMS_Disable ( SimStruct * S ,
DW_RMS_AC_VSG_T * localDW ) ; void AC_VSG_RMS_Update ( SimStruct * S ,
B_RMS_AC_VSG_T * localB , DW_RMS_AC_VSG_T * localDW , P_RMS_AC_VSG_T * localP
) ; void AC_VSG_RMS ( SimStruct * S , boolean_T rtu_Enable , real_T rtu_In ,
B_RMS_AC_VSG_T * localB , DW_RMS_AC_VSG_T * localDW , P_RMS_AC_VSG_T * localP
, X_RMS_AC_VSG_T * localX ) ; void AC_VSG_TrueRMS_Init ( SimStruct * S ,
B_TrueRMS_AC_VSG_T * localB , DW_TrueRMS_AC_VSG_T * localDW ,
P_TrueRMS_AC_VSG_T * localP , X_TrueRMS_AC_VSG_T * localX ) ; void
AC_VSG_TrueRMS_Deriv ( SimStruct * S , B_TrueRMS_AC_VSG_T * localB ,
DW_TrueRMS_AC_VSG_T * localDW , XDot_TrueRMS_AC_VSG_T * localXdot ) ; void
AC_VSG_TrueRMS_Disable ( SimStruct * S , DW_TrueRMS_AC_VSG_T * localDW ) ;
void AC_VSG_TrueRMS_Update ( SimStruct * S , B_TrueRMS_AC_VSG_T * localB ,
DW_TrueRMS_AC_VSG_T * localDW , P_TrueRMS_AC_VSG_T * localP ) ; void
AC_VSG_TrueRMS ( SimStruct * S , boolean_T rtu_Enable , real_T rtu_In ,
B_TrueRMS_AC_VSG_T * localB , DW_TrueRMS_AC_VSG_T * localDW ,
P_TrueRMS_AC_VSG_T * localP , X_TrueRMS_AC_VSG_T * localX ) ; void
AC_VSG_PR_Term ( SimStruct * const S ) ; void AC_VSG_RMS_Term ( SimStruct *
const S ) ; void AC_VSG_TrueRMS_Term ( SimStruct * const S ) ;
#endif
