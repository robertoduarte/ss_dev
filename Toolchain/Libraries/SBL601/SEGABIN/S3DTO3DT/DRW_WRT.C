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
 *		SEGA3D to 3DTable conversion program
 *
 *  DESCRIPTION:
 *		Write DRAW/COLOR information
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
drw_wrt()

{
	static	Sint8	string[120];
	static	Sint8	buf[80];
	Sint16	m_bufno;
	Uint16	color_r, color_g, color_b;
	FILE	*fppo;

/*printf("drw_wrt started ***\n\n");*/
	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

		/* ヘッダー情報書き出し（テクスチャはtex_wrtで一括出力） */

		/* ＤＲＡＷ／ＣＯＬＯＲ情報 */
		m_bufno = 0;
		/* ＤＲＡＷ */
		strcpy(string,"#define DRAW");
		strcat(string,mbuf[m_bufno].mat_nam);
		if ( mbuf[m_bufno].mode == 0 ) strcat(string,DRAW_DEF_C);
		if ( mbuf[m_bufno].mode == 1 ) strcat(string,DRAW_DEF_A);
		if ( mbuf[m_bufno].mode == 2 ) strcat(string,DRAW_DEF_I);
		if ( (mbuf[m_bufno].type&0x0001) == 1 )
			strcat(string," | DRAW_MESH");
		if ( mbuf[m_bufno].type < 2 ){
			strcat(string," | COMPO_REP");
		}else{
			if( (mbuf[m_bufno].type&0x0002) == 2 )
				strcat(string," | COMPO_TRANS");
			if( (mbuf[m_bufno].type&0x0004) == 4 )
				strcat(string," | COMPO_HARF");
		}
		strcat(string,"\n");
		fputs(string,fppo);

		/* ＣＯＬＯＲ */
		strcpy(string,"#define COLOR");
		strcat(string,mbuf[m_bufno].mat_nam);
		if ( mbuf[m_bufno].mode == 2 ){
			strcat(string," 0");
		}else{
			color_r = mbuf[m_bufno].color;
			color_r = color_r&0x001f;
			color_g = mbuf[m_bufno].color;
			color_g = (color_g>>5)&0x001f;
			color_b = mbuf[m_bufno].color;
			color_b = (color_b>>10)&0x001f;
			sprintf(buf," RGB16_COLOR(%d,%d,%d)",
			color_r,color_g,color_b);
			strcat(string,buf);
		}
		strcat(string,"\n");
		fputs(string,fppo);

		/* 出力ファイルクローズ */
		fclose ( fppo );
	}


	/* Data Dump Section */
	/*printf("\n");
	printf(" no = %d : mat_nam = %s\n",m_bufno,mbuf[m_bufno].mat_nam);
	printf(" no = %d : type = %d\n",m_bufno,mbuf[m_bufno].type);
	printf(" no = %d : color = %x\n",m_bufno,mbuf[m_bufno].color);*/
}
