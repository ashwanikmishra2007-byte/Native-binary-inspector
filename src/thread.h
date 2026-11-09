#ifndef THREAD_H
#define THREAD_H

#include <windows.h>
#include "filereader.h"

typedef struct{
    HANDLE handle; // Handle to the thread
    DWORD thread_id; // Thread ID
    bool valid;
} Thread;

Thread* creatth(void);

#endif