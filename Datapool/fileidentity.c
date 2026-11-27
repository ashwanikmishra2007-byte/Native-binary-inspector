
#include "fileidentity.h"
#include <string.h>

typedef struct
{
    ULONGLONG VolumeSerialNumber;
    FILE_ID_128 FileId;
} COMPAT_FILE_ID_INFO;

BOOL populate_file_identity(
    HANDLE file_handle,
    FileIdentity *identity
)
{
    COMPAT_FILE_ID_INFO info;

    if (file_handle == NULL ||
        file_handle == INVALID_HANDLE_VALUE ||
        identity == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (!GetFileInformationByHandleEx(
            file_handle,  //Handle of an already-open file
            (FILE_INFO_BY_HANDLE_CLASS)18, // FileIdInfo; supports SDKs that omit its enum name
            &info,        //Address where Windows will store the result
            sizeof(info)))   //Size of the receiving structure
    {
        return FALSE;
    }

    identity->volume_serial_number =
        info.VolumeSerialNumber;

    memcpy(
        &identity->file_id,
        &info.FileId,
        sizeof(FILE_ID_128)
    );

    return TRUE;
}
