/*****************************************************************************
 *      ソフトウェアライブラリ
 *
 *      Copyright (c) 1994,1995,1996 SEGA
 *
 * Library: ストリームシステム
 * Module : ローカル関数(CDブロック制御)
 * File   : stm_loc.c
 * Date   : 1995-02-01
 * Version: 2.10
 * Author : H.T
 *
 *****************************************************************************/
/* SGLの場合コンパイルオプションにより定義すること
 * #define USE_SGL
 */

/*****************************************************************************
 *      インクルードファイル
 *****************************************************************************/
#include        <stdlib.h>

#if defined(USE_SGL)
    #include    "sgl_cd.h"
#else
    #include    "sega_stm.h"
#endif
#include        "stm_def.h"
#include        "gfs_def.h"
#include        "gfs_cdc.h"
#include        "gfs_cdf.h"
#include        "gfs_dir.h"
#include        "stm_loc.h"

/*****************************************************************************
 *      定数
 *****************************************************************************/
#define TIME_40MSEC     (12186*2)

/*****************************************************************************
 *      関数の定義
 *****************************************************************************/

/*************************************************
 * 現在のFADの取得
 * 戻り値:現在のFAD
 *************************************************/
Sint32  STL_GetCurFad(void)
{
    CdcStat     stat;
    Sint32      ret;

    do {
        ret = CDC_GetCurStat(&stat);
    } while (ret != CDC_ERR_OK);
    return CDC_STAT_FAD(&stat);
}


/*************************************************
 * 再生範囲の取得
 * flow : 読み込み管理
 * sfad : 開始FAD
 * efad : 終了FAD
 *************************************************/
void    STL_GetPlayArea(GfsFlow *flow, Sint32 *sfad, Sint32 *efad)
{
    Sint32      nsct;
    Sint32      atr;

    GFCF_GetFileInfo(flow, NULL, sfad, NULL, &nsct, NULL, NULL, &atr);
    if (IS_CDFILE(atr)) {       /* CDファイルの場合     */
        *efad = *sfad + nsct - 1;
    } else {                    /* SIMM, SCSIの場合     */
        *sfad = STM_LONG_MAX;
        *efad = STM_LONG_MAX;
    }
}

    

/*************************************************
 * ストリームグループ再生
 * grp  : ストリームグループ
 * 戻り値: エラーコード
 *************************************************/
Sint32  STL_FlowIn(StmGrpHn grp)
{
    Sint32      _sfad, sfad, efad;
    CdcPly      ply;
    Sint32      ret = STM_ERR_OK;

    /* ループストリームの開始FADを取得  */
    sfad = STM_GRP_FAD(grp);                    /* 開始FAD設定  */
    /* 最終ストリームの終了FADを取得    */
    STL_GetPlayArea(STM_FLOW(STM_GRP_LSTSTM(grp)), &_sfad, &efad);
    if (efad >= sfad) {
        /* パラメータ設定 */
        CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
        CDC_PLY_SFAD(&ply) = sfad;
        CDC_PLY_ETYPE(&ply) = CDC_PTYPE_FAD;
        CDC_PLY_EFAS(&ply) = efad - sfad + 1;
        CDC_PLY_PMODE(&ply) = CDC_PM_DFL;
        ret = GFCD_Play(&ply, STM_GRP_PUID(grp));       /* 読み込み     */
        if (ret == GFCD_ERR_OPEN) {
            ret = STM_ERR_CDOPEN;
        } else if (ret != GFCD_ERR_OK) {
            ret = STM_ERR_CDRD;
        } else {
            MNG_SFAD(stm_mng_ptr) = sfad;
            MNG_EFAD(stm_mng_ptr) = efad;
        }
    }
    return (ret);
}



/*************************************************
 * 読み込み停止
 * grp  : ストリームグループ
 *************************************************/
Sint32  STL_StopIn(StmGrpHn grp)
{
    CdcPos      pos;
    Sint32      ret;

    CDC_POS_PTYPE(&pos) = CDC_PTYPE_NOCHG;
    ret = GFCD_MovePickup(&pos, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
        return (STM_ERR_CDOPEN);
    } else if (ret != GFCD_ERR_OK) {
        return (STM_ERR_CDRD);
    }
    MNG_SFAD(stm_mng_ptr) = -1;
    MNG_EFAD(stm_mng_ptr) = -1;
    return (ret);
}



/*************************************************
 * リードエラー発生時のリカバリ
 * grp  : ストリームグループ
 *************************************************/
Sint32  STL_Recover(StmGrpHn grp)
{
    CdcPly      pinfo;
    Sint32      err;

    /* CdPlayを発行する */
    CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_NOCHG;
    CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_NOCHG;
    CDC_PLY_PMODE(&pinfo) = CDC_PM_NOCHG;
    err = GFCD_Play(&pinfo, STM_GRP_PUID(grp));
    if (err != GFCD_ERR_OK) {
        return STM_ERR_CDRD;
    } else {
        return STM_ERR_OK;
    }
}
    

/*************************************************
 * ピックアップの移動(再生継続)
 * fad  : 移動先FAD
 * 戻り値:エラーコード
 *************************************************/
Sint32  STL_MovePlaypos(StmGrpHn grp, Sint32 fad)
{
    CdcPly      ply;
    Sint32      ret = STM_ERR_OK;

    /* 再生開始位置のみ変更     */
    CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
    CDC_PLY_SFAD(&ply) = fad;
    CDC_PLY_ETYPE(&ply) = CDC_PTYPE_NOCHG;      /* 終了位置変更しない   */
    CDC_PLY_PMODE(&ply) = CDC_PM_DFL;
    ret = GFCD_Play(&ply, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
        ret = STM_ERR_CDOPEN;
    } else if (ret != GFCD_ERR_OK) {
        ret = STM_ERR_CDRD;
    } else {
        MNG_SFAD(stm_mng_ptr) = fad;
    }
    return (ret);
}
    

/*************************************************
 * 再生終了位置の変更
 * grp  : ストリームグループ
 * efad : 終了位置
 *************************************************/
Sint32  STL_ChgPlayEnd(StmGrpHn grp, Sint32 efad)
{
    CdcPly      ply;
    Sint32      ret = STM_ERR_OK;

    /* 再生開始位置のみ変更     */
    CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
    CDC_PLY_SFAD(&ply) = MNG_SFAD(stm_mng_ptr);
    CDC_PLY_ETYPE(&ply) = CDC_PTYPE_FAD;        /* 終了位置変更する     */
    CDC_PLY_EFAS(&ply) = efad - MNG_SFAD(stm_mng_ptr) + 1;
    CDC_PLY_PMODE(&ply) = CDC_PM_NOCHG;
    ret = GFCD_Play(&ply, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
        ret = STM_ERR_CDOPEN;
    } else if (ret != GFCD_ERR_OK) {
        ret = STM_ERR_CDRD;
    } else {
        MNG_EFAD(stm_mng_ptr) = efad;
    }
    return (ret);
}
    

/*************************************************
 * 読み込みセクタ情報の取得
 * flow : 読み込み管理
 * sn   : セクタ番号（先頭セクタは０）
 * sinfo: セクタ情報
 * 戻り値: TRUE 指定セクタあり  FALSE 指定セクタなし
 *************************************************/
Bool    STL_GetSctInfo(GfsFlow *flow, Sint32 sn, StmSct *sinfo)
{
    Sint32      nsct;
    CdcSct      info;

    /* セクタ数のチェック       */
    nsct = GFCB_GetLenData(flow, STM_LONG_MAX, NULL);
    if (sn >= nsct) {    /* セクタ数が指定されたセクタ番号に満たない    */
        return (FALSE);
    }

    /* セクタ情報の取得         */
    GFCD_GetSctInfo(GFS_CDR_BUFNO(&GFS_FLW_DTSRC(flow)), sn, &info);

    STM_SCT_FAD(sinfo) = CDC_SCT_FAD(&info);
    STM_SCT_FN(sinfo) = CDC_SCT_FN(&info);
    STM_SCT_CN(sinfo) = CDC_SCT_CN(&info);
    STM_SCT_SM(sinfo) = CDC_SCT_SM(&info);
    STM_SCT_CI(sinfo) = CDC_SCT_CI(&info);
    return (TRUE);
}


/*************************************************
 * セクタの実データサイズの取得
 * flow : 読み込み管理
 * spos : セクタ位置
 * 戻り値: セクタの実データサイズ(ワード)
 *************************************************/
Sint32  STL_GetActSiz(GfsFlow *flow, Sint32 spos)
{
    Sint32      nword;
    Sint32      nsct;

    GFCD_GetLenData(GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow))), 
                    GFS_CDR_SCTPOS(&(GFS_FLW_DTSRC(flow))) + spos, 1, 
                    &nsct, &nword);
    GFCD_WaitServer();
    return (nword / sizeof(Uint16));
}


/*************************************************
 * CDバッファデータの消去
 * flow : 読み込み管理
 * spos : セクタ位置
 * snum : セクタ数
 *************************************************/
void    STL_DeleteSctData(GfsFlow *flow, Sint32 spos, Sint32 snum)
{
    Sint32      nsct;

    nsct = GFCB_GetLenData(flow, STM_LONG_MAX, NULL);
    if ((spos + snum) > nsct) {
        snum = nsct - spos;
    }
    if (snum > 0) {
        GFCD_DelSctData(GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow))), spos, snum);
    }
}


/*************************************************
 * CSCT関数用CDバッファ使用セクタ数の取得
 * 戻り値 : 使用セクタ数
 *************************************************/
Sint32  STL_CsctGetSctNum(GfsFlow *flow)
{
    Sint32      bufno;
    Sint32      nsct;
    Sint32      ret;

    bufno = GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow)));
    /* この関数はCSCT関数から呼ばれるので直接CDCを呼ぶ  */
    do {
        ret = CDC_GetSctNum(bufno, &nsct);
    } while (ret != CDC_ERR_OK);
    return (nsct);
}


/*************************************************
 * CSCT関数用CDバッファ空きセクタの取得
 * 戻り値 : 空きセクタ数
 *************************************************/
Sint32  STL_CsctGetFreeSctnum(void)
{
    Sint32      total, nblk, free;
    Sint32      ret;

    /* この関数はCSCT関数から呼ばれるので直接CDCを呼ぶ  */
    do {
        ret = CDC_GetBufSiz(&total, &nblk, &free);
    } while (ret != CDC_ERR_OK);
    return (free);
}


/*************************************************
 * CSCTビットが立つのを待つ
 *************************************************/
void    STL_WaitCsct(void)
{
    Sint32      i;
    Uint16      hirq;

    CDC_ClrHirqReq(~CDC_HIRQ_CSCT);
    for (i = 0; i < TIME_40MSEC; i++) {
        hirq = CDC_GetHirqReq();
        if ((hirq & CDC_HIRQ_CSCT) != 0) {
            break;
        }
    }
}


/* end of stm_loc.c     */









