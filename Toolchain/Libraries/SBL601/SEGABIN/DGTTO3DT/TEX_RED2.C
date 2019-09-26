/*------------------------------------------------------------------------
 *  FILE: tex_red2.c
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
 *		Read texture information from DGT file
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
tex_red2( no )

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
	k = fread(&linebuffer[0], sizeof(*linebuffer), 0x120, fppi);

#ifdef __BORLANDC__
	offset = ((Uint32)linebuffer[0x103])|
		 ((Uint32)linebuffer[0x102]<<8)|
		 ((Uint32)linebuffer[0x101]<<16)|
		 ((Uint32)linebuffer[0x100]<<24);
	xsize = ((Uint16)(linebuffer[0x109]))|((Uint16)linebuffer[0x108]<<8);
	ysize = ((Uint16)(linebuffer[0x10b]))|((Uint16)linebuffer[0x10a]<<8);
#else
	offset = *(Uint32 *)(&linebuffer[0x100]);
	xsize = *(Uint16 *)(&linebuffer[0x108]);
	ysize = *(Uint16 *)(&linebuffer[0x10a]);
#endif

	if ( g_col_mod <  2 ) g_pix_max = (xsize * ysize)/4;
	if ( g_col_mod >= 2 ) g_pix_max = (xsize * ysize)/2;
	plt_max = (offset - 0x0020)/2;

	/* テクスチャ構造体にデータをセット */
	txt[no].char_no = no;
	txt[no].width = xsize;
	txt[no].height = ysize;

	/* パレットデータ読み込み */
	fread(pltdata,sizeof(*pltdata),plt_max,fppi);

#ifdef __BORLANDC__
	/* 上位バイトと下位バイトの入れ替え（ＰＣ用）*/
	for ( j = 0; j < plt_max; j++ ){
		tmp = 0;
		tmp = pltdata[j]&0x00ff;
		tmp = tmp<<8;
		pltdata[j] = pltdata[j]>>8;
		pltdata[j] = pltdata[j]|tmp;
	}
#endif

	/* ＲＧＢコード変換 */
	for ( j = 1; j < plt_max; j+=2 ){
		rgb_set(&pltdata[j]);
	}

	/* 保存カラーパレットテーブルに追加 */
	for ( m = 0; m < plt_max; m++ ){
		g_pltdata[g_plt_max+m]=pltdata[m];
	}
	g_plt_max = g_plt_max + plt_max;

	/* ピクセルデータ読み込み */
	ii = 0;
	jj = 0;
	for(i = 0; i < ysize; i++){
		k = fread(&linebuffer[0], sizeof(*linebuffer), xsize*2, fppi);
		for(j = 0; j < xsize*2; j+=2){
#ifdef __BORLANDC__
			plt_no =(Uint16)(linebuffer[j]<<8) |
				(Uint16)linebuffer[j+1];
#else
			plt_no = *(Uint16 *)(&linebuffer[j]);
#endif
			if ( g_col_mod < 2 ){
				if ( jj == 0 ){
					g_texdata[ii] = plt_no<<12;
				} else if ( jj == 1 ){
					g_texdata[ii] =
						g_texdata[ii] | (plt_no<<8);
				} else if ( jj == 2 ){
					g_texdata[ii] =
						g_texdata[ii] | (plt_no<<4);
				} else if ( jj == 3 ){
					g_texdata[ii] = g_texdata[ii] | plt_no;
					ii++;
				}
				jj++;
				if ( jj > 3 ) jj = 0;
			}else{
				if ( jj == 0 ){
					g_texdata[ii] = plt_no<<8;
				}else if ( jj == 1 ){
					g_texdata[ii] = g_texdata[ii] | plt_no;
					ii++;
				}
				jj++;
				if ( jj > 1 ) jj = 0;
			}
		}
	}

	/* 入力ファイルクローズ */
	fclose ( fppi );

	}


	/* Data Dump Section */
	/*printf("g_plt_max = %d : xsize = %d : ysize = %d\n",
		g_plt_max,xsize,ysize);
	printf("plt_max = %d : g_pix_max = %d\n",plt_max,g_pix_max);
	for ( x = 0; x < g_plt_max; x+=8 ){
		printf(" %4d : %4x %4x %4x %4x %4x %4x %4x %4x\n",
			x,g_pltdata[x],g_pltdata[x+1],g_pltdata[x+2],
			g_pltdata[x+3],g_pltdata[x+4],g_pltdata[x+5],
			g_pltdata[x+6],g_pltdata[x+7]);
	}
	printf("\n");
	for ( x = 0; x < g_pix_max; x+=8 ){
		printf(" %4d : %4x %4x %4x %4x %4x %4x %4x %4x\n",
			x,g_texdata[x],g_texdata[x+1],g_texdata[x+2],
			g_texdata[x+3],g_texdata[x+4],g_texdata[x+5],
			g_texdata[x+6],g_texdata[x+7]);
	}*/

}


