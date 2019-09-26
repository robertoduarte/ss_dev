#include        "sgl.h"
#include        "ss_scrol.h"

#define	        NBG1_CEL_ADR              VDP2_VRAM_B0
#define	        NBG1_MAP_ADR            ( VDP2_VRAM_B0 + 0x10000 )
#define         NBG1_COL_ADR            ( VDP2_COLRAM  + 0x00200 )
#define	        NBG2_CEL_ADR            ( VDP2_VRAM_B1 + 0x02000 )
#define	        NBG2_MAP_ADR            ( VDP2_VRAM_B1 + 0x12000 )
#define         NBG2_COL_ADR            ( VDP2_COLRAM  + 0x00400 )
#define         BACK_COL_ADR            ( VDP2_VRAM_A1 + 0x1fffe )

void ss_main(void)
{
    Uint16  trns_flg = NBG1ON ;
    FIXED   yama_posx = SIPOSX , yama_posy = SIPOSY ;
    FIXED   am2_posx  = SIPOSX , am2_posy  = SIPOSY ;
    
    slInitSystem(TV_320x224,NULL,1);
    slTVOff();
    slPrint("Sample program 8.10.2" , slLocate(9,2)) ;

    slColRAMMode(CRM16_1024) ;
    slBack1ColSet((void *)BACK_COL_ADR , 0) ;

    slCharNbg1(COL_TYPE_256 , CHAR_SIZE_1x1);
    slPageNbg1((void *)NBG1_CEL_ADR , 0 , PNB_1WORD|CN_10BIT);
    slPlaneNbg1(PL_SIZE_1x1);
    slMapNbg1((void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR);
    Cel2VRAM(am2_cel , (void *)NBG1_CEL_ADR , 16000) ;
    Map2VRAM(am2_map , (void *)NBG1_MAP_ADR , 32 , 32 , 1 , 0) ;
    Pal2CRAM(am2_pal , (void *)NBG1_COL_ADR , 256) ;

    slCharNbg2(COL_TYPE_256 , CHAR_SIZE_1x1) ;
    slPageNbg2((void *)NBG2_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);
    slPlaneNbg2(PL_SIZE_1x1);
    slMapNbg2((void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR);
    Cel2VRAM(yama_cel , (void *)NBG2_CEL_ADR , 31808) ;
    Map2VRAM(yama_map , (void *)NBG2_MAP_ADR , 32 , 16 , 2 , 256) ;
    Pal2CRAM(yama_pal , (void *)NBG2_COL_ADR , 256) ;

    slScrPosNbg2(yama_posx , yama_posy) ;
    slScrPosNbg1(am2_posx  , am2_posy) ;
    slScrTransparent(trns_flg) ;

    slScrAutoDisp(NBG0ON | NBG1ON | NBG2ON) ;
    slTVOn();

    while(1)
    {
        if(yama_posx >= (SX + SIPOSX))
        {
            if(trns_flg == NBG1ON)
                trns_flg = 0 ;
            else
                trns_flg = NBG1ON ;
            yama_posx = SIPOSX ;
            slScrTransparent(trns_flg) ;
        }

	slScrPosNbg2(yama_posx , yama_posy) ;
	yama_posx += POSX_UP ;

	slScrPosNbg1(am2_posx , am2_posy) ;
	am2_posy += POSY_UP ;

	slSynch();
    } 
}

