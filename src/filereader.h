
#ifndef FILEREADER_H
#define FILEREADER_H

#include <stddef.h>
#include <windows.h>

typedef struct
{
    HANDLE file;             // The file handle returned by CreateFileW.
    unsigned long long size; // The size of the file in bytes, as returned by GetFileSizeEx.
    DWORD error_code;
} Binstorage;

typedef struct
{
    OVERLAPPED overlapped;     // Windows uses this to track an asynchronous I/O operation.
    unsigned char *buffer;     // Holds the bytes returned by this particular read.
    size_t buffer_size;        // The size of the buffer allocated for this particular read.
    unsigned long long offset; // The offset in the file where this particular read starts.
    DWORD requested_bytes;     // The number of bytes requested by this particular read.
    DWORD bytes_read;          // The number of bytes actually read by this particular read.
    DWORD error_code;          // The error code returned by this particular read.
} Readrequest;

typedef enum
{
    READ_SUCCESS,
    READ_EOF,
    READ_ERROR
} Resultstatus;

typedef enum
{
    OPEN_SUCCESS,
    OPEN_ERROR,
    OPEN_ALREADY_OPEN,
    OPEN_INVALID_ARGUMENT
} Openstatus;

Binstorage *create(void);

Openstatus open_file(Binstorage *store, const wchar_t *path);

Binstorage *read_file();

void destroy(Binstorage *store);

#endif
