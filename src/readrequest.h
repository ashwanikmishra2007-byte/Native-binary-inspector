#ifndef READREQUEST_H
#define READREQUEST_H

#include <stddef.h>
#include <windows.h>
#include "filereader.h"

typedef struct
{
    OVERLAPPED overlapped;     // Windows uses this to track an asynchronous I/O operation.
    unsigned char *buffer;     // Holds the bytes returned by this particular read.
    size_t buffer_size;        // The size of the buffer allocated for this particular read.
    ULONGLONG offset; // The offset in the file where this particular read starts.
    DWORD requested_bytes;     // The number of bytes requested by this particular read.
    DWORD bytes_read;          // The number of bytes actually read by this particular read.
    DWORD error_code;
    Resultstatus status;       // The status of this particular read.
} Readrequest;

typedef enum
{
    REQUEST_CREATED,
    REQUEST_PENDING,
    REQUEST_COMPLETED,
    REQUEST_ERROR
} Resultstatus;

Readrequest *create_read_request(Binstorage *store, ULONGLONG offset, size_t buffer_size, DWORD requested_bytes);
void destroy_read_request(Readrequest *request);

#endif
