/*----------------------------------------------------------------------*/
/*	Graphic Rotation						*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"ss_scrol.h"

#define		RBG0_CEL_ADR		VDP2_VRAM_A0
#define		RBG0_MAP_ADR		VDP2_VRAM_B0
#define		RBG0_COL_ADR		( VDP2_COLRAM + 0x00200 )
#define		RBG0_PAR_ADR		( VDP2_VRAM_A1 + 0x1fe00 )
#define		BACK_COL_ADR		( VDP2_VRAM_A1 + 0x1fffe )

void ss_main(void)
{
	ANGLE yama_angz = DEGtoANG(0.0);
	FIXED posx = toFIXED(128.0) , posy = toFIXED(64.0);
	
	slInitSystem(TV_320x224,NULL,1);
	slTVOff();
	slPrint("Sample program 8.9.1" , slLocate(9,2));

	slColRAMMode(CRM16_1024);
	slBack1ColSet((void *)BACK_COL_ADR , 0);

	slRparaInitSet((void *)RBG0_PAR_ADR);
	slCharRbg0(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageRbg0((void *)RBG0_CEL_ADR , 0 , PNB_1WORD|CN_10BIT);
	slPlaneRA(PL_SIZE_1x1);
	sl1MapRA((void *)RBG0_MAP_ADR);
	slOverRA(2);
	Cel2VRAM(yama_cel , (void *)RBG0_CEL_ADR , 31808);
	Map2VRAM(yama_map , (void *)RBG0_MAP_ADR , 32 , 16 , 1 , 0);
	Pal2CRAM(yama_pal , (void *)RBG0_COL_ADR , 256);

	slDispCenterR(toFIXED(160.0) , toFIXED(112.0));
	slLookR(toFIXED(128.0) , toFIXED(64.0));

	slScrAutoDisp(NBG0ON | RBG0ON);
	slTVOn();

	while(1){
		slZrotR(yama_angz);
		yama_angz += DEGtoANG(1.0);
		slSynch();
	} 
}

