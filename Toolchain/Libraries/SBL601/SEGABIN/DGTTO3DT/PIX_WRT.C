/*------------------------------------------------------------------------
 *  FILE: pix_wrt.c
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
 *		Write pixel data
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
pix_wrt( no )

	Sint16	no;

{
	static	Sint8	string[120];
	static	Sint8	buf[100];
	Sint16	i, j, j0, k;
	Sint16	count, extra;
	FILE	*fppo;

	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

	/* ピクセル情報書き出し */
	strcpy(string,"const Uint16 ");

	/* 「.」までの文字列を抽出 */
	for ( i = 0; i < 80; i++ ){
		if ( txt[no].chr_nam[i] != '.' ) {
			buf[i] = txt[no].chr_nam[i];
		}else{
			buf[i] = NULL;
			break;
		}
	}
	strcat(string,buf);

	strcat(string,"[]={\n");
	fputs(string,fppo);

	count = g_pix_max / 8;
	extra = g_pix_max % 8;
	if ( extra == 0 ){
		extra = 8;
	}else{
		count++;
	}

	j0 = 0;
	for ( j = 0; j < count-1; j++ ){
		strcpy(string,"");
		strcat(string," 0x");
		if ( (g_texdata[j0]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+1]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+1]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+1]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+1]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+2]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+2]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+2]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+2]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+3]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+3]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+3]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+3]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+4]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+4]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+4]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+4]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+5]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+5]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+5]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+5]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+6]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+6]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+6]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[j0+6]);
		strcat(string,buf);
		strcat(string," 0x");
		if ( (g_texdata[j0+7]|0x0fff) == 0x0fff ) strcat(string,"0");
		if ( (g_texdata[j0+7]|0x00ff) == 0x00ff ) strcat(string,"0");
		if ( (g_texdata[j0+7]|0x000f) == 0x000f ) strcat(string,"0");
		sprintf(buf,"%x,\n",g_texdata[j0+7]);
		strcat(string,buf);
		fputs(string,fppo);
		j0 += 8;
	}
	strcpy(string,"");
	for ( k = 0; k < extra-1; k++ ){
		strcat(string," 0x");
		if ( (g_texdata[(count-1)*8+k]|0x0fff) == 0x0fff )
			strcat(string,"0");
		if ( (g_texdata[(count-1)*8+k]|0x00ff) == 0x00ff )
			strcat(string,"0");
		if ( (g_texdata[(count-1)*8+k]|0x000f) == 0x000f )
			strcat(string,"0");
		sprintf(buf,"%x,",g_texdata[(count-1)*8+k]);
		strcat(string,buf);
	}
	strcat(string," 0x");
	if ( (g_texdata[(count-1)*8+extra-1]|0x0fff) == 0x0fff )
		strcat(string,"0");
	if ( (g_texdata[(count-1)*8+extra-1]|0x00ff) == 0x00ff )
		strcat(string,"0");
	if ( (g_texdata[(count-1)*8+extra-1]|0x000f) == 0x000f )
		strcat(string,"0");
	sprintf(buf,"%x\n",g_texdata[(count-1)*8+extra-1]);
	strcat(string,buf);
	fputs(string,fppo);

	fputs("};\n",fppo);
	fputs("\n",fppo);

		/* 出力ファイルクローズ */
		fclose ( fppo );
	}


	/* Data Dump Section */

}



