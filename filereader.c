#include <stdio.h>
#include <stdlib.h>
#include "filereader.h"

Filestorage readfile(const char*filename, Filestorage* filestore){
    FILE*fileptr= fopen(filename, "rb");
    
}