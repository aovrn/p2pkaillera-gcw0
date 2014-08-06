#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <stdarg.h>

#ifndef MAX_PATH
#define MAX_PATH 511
#endif

#define TCHAR char
#define DWORD unsigned long

#define sprintf_s snprintf
#define vsnprintf_s(a, b, c, d, e) vsnprintf(a, b, d, e)

void Sleep(int ms);
unsigned long GetTickCount();
