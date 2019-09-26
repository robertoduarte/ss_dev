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
 *		SEGA3D to 3DTable conversion program
 *
 *  DESCRIPTION:
 *		Read material information for header from SEGA3D file
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
mat_read()

{
	static	Sint8	buf[80];
	static	Sint8	keyword[120];
	static	Sint8	mat_name[16];
	static	Sint8	tex_name[16];
	Sint8	type[8][6];
	Sint16	c, i, j, kk, k0;
	Sint16	x;
	Sint16	nest, skip, mt_flg;
	Sint16	m_bufno, tex_mode;
	Sint32	color;
	FILE	*fppi;

/*printf("mat_read started ***\n\n");*/
	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
	printf("ERROR(mat_read)\n");
		exit(0);
	}else{

	/* 文字列読み込み */
	i = 0;
	skip = 0;
	nest = 0;
	mt_flg = 0;
	m_bufno = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

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

				}else if ( strcmp(buf,"color") == 0 ){
					ex_keyword(j,buf,keyword);
					sscanf(buf,"%*c%*c%4x",&color);
					/*printf(" color = %x\n",color);*/
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

		if ( mt_flg == 1 ){
		/* マテリアル情報のセット */
		if ( tex_mode == 0 ){
			/* テクスチャ以外の場合 */
			mbuf[0].mode = 0;
			strcpy(mbuf[0].mat_nam,mat_name);

			/* type */
			mbuf[0].type = 0;
			for ( kk = 0; kk < k0; kk++ ){
				if ( strcmp(type[kk],"MESH") == 0 )
					mbuf[0].type += 1;
				else if ( strcmp(type[kk],"TRAN") == 0 )
					mbuf[0].type += 2;
				else if ( strcmp(type[kk],"SHAD") == 0 )
					mbuf[0].type += 4;
			}

			/* color */
			mbuf[0].color = color;

			/* ドロー、カラー情報書き出し */
			drw_wrt();

		}else{
			/* テクスチャの場合 */
			m_bufno++;
			if ( m_bufno > MBU_MAX ){
				printf("s3dto3dt : Error : Table Limit over : MBU_MAX\n");
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

			/* color */
			mbuf[m_bufno].color = color;
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

	/* ドロー、カラー情報書き出し（テクスチャが有るとき） */
	if ( g_tex_max > 0 ) tex_wrt();

	/* バッファ初期化（mt_read2で再利用するため） */
	for ( kk = 0; kk < MBU_MAX; kk++ ) strcpy(mbuf[kk].mat_nam,"");


	/* Data Dump Section */
}
