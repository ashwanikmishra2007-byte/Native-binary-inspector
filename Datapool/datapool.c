#include "datapool.h"
#include <string.h>

Datapool *datapool_init()
{
    Datapool *pool = malloc(sizeof(Datapool));

    if (pool == NULL)
        return NULL;

    pool->total_memory = 0;
    pool->total_buffers = 0;
    pool->total_files = 0;

    pool->files = NULL;

    if (!InitializeCriticalSectionEx(&pool->lock, 0, 0)) /*This specifies the spin count.
A thread attempting to acquire a critical section may briefly spin (repeatedly check whether the lock is available) before waiting.
third argument This specifies initialization flags.
Zero means we use the default initialization behavior.*/
    {
        free(pool);
        return NULL;
    }
    pool->initialized = TRUE;

    return pool;
}

static BOOL same_file(const FileIdentity *a, const FileIdentity *b)
{
    return a->volume_serial_number == b->volume_serial_number &&
           memcmp(&a->file_id, &b->file_id, sizeof(FILE_ID_128)) == 0;
}

Datapoolstatus datapool_add(Datapool *pool, const FileIdentity *identity, ULONGLONG offset, unsigned char *buffer, size_t data_size)
{
    if (pool == NULL || identity == NULL || buffer == NULL)
        return DATAPOOL_ERROR;

    if (!pool->initialized)
        return DATAPOOL_ERROR;

    if ((ULONGLONG)data_size > (~(ULONGLONG)0 - offset)) // This line is checking for unsigned integer overflow when calculating the ending offset of a buffer.
        return DATAPOOL_INVALID_RANGE;

    ULONGLONG end = offset + (ULONGLONG)data_size;

    Bufferentry *new_buffer = malloc(sizeof(Bufferentry));

    if (new_buffer == NULL)
        return DATAPOOL_OUT_OF_MEMORY;

    new_buffer->offset = offset;
    new_buffer->data_size = data_size;
    new_buffer->buffer = buffer;
    new_buffer->reference_count = 0;
    new_buffer->next = NULL;

    EnterCriticalSection(&pool->lock);

    FileEntry *file = pool->files;

    while (file != NULL)
    {
        if (same_file(&file->identity, identity))
        {
            break;
        }
        file = file->next;
    }

    if (file != NULL)
    {
        Bufferentry *current = file->buffers;
        while (current != NULL)
        {
            ULONGLONG current_end =
                current->offset + (ULONGLONG)current->data_size;

            if (offset < current_end && current->offset < end)
            {
                LeaveCriticalSection(&pool->lock);
                free(new_buffer);
                return DATAPOOL_OVERLAP;
            }

            current = current->next;
        }
    }

    if(file==NULL){
        file = malloc(sizeof(FileEntry));

        if (file == NULL)
        {
            LeaveCriticalSection(&pool->lock);
            free(new_buffer);
            return DATAPOOL_OUT_OF_MEMORY;
        }

        file->identity = identity;
        file->buffer_count = 0;
        file->total_memory = 0;
        file->buffers = NULL;

        file->next = pool->files;
        pool->files = file;

        pool->total_files++;
    }

    new_buffer->next=file->buffers;
    file->buffers=new_buffer;
    file->buffer_count++;
    file->total_memory += data_size;
    pool->total_buffers++;
    pool->total_memory += data_size;

    LeaveCriticalSection(&pool->lock);

    return DATAPOOL_SUCCESS;
}

