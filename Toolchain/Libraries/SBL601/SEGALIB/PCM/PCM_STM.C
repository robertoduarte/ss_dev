/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:ストリームアクセス
 * File		:pcm_stm.c
 * Date		:1994-10-04
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
/*-------------------- 《インクルードファイル》 --------------------*/
#include <machine.h>
#include <sega_xpt.h>
#include <sega_dma.h>
#include "pcm_stm.h"
#include "pcm_mem.h"
#include "pcm_lib.h"

#if	1
	/*
	**■1995-07-27	高橋智延
	**	ほかのファイルの関数を参照しているので追加
	*/
	#include	"pcm_msub.h"
#endif
#if	1
	/*
	**■1995-07-27	高橋智延
	**	CSH_Purge() 使っているので追加
	*/
	#include	"sega_csh.h"
#endif

/* #define	PCM_DEBUG 1 */

#ifdef PCM_DEBUG
#include "play.h"
#endif

/*------------------------- 《マクロ定数》 -------------------------*/
/* １回のタスクで転送する最大セクタ数 */
#define		LOAD_SECT_NUM		20

/* １セクタのバイト数 */
#define		SECT_SIZE_2048	2048
#define		SECT_SIZE_2324	2324

/*------------------------- 《処理マクロ》 -------------------------*/

#ifdef PCM_DEBUG
#define		DEBUG_PRINT_CDBUF(num) \
	{ \
	char buf[128];\
	sprintf(buf, "cdbuf %03d", num);\
	PLDSP_String(buf, 100, 160);\
	}
#else

#define		DEBUG_PRINT_CDBUF(num)
#define 	PL_DEBUG_PRINTNUM(a, b, c)
#define		PL_DEBUG_PRINT(a, b, c)
#define		PL_DEBUG_START_TIME()
#define		PL_DEBUG_PRINT_TIME(str)
#endif

/* セクタ数(及びセクタサイズ)から、Byte数に換算する */
#define PCMSTM_SCT2B(pcm, nsct)	\
			(PCMSTM_SECT_BSIZE(pcm) * (nsct))

/* セクタ数(及びセクタサイズ)から、Dword数に換算する */
#define PCMSTM_SCT2D(pcm, nsct)	\
			((PCMSTM_SECT_BSIZE(pcm) >> 2) * (nsct))

/* ＳＴＭのローカルデータ */
#define	PCMSTM_FILE_PARA(pcm) \
			(PCMLIB_FILEPARA(pcm).data.stm)

/* ストリームハンドル */
#define	PCMSTM_HANDLE(pcm) \
			(PCMSTM_FILE_PARA(pcm).stm)

/* 転送セクタ数 */
#define	PCMSTM_LOAD_SECT(pcm) \
			(PCMSTM_FILE_PARA(pcm).load_sect)

/* 前回のＣＤバッファの数 */
#define PCMSTM_OLD_CD_BUFNUM(pcm) \
			(PCMSTM_FILE_PARA(pcm).old_cd_buf_num)

/* ＤＭＡ転送しているなら TRUE */
#define PCMSTM_DMA_STATE(pcm) \
			(PCMSTM_FILE_PARA(pcm).dma_state)

/* ＤＭＡ転送するセクタ数 */
#define PCMSTM_DMA_SECT(pcm) \
			(PCMSTM_FILE_PARA(pcm).dma_sect)

/* 書き込むアドレス */
#define	PCMSTM_WRITE_ADDR(pcm) \
			(PCMSTM_FILE_PARA(pcm).write_addr)

/* 空きバッファのバイト数 */
#define	PCMSTM_BUF_BSIZE(pcm) \
			(PCMSTM_FILE_PARA(pcm).buf_bsize)

/* 転送したバイト数 */
#define	PCMSTM_WRITE_BSIZE(pcm) \
			(PCMSTM_FILE_PARA(pcm).write_bsize)

/* １セクタのバイト数 */
#define	PCMSTM_SECT_BSIZE(pcm) \
			(PCMSTM_FILE_PARA(pcm).sect_bsize)

/* 先頭オーディオセクタ */
#define	PCMSTM_AUDIO_1ST_SECT(pcm) \
			(PCMSTM_FILE_PARA(pcm).audio_1st_sect)

/* 転送関数 */
#define	PCMSTM_LOAD_FUNC(pcm) \
			(PCMSTM_FILE_PARA(pcm).load_func)

/*-------------------------- 《データ型》 --------------------------*/
typedef Sint32 (*LoadFunc)(void *obj, StmHn stm, Sint32 nsct);

/*-------------------------- 《関数宣言》 --------------------------*/
static void stmInit(void);
static void dmaCpuMemCopy4(void *dst, void *src, Uint32 cnt);
static Uint32 dmaCpuResult(void);
static void dmaScuMemCopy(void *dst, void *src, Uint32 cnt);
static Uint32 dmaScuResult(void);
STATIC void pcmstm_loadBuf(PcmHn pcm);
STATIC Sint32 pcmstm_loadCpu(void *obj, StmHn stm, Sint32 nsct);
STATIC Sint32 pcmstm_loadDmaCpu(void *obj, StmHn stm, Sint32 nsct);
STATIC Sint32 pcmstm_loadDmaScu(void *obj, StmHn stm2, Sint32 nsct);
STATIC Sint32 pcmstm_loadDummy(void *obj, StmHn stm, Sint32 nsct);
STATIC void pcmstm_waitDma(PcmHn pcm);

/*-------------------------- 《変数定義》 --------------------------*/
/* ＤＭＡ転送を開始したかを表すフラグ 開始したら TRUE */
STATIC Bool pcmstm_dma_cpu_start;
STATIC Bool pcmstm_dma_scu_start;

/* STM_ExecServerを実行している最中の PcmHn が入る */
STATIC PcmHn pcmstm_exec_pcm;

/*-------------------------- 《関数定義》 --------------------------*/
static void stmInit(void)
{
	/* 変数の初期化 */
	pcmstm_dma_cpu_start = OFF;
	pcmstm_dma_scu_start = OFF;
	pcmstm_exec_pcm = NULL;
}

/********************************************************************/
/* ハンドルの作成（ストリームシステム）								*/
/* [入力]															*/
/*    para : 作成パラメータ											*/
/*    stm  : ストリームハンドル										*/
/* [関数値]															*/
/*    ハンドル（作成できない場合は NULL)							*/
/********************************************************************/
PcmHn PCM_CreateStmHandle(PcmCreatePara *para, StmHn stm)
{
	PcmHn   	pcm;

	if (PCM_lib_stm_init == OFF) {
		stmInit();
		PCM_lib_stm_init = ON;
	}

	if (stm == NULL) {
		PCM_MeSetErrCode(PCM_ERR_ILLEGAL_PARA);
		return NULL;
	}

	/* 作成パラメータのチェック */
	PCMLIB_CHK_CREATE_PARA(para);

	pcm =  PCM_StmCreate(stm, 
						 (PcmWork *)PCM_PARA_WORK(para), 
						 PCM_PARA_RING_ADDR(para), PCM_PARA_RING_SIZE(para),
						 PCM_PARA_PCM_ADDR(para), PCM_PARA_PCM_SIZE(para));
	if (pcm != NULL) {
		PCMLIB_FACCESS_TYPE(pcm) = PCMLIB_FACCESS_STM;

		/* 実行関数の設定 */
		PCMLIB_SET_START_FUNC(pcm, PCM_StmStart);
		PCMLIB_SET_TASK_FUNC(pcm, PCM_StmTask);
		PCMLIB_SET_PRELOAD_FILE_FUNC(pcm, PCM_StmPreloadFile);
		PCMLIB_SET_SET_LOAD_NUM_FUNC(pcm, PCM_StmSetLoadNum);
		PCMLIB_SET_SET_TRMODE_FUNC(pcm, PCM_StmSetTrMode);

		/* ローカルデータを初期化 */
		PCMSTM_OLD_CD_BUFNUM(pcm) = -1;
		PCMSTM_DMA_STATE(pcm) = OFF;
		PCMSTM_DMA_SECT(pcm) = 0;
		PCMSTM_WRITE_ADDR(pcm) = NULL;
		PCMSTM_BUF_BSIZE(pcm) = 0;
		PCMSTM_WRITE_BSIZE(pcm) = 0;
		PCMSTM_SECT_BSIZE(pcm) = SECT_SIZE_2048;
		PCMSTM_AUDIO_1ST_SECT(pcm) = 0;
		PCMSTM_LOAD_FUNC(pcm) = pcmstm_loadDmaCpu;

		/* ストリームハンドルの設定 */
		PCMSTM_HANDLE(pcm) = stm;

		/* 転送関数の設定 */
		STM_SetTrFunc(PCMSTM_HANDLE(pcm), PCMSTM_LOAD_FUNC(pcm), pcm);

		/* 最大転送セクタ数の設定 */
		PCMSTM_LOAD_SECT(pcm) = LOAD_SECT_NUM;
		STM_SetTrPara(PCMSTM_HANDLE(pcm), PCMSTM_LOAD_SECT(pcm));

	}
	return pcm;
}

/********************************************************************/
/* ハンドルの消去（ストリームシステム）								*/
/*   ハンドルを破棄した後は、ハンドルを引き数にもつ関数は利用でき	*/
/*   ない															*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/********************************************************************/
void PCM_DestroyStmHandle(PcmHn pcm)
{
	/* ハンドルチェック */
	PCMLIB_CHK_HANDLE(pcm);

	if (PCMLIB_FACCESS_TYPE(pcm) != PCMLIB_FACCESS_STM) {
		PCM_MeSetErrCode(PCM_ERR_ILL_CREATE_MODE);
	}

	/* ＤＭＡ転送の終了を待つ */
	pcmstm_waitDma(pcm);

	/* 転送関数の解除 */
	STM_SetTrFunc(PCMSTM_HANDLE(pcm), pcmstm_loadDummy, NULL);

	PCM_MeDestroy(pcm);
}



/********************************************************************/
/* ハンドルを生成する												*/
/*   ストリームハンドルは、ストリームシステムにより予め取得すること	*/
/*																	*/
/* [引き数]															*/
/*    stm : ストリームハンドル										*/
/*    work : ワークアドレス											*/
/*    buf : バッファアドレス										*/
/*    bufsize : バッファのバイト数									*/
/*    pcmbuf:ＰＣＭのウェーブＲＡＭの先頭アドレス					*/
/*    pcmsize:ＰＣＭのウェーブＲＡＭのバイト数						*/
/* [関数値]															*/
/*    ハンドル														*/
/*    NULL の場合はエラー											*/
/********************************************************************/
PcmHn PCM_StmCreate(StmHn stm, PcmWork *work , 
							   void *buf, Sint32 bufsize, 
							   void *pcmbuf, Sint32 pcmsize)
{
	PcmPara		para;

	/* リングバッファ */
	para.ring_addr = buf;
	para.ring_size = bufsize;

	/* トリガサイズ */
	para.start_trg_size 	= PCM_DEFAULT_SIZE_START_TRG;
	para.start_trg_sample 	= PCM_DEFAULT_SAMPLE_START_TRG;
	para.stop_trg_sample 	= PCM_DEFAULT_SAMPLE_STOP_TRG;

	/* サウンドドライバコマンドブロック番号 */
	para.command_blk_no = PCMLIB_COMMAND_BLK_NO;

	/* ＰＣＭストリーム再生番号 */
	para.pcm_stream_no = PCMLIB_PCM_STREAM_NO;

	/* ＰＣＭバッファ */
	para.pcm_addr = pcmbuf;
	para.pcm_size = pcmsize;

	/* ＰＡＮとボリューム */
	para.pcm_pan = PCMLIB_PCM_PAN;
	para.pcm_level = PCMLIB_PCM_LEVEL;

	/* ハンドルの作成 */
	return PCM_MeCreate(work, &para);
}


/********************************************************************/
/* ＣＤバッファからの最大転送セクタ数を設定する						*/
/* [入力]															*/
/*    pcm  : ハンドル												*/
/*    load_sct : 最大転送セクタ数									*/
/********************************************************************/
void PCM_StmSetLoadNum(PcmHn pcm, Sint32 load_sct)
{
	PCMSTM_LOAD_SECT(pcm) = load_sct;

	/* ＣＤバッファからの最大転送セクタ数を設定する */
	STM_SetTrPara(PCMSTM_HANDLE(pcm), PCMSTM_LOAD_SECT(pcm));
}

/********************************************************************/
/* 再生を先頭から開始する											*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
void PCM_StmStart(PcmHn pcm)
{
	/* 再生開始 */
	PCM_MeStart(pcm);
}

/* コーディング情報(ci)の取得と設定 */
void PCM_GetSetCi(PcmHn pcm)
{
	StmSct 			sinfo;

	if (PCM_MeIsNotSetCi(pcm)) {	/* コーディング情報(ci)の取得が必要 */
		for (;;) {
			/* セクタ情報を得る */
			if (STM_GetSctInfo(PCMSTM_HANDLE(pcm), PCMSTM_AUDIO_1ST_SECT(pcm), 
								&sinfo) == TRUE) {
				if (STM_SCT_SM(&sinfo) & 0x40) {
					/* コーディング情報(ci)から得た情報を設定する */
					PCM_MeSetCi(pcm, STM_SCT_CI(&sinfo));
				} else {
					PCMSTM_AUDIO_1ST_SECT(pcm)++;
					continue;
				}
			}
			break;
		}
	}
}

/********************************************************************/
/* 再生タスク														*/
/*   再生中は定期的にこの関数を呼ぶ									*/
/*   呼ぶ間隔はＰＣＭバッファの半分の再生時間以下で定期的に呼ぶ		*/
/*   必要頻度より少ないと再生が乱れる								*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/*     NULL が指定された時は現在オープンされているハンドル   		*/
/*     をすべての再生中処理をする									*/
/********************************************************************/
void PCM_StmTask(PcmHn pcm)
{
	if (PCM_IsDeath(pcm)) {
		return;
	}

#if 0
	/* コーディング情報(ci)の取得と設定 */
	PCM_GetSetCi(pcm);

	/* ＣＤバッファからリングバッファへデータを転送する */
	pcmstm_loadBuf(pcm);
#endif

	/* タスク処理 */
	PCM_MeTask(pcm);

}

/********************************************************************/
/* ＣＤバッファからのリングバッファにデータを転送する				*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
STATIC void pcmstm_loadBuf(PcmHn pcm)
{
	Uint32 write_addr32;
	Sint32 write_size;
	Sint32 total_size;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	PcmPara	*para;
#endif
	Sint32	cd_buf_num;


	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);
	PCMSTM_WRITE_ADDR(pcm) = (Uint32 *)write_addr32;
	PCMSTM_BUF_BSIZE(pcm) = write_size;
	PCMSTM_WRITE_BSIZE(pcm) = 0;

	if (write_size >= PCMSTM_SECT_BSIZE(pcm)) {

		/* ＣＤからの読み込んだデータをリングバッファに転送する */
		cd_buf_num = STM_GetNumCdbuf(PCMSTM_HANDLE(pcm));
		DEBUG_PRINT_CDBUF(cd_buf_num);

		/* ＣＤバッファにたまっているセクタ数が転送セクタ数になったら */
		/* 実際の転送処理をする */
		/* ただし、最初と最後はたまっていなくても転送する */
#if 0
		if (PCMSTM_OLD_CD_BUFNUM(pcm) == -1 || 
							PCMSTM_OLD_CD_BUFNUM(pcm) == cd_buf_num ||
							cd_buf_num >= PCMSTM_LOAD_SECT(pcm)) {}
#else
		if (cd_buf_num == 0 || 
			cd_buf_num >= PCMSTM_LOAD_SECT(pcm)) {
#endif
			pcmstm_exec_pcm = pcm;
			STM_ExecServer();
			pcmstm_exec_pcm = NULL;
		}
		PCMSTM_OLD_CD_BUFNUM(pcm) = cd_buf_num;
	}

}

/********************************************************************/
/* ＤＭＡ転送の終了を待ち											*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/********************************************************************/
STATIC void pcmstm_waitDma(PcmHn pcm)
{
	if (PCMSTM_DMA_STATE(pcm) == ON) {
		if (PCMSTM_LOAD_FUNC(pcm) == pcmstm_loadDmaCpu) {
			while (dmaCpuResult() == DMA_CPU_BUSY) ;
			pcmstm_dma_cpu_start = OFF;
		} else if (PCMSTM_LOAD_FUNC(pcm) == pcmstm_loadDmaScu) {
			while (dmaScuResult() == DMA_CPU_BUSY) ;
			pcmstm_dma_scu_start = OFF;
		}
		PCMSTM_DMA_STATE(pcm) = OFF;
	}
}

/**************************************************************/
/*  ダミー転送 												  */
/* 何も転送せず０を返す										  */
/**************************************************************/
STATIC Sint32 pcmstm_loadDummy(void *obj, StmHn stm, Sint32 nsct)
{
	return 0;
}


/**************************************************************/
/*  ＣＰＵのＤＭＡの転送関数 								  */
/**************************************************************/
STATIC Sint32 pcmstm_loadDmaCpu(void *obj, StmHn stm2, Sint32 nsct)
{
	PcmHn 		pcm;
	Uint32 		*src;
	Sint32 		adlt;
	Sint32		trans_nsct;
	Sint32 		write_addr32, write_size, total_size;

	pcm = (PcmHn)obj;

	_VTV_PRINTF((VTV_s, "P:_loadDmaCpu hn%d\n stm%X sct%d\n", 
		PCM_MeGetHandleNo(pcm), stm2, nsct));

	if (PCM_IsDeath(pcm)) {
		return 0;
	}

	if (PCMSTM_DMA_STATE(pcm) == ON) {
		if (dmaCpuResult() == DMA_CPU_BUSY) {
			return -1;/* まだ、転送中 */
		} else {
			PCMSTM_DMA_STATE(pcm) = OFF;
			PCM_MeRenewRingWrite(pcm, PCMSTM_SCT2B(pcm, PCMSTM_DMA_SECT(pcm)));
			return PCMSTM_DMA_SECT(pcm);
		}
	}

	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);

	/* 転送するセクタ数を計算する */
	trans_nsct = write_size / PCMSTM_SECT_BSIZE(pcm);
	trans_nsct = MIN(trans_nsct, nsct);

	if (trans_nsct <= 0) {
		return 0;
	}

	/* コーディング情報(ci)の取得と設定 */
	PCM_GetSetCi(pcm);

	/* 転送 */
	src = STM_StartTrans(stm2, &adlt);
	dmaCpuMemCopy4((Uint32 *)write_addr32, src, PCMSTM_SCT2D(pcm, trans_nsct));
	PCMSTM_DMA_SECT(pcm) = trans_nsct;
	PCMSTM_DMA_STATE(pcm) = ON;

	return -1;/* 転送中 */
}


static void *dma_cpu_dis_adr;                   /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ */
static Uint32 dma_cpu_cnt;                      /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｶｳﾝﾄ  */


static void dmaCpuMemCopy4(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* 共通転送パラメータ        */
    DmaCpuPrm prm;                              /* 転送パラメータ            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH1);                   /* DMA転送中止               */

    com_prm.pr = DMA_CPU_FIX;                   /* 優先順位設定(ﾗｳﾝﾄﾞﾛﾋﾞﾝ)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAﾏｽﾀｲﾈｰﾌﾞﾙ設定(許可)    */
    com_prm.msk = DMA_CPU_M_PR |                /* マスク設定(ﾌﾟﾗｲｵﾘﾃｨﾓｰﾄﾞ)  */
                  DMA_CPU_M_AE |                /* (アドレスエラーフラグ)    */
                  DMA_CPU_M_NMIF |              /* (NMIフラグ)               */
                  DMA_CPU_M_DME;                /* (DMAマスタイネーブル)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA共通転送パラメータ設定 */

    prm.sar = (Uint32)src;                      /* ソースアドレス設定        */
    prm.dar = (Uint32)dst;                      /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ設定      */
    prm.tcr = cnt;                              /* トランスファカウント設定  */
    prm.dm = DMA_CPU_AM_ADD;                    /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾓｰﾄﾞ設定  */
    prm.sm = DMA_CPU_AM_NOM;                    /* ｿｰｽｱﾄﾞﾚｽﾓｰﾄﾞ設定  moke    */
    prm.ts = DMA_CPU_4;                         /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
    prm.ar = DMA_CPU_AUTO;                      /* ｵｰﾄﾘｸｴｽﾄﾓｰﾄﾞ設定          */
    prm.ie = DMA_CPU_INT_DIS;                   /* ｲﾝﾀﾗﾌﾟﾄｲﾈｰﾌﾞﾙ設定         */
    
    prm.msk = DMA_CPU_M_SAR |                   /* マスク設定                */
              DMA_CPU_M_DAR |
              DMA_CPU_M_TCR |
              DMA_CPU_M_DM  |
              DMA_CPU_M_SM  |
              DMA_CPU_M_TS  |
              DMA_CPU_M_AR  |
              DMA_CPU_M_IE  |
              DMA_CPU_M_TE;                     /* ﾄﾗﾝｽﾌｧｴﾝﾄﾞﾋﾞｯﾄのクリア指定*/

    DMA_CpuSetPrm(&prm, DMA_CPU_CH1);           /* DMA転送パラメータ設定     */
    
    DMA_CpuStart(DMA_CPU_CH1);                  /* DMA転送開始               */
    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt * 4;
}


static Uint32 dmaCpuResult(void)
{
    DmaCpuComStatus com_status;             /* 共通ステータス                */
    DmaCpuStatus status;                    /* ステータス                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* 共通ステータス取得            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ｱﾄﾞﾚｽｴﾗｰが発生した場合        */
        return(DMA_CPU_FAIL);               /* 異常終了をリターン            */
    }
    status = DMA_CpuGetStatus(DMA_CPU_CH1); /* ステータス取得                */
    if(status == DMA_CPU_TE_MV){            /* 動作中である場合              */
        return(DMA_CPU_BUSY);               /* 実行中をリターン              */
    }
    CSH_Purge(dma_cpu_dis_adr, dma_cpu_cnt);
    return(DMA_CPU_END);                    /* 正常終了をリターン            */
}


/**************************************************************/
/*  ＳＣＵのＤＭＡの転送関数 								  */
/**************************************************************/
STATIC Sint32 pcmstm_loadDmaScu(void *obj, StmHn stm2, Sint32 nsct)
{
	PcmHn		pcm;
	Uint32 		*src;
	Sint32 		adlt;
	Sint32		trans_nsct;
	Sint32 		write_addr32, write_size, total_size;

	pcm = (PcmHn)obj;

	_VTV_PRINTF((VTV_s, "P:_loadDmaScu hn%d\n stm%X sct%d\n", 
		PCM_MeGetHandleNo(pcm), stm2, nsct));

	if (PCM_IsDeath(pcm)) {
		return 0;
	}

	if (PCMSTM_DMA_STATE(pcm) == ON) {
		if (dmaScuResult() == DMA_SCU_BUSY) {
			return -1;/* まだ、転送中 */
		} else {
			PCMSTM_DMA_STATE(pcm) = OFF;
			PCM_MeRenewRingWrite(pcm, PCMSTM_SCT2B(pcm, PCMSTM_DMA_SECT(pcm)));
			return PCMSTM_DMA_SECT(pcm);
		}
	}

	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);

	/* 転送するセクタ数を計算する */
	trans_nsct = write_size / PCMSTM_SECT_BSIZE(pcm);
	trans_nsct = MIN(trans_nsct, nsct);

	if (trans_nsct <= 0) {
		return 0;
	}

	/* コーディング情報(ci)の取得と設定 */
	PCM_GetSetCi(pcm);

	/* 転送 */
	src = STM_StartTrans(stm2, &adlt);
	dmaScuMemCopy((Uint32 *)write_addr32, src, PCMSTM_SCT2B(pcm, trans_nsct));
	PCMSTM_DMA_SECT(pcm) = trans_nsct;
	PCMSTM_DMA_STATE(pcm) = ON;

	return -1;/* 転送中 */
}

static void *dma_scu_dis_adr;
static Uint32 dma_scu_cnt;
static Uint8 dma_start_flg;             /* DMAの開始有無フラグ */

static void dmaScuMemCopy(void *dst, void *src, Uint32 cnt)
{
    DmaScuPrm prm;
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    dma_scu_dis_adr = dst;
    dma_scu_cnt = (Uint32)cnt;

    prm.dxr = (Uint32)src;
    prm.dxw = (Uint32)dst;
    prm.dxc = cnt;
    prm.dxad_r = DMA_SCU_R0;
    prm.dxad_w = DMA_SCU_W4;

    prm.dxmod = DMA_SCU_DIR;
    prm.dxrup = DMA_SCU_KEEP;
    prm.dxwup = DMA_SCU_KEEP;
    prm.dxft = DMA_SCU_F_DMA;
    prm.msk = DMA_SCU_M_DXR    |
              DMA_SCU_M_DXW    ;

    DMA_ScuSetPrm(&prm, DMA_SCU_CH0);
    DMA_ScuStart(DMA_SCU_CH0);
    dma_start_flg = ON;                         /* DMAはスタートしている     */
    set_imask(msk);                                         /* 割り込みPOP   */
}


static Uint32 dmaScuResult(void)
{
    DmaScuStatus status;
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    if(dma_start_flg == ON){                    /* DMAを開始している時       */
        DMA_ScuGetStatus(&status, DMA_SCU_CH0);
            if(status.dxmv == DMA_SCU_MV){
                set_imask(msk);                             /* 割り込みPOP   */
                return(DMA_SCU_BUSY);
            }
            CSH_Purge(dma_scu_dis_adr, dma_scu_cnt);
            set_imask(msk);                                 /* 割り込みPOP   */
            return(DMA_SCU_END);
    }else{                                      /* DMAを開始していない時     */
        set_imask(msk);                                     /* 割り込みPOP   */
        return(DMA_SCU_END);
    }
}


/**************************************************************/
/*  ソフトウェア転送の転送関数								  */
/**************************************************************/
STATIC Sint32 pcmstm_loadCpu(void *obj, StmHn stm, Sint32 nsct)
{
	PcmHn		pcm;
	Uint32 		*src, *dst;
	Sint32 		adlt;
	long		i;
	Sint32		trans_nsct;
	Sint32		trans_size;
	Sint32 		write_addr32, write_size, total_size;

	pcm = (PcmHn)obj;

	_VTV_PRINTF((VTV_s, "P:_loadCpu hn%d\n stm%X sct%d\n", 
		PCM_MeGetHandleNo(pcm), stm2, nsct));

	if (PCM_IsDeath(pcm)) {
		return 0;
	}

	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);

	/* 転送するセクタ数を計算する */
	trans_nsct = write_size / PCMSTM_SECT_BSIZE(pcm);
	trans_nsct = MIN(trans_nsct, nsct);

	if (trans_nsct <= 0) {
		return 0;
	}

	/* コーディング情報(ci)の取得と設定 */
	PCM_GetSetCi(pcm);

	/* 転送 */
	src = STM_StartTrans(stm, &adlt);
	dst = (Uint32 *)write_addr32;
	i = trans_size = PCMSTM_SCT2D(pcm, trans_nsct);
	while (--i >= 0) {
		*dst++ = *src;
		src += adlt;
	}

	/* リングバッファへの書き込みサイズの報告 */
	PCM_MeRenewRingWrite(pcm, trans_size << 2);

	return trans_nsct;
}



/********************************************************************/
/* ＣＤからリングバッファにファイルを読み込む						*/
/*																	*/
/* [引き数]															*/
/*    pcm : ハンドル												*/
/*    size : 読み込むバイト数										*/
/*           リングバッファが読み込みバイト数より小さい時は			*/
/*           バッファサイズになる。									*/
/*           読み込むバイト数はセクタ単位（２０４８の倍数）になる。	*/
/* [関数値]															*/
/*    実際に読み込んだバイト数										*/
/********************************************************************/
Sint32 PCM_StmPreloadFile(PcmHn pcm, Sint32 size)
{
	Uint32		write_addr32;
	Sint32		write_size, total_size;
	PcmPara		*para;
	Sint32		load_sct, load_size;
#if	0
	/*
	**■1995-07-27	高橋智延
	**	使ってないので削除
	*/
	Sint32		ret;
#endif
	para = PCM_MeGetPara(pcm);

	load_size = 0;

	/* リングバッファの空きバイト数を得る */
	PCM_MeGetRingWrite(pcm, (Sint8 **)&write_addr32, &write_size, &total_size);
	if (write_size >= PCMSTM_SECT_BSIZE(pcm)) {
		/* ＣＤからの読み込んだデータをリングバッファに転送する */
		if (size > write_size) {
			load_size = write_size;
		} else {
			load_size = size;
		}
		load_sct = load_size / PCMSTM_SECT_BSIZE(pcm);
		load_size = load_sct * PCMSTM_SECT_BSIZE(pcm);
		if (load_sct > 0) {
			PCMSTM_WRITE_ADDR(pcm) = (Uint32 *)write_addr32;
			PCMSTM_BUF_BSIZE(pcm) = load_size;
			PCMSTM_WRITE_BSIZE(pcm) = 0;
			pcmstm_exec_pcm = pcm;
			while (STM_ExecServer() != STM_EXEC_COMPLETED) {
				if (PCMSTM_WRITE_BSIZE(pcm) >= PCMSTM_BUF_BSIZE(pcm) &&
					PCMSTM_DMA_STATE(pcm) == OFF) {
					break;
				}
			}
			pcmstm_exec_pcm = NULL;
			load_size = PCMSTM_WRITE_BSIZE(pcm);
		}
	}
	return load_size;
}


/********************************************************************/
/* 転送方式の設定													*/
/* [入力]															*/
/*    pcm   : ムービハンドル										*/
/*    mode  : 転送方式												*/
/*      PCM_TRMODE_CPU : ソフトウェア転送							*/
/*      PCM_TRMODE_SDMA : ＤＭＡサイクルスチール					*/
/*      PCM_TRMODE_SCU  : ＳＣＵのＤＭＡ							*/
/********************************************************************/
void PCM_StmSetTrMode(PcmHn pcm, PcmTrMode mode)
{

	switch(mode) {
	case PCM_TRMODE_CPU:
	default:
		PCMSTM_LOAD_FUNC(pcm) = pcmstm_loadCpu;
		break;
	case PCM_TRMODE_SDMA:
		PCMSTM_LOAD_FUNC(pcm) = pcmstm_loadDmaCpu;
		break;
	case PCM_TRMODE_SCU:
		PCMSTM_LOAD_FUNC(pcm) = pcmstm_loadDmaScu;
		break;
	}
	/* 転送関数の設定 */
	STM_SetTrFunc(PCMSTM_HANDLE(pcm), PCMSTM_LOAD_FUNC(pcm), pcm);
}
