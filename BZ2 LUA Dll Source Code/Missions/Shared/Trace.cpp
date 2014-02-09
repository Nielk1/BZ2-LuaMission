#ifdef WIN32
#include <Windows.h>
#endif
#include <stdio.h>

#include "Trace.h"

// Dumps out items to the debugging console, in a debug build

#ifndef WIN32
#define __cdecl
#endif

void __cdecl TraceA(const char *fmt, ...)
{
#ifdef _DEBUG

	va_list v_args;
	char Text[2048];
	
	va_start (v_args, fmt);     
	_vsnprintf(Text, sizeof(Text)-1,fmt, v_args);
	va_end (v_args);            
	
	OutputDebugString	(Text);
	printf("%s",Text);
#else
	(fmt); // avoid a compiler warning
#endif	
}
