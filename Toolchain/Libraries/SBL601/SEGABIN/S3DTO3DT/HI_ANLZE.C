/*------------------------------------------------------------------------
 *  FILE: hi_anlze.c
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
 *		Link process of cluster
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
hi_anlze()
{
	Sint16	i, j, m, mm;
	Sint16	flg, nest;
	Sint16	x, x1;
	Float32	v_scl_x0, v_scl_x[20];
	Float32	v_scl_y0, v_scl_y[20];
	Float32	v_scl_z0, v_scl_z[20];
	static	Sint8	buf[8];
#ifdef __BORLANDC__
	Cluster	huge *cp1, huge *cp2;
#else
	Cluster	*cp1, *cp2;
#endif

/*printf("hi_anlze started ***\n\n");*/
	/* クラスタの親子、兄弟関係のリンク処理 */
	cp1 =  &cls[g_cls_max-1];
	for ( i = g_cls_max-1; i > -1; i-- ){

		cp2 =  &cls[i-1];
		for ( j = i-1; j > -1; j-- ){

			if ( cp1->nest == cp2->nest ){
				cp2->c_next = cp1;
				break;
			}
			if ( cp1->nest-1 == cp2->nest ){
				if ( cp2->c_child == NULL ) cp2->c_child = cp1;
				break;
			}

		cp2--;
		}

	cp1--;
	}

	/* 再帰呼出しのチェック */
	for ( i = 0; i < g_cls_max; i++ ){
		for ( j = i+1; j < g_cls_max; j++ ){
			if ( cls[i].nest >= cls[j].nest ){
				break;
			}else{
				if ( strcmp(cls[i].u_cls_name,cls[j].u_cls_name) == 0 ){
					/* エラーメッセージ出力 */
					printf("s3dto3dt : Error : Invalid object description : %s\n",cls[i].u_cls_name);
				}
			}
		}
	}

	/* 初めて現われるモデルの反転フラグ無効化 */
	obj[0].rev_x = 0;
	obj[0].rev_y = 0;
	obj[0].rev_z = 0;
	for ( i = 1; i < g_u_obj_max; i++ ){
		flg = 0;
		for ( j = 0; j < i; j++ ){
			if ( strcmp(obj[i].u_obj_name,obj[j].u_obj_name) == 0 ){
			flg = 1;
			break;
			}
		}
		if ( flg == 0 ){
			obj[i].rev_x = 0;
			obj[i].rev_y = 0;
			obj[i].rev_z = 0;
		}
	}

	/* スケール値の継承（親のスケール値を継承する） */
	nest = 1;
	m = 0;
	v_scl_x[0] = obj[0].scale_x;
	v_scl_y[0] = obj[0].scale_y;
	v_scl_z[0] = obj[0].scale_z;
	obj[0].scl_x_r = obj[i].scale_x;
	obj[0].scl_y_r = obj[i].scale_y;
	obj[0].scl_z_r = obj[i].scale_z;
	for ( i = 1; i < g_u_obj_max; i++ ){
		if ( cls[i].nest > nest ){
			m++;
		}else if ( cls[i].nest < nest ){
			m--;
		}
		v_scl_x[m] = obj[i].scale_x;
		v_scl_y[m] = obj[i].scale_y;
		v_scl_z[m] = obj[i].scale_z;
		v_scl_x0 = 1.0;
		v_scl_y0 = 1.0;
		v_scl_z0 = 1.0;
		for ( mm = 0; mm < m+1; mm++ ){
			v_scl_x0 = v_scl_x[mm] * v_scl_x0;
			v_scl_y0 = v_scl_y[mm] * v_scl_y0;
			v_scl_z0 = v_scl_z[mm] * v_scl_z0;
		}
		obj[i].scl_x_r = v_scl_x0;
		obj[i].scl_y_r = v_scl_y0;
		obj[i].scl_z_r = v_scl_z0;
		nest = cls[i].nest;
	}

	/* オブジェクト名の固有化（親は除く） */
	for ( i = 1; i < g_u_obj_max; i++ ){
		sprintf(buf,"_%d",i);
		strcat(obj[i].u_obj_name,buf);
	}

	/* クラスタ名の固有化（親は除く） */
	for ( i = 1; i < g_cls_max; i++ ){
		sprintf(buf,"_%d",i);
		strcat(cls[i].u_cls_name,buf);
	}


	/* Data Dump Section */
	/*for ( x = 0; x < g_u_obj_max; x++ ){
		*printf(" x = %d : u_obj_name = %s\n",x,obj[x].u_obj_name);*
		printf(" x = %d : l_obj_name = %s\n",x,obj[x].l_obj_name);
		printf(" x = %d : rev_x = %d\n",x,obj[x].rev_x);
		printf(" x = %d : rev_y = %d\n",x,obj[x].rev_y);
		printf(" x = %d : rev_z = %d\n",x,obj[x].rev_z);
	}*/
	/*for ( x = 0; x < g_cls_max; x++ ){
		printf(" x = %d : u_cls_name = %s\n",x,cls[x].u_cls_name);
		printf(" x = %d : nest    = %d\n",x,cls[x].nest);
		printf(" x = %d : object = %d\n",x,cls[x].object);
		printf(" x = %d : child = %d\n",x,cls[x].c_child);
		printf(" x = %d : next = %d\n",x,cls[x].c_next);
		*printf(" x = %d : object = %s\n",x,cls[x].object->u_obj_name);
		printf(" x = %d : child = %s\n",x,cls[x].c_child->u_cls_name);
		printf(" x = %d : next = %s\n",x,cls[x].c_next->u_cls_name);*
	}*/
	/*for ( x = 0; x < g_u_obj_max; x++ ){
		printf("nest = %d : scale_z = %f : scl_z_r =%f\n",
			cls[x].nest,obj[x].scale_z,obj[x].scl_z_r);
	}*/

}
