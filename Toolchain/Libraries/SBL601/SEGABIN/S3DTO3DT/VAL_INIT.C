/*------------------------------------------------------------------------
 *  FILE: val_init.c
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
 *		Initialize each parameter
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
val_init()
{
	static	Sint8	string[] = "";
	static	Sint8	buf[80];
	Sint16	i;
	FILE	*fppi;
	FILE	*fppo;

/*printf("val_init started ***\n\n");*/
	/* オブジェクト初期化 */
	for ( i = 0; i < OBJ_MAX; i++ ){

	strcat(obj[i].u_obj_name,string);	/* オブジェクト固有名 */
	strcat(obj[i].l_obj_name,string);	/* オブジェクトライブラリ名 */
	obj[i].scale_x = 1.0;	/* Ｘ方向倍率（この値を掛ける） */
	obj[i].scale_y = 1.0;	/* Ｙ方向倍率（この値を掛ける） */
	obj[i].scale_z = 1.0;	/* Ｚ方向倍率（この値を掛ける） */
	obj[i].rev_x = 0;	/* Ｘ方向反転フラグ（ 0:off, 1:on ）*/
	obj[i].rev_y = 0;	/* Ｙ方向反転フラグ（ 0:off, 1:on ）*/
	obj[i].rev_z = 0;	/* Ｚ方向反転フラグ（ 0:off, 1:on ）*/
	obj[i].shade = -1;	/* シェーディング方式 */
				/* （ -1:不定, 0:no, 1:flat, 2:gouraud ） */
	obj[i].mode = 0;	/* 描画モード */
				/* （ 0:color, 1:auto, 2:index, 3:texture ） */
	obj[i].p_type = 0;	/* 面種別タイプ */
				/* （ 0:polygon, 1:polyline ） */
	obj[i].tex_no = 0;	/* テクスチャ番号 */
	obj[i].v_max = 0;	/* 頂点数 */
	obj[i].f_max = 0;	/* 面数 */

	}

	/* クラスタ初期化 */
	for ( i = 0; i < CLS_MAX; i++ ){

	strcat(cls[i].u_cls_name,string);	/* クラスタ固有名 */
	cls[i].ang_seq = ANG_SEQ;	/* 回転順序 */
	cls[i].angle_x = 0.0;		/* Ｘ方向回転 */
	cls[i].angle_y = 0.0;		/* Ｙ方向回転 */
	cls[i].angle_z = 0.0;		/* Ｚ方向回転 */
	cls[i].point_x = 0.0;		/* Ｘ方向座標 */
	cls[i].point_y = 0.0;		/* Ｙ方向座標 */
	cls[i].point_z = 0.0;		/* Ｚ方向座標 */
	cls[i].nest = 0;		/* ネスティング・レベル */
	cls[i].object = NULL; 		/* オブジェクト構造体へのポインタ */
	cls[i].c_child = NULL;		/* 子クラスタ用ポインタ */
	cls[i].c_next = NULL;		/* ｎｅｘｔ用ポインタ */

	}

	/* テクスチャ初期化 */
	for ( i = 0; i < TEX_MAX; i++ ){

	txt[i].char_no = i;			/* キャラクタ番号 */
	strcat(txt[i].chr_nam,string);		/* キャラクタ名 */
	strcat(txt[i].clr_mod,"COLOR_5");	/* カラーモード */
	txt[i].color = i;			/* カラー       */
	txt[i].width = 96;			/* キャラクタ幅 */
	txt[i].height = 96;			/* キャラクタ高 */
	strcat(txt[i].lup_ptr,"NULL");	/* ルックアップテーブルポインタ */

	}

	/* パラメータ初期化 */
	strcpy(g_version,"1.00");	/* ソフトライブラリバージョン */
	strcpy(g_scale,"1");		/* 全体スケール */
	g_face_rev = 1;		/* 反転指定時の表面の反転（ 0:なし, 1:あり ）*/
	g_n_calc  = 0;		/* 法線ベクトルの強制計算（ 0:なし, 1:あり ）*/
	g_clstr   = 0;		/* ダミーの親クラスタ発生（ 0:なし, 1:あり ）*/
	g_s_drt   = 0;		/* 頂点の並び（ 0:cw, 1:ccw ）*/
	g_ctr_c   = 0;		/* 面の中心座標テーブル（ 0:なし, 1:あり ）*/
	strcpy(g_in_ext,"3d");		/* 入力ファイルの拡張子 */
	strcpy(g_out_ext,"dat");	/* 出力ファイルの拡張子 */
	g_dgt   = 0;		/* テクスチャファイル名（ 0:なし, 1:あり ）*/

	/* オプションパラメータファイル読み込み */
	ctl_read();

	/* ディレクトリ名と入力ファイル名の連結 */
	strcpy(g_s3dname,g_dirname);
	strcat(g_s3dname,"/");
	strcat(g_s3dname,g_outname);
	strcat(g_s3dname,".");
	strcat(g_s3dname,g_in_ext);
	/*printf("g_s3dname = %s\n",g_s3dname);*/

	/* ディレクトリ名と出力ファイル名の連結 */
	strcpy(g_c_outname,g_dirname);
	strcat(g_c_outname,"/");
	strcat(g_c_outname,g_outname);
	strcat(g_c_outname,".");
	strcat(g_c_outname,g_out_ext);

	/* 出力ファイルオープン（新規モード） */
	fppo = fopen ( g_c_outname, "w" );
	if ( fppo != NULL ){

		/* ＳＣＡＬＥ */
		strcpy(buf,"#define SCALE  ");
		strcat(buf,g_scale);
		strcat(buf,"\n");
		fputs(buf,fppo);

		/* 出力ファイルクローズ */
		fclose ( fppo );
	}

}
