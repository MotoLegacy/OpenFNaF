#ifndef __OSL_CCC_H__
#define __OSL_CCC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "intraFont/libccc.h"
/** @defgroup Character code conversion (ccc)

	Functions to convert characters from BenHur's intraFont
	@{
*/

/**
 * Character code conversion
 *
 * @param dst - output string
 *
 * @param count - size of output buffer
 *
 * @param str - input string
 *
 * @param cp - codepage
 *
 * @returns number of converted character codes
 */
#define oslCccSJIStoUCS2    cccSJIStoUCS2
#define oslCccGBKtoUCS2     cccGBKtoUCS2
#define oslCccKORtoUCS2     cccKORtoUCS2
#define oslCccBIG5toUCS2    cccBIG5toUCS2
#define oslCccUTF8toUCS2    cccUTF8toUCS2
#define oslCccCodetoUCS2    cccCodetoUCS2

/**
 * Set error character (character that's used for code points where conversion failed)
 *
 * @param code - new error character (default: 0)
 *
 * @returns previous error character
 */
#define oslCccSetErrorCharUCS2  cccSetErrorCharUCS2

/**
 * Shutdown the Character Code Conversion Library
 */
#define oslCccShutDown  cccShutDown

/** @} */ // end of Character code conversion

#ifdef __cplusplus
}
#endif

#endif
