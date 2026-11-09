
#include <windows.h>
#include <stdio.h>

DWORD WINAPI worker_function(LPVOID parameter)
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 1; i <= 5; i++)
    {
        char message[50];

        DWORD length = sprintf(
            message,
            "Worker thread: %d\n",
            i
        );

        DWORD written;
        WriteFile(output, message, length, &written, NULL);

        Sleep(500);
    }

    return 0;
}

int main(void)
{
    printf("Main thread: Starting worker...\n");

    HANDLE worker = CreateThread(
        NULL,
        0,
        worker_function,
        NULL,
        0,
        NULL
    );

    if (worker == NULL)
    {
        printf("Thread creation failed: %lu\n",
               GetLastError());
        return 1;
    }

    printf("Main thread: Worker created!\n");

    for (int i = 1; i <= 5; i++)
    {
        printf("Main thread: %d\n", i);
        Sleep(300);
    }

    printf("Main thread: Waiting for worker...\n");

    WaitForSingleObject(worker, INFINITE);

    DWORD exit_code;

    if (GetExitCodeThread(worker, &exit_code))
    {
        printf("Worker exit code: %lu\n", exit_code);
    }

    CloseHandle(worker);

    printf("Main thread: Worker finished.\n");

    return 0;
}
