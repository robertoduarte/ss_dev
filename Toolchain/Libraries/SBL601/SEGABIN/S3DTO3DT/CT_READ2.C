/*------------------------------------------------------------------------
 *  FILE: ct_read2.c
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
 *		Read option parameter from .ctl file ( 2nd. time for model )
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
ct_read2()

{
	static	Sint8	obj_name[16];
	static	Sint8	buf[80];
	static	Sint8	ctlname[80];
	static	Sint8	keyword[120];
	Sint16	c, i, j, k, k1, m;
	Sint16	x;
	Sint16	nest, skip;
	Sint16	pos;
	Sint16	shade, ang_seq;
	FILE	*fppi;

/*printf("ctl_read started ***\n\n");*/
	/* ディレクトリ名とオプションファイル名の連結 */
	strcpy(ctlname,g_dirname);
	strcat(ctlname,"/");
	strcat(ctlname,g_outname);
	strcat(ctlname,".ctl");

	/* オプションファイルオープン */
	fppi = fopen ( ctlname, "rb" );
	if ( fppi == NULL ){
		return;
	}else{

	/* 文字列読み込み */
	i = 0;
	skip = 0;
	nest = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/

	if ( i > 0 ){

	/* 各パラメータの検出 */
	/*printf("      keyword(1) = %s/\n",keyword);*/
	for ( j = 0; j < 80; j++ ){
		buf[j] = keyword[j];
		buf[j+1] = NULL;
		if ( strcmp(buf,"shading") == 0 ){
			pos = 0;
			k1 = 0;
			for ( k = j+1; k < 80; k++ ){
				if ( keyword[k] == '(' ||
				     keyword[k] == ',' ||
				     keyword[k] == ')' ||
				     keyword[k] == NULL ){
					buf[k1] = NULL;
/*printf("pos = %d : buf = %s/\n",pos,buf);*/
					if ( pos == 0 )
						strcpy(obj_name,buf);
					else if ( pos == 1 ){

					if ( strcmp(buf,"no") == 0 ){
						shade = 0;
					}else if ( strcmp(buf,"flat") == 0 ){
						shade = 1;
					}else if ( strcmp(buf,"gouraud") == 0 ){
						shade = 2;
					}

					for ( m = 0; m < g_u_obj_max; m++ ){
						if ( strcmp(obj_name,"*") == 0 || strcmp(obj_name,obj[m].l_obj_name) == 0 ){
							obj[m].shade = shade;
						}
					}
					}
					pos++;
					k1 = 0;
					if (keyword[k] == NULL){
						break;
					}
				}else{
					buf[k1] = keyword[k];
					k1++;
				}
			}

		}else if ( strcmp(buf,"rotation_seq") == 0 ){
			pos = 0;
			k1 = 0;
			for ( k = j+1; k < 80; k++ ){
				if ( keyword[k] == '(' ||
				     keyword[k] == ',' ||
				     keyword[k] == ')' ||
				     keyword[k] == NULL ){
					buf[k1] = NULL;
					if ( pos == 0 ){
						strcpy(obj_name,buf);
					}else if ( pos == 1 ){

					if ( strcmp(buf,"zyx") == 0 ){
						ang_seq = 0;
					}else if ( strcmp(buf,"zxy") == 0 ){
						ang_seq = 1;
					}else if ( strcmp(buf,"yzx") == 0 ){
						ang_seq = 2;
					}else if ( strcmp(buf,"yxz") == 0 ){
						ang_seq = 3;
					}else if ( strcmp(buf,"xyz") == 0 ){
						ang_seq = 4;
					}else if ( strcmp(buf,"xzy") == 0 ){
						ang_seq = 5;
					}
					for ( m = 0; m < g_u_obj_max; m++ ){
						if ( strcmp(obj_name,obj[m].l_obj_name) == 0 ){
							cls[m].ang_seq = ang_seq;
						}
					}

					}
					pos++;
					k1 = 0;
					if (keyword[k] == NULL){
						break;
					}
				}else{
					buf[k1] = keyword[k];
					k1++;
				}
			}
		}

		} /* end of for j = 0 */

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
	}else if ( skip == 0 ){
		keyword[i] = c;
		i++;
	}

	} /* while loop end */

	/* 入力ファイルクローズ */
	fclose ( fppi );
	}


	/* Data Dump Section */
	/*for ( x = 0; x < g_u_obj_max; x++ ){
		printf("obj[%d].shade = %d\n",x,obj[x].shade);
	}*/

}
