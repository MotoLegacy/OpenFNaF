// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

//
// zone.c - Native Zone memory allocation, based on idtech implementation.
//

#include "defs.h"
#include <stdlib.h>
#include <string.h>

// list of all allocated blocks
static block_t* allocated_blocks[MT_TAGS];

// Add a block into designated list
void Zone_InsertBlock(block_t* block)
{
    block->previous = NULL;
    block->next = allocated_blocks[block->tag];
    allocated_blocks[block->tag] = block;

    if (block->next != NULL) {
        block->next->previous = block;
    }
}

// Remove block from list
void Zone_RemoveBlock(block_t* block)
{
    // Unlink
    if (block->previous == NULL) {
        allocated_blocks[block->tag] = block->next;
    } else {
        block->previous->next = block->next;
    }

    if (block->next != NULL) {
        block->next->previous = block->previous;
    }
}

// Initialize
void Zone_Init()
{
    memset(allocated_blocks, 0, sizeof(allocated_blocks));
    Print_Normal("Zone Manager Initialized.\n");
}

// Free the specified memory block
void Zone_Free(void* ptr)
{
    // Retrieve the block
    block_t* block;
    block = (block_t *) ((int *)ptr - sizeof(block_t));

    // Ensure this is a valid block before continuing
    if (block->id == 0) {
        Print_Normal("Zone_FreeBlock: Attempted to free a block with id 0!\n");
    }

    // Remove block and free to system
    Zone_RemoveBlock(block);
    free(block);
}

// Clear cached data to make room for new block
static bool Zone_CacheClear(int size)
{
    block_t* block;
    block_t* nextblock;

    int remaining;

    block = allocated_blocks[MT_CACHED];

    if (block == NULL)
        return FALSE;

    // Search through cache blocklist to find one that is
    // likely no longer needed
    while(block->next != NULL) {
        block = block->next;
    }

    // Free backwards list until we have the size required
    remaining = size;

    while (remaining > 0) {
        // No more blocks, we tried.
        if (block == NULL)
            break;

        nextblock = block->previous;
        Zone_RemoveBlock(block);

        remaining -= block->size;
        free(block);

        block = nextblock;
    }

    return TRUE;
}

// Malloc a block with designated size and tag
void* Zone_Malloc(int size, int tag)
{
    // first ensure that the tag is valid
    if (tag < 0 || tag >= MT_TAGS || tag == MT_FREE) {
        Print_Normal("Zone_Malloc: Tried to allocate block with invalid tag (%i)\n", tag);
    }

    block_t* block = NULL;

    while(block == NULL) {
        block = (block_t *)malloc(sizeof(block_t) + size);

        if (block == NULL) {
            if (!Zone_CacheClear(sizeof(block_t) + size)) {
                Print_Normal("Zone_Malloc: Could not allocate block with %i bytes\n", size);
            }
        }
    }

    //block->id = ZONEID;
    block->tag = tag;
    block->size = size;

    Zone_InsertBlock(block);

    unsigned char* data = (unsigned char*) block;
    void* result = data + sizeof(block_t);

    return result;
}

// Free blocks of specific tags
void Zone_FreeTags(int lowtag, int hightag)
{
    for (int i = lowtag; i < hightag; i++) {
        block_t* block;
        block_t* nextblock;

        // Free all in chain
        for (block = allocated_blocks[i]; block != NULL; ) {
            nextblock = block->next;

            // Free
            free(block);

            block = nextblock;
        }

        // Empty chain now
        allocated_blocks[i] = NULL;
    }
}