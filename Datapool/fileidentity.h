
#ifndef FILEIDENTITY_H
#define FILEIDENTITY_H

#include <windows.h>

typedef struct
{
    ULONGLONG volume_serial_number;
    FILE_ID_128 file_id;

} FileIdentity;
FileIdentity get_file_identity(HANDLE file_handle);

#endif
