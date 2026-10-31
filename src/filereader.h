#ifndef FILE_READER_H
#define FILE_READER_H

#include <stddef.h>
#include <windows.h>

typedef struct
{
    HANDLE file;
    unsigned long long size;
    unsigned long long position;
    unsigned char *data;
    size_t buffer_size;
    size_t bytesread;
    DWORD error_code;
} Binstorage;

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

Binstorage *create(size_t buffersize);

Openstatus open_file(Binstorage*store, const wchar_t *path);

Binstorage *read_file();

#endif
