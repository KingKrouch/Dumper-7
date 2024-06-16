#ifdef _MSC_VER
// If the compiler is MSVC, include Windows.h
#include <Windows.h>
#else
// If the compiler is not MSVC, include windows.h. For some reason, MinGW is not case sensitive.
#include <windows.h>
#endif