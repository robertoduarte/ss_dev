/*------------------------------------------------------------------------
 *  FILE: tex_red3.c
 *      $Author: $
 *      $Date: $
 *      $Locker: $
 *      $Revision: $
 *      $Source: $
 *      $State: $
 *
 *      Copyright (c) by SEGA Enterprises Ltd. 1994. All rights reserved.
 *
 *  PURPOSE:
 *		DGT or RGB to 3D Table conversion program
 *
 *  DESCRIPTION:
 *		Read texture information from RGB file
 *
 *  INTERFACE
 *
 *
 *  CAVEATS
 *
 *
 *  AUTHOR(S):
 *		K. Hasuike	(ssdg)
 *
 *  MOD HISTORY:
 *      $Log: $
 * 
 * 
 *------------------------------------------------------------------------
 */

#include "dgtto3dt.h"

void
tex_red3( no )

	Sint16	no;

{
#ifdef __BORLANDC__
	static	Uint16	huge pltdata[512];
#else
	static	Uint16	pltdata[512];
#endif
	static	Sint8	tex_fname[120];
	static	Uint8	linebuffer[1000];
	Sint16	i, ii, j, jj, k, m;
	Uint32	offset;
	Uint16	xsize, ysize;
	Uint16	plt_max, plt_no, tmp;
	Sint16	x, x1;
	FILE	*fppi;

	/* ディレクトリ名と入力ファイル名の連結 */
	strcpy(tex_fname,g_dirname);
	strcat(tex_fname,"/");
	strcat(tex_fname,txt[no].chr_nam);
	/*printf("tex_fname = %s\n",tex_fname);*/

	/* 入力ファイルオープン */
	fppi = fopen ( tex_fname, "rb" );
	if ( fppi == NULL ){
		printf("dgtto3dt : Error : Input file not exist : Name = %s\n",tex_fname);
		exit(0);
	}else{

	/* ファイルヘッダ読み込み */
	k = fread(&linebuffer[0], sizeof(*linebuffer), 0x100, fppi);

#ifdef __BORLANDC__
	xsize = ((Uint16)(linebuffer[0x19]))|((Uint16)linebuffer[0x18]<<8);
	ysize = ((Uint16)(linebuffer[0x1b]))|((Uint16)linebuffer[0x1a]<<8);
#else
	xsize = *(Uint16 *)(&linebuffer[0x18]);
	ysize = *(Uint16 *)(&linebuffer[0x1a]);
#endif
printf("xsize = %d : ysize = %d\n",xsize,ysize);

	g_pix_max = xsize * ysize;

	/* テクスチャ構造体にデータをセット */
	txt[no].char_no = no;
	txt[no].width = xsize;
	txt[no].height = ysize;

	/* ピクセルデータ読み込み */
	ii = 0;
	g_texdata[ii] = 0;
	jj = 0;
	for(i = 0; i < ysize; i++){
		k = fread(&linebuffer[0], sizeof(*linebuffer), xsize*3, fppi);
		for(j = 0; j < xsize*3; j++){
			plt_no = linebuffer[j]&0xff;
			if ( jj == 0 ){
				g_texdata[ii] = (plt_no>>3)&0x001f;
			}else if ( jj == 1 ){
				g_texdata[ii] = g_texdata[ii] |
						((plt_no<<2)&0x03e0);
			}else if ( jj == 2 ){
				g_texdata[ii] = g_texdata[ii] |
						((plt_no<<7)&0x7c00) | 0x8000;
				ii++;
				g_texdata[ii] = 0;
			}
			jj++;
			if ( jj > 2 ) jj = 0;
		}
	}

	/* 入力ファイルクローズ */
	fclose ( fppi );

	}


	/* Data Dump Section */
	/*printf("g_pix_max = %d : xsize = %d : ysize = %d\n",
		g_pix_max,xsize,ysize);
	for ( x = 0; x < g_pix_max; x+=8 ){
		printf(" %4d : %4x %4x %4x %4x %4x %4x %4x %4x\n",
			x,g_texdata[x],g_texdata[x+1],g_texdata[x+2],
			g_texdata[x+3],g_texdata[x+4],g_texdata[x+5],
			g_texdata[x+6],g_texdata[x+7]);
	}*/

}
