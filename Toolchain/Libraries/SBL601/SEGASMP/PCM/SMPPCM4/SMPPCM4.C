/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:sample program of FILE PLAY MODE, LOOP PLAY
 * File		:smppcm4.c
 * Date		:1995-03-31
 * Version	:1.16
 * Auther	:Y.T
 *
 * Comment	:
 *	ファイルシステムを使ってＣＤ上のファイルを再生するサンプル
 *	ファイル名は "SAMPLE1.AIF"
 *
 ****************************************************************************/

/*------------------------- 《プログラム選択》 -------------------------*/

/* VTV, MON を使用したデバッグの時に定義する。 */
/* #define SMPPCM_DEBUG */

/* 画面表示を行う */
/*	#define USE_VDP	*/

/* ループ再生を行う */
#define LOOP_PLAY		(0x7fffffff)

/*------------------------- 《インクルード》 -------------------------*/
#include <stdio.h>
#include <machine.h>
#include <string.h>
#define _SH
#include "sega_xpt.h"
#include "sega_sys.h"
#include "sega_def.h"
#include "sega_mth.h"
#include "sega_scl.h" 
#include "sega_int.h"
#define  _SPR2_
#include "sega_spr.h"
#include "sega_dma.h"
#include "sega_per.h"
#include "sega_cdc.h"
#include "sega_gfs.h"
#include "sega_stm.h"
#include "sega_snd.h"
#include "sega_pcm.h"

#if	1
/*
**■1995-10-02	高橋智延
**	ペリフェラルライブラリの古いＩ／Ｆ関数を削除したので。
*/
	#include	"per_x.h"
#endif

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
#ifdef	USE_VDP
/*
**■1995-07-28	高橋智延
**	今は使ってないので削除
*/
static void smpSprEnd(void);
#endif

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

/* ポーズ処理用ワークのサイズ */
#define PWORK_SIZE		(2 * 4096)

/* ＴＶ画面のサイズ */
#define DISP_XSIZE		(320)
#define DISP_YSIZE		(224)

/* ボリュームの最小値，最大値 */
#define LEVEL_MIN	(0)
#define LEVEL_MAX	(7)

/* パンの最小値，最大値，中央値 */
#define PAN_MIN		(0)					/* 左端：左は最大、右はゼロ */
#define PAN_MAX		(31)				/* 右端：右は最大、左はゼロ */
#define PAN_CENTER	((PAN_MAX + 1) / 2)	/* 中央：左も、右も最大 	*/


/*----------------------- 《グローバル変数》 -----------------------*/

/* ワークバッファ */
static PcmWork g_movie_work;

/* リングバッファ */
static Uint32 g_movie_buf[RING_BUF_SIZE / sizeof(Uint32)];

/* ポーズ処理用ワーク */
static Sint32 pwork_addr[PWORK_SIZE / 4];

/* 再生するファイル名 */
static char filename[] = "SAMPLE1.AIF";

/* パッド情報 */
#if	0
/*
**■1995-10-02	高橋智延
**	ペリフェラルライブラリの古いＩ／Ｆ関数を削除したので。
*/
static Uint32	PadWorkArea[7];
static	Uint16	PadData1  = 0x0000;
static	Uint16	PadData1E = 0x0000;
static	Uint16	PadData2  = 0x0000;
static	Uint16	PadData2E = 0x0000;
#else
static SysPort	*__port = NULL;
static volatile trigger_t	PadData1  = 0x0000;
static volatile trigger_t	PadData1E = 0x0000;
static volatile trigger_t	PadData2  = 0x0000;
static volatile trigger_t	PadData2E = 0x0000;
#endif

#if	0
/*
**■1995-07-28	高橋智延
**	使ってないので削除
*/
/* ハンドルの強制切り替えフラグ */
static Bool		g_change_flag;
#endif

/* ポーズの状態 */
static Bool		g_pause_at_once;

/* ボリュームの値 */
Sint32 g_level = LEVEL_MAX;

/* パンの値：左右を 0..31 で表現する値 */
Sint32 g_pan = PAN_CENTER;

/* パンの値：引数に指定する値 */
Sint32 g_pan_tbl[PAN_MAX + 1] = {
	0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 
	0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

/* スプライト描画終了判定フラグ   TRUE : 描画終了 */
static volatile Bool g_spr_end;

/* VblInカウンタ */
Sint32 volatile cnt_vbl_in = 0;


/*---------------------------- 《関数》 ----------------------------*/

void errGfsFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrGfs %X %X\n", obj, ec));
}

void errPcmFunc(void *obj, Sint32 ec)
{
	VTV_PRINTF((VTV_s, "S:ErrPcm %X %X\n", obj, ec));
}


/*====================== 画面表示の処理 ===========================*/

/* フレームバッファの切り替えと、描画終了待ち。何せず単純にウエイト。 */
void displayFrameWait(void)
{
	/* フレームバッファの切り替え */
	SCL_DisplayFrame();

	/* ＶＲＡＭからフレームバッファへの描画終了待ち */
	g_spr_end = FALSE;
	while (g_spr_end == FALSE) ;
}
/* フレームバッファの切り替えと、描画終了待ち。切り替え中にCPUは働く。 */
void waitDisplayFrame(void)
{
	/* ＶＲＡＭからフレームバッファへの(前回の)描画終了待ち */
	while (g_spr_end == FALSE) ;

	/* フレームバッファの切り替え */
	SCL_DisplayFrame();

	g_spr_end = FALSE;
}

#ifdef	USE_VDP
	/*
	**■1995-07-28	高橋智延
	**	いまは使ってないので削除
	*/
static void dispInit(void)
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
    Uint8  			*VRAM;
    Uint8  			val;
	Uint16			BackCol;
	Sint32			vdp1;
#else
    Uint8  			*VRAM;
	Uint16			BackCol;
#endif

	set_imask(0);
	SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
					SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7, 7);
	SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);

	/* Ｖブランクの設定 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);

	/* V_Blank Out 割り込みを待つ */
	(*((volatile Uint32 *)0x25fe00a4)) &= 0xfffffffc;	/* まずクリア */
	while( !((*((volatile Uint32 *)0x25fe00a4)) & 2) );
	PER_Init(PER_KD_PERTIM,2,PER_ID_DGT,PER_SIZE_DGT,PadWorkArea,0);

	INT_SetScuFunc(INT_SCU_VBLK_IN, smpVblIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT, smpVblOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);

	/* スプライト描画終了割り込みの設定 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_SPR);
	INT_SetScuFunc(INT_SCU_SPR, smpSprEnd);
	INT_ChgMsk(INT_MSK_SPR, INT_MSK_NULL);

	BackCol = 0x0000;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	SPR_Initial(&VRAM);
	SPR_SetEraseData(RGB16_COLOR(0,0,0),0,0,DISP_XSIZE-1,DISP_YSIZE-1);
	SCL_SetFrameInterval(1);
	SCL_DisplayFrame();
	SCL_DisplayFrame();
	SCL_SetFrameInterval(-1);
}
#endif

/*====================== Ｖブランクの処理 ===========================*/
#if	0
/*
**■1995-10-02	高橋智延
**	ペリフェラルライブラリの古いＩ／Ｆ関数を削除したので。
*/
static void vblInit(void)
{
    set_imask(0);

	/* Ｖブランクの設定 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);

	/* V_Blank Out 割り込みを待つ */
	(*((volatile Uint32 *)0x25fe00a4)) &= 0xfffffffc;	/* まずクリア */
	while( !((*((volatile Uint32 *)0x25fe00a4)) & 2) );
	PER_Init(PER_KD_PERTIM,2,PER_ID_DGT,PER_SIZE_DGT,PadWorkArea,0);

	INT_SetScuFunc(INT_SCU_VBLK_IN, smpVblIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT, smpVblOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);
}

static void smpVblIn(void)
{
	cnt_vbl_in++;

	/* ＰＣＭライブラリの VblIn ルーチンをコール */
	PCM_VblIn();

	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankStart();

	SMPPCMD_VblIn();
}

static void smpVblOut(void)
{
	PerDgtInfo	*pad;

	PER_GetPer((PerGetPer **)&pad);
	PadData1  = pad[0].data ^ 0xffff;
	PadData1E |= pad[0].push ^ 0xffff;
	PadData2  = pad[1].data ^ 0xffff;
	PadData2E |= pad[1].push ^ 0xffff;

	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankEnd();
}
#else
static void	vblInit( void ){
	
	__port = PER_OpenPort();
	
	/* V-Blank割り込みルーチンの登録 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_IN,smpVblIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT,smpVblOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);
}
static void	smpVblIn( void ){
	cnt_vbl_in++;
	
	/* ＰＣＭライブラリの VblIn ルーチンをコール */
	PCM_VblIn();
	
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankStart();
	
	SMPPCMD_VblIn();
}
static void	smpVblOut( void ){
	SCL_VblankEnd();
	
	if( __port != NULL ){
		SysDevice	*device;
		
		if(( device = PER_GetDeviceR( &__port[0], 0 )) != NULL ){
			trigger_t	prev = PadData1;
			trigger_t	current = PER_GetTrigger( device );
			
			PadData1  = current;
			PadData1E = PER_GetPressEdge( prev, current );
		}
		if(( device = PER_GetDeviceR( &__port[1], 0 )) != NULL ){
			trigger_t	prev = PadData2;
			trigger_t	current = PER_GetTrigger( device );
			
			PadData2  = current;
			PadData2E = PER_GetPressEdge( prev, current );
		}
		
		PER_GetPort( __port );
	}
}
#endif

#ifdef	USE_VDP
/*
**■1995-07-28	高橋智延
**	今は使ってないので削除
*/
static void smpSprEnd(void)
{
	g_spr_end = TRUE;
}
#endif

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

static GfsHn fileOpen(char *fname)
{
    Sint32 fid;

    /* ファイル名からファイル識別子を求める */
    fid = GFS_NameToId((Sint8 *)fname);
    return GFS_Open(fid);
}

static void fileClose(GfsHn fp)
{
	GFS_Close(fp);
}


/********************************************************************/
/* パッド制御														*/
/********************************************************************/

/* パッド制御：再生ライブラリ */
void smp_PadCtrlPcm(PcmHn pcm, Uint16 PadData1EW, Uint16 PadData1W)
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
	int		i;
#endif

	/* ポーズ */
	if (PadData1EW & PER_DGT_S) {
		PadData1EW = 0;
		if (g_pause_at_once == FALSE) {
			/* 開始 */
			g_pause_at_once = TRUE;
			PCM_SetPauseWork(pwork_addr, PWORK_SIZE);
			PCM_Pause(pcm, PCM_PAUSE_ON_AT_ONCE);
		} else {
			/* 解除 */
			g_pause_at_once = FALSE;
			PCM_Pause(pcm, PCM_PAUSE_OFF);
		}
	}

	/* ストップ */
	if (PadData1EW & PER_DGT_A) {
		PadData1EW = 0;
		PCM_Stop(pcm);
	}

	/* ボリューム・アップ */
	if (PadData1EW & PER_DGT_U) {
		PadData1EW = 0;
		g_level = MIN(g_level + 1, LEVEL_MAX);
		PCM_SetVolume(pcm, g_level);
		PCM_ChangePcmPara(pcm);
	}
	/* ボリューム・ダウン */
	if (PadData1EW & PER_DGT_D) {
		PadData1EW = 0;
		g_level = MAX(g_level - 1, LEVEL_MIN);
		PCM_SetVolume(pcm, g_level);
		PCM_ChangePcmPara(pcm);
	}

	/* パン・右より（右最大、左を徐々に弱く） */
	if (PadData1EW & PER_DGT_R) {
		PadData1EW = 0;
		g_pan = MIN(g_pan + 1, PAN_MAX);
		PCM_SetPan(pcm, g_pan_tbl[g_pan]);
		PCM_ChangePcmPara(pcm);
	}
	/* パン・左より（左最大、右を徐々に弱く） */
	if (PadData1EW & PER_DGT_L) {
		PadData1EW = 0;
		g_pan = MAX(g_pan - 1, PAN_MIN);
		PCM_SetPan(pcm, g_pan_tbl[g_pan]);
		PCM_ChangePcmPara(pcm);
	}
}

#ifdef SMPPCM_DEBUG
	#define SMP_PAD_MODE_NUM		(4)
#else
	#define SMP_PAD_MODE_NUM		(1)
#endif
Sint32 smp_pad_mode = 0;

/* パッド制御 */
void smp_PadCtrl(PcmHn pcm)
{
	Uint16   PadData1EW;
	Uint16   PadData1W;

	PadData1EW = PadData1E;
	PadData1E = 0;
	PadData1W = PadData1;
	PadData1 = 0;

	/* パッド処理対象の切り替え */
	if (PadData1EW & PER_DGT_TL) {
		PadData1EW = 0;
		smp_pad_mode++;
	}

	switch (smp_pad_mode % SMP_PAD_MODE_NUM) {
	case 0:
		/* パッド制御：再生ライブラリ */
		smp_PadCtrlPcm(pcm, PadData1EW, PadData1W);
		break;
#ifdef VTV
	case 1:
		/* パッド制御：一般出力用の VTV */
		smp_PadCtrlVtv0(PadData1EW, PadData1W);
		break;
#endif
#ifdef MONITOR_TASK
	case 2:
		/* パッド制御：モニタ用の VTV */
		smp_PadCtrlVtv1(PadData1EW, PadData1W);
		break;
	case 3:
		/* パッド制御：モニタ用 */
		smp_PadCtrlMon(PadData1EW, PadData1W);
		break;
#endif
	}
}

/* ループ再生機能を使う */
void smp_LoopPlay(Sint32 cnt_loop){
	GfsHn			gfs;
	PcmHn 			pcm;
	PcmCreatePara	para;

	/* ファイルオープン */
	if ((gfs = fileOpen(filename)) == NULL) {
		return;
	}

	/* ハンドルの作成 */
	PCM_PARA_WORK(&para) = (struct PcmWork *)&g_movie_work;
	PCM_PARA_RING_ADDR(&para) = (Sint8 *)g_movie_buf;
	PCM_PARA_RING_SIZE(&para) = RING_BUF_SIZE;
	PCM_PARA_PCM_ADDR(&para) = PCM_ADDR;
	PCM_PARA_PCM_SIZE(&para) = PCM_SIZE;
	pcm = PCM_CreateGfsHandle(&para, gfs);
	if (pcm == NULL) {
		return;
	}

	/* データの転送方式の設定（ＣＤブロック→リングバッファ） */
	/* PCM_SetTrModeCd(pcm, TR_MODE_CD); */

	/* ループ回数の指定 */
	PCM_SetLoop(pcm, cnt_loop);

	/* 開始 */
	PCM_Start(pcm);

	while (1) {
		/* 再生タスク */
		SMPPCMD_PCM_Task(pcm);

		/* パッド制御 */
		smp_PadCtrl(pcm);

#ifdef USE_VDP
		#ifdef MONITOR_TASK
			MON_UnitStart(smpmon_hn, smpmon_uhn[3]);
		#endif

		/* フレームバッファの切り替えと、描画終了待ち */
		waitDisplayFrame();

		#ifdef MONITOR_TASK
			MON_UnitEnd(smpmon_hn, smpmon_uhn[3]);
		#endif
#endif

		if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {
			/* 指定回数の再生が終わったか,または PCM_Stop で停止した */
			break;
		}
	}

	/* ハンドルの消去 */
	PCM_DestroyGfsHandle(pcm);

	/* ファイルのクローズ*/
	fileClose(gfs);
}

/* １回の再生毎にハンドルを作り直す */
void smp_RestartPlay(void)
{
	GfsHn			gfs;
	PcmHn 			pcm;
	PcmCreatePara	para;
	Uint32			restart = 1;

	while (1) {
		if (restart) {
			/* ファイルオープン */
			if ((gfs = fileOpen(filename)) == NULL) {
				return;
			}

			/* ハンドルの作成 */
			PCM_PARA_WORK(&para) = (struct PcmWork *)&g_movie_work;
			PCM_PARA_RING_ADDR(&para) = (Sint8 *)g_movie_buf;
			PCM_PARA_RING_SIZE(&para) = RING_BUF_SIZE;
			PCM_PARA_PCM_ADDR(&para) = PCM_ADDR;
			PCM_PARA_PCM_SIZE(&para) = PCM_SIZE;
			pcm = PCM_CreateGfsHandle(&para, gfs);
			if (pcm == NULL) {
				return;
			}

			/* データの転送方式の設定（ＣＤブロック→リングバッファ） */
			/* PCM_SetTrModeCd(pcm, TR_MODE_CD); */

			/* 開始 */
			PCM_Start(pcm);

			restart = 0;
		}

		/* 再生タスク */
		SMPPCMD_PCM_Task(pcm);

		/* パッド制御 */
		smp_PadCtrl(pcm);

#ifdef USE_VDP
		#ifdef MONITOR_TASK
			MON_UnitStart(smpmon_hn, smpmon_uhn[3]);
		#endif

		/* フレームバッファの切り替えと、描画終了待ち */
		waitDisplayFrame();

		#ifdef MONITOR_TASK
			MON_UnitEnd(smpmon_hn, smpmon_uhn[3]);
		#endif
#endif

		/* 終了判定 */
		if (PCM_GetPlayStatus(pcm) == PCM_STAT_PLAY_END) {

			/* ハンドルの消去 */
			PCM_DestroyGfsHandle(pcm);

			/* ファイルのクローズ*/
			fileClose(gfs);

			SMPPCMD_MON_Reset();

			restart = 1;
		}
	}
}

void main(void)
{

#ifdef USE_VDP
	/* スプライトの設定 */
	dispInit();
#else
	/* Ｖブランクの設定 */
	vblInit();
#endif

	SMPPCMD_Init(0);

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
