/******************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1995 CSK Research Institute Corp.
 *      Copyright (c) 1995 SEGA
 *
 * Library: 分岐再生ライブラリ
 * Module : ローカル用ヘッダファイル
 * File   : bpl_def.h
 * Date   : 1995-03-31
 * Version: 1.00
 * Auther : T.K
 *
 ****************************************************************************/
#if     !defined(BPL_DEF_H)
#define BPL_DEF_H


/*****************************************************************************
 *      定数
 *****************************************************************************/
/* 現在のストリームのアクセス状態 */
#define BPD_ACCESS_BEFORE       0       /* アクセスしていない   */
#define BPD_ACCESS_DOING        1       /* アクセス中           */
#define BPD_ACCESS_END          2       /* アクセスが終了した   */

/* 分岐先のストリームの状態 */
#define BPD_ATR_NEXT_CLOSE      0       /* まだオープンしていない       */
#define BPD_ATR_NEXT_OPEN       1       /* オープンしている             */
#define BPD_ATR_NEXT_END        2       /* もう分岐先がない             */

/*****************************************************************************
 *      処理マクロ
 *****************************************************************************/

/* 分岐ストリームアクセスマクロ */
#define BPD_STM_FID(bs)                 ((bs)->fid)
#define BPD_STM_NBR(bs)                 ((bs)->nbr)
#define BPD_STM_BRANCH(bs)              ((bs)->branch)
#define BPD_STM_NKEY(bs)                ((bs)->nkey)
#define BPD_STM_KEYNO(bs, i)            ((bs)->keyno[i])


/* 管理テーブルアクセスマクロ   */
#define BPD_MNG_BSTMTBL(mng)            ((mng)->bstmtbl)
#define BPD_MNG_BSTM(mng, i)            ((mng)->bstmtbl + (i))
#define BPD_MNG_BSTMMAX(mng)            ((mng)->bstmmax)
#define BPD_MNG_BRTBL(mng)              ((mng)->brtbl)
#define BPD_MNG_BR(mng, i)              ((mng)->brtbl[i])
#define BPD_MNG_BRMAX(mng)              ((mng)->brmax)
#define BPD_MNG_NBR(mng)                ((mng)->nbr)
#define BPD_MNG_KEYTBL(mng)             ((mng)->keytbl)
#define BPD_MNG_KEYPTR(mng, i)          ((mng)->keytbl + (i))
#define BPD_MNG_KEYMAX(mng)             ((mng)->keymax)
#define BPD_MNG_NKEY(mng)               ((mng)->nkey)
#define BPD_MNG_CURSTMID(mng)           ((mng)->curstmid)
#define BPD_MNG_NEXTBRNO(mng)           ((mng)->nextbrno)
#define BPD_MNG_GRPHN(mng)              ((mng)->grphn)
#define BPD_MNG_FRANGE(mng)             ((mng)->frange)
#define BPD_MNG_CURSTM(mng)             ((mng)->curstmhn)
#define BPD_MNG_CURSTMHN(mng, i)        ((mng)->curstmhn[i])
#define BPD_MNG_NEXTSTM(mng)            ((mng)->nextstmhn)
#define BPD_MNG_NEXTSTMHN(mng, i)       ((mng)->nextstmhn[i])
#define BPD_MNG_ATR(mng)                ((mng)->atr)

/* 現在の分岐テーブルのベース*/
#define BPD_MNG_BRBASE(mng) \
    BPD_STM_BRANCH(BPD_MNG_BSTM((mng), BPD_MNG_CURSTMID(mng)))

/* 次の分岐ストリーム識別子*/
#define BPD_MNG_NEXTSTMID(mng)      \
    BPD_MNG_BR((mng), BPD_MNG_BRBASE(mng) + BPD_MNG_NEXTBRNO(mng))

#endif

/* end of file */
