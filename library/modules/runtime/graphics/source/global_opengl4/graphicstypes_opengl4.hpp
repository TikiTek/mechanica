#pragma once
#ifndef __TIKI_GRAPHICSTYPES_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSTYPES_OPENGL4_HPP_INCLUDED__

typedef unsigned int GLuint;
typedef unsigned int GLenum;

//#ifndef WIN_INTERNAL
//#define WIN_INTERNAL
//
//typedef void* HANDLE;
//#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
//
//DECLARE_HANDLE( HWND );
//DECLARE_HANDLE( HHOOK );
//
//#ifndef GDI_INTERNAL
//#define GDI_INTERNAL
//
//DECLARE_HANDLE( HDC );
//
//#endif
//
//DECLARE_HANDLE( HGLRC );
//
//#endif

namespace tiki
{
	TIKI_DEFINE_HANLE( WindowHandle );
	TIKI_DEFINE_HANLE( DeviceContextHandle );
	TIKI_DEFINE_HANLE( RenderContextHandle );
}

#endif // __TIKI_GRAPHICSTYPES_OPENGL4_HPP_INCLUDED__
