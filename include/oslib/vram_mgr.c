#include "oslib.h"

//We're beginning at the VRAM base
u32 osl_vramBase = 0x40000000;
//2 MBytes
int osl_vramSize = 2 << 20;
//Use it or not?
int osl_useVramManager = 1;
u32 osl_currentVramPtr;

#define DEFAULT_TABLE_SIZE 1024

int osl_vramBlocksMax = 0, osl_vramBlocksNb = 0;

typedef struct		{
	u32 offset, size;
} OSL_VRAMBLOCK;

#define isBlockFree(i)				(osl_vramBlocks[i].size & 0x80000000)
#define getBlockSize(i)				(osl_vramBlocks[i].size & 0x7fffffff)
#define getBlockOffset(i)			(osl_vramBlocks[i].offset)

#define setBlockFree(i, free)		(osl_vramBlocks[i].size = (osl_vramBlocks[i].size & ((free)? 0xffffffff : 0x7fffffff)) | ((free)? 0x80000000 : 0))
#define setBlockSize(i, nsize)		(osl_vramBlocks[i].size = (osl_vramBlocks[i].size & ~0x7fffffff) | (nsize))
#define setBlockOffset(i, noffset)	(osl_vramBlocks[i].offset = noffset)

OSL_VRAMBLOCK *osl_vramBlocks;

void oslVramMgrInit()		{
	//If we don't use it OR it has already been initialized
	if (!osl_useVramManager || osl_vramBlocksMax > 0)
		return;

	osl_vramBlocksMax = DEFAULT_TABLE_SIZE;
	osl_vramBlocksNb = 1;
	osl_vramBlocks = (OSL_VRAMBLOCK*)malloc(osl_vramBlocksMax * sizeof(OSL_VRAMBLOCK));
	if (!osl_vramBlocks){
		osl_useVramManager = 0;
		osl_vramBlocksMax = 0;
		osl_vramBlocksNb = 0;
		return;
	}

	//Premier bloc: libre, taille totale de la VRAM, adresse 0
	setBlockOffset(0, 0);
	//La taille en blocs doit être divisée par 16 puisqu'on n'utilise pas des octets sinon il serait impossible de coder toute la VRAM sur 16 bits
	setBlockSize(0, osl_vramSize);
	setBlockFree(0, 1);
}

void *oslVramMgrAllocBlock(int blockSize)		{
	int i;

	osl_skip = osl_vramBlocks[0].size;
	//Le bloc ne peut pas être de taille nulle ou négative
	if (blockSize <= 0)
		return NULL;

	//La taille est toujours multiple de 16 - arrondir au bloc supérieur
	if (blockSize & 15)
		blockSize += 16;

	//Sans le manager, c'est plus simple...
	if (!osl_useVramManager)		{
		int ptr = osl_currentVramPtr;
		//Dépassement de la mémoire?
		if (osl_currentVramPtr + blockSize >= osl_vramBase + osl_vramSize)
			return NULL;
		osl_currentVramPtr += blockSize;
		return (void*)ptr;
	}

	for (i=0;i<osl_vramBlocksNb;i++)		{
		//Ce bloc est-il suffisant?
		if (isBlockFree(i) && getBlockSize(i) >= blockSize)
			break;
	}

	//Aucun bloc libre
	if (i >= osl_vramBlocksNb)
		return NULL;

	//Pile la mémoire qu'il faut? - pas géré, il faut toujours que le dernier bloc soit marqué comme libre (même s'il reste 0 octet) pour ulSetTexVramParameters
	if (getBlockSize(i) == blockSize && i != osl_vramBlocksNb - 1)			{
		//Il n'est plus libre
		setBlockFree(i, 0);
	}
	else		{
		//On va ajouter un nouveau bloc
		osl_vramBlocksNb++;

		//Plus de mémoire pour le tableau? On l'aggrandit
		if (osl_vramBlocksNb >= osl_vramBlocksMax)			{
			OSL_VRAMBLOCK *oldBlock = osl_vramBlocks;
			osl_vramBlocksMax += DEFAULT_TABLE_SIZE;
			osl_vramBlocks = (OSL_VRAMBLOCK*)realloc(osl_vramBlocks, osl_vramBlocksMax);

			//Vérification que la mémoire a bien pu être allouée
			if (!osl_vramBlocks)		{
				osl_vramBlocks = oldBlock;
				osl_vramBlocksMax -= DEFAULT_TABLE_SIZE;
				//Pas assez de mémoire
				return NULL;
			}
		}

		//Décalage pour insérer notre nouvel élément
		memmove(osl_vramBlocks + i + 1, osl_vramBlocks + i, sizeof(OSL_VRAMBLOCK) * (osl_vramBlocksNb - i - 1));

		//Remplissons notre nouveau bloc
		setBlockSize(i, blockSize);
		//Il a l'adresse du bloc qui était là avant
		setBlockOffset(i, getBlockOffset(i + 1));
		//Il n'est pas libre
		setBlockFree(i, 0);

		//Pour le prochain, sa taille diminue
		setBlockSize(i + 1, getBlockSize(i + 1) - blockSize);
		//ATTENTION: calcul d'offset
		setBlockOffset(i + 1, getBlockOffset(i + 1) + blockSize);
	}

	//Note: il faut traduire l'offset en vraie adresse
	return (void*)(getBlockOffset(i) + osl_vramBase);
}

//Note: il faut traduire une vraie adresse en offset
int oslVramMgrFreeBlock(void *blockAddress, int blockSize)		{
	int i, j, updateNeeded;
	int blockOffset = (u32)blockAddress - (u32)osl_vramBase;

	//Sans le manager, c'est plus simple...
	if (!osl_useVramManager)		{
		osl_currentVramPtr -= blockSize;
		//Pas vraiment utile, juste là pour s'assurer qu'on ne dépassera jamais de l'espace alloué
		if (osl_currentVramPtr < osl_vramBase)
			osl_currentVramPtr = osl_vramBase;
		return 1;
	}

	//Trouvons le bloc qui va bien
	for (i=0;i<osl_vramBlocksNb;i++)		{
		if (getBlockOffset(i) == blockOffset)
			break;
	}

	//Impossible de trouver le bloc
	if (i >= osl_vramBlocksNb)
		return 0;

	//Le bloc est maintenant libre ^^
	setBlockFree(i, 1);

	//Bon maintenant reste à "assembler" les blocs libres adjacents
	do		{
		updateNeeded = 0;
		for (j=0;j<osl_vramBlocksNb-1;j++)			{
			//Cherchons deux blocs adjacents
			if ((isBlockFree(j) && isBlockFree(j + 1))
				|| (isBlockFree(j) && getBlockSize(j) == 0))			{
				//Assemblons ces blocs maintenant
				int newSize = getBlockSize(j) + getBlockSize(j + 1), newAdd = getBlockOffset(j);
				memmove(osl_vramBlocks + j, osl_vramBlocks + j + 1, sizeof(OSL_VRAMBLOCK) * (osl_vramBlocksNb - j - 1));
				setBlockOffset(j, newAdd);
				setBlockSize(j, newSize);
				//Le bloc entre deux est supprimé
				osl_vramBlocksNb--;
				//ATT: On devra refaire un tour pour vérifier si de nouveaux blocs n'ont pas été créés
				updateNeeded = 1;
			}
		}

	} while (updateNeeded);

	return 1;
}

int oslVramMgrSetParameters(void *baseAddr, int size)		{
   int curVramSize = osl_vramSize;
   int blockNum = osl_vramBlocksNb - 1;
   int sizeDiff;

	if (!osl_useVramManager)
		return 0;
	//La taille est toujours multiple de 16 - arrondir au bloc supérieur
	if (size & 15)
		size += 16;

	//Différence de taille (négatif pour réduction, positif pour aggrandissement)
   sizeDiff = size - curVramSize;

	//Le dernier bloc est TOUJOURS libre, même s'il reste 0 octet. Cf la bidouille dans ulTexVramAlloc
	if (isBlockFree(blockNum) && getBlockSize(blockNum) + sizeDiff >= 0)			{
		setBlockSize(blockNum, getBlockSize(blockNum) + sizeDiff);
		osl_vramBase = (u32)baseAddr;
		osl_vramSize = size;
		//Pour ceux qui ne veulent pas utiliser le gestionnaire...
		osl_currentVramPtr = osl_vramBase;
	}
	else
		return 0;
	return 1;
}

