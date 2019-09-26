/*------------------------------------------------------------------------
 *  FILE: ctl_read.c
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
 *		Read option parameter from .ctl file
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
ctl_read()

{
	static	Sint8	buf[80];
	static	Sint8	ctlname[80];
	static	Sint8	keyword[120];
	Sint16	c, i, j, k, k1;
	Sint16	x;
	Sint16	nest, skip;
	Sint16	pos, cnt_t;
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
	cnt_t = 0;
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
				if ( strcmp(buf,"version") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_version,buf);

				}else if ( strcmp(buf,"scale") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_scale,buf);

				}else if ( strcmp(buf,"face_reverse") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"yes") == 0 ){
						g_face_rev = 1;
					}else{
						g_face_rev = 0;
					}

				}else if ( strcmp(buf,"normal") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"yes") == 0 ){
						g_n_calc = 1;
					}else{
						g_n_calc = 0;
					}

				}else if ( strcmp(buf,"cluster") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"no") == 0 ){
						g_clstr = 0;
					}else{
						g_clstr = 1;
						strcpy(g_top_cls,buf);
					}

				}else if ( strcmp(buf,"surface_ctr") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"yes") == 0 ){
						g_ctr_c = 1;
					}else{
						g_ctr_c = 0;
					}

				}else if ( strcmp(buf,"direction") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"ccw") == 0 ){
						g_s_drt = 1;
					}else{
						g_s_drt = 0;
					}

				}else if ( strcmp(buf,"input") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_in_ext,buf);

				}else if ( strcmp(buf,"output") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					strcpy(g_out_ext,buf);

				}else if ( strcmp(buf,"tex_info") == 0 ){
					pos = 0;
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						if ( keyword[k] == '(' ||
						     keyword[k] == ',' ||
						     keyword[k] == ')' ||
						     keyword[k] == NULL ){
							buf[k1] = NULL;
/*printf("cnt_t = %d : pos = %d : buf = %s/\n",cnt_t,pos,buf);*/
							if ( pos == 0 )
						strcpy(txt[cnt_t].chr_nam,buf);
							else if ( pos == 1 )
						strcpy(txt[cnt_t].clr_mod,buf);
							else if ( pos == 3 )
						txt[cnt_t].width = atoi(buf);
							else if ( pos == 6 )
						txt[cnt_t].height = atoi(buf);
							else if ( pos == 8 )
						strcpy(txt[cnt_t].lup_ptr,buf);
							pos++;
							k1 = 0;
							if (keyword[k] == NULL){
								cnt_t++;
								break;
							}
						}else{
							buf[k1] = keyword[k];
							k1++;
						}
					}

				}else if ( strcmp(buf,"dgt_txc") == 0 ){
					k1 = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[k1] = keyword[k];
						if ( keyword[k] == NULL ) break;
						k1++;
					}
					if ( strcmp(buf,"no") == 0 ){
						g_dgt = 0;
					}else{
						g_dgt = 1;
						strcpy(g_dgt_nam,buf);
					}
				}

			}

		}
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

	g_t_max = cnt_t;


	/* Data Dump Section */
	/*printf(" g_scale    = %s\n",g_scale);
	printf(" g_face_rev = %d\n",g_face_rev);
	printf(" g_n_calc   = %d\n",g_n_calc);
	printf(" g_clstr    = %d : name = %s\n",g_clstr,g_top_cls);
	printf(" g_ctr_c    = %d\n",g_ctr_c);
	printf(" g_s_drt    = %d\n",g_s_drt);
	printf(" g_in_ext   = %s\n",g_in_ext);
	printf(" g_out_ext  = %s\n",g_out_ext);
	for ( x = 0; x < cnt_t; x++ ){
		printf(" txt[%d].chr_nam = %s\n",x,txt[x].chr_nam);
		printf(" txt[%d].clr_mod = %s\n",x,txt[x].clr_mod);
		printf(" txt[%d].width   = %d\n",x,txt[x].width);
		printf(" txt[%d].height  = %d\n",x,txt[x].height);
		printf(" txt[%d].lup_ptr = %s\n",x,txt[x].lup_ptr);
	}*/

}
