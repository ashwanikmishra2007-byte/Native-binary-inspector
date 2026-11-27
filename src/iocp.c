#include "iocp.h"

IOCPinfo *iocp_int(DWORD concurrency)
{
    IOCPinfo *iocp = malloc(sizeof(*iocp));

    if (iocp == NULL)
    {
        return NULL;
    }

    iocp->completion_port = NULL;
    iocp->initialised = false;
    iocp->error_code = ERROR_SUCCESS;

    HANDLE port = CreateIoCompletionPort(
        INVALID_HANDLE_VALUE, // INVALID_HANDLE_VALUE: Create a new completion port.
        NULL,                 // NULL: No existing completion port to associate.
        0,                    // 0: No completion key is needed during creation.
        concurrency           // Maximum number of concurrently active completion-processing threads.
    );

    if (port == NULL)
    {
        iocp->error_code = GetLastError();
        free(iocp);
        return NULL;
    }

    iocp->completion_port = port;
    iocp->initialised = true;

    return iocp;
}

bool iocpdestroy(IOCPinfo *iocp)
{
    if (iocp == NULL)
        return false;

    if (iocp->initialised && iocp->completion_port != NULL)
    {
        if (!CloseHandle(iocp->completion_port))
        {
            iocp->error_code = GetLastError();
            return false;
        }
    }

    free(iocp);
    return true;
}

bool iocp_associate(IOCPinfo *iocp, HANDLE file, ULONG_PTR completion_key)
{
    if (iocp == NULL || !iocp->initialised)
        return false;

    if (file == NULL || file == INVALID_HANDLE_VALUE)
        return false;

    HANDLE result = CreateIoCompletionPort(
        file,
        iocp->completion_port,
        completion_key,
        0 // Concurrency is configured when creating the completion port. It is ignored when associating an existing file handle.
    );

    if (result == NULL)
    {
        iocp->error_code = GetLastError();
        return false;
    }

    iocp->error_code = ERROR_SUCCESS;
    return true;
}

IOCPCompletionstatus*iocp_get_completion(IOCPCompletion*iocpcomplete, IOCPinfo*info, DWORD timeout){
    if (info == NULL || !info->initialised ||
        info->completion_port == NULL || iocpcomplete == NULL)
    {
        return IOCP_COMPLETION_WAIT_FAILED;
    }

    iocpcomplete->completion_key = 0;
    iocpcomplete->bytes_transferred = 0;
    iocpcomplete->overlapped = NULL;
    iocpcomplete->error_code = ERROR_SUCCESS;

    BOOL result = GetQueuedCompletionStatus(
        info->completion_port,        //Handle of our IOCP.
        &iocpcomplete->bytes_transferred,     //Receives the number of bytes transferred by the completed operation.
        &iocpcomplete->completion_key,     //Receives the completion key associated with the file handle.
        &iocpcomplete->overlapped,      //Receives a pointer to the OVERLAPPED structure belonging to the completed request.
        timeout                         //Maximum time to wait for a completion packet.
    );

    if (!result)
    {
        DWORD error = GetLastError();

        iocpcomplete->error_code = error;

        if (iocpcomplete->overlapped != NULL)
        {
            return IOCP_COMPLETION_IO_FAILED;
        }

        return IOCP_COMPLETION_WAIT_FAILED;
    }

    return IOCP_COMPLETION_SUCCESS;
}