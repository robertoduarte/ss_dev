/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ストリームアクセス
 * File		:pcm_stm.h
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_STM_H)
#define PCM_STM_H

#include <sega_stm.h>
#include <sega_pcm.h>

/* データ転送モード */
typedef enum {
		PCM_STM_LOAD_CPU,
		PCM_STM_LOAD_DMACPU,
		PCM_STM_LOAD_DMASCU
} PcmStmLoadMode;


/* ムービの生成 */
PcmHn PCM_StmCreate(StmHn stm, PcmWork *work , 
							   void *buf, Sint32 bufsize, 
							   void *pcmbuf, Sint32 pcmsize);

/* ムービの再生開始 */
void PCM_StmStart(PcmHn pcm);

/* ムービの再生タスク */
void PCM_StmTask(PcmHn pcm);

/* ムービをメモリに読み込む */
Sint32 PCM_StmPreloadFile(PcmHn pcm, Sint32 size);

/* 転送セクタ数の設定 */
void PCM_StmSetLoadNum(PcmHn pcm, Sint32 load_sct);

/* 転送方式の設定 */
void PCM_StmSetTrMode(PcmHn pcm, PcmTrMode mode);

#endif
