/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:サウンドドライバ処理
 * File		:pcm_drv.c
 * Date		:1994-09-30
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include <machine.h>
#include <sega_snd.h>
#include "pcm_msub.h"
#include "pcm_drv.h"

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ふぉかのファイルの関数を参照しているので追加
	*/
	#include	"pcm_time.h"
#endif

#ifdef _PCMD
	extern PcmHn pcm_hn_tbl[PCM_HN_MAX];
#endif

#if	0
	/*
	**■1995-07-27	高橋智延
	**	warning がでるので変更
	*/
const static Sint8 logtbl[] = {
#else
static const Sint8	logtbl[] = {
#endif
/* 0 */		0, 
/* 1 */		1, 
/* 2 */		2, 2, 
/* 4 */		3, 3, 3, 3, 
/* 8 */		4, 4, 4, 4, 4, 4, 4, 4, 
/* 16 */	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
/* 32 */	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
			6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
/* 64 */	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
			7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
/* 128 */	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
			8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
	};

static void pcm_Wait(int cnt)
{
	while (--cnt > 0) {
		;
	}
}

/*******************************************************************
【機　能】
	メモリ再生／４ｋサンプル更新処理
【引　数】
	pcm_play_number		(入力) PCM play address 増加ビットマスク
【戻り値】
	なし
*******************************************************************/
void pcm_4ksampleProcess(Uint8 pcm_play_number)
{
	PcmHn 		hn;
	PcmWork		*work;
	PcmPara		*para;
	PcmStatus	*st;
	Uint32 		i;

	for (i = 0; i < PCM_HN_MAX; i++) {
		hn = PCM_MeGetHandle(i);
		if (hn == NULL) {
			continue;
		}
		work 	= *(PcmWork **)hn;
		para	= &work->para;
		st 		= &work->status;
		if (st->play != PCM_STAT_PLAY_TIME) {
			continue;
		}
		if (pcm_bittbl[para->pcm_stream_no] & pcm_play_number) {
			if (pcm_cnt_vbl_in - st->vbl_pcm_start >= 3) {
				st->cnt_4ksample++;
				pcm_StartClock(hn);

				#ifdef __PCMD
					debug_write_str("4ks+");
					debug_write_data((Uint32)hn - (Uint32)pcm_hn_tbl);
					debug_write_data((Uint32)st->cnt_4ksample);
					debug_write_data((Uint32)pcm_cnt_vbl_in - 
											st->vbl_pcm_start);
					debug_write_data(*(Uint32 *)0x25a007a0);
					debug_write_data(*(Uint32 *)0x25a007a4);
					debug_write_data(*(Uint32 *)0x25a007a8);
					debug_write_data(*(Uint32 *)0x25a007ac);
				#endif

			}
		}
	}
}

/* 前回のPCM再生アドレス値を保持する */
STATIC Uint8 pcm_play_address_last[PCM_STREAM_MAX];

/* PCM再生アドレスのポーリング初期処理 */
STATIC void pcm_InitPolling(Sint32 stream_no)
{
	pcm_play_address_last[stream_no] = PCM_PLAY_ADDRESS(stream_no);
}

/* PCM再生アドレスのポーリング処理 */
void PCM_DrvPolling(void)
{
	Sint32		i;
	Uint8 		pcm_play_number;

	pcm_play_number = 0;
	for (i = 0; i < PCM_STREAM_MAX; i++) {
		if (pcm_play_address_last[i] != PCM_PLAY_ADDRESS(i)) {
			pcm_play_address_last[i] = PCM_PLAY_ADDRESS(i);
			pcm_play_number |= pcm_bittbl[i];
		}
	}
	if (pcm_play_number) {
		pcm_4ksampleProcess(pcm_play_number);
	}
}

#if 0

/* サウンドドライバのパラメータの設定 */
static void pcm_SetPara(Uint8 *dst, Uint8 *src, Sint32 size)
{
	/* コマンド部、リザーブ部をスキップする */
	dst += 2;
	src += 2;
	size -= 2;

	while (--size >= 0) {
		for (*dst = *src; *dst != *src; *dst = *src) {
			pcm_Wait(0x100);
		}
		dst++;
		src++;
	}
}

/* para の値でなく、指定したい場合は引数で指定する */
void PCM_DrvChangePcmPara(PcmHn hn, Sint32 level, Sint32 pan)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	PcmChangePcmPara *cmdp = &st->command.change_pcm_para;
	PcmCmdBlk	*cmd_blk;
	Sint32		oct, shift_freq, fns;
	int			imask;

    imask = get_imask();
    set_imask(15);

	cmd_blk = (PcmCmdBlk *)PCM_CMD_BLK(para->command_blk_no);

	PCM_WAIT_LAST_COMMAND_FINISH(cmd_blk, imask);/* 前回コマンドの終了待ち */

	oct 					= PCM_CALC_OCT(st->info.sampling_rate);
	shift_freq 				= PCM_CALC_SHIFT_FREQ(oct);
	fns 					= PCM_CALC_FNS(st->info.sampling_rate, 
											shift_freq);
	cmdp->stm_no 			= PCM_SET_STMNO(para);
	if (level == -1) {
		cmdp->level_pan 	= PCM_SET_LEVEL_PAN(para);
	} else {
		cmdp->level_pan 	= PCM_SET_LEVEL_PAN2(level, pan);
	}
	cmdp->pitch_word 		= PCM_SET_PITCH_WORD(oct, fns);
	cmdp->effect_r_mono 	= 0x00;		/* DSP使わない 		*/
	cmdp->effect_l 			= 0x00;		/* DSP使わない 		*/
	pcm_SetPara((Uint8 *)cmd_blk, (Uint8 *)cmdp, sizeof(PcmCmdBlk));
	cmdp->command 			= PCM_COMMAND_CHANGE_PCM_PARA;
	cmd_blk->command 		= PCM_COMMAND_CHANGE_PCM_PARA;

    set_imask(imask);
}

/* SATURN Sound Driver ＰＣＭ再生開始 */
void PCM_DrvStartPcm(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	PcmStartPcm *cmdp 	= &st->command.start_pcm;
	PcmCmdBlk	*cmd_blk;
	Sint32		oct, shift_freq, fns;
	int			imask;

    imask = get_imask();
    set_imask(15);

	cmd_blk = (PcmCmdBlk *)PCM_CMD_BLK(para->command_blk_no);

	PCM_WAIT_LAST_COMMAND_FINISH(cmd_blk, imask);/* 前回コマンドの終了待ち */

	oct 					= PCM_CALC_OCT(st->info.sampling_rate);
	shift_freq 				= PCM_CALC_SHIFT_FREQ(oct);
	fns 					= PCM_CALC_FNS(st->info.sampling_rate, shift_freq);
	cmdp->channel_bit_stm_no = (Uint8)(
								(PCM_IS_MONORAL(st) 		 ? 0x00 : 0x80) |
								(PCM_IS_16BIT_SAMPLING(st) 	 ? 0x00 : 0x10) | 
								(PCM_SET_STMNO(para)));
	cmdp->level_pan 		= PCM_SET_LEVEL_PAN(para);
	cmdp->buf_addr 			= PCM_SET_PCM_ADDR(para);
	cmdp->buf_size 			= PCM_SET_PCM_SIZE(para);
	cmdp->pitch_word 		= PCM_SET_PITCH_WORD(oct, fns);
	cmdp->effect_r_mono 	= 0x00;	/* DSP使わない 		*/
	cmdp->effect_l 			= 0x00;	/* DSP使わない 		*/
	pcm_SetPara((Uint8 *)cmd_blk, (Uint8 *)cmdp, sizeof(PcmCmdBlk));
	cmdp->command 			= PCM_COMMAND_START_PCM;
	cmd_blk->command 		= PCM_COMMAND_START_PCM;

	pcm_InitPolling(para->pcm_stream_no);

    set_imask(imask);
}

/* SATURN Sound Driver ＰＣＭ再生停止 */
void PCM_DrvStopPcm(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
	PcmStatus	*st 	= &work->status;
	PcmStopPcm 	*cmdp 	= &st->command.stop_pcm;
	PcmCmdBlk	*cmd_blk;
	int			imask;

    imask = get_imask();
    set_imask(15);

	cmd_blk = (PcmCmdBlk *)PCM_CMD_BLK(para->command_blk_no);

	PCM_WAIT_LAST_COMMAND_FINISH(cmd_blk, imask);/* 前回コマンドの終了待ち */

	cmdp->stm_no 			= PCM_SET_STMNO(para);
	pcm_SetPara((Uint8 *)cmd_blk, (Uint8 *)cmdp, sizeof(PcmCmdBlk));
	cmdp->command 			= PCM_COMMAND_STOP_PCM;
	cmd_blk->command 		= PCM_COMMAND_STOP_PCM;

    set_imask(imask);
}

#endif

/****************************************************************/
/*         サウンドＩ／Ｆライブラリを使用する                   */
/****************************************************************/

/* para の値でなく、指定したい場合は引数で指定する */
void PCM_DrvChangePcmPara(PcmHn hn, Sint32 level, Sint32 pan)
{
	PcmWork			*work 	= *(PcmWork **)hn;
	PcmPara			*para 	= &work->para;
	PcmStatus		*st 	= &work->status;
	Sint32			oct, shift_freq, fns;
	int				imask;
	SndPcmChgPrm	chg_prm;

	/* chg_prm の設定 */
	oct 					= PCM_CALC_OCT(st->info.sampling_rate);
	shift_freq 				= PCM_CALC_SHIFT_FREQ(oct);
	fns 					= PCM_CALC_FNS(st->info.sampling_rate, 
											shift_freq);
	SND_PRM_NUM(chg_prm)	= (SndPcmNum)para->pcm_stream_no;
	SND_PRM_LEV(chg_prm)	= (level == -1) ? para->pcm_level : level;
	SND_PRM_PAN(chg_prm)	= (pan   == -1) ? para->pcm_pan   : pan;
	SND_PRM_PICH(chg_prm)	= PCM_SET_PITCH_WORD(oct, fns);
	SND_R_EFCT_IN(chg_prm)	= 0;
	SND_R_EFCT_LEV(chg_prm)	= 0;
	SND_L_EFCT_IN(chg_prm)	= 0;
	SND_L_EFCT_LEV(chg_prm)	= 0;

    imask = get_imask();
    set_imask(15);

	while (SND_ChgPcm(&chg_prm) == SND_RET_NSET) {
		set_imask(imask);
		pcm_Wait(100);
		set_imask(15);
	}

    set_imask(imask);
}

/* SATURN Sound Driver ＰＣＭ再生開始 */
void PCM_DrvStartPcm(PcmHn hn)
{
	PcmWork			*work 	= *(PcmWork **)hn;
	PcmPara			*para 	= &work->para;
	PcmStatus		*st 	= &work->status;
	Sint32			oct, shift_freq, fns;
	int				imask;
	SndPcmStartPrm	start_prm;
	SndPcmChgPrm	chg_prm;

	/* chg_prm の設定 */
	oct 					= PCM_CALC_OCT(st->info.sampling_rate);
	shift_freq 				= PCM_CALC_SHIFT_FREQ(oct);
	fns 					= PCM_CALC_FNS(st->info.sampling_rate, 
											shift_freq);
	SND_PRM_NUM(chg_prm)	= (SndPcmNum)para->pcm_stream_no;
	SND_PRM_LEV(chg_prm)	= para->pcm_level;
	SND_PRM_PAN(chg_prm)	= para->pcm_pan;
	SND_PRM_PICH(chg_prm)	= PCM_SET_PITCH_WORD(oct, fns);
	SND_R_EFCT_IN(chg_prm)	= 0;
	SND_R_EFCT_LEV(chg_prm)	= 0;
	SND_L_EFCT_IN(chg_prm)	= 0;
	SND_L_EFCT_LEV(chg_prm)	= 0;

	/* start_prm の設定 */
	SND_PRM_MODE(start_prm)		= (PCM_IS_MONORAL(st)        ? 0x00 : 0x80) |
								  (PCM_IS_16BIT_SAMPLING(st) ? 0x00 : 0x10); 
	SND_PRM_SADR(start_prm)		= (Uint16)PCM_SET_PCM_ADDR(para);
	SND_PRM_SIZE(start_prm)		= (Uint16)PCM_SET_PCM_SIZE(para);

    imask = get_imask();
    set_imask(15);

	while (SND_StartPcm(&start_prm, &chg_prm) == SND_RET_NSET) {
		set_imask(imask);
		pcm_Wait(100);
		set_imask(15);
	}

	pcm_InitPolling(para->pcm_stream_no);

    set_imask(imask);
}

/* SATURN Sound Driver ＰＣＭ再生停止 */
void PCM_DrvStopPcm(PcmHn hn)
{
	PcmWork		*work 	= *(PcmWork **)hn;
	PcmPara		*para 	= &work->para;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmStatus	*st 	= &work->status;
#endif
	int			imask;

    imask = get_imask();
    set_imask(15);

	while (SND_StopPcm((SndPcmNum)para->pcm_stream_no) == SND_RET_NSET) {
		set_imask(imask);
		pcm_Wait(100);
		set_imask(15);
	}

    set_imask(imask);
}
