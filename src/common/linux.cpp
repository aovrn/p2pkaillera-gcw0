#include "linux.h"

void Sleep(int ms) {
	usleep(ms * 1000);
}

unsigned long GetTickCount() { 
    struct timeval tv; 
    gettimeofday(&tv,NULL); 
    return (tv.tv_sec*1000+tv.tv_usec/1000); 
}
