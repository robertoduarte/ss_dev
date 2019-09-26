/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : 表示の制御モジュールソースファイル
 *  File   : mpg_win.c
 *  Date   : 1996-01-18
 *  Author : H.Take
 *
 *******************************************************************/

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        "sega_dma.h"
#include        "sega_csh.h"
#include        "mpg_loc.h"

/*******************************************************************
 *      デバッグ用定数マクロ
 *******************************************************************/

/*******************************************************************
 *      定数マクロ
 *******************************************************************/

/* 倍率の初期値（等倍） */
#define RAT_DFL         1000

/* 倍率の逆数を求めるための係数 */
#define RECIP_COEF      1000000L

/* MPEGウィンドウの最大個数 */
#define MPGWN_NUM       16

/* ウィンドウの拡大率パラメータのテーブル要素数 */
#define FR_NUM          1000

/* 倍率のテーブル要素数 */
#define RAT_NUM         16

/* デフォルト転送関数内での転送終了待ちタイムアウト時間 */
#define TRNFIN_CNT      1700000

/* 画面特殊効果パラメータID */
#define SET_WN_INTPOL   0x01            /* 補間モード */
#define SET_WN_MOZAIC   0x02            /* モザイク */
#define SET_WN_SOFT     0x04            /* ぼかし */
#define SET_WN_LUMIKEY  0x08            /* ルミネッセンスキー */

/*******************************************************************
 *      処理マクロ
 *******************************************************************/

/* フレームバッファウィンドウ位置パラメータを求める */
#define GET_FPOS(fz, dl, rat)   ((fz) + (dl) * (rat) / RAT_DFL)

/* 拡大率の整数部分を求める */
#define GET_INTEG(rat)          ((rat) / RAT_DFL)

/* 拡大率の小数部分を求める */
#define GET_FRAC(rat, p)        ((rat) - (p) * RAT_DFL)

/* 逆数を求める */
#define GET_RECIP(rat)          (RECIP_COEF / (rat))

/* ＹＣ倍率を求める */
#define GET_YCRAT(ycrat)        ((Sint8)(ycrat) - 8)

/*******************************************************************
 *      関数宣言
 *******************************************************************/

static void     initWinPara(MpgWn mpgwn);
static Sint32   ratToFr(Uint32 ratio);
static Sint32   trnsScuDmaBbus(void *dst, void *src, Sint32 nbyte);
static Sint32   setWn(MpgWn mpgwn);
static Sint32   doSetWnEffect(MpgWn mpgwn, Sint32 id);
static Sint32   setOfstSub(Sint32 dpx, Sint32 dpy,
                           Sint32 fpx, Sint32 fpy);
static Uint32   frToRat(Sint32 fr);
static Sint32   setRatioSub(Sint32 frx, Sint32 fry,
                            Sint32 fpx, Sint32 fpy);
static Sint32   doTrans(MpgWn mpgwn);

/*******************************************************************
 *      変数定義
 *******************************************************************/

/* MPEGウィンドウ */
static MpgWin   mp_win[MPGWN_NUM];

/* ウィンドウの拡大率パラメータのテーブル */
static const Uint8      fr_tbl[FR_NUM] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,     /* 40 */
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,     /* 80 */
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,     /* 120 */
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
    0x0f, 0x0f, 0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,     /* 160 */
    0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,     /* 200 */
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,     /* 240 */
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,     /* 280 */
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,     /* 320 */
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,     /* 360 */
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,     /* 400 */
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,     /* 440 */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,     /* 480 */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,     /* 520 */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,     /* 560 */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,     /* 600 */
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,     /* 640 */
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,     /* 680 */
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,     /* 720 */
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,     /* 760 */
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,     /* 800 */
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,     /* 840 */
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,     /* 880 */
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,     /* 920 */
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,     /* 960 */
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09      /* 1000 */
    };

/* 倍率のテーブル */
static const Uint16     rat_tbl[RAT_NUM] = {
    0xffff,    0,  500,  666,  750,  800,  833,  857,
    0xffff, 1000,  500,  333,  250,  200,  166,  142
    };

/*******************************************************************
 *      関数定義
 *******************************************************************/

/* MPEGウィンドウの生成 */
MpgWn   MPG_WnCreate(Uint32 dzx, Uint32 dzy, Uint32 dsx, Uint32 dsy)
{
    MpgWn       mpgwn;
    MpgWin      *mwptr;
    Sint32      err;
    Sint32      i;

    MPSYS_SemaIn(FALSE);

    /* 変数の初期化 */
    mpgwn = NULL;
    err = MPG_ERR_CREATE;

    /* 空いているMPEGウィンドウを探す */
    for (i = 0; i < MPGWN_NUM; i++) {
        mwptr = &mp_win[i];
        if (MPG_WN_USEFLG(mwptr) == FALSE) {
            mpgwn = mwptr;
            break;
        }
    }
    
    if (mpgwn != NULL) {
        /* MPEGウィンドウパラメータの初期化 */
        initWinPara(mpgwn);
        
        /* MPEGウィンドウの使用フラグをTRUEにする */
        MPG_WN_USEFLG(mpgwn) = TRUE;
        
        /* MPEGウィンドウパラメータの設定 */
        MPG_WN_DZ_X(mpgwn) = dzx;
        MPG_WN_DZ_Y(mpgwn) = dzy;
        MPG_WN_DS_X(mpgwn) = dsx;
        MPG_WN_DS_Y(mpgwn) = dsy;
        MPG_WN_DP_X(mpgwn) = (Sint32)dzx + MPG_WN_DL_X(mpgwn);
        MPG_WN_DP_Y(mpgwn) = (Sint32)dzy + MPG_WN_DL_Y(mpgwn);
        MPG_WN_FZ_X(mpgwn) = dzx;
        MPG_WN_FZ_Y(mpgwn) = dzy;
        MPG_WN_FP_X(mpgwn) = GET_FPOS(MPG_WN_FZ_X(mpgwn), MPG_WN_DL_X(mpgwn),
                                      MPG_WN_RAT_X(mpgwn));
        MPG_WN_FP_Y(mpgwn) = GET_FPOS(MPG_WN_FZ_Y(mpgwn), MPG_WN_DL_Y(mpgwn),
                                      MPG_WN_RAT_Y(mpgwn));

        /* 変数の設定 */
        err = MPG_ERR_OK;
    }
    
    MPSYS_SemaOut(err);

    return mpgwn;
}

/* MPEGウィンドウパラメータの初期化 */
static void     initWinPara(MpgWn mpgwn)
{
    /* 子MPEGウィンドウへのポインタ */
    MPG_WN_CHLD(mpgwn) = NULL;
    
    /* 親MPEGウィンドウへのポインタ */
    MPG_WN_PARE(mpgwn) = NULL;
    
    /* 使用フラグ */
    MPG_WN_USEFLG(mpgwn) = FALSE;
    
    /* ディスプレイウィンドウの表示基準位置 */
    MPG_WN_DZ_X(mpgwn) = 0;
    MPG_WN_DZ_Y(mpgwn) = 0;
    
    /* ディスプレイウィンドウの大きさ */
    MPG_WN_DS_X(mpgwn) = 0;
    MPG_WN_DS_Y(mpgwn) = 0;
    
    /* ディスプレイウィンドウのオフセット */
    MPG_WN_DL_X(mpgwn) = 0;
    MPG_WN_DL_Y(mpgwn) = 0;
    
    /* フレームバッファウィンドウの倍率 */
    MPG_WN_RAT_X(mpgwn) = RAT_DFL;
    MPG_WN_RAT_Y(mpgwn) = RAT_DFL;
    
    /* フレームバッファウィンドウの位置（ズームポイント） */
    MPG_WN_FZ_X(mpgwn) = 0;
    MPG_WN_FZ_Y(mpgwn) = 0;
    
    /* ディスプレイウィンドウ位置パラメータ */
    MPG_WN_DP_X(mpgwn) = 0;
    MPG_WN_DP_Y(mpgwn) = 0;
    
    /* フレームバッファウィンドウ位置パラメータ */
    MPG_WN_FP_X(mpgwn) = 0;
    MPG_WN_FP_Y(mpgwn) = 0;
    
    /* フレームバッファウィンドウ拡大率パラメータ */
    MPG_WN_FR_X(mpgwn) = ratToFr(RAT_DFL);
    MPG_WN_FR_Y(mpgwn) = ratToFr(RAT_DFL);
    
    /* ボーダカラー */
    MPG_WN_BCLR(mpgwn) = 0;
    
    /* ＹＣ倍率 */
    MPG_WN_YCRAT_YR(mpgwn) = 0;
    MPG_WN_YCRAT_CR(mpgwn) = 0;
    
    /* 補間モード */
    MPG_WN_INTPOL_YH(mpgwn) = 0;
    MPG_WN_INTPOL_YV(mpgwn) = 0;
    MPG_WN_INTPOL_CH(mpgwn) = 0;
    MPG_WN_INTPOL_CV(mpgwn) = 0;
    
    /* モザイク */
    MPG_WN_MOZ_H(mpgwn) = 0;
    MPG_WN_MOZ_V(mpgwn) = 0;
    
    /* ぼかしスイッチ */
    MPG_WN_SOFTSW_H(mpgwn) = 0;
    MPG_WN_SOFTSW_V(mpgwn) = 0;
    
    /* ルミネッセンスキー */
    MPG_WN_LUMI_YLVL(mpgwn) = 0;
    MPG_WN_LUMI_BDR(mpgwn) = 0;
    
    /* 画像データの出力モード */
    MPG_WN_OMODE(mpgwn) = MPG_OMODE_DRCT;
    
    /* 画像データの転送関数 */
    MPG_WN_TRFUNC(mpgwn) = trnsScuDmaBbus;
    
    /* 画像データの転送領域 */
    MPG_WN_BUF(mpgwn) = NULL;
    
    /* 表示スイッチ */
    MPG_WN_DISPSW(mpgwn) = OFF;
    
    return;
}

/* フレームバッファウィンドウ拡大率の作成 */
static Sint32   ratToFr(Uint32 ratio)
{
    Sint32      fr;
    Sint32      a, b;
    Sint32      p, r, iv;
    
    if (ratio >= RAT_DFL) {
        fr = 0x8000;
        
        /* 整数部分を求める */
        p = GET_INTEG(ratio);
        
        /* 小数部分を求める */
        r = GET_FRAC(ratio, p);
        
        a = p;
        b = fr_tbl[r];
    } else if (ratio >= 0) {
        fr = 0x0000;
        
        /* 逆数を求める */
        iv = GET_RECIP(ratio);

        /* 整数部分を求める */
        p = GET_INTEG(iv);
        
        /* 小数部分を求める */
        r = GET_FRAC(iv, p);
        
        a = p - 1;
        b = fr_tbl[r];
    }
    /* 拡大率をつくる */
    fr = (fr | (a << 4) | b);
    
    return fr;
}

/* SCUのDMAによるBバスへのイメージデータ転送 */
static Sint32   trnsScuDmaBbus(void *dst, void *src, Sint32 nbyte)
{
    Sint32      cnt;
    DmaScuPrm   prm;
    DmaScuStatus        stat;
    
    if (dst == NULL) {
        return MPG_ERR_NG;
    }

    prm.dxr = (Uint32)src;
    prm.dxw = (Uint32)dst;
    prm.dxc = nbyte;
    
    /* 転送元のアドレス加算値   */
    prm.dxad_r = DMA_SCU_R0;
    
    /* 転送先のアドレス加算値   */
    prm.dxad_w = DMA_SCU_W2;
    
    prm.dxmod = DMA_SCU_DIR;

    /* アドレス更新ビットとアドレス加算値の制限を回避するため、 */
    /* DMA_SCU_KEEPとする */
    prm.dxrup = DMA_SCU_KEEP;
    prm.dxwup = DMA_SCU_KEEP;
    prm.dxft = DMA_SCU_F_DMA;
    prm.msk = (DMA_SCU_M_DXR | DMA_SCU_M_DXW);
    DMA_ScuSetPrm(&prm, DMA_SCU_CH0);
    DMA_ScuStart(DMA_SCU_CH0);
    
    /* 転送先のキャッシュをパージ */
    CSH_Purge(dst, nbyte);
    
    cnt = 0;

    /* 転送終了チェック（タイムアウト時間は２秒） */
    do {
        if (cnt++ > TRNFIN_CNT) {
            return MPG_ERR_TMOUT;
        }
        DMA_ScuGetStatus(&stat, DMA_SCU_CH0);
    } while (stat.dxmv == DMA_SCU_MV);
    
    return MPG_ERR_OK;
}

/* MPEGウィンドウの消去 */
void    MPG_WnDestroy(MpgWn mpgwn)
{
    Sint32      dswsw, fbn;
    MpgWn       parep, chldp;
    
    MPSYS_SemaIn(FALSE);
    
    /* 表示スイッチがONなら非表示にする */
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        MPG_WnDisp(mpgwn, OFF);
    }
    
    /* mpgwnを切り離す */
    MPWIN_Disconnect(mpgwn);
    
    /* MPEGウィンドウの使用フラグをFALSEにする */
    MPG_WN_USEFLG(mpgwn) = FALSE;
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* MPEGウィンドウの出力モードの設定 */
void    MPG_WnSetOutputMode(MpgWn mpgwn, Sint32 omode, void *buf)
{
    MPSYS_SemaIn(FALSE);
    
    if (omode != MPG_NOCHG) {
        /* 画像データの出力モードの設定 */
        MPG_WN_OMODE(mpgwn) = omode;

        /* 画像データの転送領域の設定 */
        MPG_WN_BUF(mpgwn) = buf;
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* 表示・非表示の設定 */
void    MPG_WnDisp(MpgWn mpgwn, Bool sw)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);
    
    /* 表示・非表示の設定実行 */
    err = MPWIN_DispSub(mpgwn, sw);

    MPSYS_SemaOut(err);

    return;
}

/* 表示・非表示の設定実行 */
Sint32  MPWIN_DispSub(MpgWn mpgwn, Bool sw)
{
    Sint32      ret;

    ret = MPG_ERR_OK;
    if ((sw == ON) || (MPG_WN_DISPSW(mpgwn) == ON)) {
        /* 表示スイッチの設定 */
        MPG_WN_DISPSW(mpgwn) = sw;
        
        if (sw == ON) {
            /* ウィンドゥの設定 */
            if ((ret = setWn(mpgwn)) != MPG_ERR_OK) {
                return ret;
            }
        }
        /* 表示スイッチに従って、画像を表示する */
        ret = MPSYS_DoDisp(sw, MPG_WN_FBN(mpgwn));
    }    
    
    return ret;
}

/* ウィンドゥの設定 */
static Sint32   setWn(MpgWn mpgwn)
{
    Sint32      ret;
    CdcMpVef    mpvef, *vfptr;
    
    /* フレームバッファウィンドゥ位置の設定 */
    if (CDC_MpSetWinFpos(TRUE, MPG_WN_FP_X(mpgwn), MPG_WN_FP_Y(mpgwn)) !=
        CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* フレームバッファウィンドゥ拡大率の設定 */
    if (CDC_MpSetWinFrat(TRUE, MPG_WN_FR_X(mpgwn), MPG_WN_FR_Y(mpgwn)) !=
        CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* ディスプレイウィンドゥ位置の設定 */
    if (CDC_MpSetWinDpos(TRUE, MPG_WN_DP_X(mpgwn), MPG_WN_DP_Y(mpgwn)) !=
        CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* ディスプレイウィンドゥサイズの設定 */
    if (CDC_MpSetWinDsiz(TRUE, MPG_WN_DS_X(mpgwn), MPG_WN_DS_Y(mpgwn)) !=
        CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* ボーダカラーの設定 */
    if (CDC_MpSetBcolor(MPG_WN_BCLR(mpgwn)) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* フェードの設定 */
    ret = MPSYS_DoSetFade(MPG_WN_YCRAT_YR(mpgwn), MPG_WN_YCRAT_CR(mpgwn));
    if (ret != MPG_ERR_OK) {
        return ret;
    }    
    /* 画面特殊効果を設定する */
    ret = doSetWnEffect(mpgwn, SET_WN_INTPOL | SET_WN_MOZAIC | SET_WN_SOFT |
                        SET_WN_LUMIKEY);

    return ret;
}

/* 画面特殊効果を設定する */
static Sint32   doSetWnEffect(MpgWn mpgwn, Sint32 id)
{
    /* 画面特殊効果の初期値 */
    static const CdcMpVef       mvf_ini = {
        CDC_PARA_NOCHG, CDC_PARA_NOCHG,
        CDC_PARA_NOCHG, CDC_PARA_NOCHG,
        CDC_PARA_NOCHG, CDC_PARA_NOCHG
    };
    Sint32      ret;
    CdcMpVef    mpvef, *vfptr;

    /* 画面特殊効果パラメータの設定 */
    vfptr = &mpvef;

    /* 初期値の設定 */
    *vfptr = mvf_ini;

    if (id & SET_WN_INTPOL) {
        CDC_MPVEF_ITP(vfptr) = ((MPG_WN_INTPOL_YH(mpgwn) << 0) |
                                (MPG_WN_INTPOL_CH(mpgwn) << 1) |
                                (MPG_WN_INTPOL_YV(mpgwn) << 2) |
                                (MPG_WN_INTPOL_CV(mpgwn) << 3));
    }
    if (id & SET_WN_MOZAIC) {
        CDC_MPVEF_MOZH(vfptr) = MPG_WN_MOZ_H(mpgwn);
        CDC_MPVEF_MOZV(vfptr) = MPG_WN_MOZ_V(mpgwn);
    }
    if (id & SET_WN_SOFT) {
        CDC_MPVEF_SOFTH(vfptr) = MPG_WN_SOFTSW_H(mpgwn);
        CDC_MPVEF_SOFTV(vfptr) = MPG_WN_SOFTSW_V(mpgwn);
    }
    if (id & SET_WN_LUMIKEY) {
        CDC_MPVEF_TRP(vfptr) = (MPG_WN_LUMI_YLVL(mpgwn) |
                                (MPG_WN_LUMI_BDR(mpgwn) << 2));
    }

    /* 画面特殊効果の設定関数実行 */ 
    ret = MPSYS_DoSetVeff(vfptr);

    return ret;
}

/* 表示基準位置の変更 */
void    MPG_WnSetLoc(MpgWn mpgwn, Uint32 dzx, Uint32 dzy)
{
    Sint32      err;
    Sint32      dpx, dpy;
    
    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    
    /* ディスプレイウィンドウの表示基準位置の設定 */
    MPG_WN_DZ_X(mpgwn) = dzx;
    MPG_WN_DZ_Y(mpgwn) = dzy;
    
    /* ディスプレイウィンドウの位置の決定 */
    dpx = ((Sint32)dzx + MPG_WN_DL_X(mpgwn));
    MPG_WN_DP_X(mpgwn) = dpx;
    dpy = ((Sint32)dzy + MPG_WN_DL_Y(mpgwn));
    MPG_WN_DP_Y(mpgwn) = dpy;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* ディスプレイウィンドウ位置の設定 */
        if (CDC_MpSetWinDpos(TRUE, dpx, dpy) != CDC_ERR_OK) {
            err = MPG_ERR_NG;
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* 表示サイズの変更 */
void    MPG_WnSetSize(MpgWn mpgwn, Uint32 dsx, Uint32 dsy)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    
    /* ディスプレイウィンドウの大きさの設定 */
    MPG_WN_DS_X(mpgwn) = dsx;
    MPG_WN_DS_Y(mpgwn) = dsy;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* ディスプレイウィンドウサイズの設定 */
        if (CDC_MpSetWinDsiz(TRUE, dsx, dsy) != CDC_ERR_OK) {
            err = MPG_ERR_NG;
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* 表示相対位置の設定 */
void    MPG_WnSetDispOfst(MpgWn mpgwn, Sint32 dlx, Sint32 dly)
{
    Sint32      err;
    Sint32      dpx, dpy, fpx, fpy;
    
    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    
    /* ディスプレイウィンドウの相対位置の設定 */
    MPG_WN_DL_X(mpgwn) = dlx;
    MPG_WN_DL_Y(mpgwn) = dly;
    
    /* ディスプレイウィンドウ位置の決定 */
    dpx = (MPG_WN_DZ_X(mpgwn) + dlx);
    MPG_WN_DP_X(mpgwn) = dpx;
    dpy = (MPG_WN_DZ_Y(mpgwn) + dly);
    MPG_WN_DP_Y(mpgwn) = dpy;
    
    /* フレームバッファウィンドウ位置の決定 */
    fpx = GET_FPOS(MPG_WN_FZ_X(mpgwn), dlx, MPG_WN_RAT_X(mpgwn));
    MPG_WN_FP_X(mpgwn) = fpx;
    fpy = GET_FPOS(MPG_WN_FZ_Y(mpgwn), dly, MPG_WN_RAT_Y(mpgwn));
    MPG_WN_FP_Y(mpgwn) = fpy;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 表示相対位置の設定実行 */
        err = setOfstSub(dpx, dpy, fpx, fpy);
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* 表示相対位置の設定実行 */
static Sint32   setOfstSub(Sint32 dpx, Sint32 dpy,
                           Sint32 fpx, Sint32 fpy)
{
    /* ディスプレイウィンドウ位置の設定 */
    if (CDC_MpSetWinDpos(TRUE, dpx, dpy) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* フレームバッファウィンドウ位置の設定 */
    if (CDC_MpSetWinFpos(TRUE, fpx, fpy) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    return MPG_ERR_OK;
}

/* 倍率の設定 */
void    MPG_WnSetDispRatio(MpgWn mpgwn, Uint32 x_rat, Uint32 y_rat)
{
    Sint32      err;
    Sint32      frx, fry, fpx, fpy;
    
    MPSYS_SemaIn(FALSE);
    
    err = MPG_ERR_OK;
    
    /* ｘ方向の倍率をまるめる */
    if (x_rat < 2) {
        x_rat = 2;
    }
    /* ｙ方向の倍率をまるめる */
    if (y_rat < 4) {
        y_rat = 4;
    }
    /* フレームバッファウィンドウ拡大率パラメータの作成 */
    frx = ratToFr(x_rat);
    MPG_WN_FR_X(mpgwn) = frx;
    fry = ratToFr(y_rat);
    MPG_WN_FR_Y(mpgwn) = fry;
    
    /* 拡大率パラメータの倍率への変換 */
    x_rat = frToRat(frx);
    y_rat = frToRat(fry);
    
    /* フレームバッファウィンドウの倍率の設定 */
    MPG_WN_RAT_X(mpgwn) = x_rat;
    MPG_WN_RAT_Y(mpgwn) = y_rat;    
    
    /* フレームバッファウィンドウ位置の決定 */
    fpx = GET_FPOS(MPG_WN_FZ_X(mpgwn), MPG_WN_DL_X(mpgwn), (Sint32)x_rat);
    MPG_WN_FP_X(mpgwn) = fpx;
    fpy = GET_FPOS(MPG_WN_FZ_Y(mpgwn), MPG_WN_DL_Y(mpgwn), (Sint32)y_rat);
    MPG_WN_FP_Y(mpgwn) = fpy;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 倍率の設定実行 */
        err = setRatioSub(frx, fry, fpx, fpy);
    }
    MPSYS_SemaOut(err);

    return;
}

/* 拡大率パラメータの倍率への変換 */
static Uint32   frToRat(Sint32 fr)
{
    Uint32      rat, a;
    Sint32      idx;
    
    a = ((fr >> 4) & 0x03ff);
    idx = (fr & 0x000f);
    
    rat = (a * RAT_DFL) + rat_tbl[idx];

    if ((fr & 0x8000) == 0) {
        rat = GET_RECIP(rat + RAT_DFL);
    }

    return rat;
}

/* 倍率の設定実行 */
static Sint32   setRatioSub(Sint32 frx, Sint32 fry,
                            Sint32 fpx, Sint32 fpy)
{
    /* フレームバッファウィンドウ拡大率の設定 */
    if (CDC_MpSetWinFrat(TRUE, frx, fry) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    /* フレームバッファウィンドウ位置の設定 */
    if (CDC_MpSetWinFpos(TRUE, fpx, fpy) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    return MPG_ERR_OK;
}

/* フレームウィンドウの位置の設定 */
void    MPG_WnSetFrmLoc(MpgWn mpgwn, Uint32 fzx, Uint32 fzy)
{
    Sint32      err;
    Sint32      fpx, fpy;
    
    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* フレームバッファウィンドウの位置（ズームポイント）の設定 */
    MPG_WN_FZ_X(mpgwn) = fzx;
    MPG_WN_FZ_Y(mpgwn) = fzy;
    
    /* フレームバッファウィンドウ位置の決定 */
    fpx = GET_FPOS((Sint32)fzx, MPG_WN_DL_X(mpgwn), MPG_WN_RAT_X(mpgwn));
    MPG_WN_FP_X(mpgwn) = fpx;
    fpy = GET_FPOS((Sint32)fzy, MPG_WN_DL_Y(mpgwn), MPG_WN_RAT_Y(mpgwn));
    MPG_WN_FP_Y(mpgwn) = fpy;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* フレームバッファウィンドウ位置の設定 */
        if (CDC_MpSetWinFpos(TRUE, fpx, fpy) != CDC_ERR_OK) {
            err = MPG_ERR_NG;
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* ホスト領域への転送 */
Sint32  MPG_WnTrans(MpgWn mpgwn)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);
    
    ret = MPG_ERR_ILLSTAT;

    if ((MPG_WN_DISPSW(mpgwn) == ON) &&
        (MPG_WN_OMODE(mpgwn) == MPG_OMODE_HOST)) {

        /* ホスト転送の実行 */
        ret = doTrans(mpgwn);
    }
    
    return MPSYS_SemaOut(ret);
}

/* ホスト転送の実行 */
static Sint32   doTrans(MpgWn mpgwn)
{
    Sint32      ret;
    Sint32      dwsiz;
    Uint32      *rgbreg;
    Uint16      *buf;
    MpgTrFunc   trfunc;

    /* ウィンドゥの設定 */
    if ((ret = setWn(mpgwn)) != MPG_ERR_OK) {
        return ret;
    }

    /* イメージデータの取り出し */
    if (CDC_MpGetImg(&dwsiz) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    /* MPEGRGBレジスタのポインタ取得 */
    rgbreg = CDC_GetMpegPtr();

    /* イメージデータの転送領域のポインタ取得 */
    buf = MPG_WN_BUF(mpgwn);

    /* 転送関数のNULLチェック */
    trfunc = MPG_WN_TRFUNC(mpgwn);
    if (trfunc == NULL) {
        return MPG_ERR_NG;
    }

    /* イメージデータの転送 */
    ret = (*trfunc)(buf, rgbreg, dwsiz * sizeof(Uint16));

    return ret;
}

/* ボーダカラーの設定 */
void    MPG_WnSetBcolor(MpgWn mpgwn, Uint16 clr)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* ボーダカラーの設定 */
    MPG_WN_BCLR(mpgwn) = clr;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* ボーダカラーの設定 */
        if (CDC_MpSetBcolor(clr) != CDC_ERR_OK) {
            err = MPG_ERR_NG;
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* ＹＣ倍率の設定 */
void    MPG_WnSetYcRatio(MpgWn mpgwn, Uint32 y_rat, Uint32 c_rat)
{
    Sint32      err;
    Sint32      yr, cr;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    yr = GET_YCRAT(y_rat);
    cr = GET_YCRAT(c_rat);

    /* 輝度信号の倍率の設定 */
    MPG_WN_YCRAT_YR(mpgwn) = yr;
    
    /* 色差信号の倍率の設定 */
    MPG_WN_YCRAT_CR(mpgwn) = cr;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* フェードの設定 */
        err = MPSYS_DoSetFade(yr, cr);
    }    
    
    MPSYS_SemaOut(err);

    return;
}

/* 補間モードの設定 */
void    MPG_WnSetIntpol(MpgWn mpgwn,
                        Bool sw_yh, Bool sw_yv, Bool sw_ch, Bool sw_cv)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* 輝度の水平方向の補間スイッチの設定 */
    MPG_WN_INTPOL_YH(mpgwn) = sw_yh;
    
    /* 輝度の垂直方向の補間スイッチの設定 */
    MPG_WN_INTPOL_YV(mpgwn) = sw_yv;
    
    /* 色差の水平方向の補間スイッチの設定 */
    MPG_WN_INTPOL_CH(mpgwn) = sw_ch;
    
    /* 色差の垂直方向の補間スイッチの設定 */
    MPG_WN_INTPOL_CV(mpgwn) = sw_cv;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 画面特殊効果を設定する */
        err = doSetWnEffect(mpgwn, SET_WN_INTPOL);
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* モザイクの設定 */
void    MPG_WnSetMozaic(MpgWn mpgwn, Sint32 moz_h, Sint32 moz_v)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* ｘ方向のモザイクの割合の設定 */
    MPG_WN_MOZ_H(mpgwn) = moz_h;
    
    /* ｙ方向のモザイクの割合の設定 */
    MPG_WN_MOZ_V(mpgwn) = moz_v;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 画面特殊効果を設定する */
        err = doSetWnEffect(mpgwn, SET_WN_MOZAIC);
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* ぼかしの設定 */
void    MPG_WnSetSoft(MpgWn mpgwn, Bool sof_h, Bool sof_v)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* ぼかしスイッチの設定 */
    MPG_WN_SOFTSW_H(mpgwn) = sof_h;
    MPG_WN_SOFTSW_V(mpgwn) = sof_v;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 画面特殊効果を設定する */
        err = doSetWnEffect(mpgwn, SET_WN_SOFT);
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* ルミネッセンスキーの設定 */
void    MPG_WnSetLumiKey(MpgWn mpgwn, Sint32 y_lvl, Bool bdr)
{
    Sint32      err;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;

    /* 輝度信号のレベルの設定 */
    MPG_WN_LUMI_YLVL(mpgwn) = y_lvl;
    
    /* MPEG映像の境界を拡大するか否かの設定 */
    MPG_WN_LUMI_BDR(mpgwn) = bdr;
    
    if (MPG_WN_DISPSW(mpgwn) == ON) {
        /* 画面特殊効果を設定する */
        err = doSetWnEffect(mpgwn, SET_WN_LUMIKEY);
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* 転送関数の登録 */
void    MPG_WnEntryTrFunc(MpgWn mpgwn, MpgTrFunc trfunc)
{
    if (trfunc == NULL) {
        /* デフォルトの転送関数の代入 */
        trfunc = trnsScuDmaBbus;
    }
    /* ユーザ転送関数の登録 */
    MPG_WN_TRFUNC(mpgwn) = trfunc;

    return;
}

/* 全MPEGウィンドウの初期化 */
void    MPWIN_InitAllWn(void)
{
    Sint32      i;
    
    for (i = 0; i < MPGWN_NUM; i++) {
        /* MPEGウィンドウパラメータの初期化 */
        initWinPara(&mp_win[i]);
    }
    return;
}

/* mpgwn1の最後にmpgwn2を接続する */
void    MPWIN_Connect(MpgWn mpgwn1, MpgWn mpgwn2)
{
    MpgWn       mpgwnwk, chld;
    
    mpgwnwk = mpgwn1;

    /* 終端を探す */
    while ((chld = MPG_WN_CHLD(mpgwnwk)) != NULL) {
        mpgwnwk = chld;
    }
    
    /* mpgwn2を接続する */
    MPG_WN_CHLD(mpgwnwk) = mpgwn2;
    MPG_WN_PARE(mpgwn2) = mpgwnwk;
    
    return;
}

/* mpgwnを切り離す */
void    MPWIN_Disconnect(MpgWn mpgwn)
{
    MpgWn       parep, chldp;
    
    /* リストのつなぎ換え */
    parep = MPG_WN_PARE(mpgwn);
    chldp = MPG_WN_CHLD(mpgwn);
    if (parep != NULL) {
        MPG_WN_CHLD(parep) = chldp;
    }
    if (chldp != NULL) {
        MPG_WN_PARE(chldp) = parep;
    }
    MPG_WN_PARE(mpgwn) = NULL;
    MPG_WN_CHLD(mpgwn) = NULL;
    
    return;
}

/* mpgwnに接続されている全てのホスト出力ウィンドウを転送する */
void    MPWIN_OutputAll(MpgWn mpgwn, Sint32 omode)
{
    MpgWn       mpgwnwk;
    
    mpgwnwk = mpgwn;
    if (omode == MPG_OMODE_DRCT) {      /* ダイレクト転送モード */
        return;
    }
    /* ホスト転送モードの場合、表示中のウィンドウを全て出力する */
    do {
        if (MPG_WN_OMODE(mpgwnwk) == MPG_OMODE_HOST) {
            MPG_WnTrans(mpgwnwk);
        }
        mpgwnwk = MPG_WN_CHLD(mpgwnwk);
    } while (mpgwnwk != NULL);

    return;
}

