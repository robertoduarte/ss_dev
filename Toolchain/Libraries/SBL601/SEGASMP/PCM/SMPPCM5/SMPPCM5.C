/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:sample program of STREAM PLAY MODE & SEAMLESS BRANCH
 * File		:smppcm5.c
 * Date		:1995-03-31
 * Version	:1.16
 * Auther	:Y.T
 *
 * Comment	:
 *		ストリームシステムを使ってＣＤ上のファイルをスムーズに連続再生する。
 *		ＣＤ上で、後半のファイルは、前半のファイルの終了位置より後ろに配置
 *		されている必要があります。
 *		このサンプルプログラムは、CD-ROM XA Audio の再生もできます。プログ
 *		ラム中の、次の宣言を有効にしてコンパイルして下さい。
 *			#define CD_ROM_XA_AUDIO
 *		
 * ファイル名：
 *		 				通常のﾌｧｲﾙの再生	CD-ROM XA Audio 再生
 *		 前半のファイル　\SAMPLE1.AIF		\PHOTO_CD\ADPCM\SEQ00002.PCD
 *		 後半のファイル　\SAMPLE2.AIF		\PHOTO_CD\ADPCM\SEQ00030.PCD
 *
 * パッド操作方法：
 *　[Ａ]
 *		前半のファイルを再生中にＡボタンを押すと、強制的に後半のファイルに
 *		切り替えて再生を開始します。
 *		Ａボタンを押さなければ、前半のファイルの終了後スムーズに後半のファ
 *		イルの再生を開始します。
 *		後半のファイルを再生中にＡボタンを押すと、そのファイルの再生を中断
 * 		し、最初に戻ります。
 *	[START]
 *		 一度目はポーズ、もう一度押すとポーズ解除。
 *	[UP], [DOWN]
 *		ボリュームの設定。０～７。初期値は７。
 *	[LEFT], [RIGHT]
 *		パンの設定。初期値は、左も、右も最大。
 *		パンの設定は、モノラルの再生の場合だけ有効です。
 ****************************************************************************/

/*------------------------- 《プログラム選択》 -------------------------*/

/* VTV, MON を使用したデバッグの時に定義する。 */
/* #define SMPPCM_DEBUG */

/* DISC type */
/* #define CD_ROM_XA_AUDIO */

/* 画面表示を行う */
/*	#define USE_VDP	*/

/* DISC */
#ifdef CD_ROM_XA_AUDIO
	#define DISC_PHOTO_CD			/* Photo CD "PORTABLE PALYER DEMO DISC"*/
/*	#define DISC_SEIMEI_HANDAN */
#endif

/* いつも STM_ExecServer を使用する */
#define ALLWAYS_STM_EXECSERVER


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


/*------------------------- 《マクロ定数》 -------------------------*/

/* スプライト面のＶＲＡＭのアドレス */
#define ADDR_VDP1 				(0x25C00000)

/* ＶＲＡＭの転送先のアドレス */
#define ADDR_VRAM_PCM 			(0x25C08000)

/* ウェーブＲＡＭの転送アドレスとサンプル数 */
#define	PCM_ADDR	((void*)0x25a20000)
#define	PCM_SIZE	(4096L*2)				/* 2.. */

/* サイズ */
#ifdef CD_ROM_XA_AUDIO
	#define	SECTOR_SIZE		(2324L)
#else
	#define	SECTOR_SIZE		(2048L)
#endif

/* リングバッファのサイズ */
#define	RING_BUF_SIZE	(SECTOR_SIZE * 100)		/* 10.. */

/* ポーズ処理用ワークのサイズ */
#define PWORK_SIZE		(2 * 4096)

/* ＴＶ画面のサイズ */
#define DISP_XSIZE		(320)
#define DISP_YSIZE		(224)

/* 再生するファイルの数 */
#define	FILE_NUM		2

/* ボリュームの最小値，最大値 */
#define LEVEL_MIN	(0)
#define LEVEL_MAX	(7)

/* パンの最小値，最大値，中央値 */
#define PAN_MIN		(0)					/* 左端：左は最大、右はゼロ */
#define PAN_MAX		(31)				/* 右端：右は最大、左はゼロ */
#define PAN_CENTER	((PAN_MAX + 1) / 2)	/* 中央：左も、右も最大 	*/

/* STM_ExecServer をコールするＶブランク周期 */
#define VBL_RATE_STM_EXEC_SERVER		(1)

/* 最大転送セクタ数 */
#define SMP_LOAD_NUM			(10)

/* １回のタスクで処理する量の上限の設定 [sample/1ch] */
/* #define SMP_1TASK_SAMPLE		(512) */
/* #define SMP_1TASK_SAMPLE		(600) */	/* CD-ROM XA Bmode の限界 */
#define SMP_1TASK_SAMPLE		(1024) 
/* #define SMP_1TASK_SAMPLE		(2048) */

/* データの転送方式 (ＣＤブロック→リングバッファ)				*/
/*		PCM_TRMODE_CPU or PCM_TRMODE_SDMA or PCM_TRMODE_SCU		*/
#define SMP_TR_MODE_CD			(PCM_TRMODE_SDMA)

/*----------------------- 《グローバル変数》 -----------------------*/

/* ワークバッファ */
static PcmWork g_movie_work[FILE_NUM];

/* リングバッファ */
static Uint32 g_movie_buf[FILE_NUM][RING_BUF_SIZE / sizeof(Uint32)];

/* ポーズ処理用ワーク */
static Sint32 pwork_addr[PWORK_SIZE / 4];

/* 再生するファイル名 */
#ifdef DISC_PHOTO_CD
	static char *filename[] = {"SEQ00002.PCD", "SEQ00030.PCD"};
/*	static char *filename[] = {"SEQ00002.PCD", "SEQ00005.PCD"}; */
#else
	 static char *filename[] = {"SAMPLE1.AIF", "SAMPLE2.AIF"}; 
	/* static char *filename[] = {"SAMPLE1M.AIF", "SAMPLE2M.AIF"}; */
	/* static char *filename[] = {"F44K16S.ADP", "F44K16S2.ADP"}; */
	/* static char *filename[] = {"F44K16M.ADP", "F44K16M2.ADP"}; */
	/* static char *filename[] = {"F22K16S.ADP", "F22K16S2.ADP"}; */
	/* static char *filename[] = {"F11K16S.ADP", "F11K16S2.ADP"}; */
	/* static char *filename[] = {"F11K16M.ADP", "F11K16M2.ADP"}; */
#endif


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

/* ハンドルの強制切り替えフラグ */
static Bool		g_change_flag;

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
static Bool g_spr_end;

/* VblInカウンタ */
Sint32 cnt_vbl_in = 0;

/* STM_ExecServer をコールした時刻[vbl/vbl_rate]を保持 */
Sint32 call_stm_exec_server;


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
**	今は使ってないので削除
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
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
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

	/* ＰＣＭの VblIn 割り込み処理 */
	PCM_VblIn();

#ifdef USE_VDP
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankStart();
#endif

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

#ifdef USE_VDP
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankEnd();
#endif

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
	
#if	defined( USE_VDP )
	/* グラフィックライブラリを使用する為には実行しなければならない */
	SCL_VblankStart();
#endif
	
	SMPPCMD_VblIn();
}
static void	smpVblOut( void ){
#if	defined( USE_VDP )
	SCL_VblankEnd();
#endif
	
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
#define OPEN_MAX	FILE_NUM

/* ストリームグループのＩＤ */
static StmGrpHn grp_hd;

#ifdef	DISC_PHOTO_CD
/*
**■1995-07-28	高橋智延
**	今は使ってないので削除
*/
/* ディレクトリ情報管理領域 */
static GfsDirTbl	dir_tbl, dir_tbl_1, dir_tbl_2;

/* ファイル名を含んだ情報 */
static GfsDirName dir_name[MAX_DIR];
static GfsDirName dir_name_1[MAX_DIR];
static GfsDirName dir_name_2[MAX_DIR];
#else
static GfsDirTbl	dir_tbl;
static GfsDirName	dir_name[MAX_DIR];
#endif

/* ＧＦＳの作業領域 */
static Uint8 g_gfs_work[GFS_WORK_SIZE(OPEN_MAX)];

static Uint8 g_stm_work[STM_WORK_SIZE(12, 24)];

#ifdef	DISC_PHOTO_CD
/*
**■1995-07-28	高橋智延
**	今は使ってないので削除
*/
static void changeDir(char *fname, GfsDirTbl *dir_tbl, GfsDirName *dir_name)
{
    Sint32 fid;

    fid = GFS_NameToId((Sint8 *)fname);

	GFS_DIRTBL_TYPE(dir_tbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(dir_tbl) = dir_name;
	GFS_DIRTBL_NDIR(dir_tbl) = MAX_DIR;

	for (;;) {
		if (GFS_LoadDir(fid, dir_tbl) >= 0) {
			break;
		}
	}
	for (;;) {
		if (GFS_SetDir(dir_tbl) == 0) {
			break;
		}
	}
}
#endif

static void fileInit(void)
{
	Sint32 file_num;

	/* GFSの初期化 */
	GFS_DIRTBL_TYPE(&dir_tbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&dir_tbl) = dir_name;
	GFS_DIRTBL_NDIR(&dir_tbl) = MAX_DIR;

    /* ファイルシステムの初期化 */
    file_num = GFS_Init(OPEN_MAX, g_gfs_work, &dir_tbl);
	if (file_num < 0) {
		return;
	}

	/* エラー関数の設定 */
	GFS_SetErrFunc(errGfsFunc, NULL);
}

static void stmInit(void)
{
#ifdef DISC_PHOTO_CD
	changeDir("PHOTO_CD", &dir_tbl_1, dir_name_1); 
	changeDir("ADPCM", &dir_tbl_2, dir_name_2);
#endif

	/* ストリームシステムの初期化 */
	STM_Init(12, 24, g_stm_work);

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
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
	Sint32 fad;
	Sint32 i;
#endif

    /* ファイル名からファイル識別子を求める */
    fid = GFS_NameToId((Sint8 *)fname);
	STM_KEY_FN(&key) = STM_KEY_CN(&key) = STM_KEY_SMMSK(&key) = 
		STM_KEY_SMVAL(&key) = STM_KEY_CIMSK(&key) = STM_KEY_CIVAL(&key) =
		STM_KEY_NONE;

#ifdef CD_ROM_XA_AUDIO
	STM_KEY_SMMSK(&key) = STM_KEY_SMVAL(&key) = 0x04;/* Audio Sector */
	STM_KEY_CN(&key) = 0;	/* channel No. */
#endif

#ifdef DISC_PHOTO_CD
	/* Stream system (Ver1.13) can't get correct file size, that file was 
	 * intreleaveed in Photo CD disc. 
	 */

	/* fidとオフセット(==0)からフレームアドレスを取得する */
	fad = GFS_GetFad(fid, 0);

	/* search dir_name table */
	for (i = 0; i < MAX_DIR; i++) {
		if (GFS_DIR_FAD(&dir_name_2[i]) == fad) {
			/* do patch */
			GFS_DIR_UNIT(&dir_name_2[i]) = 1;
			GFS_DIR_GAP(&dir_name_2[i]) = 0;
			break;
		}
	}
#endif

	return STM_OpenFid(grp_hd, fid, &key, STM_LOOP_NOREAD);
}

static void stmClose(StmHn fp)
{
	STM_Close(fp);
}


static Bool goNextStm(StmHn stm)
{
	Sint32		fad;
	Sint32		fid;
	StmFrange	frange;
	Sint32		bn;
	StmKey		stmkey;

	/* ストリームの再生範囲を取得する */
	STM_GetInfo(stm, &fid, &frange, &bn, &stmkey);

	/* 再生中のＦＡＤの位置を取得する */
	STM_GetExecStat(grp_hd, &fad);
	
	if (fad < STM_FRANGE_SFAD(&frange)) {
		/* ピックアップの移動 */
		STM_MovePickup(stm, 0);
		return TRUE;
	} else {
		return FALSE;
	}
}

Sint32 smpIsInFrage(Sint32 fad, StmHn stm)
{
	Sint32		fid;
	StmFrange	frange;
	Sint32		bn;
	StmKey		stmkey;

	/* ストリーム情報の取得			*/
	STM_GetInfo(stm, &fid, &frange, &bn, &stmkey);

	if (fad >= STM_FRANGE_SFAD(&frange) &&
		fad < STM_FRANGE_SFAD(&frange) + STM_FRANGE_FASNUM(&frange)) {
		return 1;
	} else {
		return 0;
	}
}

/* ストリームシステムサーバの実行 */
static void smpStmTask(StmHn stm[])
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
	Sint32 		stat, fad, flag_stm_exec_trans;
#endif

	if (call_stm_exec_server < cnt_vbl_in / VBL_RATE_STM_EXEC_SERVER) {

		#ifdef MONITOR_TASK
			MON_UnitStart(smpmon_hn, smpmon_uhn[0]);
		#endif

#ifdef ALLWAYS_STM_EXECSERVER
		STM_ExecServer();
#else
		if (call_stm_exec_server < 0) {
			STM_ExecServer();
		} else {

			/* 必要な場合だけ STM_ExecServer をコールする */
			flag_stm_exec_trans = 0;
			if (STM_GetNumCdbuf(stm[0]) > 0) {
				STM_ExecTrans(stm[0]);
				flag_stm_exec_trans = 1;
			}
			if (STM_GetNumCdbuf(stm[1]) > 0) {
				STM_ExecTrans(stm[1]);
				flag_stm_exec_trans = 1;
			}
			if (flag_stm_exec_trans == 0) {
				STM_ExecServer();
			}
		}
#endif

		#ifdef MONITOR_TASK
			MON_UnitEnd(smpmon_hn, smpmon_uhn[0]);
		#endif

		if (call_stm_exec_server < 0) {
			call_stm_exec_server++;
		} else {
			call_stm_exec_server = cnt_vbl_in / VBL_RATE_STM_EXEC_SERVER;
		}
	}
}

/* ＰＣＭ・ＡＤＰＣＭ再生ライブラリの初期化 */
void smpPcmInit(void)
{
	/* ＰＣＭ・ＡＤＰＣＭ再生ライブラリの初期化 */
	PCM_Init();

	/* ＡＤＰＣＭ使用宣言 (ADPCM伸張ﾗｲﾌﾞﾗﾘがﾘﾝｸされます) */
	PCM_DeclareUseAdpcm();

	/* ＰＣＭライブラリのエラーハンドルの設定 */
	PCM_SetErrFunc(errPcmFunc, NULL);
}


static PcmHn createHandle(StmHn stm, int file_no)
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
	static Uint32 movie_x, movie_y;
#endif
	PcmCreatePara	para;
	PcmHn			pcm;

	/* ハンドルの作成 */
	PCM_PARA_WORK(&para) = (struct PcmWork *)&g_movie_work[file_no];
	PCM_PARA_RING_ADDR(&para) = (Sint8 *)g_movie_buf[file_no];
	PCM_PARA_RING_SIZE(&para) = RING_BUF_SIZE;
	PCM_PARA_PCM_ADDR(&para) = PCM_ADDR;
	PCM_PARA_PCM_SIZE(&para) = PCM_SIZE;
	pcm = PCM_CreateStmHandle(&para, stm);
	if (pcm == NULL) {
		return NULL;
	}

	if (file_no == 0) {
		/* ピックアップの移動 */
		STM_MovePickup(stm, 0);
	}

	PCM_SetVolume(pcm, g_level);
	PCM_SetPan(pcm, g_pan_tbl[g_pan]);
	return pcm;
}

static void pcmTask(PcmHn pcm[], StmHn stm[])
{
	int		i;

	/* ストリームシステムサーバの実行 */
	smpStmTask(stm);

	/* 再生タスク */
	SMPPCMD_PCM_Task(pcm[0]);

	for (i = 1; i < FILE_NUM; i++) {
		/* 再生タスク */
		PCM_Task(pcm[i]);
	}
}


/********************************************************************/
/* パッド制御														*/
/********************************************************************/

/* パッド制御：再生ライブラリ */
void smp_PadCtrlPcm(PcmHn pcm[], StmHn stm[], Uint16 PadData1EW, Uint16 PadData1W)
{
	int		i;

	/* ポーズ */
	if (PadData1EW & PER_DGT_S) {
		PadData1EW = 0;
		if (g_pause_at_once == FALSE) {
			/* 開始 */
			g_pause_at_once = TRUE;
			for (i = 0; i < FILE_NUM; i++) {
				PCM_SetPauseWork(pwork_addr, PWORK_SIZE);
				PCM_Pause(pcm[i], PCM_PAUSE_ON_AT_ONCE);
			}
		} else {
			/* 解除 */
			g_pause_at_once = FALSE;
			for (i = 0; i < FILE_NUM; i++) {
				PCM_Pause(pcm[i], PCM_PAUSE_OFF);
			}
		}
	}

	/* ストップ */
	if (PCM_GetPlayStatus(pcm[FILE_NUM - 2]) == PCM_STAT_PLAY_END) {
		if (PadData1EW & PER_DGT_A) {
			PadData1EW = 0;
			PCM_Stop(pcm[FILE_NUM - 1]);
		}
	}

	/* チェンジ */
	if ((g_change_flag == FALSE) && (PadData1EW & PER_DGT_A)) {
		PadData1EW = 0;
		if (goNextStm(stm[1])) {
			g_change_flag = TRUE;
		}
	}
	/* ハンドルの強制切り替え */
	if (g_change_flag == TRUE) {
		if (PCM_CheckChange() == PCM_CHANGE_OK_AT_ONCE) {
			PCM_Change();
			g_change_flag = FALSE;
		}
	}

	/* ボリューム・アップ */
	if (PadData1EW & PER_DGT_U) {
		PadData1EW = 0;
		g_level = MIN(g_level + 1, LEVEL_MAX);
		for (i = 0; i < FILE_NUM; i++) {
			PCM_SetVolume(pcm[i], g_level);
			PCM_ChangePcmPara(pcm[i]);
		}
	}
	/* ボリューム・ダウン */
	if (PadData1EW & PER_DGT_D) {
		PadData1EW = 0;
		g_level = MAX(g_level - 1, LEVEL_MIN);
		for (i = 0; i < FILE_NUM; i++) {
			PCM_SetVolume(pcm[i], g_level);
			PCM_ChangePcmPara(pcm[i]);
		}
	}

	/* パン・右より（右最大、左を徐々に弱く） */
	if (PadData1EW & PER_DGT_R) {
		PadData1EW = 0;
		g_pan = MIN(g_pan + 1, PAN_MAX);
		for (i = 0; i < FILE_NUM; i++) {
			PCM_SetPan(pcm[i], g_pan_tbl[g_pan]);
			PCM_ChangePcmPara(pcm[i]);
		}
	}
	/* パン・左より（左最大、右を徐々に弱く） */
	if (PadData1EW & PER_DGT_L) {
		PadData1EW = 0;
		g_pan = MAX(g_pan - 1, PAN_MIN);
		for (i = 0; i < FILE_NUM; i++) {
			PCM_SetPan(pcm[i], g_pan_tbl[g_pan]);
			PCM_ChangePcmPara(pcm[i]);
		}
	}
}

#ifdef SMPPCM_DEBUG
	#define SMP_PAD_MODE_NUM		(4)
#else
	#define SMP_PAD_MODE_NUM		(1)
#endif
Sint32 smp_pad_mode = 0;

/* パッド制御 */
void smp_PadCtrl(PcmHn pcm[], StmHn stm[])
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
		smp_PadCtrlPcm(pcm, stm, PadData1EW, PadData1W);
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

void main(void)
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
	PcmHn 			pcm[FILE_NUM];
	StmHn			stm[FILE_NUM];
	PcmInfo 		info;
	Uint32			restart;
	int				i, j;
#else
	PcmHn	pcm[FILE_NUM];
	StmHn	stm[FILE_NUM];
	Uint32	restart;
	int		i;
#endif

	/* 変数の初期化 */
/*	g_spr_end = FALSE; */
	g_spr_end = TRUE;
	g_change_flag = FALSE;
	g_pause_at_once = FALSE;

	/* ＰＣＭ・ＡＤＰＣＭ再生ライブラリの初期化 */
	smpPcmInit();

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

	/* ストリームシステムの初期化 */
	stmInit();

	restart = 1;

	for (;;) {
		if (restart) {

			/* ストリームオープン */
			for (i = 0; i < FILE_NUM; i++) {
				if ((stm[i] = stmOpen(filename[i])) == NULL) {
					return;
				}
				/* 取り出し領域のリセット
				 *	ストリームシステムの仕様が...
				 *	最初の転送を STM_ExecServer で行うなら、これは不要。
				*/
				STM_ResetTrBuf(stm[i]);

				/* ハンドルの作成 */
				if ((pcm[i] = createHandle(stm[i], i)) == NULL) {
					return;
				}
#ifdef CD_ROM_XA_AUDIO
				/* 再生情報の設定(CD-ROM XA Audio sector の場合) */
				PCM_INFO_FILE_TYPE(&info) = PCM_FILE_TYPE_NO_HEADER;
				PCM_INFO_DATA_TYPE(&info) = PCM_DATA_TYPE_ADPCM_SCT;
				PCM_SetInfo(pcm[i], &info);
#endif
				PCM_SetLoadNum(pcm[i], SMP_LOAD_NUM);
				PCM_SetTrModeCd(pcm[i], SMP_TR_MODE_CD);
				PCM_Set1TaskSample(pcm[i], SMP_1TASK_SAMPLE);
			}

			/* 開始 */
			PCM_Start(pcm[0]);

			/* 次に再生するハンドルの登録 */
			PCM_EntryNext(pcm[1]);

			/* 最初に STM_ExecServer をコールさせる */
			call_stm_exec_server = -1;

			restart = 0;

			VTV_PRINTF((VTV_s, "S:Restart %X\n", pcm[0]));
		}

		/* 再生タスク */
		pcmTask(pcm, stm);

		/* パッド制御 */
		smp_PadCtrl(pcm, stm);

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
		if (PCM_GetPlayStatus(pcm[FILE_NUM-1]) == PCM_STAT_PLAY_END) {

			for (i = 0; i < FILE_NUM; i++) {
				/* ハンドルの消去 */
				PCM_DestroyStmHandle(pcm[i]);

				/* ストリームのクローズ*/
				stmClose(stm[i]);
			}

			SMPPCMD_MON_Reset();

			restart = 1;
		}
	}
}
