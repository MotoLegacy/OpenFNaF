#include "oslib.h"

//state: 0: normal
void oslUIDrawButtonFrame(int x0, int y0, int w, int h, int state)
{
	#define x1 (x0+w)
	#define y1 (y0+h)
	//Cadre gris
//	if (state == 1)
//		oslDrawFillRect(x0,y0,x1,y1,RGB(192,192,192));
//	else
		oslDrawGradientRect(x0,y0,x1,y1, RGB(208,208,208), RGB(188,188,188), RGB(188,188,188), RGB(168, 168, 168));
	if (state == 1)		{
		//Gris en haut à gauche
		oslDrawRect(x0,y0,x1,y1,RGB(128,128,128));
		x0++, y0++, w-=2, h-=2;
	}
	//Cadre noir en bas à droite
	oslDrawLine(x1-1,y0,x1-1,y1,RGB(0,0,0));
	oslDrawLine(x0,y1-1,x1,y1-1,RGB(0,0,0));
	//Ombre grise en bas à droite
	oslDrawLine(x1-2,y0+1,x1-2,y1-1,RGB(128,128,128));
	oslDrawLine(x0+1,y1-2,x1-2,y1-2,RGB(128,128,128));
	//Reflet blanc en haut à gauche
	oslDrawLine(x0+1,y0+1,x1-2,y0+1,RGB(255,255,255));
	oslDrawLine(x0+1,y0+1,x0+1,y1-2,RGB(255,255,255));
	#undef x1
	#undef y1
}

unsigned int oslMessageBox(const char *text, const char *title, unsigned int flags)
{
	typedef struct		{
		int key, option;
		char text[32];
	} OSL_BUTTON_INFO;
	const int x0=240-150, y0=136-60, x1=240+150, y1=136+60;
	int btX, nbButtons, i;
	OSL_BUTTON_INFO buttons[3];
	unsigned char c;
	const unsigned char keys[]={' ',' ',' ',' ',' ',' ',' ',' ','L','R',' ',' ','^','o','x','#',' ',' ',' ',' ',' ',' ',' ',' '};
	OSL_IMAGE *curBuf = oslGetDrawBuffer();
	OSL_FONT *ft = osl_curFont;
	int dither = osl_ditheringEnabled;
	
	//Pour être sûr qu'un dessin est bien en cours
	oslStartDrawing();
	//Fonte système
	if (!osl_sceFont)
		osl_sceFont = oslLoadFont(&osl_sceFontInfo);
	oslSetFont(osl_sceFont);
	//On dessine sur le buffer affiché
	oslSetDrawBuffer(OSL_SECONDARY_BUFFER);
	oslSetDithering(1);
	//Pour le cadre de la boîte: un bouton normal
	oslUIDrawButtonFrame(x0,y0,x1-x0,y1-y0,0);
	//Barre de titre
	oslDrawGradientRect(x0+3, y0+3, x1-3, y0+3+13, RGB(0,0,128), RGB(16,132,208), RGB(0,0,128), RGB(16,132,208));
	//Texte du titre (texte:blanc, fond:transparent)
	oslSetBkColor(RGBA(0,0,0,0));
	oslSetTextColor(RGB(255,255,255));
	oslSetScreenClipping(x0+3, y0+3, x1-3, y0+3+13);
	oslDrawString(x0+8, y0+6, title);
	oslResetScreenClipping();
	oslSetTextColor(RGB(0,0,0));
	oslDrawTextBox(x0+8, y0+22, x1-4, y1-30, text, 0);
	
	for (i=0;i<3;i++)		{
		//Plus rien?[i]
		if ((flags&511)==0)
			break;
		//Touche
		buttons[i].key = (flags&31);
		if (buttons[i].key >= 1 && buttons[i].key <= 24)
			c = keys[buttons[i].key-1];
		else
			c = ' ';
		buttons[i].option = (flags>>5)&15;
		switch (buttons[i].option)		{
			case OSL_MB_OK:
				sprintf(buttons[i].text, "%c Ok", c);
				break;
			case OSL_MB_CANCEL:
				sprintf(buttons[i].text, "%c Cancel", c);
				break;
			case OSL_MB_YES:
				sprintf(buttons[i].text, "%c Yes", c);
				break;
			case OSL_MB_NO:
				sprintf(buttons[i].text, "%c No", c);
				break;
			case OSL_MB_QUIT:
				sprintf(buttons[i].text, "%c Quit", c);
				break;
			default:
				strcpy(buttons[i].text, "");
		};
		flags >>= 9;
	}
	nbButtons = i;
	btX = ((x0+x1)/2)-45*nbButtons+5;

	//Boutons
	for (i=0;i<nbButtons;i++)		{
		oslUIDrawButtonFrame(btX+90*i, y1-30, 80, 21, 1);
		oslDrawString(btX+10+90*i,y1-23,buttons[i].text);
	}

	//Restore old parameters
	oslSetFont(ft);
	oslSetDithering(dither);

//	oslEndDrawing();
//	oslSwapBuffers();
	while(!osl_quit)		{
		oslReadKeys();
		for (i=0;i<nbButtons;i++)			{
			if (osl_keys->pressed.value & 1<<(buttons[i].key-1))
				break;
		}
		if (i<nbButtons)				{
			if (buttons[i].option == OSL_MB_QUIT)
				oslQuit();
			else		{
				//Restaure le buffer original
				oslSetDrawBuffer(curBuf);
				oslReadKeys();
				return buttons[i].option;
			}
		}
		if (nbButtons==0)		{
			if (osl_keys->pressed.cross)		{
				oslSetDrawBuffer(curBuf);
				break;
			}
		}
		sceDisplayWaitVblankStart();
	}
	oslReadKeys();
	return 0;
}

//Console
/*void oslFatalError(char *erreur)		{
	SceCtrlData ctl;
	pspDebugScreenInit();
	pspDebugScreenSetXY(0,0);
	pspDebugScreenSetTextColor(0xFF);
	pspDebugScreenPrintf(erreur);
	while(1)		{
		sceCtrlReadBufferPositive( &ctl, 1 );
		if(ctl.Buttons & PSP_CTRL_START)
		{
			sceKernelExitGame();
		}
		sceDisplayWaitVblankStart();
		osl_curDrawBuf = sceGuSwapBuffers();
	}
}*/

