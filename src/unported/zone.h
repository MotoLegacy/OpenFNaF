// (c) 2020 MotoLegacy
// This code is licensed under MIT license (see LICENSE for details)

typedef struct block_s block_t;

struct block_s {
    int                size;       // Size of the block
    int                tag;        // Block tags
    int                id;         // The block's ID
    block_t*           previous;   // Previous block in list
    block_t*           next;       // Next block in list
};

// Tags for block_t
enum {
    MT_STATIC         = 1, // Static for the whole time
    MT_CACHED,             // Freed after use
    MT_FREE,               // Can be allocated
    MT_PURGABLE,           // Not immediately important, so can be freed.
    //
    MT_TAGS
};

extern void Zone_Init();