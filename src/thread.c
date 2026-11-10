#include "thread.h"

typedef DWORD(WINAPI *ThreadFunction)(LPVOID);

Thread *creatth(void)
{
    Thread *worker = malloc(sizeof(*worker));
    if (worker == NULL)
    {
        return NULL;
    }

    worker->handle = NULL;
    worker->thread_id = 0;
    worker->valid = false;
    return worker;
}

Thread *threadstart(ThreadFunction function, void *argument)
/*ThreadFunction function

This is a function pointer. It represents the function that the newly created thread will execute.*/
{
    if (function == NULL)
    {
        return NULL;
    }

    Thread *worker = creatth();

    if (worker == NULL)
    {
        return NULL;
    }

    unsigned int threadid = 0;

    uintptr_t handle = _beginthreadex(
        NULL,     // Security	NULL	Use default security attributes
        0,        // Stack size	0	Use default stack size
        function, // Start function	function	Function executed by the new thread
        argument, // Argument	argument	Data passed to that function
        0,        // Creation flags	0	Start executing immediately
        &threadid // Thread ID	&thread_id	Address where Windows writes the thread ID
    );

    if (handle == 0)
    {
        free(worker);
        return NULL;
    }

    worker->handle = (HANDLE)handle;
    worker->thread_id = threadid;
    worker->valid = true;

    return worker;
}

bool threajoin(Thread *worker)
{
    if (worker == NULL || !worker->valid)
    {
        return false;
    }

    DWORD result = WaitForSingleObject(
        worker->handle, // hHandle: handle of the thread you want to wait for.
        INFINITE        // dwMilliseconds: maximum time to wait.
    );

    if (result != WAIT_OBJECT_0)
    { // WAIT_OBJECT_0	Thread has terminated
        return false;
    }

    return true;
}

bool threadclose(Thread *worker)
{
    if (worker == NULL || !worker->valid)
    {
        return false;
    }

    if (!CloseHandle(worker->handle))
    {
        return false;
    }

    worker->handle = NULL;
    worker->valid = false;

    return true;
}

bool threaddestroy(Thread *worker)
{
    if (worker == NULL)
    {
        return false;
    }

    if (worker->valid)
    {
        return false;
    }

    free(worker);
    return true;
}