#include "oslib.h"
#include "../PC_Main/GL/glext.h"
#include "../PC_Main/OpenGL.h"
#include <math.h>

u8 *emuVramTempMemoryPtr = NULL;
u32 emuCurrentTexturePixelFormat = OSL_PF_8888, emuCurrentPalettePixelFormat = OSL_PF_8888;
u32 emuCurrentClearColor = 0;
//const int emu_pixelFormats[] = {GL_RGB5, GL_RGB5_A1, GL_RGBA4, GL_RGBA8, 0, 0};
const int emu_pixelFormats[] = {GL_RGB5, GL_RGB5_A1, GL_RGBA4, GL_RGBA8, 0, 0};
const int emu_pixelPhysFormats[] = {GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_BYTE, 0, 0};
u32 emuCurrentTexSizeX, emuCurrentTexSizeY, emuCurrentTextureState=0, emuCurrentAmbientColor = 0xffffffff;
#define MAX_VRAM_TEMP_SIZE 1<<20
void *emuCurrentPalette, *emuCurrentTextureSourceData;
u32 emuCurrentTextureCache[512][512], *emuTempTextureMemory;
int emuSkipFrame = 0, emuDoubleBuffer = 1;
u8 *OSL_UVRAM_BASE = NULL;
u32 emuPaletteCache[256], emuIsAlphaWriteEnabled=0;
s64 emuCounterFrequency, emuCounterBaseValue;
//Si no texture cache support est activé
int emuCurrentTextureNo;
GLuint emuGlTexture[EMU_NB_TEXTURES];
//BlendEquation
PFNGLBLENDCOLOREXTPROC glBlendColor;
PFNGLBLENDEQUATIONPROC glBlendEquation;
GLhandleARB emuVertexShaderID, emuFragmentShaderID, emuProgramObjectID;

// The function pointers for shaders
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;
PFNGLUNIFORM1IARBPROC glUniform1iARB = NULL;
PFNGLUNIFORM1FARBPROC glUniform1fARB = NULL;
PFNGLUNIFORM2FARBPROC glUniform2fARB = NULL;
PFNGLUNIFORM3FARBPROC glUniform3fARB = NULL;
PFNGLUNIFORM4FARBPROC glUniform4fARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = NULL;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = NULL;

//Paramètres
int emuReuseTextures = 0;					//Rapide sur ATI Radeon, merde sur Intel Extreme Graphics
int emuUsePixelShaders = 0;					//Pour le color keying, ne marche pas - j'aurais besoin de pros :P
int emuFrameSkipMode = 0;					//0 = utilise les timings PC et signale au jeu qu'il rame comme sur PSP
											//1 = ralentit sur PC, ne signale jamais au jeu qu'il rame
											//2 = ne signale pas au jeu qu'il rame mais fait un frameskip du côté de l'ému (pas bien - pas encore supporté)
int emuPauseGame = 0;

int emuShadersInitGLSL()
{
	char *szGLExtensions = (char*)glGetString(GL_EXTENSIONS);

	if(!strstr(szGLExtensions, "GL_ARB_shader_objects"))
	{
		MessageBox(NULL, "GL_ARB_shader_objects extension not supported!", "Error", MB_OK);
		return 0;
	}

	if(!strstr(szGLExtensions, "GL_ARB_shading_language_100"))
	{
		MessageBox(NULL, "GL_ARB_shading_language_100 extension not supported!", "Error", MB_OK);
		return 0;
    }

	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
	glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress("glDetachObjectARB");
	glDeleteObjectARB  = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");

	return 1;
}

char *emuShadersLoadTextFile(char *strFile)
{
	FILE *f;
	char *dest = NULL;
	// Ouvre le fichier
	f = fopen(strFile, "rt");

	if (!f)		{
		strcpy(dest, "");
		return NULL;
	}

	while(!feof(f))		{
		//Par 1 ko
		dest = (char*)realloc(dest, 1<<10);
		fread(dest, 1, 1<<10, f);
	}

	fclose(f);

	return dest;
}

void emuShadersRelease()
{
	if(emuVertexShaderID)
	{
		glDetachObjectARB(emuProgramObjectID, emuVertexShaderID);
		glDeleteObjectARB(emuVertexShaderID);
		emuVertexShaderID = NULL;
	}

	if(emuFragmentShaderID)
	{
		glDetachObjectARB(emuProgramObjectID, emuFragmentShaderID);
		glDeleteObjectARB(emuFragmentShaderID);
		emuFragmentShaderID = NULL;
	}

	if(emuProgramObjectID)
	{
		glDeleteObjectARB(emuProgramObjectID);
		emuProgramObjectID = NULL;
	}
}

void emuShadersInitShaders(char *szVertexFile, char *szFragmentFile)
{
	char *szVShader, *szFShader;

	if (!szVertexFile && !szFragmentFile)
		return;

	if (emuVertexShaderID || emuFragmentShaderID || emuProgramObjectID)
		emuShadersRelease();

	if (szVertexFile)
		emuVertexShaderID = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if (szFragmentFile)
		emuFragmentShaderID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	if (szVertexFile)
		szVShader = emuShadersLoadTextFile(szVertexFile);
	if (szFragmentFile)
		szFShader = emuShadersLoadTextFile(szFragmentFile);

	if (szVertexFile)
		glShaderSourceARB(emuVertexShaderID, 1, (const GLcharARB**)&szVShader, NULL);
	if (szFragmentFile)
		glShaderSourceARB(emuFragmentShaderID, 1, (const GLcharARB**)&szFShader, NULL);

	if (szVertexFile)
		glCompileShaderARB(emuVertexShaderID);
	if (szFragmentFile)
		glCompileShaderARB(emuFragmentShaderID);

	emuProgramObjectID = glCreateProgramObjectARB();

	if (szVertexFile)
		glAttachObjectARB(emuProgramObjectID, emuVertexShaderID);
	if (szFragmentFile)
		glAttachObjectARB(emuProgramObjectID, emuFragmentShaderID);

	glLinkProgramARB(emuProgramObjectID);

	glUseProgramObjectARB(emuProgramObjectID);

	//Set the texture
	int var;
	var = glGetUniformLocationARB(emuProgramObjectID, "tex");
	glUniform1iARB(var, 0);
}

void emuInitEmulation()	{
	QueryPerformanceFrequency((LARGE_INTEGER*)&emuCounterFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&emuCounterBaseValue);
}

void emuInitGL()		{
	int i;
	for (i=0;i<EMU_NB_TEXTURES;i++)		{
		glGenTextures(1, &emuGlTexture[i]);
		glBindTexture(GL_TEXTURE_2D, emuGlTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	emuCurrentTextureNo = 0;
	emuCurrentTexSizeX=0, emuCurrentTexSizeY=0;
	emuCurrentTexturePixelFormat = -1;
	if (!emuTempTextureMemory)
		emuTempTextureMemory = (u32*)malloc(512*512*sizeof(u32));
	//The PSP doesn't write to the alpha component
	emuIsAlphaWriteEnabled = 0;
	glColorMask(TRUE, TRUE, TRUE, FALSE);
	//VRAM is 2 MB
	if (!OSL_UVRAM_BASE)
		OSL_UVRAM_BASE = (u8*)malloc(2<<20);
	glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquation");
	glBlendColor = (PFNGLBLENDCOLOREXTPROC) wglGetProcAddress("glBlendColor");
	if (emuUsePixelShaders)		{
		emuShadersInitGLSL();
		emuShadersInitShaders(NULL, "psp.frag");
	}
}

void sceKernelDcacheWritebackInvalidateRange(void *p, int n)
{
	//Ok
}

void sceKernelDcacheWritebackRange(void *p, int n)
{
	//Ok
}

void sceKernelDcacheWritebackInvalidateAll()
{
	//Ok
}

typedef struct		{
	float u, v;
	//Weights not supported
	u32 color, colored;
	//Normal not supported
	float x, y, z;
} EMU_VERTEX;

void emuSetVertexColor(unsigned int color)
{
	int col;
	//En mode non texturé, la couleur ambiante ne prend pas d'effet
	if (!emuCurrentTextureState)
		col = color;
	else	{
		//En mode texturé oui, on fait une multiplication
		col =
			(((u64)(color & 0xff000000) * (emuCurrentAmbientColor & 0xff000000)) >> 32) |
			(((u64)(color & 0xff0000) * (emuCurrentAmbientColor & 0xff0000)) >> 24) |
			(((color & 0xff00) * (emuCurrentAmbientColor & 0xff00)) >> 16) |
			(((color & 0xff) * (emuCurrentAmbientColor & 0xff)) >> 8);
	}
	glColor4ub((GLubyte)(col), (GLubyte)(col>>8), (GLubyte)(col>>16), (GLubyte)((col)>>24));
}

const void *emuGetVertex(const void *vertices, int vtype, EMU_VERTEX *v)
{
	u8 *ptr = (u8*)vertices;
	u32 value, align = 1;
	//Vertex order: [for vertices(1-8)] [texture uv] [weights (0-8)] [color] [normal] [vertex] [/for]

	/*
		GU_VERTEX_8BIT - 8-bit vertex position
		GU_VERTEX_16BIT - 16-bit vertex position
		GU_VERTEX_32BITF - 32-bit vertex position (float)
	*/
	value = vtype & GU_TEXTURE_BITS;
	if (value == GU_TEXTURE_8BIT)		{
		v->u = (float)(*(s8*)ptr);
		ptr += 1;
		v->v = (float)(*(s8*)ptr);
		ptr += 1;
	}
	else if (value == GU_TEXTURE_16BIT)		{
		v->u = (float)(*(s16*)ptr);
		ptr += 2;
		v->v = (float)(*(s16*)ptr);
		ptr += 2;
		if (align < 2)	align = 2;
	}
	else if (value == GU_TEXTURE_32BITF)		{
		v->u = *(float*)ptr;
		ptr += 4;
		v->v = *(float*)ptr;
		ptr += 4;
		if (align < 4)	align = 4;
	}
	//Weights pas supportés
	//Couleurs
	value = vtype & GU_COLOR_BITS;
	v->colored = value;
	if (value == GU_COLOR_5650)		{
		v->color = oslConvertColor(OSL_PF_8888, OSL_PF_5650, (u32)(*(u16*)ptr));
		ptr += 2;
		if (align < 2)	align = 2;
	}
	else if (value == GU_COLOR_5551)		{
		v->color = oslConvertColor(OSL_PF_8888, OSL_PF_5551, (u32)(*(u16*)ptr));
		ptr += 2;
		if (align < 2)	align = 2;
	}
	else if (value == GU_COLOR_4444)		{
		v->color = oslConvertColor(OSL_PF_8888, OSL_PF_4444, (u32)(*(u16*)ptr));
		ptr += 2;
		if (align < 2)	align = 2;
	}
	else if (value == GU_COLOR_8888)		{
		v->color = *(u32*)ptr;
		ptr += 4;
		if (align < 4)	align = 4;
	}
	else
		//Fully opaque if no color was specified
		v->color = 0xffffffff;


	//Normales pas supportées
	//Les vertex
	value = vtype & GU_VERTEX_BITS;
	if (value == GU_VERTEX_8BIT)		{
		v->x = (float)(*(s8*)ptr);
		ptr += 1;
		v->y = (float)(*(s8*)ptr);
		ptr += 1;
		v->z = (float)(*(s8*)ptr);
		ptr += 1;
	}
	else if (value == GU_VERTEX_16BIT)		{
		v->x = (float)(*(s16*)ptr);
		ptr += 2;
		v->y = (float)(*(s16*)ptr);
		ptr += 2;
		v->z = (float)(*(s16*)ptr);
		ptr += 2;
		if (align < 2)	align = 2;
	}
	else if (value == GU_VERTEX_32BITF)		{
		v->x = *(float*)ptr;
		ptr += 4;
		v->y = *(float*)ptr;
		ptr += 4;
		v->z = *(float*)ptr;
		ptr += 4;
		if (align < 4)	align = 4;
	}
	//2D -> divise u,v par la taille de la texture
	if (vtype & GU_TRANSFORM_2D)		{
		v->u /= emuCurrentTexSizeX;
		v->v /= emuCurrentTexSizeY;
	}
	//Aligné à 32 bits!
	value = (u32)ptr - (u32)vertices;
	if (value % align)
		ptr += align - (value % align);
	return ptr;
}

void sceGuDrawArray(int prim, int vtype, int count, const void* indices, const void* vertices)		{
	EMU_VERTEX v[8];
	if (emuSkipFrame)
		return;
	//A IMPLEMENTER
	if (prim == GU_SPRITES)			{
		while(count > 0)		{
			//Optimiser (plusieurs en un)
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			emuSetVertexColor(v[0].color);
			glBegin(GL_QUADS);
				glTexCoord2f(v[0].u, v[0].v); glVertex3f(v[0].x, v[0].y, 0);
				glTexCoord2f(v[1].u, v[0].v); glVertex3f(v[1].x, v[0].y, 0);
				glTexCoord2f(v[1].u, v[1].v); glVertex3f(v[1].x, v[1].y, 0);
				glTexCoord2f(v[0].u, v[1].v); glVertex3f(v[0].x, v[1].y, 0);
			glEnd();
			count -= 2;
		}
	}
	else if (prim == GU_TRIANGLE_STRIP)		{
		while(count > 0)		{
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			vertices = emuGetVertex(vertices, vtype, &v[2]);
			vertices = emuGetVertex(vertices, vtype, &v[3]);
			glBegin(GL_TRIANGLE_STRIP);
				emuSetVertexColor(v[0].color);
				glTexCoord2f(v[0].u, v[0].v); glVertex3f(v[0].x, v[0].y, v[0].z);
				emuSetVertexColor(v[1].color);
				glTexCoord2f(v[1].u, v[1].v); glVertex3f(v[1].x, v[1].y, v[1].z);
				emuSetVertexColor(v[2].color);
				glTexCoord2f(v[2].u, v[2].v); glVertex3f(v[2].x, v[2].y, v[2].z);
				emuSetVertexColor(v[3].color);
				glTexCoord2f(v[3].u, v[3].v); glVertex3f(v[3].x, v[3].y, v[3].z);
			glEnd();
			count -= 4;
		}
	}
	else if (prim == GU_LINES)		{
		glBegin(GL_LINES);
		while(count > 0)		{
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			vertices = emuGetVertex(vertices, vtype, &v[2]);
			vertices = emuGetVertex(vertices, vtype, &v[3]);
				emuSetVertexColor(v[0].color);
				glTexCoord2f(v[0].u, v[0].v); glVertex3f(v[0].x, v[0].y, v[0].z);
				emuSetVertexColor(v[1].color);
				glTexCoord2f(v[1].u, v[1].v); glVertex3f(v[1].x, v[1].y, v[1].z);
				emuSetVertexColor(v[2].color);
				glTexCoord2f(v[2].u, v[2].v); glVertex3f(v[2].x, v[2].y, v[2].z);
				emuSetVertexColor(v[3].color);
				glTexCoord2f(v[3].u, v[3].v); glVertex3f(v[3].x, v[3].y, v[3].z);
			count -= 4;
		}
		glEnd();
	}
//	glFinish();
}

void *memalign(size_t a, size_t b)
{
	return malloc(b);
}

int sceDmacMemcpy(void *dest, const void *source, unsigned int size)
{
	return (int)memcpy(dest, source, size);
}

void sceGuEnable(int state)
{
	if (state >= 0)
		glEnable(state);
	if (state == GU_TEXTURE_2D)
		emuCurrentTextureState = 1;
}

void sceGuDisable(int state)
{
	if (state >= 0)
		glDisable(state);
	if (state == GU_TEXTURE_2D)
		emuCurrentTextureState = 0;
}

void sceGuAlphaFunc(int func, int value, int mask)
{
	//MASK NOT SUPPORTED
	glAlphaFunc(func, (GLclampf)(value/255.0));
}

void sceGuAmbientColor(unsigned int color)
{
	emuCurrentAmbientColor = color;
	glColor4ub((GLubyte)(color), (GLubyte)(color>>8), (GLubyte)(color>>16), (GLubyte)((color)>>24));
}

void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix)
{
	if (glBlendEquation && glBlendColor)		{
		glBlendEquation(op);
		//Si les deux sont fixes, il y a un problème (pourrait être corrigé par pixel shader)
		if (dest == GU_FIX)
			glBlendColor(destfix/255.0f, (destfix>>8)/255.0f, (destfix>>16)/255.0f, (destfix>>24)/255.0f);
		else if (src == GU_FIX)
			glBlendColor(srcfix/255.0f, (srcfix>>8)/255.0f, (srcfix>>16)/255.0f, (srcfix>>24)/255.0f);
		glBlendFunc(src, dest);
	}
	else		{
		//Quick & dirty hack pour avoir au moins l'alpha, tinting et addition...
		if (dest == GU_FIX)		{
			if ((destfix&0x00ffffff) == 0x00ffffff)
				dest = GL_ONE;
			else
				dest = GL_ZERO;
		}
		if (src == GU_FIX)		{
			if ((srcfix&0x00ffffff) == 0x00ffffff)
				src = GL_ONE;
			else
				src = GL_ZERO;
		}

		if (op == GU_ADD)
		{
			glBlendFunc(src, dest);
		}
	}
}

void sceGuTexFunc(int tfx, int tcc)
{
	//NOT NECESSARY???
}

void sceGuColorFunc(int func, unsigned int color, unsigned int mask)
{
	//IMPOSSIBLE
}

void* sceGuGetMemory(int size)
{
	if (!emuVramTempMemoryPtr)
		emuVramTempMemoryPtr = (u8*)malloc(MAX_VRAM_TEMP_SIZE);
	return emuVramTempMemoryPtr;
}

void emuConvertPaletteTexture(int width, int height, const void* tbp)		{
	u32 *pal = (u32*)emuCurrentPalette;
	u32 *tex = (u32*)emuCurrentTextureCache;
	int x, y;

	if (emuCurrentTexturePixelFormat == GU_PSM_T4)		{
		u8 *data = (u8*)tbp;
		for (y=0;y<height;y++)		{
			for (x=0;x<width;x+=2)		{
				tex[y*width+x] = ((u32*)emuCurrentPalette)[*data & 0xf];
				tex[y*width+x+1] = ((u32*)emuCurrentPalette)[(*data & 0xf0) >> 4];
				data++;
			}
		}
	}
	else if (emuCurrentTexturePixelFormat == GU_PSM_T8)		{
		u8 *data = (u8*)tbp;
		for (y=0;y<height;y++)
			for (x=0;x<width;x++)
				tex[y*width+x] = ((u32*)emuCurrentPalette)[*data++];
	}
	else if (emuCurrentTexturePixelFormat == GU_PSM_5650)		{
		u16 *data = (u16*)tbp;
		for (y=0;y<height;y++)
			for (x=0;x<width;x++)			{
				int col = *data++;
				col =
					  (col & 0x1f) << 3
					| (((col >> 5) & 0x3f) << 2) << 8
					| (((col >> 11) & 0x1f) << 3) << 16
					| 0xff << 24;
				tex[y*width+x] = col;
			}
	}
	else if (emuCurrentTexturePixelFormat == GU_PSM_5551)		{
		u16 *data = (u16*)tbp;
		for (y=0;y<height;y++)
			for (x=0;x<width;x++)			{
				int col = *data++;
				col =
					  (col & 0x1f) << 3
					| (((col >> 5) & 0x1f) << 3) << 8
					| (((col >> 10) & 0x1f) << 3) << 16
					| (((col >> 15) & 0x1) << 7) << 24;
				tex[y*width+x] = col;
			}
	}
	else if (emuCurrentTexturePixelFormat == GU_PSM_4444)		{
		u16 *data = (u16*)tbp;
		for (y=0;y<height;y++)
			for (x=0;x<width;x++)			{
				int col = *data++;
				col =
					  (col & 0xf) << 4
					| (((col >> 4) & 0xf) << 4) << 8
					| (((col >> 8) & 0xf) << 4) << 16
					| (((col >> 12) & 0xf) << 4) << 24;
				tex[y*width+x] = col;
			}
	}
}

void sceGuTexImage(int mipmap, int width, int height, int tbw, const void* tbp)
{
	int pf, needConvert = 0;
//	glGenTextures(1, &emuGlTexture);
	if (emuSkipFrame)
		return;

	emuCurrentTextureSourceData = (void*)tbp;

	if (emuCurrentTexturePixelFormat == GU_PSM_T4 || emuCurrentTexturePixelFormat == GU_PSM_T8)
		needConvert = 1;
/*	if (
		((emuConvertPixelTypes & 1) && emuCurrentTexturePixelFormat == GU_PSM_5650) ||
		((emuConvertPixelTypes & 2) && emuCurrentTexturePixelFormat == GU_PSM_5551) ||
		((emuConvertPixelTypes & 4) && emuCurrentTexturePixelFormat == GU_PSM_4444))
		needConvert = 1;*/

redo:
	if (needConvert)		{
		emuConvertPaletteTexture(width, height, tbp);
		tbp = emuCurrentTextureCache;
		pf = GU_PSM_8888;
	}
	else
		pf = emuCurrentTexturePixelFormat;

	if (!emuReuseTextures)
		glFinish();

//	emuCurrentTexSizeX = width;
	emuCurrentTexSizeX = tbw;
	emuCurrentTexSizeY = height;

	glBindTexture(GL_TEXTURE_2D, emuGlTexture[emuCurrentTextureNo]);
//	if (width == emuCurrentTexSizeX && height == emuCurrentTexSizeY)
//		glTexSubImage2D(GL_TEXTURE_2D, mipmap, 0, 0, width, oslGetNextPower2(height), GL_RGBA, emu_pixelPhysFormats[pf], tbp);
//	else
		glTexImage2D(GL_TEXTURE_2D, mipmap, emu_pixelFormats[pf], emuCurrentTexSizeX, oslGetNextPower2(emuCurrentTexSizeY), 0, GL_RGBA, emu_pixelPhysFormats[pf], tbp);
	
	//Unable to set current mode? => convert the texture to 8888
	if (glGetError() != GL_NO_ERROR)			{
		if (pf != GU_PSM_8888)		{
			needConvert = 1;
			goto redo;
		}
	}

	emuCurrentTextureNo = (emuCurrentTextureNo + 1) % EMU_NB_TEXTURES;
//	glMatrixMode(GL_TEXTURE);
//	glScalef(32.0f, 32.0f, 0.f);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void sceGuTexMode(int tpsm, int maxmips, int a2, int swizzle)
{
	emuCurrentTexturePixelFormat = tpsm;
}

void sceGuClutLoad(int num_blocks, const void* cbp)
{
	if (emuSkipFrame)
		return;
	if (emuCurrentPalettePixelFormat == OSL_PF_8888)
		emuCurrentPalette = (void*)cbp;
	else	{
		//La palette n'est pas dans le bon format -> conversion 8888
		int i;
		emuCurrentPalette = (void*)emuPaletteCache;
		for (i=0;i<num_blocks<<3;i++)
			emuPaletteCache[i] = oslConvertColor(OSL_PF_8888, emuCurrentPalettePixelFormat, ((u32*)cbp)[i]);
	}
	//Une texture palettisée est chargée, on la met à jour
	if (emuCurrentTexturePixelFormat == GU_PSM_T4 || emuCurrentTexturePixelFormat == GU_PSM_T8)
	{
		sceGuTexImage(0, emuCurrentTexSizeX, emuCurrentTexSizeY, emuCurrentTexSizeX, emuCurrentTextureSourceData);
	}
	//A IMPLEMENTER
}
void sceGuClutMode(unsigned int cpsm, unsigned int a1, unsigned int a2, unsigned int a3)
{
	//A IMPLEMENTER
	emuCurrentPalettePixelFormat = cpsm;
}

void sceGuStart(int cid, void* list)
{
	if (!emuSkipFrame)
		emuStartDrawing();
}

int sceGuSync(int mode, int a1)
{
	if (!emuSkipFrame)
		glFinish();
	return 0;
}

int sceGuFinish(void)
{
	if (!emuSkipFrame)
		emuEndDrawing();
	return 0;
}

void sceDisplayWaitVblankStart()
{
	struct timeval timer;
	int last = 0, val;
	if (osl_curBuf == OSL_SECONDARY_BUFFER || !emuDoubleBuffer)
		glFinish();
	//Synchronise à 16.666 millisecondes
	while(1)		{
		gettimeofday(&timer,0);
		val = (timer.tv_sec * 1000000 + timer.tv_usec) % 16666;
		if (val < last)
			break;
		last = val;
	}
	//Bidouille
	if (emuFrameSkipMode == 0 && !emuPauseGame)
		osl_vblCount = (timer.tv_sec * 1000000 + timer.tv_usec) / 16666;
	else
		osl_vblCount ++;

	//Step
	if (emuPauseGame == 2)
		emuPauseGame = 1;
	//Pause
	while (emuPauseGame == 1)
	{
		Sleep(10);
	}
}

int sceGuDisplay(int state)
{
	//A IMPLEMENTER
	return 1;
}

void sceGuShadeModel(int mode)
{
	glShadeModel(mode);
}

void sceGuTexFilter(int min, int mag)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void sceGuScissor(int x, int y, int w, int h)
{
	//Les coordonnées sont inversées en openGL (0,0 = bas-gauche)
	glScissor(x, 272 - y - oslAbs(h - y), oslAbs(w - x), oslAbs(h - y));
}

void sceGuDrawBuffer(int psm, void* fbp, int fbw)
{
	//PAS SUPPORTE
}

void sceGuInit(void)
{
	//A IMPLEMENTER
}

void pspDebugScreenInit()
{
}

void* sceGuSwapBuffers(void)
{
	//A IMPLEMENTER
	if (emuDoubleBuffer)
		SwapBuffers(wglGetCurrentDC());
/*	if (!emuDoubleBuffer)		{
		glReadBuffer(GL_FRONT);
		glDrawBuffer(GL_FRONT);
	}*/
	return NULL;
}

void sceGuClearColor(unsigned int color)
{
	// make the color a deep blue hue 
	glClearColor((GLclampf)((color&0xff)/255.0), (GLclampf)(((color>>8)&0xff)/255.0), (GLclampf)(((color>>16)&0xff)/255.0), (GLclampf)255);
	emuCurrentClearColor = color;
}

void sceGuClear(int flags)
{
	glClear(flags);
}

void sceGuTerm()
{
	//PAS IMPLEMENTE
}

void sceKernelExitGame()
{
	emuExit();
}

SceUID sceIoOpen(const char *file, int flags, SceMode mode)
{
	FILE *f;
	//Crée le fichier
	if (flags & PSP_O_CREAT)
		fclose(fopen(file, "wb"));
	if ((flags&0xff) == PSP_O_RDONLY)
		f = fopen(file, "rb");
	else if ((flags&0xff) == PSP_O_WRONLY)
		f = fopen(file, "wb");
	else if ((flags&0xff) == PSP_O_RDWR)
		f = fopen(file, "a+");

	if (!f)
		f = (FILE*)-1;
	return (SceUID)f;
}

int sceIoClose(SceUID fd)
{
	return fclose((FILE*)fd);
}

int sceIoRead(SceUID fd, void *data, SceSize size)
{
	return fread(data, 1, size, (FILE*)fd);
}

int sceIoWrite(SceUID fd, const void *data, SceSize size)
{
	return fwrite(data, 1, size, (FILE*)fd);
}

int sceCtrlSetSamplingCycle(int cycle)
{
	//Ok
	return 0;
}

int sceCtrlSetSamplingMode(int mode)
{
	//Ok
	return 0;
}

int sceCtrlPeekBufferPositive(SceCtrlData *pad_data, int count)
{
	pad_data->Buttons = 0;
	pad_data->Lx = 128;
	pad_data->Ly = 128;
	if (emuKeyHeld[VK_UP])
		pad_data->Buttons |= PSP_CTRL_UP;
	if (emuKeyHeld[VK_DOWN])
		pad_data->Buttons |= PSP_CTRL_DOWN;
	if (emuKeyHeld[VK_LEFT])
		pad_data->Buttons |= PSP_CTRL_LEFT;
	if (emuKeyHeld[VK_RIGHT])
		pad_data->Buttons |= PSP_CTRL_RIGHT;
	if (emuKeyHeld[VK_NUMPAD1] || emuKeyHeld['1'])
		pad_data->Buttons |= PSP_CTRL_SQUARE;
	if (emuKeyHeld[VK_NUMPAD2] || emuKeyHeld['2'])
		pad_data->Buttons |= PSP_CTRL_CROSS;
	if (emuKeyHeld[VK_NUMPAD3] || emuKeyHeld['3'])
		pad_data->Buttons |= PSP_CTRL_CIRCLE;
	if (emuKeyHeld[VK_NUMPAD5] || emuKeyHeld['5'])
		pad_data->Buttons |= PSP_CTRL_TRIANGLE;
	if (emuKeyHeld[VK_NUMPAD4] || emuKeyHeld['4'])
		pad_data->Buttons |= PSP_CTRL_LTRIGGER;
	if (emuKeyHeld[VK_NUMPAD6] || emuKeyHeld['6'])
		pad_data->Buttons |= PSP_CTRL_RTRIGGER;
	if (emuKeyHeld[VK_NUMPAD0] || emuKeyHeld['0'])
		pad_data->Buttons |= PSP_CTRL_SELECT;
	if (emuKeyHeld[VK_RETURN])
		pad_data->Buttons |= PSP_CTRL_START;
	//Joystick
	if (emuKeyHeld['A'])
		pad_data->Lx = 0;
	if (emuKeyHeld['D'])
		pad_data->Lx = 255;
	if (emuKeyHeld['W'])
		pad_data->Ly = 0;
	if (emuKeyHeld['S'])
		pad_data->Ly = 255;
	return 1;
}

//Lit dans le bon ordre
void emuGlReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)		{
	int i, j, lineSize;
	//Synchro
//	glFinish();
	if (emuSkipFrame)
		return;
	glReadPixels(x, y, width, height, format, emu_pixelPhysFormats[type], emuTempTextureMemory);
	//Inversion (top <-> bottom)
	lineSize = (osl_pixelWidth[type] * width)>>3;
	for (i=0, j=height-1;i<height;i++, j--)			{
		memcpy((u8*)pixels + i * lineSize, (u8*)emuTempTextureMemory + j * lineSize, lineSize);
	}
}

void gettimeofday(struct timeval *tv, int unused)		{
	s64 currentValue;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentValue);
	tv->tv_sec = (currentValue - emuCounterBaseValue) / emuCounterFrequency;
	tv->tv_usec = (((currentValue - emuCounterBaseValue) * 1000000) / emuCounterFrequency) % 1000000;
}

void emuConfigure2DTransfer(int enable)		{
	static int bTextureEnabled, bBlendingEnabled;
	if (enable)		{
		bTextureEnabled = glIsEnabled(GL_TEXTURE_2D), bBlendingEnabled = glIsEnabled(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glColorMask(TRUE, TRUE, TRUE, TRUE);
	}
	else	{
		if (bTextureEnabled)
			glEnable(GL_TEXTURE_2D);
		if (bBlendingEnabled)
			glEnable(GL_BLEND);
		glColorMask(TRUE, TRUE, TRUE, emuIsAlphaWriteEnabled);
	}
}

int sceIoLseek32(SceUID  fd, int  offset, int  whence)
{
	int retval = ftell((FILE*)fd);
	fseek((FILE*)fd, offset, whence);
	return retval;
}

int sceAudioChReserve (int channel, int samplecount, int format)
{
	//Pas implémenté
	return 0;
}

int sceAudioChRelease (int channel)
{
	//Pas implémenté
	return 0;
}

int sceAudioOutputPannedBlocking (int channel, int leftvol, int rightvol, void *buffer)
{
	//Pas implémenté
	return 0;
}

int sceAudioChangeChannelConfig (int channel, int format)
{
	//Pas implémenté
	return 0;
}

SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, int initPriority, int stackSize, SceUInt attr, SceKernelThreadOptParam *option)
{
	//Pas implémenté
	return 0;
}

int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp)
{
	//Pas implémenté
	return 0;
}

int sceKernelWaitThreadEnd(SceUID thid, SceUInt *timeout)
{
	//Pas implémenté
	return 0;
}

int sceKernelDeleteThread(SceUID thid)
{
	//Pas implémenté
	return 0;
}

int sceKernelExitDeleteThread(int status)
{
	//Pas implémenté
	return 0;
}

void sceGuDispBuffer(int width, int height, void* dispbp, int dispbw)
{
	if (dispbp)
		emuDoubleBuffer = 1;
	else		{
		emuDoubleBuffer = 0;
		glReadBuffer(GL_FRONT);
		glDrawBuffer(GL_FRONT);
	}
}

void sceGuTexWrap(int u, int v)			{
	if (u == GU_CLAMP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if (v == GU_CLAMP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


