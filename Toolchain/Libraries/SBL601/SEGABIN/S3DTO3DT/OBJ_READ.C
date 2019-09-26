/*------------------------------------------------------------------------
 *  FILE: obj_read.c
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
 *		Read object information for header from SEGA3D file
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
obj_read()
{
	static	Sint8	buf[80];
	static	Sint8	keyword[170];
	static	Sint8	model_nam[32];
	Sint16	c, i, j, k, kk, k1;
	Sint16	x, x1;
	Sint16	cnt, pos;
	Sint16	skip, nest, ob_flg;
	FILE	*fppi;

/*printf("obj_read started ***\n\n");*/
	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
		printf("s3dto3dt : Error : Input file not exist : Name = %s\n",g_s3dname);
		exit(0);
	}else{

	/* 文字列読み込み */
	i = 0;
	cnt = 0;
	skip = 0;
	nest = 0;
	ob_flg = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	/* 日本語文字検出 */
#ifdef __BORLANDC__
	if ( c >= 0x81 && c <= 0x9f || c >= 0xe0 && c <= 0xfc ){
#else
	if ( c >= 0x8e && c <= 0xfe && c != 0x8f && c != 0xa0 ){
#endif
		printf("s3dto3dt : Error : Japanese character exist\n");
	}

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/

		/* 空白行の排除 */
		if ( i > 0 ){

		/* object行の検出 */
		if ( nest == 0 ){
			for ( j = 0; j < 160; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"object") == 0 ){
					ob_flg = 1;
					kk = 0;
					for ( k = j+1; k < 160; k++ ){
						buf[kk] = keyword[k];
						if ( keyword[k] == NULL ) break;
						kk++;
					}
				/* objectがないときのためにmodel名読み込み */
				}else if ( strcmp(buf,"model") == 0 ){
					ob_flg = 0;
					kk = 0;
					for ( k = j+1; k < 160; k++ ){
						buf[kk] = keyword[k];
						if ( keyword[k] == NULL ) break;
						kk++;
					}
					strcpy(model_nam,buf);
				}else if ( strcmp(buf,"material") == 0 ||
					   strcmp(buf,"slips") == 0 ){
					ob_flg = 0;
				}
			}
		/* オブジェクト情報の読み込み */
		}else if ( ob_flg == 1 ){
			/*printf("      keyword(2) = %s/\n",keyword);*/
			pos = 0;
			k1 = 0;
			for ( k = 0; k < 160; k++ ){
				if ( keyword[k] == NULL ){

					/* 省略記述modelの自動発生 */
					obj_copy(&cnt);

					cnt++;
					if ( cnt > OBJ_MAX ){
						printf("s3dto3dt : Error : Table Limit over : OBJ_MAX\n");
				 		exit(0);
					}
					break;
				}else if ( keyword[k] == '(' ){
				}else if ( keyword[k] == ',' ||
					   keyword[k] == ')' ){
					buf[k1] = NULL;
/*printf("cnt = %d : pos = %d : buf = %s/\n",cnt,pos,buf);*/

					if ( pos == 0 ){
						/* モデル情報セット */
						strcpy(obj[cnt].u_obj_name,buf);
						strcpy(obj[cnt].l_obj_name,buf);

						/* クラスタ情報セット */
						strcpy(cls[cnt].u_cls_name,buf);
						cls[cnt].ang_seq = ANG_SEQ;
						cls[cnt].nest = nest;
						if ( nest > g_nest_max ) g_nest_max = nest;

						/* オブジェクト情報セット */
						cls[cnt].object = &obj[cnt];
					}
					else if ( pos == 1 )
						obj[cnt].scale_x = strtod(buf,(char **)NULL);
					else if ( pos == 2 )
						obj[cnt].scale_y = strtod(buf,(char **)NULL);
					else if ( pos == 3 )
						obj[cnt].scale_z = strtod(buf,(char **)NULL);
					else if ( pos == 5 )
						cls[cnt].angle_x = strtod(buf,(char **)NULL);
					else if ( pos == 6 )
						cls[cnt].angle_y = strtod(buf,(char **)NULL);
					else if ( pos == 7 )
						cls[cnt].angle_z = strtod(buf,(char **)NULL);
					else if ( pos == 9 )
						cls[cnt].point_x = strtod(buf,(char **)NULL);
					else if ( pos == 10 )
						cls[cnt].point_y = strtod(buf,(char **)NULL);
					else if ( pos == 11 )
						cls[cnt].point_z = strtod(buf,(char **)NULL);
					else if ( pos == 13 )
						obj[cnt].rev_x = atoi(buf);
					else if ( pos == 14 )
						obj[cnt].rev_y = atoi(buf);
					else if ( pos == 15 )
						obj[cnt].rev_z = atoi(buf);
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
		if ( nest == 0 ) ob_flg = 0;
	}else if ( skip == 0 ){
		keyword[i] = c;
		i++;
	}

	} /* while loop end */

	g_u_obj_max = cnt;
	g_cls_max = cnt;

	/* オブジェクト処理 */
	if ( g_u_obj_max == 0 ){
		/* オブジェクト行がない->最後に読んだモデルのみ対象 */
		strcpy(obj[0].u_obj_name,model_nam);
		strcpy(obj[0].l_obj_name,model_nam);
		strcpy(cls[0].u_cls_name,model_nam);
		cls[0].ang_seq = ANG_SEQ;
		cls[0].nest = 1;
		cls[0].object = &obj[0];
		g_nest_max = 1;
		g_u_obj_max = 1;
		g_cls_max = 1;
	}else{
		/* オブジェクト行が複数->オブジェクト行の解析 */
		hi_anlze();
	}

	/* 入力ファイルクローズ */
	fclose ( fppi );

	/* オプションパラメータファイル読み込み（２回目：モデル関連）*/
	ct_read2();

	}


	/* Data Dump Section */
	/*printf("\n");
	printf(" g_u_obj_max = %d\n",g_u_obj_max);
	for ( x = 0; x < g_u_obj_max; x++ ){
		printf(" x = %d : u_obj_name = %s\n",x,obj[x].u_obj_name);
		printf(" x = %d : l_obj_name = %s\n",x,obj[x].l_obj_name);
		printf(" x = %d : rev_x = %d\n",x,obj[x].rev_x);
		printf(" x = %d : rev_y = %d\n",x,obj[x].rev_y);
		printf(" x = %d : rev_z = %d\n",x,obj[x].rev_z);
	}
	printf("\n");
	printf(" g_cls_max = %d\n",g_cls_max);
	for ( x = 0; x < g_cls_max; x++ ){
		printf(" x = %d : u_cls_name = %s\n",x,cls[x].u_cls_name);
		printf(" x = %d : ang_seq = %d\n",x,cls[x].ang_seq);
		printf(" x = %d : angle_x = %f\n",x,cls[x].angle_x);
		printf(" x = %d : point_y = %f\n",x,cls[x].point_y);
		printf(" x = %d : nest    = %d\n",x,cls[x].nest);
		printf(" x = %d : object = %s\n",x,cls[x].object->u_obj_name);
		printf(" x = %d : child = %s\n",x,cls[x].c_child->u_cls_name);
		printf(" x = %d : next = %s\n",x,cls[x].c_next->u_cls_name);
	}*/

}



void
obj_copy(j)

	Sint16	*j;

{
	Sint16	i, k;
	Sint16	x, z;
	Sint16	b_flg, p_nest;

/*printf("obj_copy started *** j = %d\n\n",*j);*/
	/* 省略記述modelの自動発生 */
	z = *j + 1;
	p_nest = cls[*j].nest;
	b_flg = 0;
	for ( i = 0; i < *j; i++ ){
		if ( strcmp(cls[i].u_cls_name,cls[*j].u_cls_name) == 0 ){
			for ( k = i+1; k < *j; k++ ){
				if ( cls[i].nest < cls[k].nest ){
					*j = *j + 1;
				strcpy(obj[*j].u_obj_name,obj[k].u_obj_name);
				strcpy(obj[*j].l_obj_name,obj[k].l_obj_name);
					obj[*j].scale_x = obj[k].scale_x;
					obj[*j].scale_y = obj[k].scale_y;
					obj[*j].scale_z = obj[k].scale_z;
					obj[*j].rev_x = obj[k].rev_x;
					obj[*j].rev_y = obj[k].rev_y;
					obj[*j].rev_z = obj[k].rev_z;

				strcpy(cls[*j].u_cls_name,cls[k].u_cls_name);
					cls[*j].ang_seq = cls[k].ang_seq;
					cls[*j].angle_x = cls[k].angle_x;
					cls[*j].angle_y = cls[k].angle_y;
					cls[*j].angle_z = cls[k].angle_z;
					cls[*j].point_x = cls[k].point_x;
					cls[*j].point_y = cls[k].point_y;
					cls[*j].point_z = cls[k].point_z;
					cls[*j].nest = cls[k].nest
						- cls[i].nest + p_nest;
					cls[*j].object = &obj[*j];
				}else{
					b_flg = 1;
					break;
				}
			}
			if ( b_flg == 1 ) break;
		}
	}

	/* Data Dump Section */
	/*printf("\n");
	for ( x = z; x < *j+1; x++ ){
		printf(" x = %d : u_obj_name = %s\n",x,obj[x].u_obj_name);
		printf(" x = %d : l_obj_name = %s\n",x,obj[x].l_obj_name);
		printf(" x = %d : rev_x = %d\n",x,obj[x].rev_x);
		printf(" x = %d : rev_y = %d\n",x,obj[x].rev_y);
		printf(" x = %d : rev_z = %d\n",x,obj[x].rev_z);
	}
	printf("\n");
	for ( x = z; x < *j+1; x++ ){
		printf(" x = %d : u_cls_name = %s\n",x,cls[x].u_cls_name);
		printf(" x = %d : ang_seq = %d\n",x,cls[x].ang_seq);
		printf(" x = %d : angle_x = %f\n",x,cls[x].angle_x);
		printf(" x = %d : point_y = %f\n",x,cls[x].point_y);
		printf(" x = %d : nest    = %d\n",x,cls[x].nest);
		printf(" x = %d : object = %s\n",x,cls[x].object->u_obj_name);
	}*/

}
