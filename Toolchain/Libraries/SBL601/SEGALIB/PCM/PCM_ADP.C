/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ADPCM buffer management, decompression, PCM data copy
 * File		:pcm_adp.c
 * Date		:1994-10-14
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include <string.h>
#include <sega_adp.h>
#include "pcm_msub.h"

#ifdef ADPCM_FUNCTION

/* ADPCMテスト */
/* #define NEW_ADPCM_TEST */

#ifdef NEW_ADPCM_TEST
	Sint32 cnt_pcm_top = 0;

	void pcm_adp_break(void){}
	
#endif

/*******************************************************************
【機　能】
	メモリ再生／ＡＤＰＣＭオーディオ処理 (連続したサウンドグループ)
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【備　考】
	サウンドグループはリングバッファ上で連続している保証はない。

		+-------------------------------------------------------+
		|					リングバッファ						|
		+-------------------------------------------------------+
											..... S------G, S--- 
		---G, S------G, .....					  <------>
												サウンドグループ
*******************************************************************/
void pcm_AudioAdpcmSg(PcmHn hn)
{
	return;
}

#define PCM_CHECK_SAMPLE_FILE(st)	\
			if (st->info.sample_file != -1) {	\
				if (st->sample_write_file >= st->info.sample_file) {	\
					break;	\
				}	\
			}

#define PCM_SIZE_PAD	(PCM_SIZE_SECTOR - (PCM_SG_IN_SECTOR*ADP_SIZE_SG_CODE))

/*******************************************************************
【機　能】
	メモリ再生／ＡＤＰＣＭオーディオ処理 (セクタバウンダリのサウンドグループ)
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【備　考】
	１８サウンドグループ毎に２０バイトのパッドを読み飛ばす。
	サウンドグループはリングバッファ上で連続している保証はない。
*******************************************************************/
void pcm_AudioAdpcmSct(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	Sint8 		*addr_w1, *addr_w2;	/* PCMﾊﾞｯﾌｧ書き込みアドレス 			*/
	Sint32		size_w, size_w_t; 	/* PCMﾊﾞｯﾌｧ書き込み可能サイズ [byte] 	*/
	Sint32		ring_bsize, 		/* ﾘﾝｸﾞﾊﾞｯﾌｧの在庫サイズ [byte] 		*/
				ring_sg, 		/* ﾘﾝｸﾞﾊﾞｯﾌｧの在庫ｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数[sect/1ch]	*/
				pcm_sg, 		/* PCMﾊﾞｯﾌｧの受け入れｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数[sect/1ch]*/
				copy_sg, 		/* 実行コピーｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数 [sect/1ch] 		*/
				size_1, size_2, 	/* コピーサイズ [byte/1ch] 				*/
				dsize_1, dsize_2,	/* コピーサイズ [dword/1ch] 			*/
				j;
	Sint32		flop32[ADP_SIZE_SG_CODE / 4];
	Sint32		buf32[ADP_SIZE_SG_DECO / 4];
	Sint8		*flop = (Sint8 *)flop32;
	Sint8		*buf = (Sint8 *)buf32;
	Sint8		*buf_r = buf + ADP_SIZE_SG_DECO_1;
	Sint8		*buf_l = buf;
	Sint8		*input_addr;		/* リングバッファ読み取りアドレス 		*/
/*	Sint32 		sample_now; */


	/* CD-ROM XA : リングバッファには EOF 以降のゴミは供給しない */
	ring_bsize = st->ring_write_offset - st->ring_read_offset;

	/* 在庫量をｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数に換算する [sg/1ch] */
	ring_sg = (ring_bsize / PCM_SIZE_SECTOR) * PCM_SG_IN_SECTOR
			+ (ring_bsize % PCM_SIZE_SECTOR) / ADP_SIZE_SG_CODE;
	if (PCM_IS_STEREO(st)) {
		ring_sg >>= 1;
	}

	/* ADPCM ENCODER */
	if (st->info.sample_file != -1) {
		if (st->sample_write_file >= st->info.sample_file) {
			ring_sg = 0;
		}
	}

	pcm_GetPcmWrite(hn, &addr_w1, &addr_w2, 
						&size_w, &size_w_t);

	/* ＰＣＭバッファが受け入れ可能なサイズを、ｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数に換算する。 */
	pcm_sg = size_w_t / ADP_SIZE_SG_DECO;

	/* コピーするサイズ [sg/1ch] */
	copy_sg = MIN(pcm_sg, ring_sg);

	/* 94.11.21 タスク関数仕様変更
	 *	再生状態に関わらず、１回のタスク関数で処理する量は最大４ｋサンプル。
	 *		copy_sg = MIN(copy_sg, PCM_4KSAMPEL_SG);
	 * 95.01.25 タスク関数仕様変更
	 *	１回のタスクで処理する量の上限はユーザが調節できる。
	 */
	/* １回のタスクで処理する量の上限[sg/1ch]	*/
	copy_sg = MIN(copy_sg, st->onetask_sg);

	if (copy_sg == 0) {
		return;
	}

	if (st->fill_silence) {
		VTV_PRINTF((VTV_s, "P:fill_silence OFF\n"));
	}

	st->fill_silence = PCM_OFF;	/* 無音設定済みフラグを寝せる */

	/* ＡＤＰＣＭ伸張の実行 */
	
	if (PCM_IS_STEREO(st)) {
		for (j = 0; j < 2 * copy_sg; j++) {
			if (st->ring_read_addr + ADP_SIZE_SG_CODE > st->ring_end_addr){
				size_1 = st->ring_end_addr - st->ring_read_addr;
				size_2 = ADP_SIZE_SG_CODE - size_1;
				memcpy(flop, st->ring_read_addr, size_1);
				memcpy(flop + size_1, para->ring_addr, size_2);
				input_addr = flop;
			} else {
				input_addr = st->ring_read_addr;
			}
			ADP_DecStereo(input_addr, buf);/* １ＳＧのステレオ伸張 */
			pcm_GetPcmWrite(hn, &addr_w1, &addr_w2, &size_w, &size_w_t);
			size_1 = MIN(size_w, ADP_SIZE_SG_DECO_1);
			size_2 = ADP_SIZE_SG_DECO_1 - size_1;
			dsize_1 = size_1 >> 2;
			dsize_2 = size_2 >> 2;
			pcm_MemcpyDword((Uint32 *)addr_w2, (Uint32 *)buf_l, dsize_1);
			pcm_MemcpyDword((Uint32 *)addr_w1, (Uint32 *)buf_r, dsize_1);
			pcm_RenewPcmWrite(hn, size_1);
			if (size_2 != 0) {

#ifdef NEW_ADPCM_TEST
	cnt_pcm_top++;
	if (cnt_pcm_top == 5) {
		pcm_adp_break();
	}
#endif

				pcm_GetPcmWrite(hn, &addr_w1, &addr_w2, &size_w,&size_w_t);
				pcm_MemcpyDword((Uint32 *)addr_w2, 
								(Uint32 *)(buf_l + size_1), dsize_2);
				PCM_KILL_NOISE16(addr_w2, st->pcm_bsize);
				pcm_MemcpyDword((Uint32 *)addr_w1, 
								(Uint32 *)(buf_r + size_1), dsize_2);
				PCM_KILL_NOISE16(addr_w1, st->pcm_bsize);
				pcm_RenewPcmWrite(hn, size_2);
			}
			/* リングバッファ読み取り位置更新 */
			pcm_RenewRingRead(hn, ADP_SIZE_SG_CODE);
			PCM_CHECK_SAMPLE_FILE(st);
			st->cnt_sg++;
			if ((st->cnt_sg % PCM_SG_IN_SECTOR) == 0) {
				/* リングバッファ読み取り位置更新 */
				pcm_RenewRingRead(hn, PCM_SIZE_PAD);
				PCM_CHECK_SAMPLE_FILE(st);
			}
		}
	} else {
		/* monoral */
		for (j = 0; j < copy_sg; j++) {
			if (st->ring_read_addr + ADP_SIZE_SG_CODE > st->ring_end_addr){
				size_1 = st->ring_end_addr - st->ring_read_addr;
				size_2 = ADP_SIZE_SG_CODE - size_1;
				memcpy(flop, st->ring_read_addr, size_1);
				memcpy(flop + size_1, para->ring_addr, size_2);
				input_addr = flop;
			} else {
				input_addr = st->ring_read_addr;
			}
			ADP_DecMono(input_addr, buf);/* １ＳＧのモノラル伸張 */
			pcm_GetPcmWrite(hn, &addr_w1, &addr_w2, &size_w, &size_w_t);
			size_1 = MIN(size_w, ADP_SIZE_SG_DECO);
			size_2 = ADP_SIZE_SG_DECO - size_1;
			dsize_1 = size_1 >> 2;
			dsize_2 = size_2 >> 2;
			pcm_MemcpyDword((Uint32 *)addr_w1, (Uint32 *)buf, dsize_1);
			pcm_RenewPcmWrite(hn, size_1);
			if (size_2 != 0) {
				pcm_GetPcmWrite(hn, &addr_w1, &addr_w2, &size_w,&size_w_t);
				pcm_MemcpyDword((Uint32 *)addr_w1, 
								(Uint32 *)(buf + size_1), dsize_2);
				PCM_KILL_NOISE16(addr_w1, st->pcm_bsize);
				pcm_RenewPcmWrite(hn, size_2);
			}
			/* リングバッファ読み取り位置更新 */
			pcm_RenewRingRead(hn, ADP_SIZE_SG_CODE);
			PCM_CHECK_SAMPLE_FILE(st);
			st->cnt_sg++;
			if ((st->cnt_sg % PCM_SG_IN_SECTOR) == 0) {
				/* リングバッファ読み取り位置更新 */
				pcm_RenewRingRead(hn, PCM_SIZE_PAD);
				PCM_CHECK_SAMPLE_FILE(st);
			}
		}
	}

	return;
}

/*******************************************************************
【機　能】
	メモリ再生／１オーディオサンプル処理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【備　考】
	関数ポインタ st->audio_process_fp に設定されてコールされる
*******************************************************************/
void pcm_AudioProcessAdpcm(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (PCM_IS_ADPCM_SCT(st)) {
		/* ＡＤＰＣＭオーディオ処理 (セクタバウンダリのサウンドグループ) */
		pcm_AudioAdpcmSct(hn);
	} else if (PCM_IS_ADPCM_SG(st)) {
		/* ＡＤＰＣＭオーディオ処理 (連続したサウンドグループ) */
		pcm_AudioAdpcmSg(hn);
	}
}

/*******************************************************************
【機　能】
	メモリ再生／PCM_SetInfo のADPCM対応処理
【引　数】
	hn　	（入力）　シネパックハンドル
【戻り値】
	なし
【備　考】
	関数ポインタ pcm_set_info_adpcm_fp に設定されてコールされる
*******************************************************************/
void pcm_SetInfoAdpcm(PcmHn hn)
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
	PcmFilePara *filepara = &work->filepara;

	PCM_INFO_FILE_SIZE(&st->info) = -1;			/* 使用しない */
	PCM_INFO_SAMPLING_BIT(&st->info) = 16;		/* 16 [bit/sample] */
	PCM_INFO_CHANNEL(&st->info) = -1;			/* 先頭ｾｸﾀのciを見て判断 */
	PCM_INFO_SAMPLING_RATE(&st->info) = -1;		/* 先頭ｾｸﾀのciを見て判断 */
	PCM_INFO_SAMPLE_FILE(&st->info) = -1;		/* 使用しない */
	PCM_INFO_COMPRESSION_TYPE(&st->info) = -1;	/* 使用しない */
	if (PCM_IS_NO_HEADER(st)) {
		st->need_ci = PCM_ON;
	}
	filepara->data.stm.sect_bsize = 2324;

	/* オーディオ処理関数ポインタの設定 */
	st->audio_process_fp = pcm_AudioProcessAdpcm;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ＡＤＰＣＭ使用宣言
	ADPCMに対応する関数を登録する。
【引　数】
	なし
【戻り値】
	なし
【注　意】
	PCM_Init の後、他の関数を使用する前に１度だけコールする。
【備　考】
	この関数を使用すれば「ＡＤＰＣＭ伸張ライブラリ」がリンクされる。
*******************************************************************/
void PCM_DeclareUseAdpcm(void)
{

	/* ＡＰＣＭチャンク処理 */
	pcm_chunk_adpcm_fp = pcm_ChunkAdpcm;

	/* PCM_SetInfo のADPCM対応処理 */
	pcm_set_info_adpcm_fp = pcm_SetInfoAdpcm;
}

#endif /* ADPCM_FUNCTION */
