/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:時間管理
 * File		:pcm_time.c
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include <machine.h>
#include <string.h>
#include <sega_sys.h>
#include <sega_dma.h>
#include <sega_tim.h>
#include "pcm_msub.h"
#include "pcm_time.h"

/* ＣＰＵクロックによるタイマ */
static Sint32 pcm_count_32;

/* 前回に取得したＣＰＵクロック128分周値 */
static Uint16 pcm_last_count;


/*******************************************************************
【機　能】
	時間管理の初期化
【引　数】
	なし
【戻り値】
	なし
*******************************************************************/
void pcm_InitClock(void)
{
	TIM_FRT_INIT(PCM_TIM_MODE);
	pcm_count_32 = 0L;
	pcm_last_count = 0;
}

/*******************************************************************
【機　能】
	ＣＰＵクロックによるタイマ値を得る
【引　数】
	なし
【戻り値】
	ＣＰＵクロックを128で分周した値の、256分の１の値。
【注　意】
	ＣＰＵクロックの128分周値が16ビットを超えない頻度でコールされる必要
　がある。４ｋサンプル毎の割り込みだけでは、11kHzの場合に満足できない。
	ＰＡＬかＮＴＳＣか、高解像モードか否かにより、ＣＰＵクロックは異な
　るので、換算用マクロ値 PCM_CLOCK_SCALE を使用すること。
*******************************************************************/
Sint32 pcm_GetCount(void)
{
	Sint32		ret;
	Uint16		count;
	int			imask;

    imask = get_imask();
    set_imask(15);

	count = TIM_FRT_GET_16();
	if (count < pcm_last_count) {
		pcm_count_32 += 0x00000100;
	}
	pcm_last_count = count;
	ret = pcm_count_32 + ((count >> 8) & 0x000000FF);

    set_imask(imask);

	return ret;
}

/* ４ｋサンプル割り込み毎にスタートさせる */
void pcm_StartClock(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	int			imask;

    imask = get_imask();
    set_imask(15);

	st->count_start = pcm_GetCount();

    set_imask(imask);
}

/* 最近の４ｋサンプル割り込みからの経過時間を得る */
static Sint32 pcm_GetClock(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		clock;
	int			imask;

    imask = get_imask();
    set_imask(15);

	clock = pcm_GetCount() - st->count_start;

    set_imask(imask);

	return clock;
}

/*******************************************************************
【機　能】
	ＰＣＭスタートから現在までの経過時間の取得
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	ＰＣＭスタートから現在までの経過時間[sample]
*******************************************************************/
Sint32 pcm_GetTimePcm(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara		*para 	= &work->para;
#endif
	PcmStatus	*st 	= &work->status;
	Sint32		clock, sample;

	switch (st->play) {
	case PCM_STAT_PLAY_ERR_STOP:
	case PCM_STAT_PLAY_CREATE:
		return -1;
		break;
	case PCM_STAT_PLAY_PAUSE:
		clock = 0;
		break;
	case PCM_STAT_PLAY_START:
	case PCM_STAT_PLAY_HEADER:
	case PCM_STAT_PLAY_TIME:
	case PCM_STAT_PLAY_END:
		clock = pcm_GetClock(hn);
		break;
	default:
		clock = 0;
		break;
	}
	sample = st->cnt_4ksample * 4096 + 
			PCM_CALC_XY_DIVIDE_Z(clock, 
								st->info.sampling_rate, 
								st->clock_scale);
	sample &= 0xFFFFFFFC;

	return sample;
}

/*******************************************************************
【機　能】													公開関数
	フィルムの再生開始から現在までの経過時間の取得
【引　数】
	hn			(入力)　ハンドル
	sample_now	(出力)　フィルムの開始から現在までの総再生量[sample]
【戻り値】
	なし
【注　意】時間はＰＣＭ再生量をマスタクロックとして換算、調整された値
*******************************************************************/
void PCM_MeGetTime(PcmHn hn, Sint32 *sample_now)
{
	PcmWork		*work 	= *(PcmWork **)hn;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara		*para 	= &work->para;
#endif
	PcmStatus	*st 	= &work->status;

	switch (st->play) {
	case PCM_STAT_PLAY_ERR_STOP:
	case PCM_STAT_PLAY_CREATE:
		*sample_now = -1;
		break;
	case PCM_STAT_PLAY_START:
	case PCM_STAT_PLAY_HEADER:
		*sample_now = 0;
		break;
	case PCM_STAT_PLAY_PAUSE:
		*sample_now = st->sample_pause - st->sample_film_start;

		break;
	case PCM_STAT_PLAY_TIME:
	case PCM_STAT_PLAY_END:
		*sample_now = st->sample_pause + pcm_GetTimePcm(hn) 
						- st->sample_film_start;
		break;
	}
	return;
}

/*******************************************************************
【機　能】													公開関数
	連続再生の最初の再生開始から現在までの経過時間の取得
【引　数】
	hn			(入力)　ハンドル
	sample_now	(出力)　先頭ﾌｨﾙﾑからの現在までの総再生量[sample]
【戻り値】
	なし
【注　意】
*******************************************************************/
void PCM_MeGetTimeTotal(PcmHn hn, Sint32 *sample_now)
{
	PcmWork		*work 	= *(PcmWork **)hn;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara		*para 	= &work->para;
#endif
	PcmStatus	*st 	= &work->status;

	switch (st->play) {
	case PCM_STAT_PLAY_ERR_STOP:
	case PCM_STAT_PLAY_CREATE:
		*sample_now = -1;
		break;
	case PCM_STAT_PLAY_PAUSE:
		*sample_now = st->sample_pause;
		break;
	case PCM_STAT_PLAY_START:
	case PCM_STAT_PLAY_HEADER:
	case PCM_STAT_PLAY_TIME:
	case PCM_STAT_PLAY_END:
		*sample_now = st->sample_pause + pcm_GetTimePcm(hn);
		break;
	}
	return;
}
