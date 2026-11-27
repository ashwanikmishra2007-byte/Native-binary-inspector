#ifndef IOCP_H
#define IOCP_H

#include <windows.h>
#include <stdbool.h>

typedef struct
{
    HANDLE completion_port;
    bool initialised;
    DWORD error_code;
} IOCPinfo;

typedef enum
{
    IOCP_COMPLETION_IO_FAILED,
    IOCP_COMPLETION_WAIT_FAILED,
    IOCP_COMPLETION_SUCCESS,
} IOCPCompletionstatus;

typedef struct
{
    ULONG_PTR completion_key; // Identifies the associated file or other context.
    DWORD bytes_transferred;  // Number of bytes transferred by the completed operation.
    OVERLAPPED *overlapped;   // Identifies the original I/O request.
    DWORD error_code;         // Stores the Windows error code, if applicable.
} IOCPCompletion;

IOCPinfo *iocp_int(DWORD concurrency);
bool iocpdestroy(IOCPinfo *iocp);
bool iocpassociate(IOCPinfo *iocp, HANDLE file, ULONG_PTR completion_key);
IOCPCompletionstatus *iocp_get_completion(IOCPCompletion *iocpcomplete, IOCPinfo *info, DWORD timeout);

#endif