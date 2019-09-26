/*****************************************************************************
 *
 *	Copyright (c) 1994 SEGA
 *
 * File	  : main.c
 * Date   : 1995-02-20
 * Version: 0.00
 * Author :
 ****************************************************************************/
#include	"sgl.h"
#include	"sega_cdc.h"

/****************************************************************************/
char	map[] = {0xff,0xff,0xff,0xff};
extern	char	sddrvs[];
extern	char	sddrvs_end[];

#define		scsp_map_size	4

/****************************************************************************/
/* Program to test that area 30000-40000 in SndRAM is not being corrupted */    
/****************************************************************************/
void	ss_main(void)
{
	int i,y=0;
	CdcPly plywk;
	CdcPly *ply = &plywk;

    slInitSystem(TV_320x224, NULL, 1);		
    slPrint("Sound Driver Corruption Test v1",slLocate(1,y++));

    slInitSound(sddrvs , (sddrvs_end-sddrvs) , (Uint8 *)map , (4));

	CDC_CdInit(0x00,0x00,0x05,0x0f) ;

	CDC_PLY_STYPE(ply) = CDC_PTYPE_TNO ;
	CDC_PLY_STNO(ply)  = 2 ;
	CDC_PLY_SIDX(ply) = 1 ;
	CDC_PLY_ETYPE(ply) = CDC_PTYPE_TNO ;
	CDC_PLY_ETNO(ply)  = 2 ;
	CDC_PLY_EIDX(ply) = 99 ;
	CDC_PLY_PMODE(ply) = CDC_PM_DFL+ 10 ;
    CDC_CdPlay(ply);

	slCDDAOn(127,127,0,0);
	slSndVolume(127);
	
	while (1) 
    {
		slSynch();     			
	}
}

