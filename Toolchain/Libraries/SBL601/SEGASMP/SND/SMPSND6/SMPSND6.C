/*-----------------------------------------------------------------------------
 *  FILE: smpsnd6.c
 *
 *  Copyright(c) 1994,1995 SEGA
 *
 *  PURPOSE:
 *
 *      サウンドサンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      PCMの連続再生を行います。(割り込み使用)
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *      SCU2.1以降で動作します。
 *
 *  AUTHOR(S)
 *
 *      1994-05-19  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-10-15  N.T Ver.1.03
 *      1995-05-01  Y.K Ver.1.10
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"
#include <machine.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_snd.h"
#include "sega_sys.h"
#include "sega_int.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */
#if	0
/*
**1995-07-25 TomonobuTakahashi
**
*/
static Uint16 *move_pcm_adr;                    /* PCM転送アドレス           */
#endif
static Uint16 * volatile move_pcm_adr;          /* PCM転送アドレス           */
static Uint8 move_bnk;                          /* PCM転送先バンク           */
#if	0
/*
**■1995-07-28	高橋智延
**	使ってないので削除
*/
static Uint32 cnt = 0;
#endif

/* 割込み優先レベル変更テーブル */
/****************************************************************************/
/* (注意)　以下の割り込み優先レベルテーブルは,サウンドライブラリで使用してい*/
/*         DMAを割り込み処理内で実行できるように簡易的に作成したものです.よ */
/*         ってゲームでは,そのゲームにあったテーブルを検討し作成してください*/
/*         作成方法はシステムライブラリのディスク内ドキュメントに記述してあ */
/*         ります.                                                          */
/****************************************************************************/
const Uint32 pri_tbl[32] = {
                                0x0000E17C,
                                0x0000FF7D,
                                0x0000E178,
                                0x0000E170,
                                0x0000E160,
                                0x0000E140,
                                0x0000C000,
                                0x0000FF7F,
                                0x0000C000,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000};

/******************************************************************************
 *
 * NAME:    main()      - メイン
 *
 * PARAMETERS :
 *      なし
 *
 * DESCRIPTION:
 *      メイン
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
void PcmIntr(void);
void m_PcmIntr(void);
void main(void)
{

#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
    SndIniDt snd_init;                                  /* システム起動データ*/
    Uint32 i;
    SndPcmStartPrm pcm_start;
    SndPcmChgPrm pcm_chg;
    SndPcmPlayAdr pcm_adr;
    Uint8 before_pcm_adr = 0;
#else
    SndIniDt snd_init;                                  /* システム起動データ*/
    SndPcmStartPrm pcm_start;
    SndPcmChgPrm pcm_chg;
#endif

    /** BEGIN ****************************************************************/
    /*
     *  process 1   （各PAD情報初期化）
     */

    SYS_CHGUIPR(pri_tbl);                   /* 割込み優先レベルの変更        */

	#if	0
	/*
	**1995-07-25 TomonobuTakahashi
	**
	*/
    move_pcm_adr = (Uint16 *)0x6010000;     /* 右チャネル転送元              */
    #endif
    move_pcm_adr = (Uint16 *)0x6020000;     /* 右チャネル転送元              */

	set_imask(0);
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_SND);      /* サウンド割り込み許可      */
    INT_SetScuFunc(INT_SCU_SND, PcmIntr);          /* 割り込み処理登録     */       INT_ChgMsk(INT_MSK_SND,INT_MSK_NULL);       /* サウンド割り込み許可      */
	
	/*
	** 1995-11-07 K.Kawai
	** for sddrvs ver 2.00
	*/
    SND_INI_PRG_ADR(snd_init) = (Uint16 *)0x60f0000;
    SND_INI_PRG_SZ(snd_init) = (Uint16 )0x6390;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)0x60ec000;
    SND_INI_ARA_SZ(snd_init) = (Uint16)0x0022;
    
    SND_Init(&snd_init);						/* サウンドシステム起動		 */
    SND_ChgMap(0);	/* サウンドエリアマップ変更	 */

    SND_RESET_INT();                            /* 割込みステータスリセット  */
    SND_SET_ENA_INT(SND_INT_PCM_ADR);           /* PCM割込み許可(サウンド)   */

    move_bnk = 0;

    m_PcmIntr();                       /* first bank  */
    m_PcmIntr();                       /* second bank */
  
    SND_PRM_MODE(pcm_start) = SND_MD_MONO | SND_MD_8;
    SND_PRM_SADR(pcm_start) = 0x1000;
	SND_PRM_SIZE(pcm_start) = 0x2000;
	SND_PRM_NUM(pcm_chg) = 2;
    SND_PRM_LEV(pcm_chg) = 7;
    SND_PRM_PAN(pcm_chg) = 0;
    SND_PRM_PICH(pcm_chg) = 0;
    SND_L_EFCT_IN(pcm_chg) = 0;
    SND_L_EFCT_LEV(pcm_chg) = 7;
    SND_R_EFCT_IN(pcm_chg) = 0;
    SND_R_EFCT_LEV(pcm_chg) =  7;
    #if	0
    /*
    **1995-07-25 TomonobuTakahashi
    **
    */
	SND_PRM_TL(pcm_chg) = 0;
	#endif

    SND_StartPcm(&pcm_start, &pcm_chg);	    /* PCM開始			 */
	while(1){
		#if	0
		/*
		**1995-07-25 TomonobuTakahashi
		**
		*/
        if( move_pcm_adr > (Uint16 *)0x605cc58){
        #endif
        if( move_pcm_adr > (Uint16 * volatile )0x606cc58){
            SND_StopPcm(2);
        }
    }
}
/*****************************************************************************/
/* サウンド割り込み処理 */
/*****************************************************************************/
void PcmIntr(void)
{
    SND_RESET_INT();                            /* 割込みステータスリセット  */
    if(SND_GET_FCT_INT() == SND_FCT_PCM_ADR){   /* PCM adress更新割込みか？  */
        if(SND_GET_INT_STAT() == 0x4){
            m_PcmIntr();
 
         }
    }
}
/*****************************************************************************/
/* PCM DATA 転送 */
/*****************************************************************************/
void m_PcmIntr(void)
{
		SND_MoveData(move_pcm_adr,			/* PCMデータ転送(src adrs	*/
				(Uint32)0x1000,				/* size						*/
				SND_KD_TONE,				/* BANK						*/
				move_bnk);					/* bank 0 or 1)				*/
#if	0
/*
**■1995-07-28	高橋智延
**	コメントのネストはできない
*/
/*		move_bnk = (move_bnk+1) & 3;	*/	/*							*/
#endif
		move_bnk = ~move_bnk & 1;			/* next bank				*/
		move_pcm_adr += 0x800;				/* arc adrs ptr add			*/
}
