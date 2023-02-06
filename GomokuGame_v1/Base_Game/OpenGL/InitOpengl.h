
#ifndef INITOPENGL_H__
#define INITOPENGL_H__
#pragma warning(disable:4996)

#include "include\vgl.h"




#define GLEW_STATIC
#ifdef _MSC_VER
#  ifdef _DEBUG
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "OpenGL/lib/glew_static_vs2010_d.lib")
#    else
#      pragma comment (lib, "OpenGL/lib/glew_static_d.lib")
#    endif
#  else
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "OpenGL/lib/glew_static_vs2010.lib")
#    else
#      pragma comment (lib, "OpenGL/lib/glew_static.lib")
#    endif
#  endif
#endif

#define FREEGLUT_STATIC
#ifdef _MSC_VER
#  ifdef _DEBUG
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "OpenGL/lib/freeglut_static_vs2010_d.lib")
#    else
#      pragma comment (lib, "OpenGL/lib/freeglut_static_d.lib")
#    endif
#  else
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "OpenGL/lib/freeglut_static_vs2010.lib")
#    else
#      pragma comment (lib, "OpenGL/lib/freeglut_static.lib")
#    endif
#  endif
#endif

#endif

#if _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif
