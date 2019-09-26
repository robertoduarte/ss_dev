/*****************************************************************************
 *
 *	Copyright (c) 1994 SEGA
 *
 * File	  :main.c
 * Date   :1995-02-20
 * Version:0.00
 * Auther :
 ****************************************************************************/
#include	"sgl.h"
#include	"sgl_cd.h"

/****************************************************************************/
#define	MAX_FILE	128
#define READSECT         50
/****************************************************************************/
Sint32	dirwork[SLCD_WORK_SIZE(MAX_FILE)];
Sint32	readbuf[ READSECT * CDBUF_FORM1 / sizeof(Sint32)];
    
/****************************************************************************/
void	ss_main(void)
{
    Sint32	ndir;
    CDHN	cdhn;
    CDKEY	key[2];
    CDBUF	buf[2];
    Sint32	stat;
    Sint32	len[2];
    Sint32	ypos = 1;

    slInitSystem(TV_320x224, NULL, 1);		
    ndir = slCdInit(MAX_FILE, dirwork);	
    slPrint("slCdInit:", slLocate(1,ypos));
    slPrintFX(toFIXED(ndir), slLocate(11, ypos));
    ypos++;

    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;	
    cdhn = slCdOpen("S2100D0_.M", key);	
    slPrint("slCdOpen:", slLocate(1, ypos));
    slDispHex((Uint32)cdhn, slLocate(11, ypos));

    buf[0].type = CDBUF_COPY;	
    buf[0].trans.copy.addr = readbuf;	
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = READSECT;	
    buf[1].type = CDBUF_TERM;
    slCdLoadFile(cdhn, buf);		
    ypos++;

    while (1) {
	slSynch();     			
	stat = slCdGetStatus(cdhn, len);
	slPrint("stat:", slLocate(1, ypos));
	slDispHex((Uint32)stat, slLocate(7, ypos));
	ypos++;
	if (ypos >= 27)		ypos = 1;
	if (stat == CDSTAT_COMPLETED)	break;
    }
    while (1) ;
}

