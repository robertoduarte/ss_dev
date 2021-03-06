/******************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1994,1995,1996 SEGA
 *
 * Library:ファイルシステム
 * Module :CDファイルヘッダ
 * File   :gfs_cdf.h
 * Date   :1996-02-01
 * Version:2.10
 * Author :H.T.
 *
 *************************************************************************** */

#if !defined(GFS_CDF_H)
#define GFS_CDF_H

/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/

/*****************************************************************************
 *      定数マクロ
 *****************************************************************************/

/*****************************************************************************
 *      列挙定数
 *****************************************************************************/

/*****************************************************************************
 *      処理マクロ
 *****************************************************************************/

/*****************************************************************************
 *      データ型の定義
 *****************************************************************************/

/*****************************************************************************
 *      変数の宣言
 *****************************************************************************/
extern Sint32   gfcf_stat;
extern Sint32   gfcf_fad;

/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/

/*  CDFの初期化 */
Sint32 GFCF_Init(Bool use_cdbfs);

/*  読み込み管理構造体の初期化 */
Bool GFCF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid);

/*  読み込みセクタ数設定 */
void GFCF_SetSct(GfsFlow *flow, Sint32 sct);

/*  ファイル情報の取得  */
void GFCF_GetFileInfo(GfsFlow *flow, Sint32 *fid, Sint32 *fad, Sint32 *sctsiz,
                      Sint32 *nsct, Sint32 *lastsiz, Sint32 *fno, Sint32 *atr);

/*  ＣＤバッファへのデータ読み込み */
Sint32 GFCF_FlowInBuf(GfsFlow *flow);

/*  ＣＤバッファへのデータ読み込み中止 */
void GFCF_StopInBuf(GfsFlow *flow, Bool stop_flag);

/* リードエラー発生時のリカバリ         */
Sint32  GFCF_Recover(GfsFlow *flow);

/*  アクセスポインタ取得 */
Sint32 GFCF_Tell(GfsFlow *flow);

/*  アクセスポインタ設定 */
Sint32 GFCF_Seek(GfsFlow *flow, Sint32 off);

/*  ピックアップ移動 */
Sint32 GFCF_MovePickup(GfsFlow *flow, Sint32 off);

/* インターリーブのギャップを含めたオフセットの計算 */
Sint32 GFCF_cnvIntlvOfs(GfsFinfo *finfo, Sint32 ofs);

/*  ステータス取得 */
Sint32 GFCF_GetStat(Sint32 *fad, Sint32 *hirq);

#endif      /* GFS_CDF_H    */

/* end of file */
