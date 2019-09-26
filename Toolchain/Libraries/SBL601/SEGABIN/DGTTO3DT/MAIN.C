/*------------------------------------------------------------------------
 *  FILE: main.c
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
 *		Main routine
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

#define	MAIN
#include "dgtto3dt.h"

void main( argc, argv )
	Sint16	argc;
	Sint8	*argv[];
{
	Sint16	i;

	/* コピーライト表示 */
	printf("DGT to 3D_Table Converter Ver. 1.00\n");
	printf("Copyright (c) by SEGA Enterprises, Ltd. 1994. All right reserved.\n");

	/* コマンドラインパラメータ読み込み */
	for( i = 1; i < argc; i++ ){
		if( g_dirname == NULL )
			g_dirname = argv[i];
		else if ( g_outname == NULL )
			g_outname = argv[i];
		else
			printUsage();
	}
	if( g_dirname == NULL ) printUsage();
	if( g_outname == NULL ) printUsage();

	/* .ctlファイル読み込み */
	ctl_read();

	/* 入力ファイル読み込み */
	mat_read();

	/* テクスチャファイル数Loop */
	for ( i = 0; i < g_tex_cnt; i++ ){

		/* 入力ファイル読み込み */
		if ( g_plt_mod ==  1 ) tex_read( i );
		if ( g_plt_mod ==  2 ) tex_red2( i );
		if ( g_plt_mod ==  3 ) tex_red3( i );

		/* 出力ファイル書き出し（ピクセルデータ） */
		pix_wrt( i );

		/* 出力ファイル書き出し（パレットデータ：分離モード） */
		if ( g_plt_mod ==  1 ) plt_wrt( i );

	}

	/* 出力ファイル書き出し（パレットデータ：合体モード） */
	if ( g_plt_mod ==  2 ) plt_wrt2();

	/* 出力ファイル書き出し（DRAW/COLORデータ） */
	drw_wrt();

}

void printUsage()
{
	printf("\n");
	printf("Usage: dgtto3dt <input/output directory path> <file_name>\n");
	printf("Release no.: 08\n");
	printf("\n");
	exit(-1);
}
