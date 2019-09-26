/*------------------------------------------------------------------------
 *  FILE: mat_read.c
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
 *		DGT or RGB to 3D Table conversion program
 *
 *  DESCRIPTION:
 *		Read material information from SEGA3D file
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

#include "dgtto3dt.h"

void
mat_read()

{
	static	Sint8	buf[80];
	static	Sint8	keyword[120];
	static	Sint8	mat_name[16];
	static	Sint8	tex_name[16];
	Sint8	type[8][6];
	Sint16	c, i, j, kk, k0;
	Sint16	x;
	Sint16	nest, skip, mt_flg, flg;
	Sint16	m_bufno, tex_mode;
	Sint32	color;
	FILE	*fppi;

	/* ディレクトリ名と入力ファイル名の連結 */
	strcpy(g_s3dname,g_dirname);
	strcat(g_s3dname,"/");
	strcat(g_s3dname,g_outname);
	strcat(g_s3dname,".");
	strcat(g_s3dname,g_in_ext);
	/*printf("g_s3dname = %s\n",g_s3dname);*/

	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
		printf("dgtto3dt : Error : Input file not exist : Name = %s\n",g_s3dname);
		exit(0);
	}else{

	/* 文字列読み込み */
	i = 0;
	skip = 0;
	nest = 0;
	mt_flg = 0;
	m_bufno = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	/* 日本語文字検出 */
#ifdef __BORLANDC__
	if ( c >= 0x81 && c <= 0x9f || c >= 0xe0 && c <= 0xfc ){
#else
	if ( c >= 0x8e && c <= 0xfe && c != 0x8f && c != 0xa0 ){
#endif
		printf("dgtto3dt : Error : Japanese character exist\n");
	}

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/

		/* 空白行の排除 */
		if ( i > 0 ){

			/* material行の検出 */
			if ( nest == 0 ){

			for ( j = 0; j < 80; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"material") == 0 ){
					mt_flg = 1;
					tex_mode = 0;
					ex_keyword(j,buf,keyword);
					strcpy(mat_name,buf);
				/*printf("mat_name = %s/\n",mat_name);*/
				}else if ( strcmp(buf,"model") == 0 ||
					   strcmp(buf,"slips") == 0 ||
					   strcmp(buf,"object") == 0 ){
					mt_flg = 0;
				}
			}

			/* material行内の各パラメータの検出 */
			}else if ( mt_flg == 1 ){
				/*printf("      keyword(1) = %s/\n",keyword);*/

			for ( j = 0; j < 80; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"type") == 0 ){
					ex_keyword2(&k0,type,j,buf,keyword);
					for ( x = 0; x < k0; x++ ){
					/*printf(" type = %s/\n",type[x]);*/
						if (strcmp(type[x],"TEXT") == 0)
							tex_mode = 1;
					}

				}else if ( strcmp(buf,"texture") == 0 ){
					ex_keyword(j,buf,keyword);
					strcpy(tex_name,buf);
					/*printf(" texture = %s/\n",buf);*/

				}
			}

			}
		}
		skip = 0;
		i = 0;
	}else if ( skip == 0 && ( c == ' ' || c == TAB || c =='"') ){
	}else if ( c == ';' ){
		skip = 1;
	}else if ( skip == 0 && c == '{' ){
		nest++;
	}else if ( skip == 0 && c == '}' ){
		nest--;

		if ( mt_flg == 1 ){
		/* マテリアル情報のセット */
		if ( tex_mode == 0 ){
			/* テクスチャ以外の場合（何もしない） */
		}else{
			/* テクスチャの場合 */
			m_bufno++;
			if ( m_bufno > MBU_MAX ){
				printf("dgtto3dt : Error : Table Limit over : MBU_MAX\n");
				exit(0);
			}
			g_tex_max++;
			mbuf[m_bufno].mode = 3;
			strcpy(mbuf[m_bufno].mat_nam,mat_name);
			strcpy(mbuf[m_bufno].tex_nam,tex_name);

			/* type */
			mbuf[m_bufno].type = 0;
			for ( kk = 0; kk < k0; kk++ ){
				if ( strcmp(type[kk],"MESH") == 0 )
					mbuf[m_bufno].type += 1;
				else if ( strcmp(type[kk],"TRAN") == 0 )
					mbuf[m_bufno].type += 2;
				else if ( strcmp(type[kk],"SHAD") == 0 )
					mbuf[m_bufno].type += 4;
			}

		}
		}

	}else if ( skip == 0 ){
		keyword[i] = c;
		i++;
	}

	} /* while loop end */

	/* 入力ファイルクローズ */
	fclose ( fppi );
	}


	/* マテリアル情報からテクスチャ情報作成（重複の削除含む）*/
	g_tex_cnt = g_t_max;
	for ( i = 1; i < g_tex_max+1; i++ ){
		flg = 0;
		for ( j = 0; j < g_tex_cnt; j++ ){
			if ( strcmp(mbuf[i].tex_nam,txt[j].chr_nam) == 0 ){
				mbuf[i].mode = j;	/* 領域を一時的に流用 */
				flg = 1;
			}
		}
		if ( flg == 0 ){
			mbuf[i].mode = g_tex_cnt;	/* 領域を一時的に流用 */
			strcpy(txt[g_tex_cnt].chr_nam,mbuf[i].tex_nam);
			g_tex_cnt++;
			if ( g_tex_cnt > TEX_MAX ){
				printf("dgtto3dt : Error : Table Limit over : TEX_MAX\n");
				exit(0);
			}
		}
	}


	/* Data Dump Section */
	/*for ( i = 0; i < g_tex_cnt; i++ ){
		printf(" no = %d : tex_nam = %s\n",i,txt[i].chr_nam);
	}*/
}


void
ex_keyword(j,buf,keyword)
	Sint16	j;
	Sint8	*buf;
	Sint8	*keyword;
{
	Sint16	k, k1;

	k1 = 0;
	for ( k = j+1; k < 80; k++ ){
		buf[k1] = keyword[k];
		if ( keyword[k] == NULL ) break;
		k1++;
	}
}

void
ex_keyword2(k0,type,j,buf,keyword)
	Sint16	*k0;
	Sint16	j;
	Sint8	type[8][6];
	Sint8	*buf;
	Sint8	*keyword;
{
	Sint16	k, k1;

	*k0 = 0;
	k1 = 0;
	for ( k = j+1; k < 80; k++ ){
		if ( keyword[k] == '(' ){
		}else if ( keyword[k] == '|' ){
			buf[k1] = NULL;
			strcpy(type[*k0],buf);
			(*k0)++;
			k1 = 0;
		}else if ( keyword[k] == ')' ){
			buf[k1] = NULL;
			strcpy(type[*k0],buf);
			(*k0)++;
			break;
		}else{
			buf[k1] = keyword[k];
			k1++;
		}
	}
}
