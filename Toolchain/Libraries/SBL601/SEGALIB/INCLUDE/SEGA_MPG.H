/*
 *     ソフトウェアライブラリ
 *
 *     Copyright (c) 1995 SEGA
 *
 *  Library: MPEGライブラリ（MPG）
 *  Module : MPEGライブラリ用ヘッダファイル
 *  File   : sega_mpg.h
 *  Date   : 1996-01-31
 *  Version: 1.24
 *  Author : H.T.
 *
 */

/* 多重インクルードへの対応 */
#ifndef SEGA_MPG_H
#define SEGA_MPG_H

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        "sega_stm.h"

/*******************************************************************
 *      定数マクロ
 *******************************************************************/

/* バージョン */
                    /*             1         2         3         4    */
                    /*    1234567890123456789012345678901234567890    */
                    /*    ++++++++++++++++++++++++++++++++            */
#define MPG_VERSION_STR ("MPG Version 1.24 1995-12-25")
#define MPG_VERSION_LEN         (32)

/* MPEGストリームの指定 */
#define MPG_STM_CUR             (0x00)          /* 現ストリーム */
#define MPG_STM_NEXT            (0x01)          /* 次のストリーム */
#define MPG_AVAILABLE_ID        (0x01)          /* ストリームIDが有効 */
#define MPG_AVAILABLE_CH        (0x02)          /* チャネル番号が有効 */

/* パラメータの変更なし指定 */
#define MPG_NOCHG               CDC_PARA_NOCHG

/* CDバッファ区画が割り付けられていないときの指定 */
#define MPG_NOCDB               CDC_PARA_NOCHG

/* パラメータを無視するときの指定 */
#define MPG_IGNORE              CDC_PARA_NOCHG

/* 再生モード */
#define MPG_PMODE_AV            (0)     /* AV同期再生モード */
#define MPG_PMODE_A             (1)     /* ビデオ再生モード */
#define MPG_PMODE_V             (2)     /* オーディオ再生モード */

/* ポーズモードの指定 */
#define MPG_PAUSE_ON            (0)     /* 一時停止／コマ送り */
#define MPG_PAUSE_OFF           (1)     /* 通常再生 */
#define MPG_PAUSE_SLOW          (2)     /* スロー再生 */

/* フリーズモードの指定 */
#define MPG_FREEZE_ON           (0)     /* フリーズ／再フリーズ */
#define MPG_FREEZE_OFF          (1)     /* 通常再生 */
#define MPG_FREEZE_STRB         (2)     /* ストロボ再生 */

/* ストリームのレイヤ指定 */
#define MPG_LAY_SYS             (0)     /* システムレイヤ */
#define MPG_LAY_VIDEO           (1)     /* ビデオレイヤ */
#define MPG_LAY_AUDIO           (1)     /* オーディオレイヤ */

/* AV同期再生の指定 */
#define MPG_AV_SYNC             (0)     /* AV同期再生 */
#define MPG_AV_ASYNC            (1)     /* AV独立再生 */

/* 表示の走査方法 */
#define MPG_DSCN_NITL           (0x00)  /* NTSCノンインタレースモード */
#define MPG_DSCN_ITL            (0x01)  /* NTSCインタレースモード */
#define MPG_DSCN_PLNITL         (0x02)  /* PALノンインタレースモード */
#define MPG_DSCN_PLITL          (0x03)  /* PALインタレースモード */

/* MPEGビデオ動作モード */
#define MPG_VMODE_MOVIE         (0x00)          /* 動画再生モード */
#define MPG_VMODE_SPIC          (0x01)          /* 静止画再生モード */
#define MPG_VMODE_SPIC_H        (0x03)          /* 高精細静止画再生モード */

/* ビデオの動作状態 */
#define MPG_VSTAT_STOP          (0x00)          /* 停止中 */
#define MPG_VSTAT_PREP          (0x01)          /* 準備中 */
#define MPG_VSTAT_PLAY          (0x02)          /* 再生中 */
#define MPG_VSTAT_ERR           (0x06)          /* エラー */

/* オーディオの動作状態 */
#define MPG_ASTAT_STOP          (0x00)          /* 停止中 */
#define MPG_ASTAT_PREP          (0x01)          /* 準備中 */
#define MPG_ASTAT_PLAY          (0x02)          /* 再生中 */
#define MPG_ASTAT_ERR           (0x06)          /* エラー */

/* 静止画の動作状態 */
#define MPG_SSTAT_STOP          MPG_VSTAT_STOP  /* 停止中 */
#define MPG_SSTAT_PREP          MPG_VSTAT_PREP  /* 準備中 */
#define MPG_SSTAT_PLAY          MPG_VSTAT_PLAY  /* 再生中 */
#define MPG_SSTAT_ERR           MPG_VSTAT_ERR   /* エラー */

/* MPEGストリームの終了条件 */
#define MPG_TCND_NONE           (0)             /* 終了条件なし */
#define MPG_TCND_EOR            (1)             /* EORビット検出 */
#define MPG_TCND_SEC            (2)             /* システムエンドコード検出 */

/* MPEGビデオの出力先 */
#define MPG_OUTDST_DRCT         (0)             /* VDP2直接出力 */
#define MPG_OUTDST_HOST         (1)             /* ホスト転送出力 */

/* MPEGビデオのデコードタイミング */
#define MPG_DECTMG_VSYNC        (0)             /* VSYNC同期デコード */
#define MPG_DECTMG_HOST         (1)             /* ホスト同期デコード */

/* ビデオストリームの消去モード */
#define MPG_VCLR_VBV_IP (0x02)  /* 次のIまたはPピクチャスタートでVBVクリア */
#define MPG_VCLR_ALL    (0x00)  /* フレームバッファとVBVを即座にクリア */

/* オーディオストリームの消去モード */
#define MPG_ACLR_NONE           (0x00)          /* クリアしない */
#define MPG_ACLR_ALL            (0x01)          /* 即座にすべてクリア */

/* デコードの開始待ちパラメータ */
#define MPG_HDEC_PRE            (0)             /* デコード開始前処理 */
#define MPG_HDEC_EXE            (1)             /* デコード開始待ち */

/* ディスプレイウィンドウの出力方法 */
#define MPG_OMODE_DRCT          (0)             /* ＶＤＰ２への直接出力 */
#define MPG_OMODE_HOST          (1)     /* ホスト領域への転送による出力 */

/*******************************************************************
 *      列挙定数
 *******************************************************************/

/* エラーコード */
enum MpgErrCode {
    MPG_ERR_OK       = OK,      /* 正常終了 */
    
    MPG_ERR_CDINIT   = -1,      /* CDブロックの初期化に失敗 */
    MPG_ERR_NOMC     = -2,      /* MPEGカートリッジがない */
    MPG_ERR_MPNG     = -3,      /* MPEGチェックに失敗 */
    MPG_ERR_MPINIT   = -4,      /* MPEGデコーダの初期化に失敗 */
    MPG_ERR_TMOUT    = -5,      /* タイムアウト */
    MPG_ERR_PLAYING  = -6,      /* 他のMPEGストリームを再生中 */
    MPG_ERR_MCON     = -7,      /* MPEGデコーダの接続に失敗 */
    MPG_ERR_RCV      = -8,      /* 復活処理中に停止処理を行った */
    MPG_ERR_CHG      = -9,      /* 切替処理中に停止処理を行った */
    MPG_ERR_ILLSTAT  = -10,     /* ハンドルが動作状態ではない */
    MPG_ERR_ENTRYNG  = -11,     /* 次の動画の登録に失敗した */
    MPG_ERR_ILLHDL   = -12,     /* 不正なハンドルを使用した */
    MPG_ERR_CREATE   = -13,     /* ハンドルの生成に失敗した */
    MPG_ERR_GETTC    = -14,     /* タイムコードの取得に失敗した */
    MPG_ERR_DESTROY  = -15,     /* ハンドルの消去に失敗した */
    MPG_ERR_WAIT     = -16,     /* MPEGコマンドの実行がWAITになった */
    MPG_ERR_NG       = -17      /* その他の異常終了 */
};

/*******************************************************************
 *      構造体アクセス処理マクロ
 *******************************************************************/

/* MPEGウィンドウ */
#define MPG_WN_CHLD(wn)                         ((wn)->chld)
#define MPG_WN_PARE(wn)                         ((wn)->pare)
#define MPG_WN_TRFUNC(wn)                       ((wn)->trfunc)
#define MPG_WN_BUF(wn)                          ((wn)->buf)
#define MPG_WN_USEFLG(wn)                       ((wn)->useflg)
#define MPG_WN_DISPSW(wn)                       ((wn)->dispsw)
#define MPG_WN_DZ_X(wn)                         ((wn)->dz.x)
#define MPG_WN_DZ_Y(wn)                         ((wn)->dz.y)
#define MPG_WN_DS_X(wn)                         ((wn)->ds.x)
#define MPG_WN_DS_Y(wn)                         ((wn)->ds.y)
#define MPG_WN_DL_X(wn)                         ((wn)->dl.x)
#define MPG_WN_DL_Y(wn)                         ((wn)->dl.y)
#define MPG_WN_RAT_X(wn)                        ((wn)->rat.x)
#define MPG_WN_RAT_Y(wn)                        ((wn)->rat.y)
#define MPG_WN_FZ_X(wn)                         ((wn)->fz.x)
#define MPG_WN_FZ_Y(wn)                         ((wn)->fz.y)
#define MPG_WN_DP_X(wn)                         ((wn)->dp.x)
#define MPG_WN_DP_Y(wn)                         ((wn)->dp.y)
#define MPG_WN_FP_X(wn)                         ((wn)->fp.x)
#define MPG_WN_FP_Y(wn)                         ((wn)->fp.y)
#define MPG_WN_FR_X(wn)                         ((wn)->fr.x)
#define MPG_WN_FR_Y(wn)                         ((wn)->fr.y)
#define MPG_WN_YCRAT_YR(wn)                     ((wn)->ycrat.yr)
#define MPG_WN_YCRAT_CR(wn)                     ((wn)->ycrat.cr)
#define MPG_WN_INTPOL_YH(wn)                    ((wn)->intpol.yh)
#define MPG_WN_INTPOL_YV(wn)                    ((wn)->intpol.yv)
#define MPG_WN_INTPOL_CH(wn)                    ((wn)->intpol.ch)
#define MPG_WN_INTPOL_CV(wn)                    ((wn)->intpol.cv)
#define MPG_WN_MOZ_H(wn)                        ((wn)->moz.h)
#define MPG_WN_MOZ_V(wn)                        ((wn)->moz.v)
#define MPG_WN_SOFTSW_H(wn)                     ((wn)->softsw.h)
#define MPG_WN_SOFTSW_V(wn)                     ((wn)->softsw.v)
#define MPG_WN_LUMI_YLVL(wn)                    ((wn)->lumi.ylvl)
#define MPG_WN_LUMI_BDR(wn)                     ((wn)->lumi.bdr)
#define MPG_WN_BCLR(wn)                         ((wn)->bclr)
#define MPG_WN_OMODE(wn)                        ((wn)->omode)
#define MPG_WN_FBN(wn)                          ((wn)->fbn)

/* MPEG動画ハンドル */
#define MPG_MV_MPGWN(mv, i)                     ((mv)->mpgwn[i])
#define MPG_MV_NEXTMV(mv)                       ((mv)->nextmv)
#define MPG_MV_USEDFLAG(mv)                     ((mv)->used_flag)
#define MPG_MV_ISMVEND(mv)                      ((mv)->ismvend)
#define MPG_MV_TCFLAG(mv)                       ((mv)->tcflag)
#define MPG_MV_FTC(mv)                          ((mv)->ftc)
#define MPG_MV_VCH(mv)                          ((mv)->vch)
#define MPG_MV_ACH(mv)                          ((mv)->ach)
#define MPG_MV_PAUTIM(mv)                       ((mv)->pautim)
#define MPG_MV_FRZTIM(mv)                       ((mv)->frztim)
#define MPG_MV_PMODE(mv)                        ((mv)->pmode)
#define MPG_MV_ACTV(mv)                         ((mv)->actv)
#define MPG_MV_ACTA(mv)                         ((mv)->acta)
#define MPG_MV_VBNO(mv)                         ((mv)->vbno)
#define MPG_MV_ABNO(mv)                         ((mv)->abno)
#define MPG_MV_VCND(mv)                         ((mv)->vcnd)
#define MPG_MV_ACND(mv)                         ((mv)->acnd)
#define MPG_MV_VLAY(mv)                         ((mv)->vlay)
#define MPG_MV_ALAY(mv)                         ((mv)->alay)
#define MPG_MV_OUTDST(mv)                       ((mv)->outdst)
#define MPG_MV_DECTMG(mv)                       ((mv)->dectmg)
#define MPG_MV_AMUTE(mv)                        ((mv)->amute)
#define MPG_MV_VID(mv)                          ((mv)->vid)
#define MPG_MV_AID(mv)                          ((mv)->aid)
#define MPG_MV_CLRV(mv)                         ((mv)->clr_v)
#define MPG_MV_CLRA(mv)                         ((mv)->clr_a)

/* エラー状態 */
#define MPG_ERR_FUNC(err)                       ((err)->func)
#define MPG_ERR_OBJ(err)                        ((err)->obj)
#define MPG_ERR_CODE(err)                       ((err)->code)

/*******************************************************************
 *      データ型の宣言
 *******************************************************************/

/* ウィンドウの位置 */
typedef struct {
    Sint16      x;                              /* ｘ座標 */
    Sint16      y;                              /* ｙ座標 */
} MpgPos;

/* ウィンドウの大きさ */
typedef struct {
    Sint16      x;                              /* ｘ方向 */
    Sint16      y;                              /* ｙ方向 */
} MpgSiz;

/* ウィンドウの倍率 */
typedef struct {
    Sint16      x;                              /* ｘ方向 */
    Sint16      y;                              /* ｙ方向 */
} MpgRat;

/* ＹＣ倍率 */
typedef struct {
    Sint8       yr;                             /* 輝度信号の倍率 */
    Sint8       cr;                             /* 色差信号の倍率 */
} MpgYcrat;

/* 補間モード */
typedef struct {
    Sint8       yh;             /* 輝度の水平方向の補間スイッチ */
    Sint8       yv;             /* 輝度の垂直方向の補間スイッチ */
    Sint8       ch;             /* 色差の水平方向の補間スイッチ */
    Sint8       cv;             /* 色差の垂直方向の補間スイッチ */
} MpgIntpol;

/* モザイク */
typedef struct {
    Sint16      h;              /* 水平方向のモザイクの割合 */
    Sint16      v;              /* 垂直方向のモザイクの割合 */
} MpgMoz;

/* ぼかしスイッチ */
typedef struct {
    Sint8       h;              /* 水平方向のぼかしスイッチ */
    Sint8       v;              /* 垂直方向のぼかしスイッチ */
} MpgSoftsw;

/* ルミネッセンスキー */
typedef struct {
    Uint8       ylvl;           /* 輝度信号のレベル */
    Sint8       bdr;            /* MPEG映像の境界を拡大するか否か */
} MpgLumi;

/* MPEGウィンドウ */
typedef struct MpgWin {
    struct MpgWin       *chld;  /* 子MPEGウィンドウへのポインタ */
    struct MpgWin       *pare;  /* 親MPEGウィンドウへのポインタ */
    Sint32      (*trfunc)(void *dst, void *src, Sint32 nbyte);
    /* 画像データの転送関数 */
    void        *buf;           /* 画像データの転送領域 */
    Bool        useflg;         /* 使用フラグ */
    Bool        dispsw;         /* 表示スイッチ */
    MpgPos      dz;             /* ディスプレイウィンドウの表示基準位値 */
    MpgSiz      ds;             /* ディスプレイウィンドウの表示サイズ */
    MpgPos      dl;             /* ディスプレイウィンドウの表示相対位置 */
    MpgRat      rat;            /* フレームバッファウィンドウの倍率 */
    MpgPos      fz;     /* フレームバッファウィンドウの位置(ズームポイント) */
    MpgPos      dp;     /* ディスプレイウィンドウ位置パラメータ */
    MpgPos      fp;     /* フレームバッファウィンドウ位置パラメータ */
    MpgRat      fr;     /* フレームバッファウィンドウ拡大率パラメータ */
    MpgYcrat    ycrat;          /* ＹＣ倍率 */
    MpgIntpol   intpol;         /* 補間モード */
    MpgMoz      moz;            /* モザイク */
    MpgSoftsw   softsw;         /* ぼかしスイッチ */
    MpgLumi     lumi;           /* ルミネッセンスキー */
    Uint16      bclr;           /* ボーダカラー */
    Sint8       omode;          /* 画像データの出力モード */
    Sint8       fbn;            /* 使用するフレームバンク番号 */
} MpgWin;

typedef MpgWin  *MpgWn;

/* MPEG動画ハンドル */
typedef struct MpgMovie {
    MpgWn       mpgwn[4];               /* MPEGウィンドウ */
    struct MpgMovie     *nextmv;        /* 次に再生するMPEG動画 */
    Bool        used_flag;              /* 使用中か否か? */
    Bool        actv;                   /* ビデオが再生動作中か否か? */
    Bool        acta;                   /* オーディオが再生動作中か否か? */
    Bool        ismvend;                /* 再生が終了したか */
    Bool        tcflag;                 /* 相対タイムコードフラグ */
    Sint32      ftc;                    /* 最初の絵のタイムコード */
    Sint16      vch;                    /* ビデオのチャネル番号 */
    Sint16      ach;                    /* オーディオのチャネル番号 */
    Sint16      pautim;                 /* ポーズタイミング */
    Sint16      frztim;                 /* フリーズタイミング */
    Sint8       pmode;                  /* 再生モード (AV, A_only, V_only) */
    Sint8       vbno;                   /* ビデオバッファ区画番号 */
    Sint8       abno;                   /* オーディオバッファ区画番号 */
    Sint8       vcnd;                   /* ビデオの終了条件 */
    Sint8       acnd;                   /* オーディオの終了条件 */
    Sint8       vlay;                   /* ビデオのレイヤ指定 */
    Sint8       alay;                   /* オーディオのレイヤ指定 */
    Sint8       outdst;                 /* 出力先 */
    Sint8       dectmg;                 /* デコードタイミング */
    Sint8       amute;                  /* オ－ディオのミュート */
    Sint8       vid;                    /* ビデオのストリームID */
    Sint8       aid;                    /* オーディオのストリームID */
    Sint8       clr_v;                  /* デコードバッファの消去方法 */
    Sint8       clr_a;                  /* デコードバッファの消去方法 */
} MpgMovie;

typedef MpgMovie *MpgMv;

/* MPEG静止画ハンドル */
typedef MpgMv MpgSp;

/* 転送関数 */
typedef Sint32 (*MpgTrFunc)(void *dst, void *src, Sint32 nbyte);

/* エラー関数 */
typedef void (*MpgErrFunc)(void *obj, Sint32 errcode);

/* エラー状態 */
typedef struct {
    Sint32      code;
    MpgErrFunc  func;
    void        *obj;
} MpgErrStat;

/*******************************************************************
 *      関数宣言
 *******************************************************************/

/* mpg_sys.c */
Sint32  MPG_Init(Sint32 dscn);
void    MPG_CaptStat(void);
Sint32  MPG_SetVideoMode(Sint32 vmode);
Sint32  MPG_GetVideoMode(void);
Bool    MPG_IsDecReady(Sint32 decpic);
void    MPG_ResetHng(void);
Bool    MPG_CheckHng(void);
Sint32  MPG_GetReport(Sint32 *actst, Sint32 *vcnt, Sint32 *pict,
                      Sint32 *stat_a, Sint32 *stat_v);
Sint32  MPG_GetInt(Sint32 *mpint);
Sint32  MPG_ResetMp(Bool mpsw);
Sint32  MPG_GetErrStat(MpgErrStat *stat);
void    MPG_SetErrFunc(MpgErrFunc func, void *obj);

/* mpg_mve.c */
MpgMv   MPG_MvCreate(StmHn vstm, StmHn astm);
Sint32  MPG_MvDestroy(MpgMv mpgmv);
void    MPG_MvSetTermCond(MpgMv mpgmv, Sint32 vcnd, Sint32 acnd);
void    MPG_MvSetVideoDest(MpgMv mpgmv, Sint32 dst);
void    MPG_MvSetDecTim(MpgMv mpgmv, Sint32 tmg);
void    MPG_MvPause(MpgMv mpgmv, Sint32 cmd, Sint32 itvl);
void    MPG_MvFreeze(MpgMv mpgmv, Sint32 cmd, Sint32 itvl);
void    MPG_MvSetPan(MpgMv mpgmv, Bool lsw, Bool rsw);
Sint32  MPG_MvGetVideoStat(MpgMv mpgmv);
Sint32  MPG_MvGetAudioStat(MpgMv mpgmv);
void    MPG_MvConnectWin(MpgMv mpgmv, MpgWn mpgwn);
void    MPG_MvDisconnectWin(MpgMv mpgmv, MpgWn mpgwn);
Sint32  MPG_MvDecNext(MpgMv mpgmv);
Sint32  MPG_MvOutput(MpgMv mpgmv, Bool chkupic);
Sint32  MPG_MvChange(MpgMv mpgmv);
void    MPG_MvSetClearMode(MpgMv mpgmv, Sint32 vmode, Sint32 amode);
void    MPG_MvSetLay(MpgMv mpgmv, Sint32 vlay, Sint32 alay);

/* mpg_mve2.c */
void    MPG_MvSetVideoChan(MpgMv mpgmv, Sint32 vch);
void    MPG_MvSetAudioChan(MpgMv mpgmv, Sint32 ach);
void    MPG_MvSetVideoId(MpgMv mpgmv, Sint32 id);
void    MPG_MvSetAudioId(MpgMv mpgmv, Sint32 id);
Sint32  MPG_MvStart(MpgMv mpgmv, Bool picsch);
Sint32  MPG_MvStop(MpgMv mpgmv);
Sint32  MPG_MvStopVideo(MpgMv mpgmv);
Sint32  MPG_MvStopAudio(MpgMv mpgmv);
void    MPG_MvGetTimeCode(MpgMv mpgmv,
                          Sint32 *hh, Sint32 *mm, Sint32 *ss, Sint32 *pp);
Sint32  MPG_MvGetPlaytime(MpgMv mpgmv);
Sint32  MPG_MvEntryNext(MpgMv mpgmv, MpgMv nextmv);

/* mpg_spc.c */
MpgSp   MPG_SpCreate(StmHn vstm);
Sint32  MPG_SpDestroy(MpgSp mpgsp);
void    MPG_SpSetVideoDest(MpgSp mpgsp, Sint32 dst);
void    MPG_SpSetTermCond(MpgSp mpgsp, Sint32 vcnd);
void    MPG_SpSetChan(MpgSp mpgsp, Sint32 ch);
void    MPG_SpSetId(MpgSp mpgsp, Sint32 id);
Sint32  MPG_SpStart(MpgSp mpgsp);
Sint32  MPG_SpStop(MpgSp mpgsp);
Sint32  MPG_SpDecNext(MpgSp mpgsp, Sint32 bnk);
Sint32  MPG_SpGetVideoStat(MpgSp mpgsp);
void    MPG_SpGetTimeCode(MpgSp mpgsp,
                          Sint32 *hh, Sint32 *mm, Sint32 *ss, Sint32 *pp);
void    MPG_SpConnectWin(MpgSp mpgsp, MpgWn mpgwn, Sint32 bnk);
void    MPG_SpDisconnectWin(MpgSp mpgsp, MpgWn mpgwn, Sint32 bnk);
Sint32  MPG_SpOutput(MpgSp mpgsp, Sint32 bnk);

/* mpg_win.c */
MpgWn   MPG_WnCreate(Uint32 dzx, Uint32 dzy, Uint32 dsx, Uint32 dsy);
void    MPG_WnDestroy(MpgWn mpgwn);
void    MPG_WnSetOutputMode(MpgWn mpgwn, Sint32 omode, void *buf);
void    MPG_WnDisp(MpgWn mpgwn, Bool sw);
void    MPG_WnSetLoc(MpgWn mpgwn, Uint32 dzx, Uint32 dzy);
void    MPG_WnSetSize(MpgWn mpgwn, Uint32 dsx, Uint32 dsy);
void    MPG_WnSetDispOfst(MpgWn mpgwn, Sint32 dlx, Sint32 dly);
void    MPG_WnSetDispRatio(MpgWn mpgwn, Uint32 x_rat, Uint32 y_rat);
void    MPG_WnSetFrmLoc(MpgWn mpgwn, Uint32 fzx, Uint32 fzy);
Sint32  MPG_WnTrans(MpgWn mpgwn);
void    MPG_WnSetBcolor(MpgWn mpgwn, Uint16 clr);
void    MPG_WnSetYcRatio(MpgWn mpgwn, Uint32 y_rat, Uint32 c_rat);
void    MPG_WnSetIntpol(MpgWn mpgwn,
                        Bool sw_yh, Bool sw_yv, Bool sw_ch, Bool sw_cv);
void    MPG_WnSetMozaic(MpgWn mpgwn, Sint32 moz_h, Sint32 moz_v);
void    MPG_WnSetSoft(MpgWn mpgwn, Bool sof_h, Bool sof_v);
void    MPG_WnSetLumiKey(MpgWn mpgwn, Sint32 y_lvl, Bool bdr);
void    MPG_WnEntryTrFunc(MpgWn mpgwn, MpgTrFunc trfunc);

#endif  /* ifndef SEGA_MPG_H */
