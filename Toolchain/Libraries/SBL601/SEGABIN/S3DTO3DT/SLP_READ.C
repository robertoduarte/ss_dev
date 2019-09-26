/*------------------------------------------------------------------------
 *  FILE: slp_read.c
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
 *		Read slips information from SEGA3D file
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
slp_read( no )

	Sint16	no;

{
#ifdef __BORLANDC__
	static	Sint8 huge slp_face[SLP_MAX][16];
#else
	static	Sint8	slp_face[SLP_MAX][16];
#endif
	static	Sint8	buf[130];
	static	Sint8	keyword[130];
	static	Sint8	slp_name[16];
	Sint16	c, i, j, k, k1, kk, s;
	Sint16	x;
	Sint16	skip, nest;
	Sint16	m_bufno, sl_flg;
	Sint16	s_flag;
	Sint16	slp_max;
	FILE	*fppi;

/* printf("slp_read started ***\n\n");*/
printf("*** Slip Read\r" );
	/* 入力ファイルオープン */
	fppi = fopen ( g_s3dname, "rb" );
	if ( fppi == NULL ){
		printf("ERROR:(slip_read)\n");
		exit(0);
	}else{

	/* 文字列読み込み */
	i = 0;
	s = 0;
	skip = 0;
	nest = 0;
	sl_flg = 0;
	while ( ( c = getc(fppi) ) !=  EOF ){

	if ( c == CR_D || c == CR_A ){
		keyword[i] = NULL;
		/*printf("%3d : keyword(0) = %s/\n",i,keyword);*/
		if ( i > 0 ){
			/* slips行の検出 */
			if ( nest == 0 ){
			for ( j = 0; j < 80; j++ ){
				buf[j] = keyword[j];
				buf[j+1] = NULL;
				if ( strcmp(buf,"slips") == 0 ){
					kk = 0;
					for ( k = j+1; k < 80; k++ ){
						buf[kk] = keyword[k];
						if ( keyword[k] == NULL ) break;
						kk++;
					}
					strcpy(slp_name,buf);
					/* 対象モデル選定 */
					if( strcmp(obj[no].l_obj_name,slp_name) == 0 ){
						sl_flg = 1;
						/*printf("slp_name = %s/\n",slp_name);*/
					} else {
						sl_flg = 0;
					}
				}else if ( strcmp(buf,"model") == 0 ||
					   strcmp(buf,"material") == 0 ||
					   strcmp(buf,"object") == 0 ){
					sl_flg = 0;
				}
			}
			/* slips行内のパラメータの検出 */
			}else if ( sl_flg == 1 ){
					strcpy(slp_face[s],keyword);
					/*printf("slp_face[%d] = %s/\n",s,slp_face[s]);*/
					s++;
					if ( s > SLP_MAX ){
				 		printf("s3dto3dt : Error : Table Limit over :slp_face\n");
				 		exit(0);
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

	slp_max = s;

	/* シェーディング種別の強制指示チェック */
	s_flag = 1;
	if ( obj[no].shade == -1 ){
		obj[no].shade = 0;
		s_flag = 0;
	}
	/*for ( k = 0; k < g_shd_max; k++ ){
		if ( strcmp(obj[no].l_obj_name,shd[k].l_obj_name) == 0 ){
			obj[no].shade = shd[k].shade;
			s_flag = 1;
			break;
		}
	}*/

	/* マテリアル情報のセット */
	if ( slp_max > 0 ){

		for ( k = 0; k < slp_max; k++ ){

		/* バッファ検索 */
		m_bufno = -1;
		for ( k1 = 0; k1 < MBU_MAX; k1++ ){
			if ( strcmp(slp_face[k],mbuf[k1].mat_nam) == 0 )
				m_bufno = k1;
		}

		/* バッファにないとき */
		if ( m_bufno == -1 ){

			/* マテリアル情報の読み込み */
			mt_read2(&slp_face[k][0],&m_bufno);

		}

		/* 各面にマテリアル名をセット */
		strcpy(face[k].mat_nam,slp_face[k]);

		/* direction */
		face[k].direction = mbuf[m_bufno].direction;

		/* 面ごとにはセットできないパラメータはオブジェクト構造体へ */
		/* type ( NULL/FLAT/GOUR ) */
		if ( s_flag == 0 ){
			obj[no].shade = 0;
			if ( (mbuf[m_bufno].type&0x0003) == 0 )
				obj[no].shade = 0;
			else if ( (mbuf[m_bufno].type&0x0003) == 1 )
				obj[no].shade = 1;
			else if ( (mbuf[m_bufno].type&0x0003) == 2 )
				obj[no].shade = 2;
		}

		/* type ( POLYGON/POLYLINE ) */
		if ( (mbuf[m_bufno].type&0x0004) == 4 ) obj[no].p_type = 1;

		}

	}else{
		/* slips文がないとき（最初のマテリアルを採用）*/

		/* マテリアル情報の読み込み */
		mt_read2("",&m_bufno);

		for ( k = 0; k < g_face_max; k++ ){

		/* 各面にマテリアル名をセット */
		strcpy(face[k].mat_nam,mbuf[0].mat_nam);

		/* direction */
		face[k].direction = mbuf[0].direction;

		/* 面ごとにはセットできないパラメータはオブジェクト構造体へ */
		/* type ( NULL/FLAT/GOUR ) */
		if ( s_flag == 0 ){
			obj[no].shade = 0;
			if ( (mbuf[0].type&0x0003) == 0 )
				obj[no].shade = 0;
			else if ( (mbuf[0].type&0x0003) == 1 )
				obj[no].shade = 1;
			else if ( (mbuf[0].type&0x0003) == 2 )
				obj[no].shade = 2;
		}

		/* type ( POLYGON/POLYLINE ) */
		if ( (mbuf[0].type&0x0004) == 4 ) obj[no].p_type = 1;

		}

	}


	/* Data Dump Section */
	/*printf("\n");
	printf(" obj_name = %s\n",obj[no].l_obj_name);
	printf(" slp_max = %d\n",slp_max);
	for ( x = 0; x < slp_max; x++ ){
		printf(" x = %d : slp_face = %s\n",x,slp_face[x]);
	}*/
}
