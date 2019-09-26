//------------------------------------------------------------------------
//
//	CINIT.C
//	C Startup module for Saturn/SGL apps
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Unknown
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Header added for demo release
//
//------------------------------------------------------------------------

/* 
    C言語で必要な初期化処理サンプルルーチン(SGLサンプルプログラム用)
      1.BSS(未初期化領域)のクリア
      2.ROM領域からRAM領域へのコピー(存在しない場合はいらない)
*/

#include	"sgl.h"

/* sl.lnk で指定した.bssセクションの開始、終了シンボル */
extern Uint32 _bstart, _bend;
/* */
extern void ss_main( void );

void	main( void )
{
	Uint8	*dst;

	/* Zero Set .bss Section */
	for( dst = (Uint8 *)&_bstart; dst < (Uint8 *)&_bend; dst++ ) {
		*dst = 0;
	}
	/* ROM has data at end of text; copy it. */

	/* Application Call */
	ss_main();
}

