#ifndef DATAPOOL_H
#define DATAPOOL_H

#include <windows.h>
#include <stdlib.h>
#include "fileidentity.h"
#include "fileavl.h"

typedef struct Bufferentry Bufferentry;
typedef struct FileEntry FileEntry;

typedef struct 
{
    const unsigned char *data;
    size_t size;
    Bufferentry *entry;
} Dataview;

typedef struct
{
    ULONGLONG offset;
    size_t data_size;
    unsigned char *buffer;
    LONG reference_count;
    struct Bufferentry *next; // Allows us to connect multiple buffers belonging to the same file.

} Bufferentry;

typedef struct
{
    size_t total_memory;
    size_t total_buffers;
    size_t total_files;
    FileEntry *files;

    CRITICAL_SECTION lock; // Without synchronization, they could modify the same linked list simultaneously, corrupting its structure or accounting.

    // the critical section protects these operations.
    BOOL initialized;
    Fileavlnode*filenode;

} Datapool;

typedef struct
{
    FileIdentity *identity;
    size_t buffer_count;
    size_t total_memory;
    Bufferentry *buffers;
    FileEntry *next;

} FileEntry;

typedef enum
{
    DATAPOOL_SUCCESS,
    DATAPOOL_NOT_AVAILABLE,
    DATAPOOL_INVALID_RANGE,
    DATAPOOL_OVERLAP,
    DATAPOOL_OUT_OF_MEMORY,
    DATAPOOL_ERROR,
    DATAPOOL_INVALID_ARGUMENT,
    DATAPOOL_NOT_FOUND,
    DATAPOOL_OFFSET_UNAVAILABLE,
    DATAPOOL_INVALID_VIEW
} Datapoolstatus;

Datapool *datapool_init();
Datapoolstatus datapool_add(Datapool *pool, const FileIdentity *identity, ULONGLONG offset, unsigned char *buffer, size_t data_size);
Datapoolstatus datapool_acquire(Datapool*pool, const FileIdentity*identity, ULONGLONG offset, Dataview*view);

#endif