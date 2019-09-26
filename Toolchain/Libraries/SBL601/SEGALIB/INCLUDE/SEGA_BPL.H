/*****************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1995 SEGA
 *
 * Library: 分岐再生ライブラリ
 * Module : 公開用ヘッダファイル
 * File   : sega_bpl.h
 * Date   : 1995-03-31
 * Version: 1.00
 * Auther : T.K
 *
 *****************************************************************************/
#if     !defined(SEGA_BPL_H)
#define SEGA_BPL_H

/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/
#include        "sega_xpt.h"
#include        "sega_stm.h"

/*****************************************************************************
 *      定数マクロ
 *****************************************************************************/

/* テーブルサイズ       */
#define BPL_STMKEY_MAX  6       /* １つの分岐ストリームに指定可能なキーの数 */
#define BPL_BLK_NUM     24      /* CDバッファの区画数 */

/* 分岐しない場合の分岐テーブル設定値   */
#define BPL_BR_NONE     (-1)

/* 分岐再生状態         */
enum    BplSvrStat {
    BPL_SVR_COMPLETED   = 0x200,        /* 分岐再生終了                 */
    BPL_SVR_WAITSEL     = 0x201,        /* 分岐先選択待ち               */
    BPL_SVR_SELECT      = 0x202,        /* 分岐先決定                   */
    BPL_SVR_NOBRN       = 0x203,        /* 分岐先なし                   */
    BPL_SVR_END
};


/* エラーコード         */
enum    BplErrCode {
    BPL_ERR_OK          = 0,            /* 正常終了                     */
    BPL_ERR_KYOVRFLW    = (-256),       /* ストリームキー設定が多すぎる */
    BPL_ERR_BROVRFLW    = (-255),       /* 分岐先の設定が多すぎる       */
    BPL_ERR_BSTMID      = (-254),       /* 不正なストリーム識別子       */
    BPL_ERR_BRNO        = (-253),       /* 不正な分岐番号               */
    BPL_ERR_BRSPC       = (-252),       /* 既に分岐先が指定されている   */
    BPL_ERR_NOKEY       = (-251),       /* 該当ストリームキーが未設定   */
    BPL_ERR_OPNSTM      = (-250),       /* ストリームオープンエラー     */
    BPL_ERR_END
};


/*****************************************************************************
 *      処理マクロ
 *****************************************************************************/

/* 作業領域のサイズ取得         */
#define BPL_WORK_SIZE(stmmax, brmax, keymax)    (sizeof(BplMng) + \
                                                (sizeof(BplStm) * stmmax) +\
                                                (sizeof(Sint32) * brmax) +\
                                                (sizeof(StmKey) * keymax))


/*****************************************************************************
 *      データ型の定義
 *****************************************************************************/

/* ストリームの定義     */
typedef struct {
    Sint32      fid;                    /* ファイル識別子               */
    Sint32      nbr;                    /* 分岐先の数                   */
    Sint32      branch;                 /* 分岐テーブルの位置           */
    Sint32      nkey;                   /* 使用するストリームキーの数   */
    Sint32      keyno[BPL_STMKEY_MAX];  /* ストリームキー番号テーブル   */
} BplStm;

/* 分岐動画再生管理テーブル     */
typedef struct {
    BplStm      *bstmtbl;               /* 分岐ストリーム登録領域       */
    Sint32      bstmmax;                /* 分岐ストリームの登録最大数   */
    Sint32      *brtbl;                 /* 分岐テーブル                 */
    Sint32      brmax;                  /* 分岐の登録最大数             */
    Sint32      nbr;                    /* 分岐テーブルの有効数         */
    StmKey      *keytbl;                /* ストリームキー登録領域       */
    Sint32      keymax;                 /* ストリームキーの最大数       */
    Sint32      nkey;                   /* キーテーブルの有効数         */
    Sint32      curstmid;               /* 現在のストリーム識別子       */
    Sint32      nextbrno;               /* 次の分岐番号                 */
    StmGrpHn    grphn;                  /* ストリームグループハンドル   */
    StmFrange   frange;                 /* 現在のストリーム再生範囲     */
    StmHn       curstmhn[BPL_STMKEY_MAX];/* 現在のストリームハンドル    */
    StmHn       nextstmhn[BPL_BLK_NUM]; /* 分岐先のストリームハンドル   */
    Uint32      atr;                    /* アトリビュート               */
} BplMng;

/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/

/* 分岐再生の初期化             */
Sint32  BPL_Init(Sint32 bstmmax, Sint32 brmax, Sint32 keymax, void *work);

/* 分岐再生のリセット           */
Sint32  BPL_Reset(void);

/* 分岐ストリーム情報の設定     */
Sint32  BPL_SetStmInfo(Sint32 bstmid, Sint32 fid, Sint32 nkey, StmKey *keytbl);

/* 分岐ストリーム情報の取得     */
Sint32  BPL_GetStmInfo(Sint32 bstmid, 
                       Sint32 *fid, Sint32 *nkey, StmKey *keytbl);

/* 分岐先情報の設定             */
Sint32  BPL_SetBranchInfo(Sint32 bstmid, Sint32 nbranch, Sint32 *brtbl);

/* 分岐先情報の取得             */
Sint32  BPL_GetBranchInfo(Sint32 bstmid, Sint32 *nbranch, Sint32 *brtbl,
                          Sint32 nelem);

/* 再生開始ストリームの指定     */
Sint32  BPL_SetStart(Sint32 bstmid);

/* 分岐再生サーバの実行         */
Sint32  BPL_ExecServer(Bool chgsw);

/* 分岐先の選択                 */
Sint32  BPL_SelectBranch(Sint32 brno);

/* 現在のストリームの取得       */
Sint32  BPL_GetCurStm(Sint32 nelem, StmHn *stmtbl);

/* 分岐先のストリームの取得 */
Sint32  BPL_GetNextStm(Sint32 nelem, StmHn *stmtbl);

/* ストリームグループの取得     */
StmGrpHn  BPL_GetStmGrp(void);

#endif

/* end of file */
