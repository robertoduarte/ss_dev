/*------------------------------------------------------------------------
 *  FILE: tex_wrt.c
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
 *		SEGA3D to 3DTable conversion program
 *
 *  DESCRIPTION:
 *		Write DRAW/COLOR/TEXTURE information for texture
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

#include "s3dto3dt.h"

void
tex_wrt()

{
	static	Sint8	string[120];
	static	Sint8	buf[80];
	Sint16	i, j, flg;
	Sint16	tex_cnt;
	Uint16	color_r, color_g, color_b;
	FILE	*fppo;

/*printf("tex_wrt started ***\n\n");*/
	/* マテリアル情報からテクスチャ情報作成（重複の削除含む）*/
	tex_cnt = g_t_max;
	for ( i = 1; i < g_tex_max+1; i++ ){
		flg = 0;
		for ( j = 0; j < tex_cnt; j++ ){
			if ( strcmp(mbuf[i].tex_nam,txt[j].chr_nam) == 0 ){
				mbuf[i].mode = j;	/* 領域を一時的に流用 */
				flg = 1;
			}
		}
		if ( flg == 0 ){
			mbuf[i].mode = tex_cnt;	/* 領域を一時的に流用 */
			strcpy(txt[tex_cnt].chr_nam,mbuf[i].tex_nam);
			tex_cnt++;
			if ( tex_cnt > TEX_MAX ){
				printf("s3dto3dt : Error : Table Limit over : TEX_MAX\n");
				exit(0);
			}
		}
	}

	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

	if ( g_dgt == 1 ){
		/* ＤＧＴファイルありの時、インクルード文のみ出力 */
		strcpy(string,"#include ");
		strcat(string,g_dgt_nam);
		strcat(string,"\n");
		fputs(string,fppo);
		fputs("\n",fppo);
	}else{
		/* ＤＲＡＷ／ＣＯＬＯＲ情報 */
		for ( i = 1; i < g_tex_max+1; i++ ){

			/* ＤＲＡＷ */
			strcpy(string,"#define DRAW");
			strcat(string,mbuf[i].mat_nam);
			strcat(string,DRAW_DEF_T);
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

		/* インクルード文 */
		for ( i = 0; i < tex_cnt; i++ ){
			strcpy(string,"#include ");
			strcat(string,txt[i].chr_nam);
			strcat(string,"\n");
			fputs(string,fppo);
		}
		fputs("\n",fppo);

		/* テクスチャテーブル本体 */
		fputs("static SprTexture Texture[] = {\n",fppo);
		for ( i = 0; i < tex_cnt; i++ ){

			/* 開始括弧 */
			fputs("    {\n",fppo);

			/* キャラクタＮｏ． */
			fprintf(fppo,"/* character no.        */ %d,\n",txt[i].char_no);

			/* カラーモード */
			strcpy(string,"/* color mode           */ ");
			strcat(string,txt[i].clr_mod);
			strcat(string,",\n");
			fputs(string,fppo);

			/* カラー */
			fprintf(fppo,"/* color data           */ %d,\n",txt[i].color);

			/* キャラクタ幅 */
			fprintf(fppo,"/* character width      */ %d,\n",txt[i].width);

			/* キャラクタ高 */
			fprintf(fppo,"/* character height     */ %d,\n",txt[i].height);

			/* キャラクタポインタ */
			strcpy(string,"/* character pointer    */ (Uint8 *)");
			/* 始めの「"」と「.」に挾まれる文字列を抽出 */
			for ( j = 0; j < 80; j++ ){
				if ( txt[i].chr_nam[j+1] != '.' &&
				     txt[i].chr_nam[j+1] != '"' ) {
					buf[j] = txt[i].chr_nam[j+1];
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
			if ( strcmp(txt[i].lup_ptr,"NULL") == 0 ){
				strcat(string,txt[i].lup_ptr);
			} else {
				strcat(string,"(SprLookupTbl *)");
				strcat(string,txt[i].lup_ptr);
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
		strcat(string,txt[tex_cnt-1].clr_mod);
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

		/* シェーディングテーブル（現状出力せず） */

		/* シェーディングテーブル本体 */
		/*fputs("static Uint16 grayCode[32] = {\n",fppo);
		fputs("        RGB16_COLOR( 0, 0, 0), RGB16_COLOR( 1, 1, 1), RGB16_COLOR( 2, 2, 2), RGB16_COLOR( 3, 3, 3),\n",fppo);
		fputs("        RGB16_COLOR( 4, 4, 4), RGB16_COLOR( 5, 5, 5), RGB16_COLOR( 6, 6, 6), RGB16_COLOR( 7, 7, 7),\n",fppo);
		fputs("        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 9, 9, 9), RGB16_COLOR(10,10,10), RGB16_COLOR(11,11,11),\n",fppo);
		fputs("        RGB16_COLOR(12,12,12), RGB16_COLOR(13,13,13), RGB16_COLOR(14,14,14), RGB16_COLOR(15,15,15),\n",fppo);
		fputs("        RGB16_COLOR(16,16,16), RGB16_COLOR(17,17,17), RGB16_COLOR(18,18,18), RGB16_COLOR(19,19,19),\n",fppo);
		fputs("        RGB16_COLOR(20,20,20), RGB16_COLOR(21,21,21), RGB16_COLOR(22,22,22), RGB16_COLOR(23,23,23),\n",fppo);
		fputs("        RGB16_COLOR(24,24,24), RGB16_COLOR(25,25,25), RGB16_COLOR(26,26,26), RGB16_COLOR(27,27,27),\n",fppo);
		fputs("        RGB16_COLOR(28,28,28), RGB16_COLOR(29,29,29), RGB16_COLOR(30,30,30), RGB16_COLOR(31,31,31)\n",fppo);
		fputs("};\n",fppo);
		fputs("\n",fppo);*/

		/* シェーディングインデックステーブル */
		/*fputs("static Uint16 *shdIdxTbl[] = {\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode,\n",fppo);
		fputs("        grayCode\n",fppo);
		fputs("};\n",fppo);
		fputs("\n",fppo);*/

	}

		/* 出力ファイルクローズ */
		fclose ( fppo );
	}


	/* Data Dump Section */
	/*printf("\n");
	for ( i = 1; i < g_tex_max+1; i++ ){
		printf(" no = %d : mat_nam = %s\n",i,mbuf[i].mat_nam);
		printf(" no = %d : type = %d\n",i,mbuf[i].type);
		printf(" no = %d : color = %x\n",i,mbuf[i].color);
	}*/

}
