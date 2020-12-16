#include "oslib.h"
#include "vfpu_ops.h"

#ifdef PSP
float vfpu_vars[4] __attribute__((aligned(64)));

	//DONT USE THEM

	//r = f1 + f2
	extern float vfpu_add(float f1, float f2);
	//r = f1 - f2
	extern float vfpu_sub(float f1, float f2);
	//r = f1 * f2
	extern float vfpu_mul(float f1, float f2);
	//r = f1 / f2
	extern float vfpu_div(float f1, float f2);
	//r = (int)v
	extern int vfpu_f2i(float v);
	//r = (float)v
	extern float vfpu_i2f(int v);
	//r = sin(f1°)*f2
	extern float vfpu_sini(int f1, int f2);
	//r = cos(f1°)*f2
	extern float vfpu_cosi(int f1, int f2);
	//r = (int)(f1 - f2)
	extern int vfpu_isubf(float f1, float f2);


	//Int to float
	#define vi2f_s(vfpu_rd,vfpu_rs,scale) (0xd2800000 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
	//Float to int
	#define vf2in_s(vfpu_rd,vfpu_rs,scale) (0xd2000000 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))

	//END DONT USE THEM

/*float vfpu_add(float f1, float f2)
{
   vfpu_vars[0] = f1;
   vfpu_vars[1] = f2;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(vadd_s(124, 0, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float vfpu_sub(float f1, float f2)
{
   vfpu_vars[0] = f1;
   vfpu_vars[1] = f2;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(vsub_s(124, 0, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

int vfpu_isubf(float f1, float f2)
{
   vfpu_vars[0] = f1;
   vfpu_vars[1] = f2;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(vsub_s(2, 0, 1))
      cgen_asm(vf2in_s(124, 2, 0))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return *(int*)ptr;
}

int vfpu_f2i(float v)
{
   vfpu_vars[0] = v;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(vf2in_s(124, 0, 0))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return *(int*)ptr;
}

//Int to float
#define vi2f_s(vfpu_rd,vfpu_rs,scale) (0xd2800000 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))

float vfpu_i2f(int v)
{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(int*)ptr = v;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(vi2f_s(124, 0, 0))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float vfpu_mul(float f1, float f2)
{
   vfpu_vars[0] = f1;
   vfpu_vars[1] = f2;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(vmul_s(124, 0, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float vfpu_div(float f1, float f2)
{
   vfpu_vars[0] = f1;
   vfpu_vars[1] = f2;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(vdiv_s(124, 0, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float vfpu_sini(int f1, int f2)
{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(int*)ptr = f1;
   *((int*)ptr+1) = f2;
   vfpu_vars[2] = 90;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(lv_s(2, 2, R_a0, 0))
      cgen_asm(vi2f_s(0, 0, 0))
      cgen_asm(vi2f_s(1, 1, 0))
      cgen_asm(vdiv_s(0, 0, 2))					//f1/90
      cgen_asm(vsin_s(124, 0))					//sin(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float vfpu_cosi(int f1, int f2)
{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(int*)ptr = f1;
   *((int*)ptr+1) = f2;
   vfpu_vars[2] = 90;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(lv_s(2, 2, R_a0, 0))
      cgen_asm(vi2f_s(0, 0, 0))
      cgen_asm(vi2f_s(1, 1, 0))
      cgen_asm(vdiv_s(0, 0, 2))					//f1/90
      cgen_asm(vcos_s(124, 0))					//cos(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}*/

float oslVfpu_sinf(float f1, float f2)
{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = f1;
   *((float*)ptr+1) = f2;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))

	  //Trigonometric functions on the vfpu expect input values like vsin(degrees/90) or vsin(2/PI * radians)
	  cgen_asm(vcst_s(2, 5))
      cgen_asm(vmul_s(0, 0, 2))					//angle*2/PI

      cgen_asm(vsin_s(124, 0))					//sin(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
//   return sin(f1) * f2;
   return vfpu_vars[0];
}

float oslVfpu_cosf(float f1, float f2)
{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = f1;
   *((float*)ptr+1) = f2;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))

	  //Trigonometric functions on the vfpu expect input values like vsin(degrees/90) or vsin(2/PI * radians)
	  cgen_asm(vcst_s(2, 5))
      cgen_asm(vmul_s(0, 0, 2))					//angle*2/PI

      cgen_asm(vcos_s(124, 0))					//cos(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
//   return cos(f1) * f2;
   return vfpu_vars[0];
}

/*int vfpu_icos(int f1, int f2)
{
   *(int*)ptr = f1;
   *((int*)ptr+1) = f2;
   vfpu_vars[2] = 90;
   register void *ptr __asm ("a0") = vfpu_vars;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(lv_s(2, 2, R_a0, 0))
      cgen_asm(vdiv_s(0, 0, 2))					//f1/90
      cgen_asm(vcos_s(124, 0))					//cos(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return *(int*)ptr;
}*/

float oslCos(float angle, float dist)		{
   register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = angle;
   *((float*)ptr+1) = dist;
   *((float*)ptr+2) = 90.0f;

   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(lv_s(2, 2, R_a0, 0))

	  //Trigonometric functions on the vfpu expect input values like vsin(degrees/90) or vsin(2/PI * radians)
      cgen_asm(vdiv_s(0, 0, 2))					//f1/90

      cgen_asm(vcos_s(124, 0))					//cos(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2

      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

float oslSin(float angle, float dist)		{
	register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = angle;
   *((float*)ptr+1) = dist;
   *((float*)ptr+2) = 90.0f;

   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))
      cgen_asm(lv_s(2, 2, R_a0, 0))

	  //Trigonometric functions on the vfpu expect input values like vsin(degrees/90) or vsin(2/PI * radians)
      cgen_asm(vdiv_s(0, 0, 2))					//f1/90

      cgen_asm(vsin_s(124, 0))					//sin(f1)
      cgen_asm(vmul_s(124, 124, 1))				//f1*f2

      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}

#else

//For PC, we use different methods (much slower, of course)
#include <math.h>

float vfpu_sini(int f1, int f2)
{
	return (float)((sin((float)f1*3.14159265f/180.0f)*f2));
}

float vfpu_cosi(int f1, int f2)
{
	return (float)((cos((float)f1*3.14159265f/180.0f)*f2));
}

float vfpu_cosf(float f1, float f2)		{
	return cos(f1) * f2;
}

float vfpu_sinf(float f1, float f2)		{
	return sin(f1) * f2;
}

float oslCos(float angle, float dist)		{
	//Degree => Radian
	angle = angle * 0.0174532925f;
	return vfpu_cosf(angle, dist);
}

float oslSin(float angle, float dist)		{
	//Degree => Radian
	angle = angle * 0.0174532925f;
	return vfpu_sinf(angle, dist);
}

#endif
