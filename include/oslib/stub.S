	.set noreorder

#include "pspstub.s"

	STUB_START "sceDmac",0x40010000,0x00020005 
	STUB_FUNC 0x617f3fe6,sceDmacMemcpy 
	STUB_FUNC 0xd97f94d8,sceDmacTryMemcpy 
	STUB_END 
