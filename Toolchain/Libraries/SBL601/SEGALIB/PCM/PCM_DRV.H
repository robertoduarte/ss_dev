/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:サウンドドライバ処理
 * File		:pcm_drv.h
 * Date		:1994-09-30
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_DRV_H)
#define PCM_DRV_H


/*-------------------- 《インクルードファイル》 --------------------*/
#include "sega_xpt.h"


/*------------------------- 《マクロ定数》 -------------------------*/

/* サウンドコントロールコマンド(ＰＣＭ関連) */
#define PCM_COMMAND_OK						(0x00)
#define PCM_COMMAND_START_PCM				(0x85)
#define PCM_COMMAND_STOP_PCM				(0x86)
#define PCM_COMMAND_CHANGE_PCM_PARA			(0x8A)
#define PCM_COMMAND_CHANGE_MAP				(0x08)
#define PCM_COMMAND_TOTAL_VOLUME			(0x82)
#define PCM_COMMAND_CHANGE_EFFECT			(0x83)
#define PCM_COMMAND_CHANGE_MIXER			(0x87)
#define PCM_COMMAND_CHANGE_MIXER_PARA		(0x88)


/* ＳＣＳＰ基本周波数 44.1[kHz] */
#define PCM_SCSP_FREQUENCY					(44100L)

/* サウンドドライバインタフェースアドレス */
#define PCM_STREAM_MAX			(8)				/* 8本のPCMｽﾄﾘｰﾑを再生できる */
#define PCM_SOUND_BASE_ADDR		(0x25A00000)
#define PCM_SYS_IF_TBL			(PCM_SOUND_BASE_ADDR + 0x400)
#define PCM_SYS_IF_WORK			(PCM_SOUND_BASE_ADDR + 0x480)
#define PCM_HOST_IF_WORK		(PCM_SOUND_BASE_ADDR + *(Uint32 *)(PCM_SYS_IF_TBL + 4))
#define PCM_CMD_BLK(n)			(PCM_HOST_IF_WORK + 0x10 * (n))
#define PCM_PLAY_ADDRESS(n)		(*(Uint8 *)(PCM_HOST_IF_WORK + 0xA0 + 2 * (n)))


/*------------------------- 《処理マクロ》 -------------------------*/

/* 1,3,4,5,10 bit mask */
#define PCM_MSK1(a)				((a)&0x0001)
#define PCM_MSK3(a)				((a)&0x0007)
#define PCM_MSK4(a)				((a)&0x000F)
#define PCM_MSK5(a)				((a)&0x001F)
#define PCM_MSK10(a)			((a)&0x03FF)

/* オクターブ値の計算 */
#define PCM_CALC_OCT(smpling_rate) 											\
		((Sint32)logtbl[PCM_SCSP_FREQUENCY / ((smpling_rate) + 1)])

/* シフト基準周波数の計算 */
#define PCM_CALC_SHIFT_FREQ(oct)											\
		(PCM_SCSP_FREQUENCY >> (oct))

/* ＦＮＳの計算 */
#define PCM_CALC_FNS(smpling_rate, shift_freq)								\
		((((smpling_rate) - (shift_freq)) << 10) / (shift_freq))

/* SATURN Sound Driver パラメータ設定値 */
#define PCM_SET_STMNO(para)													\
		((Uint8)PCM_MSK3((para)->pcm_stream_no))
#define PCM_SET_LEVEL_PAN(para)												\
		((Uint8)((PCM_MSK3((para)->pcm_level) << 5) | PCM_MSK5((para)->pcm_pan)))
#define PCM_SET_LEVEL_PAN2(level, pan)										\
		((Uint8)((PCM_MSK3(level) << 5) | PCM_MSK5(pan)))
#define PCM_SET_PITCH_WORD(oct, fns)										\
		((Uint16)((PCM_MSK4(-(oct)) << 11) | PCM_MSK10(fns)))
#define PCM_SET_PCM_ADDR(para) 	(((Uint32)((para)->pcm_addr)) >> 4)
#define PCM_SET_PCM_SIZE(para) 	((para)->pcm_size)



/* 当該コマンドブロックの、前回コマンドの終了待ち */
#define PCM_WAIT_LAST_COMMAND_FINISH(cmd_blk, imask)	\
	while ((cmd_blk)->command != PCM_COMMAND_OK) {	\
		set_imask(imask);	\
		pcm_Wait(100);	\
		set_imask(15);	\
	}


/*-------------------------- 《プロトタイプ》 --------------------------*/

extern void PCM_DrvPolling(void);
extern void PCM_DrvStartPcm(PcmHn hn);
extern void PCM_DrvStopPcm(PcmHn hn);
extern void PCM_DrvChangePcmPara(PcmHn hn, Sint32 level, Sint32 pan);


#endif	/* PCM_DRV_H */
