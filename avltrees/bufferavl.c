#include "bufferavl.h"

Buffernode *create(Bufferentry *entry)
{
    if (entry == NULL || entry->offset < 0)
    {
        return NULL;
    }

    Buffernode *node = malloc(sizeof(Buffernode));

    node->offset = entry->offset;
    node->left = NULL;
    node->right = NULL;
    node->entry = entry;
    node->height = 1;
    return node;
}

int getheight(Buffernode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

int getbf(Buffernode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return (getheight(node->left)) - (getheight(node->right));
}

int maxh(int a, int b)
{
    return a > b ? a : b;
}

Buffernode *leftrotate(Buffernode *node)
{
    Buffernode *y = node->right;
    Buffernode *n = y->left;

    y->left = node;
    node->right = n;

    y->height = maxh(getheight(y->left), getheight(y->right));
    node->height = maxh(getheight(node->left), getheight(node->right)) + 1;
    return y;
}

Buffernode *rightrotate(Buffernode *node)
{
    Buffernode *y = node->left;
    Buffernode *n = y->right;

    y->right = node;
    node->left = n;

    y->height = maxh(getheight(y->left), getheight(y->right)) + 1;
    node->height = maxh(getheight(node->left), getheight(node->right)) + 1;
    return y;
}

int cmpoffset(Bufferentry *first, Bufferentry *second)
{
    if (first->offset < second->offset)
    {
        return 1;
    }
    else if (first->offset > second->offset)
    {
        return -1;
    }
    return 0;
}

Buffernode *insert(Buffernode *node, Bufferentry *entry)
{
    if (entry == NULL || entry->offset < 0)
        return node;

    if (node == NULL)
        return createfile(entry);

    int cmp = cmpoffset(node->entry, entry);

    if (cmp < 0)
    {
        node->left = insert(node->left, entry);
    }
    else if (cmp > 0)
    {
        node->right = insert(node->right, entry);
    }
    else
    {
        return node;
    }
    node->height = 1 + maxh(
                               get_height(node->left),
                               get_height(node->right));

    int bf = getbf(node);
    //LL case
    if (bf > 1 && cmpnode(node->entry, entry)<0){
        return rightrotate(node);
    }
    //RR case 
    if(bf<-1 && cmpnode(node->entry, entry)>0){
        return leftrotate(node);
    }
    //LR case
    if(bf<-1 && cmpnode(node->entry, entry)>0){
        node->left= leftrotate(node->left);
        return rightrotate(node);
    }
    //RL case
    if(bf>1 && cmpnode(node->entry, entry)>0){
        node->right= rightrotate(node->left);
        return leftrotate(node);
    }
    return node;
}
