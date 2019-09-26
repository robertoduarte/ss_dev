/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:sample program of MEMORY PLAY MODE, MULTI PLAY, LOOP PLAY
 * File		:smppcm13.c
 * Date		:1995-03-31
 * Version	:1.16
 * Auther	:Y.T
 *
 * Comment	:
 *	メモリ上にあるファイルを再生するサンプル。
 *	ファイルは 0x4000000, 0x4800000 番地にロードしておくこと。
 *	再生できるファイルサイズは8ＭＢまで。
 *
 ****************************************************************************/

/*------------------------- 《プログラム選択》 -------------------------*/

/* VTV, MON を使用したデバッグの時に定義する。 */
/* #define SMPPCM_DEBUG */

/* ループ再生を行う */
#define LOOP_PLAY		(0x7fffffff)

/*------------------------- 《インクルード》 -------------------------*/
#include <machine.h>
#define _SH
#include "sega_xpt.h"
#include "sega_sys.h"
#include "sega_int.h"
#include "sega_per.h"
#include "sega_cdc.h"
#include "sega_gfs.h"
#include "sega_stm.h"
#include "sega_snd.h"
#include "sega_pcm.h"


#ifdef SMPPCM_DEBUG
	#include "..\smppcmd\smppcmd.c"
	#define SMPPCMD_VblIn				smppcmd_VblIn
	#define SMPPCMD_Init				smppcmd_Init
	#define SMPPCMD_MON_Reset			smppcmd_MON_Reset
	#define SMPPCMD_PCM_Task			smppcmd_PCM_Task
#else
	#define SMPPCMD_VblIn()				
	#define SMPPCMD_Init(a)				
	#define SMPPCMD_MON_Reset()			
	#define SMPPCMD_PCM_Task			PCM_Task
	#define VTV_Printf(a)				
	#define VTV_PRINTF(a)				
	#define _VTV_Printf(a)				
	#define _VTV_PRINTF(a)				
#endif


/*--------------------------- 《関数宣言》 ---------------------------*/

static void smpVblIn(void);
static void smpVblOut(void);


/*---------------------------- 《定数》 ----------------------------*/

/* ウェーブＲＡＭの転送アドレスとサンプル数 */
#define	PCM_ADDR_1	((void*)0x25a20000)
#define	PCM_ADDR_2	((void*)0x25a40000)
#define	PCM_SIZE	(4096L*2)				/* 2.. */

/* リングバッファのサイズ */
#define	RING_BUF_SIZE	(1024L*1024L*8)

/* リングバッファ */
#define	MOVIE_BUF_1		((Sint8 *)0x04000000)
#define	MOVIE_BUF_2		((Sint8 *)0x04800000)


/*----------------------- 《グローバル変数》 -----------------------*/

/* ワーク */
static PcmWork g_movie_work_1;
static PcmWork g_movie_work_2;


/*---------------------------- 《関数》 ----------------------------*/

void errGfsFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrGfs %X %X\n", obj, ec));
}

void errPcmFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrPcm %X %X\n", obj, ec));
}


/*====================== Ｖブランクの処理 ===========================*/

static void vblInit(void)
{
    set_imask(0);

	/* Ｖブランクの設定 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_IN, smpVblIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT, smpVblOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);
}

static void smpVblIn(void)
{
	/* ＰＣＭライブラリの VblIn ルーチンをコール */
	PCM_VblIn();

	/* グラフィックライブラリを使用する為には実行しなければならない */
	/*	SCL_VblankStart(); */
}

static void smpVblOut(void)
{
	/* グラフィックライブラリを使用する為には実行しなければならない */
	/*	SCL_VblankEnd(); */
}


/*====================== サウンドの処理 ===========================*/
#define SDDRVS_TSK_SIZE			(0x6000)
#define BOOTSND_MAP_SIZE		(0x0100)
Sint32 sddrvs_tsk[SDDRVS_TSK_SIZE / 4];
Sint32 bootsnd_map[BOOTSND_MAP_SIZE / 4];

static Sint32 fileLoad(Sint8 *name, void *addr, Sint32 bsize)
{
	Sint32 		fid, i;

	for (i = 0; i < 10; i++) {
		fid = GFS_NameToId(name);
		if (fid >= 0) {
			GFS_Load(fid, 0, addr, bsize);
			return 0;
		}
	}
	return -1;
}

static void sndInit(void)
{
	SndIniDt 	snd_init;

	if (fileLoad("SDDRVS.TSK", (void *)sddrvs_tsk, SDDRVS_TSK_SIZE)) {
		return;
	}
	if (fileLoad("BOOTSND.MAP", (void *)bootsnd_map, BOOTSND_MAP_SIZE)) {
		return;
	}
	SND_INI_PRG_ADR(snd_init) 	= (Uint16 *)sddrvs_tsk;
	SND_INI_PRG_SZ(snd_init) 	= (Uint16 )SDDRVS_TSK_SIZE;
	SND_INI_ARA_ADR(snd_init) 	= (Uint16 *)bootsnd_map;
	SND_INI_ARA_SZ(snd_init) 	= (Uint16)BOOTSND_MAP_SIZE;
	SND_Init(&snd_init);
	SND_ChgMap(0);
}


/*====================== ファイルの処理 ===========================*/
/* ルートディレクトリにあるファイルの最大数 */
#define MAX_DIR		100

/* 同時に開くファイルの最大数 */
#define OPEN_MAX	5

/* ディレクトリ情報管理領域 */
static GfsDirTbl	dir_tbl;

/* ファイル名を含んだ情報 */
static GfsDirName dir_name[MAX_DIR];

/* ＧＦＳの作業領域 */
static Uint8 gfs_work[GFS_WORK_SIZE(OPEN_MAX)];

static void fileInit(void)
{
	Sint32 file_num;

	/* GFSの初期化 */
	GFS_DIRTBL_TYPE(&dir_tbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&dir_tbl) = dir_name;
	GFS_DIRTBL_NDIR(&dir_tbl) = MAX_DIR;

    /* ファイルシステムの初期化 */
    file_num = GFS_Init(OPEN_MAX, gfs_work, &dir_tbl);
	if (file_num < 0) {
		return;
	}

	/* エラー関数の設定 */
	GFS_SetErrFunc(errGfsFunc, NULL);
}


PcmHn createHandle(PcmCreatePara *para)
{
	PcmHn pcm;

	pcm = PCM_CreateMemHandle(para);
	if (pcm == NULL) {
		return NULL;
	}

	/* メモリ上のファイルのサイズを設定 */
	PCM_NotifyWriteSize(pcm, RING_BUF_SIZE);

	return pcm;
}

PcmCreatePara g_para[2] = {
	{
		(struct PcmWork *)&g_movie_work_1, 
		MOVIE_BUF_1, 
		RING_BUF_SIZE, 
		PCM_ADDR_1, 
		PCM_SIZE
	}, {
		(struct PcmWork *)&g_movie_work_2, 
		MOVIE_BUF_2, 
		RING_BUF_SIZE, 
		PCM_ADDR_2, 
		PCM_SIZE
	}
};

/* ループ再生機能を使う */
void smp_LoopPlay(Sint32 cnt_loop)
{
	PcmHn 			pcm[2];

	pcm[0] = createHandle(&g_para[0]);
	PCM_SetPcmStreamNo(pcm[0], 0);

	pcm[1] = createHandle(&g_para[1]);
	PCM_SetPcmStreamNo(pcm[1], 1);

	/* ループ回数の指定 */
	PCM_SetLoop(pcm[0], cnt_loop);
	PCM_SetLoop(pcm[1], cnt_loop);

	/* 開始 */
	PCM_Start(pcm[0]);
	PCM_Start(pcm[1]);

	while (1) {

		/* 再生タスク */
		PCM_Task(pcm[0]);
		PCM_Task(pcm[1]);

		/* 終了判定 */
		if (PCM_GetPlayStatus(pcm[0]) == PCM_STAT_PLAY_END) {
			PCM_Stop(pcm[1]);
			break;
		}
		if (PCM_GetPlayStatus(pcm[1]) == PCM_STAT_PLAY_END) {
			PCM_Stop(pcm[0]);
			break;
		}
	}

	/* ハンドルの消去 */
	PCM_DestroyMemHandle(pcm[0]);
	PCM_DestroyMemHandle(pcm[1]);
}

/* １回の再生毎にハンドルを作り直す */
void smp_RestartPlay(void)
{
	PcmHn 			pcm[2];
	Uint32			restart[2];

	restart[0] = restart[1] = 1;

	while (1) {
		if (restart[0]) {
			pcm[0] = createHandle(&g_para[0]);
			PCM_SetPcmStreamNo(pcm[0], 0);
			PCM_Start(pcm[0]);
			restart[0] = 0;
		}
		if (restart[1]) {
			pcm[1] = createHandle(&g_para[1]);
			PCM_SetPcmStreamNo(pcm[1], 1);
			PCM_Start(pcm[1]);
			restart[1] = 0;
		}

		/* 再生タスク */
		PCM_Task(pcm[0]);
		PCM_Task(pcm[1]);

		/* 終了判定 */
		if (PCM_GetPlayStatus(pcm[0]) == PCM_STAT_PLAY_END) {
			/* ハンドルの消去 */
			PCM_DestroyMemHandle(pcm[0]);
			restart[0] = 1;
		}
		if (PCM_GetPlayStatus(pcm[1]) == PCM_STAT_PLAY_END) {
			/* ハンドルの消去 */
			PCM_DestroyMemHandle(pcm[1]);
			restart[1] = 1;
		}
	}
}

void main(void)
{
	/* Ｖブランクの設定 */
	vblInit();

	/* ファイル初期化 */
	fileInit();

	/* サウンドの設定 */
	sndInit();

	/* ＰＣＭライブラリの初期化 */
	PCM_Init();

	/* ＡＤＰＣＭ使用宣言 (ADPCM伸張ﾗｲﾌﾞﾗﾘがﾘﾝｸされます) */
	PCM_DeclareUseAdpcm();

	/* ＰＣＭライブラリのエラーハンドルの設定 */
	PCM_SetErrFunc(errPcmFunc, NULL);

	while (1) {
#ifdef LOOP_PLAY
		smp_LoopPlay(LOOP_PLAY);
#else 
		smp_RestartPlay();
#endif
	}
}
