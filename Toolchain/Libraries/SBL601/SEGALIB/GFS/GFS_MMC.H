/******************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1994,1995,1996 SEGA
 *
 * Library:ファイルシステム
 * Module :SIMMファイル管理モジュールヘッダ
 * File   :gfs_mmc.h
 * Date   :1996-02-01
 * Version:2.10
 * Author :H.T.
 *
 *************************************************************************** */

#if !defined(GFS_MMC_H)
#define GFS_MMC_H       1


/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/

/*****************************************************************************
 *      定数マクロ
 *****************************************************************************/

#define MMC_ERROR       (-1)

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

/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/

/* SIMMファイルイメージのロードアドレスを取得する */
void *GFMC_GetBase(void);

/* SIMMファイル情報を取得する */
void GFMC_GetFileInfo(Sint32 fid, void **area, Sint32 *size, Sint32 *sctsiz);

/* SIMMファイルの数を取得する */
Sint32 GFMC_GetNumFile(void);

/* SIMMファイル名を取得する */
Sint8 *GFMC_GetFname(Sint32 fid);

/* リザーブ領域を設定する */
void GFMC_SetRsv(Sint32 fid, Sint16 rsv);

/* リザーブ領域を取得する */
Sint16 GFMC_GetRsv(Sint32 fid);

#endif

/* end of file */






