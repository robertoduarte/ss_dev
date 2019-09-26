/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ファイルアクセス
 * File		:pcm_gfs.h
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
/*-------------------- 《インクルードファイル》 --------------------*/
#include <sega_xpt.h>
#include "pcm_gfs.h"
#include "pcm_mem.h"
#include "pcm_lib.h"

/* #define	PCM_DEBUG 1 */

#ifdef PCM_DEBUG
#include "play.h"
#endif

/*------------------------- 《マクロ定数》 -------------------------*/
/* １回のタスクで転送する最大セクタ数 */
#define		LOAD_SECT_NUM		20

/* ExecOne のリトライ回数 */
#define		RETRY_CNT		10


/*------------------------- 《処理マクロ》 -------------------------*/
#ifdef PCM_DEBUG
#else
#define 	PL_DEBUG_PRINTNUM(a, b, c)
#define		PL_DEBUG_PRINT(a, b, c)
#define		PL_DEBUG_START_TIME()
#define		PL_DEBUG_PRINT_TIME(str)
#endif

/* ＧＦＳのローカルデータ */
#define	PCMGFS_FILE_PARA(pcm) \
			(PCMLIB_FILEPARA(pcm).data.gfs)

/* ストリームハンドル */
#define	PCMGFS_HANDLE(pcm) \
			(PCMGFS_FILE_PARA(pcm).gfs)

/* 最大転送セクタ数 */
#define	PCMGFS_LOAD_SECT(pcm) \
			(PCMGFS_FILE_PARA(pcm).load_sect)

/* GFS_NwCdRead をコールしたら TREU */
#define PCMGFS_CALLED_CDREAD(pcm) \
			(PCMGFS_FILE_PARA(pcm).called_cdread)

/* ExecOne 実行中なら TREU */
#define PCMGFS_EXEC_ONE_STATE(pcm) \
			(PCMGFS_FILE_PARA(pcm).exec_one_state)

/* ExecOne で読み込むバイト数 */
#define	PCMGFS_EXEC_LOAD_SIZE(pcm)	\
			(PCMGFS_FILE_PARA(pcm).exec_load_size)

/* ExecOne で読み込んでいるバイト数 */
#define	PCMGFS_NOW_LOAD_SIZE(pcm)	\
			(PCMGFS_FILE_PARA(pcm).now_load_size)

/* 読み込み総セクタ数 */
#define	PCMGFS_LOAD_TOTAL_SECT(pcm) \
			(PCMGFS_FILE_PARA(pcm).load_total_sect)

/* ファイルセクタ数 */
#define	PCMGFS_FILE_SECT(pcm) \
			(PCMGFS_FILE_PARA(pcm).file_sect)

/* 転送モード */
#define	PCMGFS_TR_MODE(pcm) \
			(PCMGFS_FILE_PARA(pcm).tr_mode)


/*-------------------------- 《データ型》 --------------------------*/
/*-------------------------- 《関数宣言》 --------------------------*/
STATIC void pcmgfs_loadBuf(PcmHn pcm);

/*-------------------------- 《変数定義》 --------------------------*/

/*-------------------------- 《関数定義》 --------------------------*/
/********************************************************************/
/* ハンドルの作成（ファイルシステム）								*/
/* [入力]															*/
/*    para : 作成パラメータ											*/
/*    gfs  : ファイルハンドル										*/
/* [関数値]															*/
/*    ハンドル（作成できない場合は NULL)							*/
/********************************************************************/
PcmHn PCM_CreateGfsHandle(PcmCreatePara *para, GfsHn gfs)
{
	PcmHn   	pcm;
	Sint32		sctsize, nsct, lastsize;

	if (gfs == NULL) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return NULL;
	}

	/* 作成パラメータのチェック */
	PCMLIB_CHK_CREATE_PARA(para);

	pcm =  PCM_GfsCreate(gfs, 
						 (PcmWork *)PCM_PARA_WORK(para), 
						 PCM_PARA_RING_ADDR(para), PCM_PARA_RING_SIZE(para),
						 PCM_PARA_PCM_ADDR(para), PCM_PARA_PCM_SIZE(para));
	if (pcm != NULL) {
		PCMLIB_FACCESS_TYPE(pcm) = PCMLIB_FACCESS_GFS;

		/* 実行関数の設定 */
		PCMLIB_SET_START_FUNC(pcm, PCM_GfsStart);
		PCMLIB_SET_TASK_FUNC(pcm, PCM_GfsTask);
		PCMLIB_SET_PRELOAD_FILE_FUNC(pcm, PCM_GfsPreloadFile);
		PCMLIB_SET_SET_LOAD_NUM_FUNC(pcm, PCM_GfsSetLoadNum);
		PCMLIB_SET_SET_TRMODE_FUNC(pcm, PCM_GfsSetTrMode);

		/* ローカルデータの初期化 */
		/* ファイルサイズの取得 */
		GFS_GetFileSize(gfs, &sctsize, &nsct, &lastsize);
		PCMGFS_FILE_SECT(pcm) = nsct;
		PCMGFS_CALLED_CDREAD(pcm) = FALSE;
		PCMGFS_EXEC_ONE_STATE(pcm) = FALSE;
		PCMGFS_LOAD_TOTAL_SECT(pcm) = 0;

		/* ファイルハンドルの設定 */
		PCMGFS_HANDLE(pcm) = gfs;

		/* 最大転送セクタ数の設定 */
		PCMGFS_LOAD_SECT(pcm) = LOAD_SECT_NUM;
		GFS_SetTransPara(PCMGFS_HANDLE(pcm), PCMGFS_LOAD_SECT(pcm));

		/* 転送モードの設定 */
		PCMGFS_TR_MODE(pcm) = GFS_TMODE_SDMA1;
		GFS_SetTmode(PCMGFS_HANDLE(pcm), PCMGFS_TR_MODE(pcm));

	}
	return pcm;
}

/********************************************************************/
/* ハンドルの消去（ファイルシステム）								*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_DestroyGfsHandle(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (PCMLIB_FACCESS_TYPE(pcm) != PCMLIB_FACCESS_GFS) {
		PCM_MeSetErrCode(PCM_ERR_ILL_CREATE_MODE);
	}

	PCM_MeDestroy(pcm);
}


/********************************************************************/
/* ハンドルのリセット												*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/* [関数値]															*/
/* 　なし															*/
/********************************************************************/
void PCM_GfsReset(PcmHn pcm)
{
	GFS_Seek(PCMGFS_HANDLE(pcm), 0, GFS_SEEK_SET);
	PCMGFS_CALLED_CDREAD(pcm) = FALSE;
	PCMGFS_LOAD_TOTAL_SECT(pcm) = 0;
	PCM_MeReset(pcm);
}

/********************************************************************/
/* ハンドルを生成する												*/
/*   ファイルハンドルは、ファイルシステムにより予め取得すること		*/
/*																	*/
/* [引き数]															*/
/*    gfs : ファイルハンドル										*/
/*    work : ワークアドレス											*/
/*    buf : バッファアドレス										*/
/*    bufsize : バッファのバイト数									*/
/*    pcmbuf:ＰＣＭのウェーブＲＡＭの先頭アドレス					*/
/*    pcmsize:ＰＣＭのウェーブＲＡＭのバイト数						*/
/* [関数値]															*/
/*    ハンドル														*/
/*    NULL の場合はエラー											*/
/********************************************************************/
PcmHn PCM_GfsCreate(GfsHn gfs, PcmWork *work, 
							   void *buf, Sint32 bufsize, 
							   void *pcmbuf, Sint32 pcmsize)
{
	PcmPara		para;

	/* リングバッファ */
	para.ring_addr = buf;
	para.ring_size = bufsize;

	/* トリガサイズ */
	para.start_trg_size 	= PCM_DEFAULT_SIZE_START_TRG;
	para.start_trg_sample 	= PCM_DEFAULT_SAMPLE_START_TRG;
	para.stop_trg_sample 	= PCM_DEFAULT_SAMPLE_STOP_TRG;

	/* サウンドドライバコマンドブロック番号 */
	para.command_blk_no = PCMLIB_COMMAND_BLK_NO;

	/* ＰＣＭストリーム再生番号 */
	para.pcm_stream_no = PCMLIB_PCM_STREAM_NO;

	/* ＰＣＭバッファ */
	para.pcm_addr = pcmbuf;
	para.pcm_size = pcmsize;

	/* ＰＡＮとボリューム */
	para.pcm_pan = PCMLIB_PCM_PAN;
	para.pcm_level = PCMLIB_PCM_LEVEL;

	/* ハンドルの作成 */
	return PCM_MeCreate(work, &para);
}


/********************************************************************/
/* ＣＤバッファからの最大転送セクタ数を設定する						*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    load_sct : 最大転送セクタ数									*/
/********************************************************************/
void PCM_GfsSetLoadNum(PcmHn pcm, Sint32 load_sct)
{
	PCMGFS_LOAD_SECT(pcm) = load_sct;

	/* 転送セクタ数を設定する */
	GFS_SetTransPara(PCMGFS_HANDLE(pcm), PCMGFS_LOAD_SECT(pcm));
}

/********************************************************************/
/* ＣＤバッファへの先読み要求										*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
static void pcmgfs_NwCdRead(PcmHn pcm)
{
	Sint32	nsct;
	Sint32	ret;

	nsct = PCMGFS_FILE_SECT(pcm) - PCMGFS_LOAD_TOTAL_SECT(pcm);
	if (nsct > 0) {
		ret = GFS_NwCdRead(PCMGFS_HANDLE(pcm), nsct);
		if (ret < 0) {
			PCM_MeSetErrCode(PCM_ERR_GFS_READ);
			return;
		}
    }
}

/********************************************************************/
/* 再生を先頭から開始する											*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
void PCM_GfsStart(PcmHn pcm)
{
	/* ＣＤバッファへの先読み要求 */
	/* pcmgfs_NwCdRead(pcm); */
	/* ここで先読み要求しても、実際には先読みできず、エラーが発生する。 */

	PCM_MeStart(pcm);
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
void PCM_GfsTask(PcmHn pcm)
{
	if (PCMGFS_CALLED_CDREAD(pcm) == FALSE) {

		/* ＣＤバッファへの先読み要求 */
		pcmgfs_NwCdRead(pcm);

		PCMGFS_CALLED_CDREAD(pcm) = TRUE;
	}

	/* ＣＤバッファからリングバッファへデータを転送する */
	pcmgfs_loadBuf(pcm);

	/* タスク処理 */
	PCM_MeTask(pcm);

	/* ループ再生制御 */
	if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {

		VTV_PRINTF((VTV_s, "P:Play %X Loop %X\n", 
			PCM_STAT_PLAY_END, PCM_HN_CNT_LOOP(pcm)));

		VTV_PRINTF((VTV_s, "P:exec_one_state %X\n", 
			PCMGFS_EXEC_ONE_STATE(pcm)));


		if (--PCM_HN_CNT_LOOP(pcm) > 0) {

			PCM_GfsReset(pcm);

			VTV_PRINTF((VTV_s, "P:GfsReset %X\n", pcm));

			PCM_GfsStart(pcm);

			VTV_PRINTF((VTV_s, "P:GfsStart %X\n", pcm));
		}
	}
}


/********************************************************************/
/* ＣＤバッファからのリングバッファにデータを転送する			*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
STATIC void pcmgfs_loadBuf(PcmHn pcm)
{
	Uint32		write_addr32;
	Sint32		write_size, total_size;
	Sint32		load_sct, load_size;
	Sint32		gfs_ret;
	Sint32		stat, ndata;
	Sint32		delta_size;
#ifdef _PCMD
	Sint32 		fread_this_call = 0;
#endif

	if (PCMGFS_LOAD_TOTAL_SECT(pcm) >= PCMGFS_FILE_SECT(pcm)) {
		/* ファイルをすべて読み込んだ */
		return;
	}

	if (PCMGFS_EXEC_ONE_STATE(pcm) == FALSE) {
		/* シネパックのバッファの空きバイト数を得る */
		PCM_MeGetRingWrite(pcm, 
			(Sint8 **)&write_addr32, &write_size, &total_size);

		if (write_size >= PCMLIB_SECT_SIZE) {
			/* ＣＤバッファからシネパックのバッファへの転送を開始する */
			if (write_size >= PCMGFS_LOAD_SECT(pcm) * PCMLIB_SECT_SIZE) {
				load_size = PCMGFS_LOAD_SECT(pcm) * PCMLIB_SECT_SIZE;
			} else {
				load_size = write_size;
			}
			load_sct = load_size / PCMLIB_SECT_SIZE;
			load_size = load_sct * PCMLIB_SECT_SIZE;

			_VTV_PRINTF((VTV_s, "P:NwFread %X\n", load_sct));

			gfs_ret = GFS_NwFread(PCMGFS_HANDLE(pcm), load_sct, 
											(void *)write_addr32, load_size);
			if (gfs_ret < 0) { 
				PCM_MeSetErrCode(PCM_ERR_GFS_READ);
				return;
			}

			PCMGFS_EXEC_LOAD_SIZE(pcm) = load_size;
			PCMGFS_EXEC_ONE_STATE(pcm) = TRUE;
			PCMGFS_NOW_LOAD_SIZE(pcm) = 0;

#ifdef _PCMD
			fread_this_call = 1;
#endif
		}
	}

	if (PCMGFS_EXEC_ONE_STATE(pcm) == TRUE) {

		_VTV_PRINTF((VTV_s, "P:Call NwExecOne\n"));

		/* ＣＤバッファからシネパックへのバッファ転送処理 */
		gfs_ret = GFS_NwExecOne(PCMGFS_HANDLE(pcm));
		if (gfs_ret < 0) {
			PCM_MeSetErrCode(PCM_ERR_GFS_READ);
			return;
		}

		_VTV_PRINTF((VTV_s, "P:Retn NwExecOne %d\n", gfs_ret));

		/* ホスト領域に転送したバイト数を得る */
		GFS_NwGetStat(PCMGFS_HANDLE(pcm), &stat, &ndata);
		if (ndata > 0) {
			delta_size = ndata - PCMGFS_NOW_LOAD_SIZE(pcm);
			PCMGFS_NOW_LOAD_SIZE(pcm) = ndata;
			if (delta_size > 0) {
				PCM_MeRenewRingWrite(pcm, delta_size);
			}
		}

		if (ndata >= PCMGFS_EXEC_LOAD_SIZE(pcm) ||
							 (gfs_ret == GFS_SVR_COMPLETED)) {
			/* ホスト領域への転送が終わった */
			PCMGFS_EXEC_ONE_STATE(pcm) = FALSE;
		}
#ifdef _PCMD
		else {
			Sint32 snum;
			CDC_GetSctNum(0, &snum);/* バッファ区画０のｾｸﾀ数を得る */
			if (fread_this_call == 0) {	/* 今回でなく以前のｺｰﾙで fread した */
				if (PCMGFS_EXEC_LOAD_SIZE(pcm) / 2048 < snum) {
					/* ＣＤバッファにデータがあるのに転送できなかった */
					VTV_PRINTF((VTV_s, "C:Yet? %2X<%2X %X\n",
						ndata / 2048, PCMGFS_EXEC_LOAD_SIZE(pcm) / 2048, 
						snum));
				}
			}
		}
#endif

	}
}


/********************************************************************/
/* ＣＤからリングバッファにファイルを読み込む						*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/*    size : 読み込むバイト数										*/
/*           リングバッファが読み込みバイト数より小さい時は	*/
/*           バッファサイズになる。									*/
/*           読み込むバイト数はセクタ単位（２０４８の倍数）になる。	*/
/* [関数値]															*/
/*    実際に読み込んだバイト数										*/
/********************************************************************/
Sint32 PCM_GfsPreloadFile(PcmHn pcm, Sint32 size)
{
	Uint32		write_addr32;
	Sint32		write_size, total_size;
	Sint32		load_sct, load_size;

	load_size = 0;
	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);
	if (write_size >= PCMLIB_SECT_SIZE) {
		/* ＣＤバッファからリングバッファへの転送を開始する */
		if (size > write_size) {
			load_size = write_size;
		} else {
			load_size = size;
		}
		load_sct = load_size / PCMLIB_SECT_SIZE;
		load_size = load_sct * PCMLIB_SECT_SIZE;
		if (load_sct > 0) {
			GFS_SetTransPara(PCMGFS_HANDLE(pcm), load_sct);
			load_size = GFS_Fread(PCMGFS_HANDLE(pcm), load_sct, 
										(void *)write_addr32, load_size);
			if (load_size < 0) { 
				PCM_MeSetErrCode(PCM_ERR_GFS_READ);
				return load_size;
			}
			PCM_MeRenewRingWrite(pcm, load_size);
			PCMGFS_LOAD_TOTAL_SECT(pcm) += load_sct;
		}
	}
	return load_size;
}

/********************************************************************/
/* データの転送方式の設定											*/
/* [入力]															*/
/*    pcm   : ムービハンドル										*/
/*    mode  : 転送方式												*/
/*      PCM_TRMODE_CPU : ソフトウェア転送							*/
/*      PCM_TRMODE_SDMA : ＤＭＡサイクルスチール					*/
/*      PCM_TRMODE_SCU  : ＳＣＵのＤＭＡ							*/
/********************************************************************/
void PCM_GfsSetTrMode(PcmHn pcm, PcmTrMode mode)
{
	Sint32	tmode;

	switch(mode) {
	case PCM_TRMODE_CPU:
	default:
		tmode = GFS_TMODE_CPU;
		break;
	case PCM_TRMODE_SDMA:
		tmode = GFS_TMODE_SDMA1;
		break;
	case PCM_TRMODE_SCU:
		tmode = GFS_TMODE_SCU;
		break;
	}

	/* 転送モードの設定 */
	PCMGFS_TR_MODE(pcm) = tmode;
	GFS_SetTmode(PCMGFS_HANDLE(pcm), PCMGFS_TR_MODE(pcm));
}

