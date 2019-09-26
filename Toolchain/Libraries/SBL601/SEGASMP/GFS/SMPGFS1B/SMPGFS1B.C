/*****************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :サンプル１
 * File   :smpgfs01.c
 * Date   :1996-02-01
 * Version:2.10
 * Author :M.S.
 *
 *      ファイルの単純な読み込み。
 *      gfssmp1a.absはファイル識別子でアクセスし、
 *      gfssmp1b.absはファイル名でアクセスする。
 *
 *****************************************************************************/

/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/
#include <sega_gfs.h>

/*****************************************************************************
 *      定数マクロ
 *****************************************************************************/
/* 同時にオープンするファイルの数 */
#define MAX_OPEN        1

/* ルートディレクトリにあるファイル数 */
#define MAX_DIR         100

/* １セクタの大きさ（バイト） */
#define SECT_SIZE       2048

/* 読み込みセクタ数 */
#define RD_SIZE         1

#if defined(DEBUG)
/* アクセスするファイル名 */
#define FNAME           "SIZE10S.DAT"
#else
/* アクセスするファイル識別子 */
#define FID             2
#endif

/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/
GfsHn openFileByName(GfsDirTbl *dirtbl, Sint8 *fname);

/*****************************************************************************
 *      変数の定義
 *****************************************************************************/
/* ライブラリ作業領域 */
Uint32 lib_work[GFS_WORK_SIZE(MAX_OPEN) / sizeof(Uint32)];

GfsDirTbl dirtbl;                       /* ディレクトリ情報管理領域 */

/* ディレクトリ情報格納領域 */
#if defined(DEBUG)
GfsDirName dirname[MAX_DIR];            /* ファイル名を含んだ情報 */
#else
GfsDirId dirid[MAX_DIR];                /* ファイル名を含まない情報 */
#endif

Uint32 buf[SECT_SIZE * RD_SIZE / sizeof(Uint32)];       /* 読み込みデータ格納領域 */


/*****************************************************************************
 *      関数の定義
 *****************************************************************************/
Sint32 main()
{
    Sint32 ret;
    GfsHn gfs;                  /* ファイルハンドル */

    /* ディレクトリ情報管理領域のsetup */
#if defined(DEBUG)
    GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_NAME;
    GFS_DIRTBL_DIRNAME(&dirtbl) = dirname;
#else
    GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_ID;
    GFS_DIRTBL_DIRID(&dirtbl) = dirid;
#endif
    GFS_DIRTBL_NDIR(&dirtbl) = MAX_DIR;
    /* ファイルシステムの初期化 */
    ret = GFS_Init(MAX_OPEN, lib_work, &dirtbl);
    if (ret <= 2) {
        return 1;
    }
    /* ファイルを開く */
#if defined(DEBUG)
    gfs = openFileByName(&dirtbl, FNAME);
#else
    gfs = GFS_Open(FID);
#endif
    if (gfs == NULL) {
        return 2;
    }
    /* ファイルを読み込む */
    GFS_Fread(gfs, RD_SIZE, buf, SECT_SIZE * RD_SIZE);
    /* ファイルを閉じる */
    GFS_Close(gfs);
    return 0;
}


#if defined(DEBUG)
/* ファイル名を指定してファイルを開く */
GfsHn openFileByName(GfsDirTbl *dirtbl, Sint8 *fname)
{
    Sint32 fid;

    /* ファイル名からファイル識別子を求める */
    fid = GFS_NameToId(fname);
    return GFS_Open(fid);
}
#endif









