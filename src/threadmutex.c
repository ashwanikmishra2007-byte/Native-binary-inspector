#include "threadmutex.h"

Threadmutex *create_mutex(void)
{
    Threadmutex *mutex = malloc(sizeof(*mutex));
    if (mutex == NULL)
    {
        return NULL;
    }
    mutex->initialise = false;

    if (!InitializeCriticalSectionEx(&mutex->lock, 0, 0))
    {
        /*dwSpinCount
        Type: DWORD
        Determines how many times a thread should spin (actively retry acquiring the lock) before waiting.
        Flags
        Type: DWORD
        Reserved for future use. It must currently be set to 0.*/
        free(mutex);
        return NULL;
    }
    mutex->initialise = true;
    return mutex;
}

bool lockmutex(Threadmutex *mutex)
{
    if (mutex == NULL)
    {
        return false;
    }

    if (!mutex->initialise)
    {
        return false;
    }

    EnterCriticalSection(&mutex->lock);
    return true;
}

bool unlockmutex(Threadmutex*mutex){
    if(mutex==NULL){
        return false;
    }

    if (!mutex->initialise)
        return false;

    LeaveCriticalSection(&mutex->lock);

    return true;
}

bool destroy(Threadmutex*mutex){
    if (mutex == NULL)
        return false;

    if (!mutex->initialise)
        return false;

    DeleteCriticalSection(&mutex->lock);
    mutex->initialise=false;
    free(mutex);
    return true;
}

