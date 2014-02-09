#ifndef __Trace_H_
#define __Trace_H_

// Dumps out items to the debugging console, in a debug build
//
// Note that in a release build, the entire line with trace is
// compiled away. This is because DevStudio (stupidly!) leaves the
// format string in the resulting binary

#ifdef _DEBUG
 void __cdecl TraceA(const char *fmt, ...);
 #define Trace(x) TraceA x
#else
 #define Trace(x) {}
#endif

#endif
