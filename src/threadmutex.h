#ifndef THREADMUTEX_H
#define THREADMUTEX_H

#include <windows.h>
#include "filereader.h"

typedef struct{
    CRITICAL_SECTION lock;   //lock: The actual Windows synchronization object. It manages which thread is allowed to enter a protected section.
    bool initialise;   //Tracks whether the critical section has been successfully initialized. This prevents us from using or destroying an uninitialized lock.
} Threadmutex;

#endif