/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ライブラリインタフェース
 * File		:pcm_lib.c
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
/*-------------------- 《インクルードファイル》 --------------------*/
#include <sega_pcm.h>
#include "pcm_mem.h"
#include "pcm_lib.h"

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ほかのファイルの関数を参照しているので追加
	*/
	#include	"pcm_msub.h"
#endif

/*------------------------- 《マクロ定数》 -------------------------*/
/*------------------------- 《処理マクロ》 -------------------------*/
/*-------------------------- 《データ型》 --------------------------*/
/* プレロード関数 */
typedef Sint32 (*PreloadFunc)(PcmHn pcm, Sint32 size);

/*-------------------------- 《関数宣言》 --------------------------*/
static void memTask(PcmHn pcm);
static Sint32 memPreloadFile(PcmHn pcm, Sint32 size);
static void memSetLoadNum(PcmHn pcm, Sint32 load_sct);
static void memSetTrMode(PcmHn pcm, PcmTrMode mode);

/*-------------------------- 《変数定義》 --------------------------*/
/* PCM_STM の初期化時のフラグ */
Bool PCM_lib_stm_init;

/*-------------------------- 《関数定義》 --------------------------*/
/********************************************************************/
/* ライブラリの初期化 												*/
/* [関数値]															*/
/*    正常に初期化できた場合 TRUE									*/
/*    正常に初期化できない場合 FALSE								*/
/********************************************************************/
Bool PCM_Init(void)
{
	PCM_lib_stm_init = OFF;
	PCM_MeInit();
	return TRUE;
}

/********************************************************************/
/* ライブラリの終了処理 											*/
/********************************************************************/
void PCM_Finish(void)
{
	PCM_MeFinish();
}

/********************************************************************/
/* ハンドルの作成（メモリ）											*/
/* [入力]															*/
/*    para : 作成パラメータ											*/
/* [関数値]															*/
/*    ハンドル（作成できない場合は NULL)							*/
/********************************************************************/
PcmHn PCM_CreateMemHandle(PcmCreatePara *para)
{
	PcmPara 	me_para;
	PcmHn   	pcm;

	/* 作成パラメータのチェック */
	PCMLIB_CHK_CREATE_PARA(para);

	/* リングバッファ */
	me_para.ring_addr = PCM_PARA_RING_ADDR(para);
	me_para.ring_size = PCM_PARA_RING_SIZE(para);

	/* トリガサイズ */
	me_para.start_trg_size 		= PCM_DEFAULT_SIZE_START_TRG;
	me_para.start_trg_sample 	= PCM_DEFAULT_SAMPLE_START_TRG;
	me_para.stop_trg_sample 	= PCM_DEFAULT_SAMPLE_STOP_TRG;

	/* サウンドドライバコマンドブロック番号 */
	me_para.command_blk_no = PCMLIB_COMMAND_BLK_NO;

	/* ＰＣＭストリーム再生番号 */
	me_para.pcm_stream_no = PCMLIB_PCM_STREAM_NO;

	/* ＰＣＭバッファ */
	me_para.pcm_addr = PCM_PARA_PCM_ADDR(para);
	me_para.pcm_size = PCM_PARA_PCM_SIZE(para);

	/* ＰＡＮとボリューム */
	me_para.pcm_pan = PCMLIB_PCM_PAN;
	me_para.pcm_level = PCMLIB_PCM_LEVEL;

	/* ハンドルの作成 */
	pcm = PCM_MeCreate((PcmWork *)PCM_PARA_WORK(para), &me_para);
	PCMLIB_FACCESS_TYPE(pcm) = PCMLIB_FACCESS_MEM;

	/* 実行関数の設定 */
	PCMLIB_SET_START_FUNC(pcm, PCM_MeStart);
	PCMLIB_SET_TASK_FUNC(pcm, memTask);
	PCMLIB_SET_PRELOAD_FILE_FUNC(pcm, memPreloadFile);
	PCMLIB_SET_SET_LOAD_NUM_FUNC(pcm, memSetLoadNum);
	PCMLIB_SET_SET_TRMODE_FUNC(pcm, memSetTrMode);
	return pcm;
}

/********************************************************************/
/* ハンドルの消去（メモリ）											*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_DestroyMemHandle(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (PCMLIB_FACCESS_TYPE(pcm) != PCMLIB_FACCESS_MEM) {
		PCM_MeSetErrCode(PCM_ERR_ILL_CREATE_MODE);
	}

	PCM_MeDestroy(pcm);
}


/********************************************************************/
/* 再生タスク														*/
/*   再生中は定期的にこの関数を呼ぶ									*/
/*   呼ぶ間隔はＰＣＭバッファの半分の再生時間以下で定期的に呼ぶ		*/
/*   必要頻度より少ないと再生が乱れる								*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/*     NULL が指定された時は現在オープンされているハンドル   	 */
/*     をすべての再生中処理をする									*/
/********************************************************************/
void memTask(PcmHn pcm)
{
	Sint32		ring_size;

	/* タスク処理 */
	PCM_MeTask(pcm);

	/* ループ再生制御 */
	if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {
		if (--PCM_HN_CNT_LOOP(pcm) > 0) {
			/* メモリ再生の場合は常駐データだから、ループ再生の場合、
			 * アプリに代わってリングバッファ供給量報告を行う。
			 */
			ring_size = PCM_MeGetRingSize(pcm);
			PCM_MeReset(pcm);
			PCM_MeRenewRingWrite(pcm, ring_size);
			PCM_MeStart(pcm);
		}
	}
}

/********************************************************************/
/* 再生タスク 														*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    NULL の場合は再生しているハンドルすべてに再生タスクを実行する	*/
/********************************************************************/
void PCM_Task(PcmHn pcm)
{
	int 	i;

	if (pcm == NULL) {
		for (i = 0; i < PCM_HN_MAX; i++) {
			pcm = PCM_MeGetHandle(i);
			if (pcm != NULL) {
				/* 実行関数の呼出 */
				PCMLIB_TASK_FUNC(pcm)(pcm);
			}
		}
	} else {
		/* 実行関数の呼出 */
		PCMLIB_TASK_FUNC(pcm)(pcm);
	}
}


/********************************************************************/
/* ＶブランクＩＮ処理関数 											*/
/********************************************************************/
void PCM_VblIn(void)
{
	PCM_MeVblIn();
}


/********************************************************************/
/* 再生開始 														*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_Start(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	/* 実行関数の呼出 */
	PCMLIB_START_FUNC(pcm)(pcm);
}


/********************************************************************/
/* 再生停止 														*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_Stop(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeStop(pcm);
}


/********************************************************************/
/* 一時停止															*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    cmd  : 制御コマンド											*/
/********************************************************************/
void PCM_Pause(PcmHn pcm, PcmPauseCmd cmd)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MePause(pcm, cmd);
}



/********************************************************************/
/* 再生時刻の取得													*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/* [関数値]															*/
/*    カレント時刻（再生サンプル数）								*/
/********************************************************************/
Sint32 PCM_GetTime(PcmHn pcm)
{
	Sint32	sample_now;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, 0);

	PCM_MeGetTime(pcm, &sample_now);
	return sample_now;
}

/********************************************************************/
/* タイムスケールの取得												*/
/* [入力]															*/
/*    pcm : ハンドル												*/
/* [関数値]															*/
/*    タイムスケール												*/
/*    未確定の場合は０を返す										*/
/********************************************************************/
Sint32 PCM_GetTimeScale(PcmHn pcm)
{
	PcmStatus 	*st;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, 0);

	st = PCM_MeGetStatus(pcm);
	switch (st->play) {
	case PCM_STAT_PLAY_ERR_STOP:
	case PCM_STAT_PLAY_CREATE:
	case PCM_STAT_PLAY_START:
		return 0;
	}
	return PCM_INFO_SAMPLING_RATE(&st->info);
}

/********************************************************************/
/* １回のタスクで処理する量の上限の設定 							*/
/* [入力]															*/
/*    pcm     : ハンドル											*/
/*    sample  : １回のタスクで処理する量の上限[sample/1ch]			*/
/* [備考]															*/
/*  ・制限したくなければ、適当に大きな数を設定する。				*/
/*  ・小さすぎると正常に再生できなくなるので、タスクをコールする	*/
/*    頻度と再生周波数を考えて、余裕がある値に設定すること。		*/
/*  ・再生中の変更も可能だが、タスクの負荷の安定のためには、再生	*/
/*    開始前に設定するのが望ましい。								*/
/********************************************************************/
void PCM_Set1TaskSample(PcmHn pcm, Sint32 sample)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);
	PCM_MeSet1TaskSample(pcm, sample);
}

/********************************************************************/
/* 再生開始トリガサイズの設定 										*/
/* [入力]															*/
/*    pcm     : ハンドル											*/
/*    size    : リングバッファへの供給量[byte]						*/
/* [備考]															*/
/*  ・size より大きいデータ供給が、再生開始の必要条件となる。		*/
/*  ・リングバッファサイズ未満でなければならない。					*/
/*  ・再生ファイルサイズ未満でなければならない。					*/
/*  ・再生開始前に設定する。										*/
/* [目的]															*/
/*    再生ライブラリは、供給データ分の再生終了により再生停止を行う	*/
/*  ので、リングバッファへのデータ供給を安定してできない場合、途中	*/
/*  で再生停止してしまう場合がある。この設定をすればある程度安全性	*/
/*  が向上する。													*/
/*    この設定量に応じた、開始時刻の遅延を考慮しなければならない。	*/
/********************************************************************/
void PCM_SetStartTrgSize(PcmHn pcm, Sint32 size)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);
	PCM_MeSetStartTrgSize(pcm, size);
}

/********************************************************************/
/* 再生開始トリガサンプルの設定 									*/
/* [入力]															*/
/*    pcm     : ハンドル											*/
/*    sample  : ＰＣＭバッファへの供給量[sample/1ch]				*/
/* [備考]															*/
/*  ・sample 以上の供給が、再生開始の必要条件となる。				*/
/*  ・再生開始前に設定する。										*/
/********************************************************************/
void PCM_SetStartTrgSample(PcmHn pcm, Sint32 sample)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);
	PCM_MeSetStartTrgSample(pcm, sample);
}

/********************************************************************/
/* 再生停止トリガサンプルの設定 									*/
/* [入力]															*/
/*    pcm     : ハンドル											*/
/*    sample  : ＰＣＭバッファの残り量[sample/1ch]					*/
/* [備考]															*/
/*  ・sample 以下の残り量が、再生停止の条件となる。					*/
/*  ・再生開始前に設定する。										*/
/* [目的]															*/
/*  　タスクコールの頻度が低いと停止時にノイズが入る。どうしても	*/
/*  頻度を高くできない場合に、デフォルト値より大きめに設定すれば、	*/
/*  これを回避できる。												*/
/********************************************************************/
void PCM_SetStopTrgSample(PcmHn pcm, Sint32 sample)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);
	PCM_MeSetStopTrgSample(pcm, sample);
}


/********************************************************************/
/* ＰＣＭコマンドブロック番号の設定									*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    blk_no  : コマンドブロック番号（０～７）						*/
/********************************************************************/
void PCM_SetPcmCmdBlockNo(PcmHn pcm, Sint32 blk_no)
{
	PcmPara *para;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (blk_no < 0 || blk_no > 7) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return;
	}

	para = PCM_MeGetPara(pcm);
	para->command_blk_no = blk_no;
}


/********************************************************************/
/* ストリーム再生番号の設定 										*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    stream_no  : ストリーム再生番号（０～７）						*/
/********************************************************************/
void PCM_SetPcmStreamNo(PcmHn pcm, Sint32 stream_no)
{
	PcmPara *para;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (stream_no < 0 || stream_no > 7) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return;
	}

	para = PCM_MeGetPara(pcm);
	para->pcm_stream_no = stream_no;
}



/********************************************************************/
/* 再生ＰＡＮの設定 												*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    pan  : ＰＡＮの値（０～３１）									*/
/********************************************************************/
void PCM_SetPan(PcmHn pcm, Sint32 pan)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeSetPan(pcm, pan);
}

/********************************************************************/
/* 再生ボリュームの設定 											*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    volume : ボリューム（０～７）									*/
/********************************************************************/
void PCM_SetVolume(PcmHn pcm, Sint32 volume)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeSetVolume(pcm, volume);
}

/********************************************************************/
/* ＰＣＭ再生パラメータの変更	 									*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_ChangePcmPara(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeChangePcmPara(pcm);
}

/********************************************************************/
/* ＰＣＭ再生情報の設定 											*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    info : 再生情報												*/
/********************************************************************/
void PCM_SetInfo(PcmHn pcm, PcmInfo *info)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeSetInfo(pcm, info);
}

/********************************************************************/
/* ポーズ処理用ワークアドレス、サイズの設定 						*/
/* [入力]															*/
/*    addr : ポーズ処理用ワークアドレス								*/
/*    size : ポーズ処理用ワークサイズ								*/
/********************************************************************/
void PCM_SetPauseWork(Sint32 *addr, Sint32 size)
{
	PCM_MeSetPauseWork(addr, size);
}

/********************************************************************/
/* ループ回数の指定 												*/
/* [入力]															*/
/*    pcm       : ハンドル											*/
/*    cnt_loop  : ループ回数										*/
/********************************************************************/
void PCM_SetLoop(PcmHn pcm, Sint32 cnt_loop)
{
	PCM_MeSetLoop(pcm, cnt_loop);
}

/********************************************************************/
/* 次に再生するハンドルの登録 										*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_EntryNext(PcmHn pcm)
{
	PCM_MeEntryNext(pcm);
}

/********************************************************************/
/* ハンドルの強制切り替え 											*/
/********************************************************************/
void PCM_Change(void)
{
	PcmChangeStatus status;
	PcmHn	before_pcm, next_pcm;
	int		i;

	before_pcm = NULL;
	next_pcm = PCM_MeGetEntryNext();
	for (i = 0; i < PCM_HN_MAX; i++) {
		before_pcm = PCM_MeGetHandle(i);
		if (before_pcm != next_pcm && before_pcm != NULL) {
			break;
		}
	}

	status = PCM_MeCheckChange();
	switch (status) {
	case PCM_CHANGE_OK_AT_ONCE:
		/* 即時切り替え可能 */
	case PCM_CHANGE_NO_DATA:
		/* データ供給不足 */
		PCM_MeChange(before_pcm);
		break;
	case PCM_CHANGE_TOO_LARGE_HEADER:
		/* ヘッダが大きすぎる */
		PCM_MeSetErrCode(PCM_ERR_TOO_LARGE_HEADER);
		break;
	case PCM_CHANGE_NO_ENTRY:
	default:
		/* エントリがない */
		PCM_MeSetErrCode(PCM_ERR_CHANGE_NO_ENTRY);
		break;
	}
}


/********************************************************************/
/* ハンドルの強制切り替え状態の取得									*/
/********************************************************************/
PcmChangeStatus PCM_CheckChange(void)
{
	return PCM_MeCheckChange();
}


/********************************************************************/
/* ＣＤバッファからの最大転送セクタ数を設定する						*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    load_sct : 最大転送セクタ数									*/
/********************************************************************/
void PCM_SetLoadNum(PcmHn pcm, Sint32 load_sct)
{
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara *para;
#endif
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);
	if (load_sct <= 0) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return;
	}

	/* 実行関数の呼出 */
	PCMLIB_SET_LOAD_NUM_FUNC(pcm)(pcm, load_sct);
}


/********************************************************************/
/* 再生ステータスの取得												*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/* [関数値]															*/
/*    再生ステータス												*/
/********************************************************************/
PcmPlayStatus PCM_GetPlayStatus(PcmHn pcm)
{
	PcmStatus 	*st;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, PCM_STAT_PLAY_ERR_STOP);

	st = PCM_MeGetStatus(pcm);
	return st->play;
}

/********************************************************************/
/* エラー情報の取得													*/
/********************************************************************/
PcmErrCode PCM_GetErr(void)
{
	return PCM_MeGetErrCode();
}


/********************************************************************/
/* エラー関数の登録													*/
/* [入力]															*/
/*    func : エラー関数												*/
/*    obj  : 登録オブジェクト										*/
/********************************************************************/
void PCM_SetErrFunc(PcmErrFunc func, void *obj)
{
	PCM_MeSetErrFunc(func, obj);
}

/********************************************************************/
/* 書き込みバッファアドレス、書き込み可能サイズの取得				*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/* [出力]															*/
/*    free_size  : 連続書き込み可能バイト数							*/
/*                 連続領域の書き込み可能のサイズ					*/
/*    free_total : 書き込み可能総バイト数							*/
/*                 不連続領域も合わせた書き込み可能合計サイズ		*/
/* [関数値]															*/
/*     書き込みバッファアドレス										*/
/*     書き込み可能サイズが０時は NULL 								*/
/********************************************************************/
Uint32 *PCM_GetWriteBuf(PcmHn pcm, Sint32 *free_size, Sint32 *free_total)
{
	Sint8 	*write_addr;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, NULL);

	PCM_MeGetRingWrite(pcm, &write_addr, free_size, free_total);
	return (Uint32 *)write_addr;
}


/********************************************************************/
/* バッファへの書き込みサイズの通知									*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    write_size : バッファに書き込んだバイト数						*/
/********************************************************************/
void PCM_NotifyWriteSize(PcmHn pcm, Sint32 write_size)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (write_size < 0) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return;
	}

	PCM_MeRenewRingWrite(pcm, write_size);
}

/********************************************************************/
/* メモリにファイルを読み込む										*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    size : 読み込むバイト数										*/
/*           リングバッファが読み込みバイト数より小さい時は			*/
/*           バッファサイズになる。									*/
/*           読み込むバイト数はセクタ単位（２０４８の倍数）になる。	*/
/* [関数値]															*/
/*    実際に読み込んだバイト数										*/
/********************************************************************/
Sint32 PCM_PreloadFile(PcmHn pcm, Sint32 size)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, 0);
	if (size <= 0) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return 0;
	}

	/* 実行関数の呼出 */
	return PCMLIB_PRELOAD_FILE_FUNC(pcm)(pcm, size);
}


/********************************************************************/
/* メモリ再生時のファイル読み込み登録関数							*/
/*   実際は、何もしないダミー関数									*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    size : 読み込むバイト数										*/
/* [関数値]															*/
/*    ０を返す														*/
/********************************************************************/
static Sint32 memPreloadFile(PcmHn pcm, Sint32 size)
{
	return 0;
}

static void memSetLoadNum(PcmHn pcm, Sint32 load_sct)
{
	return;
}

/********************************************************************/
/* メモリにフィルムヘッダを読み込む									*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/* [関数値]															*/
/*    実際に読み込んだバイト数										*/
/********************************************************************/
#if 0	/* 未対応 */
Sint32 PCM_PreloadHeader(PcmHn pcm)
{
	Sint32		load_size, header_size, read_size;
	PcmHeader	*header;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, 0);

	load_size = PCMLIB_SECT_SIZE;
	if ((read_size = PCMLIB_PRELOAD_FILE_FUNC(pcm)(pcm, load_size)) <= 0) {
		return read_size;
	}
	header = PCM_MeGetHeader(pcm);
	header_size = header->size_header >> 4;
	load_size = header_size - load_size;
	load_size = ((load_size + PCMLIB_SECT_SIZE - 1) / PCMLIB_SECT_SIZE) * 
										PCMLIB_SECT_SIZE;
	read_size = PCMLIB_PRELOAD_FILE_FUNC(pcm)(pcm, load_size) + read_size;
	return read_size;
}
#endif
/********************************************************************/
/* データの転送方式の設定（ＣＤブロック→リングバッファ） 			*/
/* [入力]															*/
/*    pcm   : ムービハンドル										*/
/*    mode  : 転送方式												*/
/*      PCM_TRMODE_CPU : ソフトウェア転送							*/
/*      PCM_TRMODE_SDMA : ＤＭＡサイクルスチール					*/
/*      PCM_TRMODE_SCU  : ＳＣＵのＤＭＡ							*/
/********************************************************************/
#if 0
void PCM_SetTrModePcm(PcmHn pcm, PcmTrMode mode)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	PCM_MeSetTrModePcm(pcm, mode);
}
#endif

/********************************************************************/
/* データの転送方式の設定（リングバッファ→ＰＣＭバッファ） 		*/
/* [入力]															*/
/*    pcm   : ムービハンドル										*/
/*    mode  : 転送方式												*/
/*      PCM_TRMODE_CPU : ソフトウェア転送							*/
/*      PCM_TRMODE_SDMA : ＤＭＡサイクルスチール					*/
/*      PCM_TRMODE_SCU  : ＳＣＵのＤＭＡ							*/
/********************************************************************/
void PCM_SetTrModeCd(PcmHn pcm, PcmTrMode mode)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	/* 実行関数の呼出 */
	PCMLIB_SET_TRMODE_FUNC(pcm)(pcm, mode);
}


static void memSetTrMode(PcmHn pcm, PcmTrMode mode)
{
	return;
}

/* このハンドルについて、もう何も処理しない場合に TRUE を返す */
Bool PCM_IsDeath(PcmHn pcm)
{
	PcmPlayStatus 	play_status;

	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE_RET(pcm, TRUE);

	if (! PCM_MeIsCreate(pcm)) {
		return TRUE;
	}

	play_status = PCM_GetPlayStatus(pcm);
	switch (play_status) {
	case PCM_STAT_PLAY_END:			/* 再生完了 				*/
	case PCM_STAT_PLAY_ERR_STOP:	/* 異常停止					*/
		return TRUE;
	default:
		return FALSE;
	}
}
