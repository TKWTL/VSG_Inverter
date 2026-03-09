#include "AC_VSG_acc.h"
#include "rtwtypes.h"
#include "AC_VSG_acc_private.h"
#include "mwmathutil.h"
#include "multiword_types.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr , int_T
* tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay , real_T *
* uBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay , int_T *
maxNewBufSzPtr ) { int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = *
bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ; real_T * xBuf = ( NULL ) ;
int_T numBuffer = 2 ; if ( istransportdelay ) { numBuffer = 3 ; xBuf = *
uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz - 1 ) ) ? ( tail + 1 ) : 0
; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && ! isfixedbuf ) { int_T j ;
real_T * tempT ; real_T * tempU ; real_T * tempX = ( NULL ) ; real_T * uBuf =
* uBufPtr ; int_T newBufSz = bufSz + 1024 ; if ( newBufSz > * maxNewBufSzPtr
) { * maxNewBufSzPtr = newBufSz ; } tempU = ( real_T * ) utMalloc ( numBuffer
* newBufSz * sizeof ( real_T ) ) ; if ( tempU == ( NULL ) ) { return ( false
) ; } tempT = tempU + newBufSz ; if ( istransportdelay ) tempX = tempT +
newBufSz ; for ( j = tail ; j < bufSz ; j ++ ) { tempT [ j - tail ] = tBuf [
j ] ; tempU [ j - tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j -
tail ] = xBuf [ j ] ; } for ( j = 0 ; j < tail ; j ++ ) { tempT [ j + bufSz -
tail ] = tBuf [ j ] ; tempU [ j + bufSz - tail ] = uBuf [ j ] ; if (
istransportdelay ) tempX [ j + bufSz - tail ] = xBuf [ j ] ; } if ( * lastPtr
> tail ) { * lastPtr -= tail ; } else { * lastPtr += ( bufSz - tail ) ; } *
tailPtr = 0 ; * headPtr = bufSz ; utFree ( uBuf ) ; * bufSzPtr = newBufSz ; *
uBufPtr = tempU ; } else { * tailPtr = testIdx ; } return ( true ) ; } real_T
AC_VSG_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T tStart , real_T
* uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx , int_T newIdx ,
real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; real_T * tBuf = uBuf + bufSz ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; real_T yL_0d0 ; uint32_T bpIdx ; uint32_T iLeft ; uint32_T
iRght ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / (
bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx =
maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U )
{ if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [
iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0
- bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; }
yL_0d0 = table [ iLeft ] ; return ( table [ iLeft + 1U ] - yL_0d0 ) * frac +
yL_0d0 ; } void rt_ssGetBlockPath ( SimStruct * S , int_T sysIdx , int_T
blkIdx , char_T * * path ) { _ssGetBlockPath ( S , sysIdx , blkIdx , path ) ;
} void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * castedS = (
SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( castedS ) ) {
_ssSet_slErrMsg ( castedS , diag ) ; } else { _ssDiscardDiagnostic ( castedS
, diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag )
{ _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } void
AC_VSG_PR_Init ( SimStruct * S , X_PR_AC_VSG_T * localX ) { localX ->
TransferFcn_CSTATE_e [ 0 ] = 0.0 ; localX -> TransferFcn_CSTATE_e [ 1 ] = 0.0
; } void AC_VSG_PR ( SimStruct * S , real_T rtu_err , B_PR_AC_VSG_T * localB
, P_PR_AC_VSG_T * localP , X_PR_AC_VSG_T * localX ) { real_T u0 ; localB ->
B_0_1_0 = localP -> P_1 * rtu_err ; u0 = ( localP -> P_3 [ 0 ] * localX ->
TransferFcn_CSTATE_e [ 0 ] + localP -> P_3 [ 1 ] * localX ->
TransferFcn_CSTATE_e [ 1 ] ) + localP -> P_0 * rtu_err ; if ( u0 > localP ->
P_4 ) { localB -> B_0_4_0 = localP -> P_4 ; } else if ( u0 < localP -> P_5 )
{ localB -> B_0_4_0 = localP -> P_5 ; } else { localB -> B_0_4_0 = u0 ; } }
void AC_VSG_PR_Deriv ( SimStruct * S , B_PR_AC_VSG_T * localB , P_PR_AC_VSG_T
* localP , X_PR_AC_VSG_T * localX , XDot_PR_AC_VSG_T * localXdot ) {
localXdot -> TransferFcn_CSTATE_e [ 0 ] = 0.0 ; localXdot ->
TransferFcn_CSTATE_e [ 0 ] += localP -> P_2 [ 0 ] * localX ->
TransferFcn_CSTATE_e [ 0 ] ; localXdot -> TransferFcn_CSTATE_e [ 1 ] = 0.0 ;
localXdot -> TransferFcn_CSTATE_e [ 0 ] += localP -> P_2 [ 1 ] * localX ->
TransferFcn_CSTATE_e [ 1 ] ; localXdot -> TransferFcn_CSTATE_e [ 1 ] +=
localX -> TransferFcn_CSTATE_e [ 0 ] ; localXdot -> TransferFcn_CSTATE_e [ 0
] += localB -> B_0_1_0 ; } void AC_VSG_PR_Term ( SimStruct * const S ) { }
void AC_VSG_RMS_Init ( SimStruct * S , B_RMS_AC_VSG_T * localB ,
DW_RMS_AC_VSG_T * localDW , P_RMS_AC_VSG_T * localP , X_RMS_AC_VSG_T * localX
) { localX -> integrator_CSTATE_ld = localP -> P_3 ; localDW ->
Memory_PreviousInput = localP -> P_7 ; localX -> integrator_CSTATE_f = localP
-> P_8 ; localDW -> Memory_PreviousInput_c = localP -> P_12 ; localB ->
B_4_22_0 = localP -> P_2 ; } void AC_VSG_RMS_Disable ( SimStruct * S ,
DW_RMS_AC_VSG_T * localDW ) { localDW -> RMS_MODE = false ; } void AC_VSG_RMS
( SimStruct * S , boolean_T rtu_Enable , real_T rtu_In , B_RMS_AC_VSG_T *
localB , DW_RMS_AC_VSG_T * localDW , P_RMS_AC_VSG_T * localP , X_RMS_AC_VSG_T
* localX ) { real_T B_4_1_0 ; real_T B_4_9_0 ; int32_T isHit ; boolean_T
rtb_B_4_4_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( ! localDW ->
RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW -> RMS_MODE =
true ; } } else if ( localDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; AC_VSG_RMS_Disable ( S ,
localDW ) ; } } if ( localDW -> RMS_MODE ) { localB -> B_4_0_0 = localX ->
integrator_CSTATE_ld ; { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - localP -> P_4 ; B_4_1_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , localDW ->
TransportDelay_IWORK . CircularBufSize , & localDW -> TransportDelay_IWORK .
Last , localDW -> TransportDelay_IWORK . Tail , localDW ->
TransportDelay_IWORK . Head , localP -> P_5 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW -> TransportDelay_IWORK .
CircularBufSize ) [ localDW -> TransportDelay_IWORK . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB ->
B_4_3_0 = localP -> P_6 ; } rtb_B_4_4_0 = ( ssGetT ( S ) >= localB -> B_4_3_0
) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB ->
B_4_5_0 = localDW -> Memory_PreviousInput ; } if ( rtb_B_4_4_0 ) { localB ->
B_4_7_0 = ( localB -> B_4_0_0 - B_4_1_0 ) * localP -> P_1 ; } else { localB
-> B_4_7_0 = localB -> B_4_5_0 ; } localB -> B_4_8_0 = localX ->
integrator_CSTATE_f ; { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK_j . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - localP -> P_9 ; B_4_9_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , localDW ->
TransportDelay_IWORK_g . CircularBufSize , & localDW ->
TransportDelay_IWORK_g . Last , localDW -> TransportDelay_IWORK_g . Tail ,
localDW -> TransportDelay_IWORK_g . Head , localP -> P_10 , 0 , ( boolean_T )
( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW ->
TransportDelay_IWORK_g . CircularBufSize ) [ localDW ->
TransportDelay_IWORK_g . Head ] == ssGetT ( S ) ) ) ) ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_4_11_0 = localP
-> P_11 ; } rtb_B_4_4_0 = ( ssGetT ( S ) >= localB -> B_4_11_0 ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_4_13_0 =
localDW -> Memory_PreviousInput_c ; } if ( rtb_B_4_4_0 ) { localB -> B_4_15_0
= ( localB -> B_4_8_0 - B_4_9_0 ) * localP -> P_0 ; } else { localB ->
B_4_15_0 = localB -> B_4_13_0 ; } localB -> B_4_19_0 = ( muDoubleScalarSin (
localP -> P_15 * ssGetTaskTime ( S , 0 ) + localP -> P_16 ) * localP -> P_13
+ localP -> P_14 ) * rtu_In ; localB -> B_4_21_0 = ( muDoubleScalarSin (
localP -> P_19 * ssGetTaskTime ( S , 0 ) + localP -> P_20 ) * localP -> P_17
+ localP -> P_18 ) * rtu_In ; localB -> B_4_22_0 = localP -> P_21 *
muDoubleScalarHypot ( localB -> B_4_7_0 , localB -> B_4_15_0 ) ; if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW -> RMS_SubsysRanBC ) ;
} } } void AC_VSG_RMS_Update ( SimStruct * S , B_RMS_AC_VSG_T * localB ,
DW_RMS_AC_VSG_T * localDW , P_RMS_AC_VSG_T * localP ) { int32_T isHit ; if (
localDW -> RMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
localDW -> TransportDelay_IWORK . Head = ( ( localDW -> TransportDelay_IWORK
. Head < ( localDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? (
localDW -> TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK . Head == localDW -> TransportDelay_IWORK . Tail ) { if
( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize , & localDW -> TransportDelay_IWORK .
Tail , & localDW -> TransportDelay_IWORK . Head , & localDW ->
TransportDelay_IWORK . Last , simTime - localP -> P_4 , uBuffer , ( boolean_T
) 0 , false , & localDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize ) [ localDW ->
TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head ] = localB -> B_4_0_0 ; } isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { localDW -> Memory_PreviousInput = localB ->
B_4_7_0 ; } { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK_j . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
localDW -> TransportDelay_IWORK_g . Head = ( ( localDW ->
TransportDelay_IWORK_g . Head < ( localDW -> TransportDelay_IWORK_g .
CircularBufSize - 1 ) ) ? ( localDW -> TransportDelay_IWORK_g . Head + 1 ) :
0 ) ; if ( localDW -> TransportDelay_IWORK_g . Head == localDW ->
TransportDelay_IWORK_g . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW -> TransportDelay_IWORK_g
. CircularBufSize , & localDW -> TransportDelay_IWORK_g . Tail , & localDW ->
TransportDelay_IWORK_g . Head , & localDW -> TransportDelay_IWORK_g . Last ,
simTime - localP -> P_9 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_g . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * uBuffer + localDW ->
TransportDelay_IWORK_g . CircularBufSize ) [ localDW ->
TransportDelay_IWORK_g . Head ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK_g . Head ] = localB -> B_4_8_0 ; } isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW -> Memory_PreviousInput_c =
localB -> B_4_15_0 ; } } } void AC_VSG_RMS_Deriv ( SimStruct * S ,
B_RMS_AC_VSG_T * localB , DW_RMS_AC_VSG_T * localDW , XDot_RMS_AC_VSG_T *
localXdot ) { if ( localDW -> RMS_MODE ) { localXdot -> integrator_CSTATE_ld
= localB -> B_4_19_0 ; localXdot -> integrator_CSTATE_f = localB -> B_4_21_0
; } else { { real_T * dx ; int_T i ; dx = & ( localXdot ->
integrator_CSTATE_ld ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } }
} } void AC_VSG_RMS_Term ( SimStruct * const S ) { } void AC_VSG_TrueRMS_Init
( SimStruct * S , B_TrueRMS_AC_VSG_T * localB , DW_TrueRMS_AC_VSG_T * localDW
, P_TrueRMS_AC_VSG_T * localP , X_TrueRMS_AC_VSG_T * localX ) { localX ->
integrator_CSTATE_n = localP -> P_2 ; localDW -> Memory_PreviousInput =
localP -> P_6 ; localB -> B_6_10_0 = localP -> P_1 ; } void
AC_VSG_TrueRMS_Disable ( SimStruct * S , DW_TrueRMS_AC_VSG_T * localDW ) {
localDW -> TrueRMS_MODE = false ; } void AC_VSG_TrueRMS ( SimStruct * S ,
boolean_T rtu_Enable , real_T rtu_In , B_TrueRMS_AC_VSG_T * localB ,
DW_TrueRMS_AC_VSG_T * localDW , P_TrueRMS_AC_VSG_T * localP ,
X_TrueRMS_AC_VSG_T * localX ) { real_T B_6_2_0 ; real_T tmp ; int32_T isHit ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( ! localDW ->
TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW -> TrueRMS_MODE
= true ; } } else if ( localDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; AC_VSG_TrueRMS_Disable ( S
, localDW ) ; } } if ( localDW -> TrueRMS_MODE ) { localB -> B_6_1_0 = localX
-> integrator_CSTATE_n ; { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - localP -> P_3 ; B_6_2_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , localDW ->
TransportDelay_IWORK . CircularBufSize , & localDW -> TransportDelay_IWORK .
Last , localDW -> TransportDelay_IWORK . Tail , localDW ->
TransportDelay_IWORK . Head , localP -> P_4 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW -> TransportDelay_IWORK .
CircularBufSize ) [ localDW -> TransportDelay_IWORK . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB ->
B_6_3_0 = localP -> P_5 ; localB -> B_6_4_0 = localDW -> Memory_PreviousInput
; } if ( ssGetT ( S ) >= localB -> B_6_3_0 ) { localB -> B_6_7_0 = ( localB
-> B_6_1_0 - B_6_2_0 ) * localP -> P_0 ; } else { localB -> B_6_7_0 = localB
-> B_6_4_0 ; } localB -> B_6_8_0 = rtu_In * rtu_In ; if ( localB -> B_6_7_0 >
localP -> P_7 ) { tmp = localP -> P_7 ; } else if ( localB -> B_6_7_0 <
localP -> P_8 ) { tmp = localP -> P_8 ; } else { tmp = localB -> B_6_7_0 ; }
localB -> B_6_10_0 = muDoubleScalarSqrt ( tmp ) ; if ( ssIsModeUpdateTimeStep
( S ) ) { srUpdateBC ( localDW -> TrueRMS_SubsysRanBC ) ; } } } void
AC_VSG_TrueRMS_Update ( SimStruct * S , B_TrueRMS_AC_VSG_T * localB ,
DW_TrueRMS_AC_VSG_T * localDW , P_TrueRMS_AC_VSG_T * localP ) { int32_T isHit
; if ( localDW -> TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) &
localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head = ( ( localDW ->
TransportDelay_IWORK . Head < ( localDW -> TransportDelay_IWORK .
CircularBufSize - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head + 1 ) : 0
) ; if ( localDW -> TransportDelay_IWORK . Head == localDW ->
TransportDelay_IWORK . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW -> TransportDelay_IWORK .
CircularBufSize , & localDW -> TransportDelay_IWORK . Tail , & localDW ->
TransportDelay_IWORK . Head , & localDW -> TransportDelay_IWORK . Last ,
simTime - localP -> P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * uBuffer + localDW ->
TransportDelay_IWORK . CircularBufSize ) [ localDW -> TransportDelay_IWORK .
Head ] = simTime ; ( * uBuffer ) [ localDW -> TransportDelay_IWORK . Head ] =
localB -> B_6_1_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ localDW -> Memory_PreviousInput = localB -> B_6_7_0 ; } } } void
AC_VSG_TrueRMS_Deriv ( SimStruct * S , B_TrueRMS_AC_VSG_T * localB ,
DW_TrueRMS_AC_VSG_T * localDW , XDot_TrueRMS_AC_VSG_T * localXdot ) { if (
localDW -> TrueRMS_MODE ) { localXdot -> integrator_CSTATE_n = localB ->
B_6_8_0 ; } else { localXdot -> integrator_CSTATE_n = 0.0 ; } } void
AC_VSG_TrueRMS_Term ( SimStruct * const S ) { } static void mdlOutputs (
SimStruct * S , int_T tid ) { real_T B_24_1_0 ; real_T B_24_5_0 ; real_T
B_24_11_0 ; real_T B_24_15_0 ; real_T B_24_41_0 ; real_T B_24_139_0 ; real_T
B_24_143_0 ; real_T B_24_149_0 ; real_T B_24_153_0 ; real_T B_24_178_0 ;
real_T B_24_206_0 ; B_AC_VSG_T * _rtB ; DW_AC_VSG_T * _rtDW ; P_AC_VSG_T *
_rtP ; X_AC_VSG_T * _rtX ; real_T rtb_B_24_12_0 ; real_T rtb_B_24_16_0 ;
real_T rtb_B_24_20_0 ; real_T rtb_B_24_2_0 ; real_T rtb_B_24_6_0 ; int32_T
isHit ; boolean_T rtb_B_24_44_0 ; boolean_T rtb_B_24_84_0 ; _rtDW = ( (
DW_AC_VSG_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( ( X_AC_VSG_T * )
ssGetContStates ( S ) ) ; _rtP = ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_AC_VSG_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB -> B_24_0_0 =
_rtX -> integ1_CSTATE ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_3 ; B_24_1_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T_IWORK . CircularBufSize , & _rtDW -> T_IWORK . Last , _rtDW -> T_IWORK .
Tail , _rtDW -> T_IWORK . Head , _rtP -> P_4 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK . CircularBufSize
) [ _rtDW -> T_IWORK . Head ] == ssGetT ( S ) ) ) ) ; } rtb_B_24_2_0 = _rtB
-> B_24_0_0 - B_24_1_0 ; _rtB -> B_24_4_0 = _rtX -> Integ2_CSTATE ; { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_6 ;
B_24_5_0 = AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer ,
_rtDW -> T1_IWORK . CircularBufSize , & _rtDW -> T1_IWORK . Last , _rtDW ->
T1_IWORK . Tail , _rtDW -> T1_IWORK . Head , _rtP -> P_7 , 0 , ( boolean_T )
( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK .
CircularBufSize ) [ _rtDW -> T1_IWORK . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_24_6_0 = _rtB -> B_24_4_0 - B_24_5_0 ; _rtB -> B_24_10_0 = _rtX ->
integ1_CSTATE_f ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_k
. TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_9 ; B_24_11_0 = AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> T_IWORK_o . CircularBufSize , &
_rtDW -> T_IWORK_o . Last , _rtDW -> T_IWORK_o . Tail , _rtDW -> T_IWORK_o .
Head , _rtP -> P_10 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + _rtDW -> T_IWORK_o . CircularBufSize ) [ _rtDW -> T_IWORK_o . Head
] == ssGetT ( S ) ) ) ) ; } rtb_B_24_12_0 = _rtB -> B_24_10_0 - B_24_11_0 ;
_rtB -> B_24_14_0 = _rtX -> Integ2_CSTATE_k ; { real_T * * uBuffer = ( real_T
* * ) & _rtDW -> T1_PWORK_k . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT (
S ) ; real_T tMinusDelay = simTime - _rtP -> P_12 ; B_24_15_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_p . CircularBufSize , & _rtDW -> T1_IWORK_p . Last , _rtDW ->
T1_IWORK_p . Tail , _rtDW -> T1_IWORK_p . Head , _rtP -> P_13 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_p
. CircularBufSize ) [ _rtDW -> T1_IWORK_p . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_24_16_0 = _rtB -> B_24_14_0 - B_24_15_0 ; rtb_B_24_20_0 =
muDoubleScalarSqrt ( rtb_B_24_2_0 * rtb_B_24_2_0 + rtb_B_24_6_0 *
rtb_B_24_6_0 ) * muDoubleScalarSqrt ( rtb_B_24_12_0 * rtb_B_24_12_0 +
rtb_B_24_16_0 * rtb_B_24_16_0 ) ; rtb_B_24_2_0 = ( _rtP -> P_14 *
muDoubleScalarAtan2 ( rtb_B_24_6_0 , rtb_B_24_2_0 ) - _rtP -> P_15 *
muDoubleScalarAtan2 ( rtb_B_24_16_0 , rtb_B_24_12_0 ) ) * _rtP -> P_16 ; _rtB
-> B_24_31_0 [ 0 ] = rtb_B_24_20_0 * muDoubleScalarCos ( rtb_B_24_2_0 ) *
_rtP -> P_17 ; _rtB -> B_24_31_0 [ 1 ] = rtb_B_24_20_0 * muDoubleScalarSin (
rtb_B_24_2_0 ) * _rtP -> P_17 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 32 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 33 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 34 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_35_0
= _rtP -> P_28 * _rtB -> B_24_34_0 [ 8 ] ; } AC_VSG_TrueRMS ( S , _rtB ->
B_24_5_0 , _rtB -> B_24_35_0 , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , &
_rtP -> TrueRMS , & _rtX -> TrueRMS ) ; AC_VSG_RMS ( S , _rtB -> B_24_6_0 ,
_rtB -> B_24_35_0 , & _rtB -> RMS , & _rtDW -> RMS , & _rtP -> RMS , & _rtX
-> RMS ) ; if ( _rtB -> B_24_5_0 ) { rtb_B_24_20_0 = _rtB -> TrueRMS .
B_6_10_0 ; } else { rtb_B_24_20_0 = _rtB -> RMS . B_4_22_0 ; } _rtB ->
B_24_40_0 = _rtX -> integrator_CSTATE ; { real_T * * uBuffer = ( real_T * * )
& _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_30 ; B_24_41_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW -> TransportDelay_IWORK .
Head , _rtP -> P_31 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK . Head ] == ssGetT ( S ) ) ) ) ; } isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_43_0 = _rtP -> P_32 ; }
rtb_B_24_44_0 = ( ssGetT ( S ) >= _rtB -> B_24_43_0 ) ; isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_45_0 = _rtDW ->
Memory_PreviousInput ; } if ( rtb_B_24_44_0 ) { _rtB -> B_24_47_0 = ( _rtB ->
B_24_40_0 - B_24_41_0 ) * _rtP -> P_0 ; } else { _rtB -> B_24_47_0 = _rtB ->
B_24_45_0 ; } rtb_B_24_2_0 = _rtB -> B_24_47_0 * _rtB -> B_24_47_0 ;
AC_VSG_TrueRMS ( S , _rtB -> B_24_8_0 , _rtB -> B_24_35_0 , & _rtB ->
TrueRMS_o , & _rtDW -> TrueRMS_o , & _rtP -> TrueRMS_o , & _rtX -> TrueRMS_o
) ; AC_VSG_RMS ( S , _rtB -> B_24_9_0 , _rtB -> B_24_35_0 , & _rtB -> RMS_p ,
& _rtDW -> RMS_p , & _rtP -> RMS_p , & _rtX -> RMS_p ) ; if ( _rtB ->
B_24_8_0 ) { rtb_B_24_6_0 = _rtB -> TrueRMS_o . B_6_10_0 ; } else {
rtb_B_24_6_0 = _rtB -> RMS_p . B_4_22_0 ; } rtb_B_24_6_0 *= rtb_B_24_6_0 ; if
( rtb_B_24_6_0 > _rtP -> P_34 ) { rtb_B_24_12_0 = _rtP -> P_34 ; } else if (
rtb_B_24_6_0 < _rtP -> P_35 ) { rtb_B_24_12_0 = _rtP -> P_35 ; } else {
rtb_B_24_12_0 = rtb_B_24_6_0 ; } rtb_B_24_2_0 = ( ( rtb_B_24_20_0 *
rtb_B_24_20_0 - rtb_B_24_2_0 ) - rtb_B_24_6_0 ) / rtb_B_24_12_0 ; if (
rtb_B_24_2_0 > _rtP -> P_36 ) { rtb_B_24_2_0 = _rtP -> P_36 ; } else if (
rtb_B_24_2_0 < _rtP -> P_37 ) { rtb_B_24_2_0 = _rtP -> P_37 ; } _rtB ->
B_24_57_0 = muDoubleScalarSqrt ( rtb_B_24_2_0 ) ; isHit = ssIsSampleHit ( S ,
1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 58 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_59_0 = _rtP -> P_38 * _rtB -> B_24_34_0
[ 12 ] ; ssCallAccelRunBlock ( S , 24 , 60 , SS_CALL_MDL_OUTPUTS ) ; }
rtb_B_24_16_0 = _rtP -> P_40 * _rtX -> TransferFcn_CSTATE ; if (
rtb_B_24_16_0 > _rtP -> P_41 ) { rtb_B_24_16_0 = _rtP -> P_41 ; } else if (
rtb_B_24_16_0 < _rtP -> P_42 ) { rtb_B_24_16_0 = _rtP -> P_42 ; } _rtB ->
B_24_63_0 = rtb_B_24_16_0 - _rtB -> B_24_59_0 ; AC_VSG_PR ( S , _rtB ->
B_24_63_0 , & _rtB -> PR , & _rtP -> PR , & _rtX -> PR ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
24 , 65 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_66_0 = _rtP -> P_43 * _rtB ->
B_24_34_0 [ 13 ] ; ssCallAccelRunBlock ( S , 24 , 67 , SS_CALL_MDL_OUTPUTS )
; } _rtB -> B_24_70_0 = ( _rtB -> B_24_2_0 - _rtB -> B_24_31_0 [ 1 ] ) * _rtP
-> P_44 + _rtB -> B_24_10_0_c ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 71 , SS_CALL_MDL_OUTPUTS ) ;
_rtB -> B_24_72_0 = _rtP -> P_45 * _rtB -> B_24_34_0 [ 9 ] ; } rtb_B_24_16_0
= _rtB -> PR . B_0_4_0 / _rtB -> B_24_72_0 ; rtb_B_24_20_0 = look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_24_76_0 , _rtB -> B_24_12_0 ) ,
_rtP -> P_47 , _rtP -> P_46 , 2U ) ; rtb_B_24_44_0 = ( rtb_B_24_16_0 + _rtB
-> B_24_11_0 >= rtb_B_24_20_0 ) ; rtb_B_24_84_0 = ( _rtP -> P_48 *
rtb_B_24_16_0 + _rtB -> B_24_11_0 >= rtb_B_24_20_0 ) ; _rtB -> B_24_86_0 [ 0
] = rtb_B_24_44_0 ; _rtB -> B_24_86_0 [ 1 ] = ! rtb_B_24_44_0 ; _rtB ->
B_24_86_0 [ 2 ] = rtb_B_24_84_0 ; _rtB -> B_24_86_0 [ 3 ] = ! rtb_B_24_84_0 ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_98_0 =
_rtP -> P_49 * _rtB -> B_24_66_0 ; } _rtB -> B_24_100_0 = ( muDoubleScalarSin
( _rtP -> P_52 * ssGetTaskTime ( S , 0 ) + _rtP -> P_53 ) * _rtP -> P_50 +
_rtP -> P_51 ) * _rtB -> B_24_98_0 ; _rtB -> B_24_102_0 = ( muDoubleScalarSin
( _rtP -> P_56 * ssGetTaskTime ( S , 0 ) + _rtP -> P_57 ) * _rtP -> P_54 +
_rtP -> P_55 ) * _rtB -> B_24_98_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_24_103_0 = _rtP -> P_58 * _rtB -> B_24_35_0 ; }
_rtB -> B_24_105_0 = ( muDoubleScalarSin ( _rtP -> P_61 * ssGetTaskTime ( S ,
0 ) + _rtP -> P_62 ) * _rtP -> P_59 + _rtP -> P_60 ) * _rtB -> B_24_103_0 ;
_rtB -> B_24_107_0 = ( muDoubleScalarSin ( _rtP -> P_65 * ssGetTaskTime ( S ,
0 ) + _rtP -> P_66 ) * _rtP -> P_63 + _rtP -> P_64 ) * _rtB -> B_24_103_0 ;
_rtB -> B_24_111_0 = muDoubleScalarSin ( _rtX -> Integrator1_CSTATE ) * _rtB
-> B_24_70_0 - _rtB -> B_24_35_0 ; _rtB -> B_24_118_0 = _rtX ->
Integrator4_CSTATE ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ ssCallAccelRunBlock ( S , 24 , 119 , SS_CALL_MDL_OUTPUTS ) ; } if ( _rtB ->
B_24_118_0 > _rtP -> P_71 ) { rtb_B_24_16_0 = _rtP -> P_71 ; } else if ( _rtB
-> B_24_118_0 < _rtP -> P_72 ) { rtb_B_24_16_0 = _rtP -> P_72 ; } else {
rtb_B_24_16_0 = _rtB -> B_24_118_0 ; } _rtB -> B_24_121_0 = ( rtb_B_24_16_0 +
_rtB -> B_24_15_0 ) + _rtB -> B_24_3_0 ; isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 122 ,
SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_24_121_0 + 0 ) ; } } } _rtB -> B_24_129_0
= ( ( _rtB -> B_24_0_0_m / _rtB -> B_24_121_0 - _rtB -> B_24_31_0 [ 0 ] /
_rtB -> B_24_121_0 ) - rtb_B_24_16_0 * _rtB -> B_24_13_0 ) * _rtP -> P_73 ;
_rtB -> B_24_138_0 = _rtX -> integ1_CSTATE_l ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> T_PWORK_f . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_75 ; B_24_139_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T_IWORK_d . CircularBufSize , & _rtDW -> T_IWORK_d . Last , _rtDW ->
T_IWORK_d . Tail , _rtDW -> T_IWORK_d . Head , _rtP -> P_76 , 0 , ( boolean_T
) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK_d .
CircularBufSize ) [ _rtDW -> T_IWORK_d . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_24_20_0 = _rtB -> B_24_138_0 - B_24_139_0 ; _rtB -> B_24_142_0 = _rtX
-> Integ2_CSTATE_b ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_78 ; B_24_143_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_c . CircularBufSize , & _rtDW -> T1_IWORK_c . Last , _rtDW ->
T1_IWORK_c . Tail , _rtDW -> T1_IWORK_c . Head , _rtP -> P_79 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_c
. CircularBufSize ) [ _rtDW -> T1_IWORK_c . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_24_2_0 = _rtB -> B_24_142_0 - B_24_143_0 ; _rtB -> B_24_148_0 = _rtX ->
integ1_CSTATE_j ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_n
. TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_81 ; B_24_149_0 = AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> T_IWORK_k . CircularBufSize , &
_rtDW -> T_IWORK_k . Last , _rtDW -> T_IWORK_k . Tail , _rtDW -> T_IWORK_k .
Head , _rtP -> P_82 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + _rtDW -> T_IWORK_k . CircularBufSize ) [ _rtDW -> T_IWORK_k . Head
] == ssGetT ( S ) ) ) ) ; } rtb_B_24_6_0 = _rtB -> B_24_148_0 - B_24_149_0 ;
_rtB -> B_24_152_0 = _rtX -> Integ2_CSTATE_e ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> T1_PWORK_b . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_84 ; B_24_153_0 =
AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_e . CircularBufSize , & _rtDW -> T1_IWORK_e . Last , _rtDW ->
T1_IWORK_e . Tail , _rtDW -> T1_IWORK_e . Head , _rtP -> P_85 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_e
. CircularBufSize ) [ _rtDW -> T1_IWORK_e . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_24_12_0 = _rtB -> B_24_152_0 - B_24_153_0 ; rtb_B_24_16_0 =
muDoubleScalarSqrt ( rtb_B_24_20_0 * rtb_B_24_20_0 + rtb_B_24_2_0 *
rtb_B_24_2_0 ) * muDoubleScalarSqrt ( rtb_B_24_6_0 * rtb_B_24_6_0 +
rtb_B_24_12_0 * rtb_B_24_12_0 ) ; rtb_B_24_20_0 = ( _rtP -> P_86 *
muDoubleScalarAtan2 ( rtb_B_24_2_0 , rtb_B_24_20_0 ) - _rtP -> P_87 *
muDoubleScalarAtan2 ( rtb_B_24_12_0 , rtb_B_24_6_0 ) ) * _rtP -> P_88 ; _rtB
-> B_24_169_0 [ 0 ] = rtb_B_24_16_0 * muDoubleScalarCos ( rtb_B_24_20_0 ) *
_rtP -> P_89 ; _rtB -> B_24_169_0 [ 1 ] = rtb_B_24_16_0 * muDoubleScalarSin (
rtb_B_24_20_0 ) * _rtP -> P_89 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 170 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 171 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_172_0 = _rtP -> P_90 * _rtB -> B_24_34_0 [ 15 ] ; } AC_VSG_TrueRMS ( S ,
_rtB -> B_24_21_0 , _rtB -> B_24_172_0 , & _rtB -> TrueRMS_c , & _rtDW ->
TrueRMS_c , & _rtP -> TrueRMS_c , & _rtX -> TrueRMS_c ) ; AC_VSG_RMS ( S ,
_rtB -> B_24_22_0 , _rtB -> B_24_172_0 , & _rtB -> RMS_h , & _rtDW -> RMS_h ,
& _rtP -> RMS_h , & _rtX -> RMS_h ) ; if ( _rtB -> B_24_21_0 ) {
rtb_B_24_16_0 = _rtB -> TrueRMS_c . B_6_10_0 ; } else { rtb_B_24_16_0 = _rtB
-> RMS_h . B_4_22_0 ; } _rtB -> B_24_177_0 = _rtX -> integrator_CSTATE_l ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_f .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_92 ; B_24_178_0 = AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_e .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_e . Last , _rtDW ->
TransportDelay_IWORK_e . Tail , _rtDW -> TransportDelay_IWORK_e . Head , _rtP
-> P_93 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer +
_rtDW -> TransportDelay_IWORK_e . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_e . Head ] == ssGetT ( S ) ) ) ) ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_180_0 = _rtP
-> P_94 ; } rtb_B_24_44_0 = ( ssGetT ( S ) >= _rtB -> B_24_180_0 ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_182_0 = _rtDW
-> Memory_PreviousInput_k ; } if ( rtb_B_24_44_0 ) { _rtB -> B_24_184_0 = (
_rtB -> B_24_177_0 - B_24_178_0 ) * _rtP -> P_1 ; } else { _rtB -> B_24_184_0
= _rtB -> B_24_182_0 ; } rtb_B_24_20_0 = _rtB -> B_24_184_0 * _rtB ->
B_24_184_0 ; AC_VSG_TrueRMS ( S , _rtB -> B_24_24_0 , _rtB -> B_24_172_0 , &
_rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , & _rtP -> TrueRMS_b , & _rtX ->
TrueRMS_b ) ; AC_VSG_RMS ( S , _rtB -> B_24_25_0 , _rtB -> B_24_172_0 , &
_rtB -> RMS_m , & _rtDW -> RMS_m , & _rtP -> RMS_m , & _rtX -> RMS_m ) ; if (
_rtB -> B_24_24_0 ) { rtb_B_24_2_0 = _rtB -> TrueRMS_b . B_6_10_0 ; } else {
rtb_B_24_2_0 = _rtB -> RMS_m . B_4_22_0 ; } rtb_B_24_2_0 *= rtb_B_24_2_0 ; if
( rtb_B_24_2_0 > _rtP -> P_96 ) { rtb_B_24_12_0 = _rtP -> P_96 ; } else if (
rtb_B_24_2_0 < _rtP -> P_97 ) { rtb_B_24_12_0 = _rtP -> P_97 ; } else {
rtb_B_24_12_0 = rtb_B_24_2_0 ; } rtb_B_24_2_0 = ( ( rtb_B_24_16_0 *
rtb_B_24_16_0 - rtb_B_24_20_0 ) - rtb_B_24_2_0 ) / rtb_B_24_12_0 ; if (
rtb_B_24_2_0 > _rtP -> P_98 ) { rtb_B_24_2_0 = _rtP -> P_98 ; } else if (
rtb_B_24_2_0 < _rtP -> P_99 ) { rtb_B_24_2_0 = _rtP -> P_99 ; } _rtB ->
B_24_194_0 = muDoubleScalarSqrt ( rtb_B_24_2_0 ) ; isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 195 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_196_0 = _rtP -> P_100 * _rtB ->
B_24_34_0 [ 10 ] ; _rtB -> B_24_197_0 = _rtP -> P_101 * _rtB -> B_24_34_0 [
14 ] ; ssCallAccelRunBlock ( S , 24 , 198 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 199 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_24_202_0 = ( _rtB -> B_24_19_0 - _rtB -> B_24_169_0 [ 1 ] ) * _rtP -> P_102
+ _rtB -> B_24_26_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ ssCallAccelRunBlock ( S , 24 , 203 , SS_CALL_MDL_OUTPUTS ) ; } rtb_B_24_2_0
= _rtP -> P_104 * _rtX -> TransferFcn_CSTATE_a ; if ( rtb_B_24_2_0 > _rtP ->
P_105 ) { rtb_B_24_2_0 = _rtP -> P_105 ; } else if ( rtb_B_24_2_0 < _rtP ->
P_106 ) { rtb_B_24_2_0 = _rtP -> P_106 ; } B_24_206_0 = rtb_B_24_2_0 - _rtB
-> B_24_197_0 ; AC_VSG_PR ( S , B_24_206_0 , & _rtB -> PR_l , & _rtP -> PR_l
, & _rtX -> PR_l ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ _rtB -> B_24_208_0 = _rtP -> P_107 * _rtB -> B_24_34_0 [ 11 ] ; }
rtb_B_24_2_0 = _rtB -> PR_l . B_0_4_0 / _rtB -> B_24_208_0 ; rtb_B_24_16_0 =
look1_binlxpw ( muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_24_212_0 , _rtB
-> B_24_28_0 ) , _rtP -> P_109 , _rtP -> P_108 , 2U ) ; rtb_B_24_44_0 = (
rtb_B_24_2_0 + _rtB -> B_24_27_0 >= rtb_B_24_16_0 ) ; rtb_B_24_84_0 = ( _rtP
-> P_110 * rtb_B_24_2_0 + _rtB -> B_24_27_0 >= rtb_B_24_16_0 ) ; _rtB ->
B_24_222_0 [ 0 ] = rtb_B_24_44_0 ; _rtB -> B_24_222_0 [ 1 ] = ! rtb_B_24_44_0
; _rtB -> B_24_222_0 [ 2 ] = rtb_B_24_84_0 ; _rtB -> B_24_222_0 [ 3 ] = !
rtb_B_24_84_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_24_234_0 = _rtP -> P_111 * _rtB -> B_24_172_0 ; } _rtB ->
B_24_236_0 = ( muDoubleScalarSin ( _rtP -> P_114 * ssGetTaskTime ( S , 0 ) +
_rtP -> P_115 ) * _rtP -> P_112 + _rtP -> P_113 ) * _rtB -> B_24_234_0 ; _rtB
-> B_24_238_0 = ( muDoubleScalarSin ( _rtP -> P_118 * ssGetTaskTime ( S , 0 )
+ _rtP -> P_119 ) * _rtP -> P_116 + _rtP -> P_117 ) * _rtB -> B_24_234_0 ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_239_0
= _rtP -> P_120 * _rtB -> B_24_196_0 ; } _rtB -> B_24_241_0 = (
muDoubleScalarSin ( _rtP -> P_123 * ssGetTaskTime ( S , 0 ) + _rtP -> P_124 )
* _rtP -> P_121 + _rtP -> P_122 ) * _rtB -> B_24_239_0 ; _rtB -> B_24_243_0 =
( muDoubleScalarSin ( _rtP -> P_127 * ssGetTaskTime ( S , 0 ) + _rtP -> P_128
) * _rtP -> P_125 + _rtP -> P_126 ) * _rtB -> B_24_239_0 ; _rtB -> B_24_247_0
= muDoubleScalarSin ( _rtX -> Integrator1_CSTATE_l ) * _rtB -> B_24_202_0 -
_rtB -> B_24_196_0 ; _rtB -> B_24_254_0 = _rtX -> Integrator4_CSTATE_d ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock
( S , 24 , 255 , SS_CALL_MDL_OUTPUTS ) ; } if ( _rtB -> B_24_254_0 > _rtP ->
P_133 ) { rtb_B_24_2_0 = _rtP -> P_133 ; } else if ( _rtB -> B_24_254_0 <
_rtP -> P_134 ) { rtb_B_24_2_0 = _rtP -> P_134 ; } else { rtb_B_24_2_0 = _rtB
-> B_24_254_0 ; } _rtB -> B_24_257_0 = ( rtb_B_24_2_0 + _rtB -> B_24_31_0_k )
+ _rtB -> B_24_18_0 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ ssCallAccelRunBlock ( S , 24 , 258 , SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW
-> TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_24_257_0 + 0 ) ; } } } _rtB
-> B_24_265_0 = ( ( _rtB -> B_24_16_0 / _rtB -> B_24_257_0 - _rtB ->
B_24_169_0 [ 0 ] / _rtB -> B_24_257_0 ) - rtb_B_24_2_0 * _rtB -> B_24_29_0 )
* _rtP -> P_135 ; UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID2 (
SimStruct * S , int_T tid ) { B_AC_VSG_T * _rtB ; P_AC_VSG_T * _rtP ; _rtP =
( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( B_AC_VSG_T * )
_ssGetModelBlockIO ( S ) ) ; _rtB -> B_24_0_0_m = _rtP -> P_136 ; _rtB ->
B_24_2_0 = _rtP -> P_138 ; _rtB -> B_24_3_0 = _rtP -> P_139 ; _rtB ->
B_24_5_0 = ( _rtP -> P_140 != 0.0 ) ; _rtB -> B_24_6_0 = ! _rtB -> B_24_5_0 ;
_rtB -> B_24_8_0 = ( _rtP -> P_141 != 0.0 ) ; _rtB -> B_24_9_0 = ! _rtB ->
B_24_8_0 ; _rtB -> B_24_10_0_c = _rtP -> P_142 ; _rtB -> B_24_11_0 = _rtP ->
P_143 ; _rtB -> B_24_12_0 = _rtP -> P_144 ; _rtB -> B_24_13_0 = _rtP -> P_145
; _rtB -> B_24_15_0 = _rtP -> P_147 * _rtP -> P_146 ; _rtB -> B_24_16_0 =
_rtP -> P_148 ; _rtB -> B_24_1_0 [ 0 ] = _rtP -> P_137 [ 0 ] ; _rtB ->
B_24_17_0 [ 0 ] = _rtP -> P_149 [ 0 ] ; _rtB -> B_24_1_0 [ 1 ] = _rtP ->
P_137 [ 1 ] ; _rtB -> B_24_17_0 [ 1 ] = _rtP -> P_149 [ 1 ] ; _rtB ->
B_24_1_0 [ 2 ] = _rtP -> P_137 [ 2 ] ; _rtB -> B_24_17_0 [ 2 ] = _rtP ->
P_149 [ 2 ] ; _rtB -> B_24_1_0 [ 3 ] = _rtP -> P_137 [ 3 ] ; _rtB ->
B_24_17_0 [ 3 ] = _rtP -> P_149 [ 3 ] ; _rtB -> B_24_18_0 = _rtP -> P_150 ;
_rtB -> B_24_19_0 = _rtP -> P_151 ; _rtB -> B_24_21_0 = ( _rtP -> P_152 !=
0.0 ) ; _rtB -> B_24_22_0 = ! _rtB -> B_24_21_0 ; _rtB -> B_24_24_0 = ( _rtP
-> P_153 != 0.0 ) ; _rtB -> B_24_25_0 = ! _rtB -> B_24_24_0 ; _rtB ->
B_24_26_0 = _rtP -> P_154 ; _rtB -> B_24_27_0 = _rtP -> P_155 ; _rtB ->
B_24_28_0 = _rtP -> P_156 ; _rtB -> B_24_29_0 = _rtP -> P_157 ; _rtB ->
B_24_31_0_k = _rtP -> P_159 * _rtP -> P_158 ; _rtB -> B_24_32_0 = _rtP ->
P_160 ; _rtB -> B_24_33_0 = _rtP -> P_161 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_AC_VSG_T * _rtB ;
DW_AC_VSG_T * _rtDW ; P_AC_VSG_T * _rtP ; int32_T isHit ; _rtDW = ( (
DW_AC_VSG_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( P_AC_VSG_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_AC_VSG_T * ) _ssGetModelBlockIO ( S ) )
; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T_IWORK . Head = ( ( _rtDW ->
T_IWORK . Head < ( _rtDW -> T_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
T_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> T_IWORK . Head == _rtDW -> T_IWORK
. Tail ) { if ( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
T_IWORK . CircularBufSize , & _rtDW -> T_IWORK . Tail , & _rtDW -> T_IWORK .
Head , & _rtDW -> T_IWORK . Last , simTime - _rtP -> P_3 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> T_IWORK . CircularBufSize ) [ _rtDW -> T_IWORK . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK . Head ] = _rtB -> B_24_0_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK . TUbufferPtrs [ 0 ]
; real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK . Head = ( ( _rtDW ->
T1_IWORK . Head < ( _rtDW -> T1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
T1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> T1_IWORK . Head == _rtDW ->
T1_IWORK . Tail ) { if ( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> T1_IWORK . CircularBufSize , & _rtDW -> T1_IWORK . Tail , & _rtDW ->
T1_IWORK . Head , & _rtDW -> T1_IWORK . Last , simTime - _rtP -> P_6 ,
uBuffer , ( boolean_T ) 0 , false , & _rtDW -> T1_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> T1_IWORK . CircularBufSize ) [ _rtDW -> T1_IWORK . Head ]
= simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK . Head ] = _rtB -> B_24_4_0 ; }
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_k . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T_IWORK_o . Head = ( ( _rtDW ->
T_IWORK_o . Head < ( _rtDW -> T_IWORK_o . CircularBufSize - 1 ) ) ? ( _rtDW
-> T_IWORK_o . Head + 1 ) : 0 ) ; if ( _rtDW -> T_IWORK_o . Head == _rtDW ->
T_IWORK_o . Tail ) { if ( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> T_IWORK_o . CircularBufSize , & _rtDW -> T_IWORK_o . Tail , & _rtDW ->
T_IWORK_o . Head , & _rtDW -> T_IWORK_o . Last , simTime - _rtP -> P_9 ,
uBuffer , ( boolean_T ) 0 , false , & _rtDW -> T_IWORK_o . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> T_IWORK_o . CircularBufSize ) [ _rtDW -> T_IWORK_o .
Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK_o . Head ] = _rtB ->
B_24_10_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK_k .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK_p .
Head = ( ( _rtDW -> T1_IWORK_p . Head < ( _rtDW -> T1_IWORK_p .
CircularBufSize - 1 ) ) ? ( _rtDW -> T1_IWORK_p . Head + 1 ) : 0 ) ; if (
_rtDW -> T1_IWORK_p . Head == _rtDW -> T1_IWORK_p . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T1_IWORK_p .
CircularBufSize , & _rtDW -> T1_IWORK_p . Tail , & _rtDW -> T1_IWORK_p . Head
, & _rtDW -> T1_IWORK_p . Last , simTime - _rtP -> P_12 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T1_IWORK_p . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> T1_IWORK_p . CircularBufSize ) [ _rtDW -> T1_IWORK_p .
Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_p . Head ] = _rtB ->
B_24_14_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 24 , 34 , SS_CALL_MDL_UPDATE ) ; }
AC_VSG_TrueRMS_Update ( S , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , & _rtP
-> TrueRMS ) ; AC_VSG_RMS_Update ( S , & _rtB -> RMS , & _rtDW -> RMS , &
_rtP -> RMS ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK .
Head < ( _rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay_IWORK .
CircularBufSize , & _rtDW -> TransportDelay_IWORK . Tail , & _rtDW ->
TransportDelay_IWORK . Head , & _rtDW -> TransportDelay_IWORK . Last ,
simTime - _rtP -> P_30 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
TransportDelay_IWORK . CircularBufSize ) [ _rtDW -> TransportDelay_IWORK .
Head ] = simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] =
_rtB -> B_24_40_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ _rtDW -> Memory_PreviousInput = _rtB -> B_24_47_0 ; } AC_VSG_TrueRMS_Update
( S , & _rtB -> TrueRMS_o , & _rtDW -> TrueRMS_o , & _rtP -> TrueRMS_o ) ;
AC_VSG_RMS_Update ( S , & _rtB -> RMS_p , & _rtDW -> RMS_p , & _rtP -> RMS_p
) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_f . TUbufferPtrs
[ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T_IWORK_d . Head = ( ( _rtDW
-> T_IWORK_d . Head < ( _rtDW -> T_IWORK_d . CircularBufSize - 1 ) ) ? (
_rtDW -> T_IWORK_d . Head + 1 ) : 0 ) ; if ( _rtDW -> T_IWORK_d . Head ==
_rtDW -> T_IWORK_d . Tail ) { if ( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf
( & _rtDW -> T_IWORK_d . CircularBufSize , & _rtDW -> T_IWORK_d . Tail , &
_rtDW -> T_IWORK_d . Head , & _rtDW -> T_IWORK_d . Last , simTime - _rtP ->
P_75 , uBuffer , ( boolean_T ) 0 , false , & _rtDW -> T_IWORK_d .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * uBuffer + _rtDW -> T_IWORK_d . CircularBufSize ) [ _rtDW
-> T_IWORK_d . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK_d . Head ]
= _rtB -> B_24_138_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
T1_IWORK_c . Head = ( ( _rtDW -> T1_IWORK_c . Head < ( _rtDW -> T1_IWORK_c .
CircularBufSize - 1 ) ) ? ( _rtDW -> T1_IWORK_c . Head + 1 ) : 0 ) ; if (
_rtDW -> T1_IWORK_c . Head == _rtDW -> T1_IWORK_c . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T1_IWORK_c .
CircularBufSize , & _rtDW -> T1_IWORK_c . Tail , & _rtDW -> T1_IWORK_c . Head
, & _rtDW -> T1_IWORK_c . Last , simTime - _rtP -> P_78 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T1_IWORK_c . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> T1_IWORK_c . CircularBufSize ) [ _rtDW -> T1_IWORK_c .
Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_c . Head ] = _rtB ->
B_24_142_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_n .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T_IWORK_k .
Head = ( ( _rtDW -> T_IWORK_k . Head < ( _rtDW -> T_IWORK_k . CircularBufSize
- 1 ) ) ? ( _rtDW -> T_IWORK_k . Head + 1 ) : 0 ) ; if ( _rtDW -> T_IWORK_k .
Head == _rtDW -> T_IWORK_k . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T_IWORK_k .
CircularBufSize , & _rtDW -> T_IWORK_k . Tail , & _rtDW -> T_IWORK_k . Head ,
& _rtDW -> T_IWORK_k . Last , simTime - _rtP -> P_81 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> T_IWORK_k . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, "tdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
T_IWORK_k . CircularBufSize ) [ _rtDW -> T_IWORK_k . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> T_IWORK_k . Head ] = _rtB -> B_24_148_0 ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK_b . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK_e . Head = ( ( _rtDW -> T1_IWORK_e
. Head < ( _rtDW -> T1_IWORK_e . CircularBufSize - 1 ) ) ? ( _rtDW ->
T1_IWORK_e . Head + 1 ) : 0 ) ; if ( _rtDW -> T1_IWORK_e . Head == _rtDW ->
T1_IWORK_e . Tail ) { if ( ! AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> T1_IWORK_e . CircularBufSize , & _rtDW -> T1_IWORK_e . Tail , &
_rtDW -> T1_IWORK_e . Head , & _rtDW -> T1_IWORK_e . Last , simTime - _rtP ->
P_84 , uBuffer , ( boolean_T ) 0 , false , & _rtDW -> T1_IWORK_e .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * uBuffer + _rtDW -> T1_IWORK_e . CircularBufSize ) [ _rtDW
-> T1_IWORK_e . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_e . Head
] = _rtB -> B_24_152_0 ; } AC_VSG_TrueRMS_Update ( S , & _rtB -> TrueRMS_c ,
& _rtDW -> TrueRMS_c , & _rtP -> TrueRMS_c ) ; AC_VSG_RMS_Update ( S , & _rtB
-> RMS_h , & _rtDW -> RMS_h , & _rtP -> RMS_h ) ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_e . Head = ( ( _rtDW
-> TransportDelay_IWORK_e . Head < ( _rtDW -> TransportDelay_IWORK_e .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_e . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_e . Head == _rtDW ->
TransportDelay_IWORK_e . Tail ) { if ( !
AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay_IWORK_e .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_e . Tail , & _rtDW ->
TransportDelay_IWORK_e . Head , & _rtDW -> TransportDelay_IWORK_e . Last ,
simTime - _rtP -> P_92 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_e . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
TransportDelay_IWORK_e . CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_e
. Head ] = simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK_e . Head ]
= _rtB -> B_24_177_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit !=
0 ) { _rtDW -> Memory_PreviousInput_k = _rtB -> B_24_184_0 ; }
AC_VSG_TrueRMS_Update ( S , & _rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , &
_rtP -> TrueRMS_b ) ; AC_VSG_RMS_Update ( S , & _rtB -> RMS_m , & _rtDW ->
RMS_m , & _rtP -> RMS_m ) ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_AC_VSG_T * _rtB ;
DW_AC_VSG_T * _rtDW ; P_AC_VSG_T * _rtP ; XDot_AC_VSG_T * _rtXdot ;
X_AC_VSG_T * _rtX ; _rtDW = ( ( DW_AC_VSG_T * ) ssGetRootDWork ( S ) ) ;
_rtXdot = ( ( XDot_AC_VSG_T * ) ssGetdX ( S ) ) ; _rtX = ( ( X_AC_VSG_T * )
ssGetContStates ( S ) ) ; _rtP = ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_AC_VSG_T * ) _ssGetModelBlockIO ( S ) ) ; _rtXdot ->
integ1_CSTATE = _rtB -> B_24_105_0 ; _rtXdot -> Integ2_CSTATE = _rtB ->
B_24_107_0 ; _rtXdot -> integ1_CSTATE_f = _rtB -> B_24_100_0 ; _rtXdot ->
Integ2_CSTATE_k = _rtB -> B_24_102_0 ; AC_VSG_TrueRMS_Deriv ( S , & _rtB ->
TrueRMS , & _rtDW -> TrueRMS , & _rtXdot -> TrueRMS ) ; AC_VSG_RMS_Deriv ( S
, & _rtB -> RMS , & _rtDW -> RMS , & _rtXdot -> RMS ) ; _rtXdot ->
integrator_CSTATE = _rtB -> B_24_35_0 ; AC_VSG_TrueRMS_Deriv ( S , & _rtB ->
TrueRMS_o , & _rtDW -> TrueRMS_o , & _rtXdot -> TrueRMS_o ) ;
AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_p , & _rtDW -> RMS_p , & _rtXdot ->
RMS_p ) ; _rtXdot -> TransferFcn_CSTATE = 0.0 ; _rtXdot -> TransferFcn_CSTATE
+= _rtP -> P_39 * _rtX -> TransferFcn_CSTATE ; _rtXdot -> TransferFcn_CSTATE
+= _rtB -> B_24_111_0 ; AC_VSG_PR_Deriv ( S , & _rtB -> PR , & _rtP -> PR , &
_rtX -> PR , & _rtXdot -> PR ) ; _rtXdot -> Integrator1_CSTATE = _rtB ->
B_24_121_0 ; _rtXdot -> Integrator4_CSTATE = _rtB -> B_24_129_0 ; _rtXdot ->
integ1_CSTATE_l = _rtB -> B_24_241_0 ; _rtXdot -> Integ2_CSTATE_b = _rtB ->
B_24_243_0 ; _rtXdot -> integ1_CSTATE_j = _rtB -> B_24_236_0 ; _rtXdot ->
Integ2_CSTATE_e = _rtB -> B_24_238_0 ; AC_VSG_TrueRMS_Deriv ( S , & _rtB ->
TrueRMS_c , & _rtDW -> TrueRMS_c , & _rtXdot -> TrueRMS_c ) ;
AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_h , & _rtDW -> RMS_h , & _rtXdot ->
RMS_h ) ; _rtXdot -> integrator_CSTATE_l = _rtB -> B_24_172_0 ;
AC_VSG_TrueRMS_Deriv ( S , & _rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , &
_rtXdot -> TrueRMS_b ) ; AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_m , & _rtDW ->
RMS_m , & _rtXdot -> RMS_m ) ; _rtXdot -> TransferFcn_CSTATE_a = 0.0 ;
_rtXdot -> TransferFcn_CSTATE_a += _rtP -> P_103 * _rtX ->
TransferFcn_CSTATE_a ; _rtXdot -> TransferFcn_CSTATE_a += _rtB -> B_24_247_0
; AC_VSG_PR_Deriv ( S , & _rtB -> PR_l , & _rtP -> PR_l , & _rtX -> PR_l , &
_rtXdot -> PR_l ) ; _rtXdot -> Integrator1_CSTATE_l = _rtB -> B_24_257_0 ;
_rtXdot -> Integrator4_CSTATE_d = _rtB -> B_24_265_0 ; } static void
mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 , 1860310449U
) ; ssSetChecksumVal ( S , 1 , 4194696622U ) ; ssSetChecksumVal ( S , 2 ,
2887078336U ) ; ssSetChecksumVal ( S , 3 , 4288629547U ) ; { mxArray *
slVerStructMat = ( NULL ) ; mxArray * slStrMat = mxCreateString ( "simulink"
) ; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat
, 1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == ( NULL ) ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "10.6" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != ( SLSize ) sizeof ( DW_AC_VSG_T ) ) { static char msg [ 256 ] ;
sprintf ( msg , "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof ( DW_AC_VSG_T ) ) ;
ssSetErrorStatus ( S , msg ) ; } if ( ssGetSizeofGlobalBlockIO ( S ) != (
SLSize ) sizeof ( B_AC_VSG_T ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof ( B_AC_VSG_T ) ) ;
ssSetErrorStatus ( S , msg ) ; } { int ssSizeofParams ; ssGetSizeofParams ( S
, & ssSizeofParams ) ; if ( ssSizeofParams != sizeof ( P_AC_VSG_T ) ) {
static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_AC_VSG_T ) ) ; ssSetErrorStatus ( S , msg ) ; } }
_ssSetModelRtp ( S , ( real_T * ) & AC_VSG_rtDefaultP ) ; rt_InitInfAndNaN (
sizeof ( real_T ) ) ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) -> P_34 =
rtInf ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) -> P_36 = rtInf ; ( (
P_AC_VSG_T * ) ssGetModelRtp ( S ) ) -> P_96 = rtInf ; ( ( P_AC_VSG_T * )
ssGetModelRtp ( S ) ) -> P_98 = rtInf ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S
) ) -> TrueRMS_b . P_7 = rtInf ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) ->
TrueRMS_c . P_7 = rtInf ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) ->
TrueRMS_o . P_7 = rtInf ; ( ( P_AC_VSG_T * ) ssGetModelRtp ( S ) ) -> TrueRMS
. P_7 = rtInf ; } static void mdlInitializeSampleTimes ( SimStruct * S ) {
slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
