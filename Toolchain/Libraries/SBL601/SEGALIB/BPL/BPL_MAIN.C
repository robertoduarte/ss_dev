/*****************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1995 CSK Research Institute Corp.
 *      Copyright (c) 1995 SEGA
 *
 * Library: 分岐再生ライブラリ
 * Module : メインモジュール
 * File   : bpl_main.c
 * Date   : 1995-03-31
 * Version: 1.00
 * Auther : T.K
 *
 *****************************************************************************/

/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/
#include        <string.h>

#include        "sega_bpl.h"
#include        "bpl_def.h"

/*****************************************************************************
 *      定数マクロ
 *****************************************************************************/
/* 局所識別子を示すマクロ */
#define BPL_LOCAL


/*****************************************************************************
 *      処理マクロ
 *****************************************************************************/
/* 分岐ストリーム識別子の範囲チェック */
#define IS_BSTMID_NG(bstmid) \
        (((bstmid) < 0) || ((bstmid) >= BPD_MNG_BSTMMAX(bpl_mng_ptr)))


/*****************************************************************************
 *      関数の宣言
 *****************************************************************************/

/* ストリームのオープン */
BPL_LOCAL Sint32 bpl_openStm(Sint32 bstmid, StmHn *tbl, Sint32 ofs,
                             Sint32 max);

/* 分岐先のストリームのオープン */
BPL_LOCAL Sint32 bpl_openNextStm(Sint32 bstmid);

/* アクセス終了チェック */
BPL_LOCAL Sint32 bpl_getAccess(void);

/* 分岐先以外の分岐候補をクローズする */
BPL_LOCAL void  bpl_closeNextStm(void);

/* 読み込み中のストリームの強制終了 */
BPL_LOCAL void  bpl_closeCurStm(void);

/* 分岐先のハンドルを現在のハンドルとする */
BPL_LOCAL void  bpl_changeStm(void);

/* 分岐先の転送ゲートを開ける */
BPL_LOCAL void  bpl_openTrGate(void);

/* パラメータの初期化 */
BPL_LOCAL void bpl_initPara(Sint32 bstmmax, Sint32 brmax, Sint32 keymax);

/* 分岐再生の中止 */
BPL_LOCAL void bpl_stopStm(void);

/* 分岐再生状態のチェック */
BPL_LOCAL Sint32 bpl_checkStatus(Sint32 atr);

/* 分岐処理の実行 */
BPL_LOCAL Sint32 bpl_execBranch(Sint32 stat);


/*****************************************************************************
 *      変数の定義
 *****************************************************************************/

/* 分岐動画再生管理領域 */
BplMng  *bpl_mng_ptr;

/*****************************************************************************
 *      関数の定義
 *****************************************************************************/

/****************************************************
 * ストリームのオープン
 * bstmid       : ストリーム識別子
 * tbl          : ストリームハンドル設定テーブル
 * ofs          : 設定開始位置
 * max          : テーブルサイズ
 * 関数値       : 設定個数
 ****************************************************/
BPL_LOCAL Sint32 bpl_openStm(Sint32 bstmid, StmHn *tbl, Sint32 ofs, Sint32 max)
{
    BplStm      *bs;
    Sint32      keyno;
    Sint32      i;
    Sint32      nkey;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);

    /* ストリームキーの設定が多過ぎる   */
    if (BPD_STM_NKEY(bs) > (max - ofs)) {
        return BPL_ERR_KYOVRFLW;
    }

    tbl += ofs;
    nkey = BPD_STM_NKEY(bs);
    for (i = 0; i < nkey; i++) {
        keyno = BPD_STM_KEYNO(bs, i);
        *tbl = STM_OpenFid(BPD_MNG_GRPHN(bpl_mng_ptr), BPD_STM_FID(bs), 
                         BPD_MNG_KEYPTR(bpl_mng_ptr, keyno), STM_LOOP_NOREAD);
        if (*tbl == NULL) {
            return BPL_ERR_OPNSTM;
        }
        tbl++;
    }
    return i;
}


/****************************************************
 * 分岐先のストリームのオープン
 * bstmid       : ストリーム識別子
 * 関数値       : エラーコード
 ****************************************************/
BPL_LOCAL Sint32 bpl_openNextStm(Sint32 bstmid)
{
    BplStm      *bs;
    Sint32      branch;
    Sint32      ofs;
    Sint32      i;
    Sint32      ec;
    Sint32      stmnbr;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }

    /* 各ストリームのオープン */
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    ofs = 0;
    branch = BPD_STM_BRANCH(bs);
    stmnbr = BPD_STM_NBR(bs);
    for (i = 0; i < stmnbr; i++) {
        if (BPD_MNG_BR(bpl_mng_ptr, branch) >= 0) {
            ec = bpl_openStm(BPD_MNG_BR(bpl_mng_ptr, branch), 
                             BPD_MNG_NEXTSTM(bpl_mng_ptr), ofs, BPL_BLK_NUM);
            if (ec < 0) {
                return ec;
            }
            ofs += ec;
            BPD_MNG_ATR(bpl_mng_ptr) = BPD_ATR_NEXT_OPEN;
        }
        branch++;
    }
    for (i = 0; i < ofs; i++) {
        STM_SetTrGate(BPD_MNG_NEXTSTMHN(bpl_mng_ptr, i), STM_GATE_CLOSE);
    }
    return BPL_ERR_OK;
}


/****************************************************
 * アクセス終了チェック
 * 関数値 : アクセス状態
 ****************************************************/
BPL_LOCAL Sint32 bpl_getAccess(void)
{
    Sint32      i;
    StmHn       stm;
    Sint32      nsct;
    Sint32      fad;
    Sint32      sfad;
    StmFrange   frange;
    Bool        complete;

    nsct = 0;
    complete = TRUE;
    for (i = 0; i < BPL_STMKEY_MAX; i++) {
        stm = BPD_MNG_CURSTMHN(bpl_mng_ptr, i);
        if (stm != NULL) {
            if (STM_IsComplete(stm) == FALSE) {
                complete = FALSE;
            }
            nsct += STM_GetNumCdbuf(stm);
        } else {
            break;
        }
    }
    if ((complete == TRUE) && (nsct == 0)) {
        return BPD_ACCESS_END;
    }
    STM_GetExecStat(BPD_MNG_GRPHN(bpl_mng_ptr), &fad);
    frange = BPD_MNG_FRANGE(bpl_mng_ptr);
    sfad = STM_FRANGE_SFAD(&frange);
    if (fad <= sfad) {
        return BPD_ACCESS_BEFORE;
    }
    if ((fad >= (sfad + STM_FRANGE_FASNUM(&frange))) && (nsct == 0)) {
        return BPD_ACCESS_BEFORE;
    }
    return BPD_ACCESS_DOING;
}


/****************************************************
 * 分岐先以外の分岐候補をクローズする
 ****************************************************/
BPL_LOCAL void  bpl_closeNextStm(void)
{
    BplStm      *bs;
    Sint32      br;
    Sint32      brno;
    Sint32      nstm;
    Sint32      bstmid;
    Sint32      stmnbr;
    Sint32      stmnkey;
    Sint32      i, j;

    if (BPD_MNG_CURSTMID(bpl_mng_ptr) < 0) {
        return ;
    }
    bs = BPD_MNG_BSTM(bpl_mng_ptr, BPD_MNG_CURSTMID(bpl_mng_ptr));
    br = BPD_STM_BRANCH(bs);
    brno = BPD_MNG_NEXTBRNO(bpl_mng_ptr);
    nstm = 0;
    stmnbr = BPD_STM_NBR(bs);
    for (i = 0; i < stmnbr; i++, br++) {
        bstmid = BPD_MNG_BR(bpl_mng_ptr, br);
        if (bstmid < 0) {
            continue;
        }
        if (i != brno) {        /* 分岐先以外の候補はクローズする       */
            stmnkey = BPD_STM_NKEY(BPD_MNG_BSTM(bpl_mng_ptr, bstmid));
            for (j = 0; j < stmnkey; j++) {
                STM_Close(BPD_MNG_NEXTSTMHN(bpl_mng_ptr, nstm));
                BPD_MNG_NEXTSTMHN(bpl_mng_ptr, nstm) = NULL;
                nstm++;
            }
        } else {
            nstm += BPD_STM_NKEY(BPD_MNG_BSTM(bpl_mng_ptr, bstmid));
        }
    }

    return;
}


/****************************************************
 * 読み込み中のストリームの強制終了
 ****************************************************/
BPL_LOCAL void  bpl_closeCurStm(void)
{
#if	0
	/*
	**■1995-07-26 高橋智延
	**	使ってないので削除する。
	*/
    BplStm      *bs;
#endif
    StmHn       stm;
    Sint32      i;

    /* 読み込み中のストリームをクローズする     */
    for (i = 0; i < BPL_STMKEY_MAX; i++) {
        stm = BPD_MNG_CURSTMHN(bpl_mng_ptr, i);
        if (stm != NULL) {
            STM_Close(stm);
            BPD_MNG_CURSTMHN(bpl_mng_ptr, i) = NULL;
        }
    }

    return;
}
    


/****************************************************
 * 分岐先のハンドルを現在のハンドルとする
 ****************************************************/
BPL_LOCAL void  bpl_changeStm(void)
{
    BplStm      *bs;
    Sint32      bstmid;
    Sint32      nbr;
    Sint32      brcnt, nxcnt, crcnt, kycnt;
    Sint32      br;
    Sint32      stmnkey;

    /* 分岐先が決定していない場合は実行できない */
    if (BPD_MNG_NEXTBRNO(bpl_mng_ptr) < 0) {
        return;
    }

    bstmid = BPD_MNG_CURSTMID(bpl_mng_ptr);
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    nbr = BPD_STM_NBR(bs);
    nxcnt = crcnt = 0;
    br = BPD_STM_BRANCH(bs);
    for (brcnt = 0; brcnt < nbr; brcnt++, br++) {
        bstmid = BPD_MNG_BR(bpl_mng_ptr, br);
        if (bstmid < 0) {
            continue;
        }
        bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
        stmnkey = BPD_STM_NKEY(bs);
        for (kycnt = 0; kycnt < stmnkey; kycnt++) {
            if (BPD_MNG_NEXTSTMHN(bpl_mng_ptr, nxcnt) != NULL) {
                if (brcnt == BPD_MNG_NEXTBRNO(bpl_mng_ptr)) {
                    /* 必要なストリームは処理中のテーブルに移動 */
                    BPD_MNG_CURSTMHN(bpl_mng_ptr, crcnt) = 
                        BPD_MNG_NEXTSTMHN(bpl_mng_ptr, nxcnt);
                    STM_SetTrGate(BPD_MNG_CURSTMHN(bpl_mng_ptr, crcnt),
                                  STM_GATE_OPEN);
                    crcnt++;
                }
                BPD_MNG_NEXTSTMHN(bpl_mng_ptr, nxcnt) = NULL;
            }
            nxcnt++;
        }
    }
    BPD_MNG_CURSTMID(bpl_mng_ptr) = BPD_MNG_NEXTSTMID(bpl_mng_ptr);
    BPD_MNG_NEXTBRNO(bpl_mng_ptr) = BPL_BR_NONE;
    BPD_MNG_ATR(bpl_mng_ptr) = BPD_ATR_NEXT_CLOSE;

    return;
}


/****************************************************
 * 分岐先の転送ゲートを開ける
 ****************************************************/
BPL_LOCAL void  bpl_openTrGate(void)
{
    Sint32      i;
    StmHn       stm;

    /* 分岐先が決定していない場合は実行できない */
    if (BPD_MNG_NEXTBRNO(bpl_mng_ptr) < 0) {
        return;
    }
    for (i = 0; i < BPL_BLK_NUM; i++) {
        stm = BPD_MNG_NEXTSTMHN(bpl_mng_ptr, i);
        if (stm != NULL) {
            STM_SetTrGate(stm, STM_GATE_OPEN);
        }
    }

    return;
}


/****************************************************
 * パラメータの初期化
 * bstmmax      : 分岐ストリーム数
 * brmax        : 分岐の総数
 * keymax       : ストリームキー数
 ****************************************************/
BPL_LOCAL void bpl_initPara(Sint32 bstmmax, Sint32 brmax, Sint32 keymax)
{
    Sint32      i;

    /* 作業領域のクリア */
    memset(bpl_mng_ptr, 0, BPL_WORK_SIZE(bstmmax, brmax, keymax));

    /* 領域の割り当て   */
    BPD_MNG_BSTMTBL(bpl_mng_ptr) =
                        (BplStm *)((size_t)bpl_mng_ptr + sizeof(BplMng));
    BPD_MNG_BRTBL(bpl_mng_ptr) =
                        (Sint32 *)(BPD_MNG_BSTMTBL(bpl_mng_ptr) + bstmmax);
    BPD_MNG_KEYTBL(bpl_mng_ptr) =
                        (StmKey *)(BPD_MNG_BRTBL(bpl_mng_ptr) + brmax);

    BPD_MNG_BSTMMAX(bpl_mng_ptr) = bstmmax;
    BPD_MNG_BRMAX(bpl_mng_ptr) = brmax;
    BPD_MNG_KEYMAX(bpl_mng_ptr) = keymax;

    for (i = 0; i < bstmmax; i++) {
        BPD_STM_BRANCH(BPD_MNG_BSTM(bpl_mng_ptr, i)) = BPL_BR_NONE;
    }

    for (i = 0; i < brmax; i++) {
        BPD_MNG_BR(bpl_mng_ptr, i) = BPL_BR_NONE;
    }

    BPD_MNG_CURSTMID(bpl_mng_ptr) = BPL_BR_NONE;
    BPD_MNG_NEXTBRNO(bpl_mng_ptr) = BPL_BR_NONE;

    return;
}


/****************************************************
 * 分岐再生の中止
 ****************************************************/
BPL_LOCAL void bpl_stopStm(void)
{
    Sint32      i;

    /* ストリームグループのクローズ */
    STM_CloseGrp(BPD_MNG_GRPHN(bpl_mng_ptr));
    
    /* パラメータの初期化 */
    for (i = 0; i < BPL_STMKEY_MAX; i++) {
        BPD_MNG_CURSTMHN(bpl_mng_ptr, i) = NULL;
    }

    for (i = 0; i < BPL_BLK_NUM; i++) {
        BPD_MNG_NEXTSTMHN(bpl_mng_ptr, i) = NULL;
    }

    BPD_MNG_CURSTMID(bpl_mng_ptr) = BPL_BR_NONE;
    BPD_MNG_NEXTBRNO(bpl_mng_ptr) = BPL_BR_NONE;
    BPD_MNG_ATR(bpl_mng_ptr) = BPD_ATR_NEXT_CLOSE;
    BPD_MNG_GRPHN(bpl_mng_ptr) = NULL;

    return;
}

/****************************************************
 * 分岐再生状態のチェック
 * 関数値       : 現在の状態
 ****************************************************/
BPL_LOCAL Sint32 bpl_checkStatus(Sint32 atr)
{
    Sint32      ret;
    Sint32      bstmid;
    BplStm      *bs;

    ret = BPL_SVR_COMPLETED;

    /* 現在のストリームがオープンされているか */
    bstmid = BPD_MNG_CURSTMID(bpl_mng_ptr);
    if (bstmid >= 0) {
        bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);

        /* 分岐候補がないかチェック */
        ret = ((BPD_STM_NBR(bs) == 0) ? BPL_SVR_NOBRN : BPL_SVR_WAITSEL);

        /* BPL_BR_NONEが選択された */
        if (atr == BPD_ATR_NEXT_END) {
            ret = BPL_SVR_NOBRN;
        }
    }

    /* 分岐候補のストリームがオープンされていて、分岐先が選択されているか */
    if ((atr == BPD_ATR_NEXT_OPEN) && (BPD_MNG_NEXTBRNO(bpl_mng_ptr) >= 0)) {
        ret = BPL_SVR_SELECT;
    }

    return ret;
}


/****************************************************
 * 分岐処理の実行
 * stat         : 現在の状態
 * 関数値       : 分岐処理後の状態
 ****************************************************/
BPL_LOCAL Sint32 bpl_execBranch(Sint32 stat)
{
    Sint32      ret;
    Sint32      fid, bn;
    StmKey      key;

    switch(stat) {
    case BPL_SVR_WAITSEL:
    case BPL_SVR_NOBRN:
        bpl_closeCurStm();
        BPD_MNG_CURSTMID(bpl_mng_ptr) = BPL_BR_NONE;
        ret = BPL_SVR_COMPLETED;
        break;
    case BPL_SVR_SELECT:
        bpl_closeCurStm();
        bpl_changeStm();
        STM_GetInfo(BPD_MNG_CURSTMHN(bpl_mng_ptr, 0), &fid,
                    &BPD_MNG_FRANGE(bpl_mng_ptr), &bn, &key);
        ret = BPL_SVR_WAITSEL;
        break;
    default:
        ret = stat;
        break;
    }

    return ret;
}


/*---------------------------------------------------------------------------*/

/****************************************************
 * 分岐再生の初期化
 * bstmmax      : 登録するストリームの最大数
 * brmax        : 分岐の最大数
 * key_max      : ストリームキーの種類の最大数
 * work         : 作業領域
 * 関数値       : エラーコード
 *****************************************************/
Sint32  BPL_Init(Sint32 bstmmax, Sint32 brmax, Sint32 keymax, void *work)
{
#if	0
	/*
	**■1995-07-26 高橋智延
	**	使ってないので削除する。
	*/
    Sint32      i;
#endif

    /* 領域の割り当て   */
    bpl_mng_ptr = work;                         /* 管理領域             */

    /* 初期値設定        */
    bpl_initPara(bstmmax, brmax, keymax);

    return BPL_ERR_OK;
}


/****************************************************
 * 分岐再生のリセット
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_Reset(void)
{
#if	0
	/*
	**■1995-07-26 高橋智延
	**	使ってないので削除する。
	*/
    Sint32      i;
#endif
    Sint32      bstmmax;
    Sint32      brmax;
    Sint32      keymax;

    /* ストリームグループのクローズ */
    STM_CloseGrp(BPD_MNG_GRPHN(bpl_mng_ptr));

    /* 現在のパラメータのコピー */
    bstmmax = BPD_MNG_BSTMMAX(bpl_mng_ptr);
    brmax = BPD_MNG_BRMAX(bpl_mng_ptr);
    keymax = BPD_MNG_KEYMAX(bpl_mng_ptr);

    /* 初期値設定        */
    bpl_initPara(bstmmax, brmax, keymax);

    return BPL_ERR_OK;
}


/****************************************************
 * 分岐ストリーム情報の設定
 * bstmid       : 分岐ストリーム識別子(0 <= bstmid < bstmmax)
 * fid          : ファイル識別子
 * nkey         : ストリームキーの数
 * keytbl       : ストリームキーテーブル
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_SetStmInfo(Sint32 bstmid, Sint32 fid, Sint32 nkey, StmKey *keytbl)
{
    BplStm      *bs;
    StmKey      *stmkey, *mngkey;
    Sint32      keymax;
    Sint32      key;
    Sint32      i, j;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }
    /* ストリームキー種類数が多過ぎる   */
    if (nkey > BPL_STMKEY_MAX) {
        return BPL_ERR_KYOVRFLW;
    }

    keymax = BPD_MNG_KEYMAX(bpl_mng_ptr);

    /* パラメータ設定                   */
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    BPD_STM_FID(bs) = fid;
    BPD_STM_NKEY(bs) = nkey;
    for (i = 0; i < nkey; i++) {
        stmkey = keytbl + i;

        /* 同じキーが設定済みかどうかを調べる   */
        key = BPD_MNG_NKEY(bpl_mng_ptr);
        for (j = 0; j < key; j++) {
            mngkey = BPD_MNG_KEYPTR(bpl_mng_ptr, j);
            if (memcmp(mngkey, stmkey, sizeof(StmKey)) == 0) {
                break;
            }
        }

        /* ストリーム情報の追加 */
        if (j == key) {
            if (key >= keymax) {
                return BPL_ERR_KYOVRFLW;
            }
            memcpy(BPD_MNG_KEYPTR(bpl_mng_ptr, j), stmkey, sizeof(StmKey));
            key++;
            BPD_MNG_NKEY(bpl_mng_ptr) = key;
        }
        BPD_STM_KEYNO(bs, i) = j;
    }
    return BPL_ERR_OK;
}


/****************************************************
 * 分岐ストリーム情報の取得
 * bstmid       : 分岐ストリーム識別子
 * fid          : ファイル識別子
 * nkey         : ストリームキーの数
 * keytbl       : ストリームキーテーブル
 * 関数値       : 設定されている分岐先の数
 ****************************************************/
Sint32  BPL_GetStmInfo(Sint32 bstmid, 
                       Sint32 *fid, Sint32 *nkey, StmKey *keytbl)
{
    BplStm      *bs;
    Sint32      keyno;
    Sint32      i;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    *fid = BPD_STM_FID(bs);                 /* ファイル識別子       */
    *nkey = BPD_STM_NKEY(bs);               /* ストリームキーの数   */

    /* ストリームキー   */
    for (i = 0; i < *nkey; i++) {
        keyno = BPD_STM_KEYNO(bs, i);
        memcpy(keytbl + i, BPD_MNG_KEYPTR(bpl_mng_ptr, keyno), sizeof(StmKey));
    }
    return BPD_STM_NBR(bs);
}


/****************************************************
 * 分岐先情報の設定
 * bstmid       : 分岐ストリーム識別子
 * nbranch      : 分岐先の数
 * brtbl        : 分岐テーブル（分岐しない場合の設定値は、BPL_BR_NONE）
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_SetBranchInfo(Sint32 bstmid, Sint32 nbranch, Sint32 *brtbl)
{
    BplStm      *bs;
    Sint32      branch;
    Sint32      nbr;
    Sint32      bstmmax;
    Sint32      i;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }

    /* 分岐先が多過ぎる                 */
    nbr = BPD_MNG_NBR(bpl_mng_ptr);
    if ((nbr + nbranch) > BPD_MNG_BRMAX(bpl_mng_ptr)) {
        return BPL_ERR_BROVRFLW;
    }

    /* 分岐数の設定                     */
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    BPD_STM_NBR(bs) = nbranch;
    if (nbranch <= 0) {
        return BPL_ERR_OK;
    }
    BPD_STM_BRANCH(bs) = nbr;

    /* 分岐先の設定                     */
    bstmmax = BPD_MNG_BSTMMAX(bpl_mng_ptr);
    for (i = 0; i < nbranch; i++) {
        branch = *(brtbl + i);
        if (branch > bstmmax) {
            return BPL_ERR_BRNO;
        }
        BPD_MNG_BR(bpl_mng_ptr, nbr) = branch;
        nbr++;
        BPD_MNG_NBR(bpl_mng_ptr) = nbr;
    }
    return BPL_ERR_OK;
}



/****************************************************
 * 分岐先情報の取得
 * bstmid       : 分岐ストリーム識別子
 * nbranch      : 分岐先の数
 * brtbl        : 分岐テーブル
 * nelem        : 分岐テーブルの要素数
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_GetBranchInfo(Sint32 bstmid, Sint32 *nbranch, Sint32 *brtbl,
                                                              Sint32 nelem)
{
    BplStm      *bs;
    Sint32      i;
    Sint32      branch;
    Sint32      brnum;

    /* ストリーム識別子不正             */
    if (IS_BSTMID_NG(bstmid)) {
        return BPL_ERR_BSTMID;
    }
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    brnum = BPD_STM_NBR(bs);
    if (nelem > brnum) {
        nelem = brnum;
    }
    branch = BPD_STM_BRANCH(bs);
    for (i = 0; i < nelem; i++) {
        *brtbl = BPD_MNG_BR(bpl_mng_ptr, branch);
        brtbl++;
        branch++;
    }
    *nbranch = brnum;
    return BPL_ERR_OK;
}
    


/****************************************************
 * 再生開始ストリームの指定
 * bstmid       : 分岐ストリーム識別子
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_SetStart(Sint32 bstmid)
{
#if	0
	/*
	**■1995-07-26 高橋智延
	**	使ってないので削除する。
	*/
    Sint32      i;
#endif
    Sint32      fid;
    Sint32      bn;
    StmKey      key;
    Sint32      ec;

    /* ストリーム識別子不正             */
    if (bstmid >= BPD_MNG_BSTMMAX(bpl_mng_ptr)) {
        return BPL_ERR_BSTMID;
    }

    /* 分岐再生の中止 */
    if (BPD_MNG_GRPHN(bpl_mng_ptr) != NULL) {
        bpl_stopStm();
    }

    /* ストリーム識別子が負の場合は再生中止のみ実行 */
    if (bstmid < 0) {
        return BPL_ERR_OK;
    }

    BPD_MNG_CURSTMID(bpl_mng_ptr) = bstmid;
    BPD_MNG_NEXTBRNO(bpl_mng_ptr) = BPL_BR_NONE;

    /* ストリームグループのオープン     */
    BPD_MNG_GRPHN(bpl_mng_ptr) = STM_OpenGrp();
    if (BPD_MNG_GRPHN(bpl_mng_ptr) == NULL) {
        return BPL_ERR_OPNSTM;
    }
    STM_SetExecGrp(BPD_MNG_GRPHN(bpl_mng_ptr));

    /* ストリームのオープン処理         */
    ec = bpl_openStm(bstmid, BPD_MNG_CURSTM(bpl_mng_ptr), 0, BPL_STMKEY_MAX);
    if (ec < 0) {
        return ec;
    }
    STM_GetInfo(BPD_MNG_CURSTMHN(bpl_mng_ptr, 0), &fid, 
                &BPD_MNG_FRANGE(bpl_mng_ptr), &bn, &key);
    return ec;
}


/****************************************************
 * 分岐再生サーバの実行
 * chgsw        : 分岐実行スイッチ
 * 関数値       : 分岐再生状態
 ****************************************************/
Sint32  BPL_ExecServer(Bool chgsw)
{
    Sint32      stat;
    Sint32      ec;
    Sint32      ret;
    Sint32      atr;

    /* 変数の設定 */
    atr = BPD_MNG_ATR(bpl_mng_ptr);

    /* 現在の状態をチェック */
    ret = bpl_checkStatus(atr);

    /* ストリームの分岐処理 */
    if (chgsw == ON) {
        ret = bpl_execBranch(ret);
    }

    stat = bpl_getAccess();
    switch(stat) {
    case BPD_ACCESS_DOING:
        if (atr == BPD_ATR_NEXT_CLOSE) {
            /* 分岐先をオープンする処理             */
            ec = bpl_openNextStm(BPD_MNG_CURSTMID(bpl_mng_ptr));
            if (ec < 0) {
                return ec;
            }
        }
        break;
    case BPD_ACCESS_END:
        if (atr == BPD_ATR_NEXT_OPEN) {
            bpl_openTrGate();
        }
        break;
    }

    return ret;
}


/****************************************************
 * 分岐先の選択
 * brno         : 分岐番号
 * 関数値       : エラーコード
 ****************************************************/
Sint32  BPL_SelectBranch(Sint32 brno)
{
    Sint32      bstmid;
    BplStm      *bs;

    /* 分岐先が既に指定されている       */
    if (BPD_MNG_NEXTBRNO(bpl_mng_ptr) >= 0) {
        return BPL_ERR_BRSPC;
    }
    bstmid = BPD_MNG_CURSTMID(bpl_mng_ptr);
    bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
    if ((brno < 0) || (brno >= BPD_STM_NBR(bs))) {
        return BPL_ERR_BRNO;
    }

    /* 分岐番号の設定           */      
    if (BPD_MNG_BR(bpl_mng_ptr, BPD_STM_BRANCH(bs) + brno) < 0) {
        /* 分岐先がない場合             */
        BPD_MNG_NEXTBRNO(bpl_mng_ptr) = BPL_BR_NONE;
        BPD_MNG_ATR(bpl_mng_ptr) = BPD_ATR_NEXT_END;
    } else {
        BPD_MNG_NEXTBRNO(bpl_mng_ptr) = brno;
    }
    bpl_closeNextStm();
    return BPL_ERR_OK;
}



/****************************************************
 * 現在のストリームの取得
 * nelem        : ストリームハンドルテーブルの要素数
 * stmtbl       : ストリームハンドルテーブル
 * 関数値       : 分岐ストリーム識別子
 ****************************************************/
Sint32  BPL_GetCurStm(Sint32 nelem, StmHn *stmtbl)
{
    BplStm      *bs;
#if	0
	/*
	**■1995-07-26 高橋智延
	**	使ってないので削除する。
	*/
    Sint32      keyno;
#endif
    Sint32      stmid;
    Sint32      stmnkey;
    Sint32      i;

    /* 現在のストリームIDの取得 */
    stmid = BPD_MNG_CURSTMID(bpl_mng_ptr);
    if (stmid < 0) {
        return BPL_BR_NONE;
    }

    /* ストリーム登録領域の取得 */
    bs = BPD_MNG_BSTM(bpl_mng_ptr, stmid);

    /* 登録されているストリームキーの数を取得 */
    stmnkey = BPD_STM_NKEY(bs);
    if (nelem > stmnkey) {
        nelem = stmnkey;
    }

    /* 現在のストリームハンドルの取得 */
    for (i = 0; i < nelem; i++) {
        *(stmtbl + i) = BPD_MNG_CURSTMHN(bpl_mng_ptr, i);
    }

    return stmid;
}



/****************************************************
 * 分岐先のストリームの取得
 * nelem        : ストリームハンドルテーブルの要素数
 * stmtbl       : ストリームハンドルテーブル
 * 関数値       : 分岐ストリーム識別子
 ****************************************************/
Sint32  BPL_GetNextStm(Sint32 nelem, StmHn *stmtbl)
{
    BplStm      *bs;
    Sint32      br;
    Sint32      brno;
    Sint32      nstm;
    Sint32      bstmid;
    Sint32      curstmid;
    Sint32      i;

    /* 現在のストリームIDの取得 */
    curstmid = BPD_MNG_CURSTMID(bpl_mng_ptr);

    /* 分岐番号の取得 */
    brno = BPD_MNG_NEXTBRNO(bpl_mng_ptr);
    if ((curstmid < 0) || (brno < 0)) {
        return BPL_BR_NONE;
    }

    /* ストリーム登録領域の取得 */
    bs = BPD_MNG_BSTM(bpl_mng_ptr, curstmid);

    /* 分岐テーブルの位置の取得 */
    br = BPD_STM_BRANCH(bs);

    /* 分岐先のストリームが登録されている位置までの */
    /* ストリームキーの数を求める */
    nstm = 0;
    for (i = 0; i < brno; i++) {
        bstmid = BPD_MNG_BR(bpl_mng_ptr, br);
        if (bstmid >= 0) {
            nstm += BPD_STM_NKEY(BPD_MNG_BSTM(bpl_mng_ptr, bstmid));
        }
        br++;
    }

    /* 分岐先のストリームハンドルの取得 */
    bstmid = BPD_MNG_BR(bpl_mng_ptr, br);
    if (bstmid >= 0) {
        bs = BPD_MNG_BSTM(bpl_mng_ptr, bstmid);
        if (nelem > BPD_STM_NKEY(bs)) {
            nelem = BPD_STM_NKEY(bs);
        }
        for (i = 0; i < nelem; i++) {
            *(stmtbl + i) = BPD_MNG_NEXTSTMHN(bpl_mng_ptr, i + nstm);
        }
    } else {
        for (i = 0; i < nelem; i++) {
            *(stmtbl + i) = NULL;
        }
    }

    return bstmid;
}



/****************************************************
 * ストリームグループの取得
 * 関数値       : ストリームグループハンドル
 ****************************************************/
StmGrpHn  BPL_GetStmGrp(void)
{
    return BPD_MNG_GRPHN(bpl_mng_ptr);
}


/* end of file */
