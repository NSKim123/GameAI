#ifndef __VGL_H__
#define __VGL_H__

// #define USE_GL3W

#ifdef USE_GL3W

#include "GL3/gl3.h"
#include "GL3/gl3w.h"

#else

#define GLEW_STATIC
#include "GL/glew.h"

#endif

#define FREEGLUT_STATIC
#include "GL/freeglut.h"


#if _MSC_VER>=1900
#include "stdio.h"
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus
extern "C"
#endif
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif

#define BUFFER_OFFSET(x)  ((const void*) (x))

#endif