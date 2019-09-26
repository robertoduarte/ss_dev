/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:時間管理
 * File		:pcm_time.h
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_TIME_H)
#define PCM_TIME_H


/*******************************************************************
【機　能】
	システムプログラムを使用して、システムクロック値を識別する
【引　数】
	なし
【戻り値】
	1		: 28MHz
	0		: 26MHz
*******************************************************************/
#define PCM_IS_26MHZ		(SYS_GETSYSCK == 0)

/*******************************************************************
【機　能】
	VDP2の画面ステータスレジスタを見て、NTSC, PAL を識別する
【引　数】
	なし
【戻り値】
	1		: PAL
	0		: NTSC
*******************************************************************/
#define PCM_IS_PAL			(*(volatile Uint16 *)0x25f80004 & 0x0001)

/*******************************************************************
【機　能】
	VDP2のＴＶ画面モードレジスタを見て、interlace, non-interlace 
	を識別する
【引　数】
	なし
【戻り値】
	1		: interlace
	0		: non-interlace
*******************************************************************/
#define PCM_IS_INTERLACE	(*(volatile Uint16 *)0x25f80004 & 0x00c0)

/*******************************************************************
【機　能】
	Ｖブランク割り込みの周波数を得る[Hz]
【引　数】
	なし
【戻り値】
	Ｖブランク割り込みの周波数
*******************************************************************/
/* Ｖｂｌ周波数。16bit 右シフトすると [Hz] になる値。(奥ノ木さん) */
#define PCM_FREQ_VBL_NTSC_N	(0x3BD374)		/* 59.8261   *256*256[Hz] */
#define PCM_FREQ_VBL_NTSC_I	(0x3BF0A3)		/* 29.9700 *2*256*256[Hz] */
#define PCM_FREQ_VBL_PAL_N	(0x31EB8B)		/* 49.9201   *256*256[Hz] */
#define PCM_FREQ_VBL_PAL_I	(0x320000)		/* 25.0000 *2*256*256[Hz] */
#define PCM_FREQ_VBL_NTSC	(PCM_IS_INTERLACE ? 							\
								PCM_FREQ_VBL_NTSC_I : PCM_FREQ_VBL_NTSC_N)
#define PCM_FREQ_VBL_PAL	(PCM_IS_INTERLACE ? 							\
								PCM_FREQ_VBL_PAL_I : PCM_FREQ_VBL_PAL_N)
#define PCM_FREQ_VBL		(PCM_IS_PAL ? PCM_FREQ_VBL_PAL : PCM_FREQ_VBL_NTSC)

/*******************************************************************
【機　能】
	ＣＰＵクロック周波数を調べる[Hz]
【引　数】
	なし
【戻り値】
	ＣＰＵクロック周波数
*******************************************************************/
#define PCM_CLK_CPU_NTSC26	(26874100L)
#define PCM_CLK_CPU_NTSC28	(28636400L)
#define PCM_CLK_CPU_PAL26	(26687500L)
#define PCM_CLK_CPU_PAL28	(28437500L)
#define PCM_CLK_CPU_NTSC	(PCM_IS_26MHZ ? PCM_CLK_CPU_NTSC26 : PCM_CLK_CPU_NTSC28)
#define PCM_CLK_CPU_PAL		(PCM_IS_26MHZ ? PCM_CLK_CPU_PAL26 : PCM_CLK_CPU_PAL28)
#define PCM_CLK_CPU			(PCM_IS_PAL ? PCM_CLK_CPU_PAL : PCM_CLK_CPU_NTSC)

/* タイマライブラリ分周設定値 */
#define PCM_TIM_MODE		(2)		/* モード:分周値, 0:8, 1:32, 2:128 */

/* 4sha5nyuu migi shift */
#define PCM_ROUND_SHIFT_R(a, n)		((((a) >> ((n) - 1)) + 1) >> 1)

/*******************************************************************
【機　能】
	pcm_GetClock で得たクロック値から秒へ換算するスケール値
【メ　モ】
	>> 7 は、分周値 128 でのわり算を表す。
	>> 8 は、それの 256 分の１の値をクロック値とする換算を表す。
*******************************************************************/
/* #define PCM_CLOCK_SCALE		(PCM_CLK_CPU >> (7 + 8)) */
#define PCM_CLOCK_SCALE		(PCM_ROUND_SHIFT_R(PCM_CLK_CPU, 7 + 8))


extern Sint32 pcm_GetCount(void);
extern void pcm_InitClock(void);
extern Sint32 pcm_GetTimePcm(PcmHn hn);

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ほかのファイルから参照されているので追加
	*/
	void	pcm_StartClock( PcmHn	hn );
#endif

#endif	/* PCM_TIME_H */
