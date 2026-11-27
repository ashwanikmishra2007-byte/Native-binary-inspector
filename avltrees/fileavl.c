#include "fileavl.h"

Fileavlnode *createfile(FileEntry *entry)
{
    if (entry == NULL || entry->identity == NULL)
    {
        return NULL;
    }

    Fileavlnode *node = malloc(sizeof(Fileavlnode));

    if (node == NULL)
    {
        return NULL;
    }

    node->identity = entry->identity;
    node->entry = entry;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int getheight(Fileavlnode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return node->height;
}

int getbf(Fileavlnode *node)
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

int cmpnode(const FileIdentity *first, const FileIdentity *second)
{
    if (first->volume_serial_number <
        second->volume_serial_number)
        return -1;

    if (first->volume_serial_number >
        second->volume_serial_number)
        return 1;

    int result = memcmp(
        &first->file_id,
        &second->file_id,
        sizeof(FILE_ID_128));

    if (result < 0)
        return -1;

    if (result > 0)
        return 1;

    return 0;
}

Fileavlnode *rightrotate(Fileavlnode *filenode)
{
    Fileavlnode *y = filenode->left;
    Fileavlnode *n = y->right;

    y->right = filenode;
    filenode->left = n;

    filenode->height = maxh(
                           get_height(filenode->left),
                           get_height(filenode->right)) +
                       1;

    y->height = maxh(
                    get_height(y->left),
                    get_height(y->right)) +
                1;

    return y;
}

Fileavlnode *leftrotate(Fileavlnode *filenode)
{
    Fileavlnode *y = filenode->right;
    Fileavlnode *n = y->left;

    y->left = filenode;
    filenode->right = n;

    filenode->height = maxh(
                           get_height(filenode->left),
                           get_height(filenode->right)) +
                       1;

    y->height = maxh(
                    get_height(y->left),
                    get_height(y->right)) +
                1;

    return y;
}

Fileavlnode *insertnode(Fileavlnode *filenode, FileEntry *entry)
{
    if (entry == NULL || entry->identity == NULL)
        return filenode;

    if (filenode == NULL)
        return createfile(entry);

    int cmp = cmpnode(
        entry->identity,
        filenode->identity);

    if (cmp < 0)
    {
        filenode->left = insertnode(filenode->left, entry);
    }
    else if (cmp > 0)
    {
        filenode->right = insertnode(filenode->right, entry);
    }
    else
    {
        return filenode;
    }

    filenode->height = 1 + maxh(
                               get_height(filenode->left),
                               get_height(filenode->right));
    int bf = getbf(filenode);

    // LL case
    if (bf > 1 &&
        cmpnode(entry->identity, filenode->left->identity) < 0)
    {
        return rightrotate(filenode);
    }

    // RR case
    if (bf < -1 &&
        cmpnode(entry->identity, filenode->right->identity) > 0)
    {
        return leftrotate(filenode);
    }

    // LR case
    if (bf > 1 &&
        cmpnode(entry->identity, filenode->left->identity) > 0)
    {
        filenode->left = leftrotate(filenode->left);
        return rightrotate(filenode);
    }

    // RL case
    if (bf < -1 &&
        cmpnode(entry->identity, filenode->right->identity) < 0)
    {
        filenode->right = rightrotate(filenode->right);
        return leftrotate(filenode);
    }
    return filenode;
}