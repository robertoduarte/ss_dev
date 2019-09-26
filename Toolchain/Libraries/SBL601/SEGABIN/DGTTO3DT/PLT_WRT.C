/*------------------------------------------------------------------------
 *  FILE: plt_wrt.c, plt_wrt2.c
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
 *		Write palette data
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
plt_wrt(no)	/* パレット分離モード用 */

	Sint16	no;

{
	static	Sint8	string[120];
	static	Sint8	buf[100];
	Sint16	i, j, k;
	Uint16	plt_no[30];
	Sint16	plt_max_no;
	Sint16	cnt, flg;
	Sint16	x;
	FILE	*fppo;

	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

		/* パレット情報書き出し */
		strcpy(string,"static Uint16  ");
		strcat(string,g_col_tbl);
		strcat(string,"_");
		sprintf(buf,"%d",no);
		strcat(string,buf);
		strcat(string,"[] = {\n");
		fputs(string,fppo);

		for ( j = 0; j < g_plt_max; j+=2 ){
			if ( g_pltdata[j+1] == 0x8000 ){
				g_pltdata[j+1] = 0x0;
			}else{
				g_pltdata[j+1] = g_pltdata[j+1]|0x8000;
			}
			if ( j == g_plt_max-2 ){
				strcpy(string,"/* ");
				sprintf(buf,"%-4d(0x%-4x)",j/2, j/2);
				strcat(string,buf);
				strcat(string," */ ");
				sprintf(buf,"0x%x\n",g_pltdata[j+1]);
				strcat(string,buf);
				fputs(string,fppo);
			}else{
				strcpy(string,"/* ");
				sprintf(buf,"%-4d(0x%-4x)",j/2, j/2);
				strcat(string,buf);
				strcat(string," */ ");
				sprintf(buf,"0x%x,\n",g_pltdata[j+1]);
				strcat(string,buf);
				fputs(string,fppo);
			}
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* 出力ファイルクローズ */
		fclose ( fppo );

	}


	/* Data Dump Section */
	/*printf("plt_max_no = %d\n",plt_max_no);
	for ( x = 0; x < g_plt_max; x+=2 ){
		printf("plt_no = %x : color = %x : color(RGB) = %d\n",
			g_pltdata[x],g_pltdata[x+1],(g_pltdata[x+1]|0x8000));
	}*/
}


void
plt_wrt2()	/* パレット合体モード用 */
{
	static	Sint8	string[120];
	static	Sint8	buf[100];
	Sint16	i, j, k;
	Uint16	plt_no[30];
	Sint16	plt_max_no;
	Sint16	cnt, flg;
	Sint16	x;
	FILE	*fppo;

	/* 出力ファイルオープン（追加モード） */
	fppo = fopen ( g_c_outname, "a" );
	if ( fppo != NULL ){

		/* パレット最大番号抽出 */
		plt_max_no = 0;
		for ( j = 0; j < g_plt_max; j+=2 ){
			if ( g_pltdata[j] > plt_max_no )
				plt_max_no = g_pltdata[j];
		}
		if ( plt_max_no > g_col_lim ){
		printf("dgtto3dt : Error : Color palette over %d\n",g_col_lim);
			plt_max_no = g_col_lim;
		}

		/* パレット情報書き出し */
		strcpy(string,"static Uint16  ");
		strcat(string,g_col_tbl);
		strcat(string,"[] = {\n");
		fputs(string,fppo);

		for ( i = 0; i < plt_max_no; i++ ){

			/* パレットの検索 */
			cnt = 0;
			for ( j = 0; j < g_plt_max; j+=2 ){
				if ( i == g_pltdata[j] &&
						g_pltdata[j+1] != 0x7fff ){
					flg = 0;
					for ( k = 0; k < cnt; k++ ){
						if(plt_no[k] == g_pltdata[j+1]){
							flg = 1;
							break;
						}
					}
					if ( flg == 0 ){
						plt_no[cnt] = g_pltdata[j+1];
						cnt++;
					}
				}
			}

			/* 使用パレットの場合 */
			if ( cnt > 0 ){
				if ( cnt > 1 ){
					printf("dgtto3dt : Error : Palette No. doubly assigned : No = %d\n",i);
				} 
				plt_no[0] = plt_no[0]|0x8000;
				if ( i == plt_max_no-1 ){
					strcpy(string,"/* ");
					sprintf(buf,"%-4d(0x%-4x)",i,i);
					strcat(string,buf);
					strcat(string," */ ");
					sprintf(buf,"0x%x\n",plt_no[0]);
					strcat(string,buf);
					fputs(string,fppo);
				}else{
					strcpy(string,"/* ");
					sprintf(buf,"%-4d(0x%-4x)",i,i);
					strcat(string,buf);
					strcat(string," */ ");
					sprintf(buf,"0x%x,\n",plt_no[0]);
					strcat(string,buf);
					fputs(string,fppo);
				}

			/* 未使用パレットの場合 */
			}else{
				strcpy(string,"/* ");
				sprintf(buf,"%-4d(0x%-4x)",i,i);
				strcat(string,buf);
				strcat(string," */ ");
				sprintf(buf,"0xffff,\n");
				strcat(string,buf);
				fputs(string,fppo);
			}
		}
		fputs("};\n",fppo);
		fputs("\n",fppo);

		/* 出力ファイルクローズ */
		fclose ( fppo );

	}


	/* Data Dump Section */
/*    printf("plt_max_no = %d\n",plt_max_no);
	for ( x = 0; x < g_plt_max; x+=2 ){
		printf("plt_no = %x : color = %x : color(RGB) = %d\n",
			g_pltdata[x],g_pltdata[x+1],(g_pltdata[x+1]|0x8000));
	}*/
}
