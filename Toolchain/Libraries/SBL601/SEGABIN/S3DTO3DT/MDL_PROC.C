/*------------------------------------------------------------------------
 *  FILE: mdl_proc.c
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
 *		Calc about Vertex ( Scale , Reverse )
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
mdl_proc( no )

	Sint16	no;

{
	Sint16	i, j, k;
	Float64	x_m[4], y_m[4], z_m[4];
	Float64	leng_h, h_c;

/*printf("mdl_proc started *** : mdl_name = %s\n\n",obj[no].u_obj_name);*/
	/* 頂点座標値のスケール計算 */
	for ( i = 0; i < g_vert_max; i++ ){
		vert[i].co_x_s = obj[no].scl_x_r * vert[i].co_x;
		vert[i].co_y_s = obj[no].scl_y_r * vert[i].co_y;
		vert[i].co_z_s = obj[no].scl_z_r * vert[i].co_z;
	}

	/* 面の法線ベクトルテーブル */
	for ( j = 0; j < g_face_max; j++ ){
		if ( g_n_calc == 1 ||
		     ( hx_s[j] == 0 && hy_s[j] == 0 && hz_s[j] == 0 ) ){
			if ( g_s_drt == 0 ){
				x_m[0] = vn[face[j].v_no_0]->co_x_s;
				y_m[0] = vn[face[j].v_no_0]->co_y_s;
				z_m[0] = vn[face[j].v_no_0]->co_z_s;
				x_m[1] = vn[face[j].v_no_1]->co_x_s;
				y_m[1] = vn[face[j].v_no_1]->co_y_s;
				z_m[1] = vn[face[j].v_no_1]->co_z_s;
				x_m[2] = vn[face[j].v_no_2]->co_x_s;
				y_m[2] = vn[face[j].v_no_2]->co_y_s;
				z_m[2] = vn[face[j].v_no_2]->co_z_s;
			}else{
				x_m[0] = vn[face[j].v_no_2]->co_x_s;
				y_m[0] = vn[face[j].v_no_2]->co_y_s;
				z_m[0] = vn[face[j].v_no_2]->co_z_s;
				x_m[1] = vn[face[j].v_no_1]->co_x_s;
				y_m[1] = vn[face[j].v_no_1]->co_y_s;
				z_m[1] = vn[face[j].v_no_1]->co_z_s;
				x_m[2] = vn[face[j].v_no_0]->co_x_s;
				y_m[2] = vn[face[j].v_no_0]->co_y_s;
				z_m[2] = vn[face[j].v_no_0]->co_z_s;
			}
			hx_s[j] = ( y_m[1] - y_m[0] ) * ( z_m[2] - z_m[1] )
				- ( z_m[1] - z_m[0] ) * ( y_m[2] - y_m[1] );
			hy_s[j] = ( z_m[1] - z_m[0] ) * ( x_m[2] - x_m[1] )
				- ( x_m[1] - x_m[0] ) * ( z_m[2] - z_m[1] );
			hz_s[j] = ( x_m[1] - x_m[0] ) * ( y_m[2] - y_m[1] )
				- ( y_m[1] - y_m[0] ) * ( x_m[2] - x_m[1] );

			/* 面の法線ベクトルの単位化 */
			leng_h = sqrt( pow(hx_s[j],(double)2)
				     + pow(hy_s[j],(double)2)
				     + pow(hz_s[j],(double)2) );
			hx_s[j] = hx_s[j] / leng_h;
			hy_s[j] = hy_s[j] / leng_h;
			hz_s[j] = hz_s[j] / leng_h;
		}
	}

	/* グーロー用頂点法線ベクトルテーブル */
	if ( obj[no].shade == 2 ){

	for ( j = 0; j < g_vert_max; j++ ){
		if ( g_n_calc == 1 ||
		     ( hx_g[j] == 0 && hy_g[j] == 0 && hz_g[j] == 0 ) ){
			hx_g[j] = 0.0;
			hy_g[j] = 0.0;
			hz_g[j] = 0.0;
			h_c = 0;
			for ( k = 0; k < g_face_max; k++ ){
				if ( face[k].v_no_0 == j ||
				     face[k].v_no_1 == j ||
				     face[k].v_no_2 == j ||
				     face[k].v_no_3 == j ){
					hx_g[j] = hx_g[j] + hx_s[k];
					hy_g[j] = hy_g[j] + hy_s[k];
					hz_g[j] = hz_g[j] + hz_s[k];
					h_c++;
				}
			}
			if ( h_c == 0 ){
				/* 使われていない頂点の場合 */
				hx_g[j] = 0;
				hy_g[j] = 0;
				hz_g[j] = 0;
			}else{
				hx_g[j] = hx_g[j] / h_c;
				hy_g[j] = hy_g[j] / h_c;
				hz_g[j] = hz_g[j] / h_c;

				/* 頂点の法線ベクトルの単位化 */
				leng_h = sqrt( pow(hx_g[j],(double)2)
					     + pow(hy_g[j],(double)2)
					     + pow(hz_g[j],(double)2) );
				if ( leng_h != 0 ){
					hx_g[j] = hx_g[j] / leng_h;
					hy_g[j] = hy_g[j] / leng_h;
					hz_g[j] = hz_g[j] / leng_h;
				}
			}
		}
	}

	}

	/* 頂点座標値の反転 */
	for ( i = 0; i < g_vert_max; i++ ){
		if ( obj[no].rev_x == 1 )
			vert[i].co_x_s = ( -1 ) * vert[i].co_x_s;
		if ( obj[no].rev_y == 1 )
			vert[i].co_y_s = ( -1 ) * vert[i].co_y_s;
		if ( obj[no].rev_z == 1 )
			vert[i].co_z_s = ( -1 ) * vert[i].co_z_s;
	}

	/* 頂点座標値の反転にともなう面の法線ベクトルの反転 */
	for ( i = 0; i < g_face_max; i++ ){
		if ( obj[no].rev_x == 1 ) hx_s[i] = ( -1 ) * hx_s[i];
		if ( obj[no].rev_y == 1 ) hy_s[i] = ( -1 ) * hy_s[i];
		if ( obj[no].rev_z == 1 ) hz_s[i] = ( -1 ) * hz_s[i];
	}

	/* 頂点座標値の反転にともなう頂点の法線ベクトルの反転 */
	for ( i = 0; i < g_vert_max; i++ ){
		if ( obj[no].rev_x == 1 ) hx_g[i] = ( -1 ) * hx_g[i];
		if ( obj[no].rev_y == 1 ) hy_g[i] = ( -1 ) * hy_g[i];
		if ( obj[no].rev_z == 1 ) hz_g[i] = ( -1 ) * hz_g[i];
	}

	/* 面の中心座標テーブル（出力指定時のみ） */
	if ( g_ctr_c == 1 ){
		for ( j = 0; j < g_face_max; j++ ){
			x_m[0] = vn[face[j].v_no_0]->co_x_s;
			y_m[0] = vn[face[j].v_no_0]->co_y_s;
			z_m[0] = vn[face[j].v_no_0]->co_z_s;
			x_m[1] = vn[face[j].v_no_1]->co_x_s;
			y_m[1] = vn[face[j].v_no_1]->co_y_s;
			z_m[1] = vn[face[j].v_no_1]->co_z_s;
			x_m[2] = vn[face[j].v_no_2]->co_x_s;
			y_m[2] = vn[face[j].v_no_2]->co_y_s;
			z_m[2] = vn[face[j].v_no_2]->co_z_s;
			x_m[3] = vn[face[j].v_no_3]->co_x_s;
			y_m[3] = vn[face[j].v_no_3]->co_y_s;
			z_m[3] = vn[face[j].v_no_3]->co_z_s;
			cx_s[j] = ( x_m[0] + x_m[1] + x_m[2] + x_m[3] ) / 4;
			cy_s[j] = ( y_m[0] + y_m[1] + y_m[2] + y_m[3] ) / 4;
			cz_s[j] = ( z_m[0] + z_m[1] + z_m[2] + z_m[3] ) / 4;
		}
	}

}
