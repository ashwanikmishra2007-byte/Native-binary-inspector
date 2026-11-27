#ifndef THREAD_H
#define THREAD_H

#include <windows.h>
#include "filereader.h"

typedef unsigned (__stdcall *ThreadFunction)(void *);   //ThreadFunction is a function pointer type that describes the function a newly created thread is allowed to execute.

typedef struct
{
    HANDLE handle;   // Handle to the thread
    DWORD thread_id; // Thread ID
    bool valid;
} Thread;

Thread *creatth(void);
Thread *threadstart(ThreadFunction function, void *argument);
bool threadjoin(Thread *worker);
bool threadclose(Thread *worker);

#endif