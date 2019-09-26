/*------------------------------------------------------------------------
 *  FILE: drw_wrt.c
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
 *		Write DRAW/COLOR information for texture
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
drw_wrt()

{
	static	Sint8	string[120];
	static	Sint8	buf[100];
	Sint16	i, j;
	Sint16	x;
	Sint16	plt_max_no, flg;
	FILE	*fppo;

	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

		/* ＤＲＡＷ／ＣＯＬＯＲ情報 */
		for ( i = 1; i < g_tex_max+1; i++ ){

			/* ＤＲＡＷ */
			strcpy(string,"#define DRAW");
			strcat(string,mbuf[i].mat_nam);
			if ( g_spd_mod == 0 ) {
				strcat(string,DRAW_DEF_T);
			} else {
				strcat(string,DRAW_DEF_S);
			}
			if ( g_col_mod == 0 )
				strcat(string," | COLOR_0");
			if ( g_col_mod == 1 )
				strcat(string," | COLOR_1");
			if ( g_col_mod == 2 )
				strcat(string," | COLOR_2");
			if ( g_col_mod == 3 )
				strcat(string," | COLOR_3");
			if ( g_col_mod == 4 )
				strcat(string," | COLOR_4");
			if ( g_col_mod == 5 )
				strcat(string," | COLOR_5");
			if ( mbuf[i].type&0x0001 == 1 )
				strcat(string," | DRAW_MESH");
			if ( mbuf[i].type < 2 ){
				strcat(string," | COMPO_REP");
			}else{
				if( mbuf[i].type&0x0002 == 2 )
					strcat(string," | COMPO_HARF");
				if( mbuf[i].type&0x0004 == 4 )
					strcat(string," | COMPO_TRANS");
			}
			strcat(string,"\n");
			fputs(string,fppo);

			/* ＣＯＬＯＲ */
			strcpy(string,"#define COLOR");
			strcat(string,mbuf[i].mat_nam);
			strcat(string," ");
			sprintf(buf,"%d",mbuf[i].mode);
			strcat(string,buf);
			strcat(string,"\n");
			fputs(string,fppo);

		}
		fputs("\n",fppo);

		/* テクスチャテーブル本体 */
		fputs("static SprTexture Texture[] = {\n",fppo);
		for ( i = 0; i < g_tex_cnt; i++ ){

			/* 開始括弧 */
			fputs("    {\n",fppo);

			/* キャラクタＮｏ． */
			fprintf(fppo,"/* character no.        */ %d,\n",txt[i].char_no);

			/* カラーモード */
			strcpy(string,"/* color mode           */ ");
			if ( g_col_mod == 0 ) strcat(string,"COLOR_0");
			if ( g_col_mod == 1 ) strcat(string,"COLOR_1");
			if ( g_col_mod == 2 ) strcat(string,"COLOR_2");
			if ( g_col_mod == 3 ) strcat(string,"COLOR_3");
			if ( g_col_mod == 4 ) strcat(string,"COLOR_4");
			if ( g_col_mod == 5 ) strcat(string,"COLOR_5");
			strcat(string,",\n");
			fputs(string,fppo);

			/* カラー */
			fprintf(fppo,"/* color data           */ %d,\n",i);

			/* キャラクタ幅 */
			fprintf(fppo,"/* character width      */ %d,\n",txt[i].width);

			/* キャラクタ高 */
			fprintf(fppo,"/* character height     */ %d,\n",txt[i].height);

			/* キャラクタポインタ */
			strcpy(string,"/* character pointer    */ (Uint8 *)");

			/* 「.」までの文字列を抽出 */
			for ( j = 0; j < 80; j++ ){
				if ( txt[i].chr_nam[j] != '.' ) {
					buf[j] = txt[i].chr_nam[j];
				}else{
					buf[j] = NULL;
					break;
				}
			}
			strcat(string,buf);
			strcat(string,",\n");
			fputs(string,fppo);

			/* ルックアップテーブルポインタ */
			strcpy(string,"/* lookup table pointer */ ");
			if ( g_col_mod == 1 ){
				strcat(string,"(SprLookupTbl *)");
				strcat(string,g_col_tbl);
				strcat(string,"_");
				sprintf(buf,"%d",i);
				strcat(string,buf);
			}else{
				strcat(string,"NULL");
			}
			strcat(string,"\n");
			fputs(string,fppo);

			/* 終了括弧 */
			fputs("    },\n",fppo);

		}
		/* ストップデータ */
		fputs("    {\n",fppo);
		fputs("/*                      */ 0xffff,\n",fppo);
		/* カラーモードは直前のデータを流用 */
		strcpy(string,"/* stop data            */ ");
		if ( g_col_mod == 0 ) strcat(string,"COLOR_0");
		if ( g_col_mod == 1 ) strcat(string,"COLOR_1");
		if ( g_col_mod == 2 ) strcat(string,"COLOR_2");
		if ( g_col_mod == 3 ) strcat(string,"COLOR_3");
		if ( g_col_mod == 4 ) strcat(string,"COLOR_4");
		if ( g_col_mod == 5 ) strcat(string,"COLOR_5");
		strcat(string,",\n");
		fputs(string,fppo);
		fputs("/*                      */ 0,\n",fppo);
		fputs("/*                      */ 0,\n",fppo);
		fputs("/*                      */ 0,\n",fppo);
		fputs("/*                      */ NULL,\n",fppo);
		fputs("/*                      */ NULL\n",fppo);
		fputs("    }\n",fppo);

		/* 終了括弧 */
		fputs("};\n",fppo);
		fputs("\n",fppo);



	/* 出力ファイルクローズ */
	fclose ( fppo );

	}


	/* Data Dump Section */
	/*printf("plt_max_no = %d\n",plt_max_no);
	for ( x = 0; x < g_plt_max; x+=2 ){
		printf("plt_no = %x : color = %x\n",
			g_pltdata[x],g_pltdata[x+1]);
	}*/
}
