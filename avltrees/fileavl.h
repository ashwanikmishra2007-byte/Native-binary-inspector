#pragma once

#include<windows.h>
#include "fileidentity.h"
#include "datapool.h"

typedef struct{
    FileIdentity *identity;
    FileEntry *entry;
    Fileavlnode*left;
    Fileavlnode*right;
    int height;
} Fileavlnode;

/*typedef struct{
    Fileavlnode*root;
    size_t count;
} Filetree;*/

Fileavlnode*createfile(FileEntry*entry);
int getheight (Fileavlnode*filenode);
int getbf(Fileavlnode*filenode);
Fileavlnode*leftrotate(Fileavlnode*filenode);
Fileavlnode*insert(Fileavlnode*filenode, FileEntry*identity);
int cmpnode(const FileIdentity*first, const FileIdentity*second);
