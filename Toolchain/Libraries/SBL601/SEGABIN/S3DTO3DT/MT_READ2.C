/*------------------------------------------------------------------------
 *  FILE: mt_read2.c
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
 *		Read material information for each model from SEGA3D file
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
mt_read2(mat_name,m_bufno)

	Sint8	*mat_name;
	Sint16	*m_bufno;

{
	static	Sint8	buf[80];
	static	Sint8	keyword[120];
	static	Sint8	mat_nam2[16];
	Sint8	type[8][6];
	Sint16	direct[4];
	Sint16	c, i, j, k, kk;
	Sint16	k0;
	Sint16	x;
	Sint16	skip, nest, mt_flg;
	FILE	*fppi;

/*printf("mt_read2 started *** : mat_name = %s\n\n",mat_name);*/
	/* バッファ番号の選定 */
	g_mb_no++;
	if ( g_mb_no == MBU_MAX ) g_mb_no = 0;
	*m_bufno = g_mb_no;
	strcpy(mbuf[*m_bufno].mat_nam,mat_name);

	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
/*		printf("ERROR(mt_read2)\n");*/
		exit(0);
	}else{

	/* 文字列読み込み */
	i = 0;
	skip = 0;
	nest = 0;
	mt_flg = 0;
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
					ex_keyword(j,buf,keyword);
					strcpy(mat_nam2,buf);
					/* 対象モデル選定 */
					if( strcmp(mat_name,"") == 0 ){
						strcpy(mbuf[*m_bufno].mat_nam,
							mat_nam2);
						mt_flg = 1;
					}
					if( strcmp(mat_name,mat_nam2) == 0 ){
						mt_flg = 1;
					} else {
						mt_flg = 0;
					}
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
				if ( strcmp(buf,"type") == 0 ) {
					ex_keyword2(&k0,type,j,buf,keyword);
				}else if ( strcmp(buf,"direction") == 0 ) {
					ex_keyword3(direct,j,buf,keyword);
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

	/* バッファ構造体へのセット */
	/* type ( NULL/FLAT/GOUR/WIRE ) */
	mbuf[*m_bufno].type = 0;
	for ( kk = 0; kk < k0; kk++ ){
		if ( strcmp(type[kk],"NULL") == 0 )
			mbuf[*m_bufno].type += 0;
		else if ( strcmp(type[kk],"FLAT") == 0 )
			mbuf[*m_bufno].type += 1;
		else if ( strcmp(type[kk],"GOUR") == 0 )
			mbuf[*m_bufno].type += 2;
		else if ( strcmp(type[kk],"WIRE") == 0 )
			mbuf[*m_bufno].type += 4;
	}

	/* direction */
	if ( direct[0]==0 && direct[1]==1 && direct[2]==2 && direct[3]==3 )
		mbuf[*m_bufno].direction = 0;
	if ( direct[0]==1 && direct[1]==2 && direct[2]==3 && direct[3]==0 )
		mbuf[*m_bufno].direction = 1;
	if ( direct[0]==2 && direct[1]==3 && direct[2]==0 && direct[3]==1 )
		mbuf[*m_bufno].direction = 2;
	if ( direct[0]==3 && direct[1]==0 && direct[2]==1 && direct[3]==2 )
		mbuf[*m_bufno].direction = 3;
	if ( direct[0]==0 && direct[1]==3 && direct[2]==2 && direct[3]==1 )
		mbuf[*m_bufno].direction = 4;
	if ( direct[0]==1 && direct[1]==0 && direct[2]==3 && direct[3]==2 )
		mbuf[*m_bufno].direction = 5;
	if ( direct[0]==2 && direct[1]==1 && direct[2]==0 && direct[3]==3 )
		mbuf[*m_bufno].direction = 6;
	if ( direct[0]==3 && direct[1]==2 && direct[2]==1 && direct[3]==0 )
		mbuf[*m_bufno].direction = 7;

	/* Data Dump Section */
	/*printf("\n");
	printf(" no = %d : mat_nam = %s\n",*m_bufno,mbuf[*m_bufno].mat_nam);
	printf(" no = %d : type    = %d\n",*m_bufno,mbuf[*m_bufno].type);
	printf(" no = %d : direct  = %d\n",*m_bufno,mbuf[*m_bufno].direction);
	printf(" direct = %d : %d : %d : %d\n",
				direct[0],direct[1],direct[2],direct[3]);*/
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
	Sint8	type[8][6];
	Sint16	j;
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

void
ex_keyword3(direct,j,buf,keyword)
	Sint16	*direct;
	Sint16	j;
	Sint8	*buf;
	Sint8	*keyword;
{
	Sint16	k, k2, k3;

	k2 = 0;
	k3 = 0;
	for ( k = j+1; k < 80; k++ ){
		if ( keyword[k] == ',' ){
			buf[k3] = NULL;
			direct[k2] = atoi(buf);
			k2++;
			k3 = 0;
		}else if ( keyword[k] == NULL ){
			buf[k3] = NULL;
			direct[k2] = atoi(buf);
			k2++;
			break;
		}else{
			buf[k3] = keyword[k];
			k3++;
		}
	}
}
