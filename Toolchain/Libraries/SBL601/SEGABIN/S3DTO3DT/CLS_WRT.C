/*------------------------------------------------------------------------
 *  FILE: cls_wrt.c
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
 *		Write Cluster information
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
cls_wrt()
{
	static	Sint8	string[80];
	static	Sint8	buf[80];
	Sint16	h, i, i0;
	Sint16	x;
	Sint16	total_v;
	Sint16	total_vn;
	Sint16	total_f;
	FILE	*fppo;

#if 0
	for ( h = g_nest_max; h > -1; h-- ){
		for ( i = g_cls_max-1; i > -1; i-- ){
			if ( cls[i].nest == h ){
				if ( cls[i].c_child != NULL ) {
					cls[i].c_child->c_next = cls[i].c_next;
				}
			}
		}
	}
#endif
/*printf("cls_wrt started ***\n\n");*/
	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

	/* クラスタ情報書き出し（リンクの関係で後ろから処理する） */
	i0 = 0;
	for ( h = g_nest_max; h > -1; h-- ){

	for ( i = g_cls_max-1; i > -1; i-- ){

	if ( cls[i].nest == h ){
				/* クラスタ名 */
		strcpy(string,"static SprCluster ");
		strcat(string,cls[i].u_cls_name);
		strcat(string," = {\n");
		fputs(string,fppo);

		/* クラスタ番号 */
		fprintf(fppo,"/* cluster no.          */ %d,\n",i0);

		/* 回転順序 */
		strcpy(string,"/* rotation sequence    */ ");
		if ( cls[i].ang_seq == 0 ){
			strcat(string,"ROT_SEQ_ZYX");
		}else if ( cls[i].ang_seq == 1 ){
			strcat(string,"ROT_SEQ_ZXY");
		}else if ( cls[i].ang_seq == 2 ){
			strcat(string,"ROT_SEQ_YZX");
		}else if ( cls[i].ang_seq == 3 ){
			strcat(string,"ROT_SEQ_YXZ");
		}else if ( cls[i].ang_seq == 4 ){
			strcat(string,"ROT_SEQ_XYZ");
		}else if ( cls[i].ang_seq == 5 ){
			strcat(string,"ROT_SEQ_XZY");
		}else{
			strcat(string,"NULL");
		}
		strcat(string,",\n");
		fputs(string,fppo);

		/* Ｘ回転 */
		strcpy(string,"/* rotation x           */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].angle_x);
		strcat(string,buf);
		strcat(string,"),\n");
		fputs(string,fppo);

		/* Ｙ回転 */
		strcpy(string,"/* rotation y           */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].angle_y);
		strcat(string,buf);
		strcat(string,"),\n");
		fputs(string,fppo);

		/* Ｚ回転 */
		strcpy(string,"/* rotation z           */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].angle_z);
		strcat(string,buf);
		strcat(string,"),\n");
		fputs(string,fppo);

		/* Ｘ座標 */
		strcpy(string,"/* point x              */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].point_x);
		strcat(string,buf);
		strcat(string,"*SCALE),\n");
		fputs(string,fppo);

		/* Ｙ座標 */
		strcpy(string,"/* point y              */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].point_y);
		strcat(string,buf);
		strcat(string,"*SCALE),\n");
		fputs(string,fppo);

		/* Ｚ座標 */
		strcpy(string,"/* point z              */ MTH_FIXED(");
		sprintf(buf,"%f",cls[i].point_z);
		strcat(string,buf);
		strcat(string,"*SCALE),\n");
		fputs(string,fppo);

		/* オブジェクト（要修正？） */
		strcpy(string,"/* object               */ ");
		if ( cls[i].object == NULL ){
			strcat(string,"NULL");
		}else{
			if ( !cls[i].object->v_max ) {
				strcat(string,"NULL");
			} else {
				strcat(string,"&Obj");
				strcat(string,cls[i].object->u_obj_name);
			}
		}
		strcat(string,",\n");
		fputs(string,fppo);

		/* 次クラスタ */
		strcpy(string,"/* next cluster         */ ");
		if ( cls[i].c_next == NULL ){
			strcat(string,"NULL");
		}else{
			strcat(string,"&");
			strcat(string,cls[i].c_next->u_cls_name);
		}
		strcat(string,",\n");
		fputs(string,fppo);


		/* 子クラスタ */
		strcpy(string,"/* child cluster        */ ");
		if ( cls[i].c_child == NULL ){
			strcat(string,"NULL");
		}else{
			strcat(string,"&");
			strcat(string,cls[i].c_child->u_cls_name);
		}
		if ( strcmp(g_version,"1.00") == 0 )
			strcat(string,",\n");
		else
			strcat(string,"\n");
		fputs(string,fppo);

		if ( strcmp(g_version,"1.00") == 0 ){
			/* ３Ｄオブジェクト間接続ポリゴン情報 */
			fputs("/* inbetween obj info   */ NULL,\n",fppo);

			/* 座標変換開始前ユーザコールバックルーチン */
			fputs("/* called befour trans  */ NULL,\n",fppo);

			/* 座標変換終了前ユーザコールバックルーチン */
			fputs("/* called after trans   */ NULL,\n",fppo);

			/* ユーザコンテキストエリア */
			fputs("/* user context area    */ NULL\n",fppo);
		}

		/* 終了括弧 */
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* クラスタ数カウントアップ */
		i0++;
	}
	}

	}

	/* ダミーの親クラスタ出力 */
	if ( g_clstr == 1 ){
		strcpy(string,"static SprCluster ");
		strcat(string,g_top_cls);
		strcat(string," = {\n");
		fputs(string,fppo);
		fprintf(fppo,"/* cluster no.          */ %d,\n",i0);
		strcpy(string,"/* rotation sequence    */ ");
		if ( cls[0].ang_seq == 0 ){
			strcat(string,"ROT_SEQ_ZYX");
		}else if ( cls[0].ang_seq == 1 ){
			strcat(string,"ROT_SEQ_ZXY");
		}else if ( cls[0].ang_seq == 2 ){
			strcat(string,"ROT_SEQ_YZX");
		}else if ( cls[0].ang_seq == 3 ){
			strcat(string,"ROT_SEQ_YXZ");
		}else if ( cls[0].ang_seq == 4 ){
			strcat(string,"ROT_SEQ_XYZ");
		}else if ( cls[0].ang_seq == 5 ){
			strcat(string,"ROT_SEQ_XZY");
		}else{
			strcat(string,"NULL");
		}
		strcat(string,",\n");
		fputs(string,fppo);
		fputs("/* rotation x           */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* rotation y           */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* rotation z           */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* point x              */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* point y              */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* point z              */ MTH_FIXED(0.000000),\n",fppo);
		fputs("/* object               */ NULL,\n",fppo);
		fputs("/* next cluster         */ NULL,\n",fppo);
		strcpy(string,"/* child cluster        */ &");
		strcat(string,cls[0].u_cls_name);
		if ( strcmp(g_version,"1.00") == 0 ) strcat(string,",\n");
		else				     strcat(string,"\n");
		fputs(string,fppo);
		if ( strcmp(g_version,"1.00") == 0 ){
			fputs("/* inbetween obj info   */ NULL,\n",fppo);
			fputs("/* called befour trans  */ NULL,\n",fppo);
			fputs("/* called after trans   */ NULL,\n",fppo);
			fputs("/* user context area    */ NULL\n",fppo);
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);
	}

	/* サマリー情報の出力 */
	total_v = 0;
	total_f = 0;
	total_vn = 0;
	fputs("/*      << Data Information >>\n",fppo);
	fputs("       object_name    vertex     face   normal   normal(gouraud)\n",fppo);
	for ( i = 0; i < g_u_obj_max; i++ ){
		if ( obj[i].shade == 2 ){
			fprintf(fppo,"        %-10s   %6d   %6d   %6d   %6d\n",
				obj[i].u_obj_name,obj[i].v_max,obj[i].f_max,
				obj[i].f_max,obj[i].v_max);
			total_vn = total_vn + obj[i].v_max;
		}else{
			fprintf(fppo,"        %-10s   %6d   %6d   %6d        -\n",
				obj[i].u_obj_name,obj[i].v_max,obj[i].f_max,
				obj[i].f_max);
		}
		total_v = total_v + obj[i].v_max;
		total_f = total_f + obj[i].f_max;
	}
	fputs("       ------------------------------------------------\n",fppo);
	fprintf(fppo,"        Total        %6d   %6d   %6d   %6d\n",
		total_v,total_f,total_f,total_vn);
	fputs("*/\n",fppo);

	/* 出力ファイルクローズ */
	fclose ( fppo );

	}

	/* Data Dump Section */
	/*printf("\n");*/

}
