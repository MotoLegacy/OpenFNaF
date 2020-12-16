#ifndef _OSL_VRAM_MGR_H_
#define _OSL_VRAM_MGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define OSL_VRAM_SIZE	(2 << 20)
#define OSL_VRAM_BASE	(0x40000000)
#define OSL_VRAM_END	(OSL_VRAM_BASE + OSL_VRAM_SIZE)

extern int osl_useVramManager;
extern u32 osl_currentVramPtr;

extern void oslVramMgrInit();
extern void *oslVramMgrAllocBlock(int blockSize);
extern int oslVramMgrFreeBlock(void *blockAddress, int blockSize);
extern int oslVramMgrSetParameters(void *adrStart, int size);

#ifdef __cplusplus
}
#endif

#endif

