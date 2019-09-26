/*-----------------------------------------------------------------------------
 *  FILE: smpmem1.c
 *
 *  Copyright(c) 1995 SEGA
 *
 *  PURPOSE:
 *
 *      メモリサンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      メモリ管理を実行します。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1995-12-04  H.O Ver.0.90
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_mem.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

/******************************************************************************
 *
 * NAME:    main()      - メイン
 *
 * PARAMETERS :
 *      なし
 *
 * DESCRIPTION:
 *      メイン
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define	SZ_HEAP	0x1001
static unsigned char	__heap[SZ_HEAP];
char	sega[] = "SEGA";

void main(void){
	Uint32	biggest,ui;
	void	*ptr,*tmp;
	int	i,maxcell[32];
	char	*pc;

	/* 0から32バイトで最大いくつブロックが確保できるか */
	for( i = 0 ; i < 32 ; i++){
		maxcell[i] = 0;
	    MEM_Init( ( Uint32 )__heap, sizeof( __heap ));
		while( ( (ptr = MEM_Malloc( i )) != NULL ) )
			maxcell[i]++;
	}

	/* 確保可能な最大サイズは幾つか */
    MEM_Init( ( Uint32 )__heap, sizeof( __heap ));
	for( biggest = SZ_HEAP+15;
	(ptr = MEM_Malloc((Uint32)biggest)) == NULL;
	biggest-- )
		;
	MEM_Free( ptr );

	/* 再度確保可能か */
	while( (ptr = MEM_Malloc((Uint32)biggest)) == NULL )
		;
	MEM_Free( ptr );

	/* 連続して確保 */
	tmp = NULL;
	for( i = 0;; i++ ){
		if( (ptr = MEM_Malloc((Uint32)(sizeof(void *)))) == NULL )
			break;
		else{
			*(void **)ptr = tmp;
			tmp = ptr;
		}
	}
	/* 連続して開放 */
	for( ptr = tmp ; ptr != NULL ; i--){
		tmp = *(void **)ptr;
		MEM_Free(ptr);
		ptr = tmp;
	}

	/* 配列の確保 */
	while( (ptr = MEM_Calloc((Uint32)biggest/8,(Uint32)8)) == NULL )
		;
	for( ui = 0 ; ui < biggest ; ui++){
		pc = (char *)ptr;
		while( pc[ui] != 0 )
			;
	}
	MEM_Free( ptr );

	/* 連続して再割り付け */
	ptr = MEM_Malloc((Uint32)(sizeof(char[4])));
	for( i = 0 ; i < 4 ; i ++ ){
#if 1
		pc = (char *)ptr;
		pc[i] = sega[i];
#else
		*(char *)(ptr + i) = sega[i];
#endif
	}
	for( i = 0 ; i < 0x100 ; i++ )
		ptr = MEM_Realloc( ptr, (Uint32)sizeof(char[4]) );

	/* ここまでくれば終了 */
	for(;;)
		;
}
