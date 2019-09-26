/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : 動画再生モジュールソースファイル
 *  File   : mpg_mve.c
 *  Date   : 1996-01-31
 *  Author : M.Oshimi, H.Take
 *
 *******************************************************************/

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        <string.h>
#include        "mpg_loc.h"

/*******************************************************************
 *      定数マクロ
 *******************************************************************/

/* MPEGハンドルの最大数 */
#define MPG_MV_MAX      16

/* 再生の停止待ちのタイムアウトカウンタ（約１秒） */
#define MSTOP_CNT       7000

/* ピクチャ更新フラグがOFFである */
#define UPDPIC_OFF      0

/*******************************************************************
*       列挙定数
*******************************************************************/

/*******************************************************************
 *      処理マクロ
 *******************************************************************/

/*******************************************************************
 *      関数宣言
 *******************************************************************/

static MpgMv    createNew(void);
static Sint32   getCdbno(StmHn stm);
static void     initMpHn(MpgMv mpgmv);
static Sint32   waitMovieStop(MpgMv mpgmv);
static void     deleteMpgmv(MpgMv mpgmv);
static Sint32   getVideoStatSub(MpgMv mpgmv, Sint32 *err);
static Sint32   getAudioStatSub(MpgMv mpgmv, Sint32 *err);
static Sint32   outputSub(MpgMv mpgmv, Bool chkupic);

/*******************************************************************
 *      変数定義
 *******************************************************************/

/* MPEGビデオハンドル */
static MpgMovie mp_movie[MPG_MV_MAX];

/*******************************************************************
 *      関数定義
 *******************************************************************/

/* MPEG動画ハンドルの生成 */
MpgMv   MPG_MvCreate(StmHn vstm, StmHn astm)
{
    MpgMv       mpgmv;

    if ((vstm == NULL) && (astm == NULL)) {
        MPSYS_SetErrCode(MPG_ERR_CREATE);
        return NULL;
    }
    if ((mpgmv = createNew()) == NULL) {
        MPSYS_SetErrCode(MPG_ERR_CREATE);
        return NULL;
    }

    /* バッファ区画番号の設定 */
    MPG_MV_PMODE(mpgmv) = MPG_PMODE_AV;	/* AV 同期再生モード */
    MPG_MV_VBNO(mpgmv) = MPG_NOCDB;
    MPG_MV_ABNO(mpgmv) = MPG_NOCDB;
    MPG_MV_VLAY(mpgmv) = MPG_IGNORE;
    MPG_MV_ALAY(mpgmv) = MPG_IGNORE;
    if (vstm != NULL) {
        if (astm == NULL) {
            MPG_MV_PMODE(mpgmv) = MPG_PMODE_V;	/* ビデオ再生モード */
        }
        MPG_MV_VBNO(mpgmv) = getCdbno(vstm);
        MPG_MV_VLAY(mpgmv) = MPG_LAY_SYS;
    }
    if (astm != NULL) {
        if (vstm == NULL) {
            MPG_MV_PMODE(mpgmv) = MPG_PMODE_A;	/* オーディオ再生モード */
        }
        MPG_MV_ABNO(mpgmv) = getCdbno(astm);
        MPG_MV_ALAY(mpgmv) = MPG_LAY_SYS;
    }

    /* MPEGハンドルの初期化 */
    initMpHn(mpgmv);

    return mpgmv;
}

/* MPEGハンドルオブジェクトの生成 */
static MpgMv    createNew(void)
{
    Sint32      i;
    MpgMovie    *mvptr;
    
    for (i = 0; i < MPG_MV_MAX; i++) {
        mvptr = &mp_movie[i];
        if (MPG_MV_USEDFLAG(mvptr) == FALSE) {
            MPG_MV_USEDFLAG(mvptr) = TRUE;
            return mvptr;
        }
    }
    return NULL;
}

/* バッファ区画番号の取得 */
static Sint32   getCdbno(StmHn stm)
{
    StmFrange   range;
    StmKey      stmkey;
    Sint32      fid, bn;

    STM_GetInfo(stm, &fid, &range, &bn, &stmkey);
    
    return bn;
}

/* MPEGハンドルの初期化 */
static void     initMpHn(MpgMv mpgmv)
{
    Sint32      i;

    /* 次に再生するMPEG動画 */
    MPG_MV_NEXTMV(mpgmv) = NULL;

    /* 再生動作中フラグ */
    MPG_MV_ACTA(mpgmv) = OFF;
    MPG_MV_ACTV(mpgmv) = OFF;
        
    /* 終了条件の設定 */
    MPG_MV_VCND(mpgmv) = MPG_TCND_SEC;
    MPG_MV_ACND(mpgmv) = MPG_TCND_SEC;
        
    /* 再生終了フラグ */
    MPG_MV_ISMVEND(mpgmv) = FALSE;
        
    /* 相対タイムコードフラグ */
    MPG_MV_TCFLAG(mpgmv) = OFF;

    /* 出力先の設定 */
    MPG_MV_OUTDST(mpgmv) = MPG_OUTDST_DRCT;

    /* デコードタイミングの設定 */
    MPG_MV_DECTMG(mpgmv) = MPG_DECTMG_VSYNC;

    /* チャネル番号の設定 */
    MPG_MV_ACH(mpgmv) = MPG_IGNORE;
    MPG_MV_VCH(mpgmv) = MPG_IGNORE;

    /* ストリームIDの設定 */
    MPG_MV_AID(mpgmv) = MPG_IGNORE;
    MPG_MV_VID(mpgmv) = MPG_IGNORE;

    /* ポーズの設定 */
    MPG_MV_PAUTIM(mpgmv) = 1;

    /* フリーズの設定 */
    MPG_MV_FRZTIM(mpgmv) = 1;

    /* オーディオのミュートの設定 */
    MPG_MV_AMUTE(mpgmv) = 0x04;

    /* 最初の絵のタイムコードの設定 */
    MPG_MV_FTC(mpgmv) = -1;

    /* ディスプレイウィンドウの設定 */
    for (i = 0; i < MPL_FRM_NUM; i++) {
        MPG_MV_MPGWN(mpgmv, i) = NULL;
    }

    /* ビデオストリームの消去モード */
    MPG_MV_CLRV(mpgmv) = MPG_VCLR_ALL;

    /* オーディオストリームの消去モード */
    MPG_MV_CLRA(mpgmv) = MPG_ACLR_ALL;

    return;
}

/* MPEG動画ハンドルの消去 */
Sint32  MPG_MvDestroy(MpgMv mpgmv)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);
    
    /* 再生の停止 */
    ret = MPG_MvStop(mpgmv);

    /* 再生の停止待ち */
    /* 再生の停止関数実行結果にかかわらず、停止待ちを実行する */
    /* 正常に停止した場合は、再生の停止関数の返り値を保持する */
    if (waitMovieStop(mpgmv) == MPG_ERR_OK) {
        deleteMpgmv(mpgmv);
    } else {
        ret = MPG_ERR_DESTROY;
    }

    return MPSYS_SemaOut(ret);
}

/* 再生の停止待ち */
static Sint32   waitMovieStop(MpgMv mpgmv)
{
    Sint32      cnt;

    cnt = 0;

    while (TRUE) {
        /* エラー時はSTOPを返すので、ハンドル消去は実行する */
        /* ただし、関数内で設定したエラーコードは無効となる可能性がある */
        if ((MPG_MvGetVideoStat(mpgmv) == MPG_VSTAT_STOP) &&
            (MPG_MvGetAudioStat(mpgmv) == MPG_ASTAT_STOP)) {
            break;
        }

        /* タイムアウトのチェック */
        if (cnt++ > MSTOP_CNT) {
            return MPG_ERR_NG;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGハンドルオブジェクトの消去 */
static void     deleteMpgmv(MpgMv mpgmv)
{
    if (mpgmv != NULL) {
        MPG_MV_USEDFLAG(mpgmv) = FALSE;
    }
    /* MPEGハンドルオブジェクトの０クリア */
    memset(mpgmv, 0, sizeof(MpgMovie));
    
    return;
}

/* 動画の終了条件の設定 */
void    MPG_MvSetTermCond(MpgMv mpgmv, Sint32 vcnd, Sint32 acnd)
{
    if (vcnd != MPG_IGNORE) {
        MPG_MV_VCND(mpgmv) = vcnd;
    }
    if (acnd != MPG_IGNORE) {
        MPG_MV_ACND(mpgmv) = acnd;
    }
    return;
}

/* 動画の出力先の設定 */
void    MPG_MvSetVideoDest(MpgMv mpgmv, Sint32 dst)
{
    MPG_MV_OUTDST(mpgmv) = dst;

    return;
}

/* デコードタイミングの設定 */
void    MPG_MvSetDecTim(MpgMv mpgmv, Sint32 tmg)
{
    if (MPG_MV_PMODE(mpgmv) == MPG_PMODE_V) {
        MPG_MV_DECTMG(mpgmv) = tmg;
    }
    return;
}

/* 一時停止／コマ送り／スロー再生 */
void    MPG_MvPause(MpgMv mpgmv, Sint32 cmd, Sint32 itvl)
{
    Sint32      err;
    Sint32      pau;
    
    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_A) {
        switch (cmd) {
        case MPG_PAUSE_ON:
            pau = 0;
            break;
        case MPG_PAUSE_OFF:
            pau = 1;
            break;
        default:
            pau = itvl;
            break;
        }
        MPG_MV_PAUTIM(mpgmv) = pau;

        if (MPG_MV_ACTV(mpgmv) == ON) {
            /* デコード方法の設定関数実行 */
            err = MPSYS_DoSetDec(MPG_IGNORE, pau, MPG_IGNORE);
            if (err == MPG_ERR_OK) {
                /* MPEGの現在のポーズタイミングの設定 */
                MPSYS_SetPauTim(pau);
            }
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* フリーズ／ストロボ再生 */
void    MPG_MvFreeze(MpgMv mpgmv, Sint32 cmd, Sint32 itvl)
{
    Sint32      err;
    Sint32      frz;

    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_A) {
        switch (cmd) {
        case MPG_FREEZE_ON:
            frz = 0;
            break;
        case MPG_FREEZE_OFF:
            frz = 1;
            break;
        default:
            frz = itvl;
            break;
        }
        MPG_MV_FRZTIM(mpgmv) = frz;

        if (MPG_MV_ACTV(mpgmv) == ON) {
            /* デコード方法の設定関数実行 */
            err = MPSYS_DoSetDec(MPG_IGNORE, MPG_IGNORE, frz);
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* 音声の左右チャンネルの設定 */
void    MPG_MvSetPan(MpgMv mpgmv, Bool lsw, Bool rsw)
{
    Sint32      err;
    Sint32      mute;
    
    MPSYS_SemaIn(FALSE);

    err = MPG_ERR_OK;
    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_V) {
        /* 仕様として、３フレーム自動ミュートは常にできない（固定値） */
        mute = CDC_MPMUT_DFL;
        if (rsw == OFF) {
            mute |= CDC_MPMUT_R;
        }
        if (lsw == OFF) {
            mute |= CDC_MPMUT_L;
        }
        MPG_MV_AMUTE(mpgmv) = mute;

        if (MPG_MV_ACTA(mpgmv) == ON) {
            /* デコード方法の設定関数実行 */
            err = MPSYS_DoSetDec(mute, MPG_IGNORE, MPG_IGNORE);
        }
    }
    
    MPSYS_SemaOut(err);

    return;
}

/* ビデオの動作状態の取得 */
Sint32  MPG_MvGetVideoStat(MpgMv mpgmv)
{
    Sint32      stat;
    Sint32      err;

    MPSYS_SemaIn(TRUE);

    /* ビデオの動作状態の取得実行 */
    stat = getVideoStatSub(mpgmv, &err);

    MPSYS_SemaOut(err);

    return stat;
}

/* ビデオの動作状態の取得実行 */
static Sint32   getVideoStatSub(MpgMv mpgmv, Sint32 *err)
{
    CdcMpStat   mpstat, *msptr;
    Sint32      stat;

    stat = MPG_VSTAT_STOP;
    *err = MPG_ERR_OK;
    if (MPG_MV_ACTV(mpgmv) == OFF) {
        return stat;
    }

    msptr = &mpstat;

    /* 現在のMPEGステータス情報の取得 */
    if (CDC_MpGetCurStat(msptr) != CDC_ERR_OK) {
        *err = MPG_ERR_NG;
        return stat;
    }

    switch (CDC_MPGET_ASTV(msptr)) {
    case CDC_MPASTV_STOP:
        if (MPSYS_IsVbvEmpty(err)) {
            stat = MPG_VSTAT_STOP;
        } else {
            stat = MPG_VSTAT_PLAY;
        }
        break;
    case CDC_MPASTV_PRE1:
    case CDC_MPASTV_PRE2:
        stat = MPG_VSTAT_PREP;
        break;
    case CDC_MPASTV_TRNS:
        if ((CDC_MPSTAT_STV(msptr) & CDC_MPSTV_RDY) == 0) {
            stat = MPG_VSTAT_PREP;
        } else {
            stat = MPG_VSTAT_PLAY;
        }
        break;
    case CDC_MPASTV_CHG:
        stat = MPG_VSTAT_PLAY;
        break;
    case CDC_MPASTV_RCV:
        stat = MPG_VSTAT_ERR;
        break;
    default:
        stat = MPG_VSTAT_PREP;
        break;
    }
    return stat;
}

/* オーディオの動作状態の取得 */
Sint32  MPG_MvGetAudioStat(MpgMv mpgmv)
{
    Sint32      stat;
    Sint32      err;
    
    MPSYS_SemaIn(TRUE);
    
    /* オーディオの動作状態の取得実行 */
    stat = getAudioStatSub(mpgmv, &err);

    MPSYS_SemaOut(err);

    return stat;
}

/* オーディオの動作状態の取得実行 */
static Sint32   getAudioStatSub(MpgMv mpgmv, Sint32 *err)
{
    CdcMpStat   mpstat, *msptr;
    Sint32      stat;
    
    stat = MPG_ASTAT_STOP;
    *err = MPG_ERR_OK;
    if (MPG_MV_ACTA(mpgmv) == OFF) {
        return stat;
    }

    msptr = &mpstat;

    /* 現在のMPEGステータス情報の取得 */
    if (CDC_MpGetCurStat(msptr) != CDC_ERR_OK) {
        *err = MPG_ERR_NG;
        return stat;
    }

    switch (CDC_MPGET_ASTA(msptr)) {
    case  CDC_MPASTA_STOP:
        stat = MPG_ASTAT_STOP;
        break;
    case CDC_MPASTA_PRE1:
    case CDC_MPASTA_PRE2:
        stat = MPG_ASTAT_PREP;
        break;
    case CDC_MPASTA_TRNS:
        stat = MPG_ASTAT_PLAY;
        break;
    case CDC_MPASTA_CHG:
        stat = MPG_ASTAT_PLAY;
        break;
    case CDC_MPASTA_RCV:
        stat = MPG_ASTAT_ERR;
        break;
    default:
        stat = MPG_ASTAT_PREP;
        break;
    }
    return stat;
}

/* 動画のMPEGウィンドウの接続 */
void    MPG_MvConnectWin(MpgMv mpgmv, MpgWn mpgwn)
{
    MPSYS_SemaIn(FALSE);
    
    /* 使用フレームバンクは０で固定 */
    MPG_WN_FBN(mpgwn) = MPL_FBN_MV;
    
    if (MPG_MV_PMODE(mpgmv) != MPG_PMODE_A) {
        if (MPG_MV_MPGWN(mpgmv, MPL_FBN_MV) == NULL) {
            MPG_MV_MPGWN(mpgmv, MPL_FBN_MV) = mpgwn;
        } else {
            MPWIN_Connect(MPG_MV_MPGWN(mpgmv, MPL_FBN_MV), mpgwn);
        }
        if (MPG_MV_ACTV(mpgmv) == ON) {
            MPG_MvOutput(mpgmv, ON);
        }
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* 動画のMPEGウィンドウの切り離し */
void    MPG_MvDisconnectWin(MpgMv mpgmv, MpgWn mpgwn)
{
    MPSYS_SemaIn(FALSE);
    
    if (MPG_MV_ACTV(mpgmv) == ON) {
        MPG_WnDisp(mpgwn, OFF);
    }
    if (MPG_MV_MPGWN(mpgmv, MPL_FBN_MV) == mpgwn) {
        MPG_MV_MPGWN(mpgmv, MPL_FBN_MV) = NULL;
    } else {
        MPWIN_Disconnect(mpgwn);
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* 動画のデコードの実行 */
Sint32  MPG_MvDecNext(MpgMv mpgmv)
{
    Sint32      ret;

    /* ビデオのデコード実行 */
    /* 引数（フレームバンク番号）は静止画再生モードで有効 */
    ret = MPMVE_DecNext(mpgmv, 0);

    return ret;
}

/* ビデオのデコード実行 */
Sint32  MPMVE_DecNext(MpgMv mpgmv, Sint32 bnk)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);

    ret = MPG_ERR_ILLSTAT;
    
    if (MPG_MV_ACTV(mpgmv) == ON) {
        /* デコード同期信号の送出関数実行 */
        ret= MPSYS_DoOutDsync(bnk);
    }

    return MPSYS_SemaOut(ret);
}

/* 動画の出力 */
Sint32  MPG_MvOutput(MpgMv mpgmv, Bool chkupic)
{
    Sint32      ret;
    
    MPSYS_SemaIn(FALSE);
    
    /* 動画の出力実行 */
    ret = outputSub(mpgmv, chkupic);

    return MPSYS_SemaOut(ret);
}

/* 動画の出力実行 */
static Sint32   outputSub(MpgMv mpgmv, Bool chkupic)
{
    Sint32      ret;
    Sint32      upc;
    CdcMpStat   mpstat;
    
    /* MPEGの現在の状態の取得 */
    if (CDC_MpGetCurStat(&mpstat) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    upc = CDC_MPSTV_UPDPIC;
    if (chkupic == ON) {
        upc &= CDC_MPSTAT_STV(&mpstat);
    }

    /* ビデオの出力 */
    ret = MPMVE_Output(mpgmv, upc, 0);

    return ret;
}

/* ビデオの出力 */
Sint32  MPMVE_Output(MpgMv mpgmv, Sint32 upc, Sint32 bnk)
{
    Sint32      ret;
    Sint32      omode;

    ret = MPG_ERR_ILLSTAT;

    if (MPG_MvGetVideoStat(mpgmv) == MPG_VSTAT_PLAY) {
        omode = MPG_MV_OUTDST(mpgmv);
        if ((MPG_MV_ACTV(mpgmv) == ON) &&
            (omode == MPG_OUTDST_HOST) &&
            (upc != UPDPIC_OFF)) {

            /* 接続されている全てのウィンドウを転送する */
            MPWIN_OutputAll(MPG_MV_MPGWN(mpgmv, bnk), omode);

            ret = MPG_ERR_OK;
        }
    }
    return ret;
}

/* ストリームの強制切り替え */
Sint32  MPG_MvChange(MpgMv mpgmv)
{
    Bool        vflg, aflg, setflg;
    Sint32      ret;
    Sint32      chg_a, chg_v;
    
    MPSYS_SemaIn(TRUE);

    ret = MPG_ERR_ILLSTAT;
    chg_a = MPG_IGNORE;
    chg_v = MPG_IGNORE;

    vflg = (MPSYS_ReqVideoCon(MPG_STM_CUR) == MPG_MV_VBNO(mpgmv));
    aflg = (MPSYS_ReqAudioCon(MPG_STM_CUR) == MPG_MV_ABNO(mpgmv));

    switch (MPG_MV_PMODE(mpgmv)) {
    case MPG_PMODE_V:
        setflg = vflg;
        chg_v = CDC_MPCOF_CHG;
        break;
    case MPG_PMODE_A:
        setflg = aflg;
        chg_a = CDC_MPCOF_CHG;
        break;
    default:
        setflg = (vflg && aflg);
        chg_a = CDC_MPCOF_CHG;
        chg_v = CDC_MPCOF_CHG;
        break;
    }

    if (setflg) {
        /* MPEGデコーダの接続先の切り替え関数実行 */
        ret = MPSYS_DoChgCon(chg_a, chg_v,
                             MPG_MV_CLRA(mpgmv), MPG_MV_CLRV(mpgmv));
    }

    return MPSYS_SemaOut(ret);
}

/* バッファの消去方法の設定 */
void    MPG_MvSetClearMode(MpgMv mpgmv, Sint32 vmode, Sint32 amode)
{
    MPG_MV_CLRV(mpgmv) = vmode;
    MPG_MV_CLRA(mpgmv) = amode;
    
    return;
}

/* ストリームレイヤの設定 */
void    MPG_MvSetLay(MpgMv mpgmv, Sint32 vlay, Sint32 alay)
{
    if (vlay != MPG_NOCHG) {
        MPG_MV_VLAY(mpgmv) = vlay;
    }
    if (alay != MPG_NOCHG) {
        MPG_MV_ALAY(mpgmv) = alay;
    }
    return;
}

/* 全MPEG動画ハンドルの初期化 */
void    MPMVE_InitAllMv(void)
{
    Sint32      i;
    
    for (i = 0; i < MPG_MV_MAX; i++) {
        /* MPEGハンドルオブジェクトの消去 */
        deleteMpgmv(&mp_movie[i]);
    }
    return;
}



