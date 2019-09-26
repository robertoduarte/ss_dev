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
 *		SEGA3D to 3D Table conversion program
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
#include "s3dto3dt.h"

void main( argc, argv )
	Sint16	argc;
	Sint8	*argv[];
{
	Sint16	i;

	/* コピーライト表示 */
	printf("SEGA3D to 3D_Table Converter Ver. 1.00\n");
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

	/* パラメータの初期値設定（.ctl読み込み含む）*/
	val_init();

	/* 入力ファイル読み込み（object文）*/
	obj_read();

	/* 入力ファイル読み込み（material文->DRAW/COLOR作成）*/
	mat_read();

	/* モデル数Loop */
	g_obj_cnt = 0;

printf("SEGA3D FileName[%s]\n", g_s3dname);
	for ( i = g_u_obj_max-1; i > -1; i-- ){
printf("*** Model Read  %4d(Total %4d) *****\r", i, g_u_obj_max );
		/* 入力ファイル読み込み（model文）*/
		mdl_read( i );

printf("*** Slip Read   %4d(Total %4d) *****\r", i, g_u_obj_max );
		/* 入力ファイル読み込み（slips文）*/
		slp_read( i );

printf("*** Model Write %4d(Total %4d) *****\r", i, g_u_obj_max );
		/* 出力ファイル書き出し */
		t3d_wrt( i );
/*printf(" g_nest_max %d\n",g_nest_max);*/

	}

	/* 出力ファイル書き出し（クラスタ）*/
printf("*** Cluster Write               *****\r" );
	cls_wrt();
printf("***          Finished           *****\n" );

}

void printUsage()
{
	printf("\n");
	printf("Usage: s3dto3dt <input/output directory path> <file_name>\n");
	printf("Release no.: 09\n");
	printf("\n");
	exit(-1);
}
