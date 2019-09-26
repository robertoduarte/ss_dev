/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : MPEGシステム制御モジュールソースファイル
 *  File   : mpg_sys.c
 *  Date   : 1996-02-01
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

/* CDブロックの初期化フラグ（ソフトリセット） */
#define CDINIT_FLAG     0x01

/* CDブロックの初期化のタイムアウト時間 */
#define CDINIT_CNT      400000

/* ハングアップ検出条件（VSYNC単位） */
#define HNG_COND        30

/* MPEG割り込み要因のマスクパターン */
#define INTREQ_MSK      0x00ffffff

/* MPEGデコーダの初期化の終了待ち時間（２VSYNC） */
#define MINI_CNT        53000

/* 表示の走査方法のPALビット */
#define SCANMODE_PAL    0x02        

/* PAL用のディスプレイウィンドウオフセット（輝点単位） */
#define PL_DOX          150
#define PL_DOY          45

/* SHORT WAITに対するリトライ回数 */
#define RTRY_NUM        50

/*******************************************************************
*       列挙定数
*******************************************************************/

/* MPEG関数のセマフォ処理用定数 */
enum SemaCode {
    SEMA_IN  = 1,       /* 入口 */
    SEMA_OUT = -1       /* 出口 */
};

/* MPEG割り込み要因参照用定数 */
enum MintRef {
    MINT_HNG   = 0,     /* ハングアップ検出用 */
    MINT_DSYNC = 1,     /* デコード準備の完了確認用 */
    MINT_USR   = 2,     /* ユーザ用 */
    MINT_NUM   = 3
};

/*******************************************************************
 *      処理マクロ
 *******************************************************************/

/* MPEGチェック（今後、sega_sys.hで提供） */
#ifndef SYS_CHKMPEG
#define SYS_CHKMPEG(filtno) \
    ((**(Sint32(**)(Sint32))0x6000274)(filtno))
#endif

/*******************************************************************
 *      データ型の宣言
 *******************************************************************/

/* 割り込み要因情報チェック関数 */
typedef Bool (*ReqintFunc)(Sint32 bitpat);

/*******************************************************************
 *      関数宣言
 *******************************************************************/

static Sint32   noactFunc(void *obj);
static Sint32   mpgInitSub(Sint32 dscn);
static Sint32   initCdb(void);
static Sint32   checkMpeg(void);
static Sint32   setMpPara(void);
static Sint32   doInit(Sint32 scnmod);
static void     initStatVal(void);
static void     entryIntreqFunc(ReqintFunc func, Sint32 bitpat);
static Bool     isIntHng(Sint32 bitpat);
static Bool     isGetMpint(Sint32 intref, Sint32 *mpint, Sint32 *err);
static Sint32   updInt(Sint32 intref, Sint32 *intreq);
static void     initErr(void);
static Sint32   doCaptStat(void);
static Bool     isDecReady(Sint32 *err);
static Sint32   resetVld(void);
static Bool     checkHngSub(Sint32 *err);
static Bool     isDecodingVideo(Sint32 *err);
static Sint32   resetMpSub(Bool mpsw);
static void     semaMpg(Sint32 incnt, Bool cptflg);
static Bool     isTimeOut(Sint32 *cnt);

/*******************************************************************
 *      変数定義
 *******************************************************************/

/* バージョン */
const Sint8     mpg_version[MPG_VERSION_LEN] = MPG_VERSION_STR;

/* MPEGの現在の状態 */
static CdcMpStat        mp_stat;

/* MPEGの現在の接続状態 */
static CdcMpCon mp_con_a1;
static CdcMpCon mp_con_v1;

/* MPEGの次のストリームの登録状態 */
static CdcMpCon mp_con_a2;
static CdcMpCon mp_con_v2;

/* イベント起動関数 */
static MplEventFunc     mp_event_func;

/* イベント起動オブジェクト */
static void     *mp_event_obj;

/* 割り込み要因情報チェック関数 */
static ReqintFunc       mp_intreq_func;

/* 割り込み要因情報チェックビットパターン */
static Sint32   mp_intreq_bitpat;

/* ハングアップ検出カウンタ（VSYNC単位） */
static Sint32   hng_cnt;

/* MPG_CaptStat関数実行中フラグ */
static Bool     exe_capt_flag;

/* MPEGシステムの状態の取り込み実行フラグ */
static Bool     mp_capt_flag;

/* MPG関数実行中カウンタ（セマフォ） */
/* MPG関数実行中に、割り込みでMPG_CaptStat関数を実行しないようにする */
static Sint32   mp_in_cnt;

/* ピクチャスタート検出フラグ */
static Bool     pstrt_flag;

/* MPEG割り込み要因変数 */
static Sint32   mp_intval[MINT_NUM];

/* 表示の走査方法（インタレース／ノンインタレースの設定） */
static Sint32   scan_mode;

/* MPEGの現在のポーズタイミング（ハングアップ検出に使用する） */
static Sint32   mp_pautim;

/* MPEGビデオ動作モード */
static Sint32   mp_vmode;

/* エラー状態 */
static MpgErrStat       mp_err_stat;

/*******************************************************************
 *      関数定義
 *******************************************************************/

/* ビデオの接続先区画番号の取得 */
Sint32  MPSYS_ReqVideoCon(Sint32 next)
{
    CdcMpCon    *ptr;

    if (next == MPG_STM_CUR) {
        ptr = &mp_con_v1;
    } else {
        ptr = &mp_con_v2;
    }
    return CDC_MPCON_BN(ptr);
}

/* オーディオの接続先区画番号の取得 */
Sint32  MPSYS_ReqAudioCon(Sint32 next)
{
    CdcMpCon    *ptr;

    if (next == MPG_STM_CUR) {
        ptr = &mp_con_a1;
    } else {
        ptr = &mp_con_a2;
    }
    return CDC_MPCON_BN(ptr);
}

/* ビデオがポーズ中か否かのチェック */
Bool    MPSYS_IsPauseVideo(Sint32 *err)
{
    CdcMpStat   mpstat;
    
    *err = MPG_ERR_NG;

    if (CDC_MpGetCurStat(&mpstat) == CDC_ERR_OK) {
        *err = MPG_ERR_OK;
        if ((CDC_MPSTAT_STV(&mpstat) & CDC_MPSTV_PAUSE) != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/* イベント起動関数の登録 */
void    MPSYS_EntryEventFunc(MplEventFunc func, void *obj)
{
    mp_event_func = func;
    mp_event_obj = obj;
    
    return;
}

/* イベント起動関数の抹消 */
void    MPSYS_KillEventFunc(void)
{
    mp_event_func = noactFunc;
    mp_event_obj = NULL;
    
    return;
}

/* 何もしない関数 */
static Sint32   noactFunc(void *obj)
{
    obj = obj;

    return 0;
}

/* VBVバッファがエンプティか否かのチェック */
/* CDC関数がエラーを返した時はVBVエンプティとみなす */
Bool    MPSYS_IsVbvEmpty(Sint32 *err)
{
    Sint32      rdata, wdata;
    Sint32      rwsw, reg_no;

    *err = MPG_ERR_NG;

    /* VBVエンプティのチェック */
    rwsw = 1;
    reg_no = 6;
    wdata = 0;
    if (CDC_MpSetLsi(rwsw, reg_no, wdata, &rdata) == CDC_ERR_OK) {
        *err = MPG_ERR_OK;
        if ((rdata & 0x4000) == 0) {
            return FALSE;
        }
    }

    return TRUE;
}

/* MPEGシステムの初期化 */
Sint32  MPG_Init(Sint32 dscn)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);

    /* MPEGシステムの初期化実行 */
    ret = mpgInitSub(dscn);
    if (ret != MPG_ERR_OK) {
        MPSYS_SetErrCode(ret);
    }

    return MPSYS_SemaOut(ret);
}

/* MPEGシステムの初期化実行 */
static Sint32   mpgInitSub(Sint32 dscn)
{
    Sint32      ret;

    /* 表示の走査方法（インタレース／ノンインタレースの設定） */
    scan_mode = dscn;

    /* CDブロックの初期化処理（ソフトリセット） */
    if ((ret = initCdb()) != MPG_ERR_OK) {
        return ret;
    }
    
    /* MPEGのチェック */
    if ((ret = checkMpeg()) != MPG_ERR_OK) {
        return ret;
    }

    /* MPEGデコーダの初期化 */                  
    if ((ret = MPSYS_InitMpd(ON)) != MPG_ERR_OK) {
        return ret;
    }
    
    /* MPEGの初期化実行 */
    ret = doInit(dscn);

    return ret;
}

/* CDブロックの初期化処理（ソフトリセット） */
static Sint32   initCdb(void)
{
    Sint32      ret;
    Sint32      cnt;
    
    /* CDブロックの初期化（ソフトリセット） */
    ret = CDC_CdInit(CDINIT_FLAG, CDC_PARA_NOCHG, CDC_PARA_NOCHG,
                     CDC_PARA_NOCHG);
    if (ret != CDC_ERR_OK) {
        return MPG_ERR_CDINIT;
    }
    
    /* 初期化が終了するまで待つ */
    cnt = 0;
    while ((CDC_GetHirqReq() & CDC_HIRQ_ESEL) == 0) {
        if (cnt++ > CDINIT_CNT) {
            return MPG_ERR_TMOUT;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGのチェック */
static Sint32   checkMpeg(void)
{
    Sint32      ret;
    CdcHw       hw;

    /* MPEGカートリッジの装着チェック */
    ret = CDC_GetHwInfo(&hw);
    if (ret != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    if ((CDC_HW_HFLAG(&hw) & CDC_HFLAG_MPEG) == 0) {
        /* 装着されていない */
        return MPG_ERR_NOMC;
    }
    
    /* MPEGが使用可能か調べる */
    if (CDC_IS_MPEG_ENA(&hw) == FALSE) {
        /* MPEGが使用不可能ならば、MPEGチェックを実行する */
        /* （失敗しても２回まで実行を試みる） */
        if (SYS_CHKMPEG(0) != OK) {
            if (SYS_CHKMPEG(0) != OK) {
                return MPG_ERR_MPNG;
            }
        }
    }
    return MPG_ERR_OK;
}

/* MPEGデコーダの初期化 */
Sint32  MPSYS_InitMpd(Bool sw)
{
    Sint32      ret, err;
    Sint32      hirqflg;
    Sint32      cnt;

    err = MPG_ERR_OK;
    
    hirqflg = CDC_HIRQ_MPED;
    if (sw == ON) {
        hirqflg |= CDC_HIRQ_MPCM;
    }
    
    /* MPEGデコーダの初期化 */                  
    ret = CDC_MpInit(sw);
    if (ret != CDC_ERR_OK) {
        return MPG_ERR_MPINIT;
    }

    /* 初期化が終了するまで待つ（最短２VSYNCは待つ） */
    /* １回のループで18ステート, 28MHzで約0.64us */
    cnt = 0;
    while (((CDC_GetHirqReq() & hirqflg) != hirqflg) || (cnt < MINI_CNT)) {
        if (cnt++ > MPL_INIT_CNT) {
            return MPG_ERR_TMOUT;
        }
    }
    
    if (sw == ON) {
        /* MPEGシステムに対する設定 */
        err = setMpPara();
    }
    return err;
}

/* MPEGシステムに対する設定 */
static Sint32   setMpPara(void)
{
    Sint32      ret;

    /* MPEG割り込みマスクの設定 */
    ret = CDC_MpSetIntMsk(CDC_MPINT_PSTRT|CDC_MPINT_SQEND);
    if (ret != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    if ((scan_mode & SCANMODE_PAL) != 0) {
        /* ディスプレイウィンドウオフセットの設定 */
        ret = CDC_MpSetWinDofs(TRUE, PL_DOX, PL_DOY);
        if (ret != CDC_ERR_OK) {
            return MPG_ERR_NG;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGの初期化実行 */
static Sint32   doInit(Sint32 scnmod)
{
    Sint32      ret;

    /* MPEG動作モードの設定関数実行 */
    ret = MPSYS_DoSetMode(MPG_NOCHG, MPG_NOCHG, MPG_NOCHG, scnmod);
    if (ret != MPG_ERR_OK) {
        return ret;
    }
    
    /* 保時変数の初期化 */
    initStatVal();
    
    /* エラー状態の初期化 */
    initErr();

    /* 全ディスプレイウィンドウの初期化 */
    MPWIN_InitAllWn();
    
    /* 全MPEG動画ハンドルの初期化 */
    MPMVE_InitAllMv();
    
    MPG_CaptStat();
    
    return MPG_ERR_OK;
}

/* 保時変数の初期化 */
static void     initStatVal(void)
{
    static const CdcMpStat      mpstat_ini = {0, 0, 0, 0, 0, 0};
    static const CdcMpCon       mpcon_ini = {0, 0, CDC_NUL_SEL};
    Sint32      i;
    
    /* MPEGビデオ動作モード */
    mp_vmode = MPG_VMODE_MOVIE;
    
    /* MPEGの現在の状態 */
    mp_stat = mpstat_ini;
    
    /* MPEGストリームの接続状態と登録状態 */
    mp_con_a1 = mpcon_ini;
    mp_con_a2 = mpcon_ini;
    mp_con_v1 = mpcon_ini;
    mp_con_v2 = mpcon_ini;
    
    /* イベント起動関数の抹消 */
    MPSYS_KillEventFunc();
    
    /* ハングアップ検出割り込みチェック関数の登録 */
    entryIntreqFunc(isIntHng, CDC_MPINT_PSTRT|CDC_MPINT_SQEND);

    /* MPEGの現在のポーズタイミング */
    mp_pautim = 1;
    
    /* ハングアップ検出カウンタ */
    hng_cnt = 0;
    
    /* MPG_CaptStat関数実行中フラグ */
    exe_capt_flag = FALSE;

    /* MPG_CaptStat実行フラグ */
    mp_capt_flag = FALSE;
    
    /* MPG関数実行中カウンタ */
    mp_in_cnt = 0;
    
    /* ピクチャスタート検出フラグ */
    pstrt_flag = FALSE;

    /* MPEG割り込み要因変数 */
    for (i = 0; i < MINT_NUM; i++) {
        mp_intval[i] = 0;
    }
    return;
}

/* 割り込み要因情報チェック関数の登録 */
static void     entryIntreqFunc(ReqintFunc func, Sint32 bitpat)
{
    mp_intreq_func = func;
    mp_intreq_bitpat = bitpat;
    
    return;
}

/* ハングアップ検出用割り込みのチェック */
static Bool     isIntHng(Sint32 bitpat)
{
    Bool        chk;
    Sint32      err;
    Sint32      mpint;

    chk = isGetMpint(MINT_HNG, &mpint, &err);
    if (err != MPG_ERR_OK) {
        MPSYS_SetErrCode(err);
        return FALSE;
    }

    /* MPEG割り込み変数の取得 */
    if (chk == TRUE) {
        if ((mpint & bitpat) != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/* MPEG割り込み変数の取得 */
static Bool     isGetMpint(Sint32 intref, Sint32 *mpint, Sint32 *err)
{
    *err = MPG_ERR_OK;

    /* MPSTフラグチェック */
    if ((CDC_GetHirqReq() & CDC_HIRQ_MPST) != 0) {
        /* MPSTフラグクリア */
        CDC_ClrHirqReq(~CDC_HIRQ_MPST);
        
        /* MPEG割り込み変数の更新 */
        *err = updInt(intref, mpint);

        return TRUE;
    }
    return FALSE;
}

/* MPEG割り込み変数の更新 */
static Sint32   updInt(Sint32 intref, Sint32 *intreq)
{
    Sint32      i;
    Sint32      inttmp;
    
    /* MPEG割り込み要因の取得 */
    if (CDC_MpGetInt(&inttmp) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }

    /* MPEG割り込み変数の更新 */
    for (i = 0; i < MINT_NUM; i++) {
        mp_intval[i] |= inttmp;
    }

    *intreq = (mp_intval[intref] & INTREQ_MSK);

    /* MPEG割り込み変数のクリア */
    mp_intval[intref] = 0;

    return MPG_ERR_OK;
}

/* エラー状態の初期化 */
static void     initErr(void)
{
    MpgErrStat  *errstat;

    errstat = &mp_err_stat;

    MPG_ERR_FUNC(errstat) = NULL;
    MPG_ERR_OBJ(errstat) = NULL;
    MPG_ERR_CODE(errstat) = MPG_ERR_OK;

    return;
}

/* MPEGシステムの状態の取り込み */
void    MPG_CaptStat(void)
{
    Sint32      err;

    /* MPG_CaptStat関数を実行中ならすぐに戻る */
    if (exe_capt_flag == TRUE) {
        return;
    }

    /* MPEGシステムの状態の取り込み実行 */
    exe_capt_flag = TRUE;
    if ((err = doCaptStat()) != MPG_ERR_OK) {
        MPSYS_SetErrCode(err);
    }
    exe_capt_flag = FALSE;

    return;
}

/* MPEGシステムの状態の取り込み実行 */
static Sint32   doCaptStat(void)
{
    Sint32      ret;

    /* MPG関数を実行中ならすぐに戻る */
    if (mp_in_cnt > 0) {
        mp_capt_flag = TRUE;
        return;
    }

    /* MPEGシステムの状態取得 */
    ret = MPSYS_CaptSub();

    mp_capt_flag = FALSE;
    
    return ret;
}

/* MPEGシステムの状態取得 */
Sint32  MPSYS_CaptSub(void)
{
    Sint32      ret;

    /* 状態の取り込み */
    CDC_MpGetCurStat(&mp_stat);
    CDC_MpGetCon(CDC_MPSTF_CUR, &mp_con_a1, &mp_con_v1);
    CDC_MpGetCon(CDC_MPSTF_NEXT, &mp_con_a2, &mp_con_v2);

    /* イベント起動関数のNULLチェック */
    if (mp_event_func == NULL) {
        return MPG_ERR_OK;
    }

    /* イベントの発火 */
    ret = (*mp_event_func)(mp_event_obj);

    return ret;
}

/* MPEGビデオの動作モードの設定 */
Sint32  MPG_SetVideoMode(Sint32 vmode)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);

    /* MPEG動作モードの設定関数実行 */
    ret = MPSYS_DoSetMode(vmode, MPG_NOCHG, MPG_NOCHG, MPG_NOCHG);
    if (ret == MPG_ERR_OK) {
        mp_vmode = vmode;
    }
    
    return MPSYS_SemaOut(ret);
}

/* MPEGビデオの動作モードの取得 */
Sint32  MPG_GetVideoMode(void)
{
    return mp_vmode;
}

/* デコード準備の完了確認 */
Bool    MPG_IsDecReady(Sint32 decpic)
{
    Bool        ret;
    Sint32      err;
    Sint32      dummy;

    MPSYS_SemaIn(FALSE);
    
    ret = FALSE;
    err = MPG_ERR_OK;
    if (decpic == MPG_HDEC_PRE) {
        /* MPSTフラグクリア */
        CDC_ClrHirqReq(~CDC_HIRQ_MPST);

        /* MPEG割り込み要因のクリア */
        err = updInt(MINT_DSYNC, &dummy);
    } else {
        /* デコード準備のチェック */
        ret = isDecReady(&err);
    }
    MPSYS_SemaOut(err);

    return ret;
}

/* デコード準備のチェック */
static Bool     isDecReady(Sint32 *err)
{
    Bool        chk;
    Sint32      mpint;
    CdcMpStat   mpstat;

    *err = MPG_ERR_OK;

    /* ピクチャスタート検出のチェック */
    if (pstrt_flag == FALSE) {
        /* MPEG割り込み変数の取得 */
        chk = isGetMpint(MINT_DSYNC, &mpint, err);
        if (*err != MPG_ERR_OK) {
            return FALSE;
        }
        /* MPEG割り込み要因を取得した場合 */
        if (chk == TRUE) {
            /* PSCまたはSQE割り込みのチェック */
            if ((mpint & CDC_MPINT_PSTRT) != 0) {
                pstrt_flag = TRUE;
            } else if ((mpint & CDC_MPINT_SQEND) != 0) {
                /* VLDリセット */
                if ((*err = resetVld()) != MPG_ERR_OK) {
                    return FALSE;
                }
            }
        }
    }

    /* 復活状態でないことのチェック */
    if (pstrt_flag == TRUE) {
        if (CDC_MpGetCurStat(&mpstat) != CDC_ERR_OK) {
            *err = MPG_ERR_NG;
            return FALSE;
        }
        if (CDC_MPGET_ASTV(&mpstat) != CDC_MPASTV_RCV) {
            pstrt_flag = FALSE;
            return TRUE;
        }
    }
    return FALSE;
}

/* VLDリセット */
static Sint32   resetVld(void)
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
    wdata = (rdata & 0xdfff);
    if (CDC_MpSetLsi(rwsw, reg_no, wdata, &rdata) != CDC_ERR_OK) {
        return MPG_ERR_NG;
    }
    
    return MPG_ERR_OK;
}

/* ハングアップのリセット */
void    MPG_ResetHng(void)
{
    /* ハングアップ検出カウンタのリセット */
    hng_cnt = 0;
    
    /* MPSTフラグクリア */
    CDC_ClrHirqReq(~CDC_HIRQ_MPST);
    
    return;
}

/* ハングアップの検出 */
Bool    MPG_CheckHng(void)
{
    Bool        ret;
    Sint32      err;

    MPSYS_SemaIn(FALSE);
    
    /* ハングアップの検出実行 */
    ret = checkHngSub(&err);

    MPSYS_SemaOut(err);
    
    return ret;
}

/* ハングアップの検出実行 */
static Bool     checkHngSub(Sint32 *err)
{
    Bool        inton;
    Bool        vbv, dec, pau;

    /* VBVバッファがエンプティか否かのチェック */
    vbv = MPSYS_IsVbvEmpty(err);
    if (*err != MPG_ERR_OK) {
        return TRUE;
    }

    /* ビデオがデコード中か否かのチェック */
    dec = isDecodingVideo(err);
    if (*err != MPG_ERR_OK) {
        return TRUE;
    }

    /* ビデオがポーズ中か否かのチェック */
    pau = MPSYS_IsPauseVideo(err);
    if (*err != MPG_ERR_OK) {
        return TRUE;
    }

    /* 割り込み要因情報チェック関数がNULLの場合は、ハングアップとみなす */
    if (mp_intreq_func == NULL) {
        return TRUE;
    }

    if ((vbv == FALSE) && (dec == TRUE) && (pau == FALSE)) {
        /* PTSまたはSQEのチェック */
        inton = (*mp_intreq_func)(mp_intreq_bitpat);
        
        if (inton) {
            /* 検出カウンタのリセット */
            hng_cnt = 0;
        } else {
            hng_cnt++;
            /* タイムアウトのチェック */
            if (hng_cnt > (HNG_COND * mp_pautim)) {
                return TRUE;
            }
        }
    } else {
        /* 検出カウンタのリセット */
        hng_cnt = 0;
    }
    
    return FALSE;
}

/* ビデオがデコード中か否かのチェック */
static Bool     isDecodingVideo(Sint32 *err)
{
    CdcMpStat   mpstat;

    *err = MPG_ERR_NG;

    if (CDC_MpGetCurStat(&mpstat) == CDC_ERR_OK) {
        *err = MPG_ERR_OK;
        if ((CDC_MPSTAT_STV(&mpstat) & CDC_MPSTV_DEC) != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/* MPEGレポートの取得 */
Sint32  MPG_GetReport(Sint32 *actst, Sint32 *vcnt, Sint32 *pict,
                      Sint32 *stat_a, Sint32 *stat_v)
{
    Sint32      ret;
    CdcMpStat   mpstat, *msptr;

    MPSYS_SemaIn(FALSE);

    ret = MPG_ERR_NG;

    /* 現在のMPEGステータス情報の取得 */
    msptr = &mpstat;
    if (CDC_MpGetCurStat(msptr) == CDC_ERR_OK) {
        ret = MPG_ERR_OK;

        *actst = CDC_MPSTAT_AST(msptr);
        *vcnt = CDC_MPSTAT_VCNT(msptr);
        *pict = CDC_MPSTAT_PICT(msptr);
        *stat_a = CDC_MPSTAT_STA(msptr);
        *stat_v = CDC_MPSTAT_STV(msptr);
    }
    
    return MPSYS_SemaOut(ret);
}

/* MPEG割り込みの取得 */
Sint32  MPG_GetInt(Sint32 *mpint)
{
    Sint32      ret;

    MPSYS_SemaIn(FALSE);
    
    /* MPEG割り込み変数の更新 */
    ret = updInt(MINT_USR, mpint);

    return MPSYS_SemaOut(ret);
}

/* MPEGのリセット */
Sint32  MPG_ResetMp(Bool mpsw)
{
    Sint32      ret;
    
    MPSYS_SemaIn(FALSE);

    /* MPEGのリセット実行 */
    ret = resetMpSub(mpsw);
    if (ret != MPG_ERR_OK) {
        MPSYS_SetErrCode(ret);
    }

    return MPSYS_SemaOut(ret);
}

/* MPEGのリセット実行 */
static Sint32   resetMpSub(Bool mpsw)
{
    Sint32      ret;

    /* MPEGデコーダの初期化 */                  
    if ((ret = MPSYS_InitMpd(mpsw)) != MPG_ERR_OK) {
        return ret;
    }

    if (mpsw == OFF) {
        return MPG_ERR_OK;
    }

    /* MPEGの初期化実行 */
    ret = doInit(scan_mode);

    return ret;
}

/* エラー状態の取得 */
Sint32  MPG_GetErrStat(MpgErrStat *stat)
{
    *stat = mp_err_stat;

    return MPG_ERR_CODE(stat);
}

/* エラー関数の登録 */
void    MPG_SetErrFunc(MpgErrFunc func, void *obj)
{
    MpgErrStat  *errstat;

    errstat = &mp_err_stat;

    MPG_ERR_FUNC(errstat) = func;
    MPG_ERR_OBJ(errstat) = obj;

    return;
}

/* エラーコードの設定（エラーならばエラー関数を呼び出す） */
void    MPSYS_SetErrCode(Sint32 errcode)
{
    MpgErrFunc func;

    if (errcode == MPG_ERR_OK) {
        return;
    }

    MPG_ERR_CODE(&mp_err_stat) = errcode;
    func = MPG_ERR_FUNC(&mp_err_stat);
    if (func != NULL) {
        func(MPG_ERR_OBJ(&mp_err_stat), errcode);
    }
    return;
}

/* MPEGの現在のポーズタイミングの設定 */
void    MPSYS_SetPauTim(Sint32 pautim)
{
    if (pautim != MPG_IGNORE) {
        /* 変数の設定 */
        mp_pautim = pautim;
    }
    return;
}

/* MPG関数入口での処理 */
void    MPSYS_SemaIn(Bool flag)
{
    semaMpg(SEMA_IN, flag);

    return;
}

/* MPG関数の入口／出口の処理（セマフォ処理） */
static void     semaMpg(Sint32 incnt, Bool cptflg)
{
    if ((incnt == SEMA_IN) && (cptflg == TRUE)) {
        MPG_CaptStat();
    }
    mp_in_cnt += incnt;

    if ((incnt == SEMA_OUT) && (mp_capt_flag == TRUE)) {
        MPG_CaptStat();
    }
    return;
}

/* MPG関数出口での処理 */
Sint32  MPSYS_SemaOut(Sint32 err)
{
    if (err != MPG_ERR_OK) {
        MPSYS_SetErrCode(err);
    }
    semaMpg(SEMA_OUT, FALSE);

    return err;
}

/* MPEG動作モードの設定関数実行 */
Sint32  MPSYS_DoSetMode(Sint32 actmod, Sint32 dectim, Sint32 out,
                        Sint32 scnmod)
{
    Sint32      ret;
    Sint32      i;

    i = 0;
    while (TRUE) {
        ret = CDC_MpSetMode(actmod, dectim, out, scnmod);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* MPSYS_Do～関数でタイムアウトしたか？ */
static Bool     isTimeOut(Sint32 *cnt)
{
    if ((*cnt)++ > RTRY_NUM) {
        return TRUE;
    }

    /* ソフトウエアタイマ */
    MPSYS_SoftTimer(100);

    return FALSE;
}

/* ソフトウェアタイマ（1回のループで8ステート：26MHzで約0.3μｓ）*/
void    MPSYS_SoftTimer(Sint32 cnt)
{
    volatile Sint32     tmpval;
    tmpval = cnt;
    while (tmpval > 0) {
        tmpval--;
    }
    return;
}

/* MPEGの再生関数実行 */
Sint32  MPSYS_DoPlay(Sint32 plymod, Sint32 tmod_a, Sint32 tmod_v,
                     Sint32 dec_v)
{
    Sint32      ret;
    Sint32      i;

    i = 0;
    while (TRUE) {
        ret = CDC_MpPlay(plymod, tmod_a, tmod_v, dec_v);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* デコード方法の設定関数実行 */
Sint32  MPSYS_DoSetDec(Sint32 mute, Sint32 pautim, Sint32 frztim)
{
    Sint32      ret;
    Sint32      i;

    i = 0;
    while (TRUE) {
        ret = CDC_MpSetDec(mute, pautim, frztim);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* デコード同期信号の送出関数実行 */
Sint32  MPSYS_DoOutDsync(Sint32 fbn)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpOutDsync(fbn);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* ビデオのタイムコードの取得関数実行 */
Sint32  MPSYS_DoGetTc(Sint32 *bnk, Sint32 *pictyp, Sint32 *tr, CdcMpTc *mptc)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpGetTc(bnk, pictyp, tr, mptc);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGデコーダの接続先の設定関数実行 */
Sint32  MPSYS_DoSetCon(Sint32 next, CdcMpCon *mpcon_a, CdcMpCon *mpcon_v)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpSetCon(next, mpcon_a, mpcon_v);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGデコーダの接続先の切り替え関数実行 */
Sint32  MPSYS_DoChgCon(Sint32 chg_a, Sint32 chg_v,
                       Sint32 clr_a, Sint32 clr_v)
{
    Sint32      ret;
    Sint32      i;
    
    i = 0;
    while (TRUE) {
        ret = CDC_MpChgCon(chg_a, chg_v, clr_a, clr_v);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGストリームの設定関数実行 */
Sint32  MPSYS_DoSetStm(Sint32 next, CdcMpStm *mpstm_a, CdcMpStm *mpstm_v)
{
    Sint32      ret;
    Sint32      i;

    i = 0;
    while (TRUE) {
        ret = CDC_MpSetStm(next, mpstm_a, mpstm_v);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* 画像の表示関数実行 */
Sint32  MPSYS_DoDisp(Bool dswsw, Sint32 fbn)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpDisp(dswsw, fbn);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* フェードの設定関数実行 */ 
Sint32  MPSYS_DoSetFade(Sint32 gain_y, Sint32 gain_c)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpSetFade(gain_y, gain_c);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* 画面特殊効果の設定関数実行 */ 
Sint32  MPSYS_DoSetVeff(CdcMpVef *mpvef)
{
    Sint32      ret;
    Sint32      i;

    i = 0;    
    while (TRUE) {
        ret = CDC_MpSetVeff(mpvef);
        if (ret == CDC_ERR_OK) {
            break;
        }
        if (ret != CDC_ERR_WAIT) {
            return MPG_ERR_NG;
        }
        if (isTimeOut(&i)) {
            return MPG_ERR_WAIT;
        }
    }
    return MPG_ERR_OK;
}

/* MPEGの現在の状態の取得 */
void    MPSYS_GetMpgStat(CdcMpStat *mpstat)
{
    /* 保時変数の取得 */
    *mpstat = mp_stat;
    
    return;
}

