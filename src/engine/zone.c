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