/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ＡＰＣＭチャンク処理
 * File		:pcm_aif2.c
 * Date		:1994-12-08
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include "pcm_msub.h"
#include "pcm_aif.h"

extern void pcm_AudioProcessAdpcm(PcmHn hn);

/*******************************************************************
【機　能】
	メモリ再生／１チャンク処理：Adpcm Chunk のADPCM対応処理
【引　数】
	なし
【戻り値】
	なし
【備　考】
	関数ポインタ pcm_chunk_adpcm_fp に設定されてコールされる
*******************************************************************/
void pcm_ChunkAdpcm(PcmHn hn, PcmAdpcmChunk *chunk)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;

	st->media_offset = (Sint32)chunk + 4*4 - (Sint32)para->ring_addr;

	st->info.data_type = PCM_DATA_TYPE_ADPCM_SCT;

	/* サンプリングビット数 */
	/* file ni ha 4[bit/sample] to kai te a ru */
	st->info.sampling_bit = 16;		/* 16 [bit/sample] */

	/* オーディオ処理関数ポインタの設定 */
	st->audio_process_fp = pcm_AudioProcessAdpcm;
}
