/*******************************************************************
 *
 *                     サターン用ＣＤライブラリ
 *
 *                         MPEGライブラリ
 *
 *             Copyright (c) 1993 CSK Research Institute Corp.
 *             Copyright (c) 1994 SEGA
 *
 *  Module : MPEGライブラリ用局所ヘッダファイル
 *  File   : mpg_loc.h
 *  Date   : 1996-01-31
 *  Author : H.Take
 *
 *******************************************************************/

/* 多重インクルードへの対応 */
#ifndef MPG_LOC_H
#define MPG_LOC_H

/*******************************************************************
 *      インクルードファイル
 *******************************************************************/
#include        "sega_mpg.h"

/*******************************************************************
 *      定数マクロ
 *******************************************************************/

/* MPEGデコーダの初期化のタイムアウト時間(約320ms) */
#define MPL_INIT_CNT            1000000

/* 動画用フレームバンク（０で固定） */
#define MPL_FBN_MV              0

/* MPEGフレームバッファの面数 */
#define MPL_FRM_NUM             (4)

/*******************************************************************
 *      列挙定数
 *******************************************************************/

/*******************************************************************
 *      構造体アクセス処理マクロ
 *******************************************************************/

/*******************************************************************
 *      処理マクロ
 *******************************************************************/

/*******************************************************************
 *      データ型の宣言
 *******************************************************************/

/* イベント起動関数 */
typedef Sint32 (*MplEventFunc)(void *obj);

/*******************************************************************
 *      変数宣言
 *******************************************************************/

/*******************************************************************
 *      関数宣言
 *******************************************************************/

/* mpg_sys.c */
Sint32  MPSYS_ReqVideoCon(Sint32 next);
Sint32  MPSYS_ReqAudioCon(Sint32 next);
Bool    MPSYS_IsPauseVideo(Sint32 *err);
void    MPSYS_EntryEventFunc(MplEventFunc func, void *obj);
void    MPSYS_KillEventFunc(void);
Bool    MPSYS_IsVbvEmpty(Sint32 *err);
Sint32  MPSYS_InitMpd(Bool sw);
Sint32  MPSYS_CaptSub(void);
void    MPSYS_SetErrCode(Sint32 errcode);
void    MPSYS_SetPauTim(Sint32 pautim);
void    MPSYS_SemaIn(Bool flag);
Sint32	MPSYS_SemaOut(Sint32 err);
Sint32  MPSYS_DoSetMode(Sint32 actmod, Sint32 dectim, Sint32 out,
                        Sint32 scnmod);
void    MPSYS_SoftTimer(Sint32 cnt);
Sint32  MPSYS_DoPlay(Sint32 plymod, Sint32 tmod_a, Sint32 tmod_v,
                     Sint32 dec_v);
Sint32  MPSYS_DoSetDec(Sint32 mute, Sint32 pautim, Sint32 frztim);
Sint32  MPSYS_DoOutDsync(Sint32 fbn);
Sint32  MPSYS_DoGetTc(Sint32 *bnk, Sint32 *pictyp, Sint32 *tr, CdcMpTc *mptc);
Sint32  MPSYS_DoSetCon(Sint32 next, CdcMpCon *mpcon_a, CdcMpCon *mpcon_v);
Sint32  MPSYS_DoChgCon(Sint32 chg_a, Sint32 chg_v,
                       Sint32 clr_a, Sint32 clr_v);
Sint32  MPSYS_DoSetStm(Sint32 next, CdcMpStm *mpstm_a, CdcMpStm *mpstm_v);
Sint32  MPSYS_DoDisp(Bool dswsw, Sint32 fbn);
Sint32  MPSYS_DoSetFade(Sint32 gain_y, Sint32 gain_c);
Sint32  MPSYS_DoSetVeff(CdcMpVef *mpvef);
void    MPSYS_GetMpgStat(CdcMpStat *mpstat);

/* mpg_mve.c */
Sint32  MPMVE_DecNext(MpgMv mpgmv, Sint32 bnk);
Sint32  MPMVE_Output(MpgMv mpgmv, Sint32 upc, Sint32 bnk);
void    MPMVE_InitAllMv(void);

/* mpg_win.c */
Sint32  MPWIN_DispSub(MpgWn mpgwn, Bool sw);
void    MPWIN_InitAllWn(void);
void    MPWIN_Connect(MpgWn mpgwn1, MpgWn mpgwn2);
void    MPWIN_Disconnect(MpgWn mpgwn);
void    MPWIN_OutputAll(MpgWn mpgwn, Sint32 omode);

#endif  /* ifndef MPG_LOC_H */


