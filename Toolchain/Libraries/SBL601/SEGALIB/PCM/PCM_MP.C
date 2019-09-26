/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:メモリ再生
 * File		:pcm_mp.c
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include <machine.h>
#include <string.h>
#include <sega_sys.h>
#include <sega_dma.h>
#include "pcm_msub.h"
#include "pcm_time.h"
#include "pcm_drv.h"
#include "pcm_aif.h"


/*------------------------- 《マクロ定数》 -------------------------*/

/* システム状態値 */
#define PCM_SYSTEM_STATUS_ON		(0x12345678)
#define PCM_SYSTEM_STATUS_OFF		(-1)


/*----------------------- 《グローバル変数》 -----------------------*/

/* バージョン */
Sint8 pcm_version[PCM_VERSION_LEN];

/* システム全体の状態 */
Sint32 pcm_system_status;

/* ハンドル管理変数 */
STATIC PcmHn pcm_hn_tbl[PCM_HN_MAX];		/* ハンドルテーブル		*/
STATIC Uint32 pcm_hn_mask;

/* Vlb-In 割り込みカウンタ */
Sint32 pcm_cnt_vbl_in;

/* 次に再生するムービー */
STATIC PcmHn pcm_hn_next;

/* ＰＣＭデータ転送モード */
/* static PcmCopyModePcm pcm_copy_mode_pcm; */

/* DMA資源使用中フラグ */
static Uint32 pcm_flag_dma_use;

/* ポーズ処理用ワークアドレス、サイズ */
static Sint32 *pcm_pwork_addr;
static Sint32 pcm_pwork_size;

/* エラー管理変数 */
STATIC PcmErrCode pcm_err_code;			/* エラーコード 		*/
STATIC PcmErrFunc pcm_err_func;			/* エラー関数 			*/
STATIC void *pcm_err_obj;				/* エラー関数object 	*/

/* プロトタイプ */
void pcm_SetInfoAdpcmUnlink(PcmHn hn);

/*--------------------- 《グローバル関数ポインタ》 --------------------*/

/* PCM_SetInfo のADPCM非対応処理／対応処理 */
void (*pcm_set_info_adpcm_fp)(PcmHn hn);

/*******************************************************************
【機　能】
	メモリ再生／エラー管理変数の初期化
【引　数】
	なし
【戻り値】
	なし
*******************************************************************/
static void pcm_InitErr(void)
{
	pcm_err_code = PCM_ERR_OK;
	pcm_err_func = NULL;
	pcm_err_obj = NULL;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／エラーコードの設定。エラー関数のコール。
【引　数】
	err		(入力) エラーコード。
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetErrCode(PcmErrCode err)
{
	_VTV_PRINTF((VTV_s, "P:pcm_err_code %X\n", err));

	pcm_err_code = err;
	if (pcm_err_func != NULL) {
		pcm_err_func(pcm_err_obj, err);
	}
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／エラーコードの取得
【引　数】
	なし
【戻り値】
	なし
*******************************************************************/
PcmErrCode PCM_MeGetErrCode(void)
{
	return pcm_err_code;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／エラー関数、エラー関数オブジェクトの設定
【引　数】
	func	(入力) エラー関数ポインタ。エラー発生時にコールする。
	obj		(入力) エラー関数オブジェクト。これを引数にしてエラー関数
					をコールする。
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetErrFunc(PcmErrFunc func, void *obj)
{
	pcm_err_func = func;
	pcm_err_obj = obj;
}

void pcm_InitPcm(void)
{
	return;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／１回のタスクで処理する量の上限の設定 
【引　数】
	hn			(入力)　ハンドル
	sample		(入力)  1ch当たりのサンプル数[sample/1ch]
【戻り値】
	なし
*******************************************************************/
void PCM_MeSet1TaskSample(PcmHn hn, Sint32 sample)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	st->onetask_sample = sample;

	/* １回のタスクで処理する量の上限[byte/1ch]	*/
	st->onetask_size = PCM_SAMPLE2BSIZE(st, st->onetask_sample);

	/* １回のタスクで処理する量の上限[sg/1ch]	*/
	st->onetask_sg = PCM_SAMPLE2SG(st->onetask_sample);
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生開始トリガサイズの設定
【引　数】
	hn			(入力)　ハンドル
	size		(入力)  リングバッファへの供給量[byte]
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetStartTrgSize(PcmHn hn, Sint32 size)
{
	PCM_HN_START_TRG_SIZE(hn) = size;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生開始トリガサンプルの設定
【引　数】
	hn			(入力)　ハンドル
	sample		(入力)  ＰＣＭバッファへの供給量[sample/1ch]
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetStartTrgSample(PcmHn hn, Sint32 sample)
{
	PCM_HN_START_TRG_SAMPLE(hn) = sample;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生停止トリガサンプルの設定
【引　数】
	hn			(入力)　ハンドル
	sample		(入力)  再生停止トリガサンプル[sample/1ch]
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetStopTrgSample(PcmHn hn, Sint32 sample)
{
	PCM_HN_STOP_TRG_SAMPLE(hn) = sample;
}


/*******************************************************************
【機　能】													公開関数
	メモリ再生／ポーズ処理用ワークアドレス、サイズの設定
【引　数】
	addr 		(入力)　ポーズ処理用ワークアドレス
	size 		(入力)　ポーズ処理用ワークサイズ[byte]
【戻り値】
	なし
【備　考】
	サイズは、４ｋサンプル程度でよい。
	ポーズ機能を使用する場合に指定する。
	ポーズを起動する直前にスタックを割り付けても良い。
*******************************************************************/
void PCM_MeSetPauseWork(Sint32 *addr, Sint32 size)
{
	pcm_pwork_addr = (Sint32 *)(((Uint32)addr + 3) & 0xfffffffc);
	if (pcm_pwork_addr == addr) {
		pcm_pwork_size = size & 0xfffffffc;
	} else {
		pcm_pwork_size = (size - 4) & 0xfffffffc;
	}

	if ((pcm_pwork_addr == NULL) || (pcm_pwork_size == 0)) {
		PCM_MeSetErrCode(PCM_ERR_PAUSE_WORK_SET);
	}
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／データの転送方式の設定（リングバッファ→ＰＣＭバッファ）
【引　数】
	hn			(入力)　ハンドル
	mode		(入力) 	データの転送方式
【戻り値】
	なし
【注　意】
	クリエイト後、タスクコール前に指定すること。
*******************************************************************/
#if 0
void PCM_MeSetTrModePcm(PcmHn hn, PcmTrMode mode)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (mode == PCM_TRMODE_SCU) {
		PCM_MeSetErrCode(PCM_ERR_DMA_MODE);
	}
	st->copy_mode_pcm = mode;
}
#endif

/*******************************************************************
【機　能】													公開関数
	メモリ再生／システム全体の初期化処理
【引　数】
	なし
【戻り値】
	なし
*******************************************************************/
void PCM_MeInit(void)
{
	Sint32 i;

	pcm_hn_mask = 0;
	pcm_cnt_vbl_in = 0;
	pcm_hn_next = NULL;
/*	pcm_copy_mode_pcm = PCM_COPY_MODE_PCM_CPU_DMA; CPU DMAでPCMﾃﾞｰﾀｺﾋﾟｰ */
	pcm_flag_dma_use = 0;
	pcm_pwork_addr = NULL;
	pcm_pwork_size = 0;
	pcm_InitClock();
	pcm_InitPcm();
	pcm_InitErr();
	strncpy(pcm_version, PCM_VERSION, PCM_VERSION_LEN);
	for (i = 0; i < PCM_HN_MAX; i++) {
		pcm_hn_tbl[i] = (PcmHn)((i << 24) | (i << 16) | (i << 8) | i);
	}
	pcm_system_status = PCM_SYSTEM_STATUS_ON;


	/* ADPCM関連の関数ポインタの登録 (デフォルトはADPCM非対応処理) */

	/* ＡＰＣＭチャンク処理 (ADPCM非対応) */
	pcm_chunk_adpcm_fp = pcm_ChunkAdpcmUnlink;

	/* PCM_SetInfo のADPCM非対応処理 */
	pcm_set_info_adpcm_fp = pcm_SetInfoAdpcmUnlink;

	return;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／システム全体の終了処理
【引　数】
	なし
【戻り値】
	なし
【概  要】
	システムの休眠中に、不用意に PCM_MeSndRequest などの割り込み関数や、
　タスク関数がコールされても悪さすることがないように、全てのハンドルを
　正常に終了させる。
*******************************************************************/
void PCM_MeFinish(void)
{
	PcmHn 		hn;
	Uint32		i;

	for (i = 0; i < PCM_HN_MAX; i++) {
		if (pcm_hn_mask & pcm_bit_msk[i]) {
			hn = pcm_hn_tbl + i;
			PCM_MeDestroy(hn);
		}
	}
	pcm_hn_mask = 0;
	pcm_system_status = PCM_SYSTEM_STATUS_OFF;
	return;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ＶｂｌＩｎモジュール（ユーザにコールしてもらう）
【引　数】
	なし
【戻り値】
	なし
*******************************************************************/
void PCM_MeVblIn(void)
{
	int			imask;

    imask = get_imask();
    set_imask(15);

	if (pcm_system_status == PCM_SYSTEM_STATUS_ON) {

		pcm_cnt_vbl_in++;

		pcm_GetCount();	/* 頻繁にコールしないと正常に動作しないので */

		PCM_DrvPolling();	/* ４ｋサンプル更新処理をポーリングで起動 */

	}
    set_imask(imask);
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ハンドルのリセット
【引　数】
	work, 		(入力) ワークアドレス
	work_size, 	(入力) ワークサイズ
	para		(入力) 起動パラメータ
【戻り値】
	成功　：　ハンドル値
	失敗　：　NULL
*******************************************************************/
void PCM_MeReset(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;

	st->play = PCM_STAT_PLAY_CREATE;		/* 初期状態						*/

	/* リングバッファ制御情報 */
	st->ring_write_offset = 0;				/* バッファ書き込み位置[byte] 	*/
	st->ring_read_offset = 0;				/* バッファ読み取り位置[byte] 	*/
	st->ring_write_addr = para->ring_addr;	/* バッファ書き込み位置[byte] 	*/
	st->ring_read_addr = para->ring_addr;	/* バッファ読み取り位置[byte] 	*/

	/* PCM再生情報 */
	st->pcm_write_offset = 0;
	st->cnt_4ksample = 0;
	st->fill_silence = PCM_OFF;
	st->cnt_fill_silence = 0;
	st->sample_write_file = 0;		/* 当ファイルの供給サンプル数[sample]	*/
	st->sample_write = 0;			/* 供給サンプル数[sample]				*/
	st->sample_film_start = 0;		/* フィルム開始時サンプル数[sample]		*/
	st->sample_pause = 0;			/* ポーズ時サンプル数[sample]			*/
	st->cnt_sg = 0;					/* サウンドグループカウンタ 			*/
}

/* ガイド文字列の設定 */
void pcm_SetGuide(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	work->work_start 	= PCM_4BYTE_CHAR('W','O','R','K');
	st->stat_start 		= PCM_4BYTE_CHAR('S','T','A','T');
	st->info_start 		= PCM_4BYTE_CHAR('I','N','F','O');
	st->ring_start 		= PCM_4BYTE_CHAR('R','I','N','G');
	st->pcm_start 		= PCM_4BYTE_CHAR('P','C','M',' ');
	st->sddrv_start 	= PCM_4BYTE_CHAR('S','D','R','V');
	st->copy_start 		= PCM_4BYTE_CHAR('C','O','P','Y');
	st->vbl_start 		= PCM_4BYTE_CHAR('V','B','L',' ');
	st->err_start 		= PCM_4BYTE_CHAR('E','R','R',' ');
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ハンドル作成
【引　数】
	work, 		(入力) ワークアドレス
	work_size, 	(入力) ワークサイズ
	para		(入力) 起動パラメータ
【戻り値】
	成功　：　ハンドル値
	失敗　：　NULL
*******************************************************************/
PcmHn PCM_MeCreate(PcmWork *work, PcmPara *para)
{
	PcmHn		hn;
	PcmStatus	*st = &work->status;
	Uint32		i;
	Uint32 		pcm_block;

	/* システム状態チェック */
	PCM_CHK_SYSTEM_STATUS_RET(NULL);

	for (i = 0; i < PCM_HN_MAX; i++) {
		if ((pcm_hn_mask & pcm_bit_msk[i]) == 0) break;
	}
	if (i == PCM_HN_MAX) {
		PCM_MeSetErrCode(PCM_ERR_OUT_OF_HANDLE);
		return NULL;
	}

	/* ＰＣＭバッファサイズ ４ｋの倍数 */
	if (para->pcm_size & 0x00000fff) {
		PCM_MeSetErrCode(PCM_ERR_ILL_SIZE_PCMBUF);
		return NULL;
	}
	/* ＰＣＭバッファサイズ 4096*2 ～ 4096*16 */
	pcm_block = para->pcm_size >> 12;
	if (pcm_block < 2) {
		PCM_MeSetErrCode(PCM_ERR_TOO_SMALL_PCMBUF);
		return NULL;
	}
	if (pcm_block > 16) {
		PCM_MeSetErrCode(PCM_ERR_ILL_SIZE_PCMBUF);
		return NULL;
	}

	pcm_hn_mask |= pcm_bit_msk[i];
	hn = pcm_hn_tbl + i;
	*(PcmWork **)hn = work;

	/* 作業領域のクリア */
	memset((void *)work, 0, sizeof(PcmWork));

	/* ハンドル作成パラメータをそのまま保持 */
	work->para = *para;

	/* ガイド文字列の設定 */
	pcm_SetGuide(hn);

	/************************************/
	/* その他の作業領域の初期化 		*/
	/************************************/

	st->play = PCM_STAT_PLAY_CREATE;		/* 初期状態						*/
	st->ignore_header = PCM_OFF;
	st->need_ci = PCM_OFF;
	st->cnt_loop = 1;

	st->info.file_type = PCM_FILE_TYPE_AIFF;
	st->info.data_type = PCM_DATA_TYPE_LRLRLR;

	/* リングバッファ制御情報 */
	st->ring_write_offset = 0;				/* バッファ書き込み位置[byte] 	*/
	st->ring_read_offset = 0;				/* バッファ読み取り位置[byte] 	*/
	st->ring_write_addr = para->ring_addr;	/* バッファ書き込み位置[byte] 	*/
	st->ring_read_addr = para->ring_addr;	/* バッファ読み取り位置[byte] 	*/
	st->ring_end_addr = para->ring_addr + para->ring_size;

	/* PCM再生情報 */
	st->pcm_write_offset = 0;
	st->cnt_4ksample = 0;
	st->fill_silence = PCM_OFF;
	st->cnt_fill_silence = 0;
	st->sample_write_file = 0;		/* 当ファイルの供給サンプル数[sample]	*/
	st->sample_write = 0;			/* 供給サンプル数[sample]				*/
	st->sample_film_start = 0;		/* フィルム開始時サンプル数[sample]		*/
	st->sample_pause = 0;			/* ポーズ時サンプル数[sample]			*/
	st->cnt_sg = 0;					/* サウンドグループカウンタ 			*/

	st->cnt_load_miss = 0;			/* PCMロードミスカウンタ 				*/
	st->cnt_task_call = 0;			/* タスクコールカウンタ 				*/
	st->cnt_over_task_call = 0;		/* 過剰タスクコールカウンタ 			*/
	st->cnt_buf_empty = 0;			/* バッファ空タスクカウンタ 			*/
	st->max_late_time = 0;			/* 最大遅れ時間 						*/

	/* オーディオ処理関数ポインタの設定 */
	st->audio_process_fp = pcm_AudioProcess;

	/* １回のタスクで処理する量の上限の設定 */
	PCM_MeSet1TaskSample(hn, PCM_DEFAULT_SAMPLE_1TASK);

	return hn;
}

Bool PCM_MeIsCreate(PcmHn pcm)
{
	Uint32		i;

	i = (Uint32)(((Sint32)pcm - (Sint32)pcm_hn_tbl) >> 2);

#if	0
	/*
	**■1995-07-27	高橋智延
	**	i は unsigned なので、常に 0 以上
	*/
	if ((i >= 0) && (i < PCM_HN_MAX)) {
#else
	if( i < PCM_HN_MAX ){
#endif
		if (pcm_hn_mask & pcm_bit_msk[i]) {
			/* このハンドルは、クリエイトされている。 */
			return TRUE;
		} 
	}
	/* このハンドルは、クリエイトされていない。 */
	return FALSE;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ハンドルの消去
【引　数】
	hn　	（入力）　ハンドル
【戻り値】
	なし
*******************************************************************/
void PCM_MeDestroy(PcmHn hn)
{
	Uint32	i = (Uint32)(((Sint32)hn - (Sint32)pcm_hn_tbl) >> 2);

	/* システム状態チェック */
	PCM_CHK_SYSTEM_STATUS;

	if ((pcm_hn_mask & pcm_bit_msk[i]) == 0) {
		PCM_MeSetErrCode(PCM_ERR_INVALID_HN);
		return;
	}
	PCM_MeStop(hn);
	pcm_hn_mask &= ~pcm_bit_msk[i];
	if (pcm_hn_mask == 0) {
		/* クリエイトされたムービーが無いすきにカウンタをリセット。 */
		pcm_cnt_vbl_in = 0;
	}
	return;
}

/*******************************************************************
【機　能】
	メモリ再生／PCM_SetInfo のADPCM非対応処理
【引　数】
	hn　	（入力）　ハンドル
【戻り値】
	なし
【備　考】
	関数ポインタ pcm_set_info_adpcm_fp に設定されてコールされる
*******************************************************************/
void pcm_SetInfoAdpcmUnlink(PcmHn hn)
{
	/* ADPCM使用宣言がないのでAPCM形式ファイルを処理できない */
	PCM_MeSetErrCode(PCM_ERR_NOT_DECLARE_ADPCM);
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生情報の設定
【引　数】
	hn			(入力)　ハンドル
	info 		(入力)　再生情報
【戻り値】
	なし
【備　考】
	ヘッダがないファイルを再生する場合、または、ヘッダの情報によらず
　ここの指定により再生する場合に使用する。
	ハンドル作成後、スタートかエントリネクストする前にコールする。
*******************************************************************/
void PCM_MeSetInfo(PcmHn hn, PcmInfo *info)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmFilePara *filepara = &work->filepara;
#endif

	/* ヘッダの情報は無視する */
	st->ignore_header = PCM_ON;

	/* 再生情報 */
	st->info = *info;

	if (PCM_IS_ADPCM_SCT(st)) {
		(*pcm_set_info_adpcm_fp)(hn);
	}
	st->pcm_bsize = PCM_SAMPLE2BSIZE(st, para->pcm_size);
	st->pcm2_addr = para->pcm_addr + st->pcm_bsize;
}


Sint32 PCM_MeGetRingSize(PcmHn hn)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;

	return para->ring_size;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／
	リングバッファへの書き込みアドレス、書き込み可能サイズの取得
	　連続領域として書き込めるサイズを得る。リングバッファの先頭へ戻る
	場合は、それぞれに別に、取得、書き込み実行、報告を行う。
	　PCM_MeRenewRingWrite を参照。
【引　数】
	hn　			(入力)　ハンドル
	ring_write_addr (出力) 	書き込みアドレス
							書き込み可能サイズがゼロの場合は、０を設定する。
	write_size	 	(出力) 	連続書き込み可能サイズ [byte]
							これが、リングバッファ終了までのサイズである場合
							は、即時に後続のデータも書き込める。
	total_write_size (出力) 書き込み可能トータルサイズ [byte]
							ファイル制御の効率化のための情報。
【戻り値】
	なし
*******************************************************************/
void PCM_MeGetRingWrite(PcmHn hn, Sint8 **ring_write_addr, Sint32 *write_size, Sint32 *total_write_size)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st = &work->status;

	/* 書き込み可能トータルサイズ */
	*total_write_size = para->ring_size + st->ring_read_offset 
						- st->ring_write_offset;
	
	/* 連続書き込み可能サイズ */
	if (*total_write_size <= 0) {
		*write_size = 0;
		*ring_write_addr = NULL;
	} else {
		if (st->ring_write_addr >= st->ring_read_addr) {
			*write_size = st->ring_end_addr - st->ring_write_addr;
		} else {
			*write_size = *total_write_size;
		}
		*ring_write_addr = st->ring_write_addr;
	}
	return;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／
	リングバッファへの書き込みサイズの報告(書き込み位置更新)
	ユーザが供給したデータ量をライブラリに知らせる。
【引　数】
	hn　		(入力)　ハンドル
	write_size 	(入力)  書き込みサイズ [byte]
【戻り値】
	なし
*******************************************************************/
void PCM_MeRenewRingWrite(PcmHn hn, Sint32 write_size)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;

	if (write_size <= 0) {
		return;
	}

/*	if (st->play == PCM_STAT_PLAY_END) {
		PCM_MeSetErrCode(PCM_ERR_RING_SUPPLY);
	}
*/

	st->ring_write_offset += write_size;
	st->ring_write_addr += write_size;

	if (st->ring_write_addr >= st->ring_end_addr) {
		st->ring_write_addr -= para->ring_size;
	}
	return;
}

/* 読み取り位置更新 */
void pcm_RenewRingRead(PcmHn hn, Sint32 read_size)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st = &work->status;

	st->ring_read_offset += read_size;
	st->ring_read_addr += read_size;
	if (st->ring_read_addr >= st->ring_end_addr) {
		st->ring_read_addr -= para->ring_size;
	}

	return;
}

/* 読み取り位置更新(保留すべき場合は、更新保留にする) */
void pcm_RenewRingRead2(PcmHn hn, Sint32 size_sample)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmStatus	*st = &work->status;

	if (st->dma_hold == PCM_OFF) {
		/* 読み取り位置更新 */
		pcm_RenewRingRead(hn, size_sample);
	} else {
		/* 更新を保留する */
		st->buf_renew_size += size_sample;
	}
	return;
}


/*******************************************************************
【機　能】													公開関数
	メモリ再生／構造体ポインタ取得 : 起動パラメータ
【引　数】
	hn　		(入力)　ハンドル
【戻り値】
	起動パラメータ構造体ポインタ
*******************************************************************/
PcmPara *PCM_MeGetPara(PcmHn hn)
{
	PcmWork 	*work = *(PcmWork **)hn;

	return &work->para;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／構造体ポインタ取得 : ステータス
【引　数】
	hn　		(入力)　ハンドル
【戻り値】
	ステータス構造体ポインタ
*******************************************************************/
PcmStatus *PCM_MeGetStatus(PcmHn hn)
{
	PcmWork 	*work = *(PcmWork **)hn;

	return &work->status;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生開始
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
*******************************************************************/
void PCM_MeStart(PcmHn hn)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmStatus	*st = &work->status;

	st->play = PCM_STAT_PLAY_START;
	return;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生停止
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
*******************************************************************/
void PCM_MeStop(PcmHn hn)
{
	PcmWork		*work = *(PcmWork **)hn;
	PcmStatus	*st = &work->status;

	if (st->play == PCM_STAT_PLAY_TIME) {
		PCM_DrvStopPcm(hn); 	/* PCM再生停止 */
	}
	st->play = PCM_STAT_PLAY_END;
	st->cnt_loop = 0;	/* 停止させる */
	return;
}

/*******************************************************************
【機　能】
	メモリ再生／一時停止処理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
*******************************************************************/
STATIC void pcm_PauseOn(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	Sint32		write_to_pause, offset_rotate, size_flop;
#else
	Sint32		write_to_pause, offset_rotate;
#endif

	if (pcm_pwork_addr == NULL) {
		PCM_MeSetErrCode(PCM_ERR_PAUSE_WORK_NULL);
		return;
	}
	if (pcm_pwork_size == 0) {
		PCM_MeSetErrCode(PCM_ERR_PAUSE_WORK_SIZE);
		return;
	}

	st->vbl_pause = pcm_cnt_vbl_in;

	PCM_DrvStopPcm(hn); 	/* PCM再生停止 */

	/* ポーズ時サンプル数[sample]の更新 				*/
	/* ＰＣＭスタートから現在までの経過時間を加算する 	*/
	st->sample_pause += pcm_GetTimePcm(hn);

	/* 書き込み位置からポーズ位置までの戻り量[sample] */
	write_to_pause = st->sample_write - st->sample_pause;

	/* 回転させる量[byte]を求める */
	offset_rotate = st->pcm_write_offset 
					- PCM_SAMPLE2BSIZE(st, write_to_pause);
	offset_rotate += 256 * st->pcm_bsize;
	offset_rotate %= st->pcm_bsize;

	/* 回転処理 */
	if (PCM_IS_MONORAL(st)) {
		pcm_RotatePcmbuf((Uint32)pcm_pwork_addr, pcm_pwork_size, 
						(Uint32)para->pcm_addr, 
						st->pcm_bsize, offset_rotate);
	} else {
		pcm_RotatePcmbuf((Uint32)pcm_pwork_addr, pcm_pwork_size, 
						(Uint32)para->pcm_addr, 
						st->pcm_bsize, offset_rotate);
		pcm_RotatePcmbuf((Uint32)pcm_pwork_addr, pcm_pwork_size, 
						(Uint32)st->pcm2_addr, 
						st->pcm_bsize, offset_rotate);
	}

	/* 回転した分だけ、書き込み位置を調整する */
	st->pcm_write_offset -= offset_rotate;
	if (st->pcm_write_offset < 0) {
		st->pcm_write_offset += st->pcm_bsize;
	}

	st->play = PCM_STAT_PLAY_PAUSE;
}

/*******************************************************************
【機　能】
	メモリ再生／一時停止解除処理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
*******************************************************************/
STATIC void pcm_PauseOff(PcmHn hn)
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
	int			imask;

    imask = get_imask();
    set_imask(15);

	st->clock_scale = PCM_CLOCK_SCALE;

	PCM_DrvStartPcm(hn); 	/* PCM再生再開 */

	/* タイマースタート */
	pcm_StartClock(hn);
	st->play = PCM_STAT_PLAY_TIME;
	st->cnt_4ksample = 0;
	st->vbl_pcm_start = pcm_cnt_vbl_in;
	st->vbl_film_start += st->vbl_pcm_start - st->vbl_pause;

    set_imask(imask);
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生の一時停止制御
【引　数】
	hn			(入力)　ハンドル
	cmd			(入力)　一時停止の制御コマンド
						PCM_PAUSE_ON_AT_ONCE	: 即時一時停止
						PCM_PAUSE_OFF			: 一時停止解除
【戻り値】
	なし
*******************************************************************/
void PCM_MePause(PcmHn hn, PcmPauseCmd cmd)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	switch (st->play) {
	case PCM_STAT_PLAY_TIME:
		if (cmd == PCM_PAUSE_ON_AT_ONCE) {
			pcm_PauseOn(hn);	/* 一時停止処理 */
		} /* else 再生中の「ポーズＯＦＦ」は無視。エラーにもしない。 */
		break;
	case PCM_STAT_PLAY_PAUSE:
		if (cmd == PCM_PAUSE_OFF) {
			pcm_PauseOff(hn);	/* 一時停止解除処理 */
		} /* else ポーズ中の「ポーズＯＮ」は無視。エラーにもしない。 */
		break;
	default:
		/* PCM_MeSetErrCode(PCM_ERR_PAUSE_STATUS); */
		break;
	}
	return;
}

/* タスク初期処理その１：ヘッダ処理 */
STATIC Sint32 pcm_HeaderProcess(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	Uint32		ch_id;

	if (st->info.file_type == PCM_FILE_TYPE_NO_HEADER) {
		/* ヘッダなし。ｃｉ要求がなければヘッダ処理済みとする。 */
		if (st->need_ci == PCM_OFF) {
			st->play = PCM_STAT_PLAY_HEADER;
			return 0;
		} else {
			return -1;
		}
	}

	/* 先頭の識別子４バイトが、バッファ中にあるか？ */
	if (st->ring_write_offset < 4) {
		return -1;
	}

	ch_id = *(Sint32 *)para->ring_addr;
	if (ch_id == PCM_ID_FORM) {
		if (pcm_AifHeaderProcess(hn)) {
			return -1;
		}
	} else {
		/* Ver1.00 で対応するのは、AIFF，AFIC フォーマットのみ */
		PCM_MeSetErrCode(PCM_ERR_HEADER_DATA);
		return -1;
	}

	st->pcm_bsize = PCM_SAMPLE2BSIZE(st, para->pcm_size);
	st->pcm2_addr = para->pcm_addr + st->pcm_bsize;

	/* リングバッファ読み取り位置更新 */
	pcm_RenewRingRead(hn, st->media_offset);

	/* １回のタスクで処理する量の上限[byte/1ch]	*/
	st->onetask_size = PCM_SAMPLE2BSIZE(st, st->onetask_sample);

	st->play = PCM_STAT_PLAY_HEADER;

	return 0;
}

/*******************************************************************
【機　能】
	タスク初期処理その２：タイマースタート
	開始条件が整ったら、PCM再生開始、タイマースタートを行う
	開始条件１．リングバッファにトリガサイズ以上データが備蓄された。
	開始条件２．ＰＣＭバッファにトリガサイズ以上データが備蓄された。
【引　数】
*******************************************************************/
STATIC void pcm_StartTimer(PcmHn hn)
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
	int			imask;

	if (st->ring_write_offset < PCM_HN_START_TRG_SIZE(hn)) {
		return;
	}

	if (st->sample_write < PCM_HN_START_TRG_SAMPLE(hn)) {
		return;
	}

    imask = get_imask();
    set_imask(15);

	st->clock_scale = PCM_CLOCK_SCALE;

	PCM_DrvStartPcm(hn); 	/* PCM再生開始 */

	/* タイマースタート */
	pcm_StartClock(hn);
	st->play = PCM_STAT_PLAY_TIME;
	st->cnt_4ksample = 0;
	st->vbl_film_start = st->vbl_pcm_start = pcm_cnt_vbl_in;

    set_imask(imask);
}

Sint32 PCM_MeIsNotSetCi(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (st->need_ci == PCM_ON) {
		return 1;
	} else {
		return 0;
	}
}

/* 
	hn			(入力)　ハンドル
	ci 			(入力)　コーディング情報比較値	
*/
void PCM_MeSetCi(PcmHn hn, Uint8 ci)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if ((ci & 1) == 0) {
		PCM_INFO_CHANNEL(&st->info) = 1;		/* monoral */
	} else {
		PCM_INFO_CHANNEL(&st->info) = 2;		/* stereo */
	}
	if ((ci & 4) == 0) {
		PCM_INFO_SAMPLING_RATE(&st->info) = PCM_FREQ_37800;	/* 37.8 [kHz] */
	} else {
		PCM_INFO_SAMPLING_RATE(&st->info) = PCM_FREQ_18900;	/* 18.9 [kHz] */
	}
	st->need_ci = PCM_OFF;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ＰＡＮの指定
【引　数】
	hn			(入力)　ハンドル
	pan			(入力)　ＰＡＮ 0..31 (３２段階)
						意味はサウンドドライバの仕様書と同じ。
【戻り値】
	なし
【備　考】
	ＰＣＭ再生が開始し、再生している途中でも指定できる。
*******************************************************************/
void PCM_MeSetPan(PcmHn hn, Uint32 pan)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;

	para->pcm_pan = pan;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ボリュームの指定
【引　数】
	hn			(入力)　ハンドル
	level		(入力)　レベル 0..7 (８段階)
						意味はサウンドドライバの仕様書と同じ。
【戻り値】
	なし
【備　考】
	ＰＣＭ再生が開始し、再生している途中でも指定できる。
*******************************************************************/
void PCM_MeSetVolume(PcmHn hn, Uint32 level)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;

	para->pcm_level = level;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ＰＣＭ再生パラメータの変更
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【備　考】
	ＰＣＭ再生中に変更したい場合にコールする。
*******************************************************************/
void PCM_MeChangePcmPara(PcmHn hn)
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

	if (st->play == PCM_STAT_PLAY_TIME) {
		PCM_DrvChangePcmPara(hn, -1, -1);
	}
}

/*******************************************************************
【機　能】
	オーディオデータの条件比較
	適合条件：
		・PCMｽﾄﾘｰﾑ再生番号(ｻｳﾝﾄﾞﾄﾞﾗｲﾊﾞ用)が等しい
		・PCMﾊﾞｯﾌｧ(ｱﾄﾞﾚｽ、ｻｲｽﾞ)が等しい
		・ﾁｬﾈﾙ数(ｽﾃﾚｵ、ﾓﾉﾗﾙ)が等しい
		・ｻﾝﾌﾟﾘﾝｸﾞﾋﾞｯﾄ数が等しい
		・ｻﾝﾌﾟﾘﾝｸﾞ周波数が等しい
【マクロ評価値】
	1			：オーディオデータの条件が適合しない。
	0			：オーディオデータの条件が適合している。
*******************************************************************/
#define NOT_EQUAL_AUDIO(para_e, st_e, para_s, st_s)						\
	(	((para_e)->pcm_stream_no 		!= (para_s)->pcm_stream_no)		\
	  ||((para_e)->pcm_addr 			!= (para_s)->pcm_addr)			\
	  ||((para_e)->pcm_size 			!= (para_s)->pcm_size)			\
	  ||((st_e)->info.channel 			!= (st_s)->info.channel)		\
	  ||((st_e)->info.sampling_bit 		!= (st_s)->info.sampling_bit)	\
	  ||((st_e)->info.sampling_rate 	!= (st_s)->info.sampling_rate)	\
	)

/*******************************************************************
【機　能】
	メモリ再生／連続再生ムービーのスタート
【引　数】
	hn_e			(入力)　再生を終了するムービーのハンドル
	hn_s			(入力)　再生を開始するムービーのハンドル
【戻り値】
	0			：連続再生できた。
	-1			：連続再生できなかった。
【注　意】
*******************************************************************/
Sint32 pcm_StartNext(PcmHn hn_e, PcmHn hn_s)
{
	PcmWork		*work_e = *(PcmWork **)hn_e;
	PcmPara		*para_e = &work_e->para;
	PcmStatus	*st_e 	= &work_e->status;
	PcmWork		*work_s = *(PcmWork **)hn_s;
	PcmPara		*para_s = &work_s->para;
	PcmStatus	*st_s 	= &work_s->status;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	Sint32		finish_vbl, finish_time, finish_read;
#endif

	if (st_s->play != PCM_STAT_PLAY_CREATE) {
		/* 連続再生の開始させるムービーは初期状態でなければならない */
		PCM_MeSetErrCode(PCM_ERR_NEXT_HN_STATUS);
		return -1;
	}

	/* 連続再生の開始させるムービーを開始状態にする */
	PCM_MeStart(hn_s);

	if (pcm_HeaderProcess(hn_s)) {
		/* まだヘッダ分のデータ供給がない */
		VTV_PRINTF((VTV_s, "P:Seamless NG\n"));
		return -1;
	}

	if (NOT_EQUAL_AUDIO(para_e, st_e, para_s, st_s)) {
		/* オーディオデータの条件が一致していない */
		PCM_MeSetErrCode(PCM_ERR_NEXT_HN_AUDIO);
		return -1;
	}

	VTV_PRINTF((VTV_s, "P:Seamless OK\n"));

	/********************************************************************
		チェック完了。これならスムーズ連続再生できる。
	********************************************************************/

	/* 連続再生の開始させるムービーの PAN, LEVEL 値に変更する */
	/* PCM_DrvChangePcmPara(hn_e, -1, -1); */
	/* いや、再生中のムービーの PAN, LEVEL 値を引き継ぐ */
	para_s->pcm_pan = para_e->pcm_pan;
	para_s->pcm_level = para_e->pcm_level;

	/* 現在再生中のオーディオ書き込みアドレスを引き継ぐ */
	st_s->pcm_write_offset = st_e->pcm_write_offset;

	/* 供給サンプル数[sample]を引き継ぐ */
	st_s->sample_write = st_e->sample_write;

	/* ポーズ時サンプル数[sample]を引き継ぐ */
	st_s->sample_pause = st_e->sample_pause;

	/* フィルム開始時サンプル数[sample]を設定する */
	st_s->sample_film_start = st_s->sample_write;

	/* フィルム開始時刻[vbl]を設定する
	 * (開始させるムービーはオーディオ付きだから、この値は、
	 *  利用されないはず。大まかな値を設定しておく。        )
	 */
	st_s->vbl_film_start = pcm_cnt_vbl_in;

	/* PCM開始時刻を引き継ぐ */
	st_s->vbl_pcm_start = st_e->vbl_pcm_start;

	/* 4kｻﾝﾌﾟﾙ単位更新ｶｳﾝﾀを引き継ぐ */
	st_s->cnt_4ksample = st_e->cnt_4ksample;

	/* ＣＰＵクロックタイマ開始時刻を引き継ぐ */
	st_s->count_start = st_e->count_start;

	/* ＣＰＵクロックタイマのスケールを引き継ぐ */
	st_s->clock_scale = st_e->clock_scale;

	/* タイマースタート */
	st_s->play = PCM_STAT_PLAY_TIME;

	return 0;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／ループ回数の指定
【引　数】
	hn			(入力)　ハンドル。
	cnt_loop	(入力)　ループ回数。
【戻り値】
	なし
*******************************************************************/
void PCM_MeSetLoop(PcmHn hn, Sint32 cnt_loop)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	st->cnt_loop = cnt_loop;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／次に再生するムービーの登録
【引　数】
	hn			(入力)　ハンドル。
						NULL の場合は、登録をキャンセルする。ただし、
						再生が開始された後では、キャンセルできない。
【戻り値】
	なし
【注　意】
	これを使用するハンドルには、PCM_MeStart は決して使用しないこと。
	再生中のムービーの終了か、または、PCM_MeChange による強制切り
　替えが再生開始のトリガになる。
*******************************************************************/
void PCM_MeEntryNext(PcmHn hn)
{
	pcm_hn_next = hn;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／登録ハンドルの取得
【引　数】
	なし
【戻り値】
	登録ハンドル
【注　意】
*******************************************************************/
PcmHn PCM_MeGetEntryNext(void)
{
	return pcm_hn_next;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／強制切り替え可能性の確認
【引　数】
	なし
【戻り値】
	PCM_CHANGE_OK_AT_ONCE		= 0 ：即時切り替え可能。
	PCM_CHANGE_NO_DATA			= 1 ：データ供給不足。
	PCM_CHANGE_NO_ENTRY			= 2 ：エントリが無い。
【注　意】
*******************************************************************/
PcmChangeStatus PCM_MeCheckChange(void)
{
	PcmWork		*work;
	PcmPara		*para;
	PcmStatus	*st;

	if (pcm_hn_next == NULL) {
		return PCM_CHANGE_NO_ENTRY;		/* エントリが無い。切り替え不可	*/
	}

	work 	= *(PcmWork **)pcm_hn_next;
	st 		= &work->status;
	para 	= &work->para;

	if ((st->ring_write_offset <= 0) ||
		(st->ring_write_offset < PCM_HN_START_TRG_SIZE(pcm_hn_next))) {
		/* トリガサイズ分の供給がまだない。*/
		return PCM_CHANGE_NO_DATA;
	} else {
		return PCM_CHANGE_OK_AT_ONCE;
	}
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／再生ムービーの強制切り替え
【引　数】
	hn_e 		(入力)　再生を終了するムービーのハンドル
						NULL の場合は、登録されたムービーを再生開始する
【戻り値】
	なし
【注　意】
	　これを使用する場合は、あらかじめ PCM_MeEntryNext を使用して、
	切り替えにより再生を開始させるムービーを登録しておくこと。
	　切り替えがスムーズになる条件を次に示す。
		１．切り替えるムービーがメモリに用意されていること。
			切り替えるムービーのデータ供給がスムーズ切り替えに十分で
			あるかを確認したい場合は、PCM_MeCheckChange を使用する。
		２．オーディオデータの周波数、ビット数などが等しいこと。
*******************************************************************/
void PCM_MeChange(PcmHn hn_e)
{
	if (hn_e != NULL) {
		PCM_MeStop(hn_e);
		PCM_MeSetLoop(hn_e, 0);		/* ループを防ぐ */
	}
	PCM_MeStart(pcm_hn_next);
	pcm_hn_next = NULL;
}

/*******************************************************************
【機　能】
	メモリ再生／有効ハンドルの取得
【引　数】
	i			(入力) ハンドル番号。0..31
【戻り値】
	NULL以外	：ハンドル番号に対応するハンドル値。
	NULL		：ハンドル番号のハンドルはクリエイトしていない。
【注　意】
*******************************************************************/
PcmHn PCM_MeGetHandle(Uint32 i)
{
	if (pcm_hn_mask & pcm_bit_msk[i]) {
		return (PcmHn)(pcm_hn_tbl + i);
	} else {
		return NULL;
	}
}

/* ハンドル番号の取得 */
Sint32 PCM_MeGetHandleNo(PcmHn hn)
{
	return ((Sint32)hn - (Sint32)pcm_hn_tbl) >> 2;
}

/* 再生終了処理 */
void pcm_EndProcess(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;

	if (st->info.sample_file != -1) {
		if (st->info.sample_file > st->sample_write_file) {
			/* 再生サンプル数が既知、供給量がこれに達していない。*/
			VTV_PRINTF((VTV_s, "P:EmptyStop\n full%X > play%X", 
				st->info.sample_file, st->sample_write_file));
		}
	}

	if ((pcm_hn_next == NULL)					/* 連続再生しない 	*/
	 || (pcm_StartNext(hn, pcm_hn_next))) {		/* 連続再生できない */
		PCM_DrvStopPcm(hn);/* PCM再生停止 */
	}
	if (pcm_hn_next != NULL) {
		pcm_hn_next = NULL;
		PCM_MeSetLoop(hn, 0);		/* ループを防ぐ */
	}
	st->play = PCM_STAT_PLAY_END;
}

/* リングバッファに有効な在庫がなければ「真」 */
Sint32 PCM_IsRingEmpty(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmStatus	*st 	= &work->status;
	Sint32		stock;

	stock = (st)->ring_write_offset - (st)->ring_read_offset;

	if (stock == 0) {
		/* 正真証明のからっぽ */
		return 1;
	}

	if (st->info.sample_file != -1) {
		if (st->info.sample_file <= st->sample_write_file) {
			/* 再生サンプル数が既知の場合、供給量がこれに達していれば、
			 * リングバッファの在庫は無効である。
			 */
			return 1;
		}
	}

	/* 空ではない */
	return 0;
}

/*******************************************************************
【機　能】													公開関数
	メモリ再生／タスク処理
【引　数】
	hn			(入力)　ハンドル
【戻り値】
	なし
【注　意】
	少なくとも、ＰＣＭバッファの半分の再生時間より多い頻度で定期的
　にコールすること。
*******************************************************************/
void PCM_MeTask(PcmHn hn)
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
	Sint32		sample_now;

	st->cnt_task_call++;	/* タスクコールカウンタのカウントアップ */

	/* 再生ステータスチェック */
	if ((st->play <= PCM_STAT_PLAY_PAUSE)
	  ||(st->play >= PCM_STAT_PLAY_END)) {
		return;
	}

	/* エラー状態チェック */
	if (pcm_err_code != PCM_ERR_OK) {
		PCM_MeStop(hn);
		st->play = PCM_STAT_PLAY_ERR_STOP;
		return;
	}

	/* ヘッダ処理 */
	if (st->play == PCM_STAT_PLAY_START) {
		if (pcm_HeaderProcess(hn)) {
			return;	/* まだヘッダ分のデータ供給がない */
		}
	}

	/* オーディオ処理 */
	(*st->audio_process_fp)(hn);

	/* 再生終了処理 */
	if (st->play == PCM_STAT_PLAY_TIME) {
		if (PCM_IsRingEmpty(hn)) {
			PCM_MeGetTimeTotal(hn, &sample_now);
			if (sample_now + PCM_HN_STOP_TRG_SAMPLE(hn) > st->sample_write) {
				/* さあ、再生終了だ！ */
				pcm_EndProcess(hn);
			}
		}
	}

	/* タイマスタート・ＰＣＭ再生スタート処理 */
	if (st->play == PCM_STAT_PLAY_HEADER) {
		pcm_StartTimer(hn);
	}
	return;
}
