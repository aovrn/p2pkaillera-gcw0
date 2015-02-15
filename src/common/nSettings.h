/******************************************************************************
***  n02 v0.3 winnt                                                         ***
**   Open Kaillera Client Core                                               **
***  For latest sources, visit http://sf.net/projects/okai                  ***
******************************************************************************/
#pragma once

#include "linux.h"

#ifndef MAX_PATH
#define MAX_PATH 511
#endif

extern char szKailleraHomePath[MAX_PATH];

class nSettings {
public:
	static void Initialize();
	static void Terminate();
	static int get_int(char * key);
	static char* get_str(char * key, char * buf, size_t size);
	static void set_int(char * key, int val);
	static void set_str(char * key, char * val);

protected:
	static void LoadCfg();
	static void WriteCfg();
	static void DefaultCfg();
	static bool GetMac(char * buf, size_t size);
};
