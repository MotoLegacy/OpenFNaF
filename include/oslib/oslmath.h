#ifndef __OSL_MATH_H__
#define __OSL_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup math Math

    Math functions (using VFPU)
	Functions using the Vector Floating Point Unit (VFPU). All this functions are based on libpspmath version 4 by MrMr[iCE]
	@{
*/
#include "libpspmath/pspmath.h"

/**
  * Set vfpu random generator seed
  *
  * @param x - seed value
  *
*/
#define oslSrand vfpu_srand

/**
  * Return random float value
  *
  * @param min - minimum value to return
  * @param max - maximum value to return
  *
*/
#define oslRandf vfpu_randf


/**
  * Return random color value in 8888 format
  * This always sets the alpha channel value to 0xFF
  *
  * @param min - minimum value for each color channel (0..255)
  * @param max - maximum value for each color channel (0..255)
  *
*/
#define oslRand_8888 vfpu_rand_8888


/**
  * Calculate sine
  *
  * @param x - input in radians
  *
*/
#define oslSinf vfpu_sinf


/**
  * Calculate cosine
  *
  * @param x - input in radians
  *
*/
#define oslCosf vfpu_cosf


/**
  * Calculate tangent
  *
  * @param x - input in radians
  *
*/
#define oslTanf vfpu_tanf

/**
  * Calculate inverse sine (arcsin)
  *
  * @param x - input
  *
*/
#define oslAsinf vfpu_asinf

/**
  * Calculate inverse cosine (arccos)
  *
  * @param x - input
  *
*/
#define oslAcosf vfpu_acosf

/**
  * Calculate inverse tangent (arctan)
  *
  * @param x - input
  *
*/
#define oslAtanf vfpu_atanf

/**
  * Calculate inverse tangent, with proper quadrant fixup
  *
  * @param x - input
  *
*/
#define oslAtan2f vfpu_atan2f

/**
  * Calculate hyperbolic sine
  *
  * @param x - input
  *
*/
#define oslSinhf vfpu_sinhf

/**
  * Calculate hyperbolic cosine
  *
  * @param x - input
  *
*/
#define oslCoshf vfpu_coshf

/**
  * Calculate hyperbolic tangent
  *
  * @param x - input
  *
*/
#define oslTanhf vfpu_tanhf

/**
  * Calculate sine and cosine
  *
  * @param r - input in radians
  * @param s - pointer to float for sin
  * @param c - pointer to float for cos
*/
#define oslSincos vfpu_sincos

/**
  * Calculate exponent of x
  *
  * @param x - input
  *
*/
#define oslExpf vfpu_expf

/**
  * Calculate logarithm of x
  *
  * @param x - input
  *
*/
#define oslLogf vfpu_logf

/**
  * Calculate x raised to the power of y
  *
  * @param x - number to raise power of
  * @param y - power to raise x by
  *
*/
#define oslPowf vfpu_powf

/**
  * Calculate floating point remainder of x/y
  *
  * @param x - input
  * @param y - input
  *
*/
#define oslFmodf vfpu_fmodf

/** @} */ // end of oslMath

#ifdef __cplusplus
}
#endif

#endif
