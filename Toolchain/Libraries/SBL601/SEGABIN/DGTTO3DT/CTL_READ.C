/*------------------------------------------------------------------------
 *  FILE: ctl_read.c
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
 *		DGT or RGB to 3DTable conversion program
 *
 *  DESCRIPTION:
 *		Read option parameter from .ctl file
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
ctl_read()

{
	static	Sint8	buf[80];
	static	Sint8	ctlname[80];
	static	Sint8	keyword[120];
	Sint16	c, i, j, k, k1;
	Sint16	x;
	Sint16	nest, skip;
	Sint16	pos;
	FILE	*fppi;
	FILE	*fppo;

	/* デフォルトセット */
	strcpy(g_col_tbl,"colorData");	/* カラールックアップテーブル名 */
	strcpy(g_in_ext,"3d");		/* 入力ファイルの拡張子 */
	g_col_mod = 1;			/* カラーモード */
	g_spd_mod = 0;			/* 透明無効にしない */
	g_plt_mod = 1;		/* パレット分離(=1)/合体(=2)/なし(=3)モード */
	strcpy(g_txc_nam,"o");		/* 出力ファイル名 */
	/* 解釈時に1文字目は読みとばすので、noが指定されるとoと解釈されるため */

	/* ディレクトリ名とオプションファイル名の連結 */
	strcpy(ctlname,g_dirname);
	strcat(ctlname,"/");
	strcat(ctlname,g_outname);
	strcat(ctlname,".ctl");

	/* オプションファイルオープン */
	fppi = fopen ( ctlname, "rb" );
	if ( fppi == NULL ){
		/*return;*/
	}else{

	/* 文字列読み込み */
	i = 0;
	skip = 0;
	nest = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/
		if ( i > 0 ){
			/* 各パラメータの検出 */
			/*printf("      keyword(1) = %s/\n",keyword);*/
			for ( j = 0; j < 80; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"col_tbl") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_col_tbl,buf);

				}else if ( strcmp(buf,"input") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_in_ext,buf);

				}else if ( strcmp(buf,"col_mode") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if (strcmp(buf,"0") == 0) g_col_mod = 0;
					if (strcmp(buf,"1") == 0) g_col_mod = 1;
					if (strcmp(buf,"2") == 0) g_col_mod = 2;
					if (strcmp(buf,"3") == 0) g_col_mod = 3;
					if (strcmp(buf,"4") == 0) g_col_mod = 4;
					if (strcmp(buf,"5") == 0) g_col_mod = 5;

				}else if ( strcmp(buf,"spd_mode") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"on") == 0 )
						g_spd_mod = 1;
					if ( strcmp(buf,"off") == 0 )
						g_spd_mod = 0;

				}else if ( strcmp(buf,"palette_mode") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"sep") == 0 )
						g_plt_mod = 1;
					if ( strcmp(buf,"comp") == 0 )
						g_plt_mod = 2;

				}else if ( strcmp(buf,"dgt_txc") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					for ( k = 1; k < 80; k++ ){
					    if ( buf[k] != '"' ){
						buf[k-1] = buf[k];
					    }else{
						buf[k-1] = NULL;
						break;
					    }
					}
					strcpy(g_txc_nam,buf);
				}

			}

		}
		skip = 0;
		i = 0;
	}else if ( skip == 0 && ( c == ' ' || c == TAB ) ){
	}else if ( c == ';' ){
		skip = 1;
	}else if ( skip == 0 && c == '{' ){
		nest++;
	}else if ( skip == 0 && c == '}' ){
		nest--;
	}else if ( skip == 0 ){
		keyword[i] = c;
		i++;
	}

	} /* while loop end */

	/* 入力ファイルクローズ */
	fclose ( fppi );
	}

	/* カラーモード１のときは強制的にパレット分離モードにする */
	if ( g_col_mod == 1 ) g_plt_mod = 1;

	/* カラーモード５のときは強制的にパレットなしモードにする */
	if ( g_col_mod == 5 ) g_plt_mod = 3;

	/* カラーモードに対応する色数の上限値の設定 */
	if ( g_col_mod <  2 ) g_col_lim = 16;
	if ( g_col_mod == 2 ) g_col_lim = 64;
	if ( g_col_mod == 3 ) g_col_lim = 128;
	if ( g_col_mod == 4 ) g_col_lim = 256;


	/* 出力ファイル名の指定がないとき、終了 */
	if ( strcmp(g_txc_nam,"o") == 0 ){

		printf("dgtto3dt : Error : Output file name is not directed\n");
		exit(0);

	}else{

		/* ディレクトリ名と出力ファイル名の連結 */
		strcpy(g_c_outname,g_dirname);
		strcat(g_c_outname,"/");
		strcat(g_c_outname,g_txc_nam);
		/*printf("g_c_outname = %s\n",g_c_outname);*/

		/* 出力ファイルオープン（新規モード） */
		fppo = fopen ( g_c_outname, "w" );
		if ( fppo != NULL ){

			/* ヘッダー書き出し */
			fputs("/* Texture bitmap File */\n",fppo);
			fputs("\n",fppo);

			/* 出力ファイルクローズ */
			fclose ( fppo );
		}

	}

	/* Data Dump Section */
	/*printf(" g_scale    = %s\n",g_col_tbl);
	printf(" g_in_ext   = %s\n",g_in_ext);
	printf(" g_col_mod  = %d\n",g_col_mod);
	printf(" g_txc_nam  = %s\n",g_txc_nam);*/

}
