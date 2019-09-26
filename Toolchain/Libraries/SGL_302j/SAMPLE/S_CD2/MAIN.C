/*****************************************************************************
 *
 *	Copyright (c) 1994 SEGA
 *
 * File	  :main.c
 * Date   :1995-02-20
 * Version:0.00
 * Auther :
 ****************************************************************************/
#include "sgl.h"
#include "sgl_cd.h"

/****************************************************************************/
#define MAX_OPEN	128	
#define FNAME		"S2100D0_.M"
#define slsize		2	

/****************************************************************************/
Sint32 lib_work[SLCD_WORK_SIZE(MAX_OPEN)];
Sint32	readbuf[ (CDBUF_FORM1 * slsize) / sizeof(Sint32) ];

/****************************************************************************/
void ss_main(void)
{
    Sint32	ndir ;			
    Sint32	ypos = 1 ;	
    Sint32	ret ;		
    Sint32	ndata[2] ;	
    CDHN	cdhn ;		
    CDKEY      	key[2] ;
    CDBUF	buf[2] ;

    slInitSystem(TV_320x224, NULL, 1) ;	
    ndir = slCdInit(MAX_OPEN, lib_work);
    slPrint("slCdInit:", slLocate(1,ypos));
    slPrintFX(toFIXED(ndir), slLocate(11, ypos));
    ypos++;

    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE ;
    key[1].cn = CDKEY_TERM ;	
    cdhn = slCdOpen(FNAME, key);	
    slPrint("slCdOpen:", slLocate(1, ypos));
    slDispHex((Uint32)cdhn, slLocate(11, ypos));

    buf[0].type = CDBUF_COPY ;
    buf[0].trans.copy.addr = readbuf ;
    buf[0].trans.copy.unit = CDBUF_FORM1 ;
    buf[0].trans.copy.size = slsize ;	
    buf[1].type = CDBUF_TERM ;
    ypos++;

    slCdLoadFile(cdhn, buf);	 
    while( 1 ){

	slSynch();			
	ret = slCdGetStatus( cdhn, ndata ) ;
	slPrint("stat:", slLocate(1, ypos));
	slDispHex( (Uint32)ret, slLocate(7, ypos));
	ypos++;
	if (ypos >= 27)		ypos = 3 ;
	if ( ret == CDSTAT_COMPLETED )	break;
	if ( ndata[0] == CDBUF_FORM1 * slsize ){
		slCdResetBuf(cdhn, &(key[0]));
	} 
   }
    while( 1 ) ;          
}
