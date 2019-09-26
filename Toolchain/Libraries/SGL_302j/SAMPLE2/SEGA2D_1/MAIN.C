#include	"sgl.h"
#include	"sega_sys.h"

#include	"sega2d.h"
#include	SEGA2D_H

#define		NBG1_CEL_ADR	(VDP2_VRAM_A0	)
#define		NBG1_MAP_ADR	(VDP2_VRAM_B0	)
#define		NBG1_COL_ADR	(VDP2_COLRAM	)

#define		BACK_COL_ADR	(VDP2_VRAM_B1   + 0x1fffe)
#define		LINE_TBL_ADR	(VDP2_VRAM_A1	)

void	ss_main(void)
{
	FIXED	posX, posY;
	Uint16	i;
	Uint32	*tb, l;

	slInitSystem(TV_320x224, NULL, 1);

	slTVOff();
	slPrint("SEGA2D LINESCROLL SAMPLE" , slLocate(7,2));
	slColRAMMode(CRM16_2048);
	slBack1ColSet((void *)BACK_COL_ADR, C_RGB(0,0,0));

	slCharNbg1(S2D_COLTYPE, S2D_CHRSIZE);
	slPageNbg1((void *)NBG1_CEL_ADR, 0, S2D_PNBSIZE);
	slPlaneNbg1(PL_SIZE_1x1);
	slMapNbg1(	(void *)NBG1_MAP_ADR,
				(void *)NBG1_MAP_ADR,
				(void *)NBG1_MAP_ADR,
				(void *)NBG1_MAP_ADR
	);

	slDMACopy((void*)S2D_CEL, (void*)NBG1_CEL_ADR, (Uint32)S2D_CELSIZE);
	slDMACopy((void*)S2D_MAP, (void*)NBG1_MAP_ADR, (Uint32)(MAP_HSIZE * MAP_VSIZE * MAP_UNIT));
	slDMACopy((void*)S2D_PAL, (void*)NBG1_COL_ADR, (Uint32)S2D_PALSIZE);

	slLineScrollModeNbg1(lineSZ1|lineHScroll);
	tb = (Uint32*)LINE_TBL_ADR;
	for(i = 0 ; i < 512 ; i++)
		*tb++ = 16 * slSin(((i & 0x00ff) << 8) * 0x00ff);
	slLineScrollTable1((void*)VDP2_VRAM_A1);


	posX = toFIXED(0.0);
	posY = toFIXED(0.0);
	slScrPosNbg1(posX, posY);

	slScrTransparent(NBG1ON);
	slScrAutoDisp(NBG0ON|NBG1ON);
	slTVOn();

	i = 0;
	while(1) {

		slScrPosNbg1(posX, posY);
		slLineScrollTable1((void*)(VDP2_VRAM_A1 + (i << 2)));

		i = (i + 1) & 0x00ff;
/*		posX += toFIXED(1.0);*/
		slSynch();
	} 
}

