
#include <stdio.h>
#include <windows.h>
#include "filereader.h"

void test_existing_file(const wchar_t *path)
{
    printf("\n========== EXISTING FILE TEST ==========\n");

    Binstorage *store = create();

    if (store == NULL)
    {
        printf("create() failed\n");
        return;
    }

    printf("Binstorage created successfully\n");

    Openstatus status = open_file(store, path);

    if (status == OPEN_SUCCESS)
    {
        printf("File opened successfully\n");
        printf("File size: %llu bytes\n", store->size);
        printf("Error code: %lu\n", store->error_code);
    }
    else
    {
        printf("Opening failed. Status: %d\n", status);
        printf("Windows error: %lu\n", store->error_code);
    }

    // Try opening the same file again.
    status = open_file(store, path);

    if (status == OPEN_ALREADY_OPEN)
    {
        printf("Duplicate opening correctly prevented\n");
    }
    else
    {
        printf("Unexpected result: %d\n", status);
    }

    destroy(store);
    printf("Binstorage destroyed\n");
}

void test_nonexistent_file(void)
{
    printf("\n========== NONEXISTENT FILE TEST ==========\n");

    Binstorage *store = create();

    if (store == NULL)
        return;

    Openstatus status = open_file(
        store,
        L"this_file_does_not_exist_12345.bin");

    printf("Status: %d\n", status);
    printf("Windows error: %lu\n", store->error_code);

    destroy(store);
}

void test_empty_file(void)
{
    printf("\n========== EMPTY FILE TEST ==========\n");

    const wchar_t *path = L"test_empty.bin";

    // Create an empty test file.
    HANDLE temp = CreateFileW(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (temp == INVALID_HANDLE_VALUE)
    {
        printf("Could not create empty test file\n");
        return;
    }

    CloseHandle(temp);

    Binstorage *store = create();

    if (store == NULL)
        return;

    Openstatus status = open_file(store, path);

    if (status == OPEN_SUCCESS)
    {
        printf("Empty file opened successfully\n");
        printf("File size: %llu bytes\n", store->size);
    }
    else
    {
        printf("Opening failed. Error: %lu\n", store->error_code);
    }

    destroy(store);
    DeleteFileW(path);
}

void test_null_path(void)
{
    printf("\n========== NULL PATH TEST ==========\n");

    Binstorage *store = create();

    if (store == NULL)
        return;

    Openstatus status = open_file(store, NULL);

    printf("Status: %d\n", status);

    destroy(store);
}

int main(void)
{
    test_existing_file(L"C:\\Windows\\win.ini");

    test_nonexistent_file();

    test_empty_file();

    test_null_path();

    return 0;
}
