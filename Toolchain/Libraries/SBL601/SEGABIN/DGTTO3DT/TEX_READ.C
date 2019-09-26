/*------------------------------------------------------------------------
 *  FILE: tex_read.c
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
tex_read( no )

	Sint16	no;

{
	static	Sint8	tex_fname[120];
	static	Uint8	linebuffer[1000];
	Sint16	i, ii, j, jj, k, m;
	Uint32	offset, pt_num;
	Uint16	xsize, ysize;
	Uint16	plt_no, plt_no_sp, tmp;
	Sint16	flg_0, flg_1, flg_2, flg_a;
	Sint16	plt_use[256];
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
	pt_num = (Uint32)linebuffer[0x7];
	offset = ((Uint32)linebuffer[0x103])|
		 ((Uint32)linebuffer[0x102]<<8)|
		 ((Uint32)linebuffer[0x101]<<16)|
		 ((Uint32)linebuffer[0x100]<<24);
	xsize = ((Uint16)(linebuffer[0x109]))|((Uint16)linebuffer[0x108]<<8);
	ysize = ((Uint16)(linebuffer[0x10b]))|((Uint16)linebuffer[0x10a]<<8);
#else
	pt_num = *(Uint32 *)(&linebuffer[0x4]);
	offset = *(Uint32 *)(&linebuffer[0x100]);
	xsize = *(Uint16 *)(&linebuffer[0x108]);
	ysize = *(Uint16 *)(&linebuffer[0x10a]);
#endif

	if ( g_col_mod <  2 ) g_pix_max = (xsize * ysize)/4;
	if ( g_col_mod >= 2 ) g_pix_max = (xsize * ysize)/2;
	g_plt_max = (offset - 0x0020)/2;
	if ( g_plt_max > (g_col_lim*2) ){
		printf("dgtto3dt : Error : Color palette over %d : Name = %s\n",g_col_lim,txt[no].chr_nam);
	}

	/* テクスチャ構造体にデータをセット */
	txt[no].char_no = no;
	txt[no].width = xsize;
	txt[no].height = ysize;

	/* パレットデータ読み込み */
	fread(g_pltdata,sizeof(*g_pltdata),g_plt_max,fppi);

#ifdef __BORLANDC__
	/* 上位バイトと下位バイトの入れ替え（ＰＣ用）*/
	for ( j = 0; j < g_plt_max; j++ ){
		tmp = 0;
		tmp = g_pltdata[j]&0x00ff;
		tmp = tmp<<8;
		g_pltdata[j] = g_pltdata[j]>>8;
		g_pltdata[j] = g_pltdata[j] | tmp;
	}
#endif

	/* ＲＧＢコード変換 */
	for ( j = 1; j < g_plt_max; j+=2 ){
		rgb_set(&g_pltdata[j]);
	}

	/* パレット番号0の存在の有無チェック */
	flg_0 = 0;
	for ( j = 0; j < g_plt_max; j+=2 ){
		if ( g_pltdata[j] == 0 ) flg_0 = 1;
	}

	/* パレット番号0がデータとして存在しないとき */
	if ( flg_0 == 0 ){

		/* 空きパレットフラグテーブル初期化 */
		for ( k = 0; k < g_col_lim; k++ ){
			plt_use[k] = 0;
		}

		/* ピクセルデータ読み込み（空きパレット検索用）*/
		for ( i = 0; i < ysize; i++ ){
			k = fread(&linebuffer[0], sizeof(*linebuffer), xsize*2, fppi);
			flg_a = 0;
			for ( j = 0; j < xsize*2; j+=2 ){
#ifdef __BORLANDC__
				tmp = (Uint16)(linebuffer[j]<<8) |
				      (Uint16)linebuffer[j+1];
#else
				tmp = *(Uint16 *)(&linebuffer[j]);
#endif
				if ( tmp == 0 ){
					flg_a = 1;
				}else{
					for ( k = 0; k < g_plt_max; k++ ){
						if ( tmp == g_pltdata[k] ){
							plt_no = k/2;
							plt_use[plt_no] = 1;
							break;
						}
					}
				}
			}
		}

		/* ピクセルデータ中に0x0が存在するとき */
		if ( flg_a == 1 ){

			/* 空きパレット検索（透明ピクセル用）*/
			flg_1 = 0;
			for ( k = 0; k < g_col_lim; k++ ){
				if ( plt_use[k] == 0 ){
					plt_no_sp = k;
					g_pltdata[plt_no_sp*2+1] = 0x8000;
					flg_1 = 1;
					break;
				}
			}
			if ( flg_1 == 0 ){
				printf("dgtto3dt : Error : Can not assign palette No. for undefined pixel. : Name = %s\n",txt[no].chr_nam);
				plt_no_sp = 0;
			}

		}

		/* ファイルの頭出し */
		k = fseek(fppi, 0x120+g_plt_max*2, 0);
	}

	if ( g_plt_max > (g_col_lim*2)  ) g_plt_max = g_col_lim*2;

	/* ピクセルデータ読み込み */
	ii = 0;
	jj = 0;
	for ( i = 0; i < ysize; i++ ){
		k = fread(&linebuffer[0], sizeof(*linebuffer), xsize*2, fppi);
		for( j = 0; j < xsize*2; j+=2 ){
#ifdef __BORLANDC__
			tmp =(Uint16)(linebuffer[j]<<8)|(Uint16)linebuffer[j+1];
#else
			tmp = *(Uint16 *)(&linebuffer[j]);
#endif
			if ( flg_0 == 0 && tmp == 0 ){
				plt_no = plt_no_sp;
			}else{
				flg_2 = 0;
				for ( k = 0; k < g_plt_max; k+=2 ){
					if ( tmp == g_pltdata[k] ){
						plt_no = k/2;
						plt_use[plt_no] = 1;
						flg_2 = 1;
						break;
					}
				}
				if ( flg_2 == 0 ){
					printf("dgtto3dt : Error : Undefined palette no. used. : Name = %s\n",txt[no].chr_nam);
					plt_no = plt_no_sp;
				}
			}
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
				} else if ( jj == 1 ){
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
	/*printf("pt_num = %d : xsize = %d : ysize = %d\n",pt_num,xsize,ysize);
	printf("g_plt_max = %d : g_pix_max = %d\n",g_plt_max,g_pix_max);
	for ( x = 0; x < g_col_lim; x+=8 ){
		printf(" %4d : %4x %4x %4x %4x %4x %4x %4x %4x\n",
			x,plt_use[x],plt_use[x+1],plt_use[x+2],
			plt_use[x+3],plt_use[x+4],plt_use[x+5],
			plt_use[x+6],plt_use[x+7]);
	}
	printf("\n");
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


void
rgb_set(rgb)

#ifdef __BORLANDC__
	Uint16 huge *rgb;
#else
	Uint16 *rgb;
#endif
{
	Uint16 r, r0, g, g0, b, b0, rgb0;

	r = 0;
	r = ( 0x1000 & *rgb )>>12;
	r0 = 0;
	r0 = ( 0x000f & *rgb )<<1;
	r = r|r0;

	g = 0;
	g = ( 0x2000 & *rgb )>>8;
	g0 = 0;
	g0 = ( 0x00f0 & *rgb )<<2;
	g = g|g0;

	/*g = 0;
	g = ( 0x2000 & *rgb )>>13;
	g0 = 0;
	g0 = ( 0x00f0 & *rgb )>>3;
	g = g|g0;*/

	b = 0;
	b = ( 0x4000 & *rgb )>>4;
	b0 = 0;
	b0 = ( 0x0f00 & *rgb )<<3;
	b = b|b0;

	/*b = 0;
	b = ( 0x2000 & *rgb )>>14;
	b0 = 0;
	b0 = ( 0x0f00 & *rgb )>>7;
	b = b|b0;*/

	rgb0 = r|g|b;
	*rgb = rgb0;

	/* Data Dump Section */
	/*printf(" rgb = %x\n",rgb0);*/
	/*printf(" r = %d : g = %d : b = %d\n",r,g,b);*/
}
