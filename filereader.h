#ifndef FILE_READER_H
#define FILE_READER_H

#include<stddef.h>

typedef struct 
{
    const char*data;
    size_t size;
} Filestorage;

Filestorage readfile(const char*filename, Filestorage*filestore);

#endif
