/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:sample program of STREAM PLAY MODE
 * File		:smppcm2.c
 * Date		:1995-03-31
 * Version	:1.16
 * Auther	:Y.T
 *
 * Comment	:
 *	ストリームシステムを使ってＣＤ上のファイルを再生するサンプル
 *	ファイル名は "SAMPLE1.AIF"
 *
 ****************************************************************************/

/*------------------------- 《プログラム選択》 -------------------------*/

/* VTV, MON を使用したデバッグの時に定義する。 */
/* #define SMPPCM_DEBUG */

/*------------------------- 《インクルード》 -------------------------*/
#include <machine.h>
#include <string.h>
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

static void vblInit(void);
static void smpVblIn(void);
static void smpVblOut(void);


/*---------------------------- 《定数》 ----------------------------*/

/* ウェーブＲＡＭの転送アドレスとサンプル数 */
#define	PCM_ADDR	((void*)0x25a20000)
#define	PCM_SIZE	(4096L*2)				/* 2.. */

/* リングバッファのサイズ */
#define	RING_BUF_SIZE	(2048L*10)			/* 10.. */

/* データの転送方式（ＣＤブロック→リングバッファ） */
#define TR_MODE_CD		PCM_TRMODE_SCU		/* ＳＣＵのＤＭＡ				*/
#if 0
#define TR_MODE_CD		PCM_TRMODE_CPU		/* ソフトウェア転送 			*/
#define TR_MODE_CD		PCM_TRMODE_SDMA		/* ＤＭＡサイクルスチール 		*/
#endif

/*----------------------- 《グローバル変数》 -----------------------*/

/* ワークバッファ */
static PcmWork g_movie_work;

/* リングバッファ */
Uint32 g_movie_buf[RING_BUF_SIZE / sizeof(Uint32)];

/* 再生するファイル名 */
static char filename[] = "SAMPLE1.AIF";


/*---------------------------- 《関数》 ----------------------------*/

void errGfsFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrGfs %X %X\n", obj, ec));
}

void errStmFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrStm %X %X\n", obj, ec));
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

/* ストリームグループのＩＤ */
static StmGrpHn grp_hd;

/* ディレクトリ情報管理領域 */
static GfsDirTbl	dir_tbl;

/* ファイル名を含んだ情報 */
static GfsDirName dir_name[MAX_DIR];

/* ＧＦＳの作業領域 */
GfsMng g_gfs_work;
Uint8 g_gfs_work2[GFS_WORK_SIZE(OPEN_MAX)-sizeof(GfsMng)];

static Uint8   stm_work[STM_WORK_SIZE(12, 24)];

static void fileInit(void)
{
	Sint32 file_num;

	/* GFSの初期化 */
	GFS_DIRTBL_TYPE(&dir_tbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&dir_tbl) = dir_name;
	GFS_DIRTBL_NDIR(&dir_tbl) = MAX_DIR;

    /* ファイルシステムの初期化 */
    file_num = GFS_Init(OPEN_MAX, &g_gfs_work, &dir_tbl);
	if (file_num < 0) {
		return;
	}

	/* エラー関数の設定 */
	GFS_SetErrFunc(errGfsFunc, NULL);
}

static void stmInit(void)
{
	/* ストリームシステムの初期化 */
	STM_Init(12, 24, stm_work);

	/* エラー関数の設定 */
	STM_SetErrFunc(errStmFunc, NULL);

	/* ストリームグループのオープン */
	grp_hd = STM_OpenGrp();
	if (grp_hd == NULL) {
		return;
	}
	STM_SetLoop(grp_hd, STM_LOOP_DFL, STM_LOOP_ENDLESS);
	STM_SetExecGrp(grp_hd);
}

static StmHn stmOpen(char *fname)
{
    Sint32 fid;
	StmKey key;

    /* ファイル名からファイル識別子を求める */
    fid = GFS_NameToId((Sint8 *)fname);
	STM_KEY_FN(&key) = STM_KEY_CN(&key) = STM_KEY_SMMSK(&key) = 
		STM_KEY_SMVAL(&key) = STM_KEY_CIMSK(&key) = STM_KEY_CIVAL(&key) =
		STM_KEY_NONE;
	return STM_OpenFid(grp_hd, fid, &key, STM_LOOP_NOREAD);
}

static void stmClose(StmHn fp)
{
	STM_Close(fp);
}

void main(void)
{
	PcmHn 			pcm;
	StmHn			stm;
	PcmCreatePara	para;
	Uint32			restart;

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

	/* ストリームシステムの初期化 */
	stmInit();

	restart = 1;

	for (;;) {
		if (restart) {
			/* ストリームオープン */
			if ((stm = stmOpen(filename)) == NULL) {
				return;
			}

			/* 取り出し領域のリセット
			 *	ストリームシステムの仕様が...
			 *	最初の転送を STM_ExecServer で行うなら、これは不要。
			*/
			STM_ResetTrBuf(stm);

			/* ハンドルの作成 */
			PCM_PARA_WORK(&para) = (struct PcmWork *)&g_movie_work;
			PCM_PARA_RING_ADDR(&para) = (Sint8 *)g_movie_buf;
			PCM_PARA_RING_SIZE(&para) = RING_BUF_SIZE;
			PCM_PARA_PCM_ADDR(&para) = PCM_ADDR;
			PCM_PARA_PCM_SIZE(&para) = PCM_SIZE;
			pcm = PCM_CreateStmHandle(&para, stm);
			if (pcm == NULL) {
				return;
			}

			/* データの転送方式の設定（ＣＤブロック→リングバッファ） */
			/* PCM_SetTrModeCd(pcm, TR_MODE_CD); */

			/* 開始 */
			PCM_Start(pcm);

			restart = 0;
		}

		/* サーバの実行 */
		STM_ExecServer();

		/* 再生タスク */
		PCM_Task(pcm);

		/* ムービの終了判定 */
		if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {

			/* ハンドルの消去 */
			PCM_DestroyStmHandle(pcm);

			/* ストリームのクローズ*/
			stmClose(stm);

			restart = 1;
		}
	}
}
