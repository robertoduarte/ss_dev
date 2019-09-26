/*------------------------------------------------------------------------
 *  FILE: mdl_read.c
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
 *		Read model information from SEGA3D file
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
mdl_read( no )

	Sint16	no;

{
	static	Sint8	buf[130];
	static	Sint8	keyword[130];
	Sint16	c, i, j, k, kk, k1;
	Sint16	x, x1;
	Sint16	cnt, pos;
	Sint16	skip, nest, md_flg, md_flg2;
	FILE	*fppi;

printf("*** Model Read\r" );
	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
		printf("ERROR:(mdl_read)\n");
		exit(0);
	}else{

	for ( x = 0; x < FACE_MAX; x++ ){
		hx_s[x] = 0;
		hy_s[x] = 0;
		hz_s[x] = 0;
	}
	for ( x = 0; x < VERT_MAX; x++ ){
		hx_g[x] = 0;
		hy_g[x] = 0;
		hz_g[x] = 0;
	}

	/* 文字列読み込み */
	i = 0;
	cnt = 0;
	skip = 0;
	nest = 0;
	md_flg = 0, md_flg2 = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/

		/* 空白行の排除 */
		if ( i > 0 ){

		/* model行の検出 */
		if ( nest == 0 ){
			for ( j = 0; j < 120; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"model") == 0 ){
					md_flg = 1;
					ex_keyword4(j,buf,keyword);
					/* 対象モデル選定 */
					if( strcmp(obj[no].l_obj_name,buf)==0 ){
						md_flg = 1;
					} else {
						md_flg = 0;
					}
				}else if ( strcmp(buf,"material") == 0 ||
					   strcmp(buf,"slips") == 0 ||
					   strcmp(buf,"object") == 0 ){
					md_flg = 0;
				}
			}
		/* vertices/polygons行の検出 */
		}else if ( md_flg == 1  && md_flg2 == 0 ){
			/*printf("      keyword(1) = %s/\n",keyword);*/
			for ( j = 0; j < 120; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"vertices") == 0 ){
					md_flg2 = 1;
					cnt = 0;
					ex_keyword4(j,buf,keyword);
					g_vert_max = atoi(buf);
				}else if ( strcmp(buf,"polygons") == 0 ){
					md_flg2 = 2;
					cnt = 0;
					ex_keyword4(j,buf,keyword);
					g_face_max = atoi(buf);
				}
			}
		/* 頂点情報の読み込み */
		}else if ( md_flg == 1 && md_flg2 == 1 ){
			/*printf("      keyword(2) = %s/\n",keyword);*/
			pos = 0;
			k1 = 0;
			for ( k = 0; k < 120; k++ ){
				if ( keyword[k] == NULL ){
					cnt++;
					break;
				}else if ( keyword[k] == '(' ){
				}else if ( keyword[k] == ',' ||
					   keyword[k] == ')' ){
					buf[k1] = NULL;
/*printf("cnt = %d : pos = %d : buf = %s/\n",cnt,pos,buf);*/
					if ( pos == 0 ){
				vert[cnt].co_x = strtod(buf,(char **)NULL);
						vn[cnt]=&vert[cnt];
					}
					else if ( pos == 1 )
				vert[cnt].co_y = strtod(buf,(char **)NULL);
					else if ( pos == 2 )
				vert[cnt].co_z = strtod(buf,(char **)NULL);
					else if ( pos == 4 )
					hx_g[cnt] = strtod(buf,(char **)NULL);
					else if ( pos == 5 )
					hy_g[cnt] = strtod(buf,(char **)NULL);
					else if ( pos == 6 )
					hz_g[cnt] = strtod(buf,(char **)NULL);
					pos++;
					k1 = 0;
				}else{
					buf[k1] = keyword[k];
					k1++;
				}
			}
		/* 面情報の読み込み */
		}else if ( md_flg == 1 && md_flg2 == 2){
			/*printf("      keyword(3) = %s/\n",keyword);*/
			pos = 0;
			k1 = 0;
			for ( k = 0; k < 120; k++ ){
				if ( keyword[k] == NULL ){
					cnt++;
					break;
				}else if ( keyword[k] == '(' ){
				}else if ( keyword[k] == ',' ||
					   keyword[k] == ')' ){
					buf[k1] = NULL;
/*printf("cnt = %d : pos = %d : buf = %s/\n",cnt,pos,buf);*/
			    		if ( pos == 0 )
					face[cnt].v_no_0 = atoi(buf);
					else if ( pos == 1 )
					face[cnt].v_no_1 = atoi(buf);
					else if ( pos == 2 )
					face[cnt].v_no_2 = atoi(buf);
					else if ( pos == 3 )
					face[cnt].v_no_3 = atoi(buf);
					else if ( pos == 5 )
					hx_s[cnt] = strtod(buf,(char **)NULL);
					else if ( pos == 6 )
					hy_s[cnt] = strtod(buf,(char **)NULL);
					else if ( pos == 7 )
					hz_s[cnt] = strtod(buf,(char **)NULL);
					else if ( pos == 9 ){
					    if ( strcmp("AVE",buf) == 0 ){
						face[cnt].z_sort = 0;
					    }else if ( strcmp("MIN",buf) == 0 ){
						face[cnt].z_sort = 1;
					    }else if ( strcmp("MAX",buf) == 0 ){
						face[cnt].z_sort = 2;
					    }
					}
					pos++;
					k1 = 0;
				}else{
					buf[k1] = keyword[k];
					k1++;
				}
			}
		}

		} /* end of if ( i > 0 ) */

		skip = 0;
		i = 0;
	}else if ( skip == 0 && ( c == ' ' || c == TAB ) ){
	}else if ( c == ';' ){
		skip = 1;
	}else if ( skip == 0 && c == '{' ){
		nest++;
	}else if ( skip == 0 && c == '}' ){
		nest--;
		if ( nest == 1 ) md_flg2 = 0;
		if ( nest == 0 ) md_flg = 0;
	}else if ( skip == 0 ){
		keyword[i] = c;
		i++;
	}

	} /* while loop end */


	/* 入力ファイルクローズ */
	fclose ( fppi );
	}

	/* 頂点数と面数のオブジェクトへの登録 */
	obj[no].v_max = g_vert_max;
	obj[no].f_max = g_face_max;

	/* 頂点データのスケール計算 */
	mdl_proc( no );


	/* Data Dump Section */
	/*printf("\n");
	printf(" vert_max = %d\n",g_vert_max);
	for ( x = 0; x < g_vert_max; x++ ){
		printf("vert[%d].co_x = %f\n",x,vn[x]->co_x);
		printf("vert[%d].co_y = %f\n",x,vn[x]->co_y);
		printf("vert[%d].co_z = %f\n",x,vn[x]->co_z);
		printf("hx_g[%d] = %f : hy_g[%d] = %f : hz_g[%d] = %f\n",
			x,hx_g[x],x,hy_g[x],x,hz_g[x]);
	}
	printf(" face_max = %d\n",g_face_max);
	for ( x = 0; x < g_face_max; x++ ){
		printf("face[%d].v_no_0 = %d\n",x,face[x].v_no_0);
		printf("face[%d].v_no_1 = %d\n",x,face[x].v_no_1);
		printf("face[%d].v_no_2 = %d\n",x,face[x].v_no_2);
		printf("face[%d].v_no_3 = %d\n",x,face[x].v_no_3);
		printf("hx_s[%d] = %f : hy_s[%d] = %f : hz_s[%d] = %f\n",
			x,hx_s[x],x,hy_s[x],x,hz_s[x]);
	}*/

}

void
ex_keyword4(j,buf,keyword)
	Sint16	j;
	Sint8	*buf;
	Sint8	*keyword;
{
	Sint16	k, k1;

	k1 = 0;
	for ( k = j+1; k < 120; k++ ){
		buf[k1] = keyword[k];
		if ( keyword[k] == NULL ) break;
		k1++;
	}
}
