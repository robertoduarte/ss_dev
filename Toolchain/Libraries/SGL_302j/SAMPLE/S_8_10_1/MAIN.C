/*----------------------------------------------------------------------*/
/*	Ascii Scroll							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"ss_scrol.h"

#define		NBG1_CEL_ADR		VDP2_VRAM_B1
#define		NBG1_MAP_ADR		( VDP2_VRAM_B1 + 0x18000 )
#define		NBG1_COL_ADR		VDP2_COLRAM
#define		BACK_COL_ADR		( VDP2_VRAM_A1 + 0x1fffe )

void ss_main(void)
{
	FIXED ascii_posx = SIPOSX , ascii_posy = SIPOSY;

	slInitSystem(TV_320x224,NULL,1);
        slTVOff();
	slPrint("Sample program 8.10.1" , slLocate(9,2));

	slColRAMMode(CRM16_1024);
	slBack1ColSet((void *)BACK_COL_ADR , 0);

	slCharNbg1(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageNbg1((void *)NBG1_CEL_ADR , 0 , PNB_1WORD|CN_10BIT);
	slPlaneNbg1(PL_SIZE_1x1);
	slMapNbg1((void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR);
	Map2VRAM(ascii_map ,(void *)NBG1_MAP_ADR , 32 , 4 , 0 , 0);

	slScrAutoDisp(NBG0ON | NBG1ON);
	slTVOn();

	while(1) {
		slScrPosNbg1(ascii_posx , ascii_posy);
		ascii_posx += POSX_UP;
		slSynch();
	} 
}

