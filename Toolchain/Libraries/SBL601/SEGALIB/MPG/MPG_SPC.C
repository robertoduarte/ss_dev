/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : 静止画再生モジュールソースファイル
 *  File   : mpg_spc.c
 *  Date   : 1995-09-05
 *  Author : H.Take
 *
 *******************************************************************/

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        "mpg_loc.h"

/*******************************************************************
 *      関数定義
 *******************************************************************/

/* MpgSp型の実体はMpgMv型と同じなので、MpgMv型を引数にとる関数へ */
/* 型変換せずに渡している */

/* MPEG静止画ハンドルの生成 */
MpgSp   MPG_SpCreate(StmHn vstm)
{
    /* MPEGハンドルの生成 */    
    return MPG_MvCreate(vstm, NULL);
}

/* MPEG静止画ハンドルの消去 */
Sint32  MPG_SpDestroy(MpgSp mpgsp)
{
    /* MPEGハンドルの消去 */
    return MPG_MvDestroy(mpgsp);
}

/* 静止画の出力先の設定 */
void    MPG_SpSetVideoDest(MpgSp mpgsp, Sint32 dst)
{
    /* 出力先の設定 */
    MPG_MvSetVideoDest(mpgsp, dst);
    
    return;
}

/* 静止画の終了条件の設定 */
void    MPG_SpSetTermCond(MpgSp mpgsp, Sint32 vcnd)
{
    /* 終了条件の設定 */
    MPG_MvSetTermCond(mpgsp, vcnd, MPG_IGNORE);
    
    return;
}

/* 静止画のチャンネルの設定 */
void    MPG_SpSetChan(MpgSp mpgsp, Sint32 ch)
{
    /* ビデオチャンネルの設定 */
    MPG_MvSetVideoChan(mpgsp, ch);
    
    return;
}

/* 静止画のストリームＩＤの設定 */
void    MPG_SpSetId(MpgSp mpgsp, Sint32 id)
{
    /* ビデオのストリームＩＤの設定 */
    MPG_MvSetVideoId(mpgsp, id);
    
    return;
}

/* 静止画再生の開始 */
Sint32  MPG_SpStart(MpgSp mpgsp)
{
    /* 再生の開始 */
    return MPG_MvStart(mpgsp, CDC_MPSRCH_OFF);
}

/* 静止画再生の停止 */
Sint32  MPG_SpStop(MpgSp mpgsp)
{
    /* 再生の停止 */
    return MPG_MvStop(mpgsp);
}

/* 静止画のデコードの実行 */
Sint32  MPG_SpDecNext(MpgSp mpgsp, Sint32 bnk)
{
    Sint32      ret;

    /* ビデオのデコード実行 */
    ret = MPMVE_DecNext(mpgsp, bnk);

    return ret;
}

/* 静止画の動作状態の取得 */
Sint32  MPG_SpGetVideoStat(MpgSp mpgsp)
{
    return MPG_MvGetVideoStat(mpgsp);
}

/* 静止画のタイムコードの取得 */
void    MPG_SpGetTimeCode(MpgSp mpgsp,
                          Sint32 *hh, Sint32 *mm, Sint32 *ss, Sint32 *pp)
{
    /* タイムコードの取得 */
    MPG_MvGetTimeCode(mpgsp, hh, mm, ss, pp);
    
    return;
}

/* 静止画のMPEGウィンドウの接続 */
void    MPG_SpConnectWin(MpgSp mpgsp, MpgWn mpgwn, Sint32 bnk)
{
    MpgWn wnwk;

    MPSYS_SemaIn(FALSE);
    
    /* 使用フレームバンクの設定 */
    MPG_WN_FBN(mpgwn) = bnk;
    
    wnwk = MPG_MV_MPGWN(mpgsp, bnk);
    if (wnwk == NULL) {
        MPG_MV_MPGWN(mpgsp, bnk) = mpgwn;
    } else {
        MPWIN_Connect(wnwk, mpgwn);
    }
    if (MPG_MV_ACTV(mpgsp) == ON) {
        MPG_SpOutput(mpgsp, bnk);
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* 静止画のMPEGウィンドウの切り離し */
void    MPG_SpDisconnectWin(MpgSp mpgsp, MpgWn mpgwn, Sint32 bnk)
{
    MPSYS_SemaIn(FALSE);
    
    if (MPG_MV_ACTV(mpgsp) == ON) {
        MPG_WnDisp(mpgwn, OFF);
    }
    if (MPG_MV_MPGWN(mpgsp, bnk) == mpgwn) {
        MPG_MV_MPGWN(mpgsp, bnk) = NULL;
    } else {
        MPWIN_Disconnect(mpgwn);
    }
    
    MPSYS_SemaOut(MPG_ERR_OK);

    return;
}

/* 静止画の出力 */
Sint32  MPG_SpOutput(MpgSp mpgsp, Sint32 bnk)
{
    Sint32      ret;
    
    MPSYS_SemaIn(FALSE);

    /* ビデオの出力 */
    ret = MPMVE_Output(mpgsp, CDC_MPSTV_UPDPIC, bnk);
    
    return MPSYS_SemaOut(ret);
}



