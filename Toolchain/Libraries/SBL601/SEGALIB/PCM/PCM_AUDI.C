/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:PCM buffer management, PCM data copy
 * File		:pcm_audi.c
 * Date		:1994-09-29
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include "pcm_msub.h"

/* 無音設定する */
/* #define FILL_SILENCE */

/* SMOOTH_FILLする */
#define PCM_SMOOTH_FILL


/*------------------------- 《処理マクロ》 -------------------------*/

/* ＤＭＡ転送の起動 */
#define COPY_DMA_PCM(st)													\
	DMA_CpuMemCopy4((void *)(st)->dma_pcm_tbl[(st)->idx_dma_pcm_tbl].dst,	\
					(void *)(st)->dma_pcm_tbl[(st)->idx_dma_pcm_tbl].src,	\
					(st)->dma_pcm_tbl[(st)->idx_dma_pcm_tbl].cnt)

#define ENTRY_DMA_PCM(st, idx, dest, srce, size)							\
		(																	\
			(st)->dma_pcm_tbl[(idx)].dst = (Uint32)(dest), 					\
			(st)->dma_pcm_tbl[(idx)].src = (srce), 							\
			(st)->dma_pcm_tbl[(idx)].cnt = (size) >> 2 						\
		)

/* 転送先スイッチ */
#define DST_1ST(d1, d2)		(PCM_IS_LRLRLR(st) ? (d2) : (d1))
#define DST_2ST(d1, d2)		(PCM_IS_LRLRLR(st) ? (d1) : (d2))
#define DST_1(d1, d2)		(PCM_IS_MONORAL(st) ? (d1) : DST_1ST((d1), (d2)))
#define DST_2(d1, d2)		(PCM_IS_MONORAL(st) ? NULL : DST_2ST((d1), (d2)))

#define ENTRY_COPY_TBL(st, idx, d1, d2, srce, bsize)					\
		(																\
			(st)->copy_tbl[(idx)].dst1 = DST_1((d1), (d2)), 				\
			(st)->copy_tbl[(idx)].dst2 = DST_2((d1), (d2)), 				\
			(st)->copy_tbl[(idx)].src = (srce), 						\
			(st)->copy_tbl[(idx)].size = (bsize) 						\
		)

/*******************************************************************
【機　能】
	オフセットの位置のデータが先頭に来るようにバッファ中のデータを回転させる。
					<-- pcm_bsize ---------------------->
					<-- offset -->
		処理前：	aaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbb
		処理後：	bbbbbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaa
【引　数】
	addr_pwork		(入力)　ポーズ作業用バッファ
	bsize_pwork		(入力)　ポーズ作業用バッファサイズ[byte]
	pcm_addr		(入出力)処理対象バッファ(4バイト境界)
	pcm_bsize	(入力)	処理対象バッファサイズ[byte](4の倍数)
	offset			(入力)	回転させる量[byte]
【戻り値】
	なし
*******************************************************************/
void pcm_RotatePcmbuf(Uint32 addr_pwork, Sint32 bsize_pwork, Uint32 pcm_addr, Sint32 pcm_bsize, Sint32 offset)
{
	Sint32 	bsize_shift;

/* #define DEBUG_PRINT_ROTATE */

	#ifdef DEBUG_PRINT_ROTATE
		Sint32	cnt_rotate = 0;
		
		debug_write_str("Rota-Pcm");
		debug_write_data((Uint32)addr_pwork);
		debug_write_data((Uint32)bsize_pwork);
		debug_write_data((Uint32)pcm_addr);
		debug_write_data((Uint32)pcm_bsize);
		debug_write_data((Uint32)offset);
	#endif

	if (addr_pwork & 3) {
		addr_pwork = (addr_pwork + 3) & 0xFFFFFFFC;
		bsize_pwork -= 4;
	}
	bsize_pwork &= 0xFFFFFFFC;
	while (offset > 0) {
		#ifdef DEBUG_PRINT_ROTATE
			cnt_rotate++;
		#endif
		bsize_shift = MIN(bsize_pwork, offset);
		if (bsize_shift & 3) {
			pcm_RotateByte((Sint8 *)addr_pwork, (Sint8 *)pcm_addr, 
								pcm_bsize, bsize_shift);
		} else {
			pcm_RotateDword((Sint32 *)addr_pwork, (Sint32 *)pcm_addr, 
								pcm_bsize >> 2, bsize_shift >> 2);
		}
		offset -= bsize_shift;
	}

	#ifdef DEBUG_PRINT_ROTATE
		debug_write_data((Uint32)cnt_rotate);
	#endif
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／PCMバッファ書き込みアドレスの取得
【引　数】
	hn					(入力)　ハンドル
	addr_write1			(出力)　PCMバッファ書き込みアドレス(ﾓﾉﾗﾙ or ｽﾃﾚｵ-R)
	addr_write2			(出力)　PCMバッファ書き込みアドレス(ｽﾃﾚｵ-L)
	size_write			(出力)　連続書き込み可能サイズ[byte]
	size_write_total	(出力)　折り返しを含めた書き込み可能サイズ[byte]
【戻り値】
	なし
【注　意】
*******************************************************************/
void pcm_GetPcmWrite(PcmHn hn, Sint8 **addr_write1, Sint8 **addr_write2, Sint32 *size_write, Sint32 *size_write_total)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	Sint32		sample_read_safety;		/* 安全な読み取り量[sample] */

	/* 安全な読み取り位置「st->cnt_4ksample」による再生量の計算 */
	sample_read_safety = st->sample_pause + st->cnt_4ksample * 4096;

	/* 書き込み可能トータルサイズ */
	*size_write_total = st->pcm_bsize - 
		PCM_SAMPLE2BSIZE(st, st->sample_write - sample_read_safety);

	/* 連続書き込み可能サイズ */
	if (*size_write_total == 0) {
		*size_write = 0;
		*addr_write1 = NULL;
		*addr_write2 = NULL;
	} else {
		if (st->pcm_write_offset + *size_write_total > st->pcm_bsize) {
			*size_write = st->pcm_bsize - st->pcm_write_offset;
		} else {
			*size_write = *size_write_total;
		}
		*addr_write1 = para->pcm_addr + st->pcm_write_offset;
		*addr_write2 = st->pcm2_addr + st->pcm_write_offset;
	}
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／PCMバッファ書き込みアドレスの更新(書き込み量の報告)
【引　数】
	hn			(入力)　ハンドル
	size_write	(入力)　PCMバッファ書き込み量[byte/1ch]
【戻り値】
	なし
【注　意】
*******************************************************************/
void pcm_RenewPcmWrite(PcmHn hn, Sint32 size_write)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		sample;
#ifdef __PCMD
	PcmPara		*para 	= &work->para;
	Sint32 		dwsize;
	static Uint32 *save_addr;

	if (st->sample_write_file == 0) {
		save_addr = (Uint32 *)0x4000000;
	}
#endif

	/* 供給サンプル数[sample]の更新				*/
	sample = PCM_BSIZE2SAMPLE(st, size_write);
	st->sample_write_file += sample;	/* 当ファイルの供給サンプル数[sample]*/
	st->sample_write += sample;			/* 供給サンプル数[sample]			*/

	st->pcm_write_offset   += size_write;
	if (st->pcm_write_offset >= st->pcm_bsize) {
		st->pcm_write_offset -= st->pcm_bsize;

		if (st->pcm_write_offset != 0) {
			VTV_PRINTF((VTV_s, "P:?WritOff %X\n", st->pcm_write_offset));
		}

#ifdef __PCMD
	if ((Uint32)save_addr < 0x4100000) {
		dwsize = st->pcm_bsize >> 2;
		pcm_MemcpyDword(save_addr, (Uint32 *)para->pcm_addr, dwsize);
		save_addr += dwsize;
	}
#endif

	}
	return;
}

#if 0	/* 作成中 */

/*******************************************************************
【機　能】
	メモリ再生／ＤＭＡ転送管理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【注　意】
*******************************************************************/
STATIC void pcm_CheckDma(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	PcmHeader	*head 	= work->header;
	Uint32 		result;
	Uint32 		start_addr, end_addr;

	if (pcm_copy_mode_pcm != PCM_COPY_MODE_PCM_CPU_DMA) {
		return;
	}

	if (st->dma_hold == PCM_ON) {
		result = DMA_CpuResult();
		switch (result) {
		case DMA_CPU_BUSY:
			return;
		case DMA_CPU_FAIL:
			PCM_MeSetErrCode(PCM_ERR_DMA_CPU_PCM);
			break;
		case DMA_CPU_END:
			break;
		}
#if 1
		/* PCMバッファ折り返しノイズをなくすために。 */
		start_addr = st->dma_pcm_tbl[st->idx_dma_pcm_tbl].dst;
		end_addr = start_addr + st->bsize_pcmbuf;
		if ((start_addr == (Uint32)para->addr_pcmbuf)
		 || (start_addr == (Uint32)st->addr_pcmbuf_end)) {
			if (PCM_IS_16BIT_SAMPLING(head)) {
				*(Uint16 *)(end_addr - 2) = *(Uint16 *)start_addr;
			} else {
				*(Uint8 *)(end_addr - 1) = *(Uint8 *)start_addr;
			}
		}
#endif
		if (st->idx_dma_pcm_tbl == 0) {
			/* 全ての転送作業が完了した */
			pcm_RenewPcmWrite(hn, st->pcm_renew_size);
			pcm_RenewRead(hn, st->buf_renew_size);
			st->pcm_renew_size = 0;
			st->buf_renew_size = 0;
			st->dma_hold = PCM_OFF;
			pcm_flag_dma_use = 0;
		} else {
			/* エントリされているＤＭＡ転送を起動する */
			st->idx_dma_pcm_tbl--;
			COPY_DMA_PCM(st);
		}
	}
}

/* DMA転送完了待ち */
void pcm_WaitDmaFinish(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	while (st->dma_hold == PCM_ON) {
		pcm_CheckDma(hn);	/* ＤＭＡ転送管理 */
	}
}

/*******************************************************************
【機　能】
	メモリ再生／(ブロック)コピー情報テーブルの設定
【引　数】
	hn			(入力)　ハンドル
	size_copy	(入力)　1ch分のバイト数
【戻り値】
	なし
【備　考】
	１ブロックの連続性は保証されているものとする。
	PCMﾊﾞｯﾌｧ書き込み位置更新を含む。
*******************************************************************/
void pcm_SetBlockCopyTbl(PcmHn hn, Sint32 size_copy)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	Sint32		size_write, size_write_total;
	Sint32		size_2nd, 			/* 2回目のpcmロードサイズ(1ﾁｬﾝﾈﾙ分) */
				size_mono;			/* ｵｰﾃﾞｨｵｻﾝﾌﾟﾙの１ﾁｬﾝﾈﾙ分のサイズ */
	Sint8 		*addr_write1, *addr_write2;
	Sint8		*addr_sample;

	if (size_mono <= size_write) {
		if (PCM_IS_MONORAL(st)) {/* モノラルデータ */
			st->idx_dma_pcm_tbl = 0;
			addr_sample = st->ring_read_addr;
			ENTRY_DMA_PCM(st, 0, addr_write1, addr_sample, size_mono);
		} else {/* ステレオデータ */
			st->idx_dma_pcm_tbl = 1;
			ENTRY_DMA_PCM(st, 1, addr_write2, addr_sample, size_mono);
			ENTRY_DMA_PCM(st, 0, addr_write1, addr_sample + size_mono, 
															size_mono);
		}
	} else {	/* 折り返し転送 */
		if (PCM_IS_MONORAL(st)) {/* モノラルデータ */
			st->idx_dma_pcm_tbl = 1;
			addr_sample = st->ring_read_addr;
			ENTRY_DMA_PCM(st, 1, addr_write1, addr_sample, size_write);
		} else {/* ステレオデータ */
			st->idx_dma_pcm_tbl = 3;
			ENTRY_DMA_PCM(st, 3, addr_write2, addr_sample, size_write);
			ENTRY_DMA_PCM(st, 2, addr_write1, addr_sample + size_mono, 
														size_write);
		}
		/* ２回目のロード */
		size_2nd = size_mono - size_write;
		addr_sample += size_write;
		addr_write1 = para->pcm_addr;
		addr_sample = st->ring_read_addr + size_write;
		pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
							&size_write, &size_write_total);
		if (PCM_IS_MONORAL(st)) {/* モノラルデータ */
			ENTRY_DMA_PCM(st, 0, addr_write1, addr_sample, size_2nd);
		} else {/* ステレオデータ */
			ENTRY_DMA_PCM(st, 1, addr_write2, addr_sample, size_2nd);
			ENTRY_DMA_PCM(st, 0, addr_write1, addr_sample + size_mono, 
															size_2nd);
		}

	}
}

/*******************************************************************
【機　能】
	メモリ再生／ＬＲブロックオーディオ処理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【備　考】
	１ブロックの連続性は保証されているものとする。
*******************************************************************/
STATIC void pcm_AudioBlock(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		size_write, size_write_total;
	Sint8 		*addr_write1, *addr_write2;
	Sint32		size_sample, 		/*  */
				size_2nd, 			/* 2回目のpcmロードサイズ(1ﾁｬﾝﾈﾙ分) */
				size_mono;			/* ｵｰﾃﾞｨｵｻﾝﾌﾟﾙの１ﾁｬﾝﾈﾙ分のサイズ */

	/* DMA資源が空き状態かチェックする */
	if ((pcm_copy_mode_pcm == PCM_COPY_MODE_PCM_CPU_DMA)
	 && (pcm_flag_dma_use)) {
		st->cnt_over_task_call++;
		return;
	}

	/* リングバッファの在庫[byte] */
	size_sample = st->ring_write_offset - st->ring_read_offset;

	/*  */
	size_mono = st->ring_write_offset - st->ring_read_offset;

	 = st->ring_write_offset - st->ring_read_offset;



	/* ｵｰﾃﾞｨｵｻﾝﾌﾟﾙの１ﾁｬﾝﾈﾙ分のサイズを求める  */
	switch (st->info.channel) {
	case 1:	/* 1チャンネル（モノラル）データ */
		size_mono = size_sample;
		break;
	case 2:	/* 2チャンネル（ステレオ）データ */
		size_mono = size_sample >> 1;
		break;
	}

	/* オーディオデータがロードできるかをチェックする */
	pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
						&size_write, &size_write_total);

	/* 動画ファイルのインタリーブが適切であり、画像サンプルを正しい
	 * 時刻に処理していれば、以下のチェックは不要か。いや、オーディオ
	 * サンプルが連続するケースがあるのでこのチェックは必要。
	 */
	if (size_write_total < size_mono) {
		/* 全部をロードできる空きがないうちは、ロードしない */
		#ifdef __PCMD
			debug_write_str("LWai");
			debug_write_data((Uint32)st->CntSample);
			debug_write_data((Uint32)size_write_total);
			debug_write_data((Uint32)st->pcm_bsize);
		#endif
		st->cnt_over_task_call++;
		return;
	}
	if (size_write_total > st->pcm_bsize) {
		st->cnt_load_miss++;
		VTV_PRINTF((VTV_s, "P:LMis %d\n buf%X < writ%X\n", 
			st->cnt_load_miss, st->pcm_bsize, size_write_total));
	}

	/* コピー情報テーブルの設定 */
	pcm_SetBlockCopyTbl(hn, size_copy);

	if (pcm_copy_mode_pcm == PCM_COPY_MODE_PCM_CPU_DMA) {
		/* CPU DMA 転送 */
		st->pcm_renew_size = size_mono;
		st->buf_renew_size = size_sample;
		st->dma_hold = PCM_ON;
		pcm_flag_dma_use = 1;
		COPY_DMA_PCM(st);
	} else {
		/* CPU プログラム転送 */
		
		/* コピーの実行 (CPU プログラム転送) */
		pcm_ExecBlockCopyTbl(hn);

		pcm_RenewPcmWrite(hn, size_2nd);
		pcm_RenewRingRead(hn, size_sample);/* 読み取り位置更新 */
	}

	return;
}
#endif

/*******************************************************************
【機　能】
	メモリ再生／(ＬＲ混在)コピー情報テーブルの設定
【引　数】
	hn			(入力)　ハンドル
	size_copy	(入力)　1ch分のバイト数
【戻り値】
	なし
【備　考】
	リングバッファ上で連続している分だけ処理する。
	PCMﾊﾞｯﾌｧ書き込み位置更新を含む。
*******************************************************************/
STATIC void pcm_SetMixCopyTbl(PcmHn hn, Sint32 size_copy)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		size_write, size_write_total;
	Sint32		size_2nd; 			/* 2回目のpcmロードサイズ(1ﾁｬﾝﾈﾙ分) */
	Sint8 		*addr_write1, *addr_write2;
	Sint8		*addr_sample;

	pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
						&size_write, &size_write_total);
	if (size_copy <= size_write) {
		st->copy_idx = 0;
		addr_sample = st->ring_read_addr;
		ENTRY_COPY_TBL(st, 0, addr_write1, addr_write2, 
							addr_sample, size_copy);
		pcm_RenewPcmWrite(hn, size_copy);
		#ifdef __PCMD
			debug_write_str("Tbl0");
			debug_write_data((Uint32)addr_write1);
			debug_write_data((Uint32)addr_sample);
			debug_write_data((Uint32)size_copy);
		#endif
	} else {	/* 折り返し転送 */
		st->copy_idx = 1;
		addr_sample = st->ring_read_addr;
		ENTRY_COPY_TBL(st, 1, addr_write1, addr_write2, 
							addr_sample, size_write);
		pcm_RenewPcmWrite(hn, size_write);
		#ifdef __PCMD
			debug_write_str("Tbl1");
			debug_write_data((Uint32)addr_write1);
			debug_write_data((Uint32)addr_sample);
			debug_write_data((Uint32)size_write);
		#endif
		/* ２回目のロード */
		size_2nd = size_copy - size_write;
		addr_sample = st->ring_read_addr + PCM_1CH2NCH(st, size_write);
		pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
							&size_write, &size_write_total);
		ENTRY_COPY_TBL(st, 0, addr_write1, addr_write2, 
							addr_sample, size_2nd);
		pcm_RenewPcmWrite(hn, size_2nd);
		#ifdef __PCMD
			debug_write_str("Tbl2");
			debug_write_data((Uint32)addr_write1);
			debug_write_data((Uint32)addr_sample);
			debug_write_data((Uint32)size_2nd);
		#endif
	}
}

STATIC void pcm_MixCopy16bitStereo(PcmHn hn, PcmCopyTbl *stock)
{
	PcmWork				*work 	= *(PcmWork **)hn;
	PcmPara				*para 	= &work->para;
	PcmStatus			*st 	= &work->status;
	register Sint16 	*dst1, *dst2, *src, *src_end;
	register Sint16 	*ring_end_addr = (Sint16 *)st->ring_end_addr;
	register Sint32 	wsize;

	wsize = stock->size >> 1;
	dst1 = (Sint16 *)stock->dst1;
	dst2 = (Sint16 *)stock->dst2;
	src = (Sint16 *)stock->src;
	src_end = src + 2*wsize;

	if (src_end <= (Sint16 *)para->ring_addr) {
		while (src < src_end) {
			*dst1++ = *src++;
			*dst2++ = *src++;
		}
	} else {
		for (; wsize > 0; wsize--) {
			*dst1++ = *src++;
			if (src >= ring_end_addr) src = (Sint16 *)para->ring_addr;
			*dst2++ = *src++;
			if (src >= ring_end_addr) src = (Sint16 *)para->ring_addr;
		}
	}

	/* 折り返しノイズ対策 */
	PCM_CHECK_NOISE16(stock->dst1, para->pcm_addr, st->pcm_bsize);
	PCM_CHECK_NOISE16(stock->dst1, st->pcm2_addr, st->pcm_bsize);
	PCM_CHECK_NOISE16(stock->dst2, para->pcm_addr, st->pcm_bsize);
	PCM_CHECK_NOISE16(stock->dst2, st->pcm2_addr, st->pcm_bsize);
}

STATIC void pcm_MixCopy8bitStereo(PcmHn hn, PcmCopyTbl *stock)
{
	PcmWork				*work 	= *(PcmWork **)hn;
	PcmPara				*para 	= &work->para;
	PcmStatus			*st 	= &work->status;
	register Sint8 		*dst1, *dst2, *src, *src_end;
	register Sint8 		*ring_end_addr = st->ring_end_addr;
	register Sint32 	size;

	size = stock->size;
	dst1 = stock->dst1;
	dst2 = stock->dst2;
	src = stock->src;
	src_end = src + 2*size;

	if (src_end <= para->ring_addr) {
		while (src < src_end) {
			*dst1++ = *src++;
			*dst2++ = *src++;
		}
	} else {
		for (; size > 0; size--) {
			*dst1++ = *src++;
			if (src >= ring_end_addr) src = para->ring_addr;
			*dst2++ = *src++;
			if (src >= ring_end_addr) src = para->ring_addr;
		}
	}

	/* 折り返しノイズ対策 */
	PCM_CHECK_NOISE8(stock->dst1, para->pcm_addr, st->pcm_bsize);
	PCM_CHECK_NOISE8(stock->dst1, st->pcm2_addr, st->pcm_bsize);
	PCM_CHECK_NOISE8(stock->dst2, para->pcm_addr, st->pcm_bsize);
	PCM_CHECK_NOISE8(stock->dst2, st->pcm2_addr, st->pcm_bsize);
}

STATIC void pcm_MixCopy16bitMono(PcmHn hn, PcmCopyTbl *stock)
{
	PcmWork				*work 	= *(PcmWork **)hn;
	PcmPara				*para 	= &work->para;
	PcmStatus			*st 	= &work->status;
	register Sint16 	*dst1, *src, *src_end;
	register Sint16 	*ring_end_addr = (Sint16 *)st->ring_end_addr;
	register Sint32 	wsize;

	wsize = stock->size >> 1;
	dst1 = (Sint16 *)stock->dst1;
	src = (Sint16 *)stock->src;
	src_end = src + wsize;

	if (src_end <= ring_end_addr) {
		while (src < src_end) {
			*dst1++ = *src++;
		}
	} else {
		src_end = src + wsize - (para->ring_size >> 1);
		while (src < ring_end_addr) {
			*dst1++ = *src++;
		}
		src = (Sint16 *)para->ring_addr;
		while (src < src_end) {
			*dst1++ = *src++;
		}
	}

	/* 折り返しノイズ対策 */
	PCM_CHECK_NOISE16(stock->dst1, para->pcm_addr, st->pcm_bsize);
}

STATIC void pcm_MixCopy8bitMono(PcmHn hn, PcmCopyTbl *stock)
{
	PcmWork				*work 	= *(PcmWork **)hn;
	PcmPara				*para 	= &work->para;
	PcmStatus			*st 	= &work->status;
	register Sint8 		*dst1, *src, *src_end;
	register Sint8 		*ring_end_addr = st->ring_end_addr;
	register Sint32 	size;

	size = stock->size;
	dst1 = stock->dst1;
	src = stock->src;
	src_end = src + size;
	if (src_end <= ring_end_addr) {
		while (src < src_end) {
			*dst1++ = *src++;
		}
	} else {
		src_end = src + size - para->ring_size;
		while (src < ring_end_addr) {
			*dst1++ = *src++;
		}
		src = para->ring_addr;
		while (src < src_end) {
			*dst1++ = *src++;
		}
	}

	/* 折り返しノイズ対策 */
	PCM_CHECK_NOISE8(stock->dst1, para->pcm_addr, st->pcm_bsize);
}

STATIC void pcm_ExecMixCopyTbl(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	
	for (; st->copy_idx >= 0; st->copy_idx--) {
		if (PCM_IS_MONORAL(st)) {
			if (PCM_IS_8BIT_SAMPLING(st)) {
				pcm_MixCopy8bitMono(hn, st->copy_tbl + st->copy_idx);
			} else {
				pcm_MixCopy16bitMono(hn, st->copy_tbl + st->copy_idx);
			}
		} else {
			if (PCM_IS_8BIT_SAMPLING(st)) {
				pcm_MixCopy8bitStereo(hn, st->copy_tbl + st->copy_idx);
			} else {
				pcm_MixCopy16bitStereo(hn, st->copy_tbl + st->copy_idx);
			}
		}
	}
}

/*******************************************************************
【機　能】
	メモリ再生／ＬＲ混在オーディオ処理 
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	1ch分の処理量[byte/1ch]
【備　考】
	ここで処理する在庫はリングバッファ上で連続している保証はない。
	さらに、ヘッダサイズがサンプルフレームバウンダリでない場合、
　リングバッファ折り返し位置でサンプルフレームが分割される。

		+-------------------------------------------------------+
		|					リングバッファ						|
		+-------------------------------------------------------+
										  ..... L R, L R, L R, L 
		 R, L R, L R, .....							 <-->
													サンプルフレーム
*******************************************************************/
STATIC void pcm_AudioMix(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		size_write, size_write_total;
	Sint32		stock_bsize, 		/* リングバッファの総在庫 [byte] 	*/
				size_mono, 			/* １チャンネル分の在庫 [byte/1ch] 	*/
				size_copy; 			/* コピーするサイズ [byte/1ch] 		*/
	Sint8 		*addr_write1, *addr_write2;

#ifdef _PCMD
	Sint32 		copy_idx, i;
#endif

	if (st->ring_write_offset > st->info.file_size) {
		/* リングバッファには EOF 以降のゴミも供給されている */
		stock_bsize = st->info.file_size - st->ring_read_offset;
	} else {
		stock_bsize = st->ring_write_offset - st->ring_read_offset;
	}

	/* １チャンネル分の在庫 [byte/1ch] */
	if (PCM_IS_MONORAL(st)) {
		size_mono = stock_bsize;
	} else {
		size_mono = stock_bsize >> 1;
	}
	if (PCM_IS_8BIT_SAMPLING(st)) {
		size_mono &= 0xfffffffe;
		size_mono &= 0xfffffffc;
	} else {
		size_mono &= 0xfffffffc;
	}

	size_mono = MIN(size_mono, 
		PCM_SAMPLE2BSIZE(st, st->info.sample_file - st->sample_write_file));

	/* オーディオデータがロードできるかをチェックする */
	pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
						&size_write, &size_write_total);

	if (size_write_total > st->pcm_bsize) {
		st->cnt_load_miss++;
		VTV_PRINTF((VTV_s, "P:LMis %d\n buf%X < writ%X\n", 
			st->cnt_load_miss, st->pcm_bsize, size_write_total));
		VTV_PRINTF((VTV_s, " r%X w%X\n", 
			(st)->ring_read_offset, (st)->ring_write_offset));
	}

	/* コピーするサイズ [byte/1ch] */
	size_copy = MIN(size_mono, size_write_total);

	if (size_copy == 0) {
		return;
	}

	/* 94.11.21 タスク関数仕様変更
	 *	再生状態に関わらず、１回のタスク関数で処理する量は最大４ｋサンプル。
	 *		size_copy = MIN(size_copy, PCM_4KSAMPEL_BYTE(st));
	 * 95.01.25 タスク関数仕様変更
	 *	１回のタスクで処理する量の上限はユーザが調節できる。
	 */
	/* １回のタスクで処理する量の上限[byte/1ch]	*/
	size_copy = MIN(size_copy, st->onetask_size);

	/* コピー情報テーブルの設定 */
	pcm_SetMixCopyTbl(hn, size_copy);

#ifdef _PCMD
	copy_idx = st->copy_idx;
#endif

	/* コピーの実行 (常に、CPU プログラム転送) */
	pcm_ExecMixCopyTbl(hn);

#ifdef _PCMD

	#define C24BIT(a)			(0x00ffffff & (Sint32)(a))

	if (size_copy & 0x3ff) {
		_VTV_PRINTF((VTV_s, "P:SizCp%d %X %X\n", 
			copy_idx, size_copy, st->sample_write));
	}
	if (st->sample_write >= 3*64*1024 - 1024 &&
		st->sample_write <  3*64*1024 + 1024) {
		for (i = copy_idx; i >= 0; i--) {
			VTV_PRINTF((VTV_s, "dst %6X %6X\nsrc%8X siz%X\n", 
				C24BIT(st->copy_tbl[i].dst1), C24BIT(st->copy_tbl[i].dst2), 
				st->copy_tbl[i].src, st->copy_tbl[i].size));
		}
	}
#endif

	/* 読み取り位置更新 */
	pcm_RenewRingRead(hn, PCM_1CH2NCH(st, size_copy));

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
void pcm_AudioProcess(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (PCM_IS_LR_MIX(st)) {
		/* ＬＲ混在オーディオ処理 */
		pcm_AudioMix(hn);
	} else if (PCM_IS_LR_BLOCK(st)) {
		/* ＬＲブロックオーディオ処理 */
		/* pcm_AudioBlock(hn); */
	}
}

#ifdef FILL_SILENCE

#define PCM_PRE_VALUE16(fill_addr, pre, pcm_addr, pcm_bsize)	\
			((((fill_addr) + 2 * (pre)) < (pcm_addr)) ? 	\
				*(Sint16 *)((fill_addr) + 2 * (pre) + (pcm_bsize))	\
			:	\
				*(Sint16 *)((fill_addr) + 2 * (pre))	\
			)

#define PCM_PRE_VALUE8(fill_addr, pre, pcm_addr, pcm_bsize)	\
			((((fill_addr) + (pre)) < (pcm_addr)) ? 	\
				*((fill_addr) + (pre) + (pcm_bsize))	\
			:	\
				*((fill_addr) + (pre))	\
			)

void pcm_Fill16(Sint8 *pcm_addr, Sint32 pcm_bsize, 
				Sint8 *fill_addr, Sint32 fill_bsize)
{
	Sint32		size_1st, size_2nd;
	Sint32		delta, val, pre1_val, pre2_val;
	Sint16		val_set;
	Sint16		*fill_p, *fill_p_end;
	Sint32		fill_cnt;
	

	size_1st = MIN(fill_bsize, pcm_bsize - (fill_addr - pcm_addr));
	size_2nd = fill_bsize - size_1st;

	memset((void *)fill_addr, 0x00, size_1st);
	memset((void *)pcm_addr, 0x00, size_2nd);

#ifdef PCM_SMOOTH_FILL
	pre1_val = (Sint32)PCM_PRE_VALUE16(fill_addr, -1, pcm_addr, pcm_bsize);
	pre2_val = (Sint32)PCM_PRE_VALUE16(fill_addr, -2, pcm_addr, pcm_bsize);
	delta = pre1_val - pre2_val;
	val = pre1_val;
	fill_p = (Sint16 *)fill_addr;
	fill_p_end = (Sint16 *)(pcm_addr + pcm_bsize);
	for (fill_cnt = 0; fill_cnt < fill_bsize/2; fill_cnt++) {
		if (delta == -1) {
			if (val == 0) {
				break;
			} else if (val > 0) {
				delta = -1;
			} else if (val < 0) {
				delta = 0;
			}
		} else {
			delta >>= 1;
			if (delta == 0) {
				if (val == 0) {
					break;
				} else if (val > 0) {
					delta = -1;
				} else if (val < 0) {
					delta = 1;
				}
			}
		}
		val += delta;
		if (val < SHRT_MIN) {
			val_set = SHRT_MIN;
		} else if (val > SHRT_MAX) {
			val_set = SHRT_MAX;
		} else {
			val_set = (Sint16)val;
		}
		*fill_p++ = val_set;
		if (fill_p == fill_p_end) {
			fill_p = (Sint16 *)pcm_addr;
		}
	}
#endif
}

void pcm_Fill8(Sint8 *pcm_addr, Sint32 pcm_bsize, 
				Sint8 *fill_addr, Sint32 fill_bsize)
{
	Sint32		size_1st, size_2nd;
	Sint32		delta, val, pre1_val, pre2_val;
	Sint8		val_set;
	Sint8		*fill_p, *fill_p_end;
	Sint32		fill_cnt;
	

	size_1st = MIN(fill_bsize, pcm_bsize - (fill_addr - pcm_addr));
	size_2nd = fill_bsize - size_1st;

	memset((void *)fill_addr, 0x00, size_1st);
	memset((void *)pcm_addr, 0x00, size_2nd);

#ifdef PCM_SMOOTH_FILL
	pre1_val = (Sint32)PCM_PRE_VALUE8(fill_addr, -1, pcm_addr, pcm_bsize);
	pre2_val = (Sint32)PCM_PRE_VALUE8(fill_addr, -2, pcm_addr, pcm_bsize);
	delta = pre1_val - pre2_val;
	val = pre1_val;
	fill_p = fill_addr;
	fill_p_end = (pcm_addr + pcm_bsize);
	for (fill_cnt = 0; fill_cnt < fill_bsize; fill_cnt++) {
		if (delta == -1) {
			if (val == 0) {
				break;
			} else if (val > 0) {
				delta = -1;
			} else if (val < 0) {
				delta = 0;
			}
		} else {
			delta >>= 1;
			if (delta == 0) {
				if (val == 0) {
					break;
				} else if (val > 0) {
					delta = -1;
				} else if (val < 0) {
					delta = 1;
				}
			}
		}
		val += delta;
		if (val < SCHAR_MIN) {
			val_set = SCHAR_MIN;
		} else if (val > SCHAR_MAX) {
			val_set = SCHAR_MAX;
		} else {
			val_set = (Sint8)val;
		}
		*fill_p++ = val_set;
		if (fill_p == fill_p_end) {
			fill_p = pcm_addr;
		}
	}
#endif
}

/*******************************************************************
【機　能】
	メモリ再生／PCMバッファに無音を設定する
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
*******************************************************************/
void pcm_FillSilence(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	Sint32		size_write, size_write_total;
	Sint32		size_2nd, 			/* 2回目のpcmロードサイズ(1ﾁｬﾝﾈﾙ分) */
				size_mono;			/* ｵｰﾃﾞｨｵｻﾝﾌﾟﾙの１ﾁｬﾝﾈﾙ分のサイズ */
	Sint8  		*addr_write1, *addr_write2;
	Sint32		sample_silence_trig;
	Sint32 		sample_now;
	Sint32 		fill_silence_full;

	if (st->fill_silence == PCM_ON) {	/* 無音設定済みだった */
		return;
	}

	/* 無音の時間に相当するバイト数 */
	size_mono = PCM_SAMPLE2BSIZE(st, PCM_SILENCE_SAMPLE(st, para));
	size_mono &= 0xFFFFFFFC;

	pcm_GetPcmWrite(hn, &addr_write1, &addr_write2, 
						&size_write, &size_write_total);

	if (size_write_total <= 0) {
		/* 無音供給はできない。 */
		return;
	} else if (size_write_total < size_mono) {
		/* まだ、無音供給量に相当する空きがない。 */
		size_mono = size_write_total;
		fill_silence_full = 0;
	} else {
		fill_silence_full = 1;
	}

	if (PCM_IS_ADPCM_SCT(st)) {
		if (st->info.sample_file == -1) {
			/* データ供給で終了判定する場合、関数内で無音設定条件判断する。 */
			sample_silence_trig = PCM_SILENCE_TRIG_SAMPLE(st, para);
			PCM_MeGetTime(hn, &sample_now);
			if (sample_now + sample_silence_trig < st->sample_write_file) {
				/* PCMバッファ内の在庫にまだ余裕がある */
				return;
			}
			if (! PCM_IS_RING_EMPTY(st)) {
				/* リングバッファに在庫があるので、終了ではない */
				return;
			}
			VTV_PRINTF((VTV_s, "P:now%X + trg%X\n > write%X\n", 
				sample_now, sample_silence_trig, st->sample_write_file));
		} else {
			if (st->sample_write_file <= st->info.sample_file) {
				/* まだデータ供給がある */
				return;
			}
		}
	}

	/* よし、これなら無音を設定できるぞ。 */
	if (fill_silence_full) {
		if (pcm_hn_next != NULL) {
			/* 無音設定せずに */
			/* さあ、再生終了だ！ */
			pcm_EndProcess(hn);
			return;
		}
		st->fill_silence = PCM_ON;	/* 無音設定済みフラグをたてる */
		VTV_PRINTF((VTV_s, "P:fill_silence ON\n"));
	} else {
		VTV_PRINTF((VTV_s, "P:fill_silence PART\n"));
	}
	st->cnt_fill_silence++;

	VTV_PRINTF((VTV_s, " hn%d cnt%d bsiz%X\n", 
		PCM_MeGetHandleNo(hn), st->cnt_fill_silence, size_mono));

	if (PCM_IS_16BIT_SAMPLING(st)) {
		if (PCM_IS_MONORAL(st)) {
			pcm_Fill16(para->pcm_addr, st->pcm_bsize, addr_write1, size_mono);
		} else {
			pcm_Fill16(para->pcm_addr, st->pcm_bsize, addr_write1, size_mono);
			pcm_Fill16(st->pcm2_addr, st->pcm_bsize, addr_write2, size_mono);
		}
	} else {
		if (PCM_IS_MONORAL(st)) {
			pcm_Fill8(para->pcm_addr, st->pcm_bsize, addr_write1, size_mono);
		} else {
			pcm_Fill8(para->pcm_addr, st->pcm_bsize, addr_write1, size_mono);
			pcm_Fill8(st->pcm2_addr, st->pcm_bsize, addr_write2, size_mono);
		}
	}
	return;
}
#endif
