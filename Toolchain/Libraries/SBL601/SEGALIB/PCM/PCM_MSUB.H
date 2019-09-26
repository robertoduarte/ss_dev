/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:メモリ再生内部ヘッダ
 * File		:pcm_msub.h
 * Date		:1994-09-29
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#if !defined(PCM_MSUB_H)
#define PCM_MSUB_H

#include <sega_pcm.h>
#include <sega_adp.h>
#include "pcm_mem.h"
#include "pcm_aif.h"

/* 折り返しノイズ対策（東レポート）の処理をする */
/* #define PCM_NOISE_WORK */

/*******************************************************************
      定数マクロ
*******************************************************************/

/* セクタサイズ */
/* #define PCM_SIZE_SECTOR		(PCM_SIZE_2K) */	/* MODE1,MODE2Form1	*/
#define PCM_SIZE_SECTOR			(2324)				/* MODE2Form2 		*/

/* １セクタ当たりのサウンドグループ数 */
#define PCM_SG_IN_SECTOR		(18)

/* CD-ROM XA Audio Frequency */
#define PCM_FREQ_37800			(37800L)	/* 37.8 [kHz] */
#define PCM_FREQ_18900			(18900L)	/* 18.9 [kHz] */

/* 最後に設定する無音の時間 [ms] */
#define PCM_SILENCE_TIME		(250)		/* 1/4 [秒] */


/*******************************************************************
      処理マクロ
*******************************************************************/

/* ２分の１ */
#define PCM_HALF(a)			((a) >> 1)

/* 最後に設定する無音のサンプル数 [sample] */
#define PCM_SILENCE_SAMPLE(st, para)	\
			MIN(((st)->info.sampling_rate * PCM_SILENCE_TIME) / 1000, \
				PCM_HALF((para)->pcm_size))

/* 無音設定トリガの在庫サンプル数 [sample] */
/*#define PCM_SILENCE_TRIG_SAMPLE(st, para)	PCM_SIZE_4K */
#define PCM_SILENCE_TRIG_SAMPLE(st, para)	PCM_SIZE_1K

/* 再生開始トリガ時間のサンプル数 [sample] */
/* #define PCM_TRIGGER_SAMPLE(st, para)	PCM_HALF((para)->pcm_size) */
/* 94.11.21 タスク関数の仕様に合わせて、４ｋサンプルとする */
#define PCM_START_TRIGGER_SAMPLE		(PCM_SIZE_4K)

/* 再生停止トリガ時間のサンプル数 [sample] */
#define PCM_STOP_TRIGGER_SAMPLE			(PCM_SIZE_1K)


/* 大きな整数の計算。 x * y / z を計算する。
	x = 0..CC33266h 					: Vblカウンタ、991時間まで計算可能
	y = (head->time_scale_film) * 256	: フィルムのタイムスケール * 256
	z = 3BD3h							: Vbl周波数 * 256
*/
#define PCM_CALC_XY_Z(x, y, z)											\
			(((x) / (z)) * (y) + (((x) % (z)) * (y)) / (z))
#define PCM_CALC_XY_DIVIDE_Z(x, y, z)									\
			(((x) / (z)) * (y) + PCM_CALC_XY_Z((y), (x) % (z), (z)))


/* 先頭１バイトの識別子を得る */
#define ID_1BYTE(a)				(*(volatile Uint8 *)(a))

/* 次の３バイトのサイズを得る */
#define SIZE_3BYTE(a)			((*(volatile Uint32 *)(a)) & 0x00FFFFFF)

#define VALUE_SINT8(a)			(*(volatile Sint8 *)(a))
#define VALUE_UINT8(a)			(*(volatile Uint8 *)(a))
#define VALUE_SINT16(a)			(*(volatile Sint16 *)(a))
#define VALUE_UINT16(a)			(*(volatile Uint16 *)(a))
/*
#define VALUE_SINT32(a)			(*(volatile Sint32 *)(a))
#define VALUE_UINT32(a)			(*(volatile Uint32 *)(a))
*/
#define VALUE_SINT32(a)	((Sint32)(	((VALUE_SINT16(a) << 16) & 0xffff0000) | \
							(VALUE_SINT16((Uint32)(a) + 2) & 0x0000ffff) ))
#define VALUE_UINT32(a)	((Uint32)(	((VALUE_SINT16(a) << 16) & 0xffff0000) | \
							(VALUE_SINT16((Uint32)(a) + 2) & 0x0000ffff) ))



/* セクタ境界の端数 (n % 2048) */
#define PCM_SECTOR_REMAINDER(n)		((n) & 0x000007FF)

/* セクタ境界まで減少した(切り捨てた)サイズ */
#define PCM_SECTOR_BOUND_DEC(n)		((n) & 0xFFFFF800)

/* セクタ境界まで増加した(切り上げた)サイズ (2048 * ((n + 2047) / 2048)) */
#define PCM_SECTOR_BOUND_INC(n)		(PCM_SECTOR_BOUND_DEC((n) + 0x7FF))


/*  */
#define PCM_FU_NO_JOUYO(a, b)	(((a) >= 0) ? (a) % (b) : -((-(a)) % (b)))

/* ファイルオフセットからバッファ中のアドレスを得る */
#define PCM_OFFSET_TO_ADDR(st, offset)										\
	(																		\
		((offset) < (st)->SizeRing) ? 										\
			(st)->AddrRing +  ((offset) - (st)->SizeHead) : 				\
			(st)->AddrRing + 												\
				PCM_FU_NO_JOUYO(((offset) - (st)->SizeHead), (st)->SizeRing)\
	)

#define PCM_IS_NO_HEADER(st) ((st)->info.file_type == PCM_FILE_TYPE_NO_HEADER)
#define PCM_IS_AIFF(st)		((st)->info.file_type == PCM_FILE_TYPE_AIFF)
#define PCM_IS_AIFC(st)		((st)->info.file_type == PCM_FILE_TYPE_AIFC)

#define PCM_IS_LRLRLR(st)	((st)->info.data_type == PCM_DATA_TYPE_LRLRLR)
#define PCM_IS_RLRLRL(st)	((st)->info.data_type == PCM_DATA_TYPE_RLRLRL)
#define PCM_IS_LLLRRR(st)	((st)->info.data_type == PCM_DATA_TYPE_LLLRRR)
#define PCM_IS_RRRLLL(st)	((st)->info.data_type == PCM_DATA_TYPE_RRRLLL)
#define PCM_IS_ADPCM_SG(st)	((st)->info.data_type == PCM_DATA_TYPE_ADPCM_SG)
#define PCM_IS_ADPCM_SCT(st) ((st)->info.data_type == PCM_DATA_TYPE_ADPCM_SCT)
#define PCM_IS_LR_MIX(st) 	(PCM_IS_LRLRLR(st) || PCM_IS_RLRLRL(st))
#define PCM_IS_LR_BLOCK(st) (PCM_IS_LLLRRR(st) || PCM_IS_RRRLLL(st))
#define PCM_IS_ADPCM(st) 	(PCM_IS_ADPCM_SG(st) || PCM_IS_ADPCM_SCT(st))

#define PCM_IS_STEREO(st)	((st)->info.channel == 0x02)
#define PCM_IS_MONORAL(st)	((st)->info.channel == 0x01)
#define PCM_IS_NO_AUDIO(st)	((st)->info.channel == 0x00)
#define PCM_IS_16BIT_SAMPLING(st)	(((st)->info.sampling_bit <= 0x10) && \
									((st)->info.sampling_bit > 0x08))
#define PCM_IS_8BIT_SAMPLING(st)	((st)->info.sampling_bit <= 0x08)

/* リングバッファが空 */
#define PCM_IS_RING_EMPTY(st)	\
			((st)->ring_write_offset == (st)->ring_read_offset)

/* バイト数をサンプル数に変換する */
#define PCM_BSIZE2SAMPLE(st, bsize)		\
			(PCM_IS_8BIT_SAMPLING(st) ? (bsize) : (bsize) >> 1)

/* サンプル数をバイト数に変換する */
#define PCM_SAMPLE2BSIZE(st, sample)	\
			(PCM_IS_8BIT_SAMPLING(st) ? (sample) : (sample) << 1)

/* １ｃｈ分のサンプル数、バイト数を、実際のチャンネル数分に変換する */
#define PCM_1CH2NCH(st, a)	(PCM_IS_MONORAL(st) ? (a) : (a) << 1)

/* ４ｋサンプルのバイト数[byte/1ch] */
#define PCM_4KSAMPEL_BYTE(st)	\
			(PCM_IS_8BIT_SAMPLING(st) ? PCM_SIZE_4K : PCM_SIZE_8K)

/* バイト数をセクタ数に換算する */
/* #define PCM_BYTE2SECT(b)	((b) / (ADP_SIZE_SG_DECO * PCM_SG_IN_SECTOR)) */

/* バイト数をｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数に換算する */
/* #define PCM_BYTE2SG(b)		((b) / (ADP_SIZE_SG_DECO)) */

/* ４ｋサンプルのセクタ数[sector/1ch] */
/* #define PCM_4KSAMPEL_SECT	PCM_BYTE2SECT(PCM_SIZE_8K) */

/* ４ｋサンプルのｻｳﾝﾄﾞｸﾞﾙｰﾌﾟ数[sg/1ch] */
/* #define PCM_4KSAMPEL_SG			\
			((PCM_SIZE_8K + ADP_SIZE_SG_DECO - 1) / ADP_SIZE_SG_DECO)	*/

/* サンプル数をサウンドグループ数に換算する (切り上げ) */
#define PCM_SAMPLE2SG(sample)	\
			(((sample) + ADP_WSIZE_SG_DECO - 1) / ADP_WSIZE_SG_DECO)


#ifdef PCM_NOISE_WORK
	/* 折り返しノイズ対策する */
	/* w:書き込みｱﾄﾞﾚｽ, t:ﾊﾞｯﾌｧ先頭ｱﾄﾞﾚｽ, s:ﾊﾞｯﾌｧｻｲｽﾞ[byte] */

	#define PCM_KILL_NOISE16(w, s)	\
		(*(Sint16 *)((Uint32)(w) + (s) - 2) = *(Sint16 *)(w))

	#define PCM_KILL_NOISE8(w, s)	\
		(*(Sint8 *)((Uint32)(w) + (s) - 1) = *(Sint8 *)(w))

	#define PCM_CHECK_NOISE16(w, t, s)	\
			if ((w) == (t)) {	\
				PCM_KILL_NOISE16(w, s);	\
			}

	#define PCM_CHECK_NOISE8(w, t, s)	\
			if ((w) == (t)) {	\
				PCM_KILL_NOISE8(w, s);	\
			}
#else
	/* 折り返しノイズ対策しない */
	#define PCM_KILL_NOISE16(w, s)
	#define PCM_KILL_NOISE8(w, s)
	#define PCM_CHECK_NOISE16(w, t, s)
	#define PCM_CHECK_NOISE8(w, t, s)
#endif


/* システム状態チェック */
#define	PCM_CHK_SYSTEM_STATUS	\
	if (pcm_system_status != PCM_SYSTEM_STATUS_ON) { \
		PCM_MeSetErrCode(PCM_ERR_NO_INIT);\
		return;\
	}

/* システム状態チェック(返り値あり) */
#define	PCM_CHK_SYSTEM_STATUS_RET(ret)	\
	if (pcm_system_status != PCM_SYSTEM_STATUS_ON) { \
		PCM_MeSetErrCode(PCM_ERR_NO_INIT);\
		return (ret);\
	}

/*******************************************************************
      プロトタイプ宣言
*******************************************************************/
extern Sint32 pcm_cnt_vbl_in;
extern const Uint32 pcm_bit_msk[32];
extern const Uint8 pcm_bittbl[8];
extern void pcm_RenewRingRead(PcmHn hn, Sint32 read_size);
extern void pcm_RenewRingRead2(PcmHn hn, Sint32 size_sample);
extern void pcm_MemsetWord(Uint16 *dst, Uint16 value, Sint32 cnt);
extern void pcm_MemsetDword(Uint32 *dst, Uint32 value, Sint32 cnt);
extern void pcm_MemcpyDword(Uint32 *dst, Uint32 *src, Sint32 dwsize);
extern void pcm_MemmoveByte(Sint8 *dst, Sint8 *src, Sint32 bsize);
extern void pcm_MemmoveDword(Sint32 *dst, Sint32 *src, Sint32 dwsize);
extern void pcm_RotateByte(Sint8 *addr_pwork, Sint8 *addr_pcmbuf, Sint32 bsize_pcmbuf, Sint32 bsize_shift);
extern void pcm_RotateDword(Sint32 *addr_pwork, Sint32 *addr_pcmbuf, Sint32 dwsize_pcmbuf, Sint32 dwsize_shift);
extern void pcm_RotatePcmbuf(Uint32 addr_pwork, Sint32 bsize_pwork, Uint32 addr_pcmbuf, Sint32 bsize_pcmbuf, Sint32 offset);
extern void pcm_GetPcmWrite(PcmHn hn, Sint8 **AddrWrite1, Sint8 **AddrWrite2, Sint32 *SizeWrite, Sint32 *SizeWriteTotal);
extern void pcm_RenewPcmWrite(PcmHn hn, Sint32 size_write);
extern void pcm_4ksampleProcess(Uint8 pcm_play_number);
extern void pcm_AudioProcess(PcmHn hn);
extern void pcm_FillSilence(PcmHn hn);
extern void pcm_EndProcess(PcmHn hn);
extern void pcm_ChunkAdpcm(PcmHn hn, PcmAdpcmChunk *chunk);
extern void pcm_ChunkAdpcmUnlink(PcmHn hn, PcmAdpcmChunk *chunk);
extern void (*pcm_chunk_adpcm_fp)(PcmHn hn, PcmAdpcmChunk *chunk);
extern void (*pcm_set_info_adpcm_fp)(PcmHn hn);

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ほかのファイルから参照しているので追加
	*/
	void	PCM_MeChangePcmPara( PcmHn	hn );
	void	PCM_MeSetLoop( PcmHn	hn, Sint32	cnt_loop );
	Sint32	PCM_MeIsNotSetCi( PcmHn	hn );
	void	PCM_MeSetCi( PcmHn	hn, Uint8	ci );
#endif

#endif	/* PCM_MSUB_H */
