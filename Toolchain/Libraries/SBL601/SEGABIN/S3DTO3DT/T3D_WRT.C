/*------------------------------------------------------------------------
 *  FILE: t3d_wrt.c
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
 *		Write Vert/Normal/Face/Object information
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
t3d_wrt( no )

	Sint16	no;

{
	static	Sint8	string[200];
	static	Sint8	buf[124];
	Sint16	i;
	Sint16	x;
	Uint16	color_r, color_g, color_b;
	FILE	*fppo;

/* printf("t3d_wrt started ***\n\n"); */
	/* TypeBファイルオープン（追加モード） */
	if ( !g_vert_max ) {
	} else {
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

		/* オブジェクト情報書き出し */
		/* 頂点座標テーブル */
		strcpy(string,"static MthXyz ");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"VertPoint[] = {\n");
		fputs(string,fppo);
		for ( i = 0; i < g_vert_max; i++ ){
			strcpy(string,"/* ");
			sprintf(buf,"%-4d",i);
			strcat(string,buf);
			strcat(string," */ {MTH_FIXED(");
			sprintf(buf,"%f",vert[i].co_x_s);
			strcat(string,buf);
			strcat(string,"*SCALE),MTH_FIXED(");
			sprintf(buf,"%f",vert[i].co_y_s);
			strcat(string,buf);
			strcat(string,"*SCALE),MTH_FIXED(");
			sprintf(buf,"%f",vert[i].co_z_s);
			strcat(string,buf);
			if ( i == g_vert_max - 1 ){
				/* 最終行（カンマなし） */
				strcat(string,"*SCALE)}\n");
			} else {
				strcat(string,"*SCALE)},\n");
			}
			fputs(string,fppo);
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* 面の法線ベクトルテーブル */
		strcpy(string,"static MthXyz ");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"SurfaceNormal[] = {\n");
		fputs(string,fppo);
		for ( i = 0; i < g_face_max; i++ ){
			strcpy(string,"/* ");
			sprintf(buf,"%-4d",i);
			strcat(string,buf);
			strcat(string," */ {MTH_FIXED(");
			sprintf(buf,"%f",hx_s[i]);
			strcat(string,buf);
			strcat(string,"),MTH_FIXED(");
			sprintf(buf,"%f",hy_s[i]);
			strcat(string,buf);
			strcat(string,"),MTH_FIXED(");
			sprintf(buf,"%f",hz_s[i]);
			strcat(string,buf);
			if ( i == g_face_max - 1 ){
				/* 最終行（カンマなし） */
				strcat(string,")}\n");
			} else {
				strcat(string,")},\n");
			}
			fputs(string,fppo);
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* 面の中心座標テーブル（出力指定時のみ） */
		if ( g_ctr_c == 1 ){
			strcpy(string,"static MthXyz ");
			strcat(string,obj[no].u_obj_name);
			strcat(string,"surfaceVert[] = {\n");
			fputs(string,fppo);
			for ( i = 0; i < g_face_max; i++ ){
				strcpy(string,"/* ");
				sprintf(buf,"%-4d",i);
				strcat(string,buf);
				strcat(string," */ {MTH_FIXED(");
				sprintf(buf,"%f",cx_s[i]);
				strcat(string,buf);
				strcat(string,"),MTH_FIXED(");
				sprintf(buf,"%f",cy_s[i]);
				strcat(string,buf);
				strcat(string,"),MTH_FIXED(");
				sprintf(buf,"%f",cz_s[i]);
				strcat(string,buf);
				if ( i == g_face_max - 1 ){
					/* 最終行（カンマなし） */
					strcat(string,")}\n");
				} else {
					strcat(string,")},\n");
				}
				fputs(string,fppo);
			}
			fputs("};\n",fppo);
			fputs("\n",fppo);
		}

		/* グーロー用頂点法線ベクトルテーブル（グーローの時のみ） */
		if ( obj[no].shade == 2 ){
			strcpy(string,"static MthXyz ");
			strcat(string,obj[no].u_obj_name);
			strcat(string,"VertNormal[] = {\n");
			fputs(string,fppo);
			for ( i = 0; i < g_vert_max; i++ ){
				strcpy(string,"/* ");
				sprintf(buf,"%-4d",i);
				strcat(string,buf);
				strcat(string," */ {MTH_FIXED(");
				sprintf(buf,"%f",hx_g[i]);
				strcat(string,buf);
				strcat(string,"),MTH_FIXED(");
				sprintf(buf,"%f",hy_g[i]);
				strcat(string,buf);
				strcat(string,"),MTH_FIXED(");
				sprintf(buf,"%f",hz_g[i]);
				strcat(string,buf);
				if ( i == g_vert_max - 1 ){
					/* 最終行（カンマなし） */
					strcat(string,")}\n");
				} else {
					strcat(string,")},\n");
				}
				fputs(string,fppo);
			}
			fputs("};\n",fppo);
			fputs("\n",fppo);
		}

		/* 面定義テーブル */
		strcpy(string,"static SprSurface ");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"Surface[] = {\n");
		fputs(string,fppo);
		for ( i = 0; i < g_face_max; i++ ){
			strcpy(string,"/* ");
			sprintf(buf,"%-4d",i);
			strcat(string,buf);
			strcat(string," */ {    ");

/*printf("direction = %d\n",face[i].direction);*/
			if ( face[i].direction == 0 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_0,face[i].v_no_1,
				face[i].v_no_2,face[i].v_no_3);
			else if ( face[i].direction == 1 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_1,face[i].v_no_2,
				face[i].v_no_3,face[i].v_no_0);
			else if ( face[i].direction == 2 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_2,face[i].v_no_3,
				face[i].v_no_0,face[i].v_no_1);
			else if ( face[i].direction == 3 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_3,face[i].v_no_0,
				face[i].v_no_1,face[i].v_no_2);
			else if ( face[i].direction == 4 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_0,face[i].v_no_3,
				face[i].v_no_2,face[i].v_no_1);
			else if ( face[i].direction == 5 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_1,face[i].v_no_0,
				face[i].v_no_3,face[i].v_no_2);
			else if ( face[i].direction == 6 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_2,face[i].v_no_1,
				face[i].v_no_0,face[i].v_no_3);
			else if ( face[i].direction == 7 )
			sprintf(buf,"%d, %d, %d, %d,",
				face[i].v_no_3,face[i].v_no_2,
				face[i].v_no_1,face[i].v_no_0);

			strcat(string,buf);
			strcat(string,"\n");
			fputs(string,fppo);

			strcpy(string,"                DRAW");
			strcat(string,face[i].mat_nam);
			if ( face[i].z_sort == 0 )
				strcat(string," | ZSORT_MID,\n");
			if ( face[i].z_sort == 1 )
				strcat(string," | ZSORT_MIN,\n");
			if ( face[i].z_sort == 2 )
				strcat(string," | ZSORT_MAX,\n");
			fputs(string,fppo);
			strcpy(string,"                COLOR");
			strcat(string,face[i].mat_nam);
			strcat(string,"\n");
			fputs(string,fppo);
			if ( i == g_face_max - 1 ){
				/* 最終行（カンマなし） */
				strcpy(string,"           }\n");
			} else {
				strcpy(string,"           },\n");
			}
			fputs(string,fppo);
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* オブジェクトメイン */
		strcpy(string,"static SprObject3D Obj");
		strcat(string,obj[no].u_obj_name);
		strcat(string," = {\n");
		fputs(string,fppo);

		/* オブジェクト番号 */
		fprintf(fppo,"/* object no.           */ %d,\n",g_obj_cnt);

		/* 表示フラグ（ポリゴン or ポリライン） */
		if ( obj[no].p_type == 0 )
		fputs("/* display flag         */ DISP_POLYGON |\n",fppo);
		if ( obj[no].p_type == 1 )
		fputs("/* display flag         */ DISP_POLYLINE |\n",fppo);

		/* 表示フラグ（シェーディング種別） */
		if ( obj[no].shade == 0 )
		fputs("                           NO_SHADING,\n",fppo);
		if ( obj[no].shade == 1 )
		fputs("                           FLAT_SHADING,\n",fppo);
		if ( obj[no].shade == 2 )
		fputs("                           GOURAUD_SHADING,\n",fppo);

		/* 頂点数 */
		fprintf(fppo,"/* number of vertex     */ %d,\n",g_vert_max);

		/* 面数 */
		fprintf(fppo,"/* number of surface    */ %d,\n",g_face_max);

		/* 頂点座標テーブル */
		strcpy(string,"/* vertex table         */ (MthXyz *)&");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"VertPoint,\n");
		fputs(string,fppo);

		/* 頂点の法線ベクトルテーブル（グーローの時のみ） */
		if ( obj[no].shade == 0 ){
			strcpy(string,"/* vertex normal table  */ NULL,\n");
		} else if ( obj[no].shade == 1 ){
			strcpy(string,"/* vertex normal table  */ NULL,\n");
		} else if ( obj[no].shade == 2 ){
			strcpy(string,"/* vertex normal table  */ (MthXyz *)&");
			strcat(string,obj[no].u_obj_name);
			strcat(string,"VertNormal,\n");
		}
		fputs(string,fppo);

		/* 面定義テーブル */
		strcpy(string,"/* surface table        */ (SprSurface *)&");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"Surface,\n");
		fputs(string,fppo);

		/* 面の法線ベクトルテーブル */
		strcpy(string,"/* surface normal table */ (MthXyz *)&");
		strcat(string,obj[no].u_obj_name);
		strcat(string,"SurfaceNormal,\n");
		fputs(string,fppo);

		/* 面の中心座標テーブル */
		if ( g_ctr_c == 0 ){
			strcpy(string,"/* surface vertex table */ NULL,\n");
		} else {
			strcpy(string,"/* surface vertex table */ (MthXyz *)&");
			strcat(string,obj[no].u_obj_name);
			strcat(string,"surfaceVert,\n");
		}
		fputs(string,fppo);

		/* シェーディングインデックステーブル */
		if ( obj[no].mode == 2 ){
			fputs("/* shading index table  */ shdIdxTbl,\n",fppo);
		}else{
			fputs("/* shading index table  */ NULL,\n",fppo);
		}

		/* 面法線ベクトルの計算補正値 */
		if ( strcmp(g_version,"1.00") == 0 )
		fputs("/* surf normal keisuu   */ MTH_FIXED(1.0),\n",fppo);

		/* 次オブジェクト（常にＮＵＬＬ） */
		strcpy(string,"/* next object          */ ");
		strcat(string,"NULL\n");
		fputs(string,fppo);

		/* 終了括弧 */
		fputs("};\n",fppo);
		fputs("\n",fppo);


		/* オブジェクト番号カウントアップ */
		g_obj_cnt++;

		/* TypeBファイルクローズ */
		fclose ( fppo );
    }
	}

	/* Data Dump Section */
	/*printf("\n");
	printf(" face_max = %d\n",g_face_max);
	for ( x = 0; x < g_vert_max; x++ ){
		printf("vert[%d].co_x_s = %f\n",x,vn[x]->co_x_s);
		printf("vert[%d].co_y_s = %f\n",x,vn[x]->co_y_s);
		printf("vert[%d].co_z_s = %f\n",x,vn[x]->co_z_s);
	}
	for ( x = 0; x < g_face_max; x++ ){
		printf("face[%d].v_no_0 = %d\n",x,face[x].v_no_0);
		printf("face[%d].v_no_1 = %d\n",x,face[x].v_no_1);
		printf("face[%d].v_no_2 = %d\n",x,face[x].v_no_2);
		printf("face[%d].v_no_3 = %d\n",x,face[x].v_no_3);
		printf("hx_s[%d] = %f : hy_s[%d] = %f : hz_s[%d] = %f\n",
			x,hx_s[x],x,hy_s[x],x,hz_s[x]);
	}*/

}
