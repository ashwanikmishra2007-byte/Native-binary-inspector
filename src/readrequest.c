#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "readrequest.h"
#include "filereader.h"

Readrequest *create_read_request(Binstorage *store, ULONGLONG offset, size_t buffer_size, DWORD requested_bytes)
{
    if (store == NULL ||
        store->file == INVALID_HANDLE_VALUE ||
        buffer_size == 0 ||
        requested_bytes == 0 ||
        requested_bytes > buffer_size)
    {
        return NULL;
    }

    if (store->size < offset || requested_bytes > (store->size - offset))
    {
        return NULL;
    }

    Readrequest *request = malloc(sizeof(*request));

    if (request == NULL)
    {
        return NULL;
    }

    request->buffer = malloc(buffer_size);
    if (request->buffer == NULL)
    {
        free(request);
        return NULL;
    }

    memset(&request->overlapped, 0, sizeof(request->overlapped));   // Initialize the OVERLAPPED structure to zero.

    request->buffer_size = buffer_size;
    request->offset = offset;
    request->requested_bytes = requested_bytes;
    request->bytes_read = 0;
    request->error_code = ERROR_SUCCESS;
    request->status = REQUEST_CREATED;
    request->overlapped.Offset = (DWORD)(offset & 0xFFFFFFFF);         /*0xFFFFFFFF is a 32-bit mask containing all ones.
               The bitwise AND operation extracts the lower 32 bits.*/
    request->overlapped.OffsetHigh = (DWORD)((offset >> 32) & 0xFFFFFFFF); /*offset >> 32 shifts the upper 32 bits into the lower 32-bit position.
    Casting to DWORD retains those 32 bits
    We assign them to OffsetHigh.*/
    return request;
}

void destroy_read_request(Readrequest *request)
{
    if (request == NULL)
    {
        return;
    }

    if (request->buffer != NULL)
    {
        free(request->buffer);
    }

    free(request);
}