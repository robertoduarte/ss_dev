/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ライブラリインタフェース
 * File		:pcm_lib.h
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_LIB_H)
#define PCM_LIB_H
/*------------------------- 《マクロ定数》 -------------------------*/
/* ファイルアクセスタイプ */
typedef enum {
	PCMLIB_FACCESS_MEM,
	PCMLIB_FACCESS_GFS,
	PCMLIB_FACCESS_STM
} FaccessType;

/* １セクタのバイト数 */
#define		PCMLIB_SECT_SIZE		2048

/* デフォルト値 */
/* トリガサイズ */
#define		PCMLIB_SIZE_TRIGGER		0
/* 展開先アドレス */
#define		PCMLIB_ADDR_VRAM		0
/* 展開先の横幅サイズ */
#define		PCMLIB_SIZE_VRAM_LINE	0
/* 音の処理フラグ 0:処理なし 1:再生 */
#define		PCMLIB_PLAY_PCM			1
/* ＰＣＭコマンドブロック番号 */
#define		PCMLIB_COMMAND_BLK_NO	1
/* ＰＣＭストリーム再生番号 */
#define		PCMLIB_PCM_STREAM_NO	1
/* ＰＣＭパン */
#define		PCMLIB_PCM_PAN			0
/* ＰＣＭ再生レベル */
#define		PCMLIB_PCM_LEVEL		7

/*------------------------- 《処理マクロ》 -------------------------*/
/* ファイル関係のローカルデータへのアクセス */
#define	PCMLIB_FILEPARA(pcm)	\
			((*(PcmWork **)(pcm))->filepara)

/* ファイルタイプ */
#define	PCMLIB_FACCESS_TYPE(pcm) \
			(PCMLIB_FILEPARA(pcm).faccess_type)


/* 実行関数の設定 */
#define	PCMLIB_INIT_FUNC(pcm)	\
			((*(PcmWork **)(pcm))->execfunc.init)
#define	PCMLIB_START_FUNC(pcm)	\
			((*(PcmWork **)(pcm))->execfunc.start)
#define	PCMLIB_TASK_FUNC(pcm) \
			((*(PcmWork **)(pcm))->execfunc.task)
#define	PCMLIB_PRELOAD_FILE_FUNC(pcm) \
			((*(PcmWork **)(pcm))->execfunc.preload_file)
#define	PCMLIB_SET_LOAD_NUM_FUNC(pcm) \
			((*(PcmWork **)(pcm))->execfunc.set_load_num)
#define	PCMLIB_SET_TRMODE_FUNC(pcm) \
			((*(PcmWork **)(pcm))->execfunc.set_trmode)

#define	PCMLIB_SET_INIT_FUNC(pcm, func) \
			(PCMLIB_INIT_FUNC(pcm) = (func))
#define	PCMLIB_SET_START_FUNC(pcm, func) \
			(PCMLIB_START_FUNC(pcm) = (func))
#define	PCMLIB_SET_TASK_FUNC(pcm, func) \
			(PCMLIB_TASK_FUNC(pcm) = (func))
#define	PCMLIB_SET_PRELOAD_FILE_FUNC(pcm, func) \
			(PCMLIB_PRELOAD_FILE_FUNC(pcm) = (func))
#define	PCMLIB_SET_SET_LOAD_NUM_FUNC(pcm, func) \
			(PCMLIB_SET_LOAD_NUM_FUNC(pcm) = (func))
#define	PCMLIB_SET_SET_TRMODE_FUNC(pcm, func) \
			(PCMLIB_SET_TRMODE_FUNC(pcm) = (func))

/* 作成パラメータチェック */
#define	PCMLIB_CHK_CREATE_PARA(para)	\
	if ((para) == NULL) { \
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);\
		return NULL;\
	}\
	if (PCM_PARA_WORK(para) == NULL ||\
		PCM_PARA_RING_ADDR(para) == NULL || PCM_PARA_RING_SIZE(para) <= 0 ||\
		PCM_PARA_PCM_ADDR(para) == NULL || PCM_PARA_PCM_SIZE(para) <= 0) {\
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);\
		return NULL;\
	}

/* ハンドルチェック */
#define	PCMLIB_CHK_HANDLE(pcm)	\
	if ((pcm) == NULL) { \
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_HANDLE);\
		return;\
	}

/* 返り値ありハンドルチェック */
#define	PCMLIB_CHK_HANDLE_RET(pcm, ret)	\
	if ((pcm) == NULL) { \
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_HANDLE);\
		return (ret);\
	}


/*-------------------------- 《データ型》 --------------------------*/

/*-------------------------- 《変数宣言》 --------------------------*/
/* ストリームライブラリの初期化フラグ */
/* OFF ならまだ初期化していない ON なら初期化済み */
extern Bool PCM_lib_stm_init;

#endif
