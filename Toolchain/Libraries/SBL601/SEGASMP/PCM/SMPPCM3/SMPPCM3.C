/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:sample program of MEMORY PLAY MODE
 * File		:smppcm3.c
 * Date		:1995-03-31
 * Version	:1.16
 * Auther	:Y.T
 *
 * Comment	:
 *	メモリ上にあるファイルを再生するサンプル。
 *	ファイルは 0x4000000 番地にロードしておくこと。
 *	再生できるファイルサイズは１６ＭＢまで。
 *
 ****************************************************************************/

/*------------------------- 《プログラム選択》 -------------------------*/

/* VTV, MON を使用したデバッグの時に定義する。 */
/* #define SMPPCM_DEBUG */

/* リングバッファ機能のテスト */
/* #define RING_BUF_TEST */

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

#if	1
	/*
	**■1995-07-28	高橋智延
	**	SCL_VblankStart(), SCL_VBlankEnd() を使っているので追加
	*/
	#include	"sega_scl.h"
#endif

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
#define	PCM_ADDR	((void*)0x25a20000)
#define	PCM_SIZE	(4096L*2)				/* 2.. */

/* リングバッファのサイズ */
#define	RING_BUF_SIZE	(1024L*1024L*16)


/*----------------------- 《グローバル変数》 -----------------------*/

/* ワーク */
static PcmWork g_movie_work;

/* リングバッファ */
static Sint8 *g_movie_buf = (Sint8 *)0x04000000;


#ifdef RING_BUF_TEST
	#define SIZE_SECTOR				(2048)
	#define SIZE_COPY_RING			(SIZE_SECTOR)
	#define SIZE_RING_BUF			(200*1024)

	Uint32 ring_buf[SIZE_RING_BUF / 4];
#endif

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
	SCL_VblankStart();
}

static void smpVblOut(void)
{
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankEnd();
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


void main(void)
{
	PcmHn 			pcm;
	Uint32			restart;
	PcmCreatePara	para;
#ifdef RING_BUF_TEST
	Uint32 			addr_write, addr_data1, addr_data2;
	Sint32 			size_write, size_write_total, size_copy;
#endif


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

	restart = 1;

	for (;;) {
		if (restart) {

			/* ハンドルの作成 */
			PCM_PARA_WORK(&para) = (struct PcmWork *)&g_movie_work;
			PCM_PARA_RING_ADDR(&para) = g_movie_buf;
			PCM_PARA_RING_SIZE(&para) = RING_BUF_SIZE;
			PCM_PARA_PCM_ADDR(&para) = PCM_ADDR;
			PCM_PARA_PCM_SIZE(&para) = PCM_SIZE;
#ifdef RING_BUF_TEST
			PCM_PARA_RING_ADDR(&para) = (Sint8 *)ring_buf;
			PCM_PARA_RING_SIZE(&para) = SIZE_RING_BUF;
			addr_data1 = (Uint32)g_movie_buf;
#endif
			pcm = PCM_CreateMemHandle(&para);
			if (pcm == NULL) {
				return;
			}

#ifndef RING_BUF_TEST
			/* メモリ上のファイルのサイズを設定 */
			PCM_NotifyWriteSize(pcm, RING_BUF_SIZE);
#endif

			/* 開始 */
			PCM_Start(pcm);

			restart = 0;
		}

#ifdef RING_BUF_TEST
		for (;;) {
			addr_write = (Uint32)PCM_GetWriteBuf(pcm, &size_write, &size_write_total);
			if (size_write == 0) {
				break;
			} else if (size_write < SIZE_COPY_RING) {
				size_copy = size_write;
			} else {
				size_copy = SIZE_COPY_RING;
			}
			memcpy((void *)addr_write, (void *)addr_data1, size_copy);

			#ifdef __PCMD
				debug_write_str("WRin");
				debug_write_data((Uint32)addr_write);
				debug_write_data((Uint32)addr_data1);
				debug_write_data((Uint32)size_copy);
			#endif
			
			PCM_NotifyWriteSize(pcm, size_copy);
			addr_data1 += size_copy;
		}
#endif

		/* 再生タスク */
		PCM_Task(pcm);

		/* 終了判定 */
		if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {

			/* ハンドルの消去 */
			PCM_DestroyMemHandle(pcm);

			restart = 1;
		}
	}
}
