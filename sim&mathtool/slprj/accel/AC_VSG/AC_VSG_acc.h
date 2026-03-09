#ifndef RTW_HEADER_AC_VSG_acc_h_
#define RTW_HEADER_AC_VSG_acc_h_
#ifndef AC_VSG_acc_COMMON_INCLUDES_
#define AC_VSG_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn
#define S_FUNCTION_LEVEL 2
#ifndef RTW_GENERATED_S_FUNCTION
#define RTW_GENERATED_S_FUNCTION
#endif
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "AC_VSG_acc_types.h"
#include <stddef.h>
#include <float.h>
#include "mwmathutil.h"
#include "rtGetInf.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { real_T B_0_1_0 ; real_T B_0_4_0 ; } B_PR_AC_VSG_T ; typedef
struct { int32_T PR_sysIdxToRun ; char_T pad_PR_sysIdxToRun [ 4 ] ; }
DW_PR_AC_VSG_T ; typedef struct { real_T TransferFcn_CSTATE_e [ 2 ] ; }
X_PR_AC_VSG_T ; typedef struct { real_T TransferFcn_CSTATE_e [ 2 ] ; }
XDot_PR_AC_VSG_T ; typedef struct { boolean_T TransferFcn_CSTATE_e [ 2 ] ; }
XDis_PR_AC_VSG_T ; typedef struct { real_T B_4_0_0 ; real_T B_4_3_0 ; real_T
B_4_5_0 ; real_T B_4_7_0 ; real_T B_4_8_0 ; real_T B_4_11_0 ; real_T B_4_13_0
; real_T B_4_15_0 ; real_T B_4_19_0 ; real_T B_4_21_0 ; real_T B_4_22_0 ; }
B_RMS_AC_VSG_T ; typedef struct { real_T Memory_PreviousInput ; real_T
Memory_PreviousInput_c ; struct { real_T modelTStart ; } TransportDelay_RWORK
; struct { real_T modelTStart ; } TransportDelay_RWORK_g ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; struct { void * TUbufferPtrs [
2 ] ; } TransportDelay_PWORK_j ; int32_T RMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_j ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_g ; int8_T
RMS_SubsysRanBC ; boolean_T RMS_MODE ; char_T pad_RMS_MODE [ 2 ] ; }
DW_RMS_AC_VSG_T ; typedef struct { real_T integrator_CSTATE_ld ; real_T
integrator_CSTATE_f ; } X_RMS_AC_VSG_T ; typedef struct { real_T
integrator_CSTATE_ld ; real_T integrator_CSTATE_f ; } XDot_RMS_AC_VSG_T ;
typedef struct { boolean_T integrator_CSTATE_ld ; boolean_T
integrator_CSTATE_f ; } XDis_RMS_AC_VSG_T ; typedef struct { real_T B_6_1_0 ;
real_T B_6_3_0 ; real_T B_6_4_0 ; real_T B_6_7_0 ; real_T B_6_8_0 ; real_T
B_6_10_0 ; } B_TrueRMS_AC_VSG_T ; typedef struct { real_T
Memory_PreviousInput ; struct { real_T modelTStart ; } TransportDelay_RWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; int32_T
TrueRMS_sysIdxToRun ; int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; int8_T TrueRMS_SubsysRanBC ;
boolean_T TrueRMS_MODE ; char_T pad_TrueRMS_MODE [ 2 ] ; }
DW_TrueRMS_AC_VSG_T ; typedef struct { real_T integrator_CSTATE_n ; }
X_TrueRMS_AC_VSG_T ; typedef struct { real_T integrator_CSTATE_n ; }
XDot_TrueRMS_AC_VSG_T ; typedef struct { boolean_T integrator_CSTATE_n ; }
XDis_TrueRMS_AC_VSG_T ; typedef struct { real_T B_24_0_0 ; real_T B_24_4_0 ;
real_T B_24_10_0 ; real_T B_24_14_0 ; real_T B_24_31_0 [ 2 ] ; real_T
B_24_34_0 [ 16 ] ; real_T B_24_34_1 [ 8 ] ; real_T B_24_35_0 ; real_T
B_24_40_0 ; real_T B_24_43_0 ; real_T B_24_45_0 ; real_T B_24_47_0 ; real_T
B_24_57_0 ; real_T B_24_59_0 ; real_T B_24_63_0 ; real_T B_24_66_0 ; real_T
B_24_70_0 ; real_T B_24_72_0 ; real_T B_24_76_0 ; real_T B_24_86_0 [ 4 ] ;
real_T B_24_98_0 ; real_T B_24_100_0 ; real_T B_24_102_0 ; real_T B_24_103_0
; real_T B_24_105_0 ; real_T B_24_107_0 ; real_T B_24_111_0 ; real_T
B_24_118_0 ; real_T B_24_121_0 ; real_T B_24_129_0 ; real_T B_24_138_0 ;
real_T B_24_142_0 ; real_T B_24_148_0 ; real_T B_24_152_0 ; real_T B_24_169_0
[ 2 ] ; real_T B_24_172_0 ; real_T B_24_177_0 ; real_T B_24_180_0 ; real_T
B_24_182_0 ; real_T B_24_184_0 ; real_T B_24_194_0 ; real_T B_24_196_0 ;
real_T B_24_197_0 ; real_T B_24_202_0 ; real_T B_24_208_0 ; real_T B_24_212_0
; real_T B_24_222_0 [ 4 ] ; real_T B_24_234_0 ; real_T B_24_236_0 ; real_T
B_24_238_0 ; real_T B_24_239_0 ; real_T B_24_241_0 ; real_T B_24_243_0 ;
real_T B_24_247_0 ; real_T B_24_254_0 ; real_T B_24_257_0 ; real_T B_24_265_0
; real_T B_24_0_0_m ; real_T B_24_1_0 [ 4 ] ; real_T B_24_2_0 ; real_T
B_24_3_0 ; real_T B_24_10_0_c ; real_T B_24_11_0 ; real_T B_24_12_0 ; real_T
B_24_13_0 ; real_T B_24_15_0 ; real_T B_24_16_0 ; real_T B_24_17_0 [ 4 ] ;
real_T B_24_18_0 ; real_T B_24_19_0 ; real_T B_24_26_0 ; real_T B_24_27_0 ;
real_T B_24_28_0 ; real_T B_24_29_0 ; real_T B_24_31_0_k ; real_T B_24_32_0 ;
real_T B_24_33_0 ; boolean_T B_24_5_0 ; boolean_T B_24_6_0 ; boolean_T
B_24_8_0 ; boolean_T B_24_9_0 ; boolean_T B_24_21_0 ; boolean_T B_24_22_0 ;
boolean_T B_24_24_0 ; boolean_T B_24_25_0 ; B_TrueRMS_AC_VSG_T TrueRMS_b ;
B_RMS_AC_VSG_T RMS_m ; B_TrueRMS_AC_VSG_T TrueRMS_c ; B_RMS_AC_VSG_T RMS_h ;
B_PR_AC_VSG_T PR_l ; B_TrueRMS_AC_VSG_T TrueRMS_o ; B_RMS_AC_VSG_T RMS_p ;
B_TrueRMS_AC_VSG_T TrueRMS ; B_RMS_AC_VSG_T RMS ; B_PR_AC_VSG_T PR ; }
B_AC_VSG_T ; typedef struct { real_T StateSpace_DSTATE [ 6 ] ; real_T
Memory_PreviousInput ; real_T Memory_PreviousInput_k ; struct { real_T
modelTStart ; } T_RWORK ; struct { real_T modelTStart ; } T1_RWORK ; struct {
real_T modelTStart ; } T_RWORK_n ; struct { real_T modelTStart ; } T1_RWORK_n
; struct { real_T modelTStart ; } TransportDelay_RWORK ; struct { real_T
modelTStart ; } T_RWORK_i ; struct { real_T modelTStart ; } T1_RWORK_m ;
struct { real_T modelTStart ; } T_RWORK_h ; struct { real_T modelTStart ; }
T1_RWORK_a ; struct { real_T modelTStart ; } TransportDelay_RWORK_d ; struct
{ void * TUbufferPtrs [ 2 ] ; } T_PWORK ; struct { void * TUbufferPtrs [ 2 ]
; } T1_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; } T_PWORK_k ; struct {
void * TUbufferPtrs [ 2 ] ; } T1_PWORK_k ; struct { void * AS ; void * BS ;
void * CS ; void * DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void *
TMP2 ; void * XTMP ; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void
* SW_CHG ; void * G_STATE ; void * USWLAST ; void * XKM12 ; void * XKP12 ;
void * XLAST ; void * ULAST ; void * IDX_SW_CHG ; void * Y_SWITCH ; void *
SWITCH_TYPES ; void * IDX_OUT_SW ; void * SWITCH_TOPO_SAVED_IDX ; void *
SWITCH_MAP ; } StateSpace_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK ; void * Scope5_PWORK [ 2 ] ; void * Scope1_PWORK [ 2 ]
; void * Scope40_PWORK [ 2 ] ; void * Scope66_PWORK ; void * Scope6_PWORK ;
void * Scope7_PWORK ; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } T_PWORK_f ; struct { void * TUbufferPtrs [ 2 ] ; }
T1_PWORK_o ; struct { void * TUbufferPtrs [ 2 ] ; } T_PWORK_n ; struct { void
* TUbufferPtrs [ 2 ] ; } T1_PWORK_b ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_f ; void * Scope3_PWORK [ 2 ] ; void * Scope4_PWORK [ 2
] ; void * Scope6_PWORK_b ; void * Scope6_PWORK_d ; void * Scope7_PWORK_p ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_j ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } T1_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_o ; struct { int_T
Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize
; } T1_IWORK_p ; int_T StateSpace_IWORK [ 11 ] ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_d ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
T1_IWORK_c ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_k ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
T1_IWORK_e ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_e ; char_T
pad_TransportDelay_IWORK_e [ 4 ] ; DW_TrueRMS_AC_VSG_T TrueRMS_b ;
DW_RMS_AC_VSG_T RMS_m ; DW_TrueRMS_AC_VSG_T TrueRMS_c ; DW_RMS_AC_VSG_T RMS_h
; DW_PR_AC_VSG_T PR_l ; DW_TrueRMS_AC_VSG_T TrueRMS_o ; DW_RMS_AC_VSG_T RMS_p
; DW_TrueRMS_AC_VSG_T TrueRMS ; DW_RMS_AC_VSG_T RMS ; DW_PR_AC_VSG_T PR ; }
DW_AC_VSG_T ; typedef struct { real_T integ1_CSTATE ; real_T Integ2_CSTATE ;
real_T integ1_CSTATE_f ; real_T Integ2_CSTATE_k ; real_T integrator_CSTATE ;
real_T TransferFcn_CSTATE ; real_T Integrator1_CSTATE ; real_T
Integrator4_CSTATE ; real_T integ1_CSTATE_l ; real_T Integ2_CSTATE_b ; real_T
integ1_CSTATE_j ; real_T Integ2_CSTATE_e ; real_T integrator_CSTATE_l ;
real_T TransferFcn_CSTATE_a ; real_T Integrator1_CSTATE_l ; real_T
Integrator4_CSTATE_d ; X_TrueRMS_AC_VSG_T TrueRMS_b ; X_RMS_AC_VSG_T RMS_m ;
X_TrueRMS_AC_VSG_T TrueRMS_c ; X_RMS_AC_VSG_T RMS_h ; X_PR_AC_VSG_T PR_l ;
X_TrueRMS_AC_VSG_T TrueRMS_o ; X_RMS_AC_VSG_T RMS_p ; X_TrueRMS_AC_VSG_T
TrueRMS ; X_RMS_AC_VSG_T RMS ; X_PR_AC_VSG_T PR ; } X_AC_VSG_T ; typedef
int_T PeriodicIndX_AC_VSG_T [ 2 ] ; typedef real_T PeriodicRngX_AC_VSG_T [ 4
] ; typedef struct { real_T integ1_CSTATE ; real_T Integ2_CSTATE ; real_T
integ1_CSTATE_f ; real_T Integ2_CSTATE_k ; real_T integrator_CSTATE ; real_T
TransferFcn_CSTATE ; real_T Integrator1_CSTATE ; real_T Integrator4_CSTATE ;
real_T integ1_CSTATE_l ; real_T Integ2_CSTATE_b ; real_T integ1_CSTATE_j ;
real_T Integ2_CSTATE_e ; real_T integrator_CSTATE_l ; real_T
TransferFcn_CSTATE_a ; real_T Integrator1_CSTATE_l ; real_T
Integrator4_CSTATE_d ; XDot_TrueRMS_AC_VSG_T TrueRMS_b ; XDot_RMS_AC_VSG_T
RMS_m ; XDot_TrueRMS_AC_VSG_T TrueRMS_c ; XDot_RMS_AC_VSG_T RMS_h ;
XDot_PR_AC_VSG_T PR_l ; XDot_TrueRMS_AC_VSG_T TrueRMS_o ; XDot_RMS_AC_VSG_T
RMS_p ; XDot_TrueRMS_AC_VSG_T TrueRMS ; XDot_RMS_AC_VSG_T RMS ;
XDot_PR_AC_VSG_T PR ; } XDot_AC_VSG_T ; typedef struct { boolean_T
integ1_CSTATE ; boolean_T Integ2_CSTATE ; boolean_T integ1_CSTATE_f ;
boolean_T Integ2_CSTATE_k ; boolean_T integrator_CSTATE ; boolean_T
TransferFcn_CSTATE ; boolean_T Integrator1_CSTATE ; boolean_T
Integrator4_CSTATE ; boolean_T integ1_CSTATE_l ; boolean_T Integ2_CSTATE_b ;
boolean_T integ1_CSTATE_j ; boolean_T Integ2_CSTATE_e ; boolean_T
integrator_CSTATE_l ; boolean_T TransferFcn_CSTATE_a ; boolean_T
Integrator1_CSTATE_l ; boolean_T Integrator4_CSTATE_d ; XDis_TrueRMS_AC_VSG_T
TrueRMS_b ; XDis_RMS_AC_VSG_T RMS_m ; XDis_TrueRMS_AC_VSG_T TrueRMS_c ;
XDis_RMS_AC_VSG_T RMS_h ; XDis_PR_AC_VSG_T PR_l ; XDis_TrueRMS_AC_VSG_T
TrueRMS_o ; XDis_RMS_AC_VSG_T RMS_p ; XDis_TrueRMS_AC_VSG_T TrueRMS ;
XDis_RMS_AC_VSG_T RMS ; XDis_PR_AC_VSG_T PR ; } XDis_AC_VSG_T ; struct
P_PR_AC_VSG_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 [ 2 ] ; real_T P_3 [ 2
] ; real_T P_4 ; real_T P_5 ; } ; struct P_RMS_AC_VSG_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T
P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ;
real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; } ; struct
P_TrueRMS_AC_VSG_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ;
real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ; } ; struct
P_AC_VSG_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4
; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T
P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ;
real_T P_16 ; real_T P_17 ; real_T P_18 [ 2 ] ; real_T P_19 [ 36 ] ; real_T
P_20 [ 2 ] ; real_T P_21 [ 60 ] ; real_T P_22 [ 2 ] ; real_T P_23 [ 96 ] ;
real_T P_24 [ 2 ] ; real_T P_25 [ 160 ] ; real_T P_26 [ 2 ] ; real_T P_27 [ 6
] ; real_T P_28 ; real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T P_32 ;
real_T P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T
P_38 ; real_T P_39 ; real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T P_43 ;
real_T P_44 ; real_T P_45 ; real_T P_46 [ 3 ] ; real_T P_47 [ 3 ] ; real_T
P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ;
real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T
P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T P_63 ; real_T P_64 ;
real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ; real_T P_69 ; real_T
P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T P_74 ; real_T P_75 ;
real_T P_76 ; real_T P_77 ; real_T P_78 ; real_T P_79 ; real_T P_80 ; real_T
P_81 ; real_T P_82 ; real_T P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ;
real_T P_87 ; real_T P_88 ; real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T
P_92 ; real_T P_93 ; real_T P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ;
real_T P_98 ; real_T P_99 ; real_T P_100 ; real_T P_101 ; real_T P_102 ;
real_T P_103 ; real_T P_104 ; real_T P_105 ; real_T P_106 ; real_T P_107 ;
real_T P_108 [ 3 ] ; real_T P_109 [ 3 ] ; real_T P_110 ; real_T P_111 ;
real_T P_112 ; real_T P_113 ; real_T P_114 ; real_T P_115 ; real_T P_116 ;
real_T P_117 ; real_T P_118 ; real_T P_119 ; real_T P_120 ; real_T P_121 ;
real_T P_122 ; real_T P_123 ; real_T P_124 ; real_T P_125 ; real_T P_126 ;
real_T P_127 ; real_T P_128 ; real_T P_129 ; real_T P_130 ; real_T P_131 ;
real_T P_132 ; real_T P_133 ; real_T P_134 ; real_T P_135 ; real_T P_136 ;
real_T P_137 [ 4 ] ; real_T P_138 ; real_T P_139 ; real_T P_140 ; real_T
P_141 ; real_T P_142 ; real_T P_143 ; real_T P_144 ; real_T P_145 ; real_T
P_146 ; real_T P_147 ; real_T P_148 ; real_T P_149 [ 4 ] ; real_T P_150 ;
real_T P_151 ; real_T P_152 ; real_T P_153 ; real_T P_154 ; real_T P_155 ;
real_T P_156 ; real_T P_157 ; real_T P_158 ; real_T P_159 ; real_T P_160 ;
real_T P_161 ; P_TrueRMS_AC_VSG_T TrueRMS_b ; P_RMS_AC_VSG_T RMS_m ;
P_TrueRMS_AC_VSG_T TrueRMS_c ; P_RMS_AC_VSG_T RMS_h ; P_PR_AC_VSG_T PR_l ;
P_TrueRMS_AC_VSG_T TrueRMS_o ; P_RMS_AC_VSG_T RMS_p ; P_TrueRMS_AC_VSG_T
TrueRMS ; P_RMS_AC_VSG_T RMS ; P_PR_AC_VSG_T PR ; } ; extern P_AC_VSG_T
AC_VSG_rtDefaultP ;
#endif
