/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:メモリ再生
 * File		:pcm_mem.h
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_MEM_H)
#define PCM_MEM_H

#ifdef _PCMD
	/* Debug of sega_pcm */
	#include "sega_vtv.h"
#else
	#define VTV_PRINTF(a)
	#define VTV_Printf(a)
#endif
	#define _VTV_PRINTF(a)
	#define _VTV_Printf(a)

/* 本当は static 宣言する関数であるがデバッグを容易にするために。 */
#define STATIC

/*******************************************************************
      公開関数のプロトタイプ宣言：メモリ再生
*******************************************************************/
extern void PCM_MeSetErrCode(PcmErrCode err);
extern PcmErrCode PCM_MeGetErrCode(void);
extern void PCM_MeSetErrFunc(PcmErrFunc func, void *obj);
extern void PCM_MeSet1TaskSample(PcmHn hn, Sint32 sample);
extern void PCM_MeSetStartTrgSize(PcmHn pcm, Sint32 size);
extern void PCM_MeSetStartTrgSample(PcmHn hn, Sint32 sample);
extern void PCM_MeSetStopTrgSample(PcmHn hn, Sint32 sample);
/* extern void PCM_MeSetCopyModePcm(PcmCopyModePcm mode); */
extern void PCM_MeSetPauseWork(Sint32 *addr, Sint32 size);
extern void PCM_MeSetTrModePcm(PcmHn hn, PcmTrMode mode);
extern void PCM_MeInit(void);
extern void PCM_MeFinish(void);
extern void PCM_MeReset(PcmHn hn);
extern PcmHn PCM_MeCreate(PcmWork *work, PcmPara *para);
extern void PCM_MeDestroy(PcmHn hn);
extern void PCM_MeSetInfo(PcmHn hn, PcmInfo *info);
extern Sint32 PCM_MeGetRingSize(PcmHn hn);
extern void PCM_MeGetRingWrite(PcmHn hn, Sint8 **ring_write_addr, Sint32 *write_size, Sint32 *total_write_size);
extern void PCM_MeRenewRingWrite(PcmHn hn, Sint32 SizeWrite);
extern PcmPara *PCM_MeGetPara(PcmHn hn);
extern PcmStatus *PCM_MeGetStatus(PcmHn hn);
extern void PCM_MeStart(PcmHn hn);
extern void PCM_MeStop(PcmHn hn);
extern void PCM_MePause(PcmHn hn, PcmPauseCmd cmd);
extern void PCM_MeGetTime(PcmHn hn, Sint32 *sample_now);
extern void PCM_MeGetTimeTotal(PcmHn hn, Sint32 *sample_now);
extern void PCM_MeSetVolume(PcmHn hn, Uint32 level);
extern void PCM_MeSetPan(PcmHn hn, Uint32 pan);
extern void PCM_MeEntryNext(PcmHn hn);
extern PcmHn PCM_MeGetEntryNext(void);
extern PcmChangeStatus PCM_MeCheckChange(void);
extern void PCM_MeChange(PcmHn hn_e);
extern PcmHn PCM_MeGetHandle(Uint32 i);
extern Sint32 PCM_MeGetHandleNo(PcmHn hn);
extern void PCM_MeTask(PcmHn hn);
extern void PCM_MeVblIn(void);
extern Bool PCM_MeIsCreate(PcmHn pcm);

#endif	/* PCM_MEM_H */
