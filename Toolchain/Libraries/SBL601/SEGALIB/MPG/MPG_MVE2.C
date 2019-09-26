/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : 動画再生モジュールソースファイル２
 *  File   : mpg_mve2.c
 *  Date   : 1996-01-31
 *  Author : M.Oshimi, H.Take
 *
 *******************************************************************/

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        <machine.h>
#include        <string.h>
#include        "mpg_loc.h"

/*******************************************************************
 *      定数マクロ
 *******************************************************************/

/* 単位時間のピクチャ数 */
#define SEC_PICS        30              /* １秒のピクチャ数 */
#define MIN_PICS        (60 * SEC_PICS) /* １分のピクチャ数 */
#define HOUR_PICS       (60 * MIN_PICS) /* １時間のピクチャ数 */

/* MPEG A/Vストリームの識別用定数 */
#define SID_V           0               /* ビデオ */
#define SID_A           1               /* オーディオ */

/* 動画再生の停止処理のタイムアウトカウンタ（約１秒） */
#define MVABT_CNT       200

/* 割り込みマスク */
#define IR_MASK         15

/* オーディオの停止確認のタイムアウトカウンタ */
#define ASTOP_CNT       50

/*******************************************************************
*       列挙定数
*******************************************************************/

/* MPEGストリームの設定ID */
enum setstmId {
    SETSTM_CN_V = 0,    /* ビデオチャネル */
    SETSTM_CN_A = 1,    /* オーディオチャネル */
    SETSTM_ID_V = 2,    /* ビデオのストリームＩＤ */
    SETSTM_ID_A = 3     /* オーディオのストリームＩＤ */
};

/*******************************************************************
 *      処理マクロ
 *******************************************************************/

/* 時／分／秒／ピクチャから合計ピクチャ数を求める */
#define GET_TOTAL(h, m, s, p)   ((((h) * 60 + (m)) * 60 + (s)) * 30 + (p))

/* PTSをミリ秒単位に変換する */
#define CONV_PTS(pts)           ((pts) / 90)

/*******************************************************************
 *      関数宣言
 *******************************************************************/

static void     setCurStm(MpgMv mpgmv, Sint32 para, Sint32 type);
static Sint32   setStm(MpgMv mpgmv, Sint32 next);
static void     setStmSub(CdcMpStm *sptr, Sint32 ch, Sint32 id);
static Sint32   execMvStart(MpgMv mpgmv, Bool picsch);
static Sint32   setMvCond(MpgMv mpgmv);
static Sint32   setMovie(MpgMv mpgmv);
static Sint32   setConNext(MpgMv mpgmv);
static Sint32   checkAudioStat(MpgMv mpgmv);
static Sint32   chgMpHn(MpgMv mpgmv);
static void     killMovie(MpgMv mpgmv);
static Sint32   checkVideoStat(MpgMv mpgmv);
static void     get1stTc(MpgMv mpgmv, Sint32 *fpic);
static Sint32   getTc(void);
static void     decompTc(Sint32 total,
                         Sint32 *h, Sint32 *m, Sint32 *s, Sint32 *p);
static Sint32   chkVideo(MpgMv mpgmv, Sint32 fpic);
static Sint32   checkAvStat(MpgMv mpgmv);
static Sint32   setConCur(MpgMv mpgmv, Bool picsch);
static Sint32   execMvStop(MpgMv mpgmv);
static Sint32   stopFunc(MpgMv mpgmv, Sint32 avsw);
static Sint32   cancelNext(CdcMpCon *mpcon_a, CdcMpCon *mpcon_v);
static Sint32   doStop(MpgMv mpgmv,
                       Sint32 avsw, CdcMpCon *acptr, CdcMpCon *vcptr);
static Sint32   abortDecode(MpgMv mpgmv,
                            Sint32 avsw, CdcMpCon *acptr, CdcMpCon *vcptr);
static Sint32   forceInit(MpgMv mpgmv);
static Sint32   resetVbv(void);
static Sint32   waitAstaStop(void);
static Sint32   doEntryNext(MpgMv mpgmv, MpgMv nextmv,
                            Bool setflg, Bool nxtflg);

/*******************************************************************
 *      変数定義
 *******************************************************************/

/*******************************************************************
 *      関数定義
 *******************************************************************/

/* ビデオチャネルの設定 */
void    MPG_MvSetVideoChan(MpgMv mpgmv, Sint32 vch)
{
    /* 現在のMPEGストリームの設定 */
    setCurStm(mpgmv, vch, SETSTM_CN_V);

    return;
}

/* 現在のMPEGストリームの設定 */
static void     setCurStm(MpgMv mpgmv, Sint32 para, Sint32 type)
{
    Sint32      err;
    Bool        chgflg;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    chgflg = FALSE;
    switch (type) {
    case SETSTM_CN_V:
        if (MPG_MV_VCH(mpgmv) != para) {
            MPG_MV_VCH(mpgmv) = para;
            chgflg = TRUE;
        }
        break;
    case SETSTM_CN_A:
        if (MPG_MV_ACH(mpgmv) != para) {
            MPG_MV_ACH(mpgmv) = para;
            chgflg = TRUE;
        }
        break;
    case SETSTM_ID_V:
        if (MPG_MV_VID(mpgmv) != para) {
            MPG_MV_VID(mpgmv) = para;
            chgflg = TRUE;
        }
        break;
    case SETSTM_ID_A:
        if (MPG_MV_AID(mpgmv) != para) {
            MPG_MV_AID(mpgmv) = para;
            chgflg = TRUE;
        }
        break;
    default:
        break;
    }
    if (chgflg) {
        err = setStm(mpgmv, MPG_STM_CUR);
    }

    MPSYS_SemaOut(err);

    return;
}

/* MPEGストリーム（ストリームIDとチャネル）の設定 */
static Sint32   setStm(MpgMv mpgmv, Sint32 next)
{
    Sint32      ret;
    CdcMpStm    mpstm_a, mpstm_v;

    /* MPEGストリームのパラメータ設定（オーディオ） */
    setStmSub(&mpstm_a, MPG_MV_ACH(mpgmv), MPG_MV_AID(mpgmv));

    /* MPEGストリームのパラメータ設定（ビデオ） */
    setStmSub(&mpstm_v, MPG_MV_VCH(mpgmv), MPG_MV_VID(mpgmv));

    /* MPEGストリームの設定関数実行 */
    ret = MPSYS_DoSetStm(next, &mpstm_a, &mpstm_v);

    return ret;
}

/* MPEGストリームのパラメータ設定 */
static void     setStmSub(CdcMpStm *sptr, Sint32 ch, Sint32 id)
{
    /* パラメータの初期化 */
    CDC_MPSTM_SMOD(sptr) = 0;
    CDC_MPSTM_CN(sptr) = 0;
    CDC_MPSTM_ID(sptr) = 0;

    /* チャネルの設定 */
    if (ch != MPG_IGNORE) {
        CDC_MPSTM_SMOD(sptr) |= (CDC_MPSMOD_CNSET | CDC_MPSMOD_CNIDF);
        CDC_MPSTM_CN(sptr) = ch;
    }

    /* ID の設定 */
    if (id != MPG_IGNORE) {
        CDC_MPSTM_SMOD(sptr) |= (CDC_MPSMOD_SNSET | CDC_MPSMOD_SNIDF);
        CDC_MPSTM_ID(sptr) = id;
    }
    
    return;
}

/* オーディオチャネルの設定 */
void    MPG_MvSetAudioChan(MpgMv mpgmv, Sint32 ach)
{
    /* 現在のMPEGストリームの設定 */
    setCurStm(mpgmv, ach, SETSTM_CN_A);

    return;
}

/* ビデオのストリームＩＤの設定 */
void    MPG_MvSetVideoId(MpgMv mpgmv, Sint32 vid)
{
    /* 現在のMPEGストリームの設定 */
    setCurStm(mpgmv, vid, SETSTM_ID_V);

    return;
}

/* オーディオのストリームＩＤの設定 */
void    MPG_MvSetAudioId(MpgMv mpgmv, Sint32 aid)
{
    /* 現在のMPEGストリームの設定 */
    setCurStm(mpgmv, aid, SETSTM_ID_A);

    return;
}

/* 動画再生の開始 */
Sint32  MPG_MvStart(MpgMv mpgmv, Bool picsch)
{
    Sint32      ret;
    
    MPSYS_SemaIn(TRUE);
    
    /* 動画再生の開始実行 */
    ret = execMvStart(mpgmv, picsch);

    return MPSYS_SemaOut(ret);
}

/* 動画再生の開始実行 */
static Sint32   execMvStart(MpgMv mpgmv, Bool picsch)
{
    Sint32      ret;
    Bool        vflg, aflg;

    vflg = (MPSYS_ReqVideoCon(MPG_STM_CUR) != CDC_NUL_SEL);
    aflg = (MPSYS_ReqAudioCon(MPG_STM_CUR) != CDC_NUL_SEL);

    /* 他のストリームが再生中か否かのチェック */
    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_V:
        if (vflg) {
            return MPG_ERR_PLAYING;
        }
        break;
    case MPG_PMODE_A:
        if (aflg) {
            return MPG_ERR_PLAYING;
        }
        break;
    case MPG_PMODE_AV:
        if (vflg || aflg) {
            return MPG_ERR_PLAYING;
        }
        break;
    default:
        return MPG_ERR_PLAYING;
    }

    if ((MPG_MV_ACTA(mpgmv) == OFF) && (MPG_MV_ACTV(mpgmv) == OFF)) {
        /* MPEG動作モードの設定関数実行 */
        ret = MPSYS_DoSetMode(MPG_NOCHG, MPG_MV_DECTMG(mpgmv),
                              MPG_MV_OUTDST(mpgmv), MPG_NOCHG);
        if (ret != MPG_ERR_OK) {
            return ret;
        }

        /* 再生条件の設定 */
        if ((ret = setMvCond(mpgmv)) != MPG_ERR_OK) {
            return ret;
        }

        /* 再生の設定 */
        if ((ret = setMovie(mpgmv)) != MPG_ERR_OK) {
            return ret;
        }
    }
    /* CDブロックへの現在のストリームの設定 */
    if (setConCur(mpgmv, picsch) != MPG_ERR_OK) {
        return MPG_ERR_MCON;
    }
    return MPG_ERR_OK;
}

/* 再生条件の設定 */
static Sint32   setMvCond(MpgMv mpgmv)
{
    Sint32      ret;
    Sint32      omode;
    Sint32      pau;
    Sint32      plymod, tmod_v;
    MpgWn       mpgwn;

    /* 再生状態になったら画像出力開始 */
    if (MPG_MvGetVideoStat(mpgmv) == MPG_VSTAT_PLAY) {
        mpgwn = MPG_MV_MPGWN(mpgmv, MPL_FBN_MV);
        omode = MPG_MV_OUTDST(mpgmv);
        if ((mpgwn != NULL) && (omode == MPG_OUTDST_HOST)) {
            MPWIN_OutputAll(mpgwn, omode);
        }
    }

    /* デコード方法の設定 */
    pau = MPG_MV_PAUTIM(mpgmv);
    if ((MPSYS_IsPauseVideo(&ret)) && (pau == 0x0000)) {
        pau = MPG_IGNORE;
    }
    if (ret != MPG_ERR_OK) {
        return ret;
    }
    /* デコード方法の設定関数実行 */
    ret = MPSYS_DoSetDec(MPG_MV_AMUTE(mpgmv), pau, MPG_MV_FRZTIM(mpgmv));
    if (ret != MPG_ERR_OK) {
        return ret;
    }
    /* MPEGの現在のポーズタイミングの設定 */
    MPSYS_SetPauTim(pau);
    
    /* MPEGの再生関数実行 */
    if (MPG_MV_PMODE(mpgmv) == MPG_PMODE_AV) {
        plymod = MPG_AV_SYNC;
    } else {
        plymod = MPG_AV_ASYNC;
    }
    if (MPG_GetVideoMode() == MPG_VMODE_MOVIE) {
        tmod_v = CDC_MPTRN_AUTO;        /* 自動転送 */
    } else {
        tmod_v = CDC_MPTRN_FORCE;       /* 強制転送 */
    }
    ret = MPSYS_DoPlay(plymod, MPG_IGNORE, tmod_v, MPG_IGNORE);
    if (ret != MPG_ERR_OK) {
        return ret;
    }
    
    ret = setStm(mpgmv, MPG_STM_CUR);
    
    return ret;
}

/* 再生の設定 */
/* ACTAとACTVは必ずOFFの状態で呼ばれる */
static Sint32   setMovie(MpgMv mpgmv)
{
    Sint32      ret;
    MpgMv       nextmv;
    MplEventFunc        eventfunc;

    if (MPG_MV_USEDFLAG(mpgmv) == FALSE) {
        return MPG_ERR_ILLHDL;
    }

    nextmv = MPG_MV_NEXTMV(mpgmv);
    if (nextmv != NULL) {
        /* 次に再生する動画ハンドルが消去されていたらエラー */
        if (MPG_MV_USEDFLAG(nextmv) == FALSE) {
            return MPG_ERR_ILLHDL;
        }
        /* 次のMPEGストリームの登録 */
        if ((ret = setStm(nextmv, MPG_STM_NEXT)) != MPG_ERR_OK) {
            return ret;
        }

        /* 次の接続先の登録 */
        if ((ret = setConNext(nextmv)) != MPG_ERR_OK) {
            return ret;
        }
    }
    
    /* イベント起動関数の登録 */
    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_A:
        eventfunc = (MplEventFunc)checkAudioStat;
        MPG_MV_ACTA(mpgmv) = ON;
        break;
    case MPG_PMODE_V:
        eventfunc = (MplEventFunc)checkVideoStat;
        MPG_MV_ACTV(mpgmv) = ON;
        break;
    default:
        eventfunc = (MplEventFunc)checkAvStat;
        MPG_MV_ACTA(mpgmv) = ON;
        MPG_MV_ACTV(mpgmv) = ON;
        break;
    }
    MPSYS_EntryEventFunc(eventfunc, mpgmv);
    MPG_MV_ISMVEND(mpgmv) = FALSE;
    
    return MPG_ERR_OK;
}

/* CDブロックへの次のストリームの設定 */
static Sint32   setConNext(MpgMv mpgmv)
{
    Sint32      ret;
    Sint32      cmod_a, lay_a, bn_a;
    Sint32      cmod_v, lay_v, bn_v;
    CdcMpCon    mpcon_a, mpcon_v, *acptr, *vcptr;

    /* 初期値の設定 */
    cmod_a = cmod_v = MPG_IGNORE;
    lay_a = lay_v = MPG_LAY_SYS;
    bn_a = bn_v = MPG_NOCDB;

    if (mpgmv == NULL) {
        /* 次ストリーム取消の場合 */
        cmod_a = 0x00;
        cmod_v = 0x00;
    } else {
        if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_A) {
            /* VideoのみまたはAV同期再生の場合 */
            cmod_v = (CDC_MPCMOD_DEL | MPG_MV_VCND(mpgmv));
            lay_v = (MPG_MV_VLAY(mpgmv) | CDC_MPSRCH_OFF);
            bn_v = MPG_MV_VBNO(mpgmv);
        }
        if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_V) {
            /* AudioのみまたはAV同期再生の場合 */
            cmod_a = (CDC_MPCMOD_DEL | MPG_MV_ACND(mpgmv));
            lay_a = MPG_MV_ALAY(mpgmv);
            bn_a = MPG_MV_ABNO(mpgmv);
        }
    }

    /* 値の設定 */
    acptr = &mpcon_a;
    vcptr = &mpcon_v;

    CDC_MPCON_CMOD(acptr) = cmod_a;
    CDC_MPCON_LAY(acptr) = lay_a;
    CDC_MPCON_BN(acptr) = bn_a;
    CDC_MPCON_CMOD(vcptr) = cmod_v;
    CDC_MPCON_LAY(vcptr) = lay_v;
    CDC_MPCON_BN(vcptr) = bn_v;

    /* MPEGデコーダの接続先の設定関数実行(ソフトピクチャサーチは無効) */
    ret = MPSYS_DoSetCon(CDC_MPSTF_NEXT, acptr, vcptr);

    return ret;
}

/* オーディオのステータスのチェック */
static Sint32   checkAudioStat(MpgMv mpgmv)
{
    Sint32      ret;

    /* 現在の接続先のチェック */
    if (MPSYS_ReqAudioCon(MPG_STM_CUR) != MPG_MV_ABNO(mpgmv)) {
        MPG_MV_ACTA(mpgmv) = OFF;
    }
    
    /* MPEGハンドルの停止と切替え処理 */
    ret = chgMpHn(mpgmv);

    return ret;
}

/* MPEGハンドルの停止と切替え処理 */
static Sint32   chgMpHn(MpgMv mpgmv)
{
    Sint32      ret;
    MpgMv       nextmv;

    ret = MPG_ERR_OK;
    if ((MPG_MV_ACTA(mpgmv) == OFF) && (MPG_MV_ACTV(mpgmv) == OFF)) {
        /* MPEG動画ハンドルの動作停止処理 */
        killMovie(mpgmv);
        nextmv = MPG_MV_NEXTMV(mpgmv);
        if (nextmv != NULL) {
            ret = setMovie(nextmv);
        }
    }
    return ret;
}

/* MPEG動画ハンドルの動作停止処理 */
static void     killMovie(MpgMv mpgmv)
{
    MPSYS_KillEventFunc();
    MPG_MV_ISMVEND(mpgmv) = TRUE;
    
    return;
}

/* ビデオのステータスのチェック */
static Sint32   checkVideoStat(MpgMv mpgmv)
{
    Sint32      ret;
    Sint32      fpic;
    Sint32      bn;
    
    /* 最初の絵ならタイムコードを取得する */
    get1stTc(mpgmv, &fpic);
    
    /* ビデオのチェック */
    if ((ret = chkVideo(mpgmv, fpic)) != MPG_ERR_OK) {
        return ret;
    }
    
    /* MPEGハンドルの停止と切替え処理 */
    ret = chgMpHn(mpgmv);

    return ret;
}

/* 最初の絵ならタイムコードを取得する */
static void     get1stTc(MpgMv mpgmv, Sint32 *fpic)
{
    Sint32      fpc;
    CdcMpStat   mpstat;
    
    /* 最初の絵フラグの検出 */
    MPSYS_GetMpgStat(&mpstat);
    fpc = (CDC_MPSTAT_STV(&mpstat) & CDC_MPSTV_1STPIC);
    *fpic = fpc;
    
    /* 最初の絵なら絶対タイムコード取得 */
    if ((fpc != 0) && (MPG_MV_TCFLAG(mpgmv) == ON)) {
        MPG_MV_FTC(mpgmv) = getTc();
    }
    return;
}

/* タイムコードの取得 */
static Sint32   getTc(void)
{
    Sint32      bnk, pictyp, tr;
    CdcMpTc     mptcwk, *mptc;
    Sint32      total;
    Sint32      h1, m1, s1, p1;
    Sint32      h2, m2, s2, p2;

    /* ビデオのタイムコードの取得関数実行 */
    mptc = &mptcwk;
    if (MPSYS_DoGetTc(&bnk, &pictyp, &tr, mptc) != MPG_ERR_OK) {
        MPSYS_SetErrCode(MPG_ERR_GETTC);
    }

    /* テンポラルリファレンスからタイムコードへの変換 */
    h1 = CDC_MPTC_HOUR(mptc);
    m1 = CDC_MPTC_MIN(mptc);
    s1 = CDC_MPTC_SEC(mptc);
    p1 = CDC_MPTC_PIC(mptc);
    total = GET_TOTAL(h1, m1, s1, p1) + tr;

    /* タイムコードの分解 */
    decompTc(total, &h2, &m2, &s2, &p2);

    /* フレーム落ちの修正 */
    total = total + (m2 - m1)*2 - (m2/10 - m1/10)*2;

    return total;
}

/* タイムコードの分解 */
static void     decompTc(Sint32 total,
                         Sint32 *h, Sint32 *m, Sint32 *s, Sint32 *p)
{
    /* 時間の計算 */
    *h = total / HOUR_PICS;
    total -= ((*h) * HOUR_PICS);

    /* 分の計算 */
    *m = total / MIN_PICS;
    total -= ((*m) * MIN_PICS);

    /* 秒の計算 */
    *s = total / SEC_PICS;
    total -= ((*s) * SEC_PICS);

    /* ピクチャの計算 */
    *p = total;

    return;
}

/* ビデオのチェック */
static Sint32   chkVideo(MpgMv mpgmv, Sint32 fpic)
{
    Sint32      ret;
    Sint32      bn;

    ret = MPG_ERR_OK;

    /* 現在の接続先のチェック */
    if ((bn = MPSYS_ReqVideoCon(MPG_STM_CUR)) != MPG_MV_VBNO(mpgmv)) {
        if (bn == CDC_NUL_SEL) {
            /* VBVエンプティチェック */
            if (MPSYS_IsVbvEmpty(&ret)) {
                MPG_MV_ACTV(mpgmv) = OFF;
            }
        } else {
            /* １枚目の絵フラグをチェック */
            if (fpic != 0) {
                MPG_MV_ACTV(mpgmv) = OFF;
            }
        }
    }
    return ret;
}

/* オーディオとビデオのステータスのチェック */
static Sint32   checkAvStat(MpgMv mpgmv)
{
    Sint32      ret;
    Sint32      fpic;
    Sint32      bn;
    
    /* 最初の絵ならタイムコードを取得する */
    get1stTc(mpgmv, &fpic);
    
    /* オーディオのチェック */
    if ((bn = MPSYS_ReqAudioCon(MPG_STM_CUR)) != MPG_MV_ABNO(mpgmv)) {
        MPG_MV_ACTA(mpgmv) = OFF;
    }
    
    /* ビデオのチェック */
    if ((ret = chkVideo(mpgmv, fpic)) != MPG_ERR_OK) {
        return ret;
    }
    
    /* MPEGハンドルの停止と切替え処理 */
    ret = chgMpHn(mpgmv);

    return ret;
}

/* CDブロックへの現在のストリームの設定 */
static Sint32   setConCur(MpgMv mpgmv, Bool picsch)
{
    Bool        vflg, aflg;
    Sint32      ret;
    Sint32      cmod_a, lay_a, bn_a;
    Sint32      cmod_v, lay_v, bn_v;
    CdcMpCon    mpcon_a, mpcon_v, *acptr, *vcptr;
    
    /* 初期値の設定 */
    vflg = aflg = TRUE;
    ret = MPG_ERR_NG;
    cmod_a = cmod_v = MPG_IGNORE;
    lay_a = lay_v = MPG_LAY_SYS;
    bn_a = bn_v = MPG_NOCDB;

    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_A) {
        /* VideoのみまたはAV同期再生の場合 */
        vflg = (MPSYS_ReqVideoCon(MPG_STM_CUR) == CDC_NUL_SEL);

        cmod_v = (CDC_MPCMOD_DEL | CDC_MPCMOD_VWCLR | MPG_MV_VCND(mpgmv));
        lay_v = MPG_MV_VLAY(mpgmv);
        if (picsch == ON) {
            if (MPG_MV_PMODE(mpgmv) == MPG_PMODE_V) {
                lay_v |= CDC_MPSRCH_VIDEO;
            } else {
                lay_v |= CDC_MPSRCH_AV;
            }
        }
        bn_v = MPG_MV_VBNO(mpgmv);
    }
    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_V) {
        /* AudioのみまたはAV同期再生の場合 */
        aflg = (MPSYS_ReqAudioCon(MPG_STM_CUR) == CDC_NUL_SEL);

        cmod_a = (CDC_MPCMOD_DEL | MPG_MV_ACND(mpgmv));
        lay_a = MPG_MV_ALAY(mpgmv);
        bn_a = MPG_MV_ABNO(mpgmv);
    }
    if (vflg && aflg) {
        acptr = &mpcon_a;
        vcptr = &mpcon_v;

        CDC_MPCON_CMOD(acptr) = cmod_a;
        CDC_MPCON_LAY(acptr) = lay_a;
        CDC_MPCON_BN(acptr) = bn_a;
        CDC_MPCON_CMOD(vcptr) = cmod_v;
        CDC_MPCON_LAY(vcptr) = lay_v;
        CDC_MPCON_BN(vcptr) = bn_v;

        /* MPEGデコーダの接続先の設定関数実行 */
        ret = MPSYS_DoSetCon(CDC_MPSTF_CUR, acptr, vcptr);
    }
    return ret;
}

/* 動画再生の停止 */
Sint32  MPG_MvStop(MpgMv mpgmv)
{
    Sint32      ret;
    
    MPSYS_SemaIn(TRUE);
    
    /* 動画再生の停止実行 */
    ret = execMvStop(mpgmv);

    return MPSYS_SemaOut(ret);
}

/* 動画再生の停止実行 */
static Sint32   execMvStop(MpgMv mpgmv)
{
    Sint32      ret;

    ret = MPG_ERR_OK;
    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_AV:	/* オーディオを先に停止させる */
    case MPG_PMODE_A:
        ret = stopFunc(mpgmv, SID_A);
        break;
    }
    if (ret != MPG_ERR_OK) {
        return ret;
    }

    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_AV:
        /* オーディオの停止確認 */
        if ((ret = waitAstaStop()) != MPG_ERR_OK) {
            return ret;
        }
        /* nobreak; */
    case MPG_PMODE_V:
        ret = stopFunc(mpgmv, SID_V);
        break;
    }
    return ret;
}

/* MPEG A/Vストリームの停止 */
static Sint32   stopFunc(MpgMv mpgmv, Sint32 avsw)
{
    CdcMpCon    acon, vcon;
    CdcMpCon    *acptr, *vcptr;
    Sint32      ret;

    acptr = &acon;
    vcptr = &vcon;

    CDC_MPCON_LAY(acptr) = CDC_MPCON_LAY(vcptr) = MPG_LAY_SYS;
    CDC_MPCON_BN(acptr) = CDC_MPCON_BN(vcptr) = MPG_NOCDB;

    if (avsw == SID_A) {   /* オーディオ停止 */
        if (MPSYS_ReqAudioCon(MPG_STM_CUR) != MPG_MV_ABNO(mpgmv)) {
            return MPG_ERR_OK;
        }
        CDC_MPCON_CMOD(acptr) = 0x00;
        CDC_MPCON_CMOD(vcptr) = 0xff;
    } else {            /* ビデオ停止 */
        if (MPSYS_ReqVideoCon(MPG_STM_CUR) != MPG_MV_VBNO(mpgmv)) {
            return MPG_ERR_OK;
        }
        CDC_MPCON_CMOD(acptr) = 0xff;
        CDC_MPCON_CMOD(vcptr) = 0x00;
    }

    /* 次発ストリームの取消 */
    ret = cancelNext(acptr, vcptr);
    if (ret != MPG_ERR_OK) {
        return ret;
    }

    ret = doStop(mpgmv, avsw, acptr, vcptr);

    return ret;
}

/* 次発ストリームの取消 */
static Sint32   cancelNext(CdcMpCon *mpcon_a, CdcMpCon *mpcon_v)
{
    /* MPEGデコーダの接続先の設定関数実行(設定の取消) */
    return MPSYS_DoSetCon(CDC_MPSTF_NEXT, mpcon_a, mpcon_v);
}

/* 現在のストリームの停止 */
static Sint32   doStop(MpgMv mpgmv,
                       Sint32 avsw, CdcMpCon *acptr, CdcMpCon *vcptr)
{
    Sint32      ret;
    Sint32      cnt;

    ret = MPG_ERR_OK;

    cnt = 0;
    while (TRUE) {
        /* デコードの停止 */
        ret = abortDecode(mpgmv, avsw, acptr, vcptr);
        if (ret != MPG_ERR_WAIT) {
            break;
        }
        if (cnt++ > MVABT_CNT) {
            return MPG_ERR_TMOUT;
        }
    }

    if ((ret == MPG_ERR_OK) && (avsw == SID_V)) {
        /* VBVリセット(VLDもリセットする) */
        ret = resetVbv();
    }

    return ret;
}

/* デコードの停止 */
static Sint32   abortDecode(MpgMv mpgmv,
                            Sint32 avsw, CdcMpCon *acptr, CdcMpCon *vcptr)
{
    Sint32      ret, ast;
    Sint32      chg_a, chg_v;
    CdcMpStat   mpstat, *msptr;

    ret = MPG_ERR_OK;
    msptr = &mpstat;
    /* MPEGステータス情報の取得 */
    if (CDC_MpGetCurStat(msptr) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    if (avsw == SID_A) {
        ast = CDC_MPGET_ASTA(msptr);
    } else {
        ast = CDC_MPGET_ASTV(msptr);
    }

    switch (ast) {
    case CDC_MPASTA_STOP:
    case CDC_MPASTV_STOP:
        break;
    case CDC_MPASTA_PRE1:
    case CDC_MPASTA_PRE2:
    case CDC_MPASTV_PRE1:
    case CDC_MPASTV_PRE2:
        /* MPEGデコーダの接続先の設定関数実行(設定の取消) */
        ret = MPSYS_DoSetCon(CDC_MPSTF_CUR, acptr, vcptr);
        break;
    case CDC_MPASTA_TRNS:
    case CDC_MPASTV_TRNS:
        if (avsw == SID_A) {
            chg_a = CDC_MPCOF_ABT;
            chg_v = MPG_IGNORE;
        } else {
            chg_a = MPG_IGNORE;
            chg_v = CDC_MPCOF_ABT;
        }
        /* MPEGデコーダの接続先の切り替え関数実行 */
        ret = MPSYS_DoChgCon(chg_a, chg_v, 0, 0);
        break;
    case CDC_MPASTA_CHG:
    case CDC_MPASTV_CHG:
        ret = MPG_ERR_CHG;
        break;
    default:
        /* その他（復活処理）の場合は、MPEGデコーダを強制初期化する */
        /* （エラーコードとしてMPG_ERR_RCVを返す） */
        ret = forceInit(mpgmv);
        break;
    }
    return ret;
}

/* MPEGデコーダの強制初期化 */
/* （エラーコードとしてMPG_ERR_RCVを返す） */
static Sint32   forceInit(MpgMv mpgmv)
{
    MpgWn       wnwk;
    Sint32      ret;
    Sint32      intmask;
    Sint32      i;

    /* MPEGデコーダの初期化 */
    ret = MPSYS_InitMpd(ON);
    if (ret != MPG_ERR_OK) {
        return ret;
    }

    /* MPEG動画ハンドルの動作停止処理 */
    killMovie(mpgmv);

    /* 割り込み禁止処理 */
    intmask = get_imask();
    set_imask(IR_MASK);

    /* MPEGシステムの状態取得 */
    MPSYS_CaptSub();

    /* 割り込み禁止解除処理 */
    set_imask(intmask);

    /* MPEGウィンドウのチェック */
    for (i = 0; i < MPL_FRM_NUM; i++) {
        if ((wnwk = MPG_MV_MPGWN(mpgmv, i)) != NULL) {
            if ((MPG_WN_DISPSW(wnwk) == ON) && (MPG_WN_USEFLG(wnwk) == TRUE)) {
                /* 表示・非表示の設定実行 */
                MPWIN_DispSub(wnwk, ON);

                break;
            }
        }
    }

    /* 動画ハンドルの無効化 */
    MPG_MV_ACTV(mpgmv) = OFF;
    MPG_MV_ACTA(mpgmv) = OFF;

    return MPG_ERR_RCV;
}

/* VBVリセット(VLDもリセットする) */
static Sint32   resetVbv(void)
{
    Sint32      r_sw, reg_no, rdata;
    Sint32      rwsw, wdata;
    
    /* MPEG LSI パラメータの取得 */
    r_sw = 0;
    reg_no = 0x1a;
    if (CDC_MpGetLsi(r_sw, reg_no, &rdata) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    
    /* MPEG LSI の設定 */
    rwsw = 0;
    wdata = (rdata & 0xcfff);
    if (CDC_MpSetLsi(rwsw, reg_no, wdata, &rdata) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    return MPG_ERR_OK;
}

/* オーディオの停止確認 */
static Sint32   waitAstaStop(void)
{
    Sint32      ret, cnt;
    CdcMpStat   mpstat;
    
    cnt = 0;
    while (TRUE) {
        ret = CDC_MpGetCurStat(&mpstat);
        if (ret == CDC_ERR_OK) {
            if (CDC_MPGET_ASTA(&mpstat) == CDC_MPASTA_STOP) {
                break;
            }
        }
        /* タイムアウトのチェック */
        if (cnt++ > ASTOP_CNT) {
            return MPG_ERR_NG;
        }
        /* ソフトウエアタイマ */
        MPSYS_SoftTimer(1000);
    }
    return MPG_ERR_OK;
}

/* ビデオ再生の停止 */
Sint32  MPG_MvStopVideo(MpgMv mpgmv)
{
    Sint32      ret;
    
    MPSYS_SemaIn(TRUE);
    
    ret = stopFunc(mpgmv, SID_V);
    
    return MPSYS_SemaOut(ret);
}

/* オーディオ再生の停止 */
Sint32  MPG_MvStopAudio(MpgMv mpgmv)
{
    Sint32      ret;
    
    MPSYS_SemaIn(TRUE);
    
    ret = stopFunc(mpgmv, SID_A);
    
    return MPSYS_SemaOut(ret);
}

/* 動画のタイムコードの取得 */
void    MPG_MvGetTimeCode(MpgMv mpgmv,
                          Sint32 *hh, Sint32 *mm, Sint32 *ss, Sint32 *pp)
{
    Sint32      total;
    CdcMpStat   mpstat;

    MPSYS_SemaIn(FALSE);

    MPSYS_GetMpgStat(&mpstat);
    
    if ((MPG_MV_ACTV(mpgmv) == OFF) ||
        (CDC_MPGET_ASTV(&mpstat) != CDC_MPASTV_TRNS)) {

        /* mpgmvが再生状態でないときは０を返す */
        *hh = *mm = *ss = *pp = 0;
    } else {
        /* タイムコードの取得 */
        total = getTc();
        
        /* 相対タイムコードを返す */
        if (MPG_MV_TCFLAG(mpgmv) == ON) {
            total -= MPG_MV_FTC(mpgmv);
        }

        /* タイムコードの分解 */
        decompTc(total, hh, mm, ss, pp);
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* オーディオの再生時間の取得 */
Sint32  MPG_MvGetPlaytime(MpgMv mpgmv)
{
    Sint32      err;
    Sint32      pts;
    
    MPSYS_SemaIn(FALSE);
    
    pts = 0;
    err = MPG_ERR_OK;
    if (MPG_MV_ACTA(mpgmv) == ON) {
        if (CDC_MpGetPts(&pts) != CDC_ERR_OK) {
            err = MPG_ERR_NG;
        }
    }
    
    MPSYS_SemaOut(err);

    /* msec単位の値を返す */
    return CONV_PTS(pts);
}

/* 次に再生するMPEGハンドルの登録 */
Sint32  MPG_MvEntryNext(MpgMv mpgmv, MpgMv nextmv)
{
    Bool        vflg, aflg, setflg;
    Bool        vnxt, anxt, nxtflg;
    Sint32      ret;

    MPSYS_SemaIn(TRUE);
    
    vflg = (MPSYS_ReqVideoCon(MPG_STM_CUR) == MPG_MV_VBNO(mpgmv));
    aflg = (MPSYS_ReqAudioCon(MPG_STM_CUR) == MPG_MV_ABNO(mpgmv));

    vnxt = FALSE;
    anxt = FALSE;
    if (nextmv != NULL) {
        vnxt = (MPSYS_ReqVideoCon(MPG_STM_NEXT) == MPG_MV_VBNO(nextmv));
        anxt = (MPSYS_ReqAudioCon(MPG_STM_NEXT) == MPG_MV_ABNO(nextmv));
    }

    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_V:
        setflg = vflg;
        nxtflg = vnxt;
        break;
    case MPG_PMODE_A:
        setflg = aflg;
        nxtflg = anxt;
        break;
    default:
        setflg = (vflg && aflg);
        nxtflg = (vnxt && anxt);
        break;
    }

    /* 次発ストリームの登録実行 */
    ret = doEntryNext(mpgmv, nextmv, setflg, nxtflg);

    return MPSYS_SemaOut(ret);
}

/* 次発ストリームの登録実行 */
static Sint32   doEntryNext(MpgMv mpgmv, MpgMv nextmv,
                            Bool setflg, Bool nxtflg)
{
    Sint32      ret;

    if (setflg && nxtflg) {
        /* nextmvが次発に登録済みの場合 */
        MPG_MV_NEXTMV(mpgmv) = nextmv;
        return MPG_ERR_OK;
    }

    /* 先発ストリームが再生中なら次発ストリームの設定を行なう */
    ret = MPG_ERR_ILLSTAT;
    if (setflg) {
        if ((ret = setConNext(nextmv)) != MPG_ERR_OK) {
            ret = MPG_ERR_ENTRYNG;
        }
    }

    switch (ret) {
    case MPG_ERR_OK:
    case MPG_ERR_ILLSTAT:
        MPG_MV_NEXTMV(mpgmv) = nextmv;
        break;
    }

    return ret;
}

/* 相対タイムコードの設定 */
void    MPG_MvAdjustTc(MpgMv mpgmv)
{
    /* 相対タイムコードフラグをONにする */
    MPG_MV_TCFLAG(mpgmv) = ON;
    
    return;
}

/* 再生終了の判定 */
Bool    MPG_MvIsEnd(MpgMv mpgmv)
{
    return MPG_MV_ISMVEND(mpgmv);
}


