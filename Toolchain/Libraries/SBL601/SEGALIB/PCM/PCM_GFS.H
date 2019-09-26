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
#if !defined(PCM_GFS_H)
#define PCM_GFS_H

#include <sega_gfs.h>
#include <sega_pcm.h>

/* ムービの生成 */
PcmHn PCM_GfsCreate(GfsHn gfs, PcmWork *work, 
							   void *buf, Sint32 bufsize, 
							   void *pcmbuf, Sint32 pcmsize);

/* ムービの再生開始 */
void PCM_GfsStart(PcmHn pcm);

/* ムービの再生タスク */
void PCM_GfsTask(PcmHn pcm);

/* ムービをメモリに読み込む */
Sint32 PCM_GfsPreloadFile(PcmHn pcm, Sint32 size);

/* 転送セクタ数の設定 */
void PCM_GfsSetLoadNum(PcmHn pcm, Sint32 load_sct);

/* データの転送方式の設定 */
void PCM_GfsSetTrMode(PcmHn pcm, PcmTrMode mode);

#endif
