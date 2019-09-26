/*****************************************************************************
 *
 *	Copyright (c) 1994 SEGA
 *
 * File	  :main.c
 * Date   :1995-02-20
 * Version:0.00
 * Auther :
 *
 ****************************************************************************/
#include	"sgl.h"
#include	"sgl_cd.h"
#include	"sddrvs.dat"

/****************************************************************************/
#define	WIN_ORG_X	0		
#define	WIN_ORG_Y	0		
#define	WIN_SIZE_X	320		
#define	WIN_SIZE_Y	240	
#define	MAX_FILE	128
Sint32	dirwork[SLCD_WORK_SIZE(MAX_FILE)];

Uint8	sdmap[] = {
	0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x10, 0x01, 0x30, 0x00, 0x00, 0x00, 0x10, 0x00,
	0x11, 0x01, 0x40, 0x00, 0x00, 0x00, 0x20, 0x00,
	0x20, 0x01, 0x60, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x21, 0x01, 0x68, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x22, 0x01, 0x70, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x23, 0x01, 0x78, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x24, 0x01, 0x80, 0x00, 0x00, 0x00, 0x06, 0x00,
	0x01, 0x01, 0x86, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x30, 0x05, 0xA0, 0x00, 0x00, 0x02, 0x00, 0x00,
	0xFF, 0xFF
    };
    
/****************************************************************************/

void	ss_main(void)
{
    Sint32	ndir;
    CDHN	cdhn;
    CDBUF	buf[2];
    CDKEY	key[2];
    Sint32	stat;

    slInitSystem(TV_320x224, NULL, 1);
    slInitSound(sddrvstsk, sizeof(sddrvstsk), sdmap, sizeof(sdmap));
    slCDDAOn(127, 127, 0, 0);
    ndir = slCdInit(MAX_FILE, dirwork);

    key[0].cn = CDKEY_NONE;
    key[0].sm = CDKEY_NONE;
    key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;
    cdhn = slCdOpen("cdda1", key);

    buf[0].type = CDBUF_COPY;
    buf[0].trans.copy.addr = NULL;
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = 0;
    buf[1].type = CDBUF_TERM;
    slCdLoadFile(cdhn, buf);

    while (1) {
	slSynch();
	stat = slCdGetStatus(cdhn, NULL);
	if (stat == CDSTAT_COMPLETED)	break;
    }
    while (1)
	;
}

