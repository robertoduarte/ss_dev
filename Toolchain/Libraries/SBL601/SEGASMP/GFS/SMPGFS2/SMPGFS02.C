/*****************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1994 SEGA
 *
 * Library:ソフトウェアライブラリ
 * Module :サンプル２
 * File   :smpgfs02.c
 * Date   :1994-02-01
 * Version:2.10
 * Author :M.S.
 *
 *      複数ファイルをアクセスします。
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
#define MAX_OPEN        3

/* ルートディレクトリにあるファイル数 */
#define MAX_DIR         100

/* １セクタの大きさ（バイト） */
#define SECT_SIZE       2048

/* 読み込みセクタ数 */
#define RD_SIZE1        2
#define RD_SIZE2        3
#define RD_SIZE3        4

/* アクセスするファイル名 */
#define FNAME1          "SIZE10S.DAT"
#define FNAME2          "SIMM1.DAT"
#define FNAME3          "SIMM_DOS.DAT"

/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/
GfsHn openFileByName(GfsDirTbl *dirtbl, Sint8 *fname);
void user(void);

/*****************************************************************************
 *      変数の定義
 *****************************************************************************/
GfsDirTbl dirtbl;                       /* ディレクトリ情報管理領域 */

/* ディレクトリ情報格納領域 */
GfsDirName dir_name[MAX_DIR];           /* ファイル名を含んだ情報 */

/* ライブラリ作業領域 */
Uint32 lib_work[GFS_WORK_SIZE(MAX_OPEN) / sizeof(Uint32)];

Uint32 buf1[SECT_SIZE * RD_SIZE1 / sizeof(Uint32)];     /* 読み込みデータ格納領域 */
Uint32 buf2[SECT_SIZE * RD_SIZE2 / sizeof(Uint32)];     /* 読み込みデータ格納領域 */
Uint32 buf3[SECT_SIZE * RD_SIZE3 / sizeof(Uint32)];     /* 読み込みデータ格納領域 */

/*****************************************************************************
 *      関数の定義
 *****************************************************************************/

Sint32 main()
{
    GfsHn gfs1;                 /* ファイルハンドル1 */
    GfsHn gfs2;                 /* ファイルハンドル2 */
    GfsHn gfs3;                 /* ファイルハンドル3 */
    GfsHn now_gfs;
    Sint32 stat, nfile;

    /* ディレクトリ情報管理領域のsetup */
    GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_NAME;
    GFS_DIRTBL_NDIR(&dirtbl) = MAX_DIR;
    GFS_DIRTBL_DIRNAME(&dirtbl) = dir_name;
    /* ファイルシステムの初期化 */
    nfile = GFS_Init(MAX_OPEN, lib_work, &dirtbl);
    if (nfile <= 2) {
        return 1;
    }
    /* ファイルを開く */
    gfs1 = openFileByName(&dirtbl, FNAME1);
    if (gfs1 == NULL) {
        return 2;
    }
    gfs2 = openFileByName(&dirtbl, FNAME2);
    if (gfs2 == NULL) {
        return 3;
    }
    gfs3 = openFileByName(&dirtbl, FNAME3);
    if (gfs3 == NULL) {
        return 4;
    }
    /* ファイル読み込み開始 */
    GFS_NwFread(gfs1, RD_SIZE1, buf1, SECT_SIZE * RD_SIZE1);
    GFS_NwFread(gfs2, RD_SIZE2, buf2, SECT_SIZE * RD_SIZE2);
    GFS_NwFread(gfs3, RD_SIZE3, buf3, SECT_SIZE * RD_SIZE3);
    for (;;) {
        stat = GFS_NwExecServer(&now_gfs);
        if (stat == GFS_SVR_COMPLETED) {
            break;
        }
        user();
    }
    /* ファイルを閉じる */
    GFS_Close(gfs1);
    GFS_Close(gfs2);
    GFS_Close(gfs3);
    return 0;
}


/* ファイル名を指定してファイルを開く */
GfsHn openFileByName(GfsDirTbl *dirtbl, Sint8 *fname)
{
    Sint32 fid;

    /* ファイル名からファイル識別子を求める */
    fid = GFS_NameToId(fname);
    return GFS_Open(fid);
}


void user(void)
{
    /* ファイル読み込みと並行して行ないたい処理を記述する */
    /* このサンプルでは何も行なわない */
}
