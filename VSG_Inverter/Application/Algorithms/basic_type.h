#ifndef __BASIC_TYPE__
#define __BASIC_TYPE__

#include <stdbool.h>
#include <stdint.h>

#define NULL 0 

#define FALSE false
#define TRUE true

typedef float fp32;
typedef double fp64;

#define mat         arm_matrix_instance_f32 
#define mat_init    arm_mat_init_f32
#define mat_add     arm_mat_add_f32
#define mat_sub     arm_mat_sub_f32
#define mat_mult    arm_mat_mult_f32
#define mat_trans   arm_mat_trans_f32
#define mat_inv     arm_mat_inverse_f32

#endif // __BASIC_TYPE__
