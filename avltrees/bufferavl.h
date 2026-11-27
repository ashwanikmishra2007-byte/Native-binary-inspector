#ifndef BUFFERAVL_H
#define BUFFERAVL_H

#include <windows.h>
#include "datapool.h"

typedef struct {
    ULONGLONG offset;
    Bufferentry* entry;
    Buffernode*left;
    Buffernode*right;
    int height;
} Buffernode;

Buffernode*create(Bufferentry*entry);
int getheight(Buffernode*node);
int getbf(Buffernode*node);
Buffernode*leftrotate(Buffernode*node);
Buffernode*rightrotate(Buffernode*node);
Buffernode*insert(Buffernode*node, Bufferentry*entry);
#endif