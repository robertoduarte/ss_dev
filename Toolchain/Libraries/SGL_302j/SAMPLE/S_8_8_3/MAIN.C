/*----------------------------------------------------------------------*/
/*	Graphic Scroll & Expansion & Reduction				*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"ss_scrol.h"

#define		NBG1_CEL_ADR		VDP2_VRAM_B0
#define		NBG1_MAP_ADR		( VDP2_VRAM_B0 + 0x10000 )
#define		NBG1_COL_ADR		( VDP2_COLRAM + 0x00200 )
#define		BACK_COL_ADR		( VDP2_VRAM_A1 + 0x1fffe )

void ss_main(void)
{
	FIXED  yama_posx = toFIXED(0.0) , yama_posy = toFIXED(0.0);
	FIXED  yama_zoom = toFIXED(1.0);
	FIXED  up_down  = toFIXED(-0.1);

	slInitSystem(TV_320x224,NULL,1);
	slTVOff();
	slPrint("Sample program 8.8.3" , slLocate(9,2));

	slColRAMMode(CRM16_1024);
	slBack1ColSet((void *)BACK_COL_ADR , 0);

	slCharNbg1(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageNbg1((void *)NBG1_CEL_ADR , 0 , PNB_1WORD|CN_10BIT);
	slPlaneNbg1(PL_SIZE_1x1);
	slMapNbg1((void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR);
	Cel2VRAM(yama_cel , (void *)NBG1_CEL_ADR , 31808);
	Map2VRAM(yama_map , (void *)NBG1_MAP_ADR , 32 , 16 , 1 , 0);
	Pal2CRAM(yama_pal , (void *)NBG1_COL_ADR , 256);

	slZoomModeNbg1(ZOOM_HALF);

	slScrPosNbg1(yama_posx , yama_posy);
	slScrAutoDisp(NBG0ON | NBG1ON);
	slTVOn();

	while(1){
		if(yama_posx >= SX) {
			if(yama_zoom >= toFIXED(1.5))
				up_down = toFIXED(-0.1);
			else if(yama_zoom <= toFIXED(0.7))
				up_down = toFIXED(0.1);
			yama_zoom += up_down;
			yama_posx = toFIXED(0.0);
			slZoomNbg1(yama_zoom , yama_zoom);
		}
		slScrPosNbg1(yama_posx , yama_posy);
		yama_posx += POSX_UP;

		slSynch();
	} 
}

