#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "filereader.h"
#include <windows.h>

Binstorage *create(void)
{

    Binstorage *store = malloc(sizeof(*store));

    if (store == NULL)
    {
        return NULL;
    }

    store->file = INVALID_HANDLE_VALUE;
    store->size = 0;
    store->error_code = ERROR_SUCCESS;

    return store;
}

Openstatus open_file(Binstorage *store, const wchar_t *path)
{

    if (path == NULL)
    {
        printf("Incorrect path\n");
        return OPEN_INVALID_ARGUMENT;
    }

    if (store == NULL)
    {
        printf("Could not allocate memory\n");
        return OPEN_INVALID_ARGUMENT;
    }

    if (store->file != INVALID_HANDLE_VALUE)
    {
        return OPEN_ALREADY_OPEN;
    }

    HANDLE file = CreateFileW(
        path,
        GENERIC_READ,                                           // GENERIC_READ, because our inspector only needs to read.
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, /*FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, allowing other processes to read,
       write, or delete the file while we have it open, subject to Windows' sharing rules.*/
        NULL,
        OPEN_EXISTING,                                /*OPEN_EXISTING, because we must inspect an existing file, never create a missing one.*/
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, /*FILE_ATTRIBUTE_NORMAL for ordinary file access, and FILE_FLAG_OVERLAPPED to enable asynchronous I/O operations.*/
        NULL);
    wprintf(L"Attempting to open: [%ls]\n", path);
    if (file == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();

        printf("CreateFileW failed. Error code: %lu\n",
               (unsigned long)error);
        store->error_code = error;
        return OPEN_ERROR;
    }

    LARGE_INTEGER filesize;

    if (!GetFileSizeEx(file, &filesize))
    { // Get file size didnt work
        DWORD error = GetLastError();
        CloseHandle(file);
        store->file = INVALID_HANDLE_VALUE;
        printf("Function failed\n");
        store->error_code = error;
        return OPEN_ERROR;
    }

    if (filesize.QuadPart < 0)
    { // If file size is valid
        CloseHandle(file);
        printf("Invalid size returned\n");
        store->error_code = INVALID_FILE_SIZE;
        return OPEN_ERROR;
    }

    store->size = (unsigned long long)filesize.QuadPart;
    store->file = file;
    printf("Successful implementation\n");
    store->error_code = ERROR_SUCCESS; // Reset error code on success

    return OPEN_SUCCESS;
}

void destroy(Binstorage *store)
{
    if (store == NULL)
    {
        return;
    }

    if (store->file != INVALID_HANDLE_VALUE)
    {

        CloseHandle(store->file);
    }
    free(store);
}

/*int main(void)
{
    wchar_t cwd[MAX_PATH];

    GetCurrentDirectoryW(MAX_PATH, cwd);

    wprintf(L"Current directory: %ls\n", cwd);
    Binstorage*store= open_file(L"filereader.c");
    if (store == NULL)
    {
        printf("Failed to open file.\n");
        return 1;
    }

    printf("File opened successfully!\n");
    printf("File size: %llu bytes\n", store->size);
    printf("Current position: %llu\n", store->position);

    return 0;
}*/