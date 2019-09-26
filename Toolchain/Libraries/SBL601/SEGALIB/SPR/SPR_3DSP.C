/*----------------------------------------------------------------------------
 *  spr_3dsp.c -- SPR ライブラリ ＤＳＰによるマトリックス合成処理 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-20 Ver.1.00
 *  Updated by H.E on 1994-02-29 Ver.1.00
 *
 *  このライブラリはＤＳＰによる３Ｄ座標変換処理モジュールで
 *  以下のルーチンを含む。
 *
 *  SPR_3MatrixCompoInit     -  マトリックス合成処理の初期化
 *  SPR_3MatrixCompoExec     -  マトリックス合成処理実行
 *  SPR_3MatrixCompoCheck    -  マトリックス合成処理完了チェック
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#define  _SPR3_
#include "sega_spr.h"
#include "sega_dsp.h"
#include "sega_csh.h"
#include <machine.h>

#ifdef USE_DSP

static Uint32 dspProgram[] = {
#include "dspmtx.cod"
};

static MthMatrix *outputMatrix;


/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoInit  -  Initialize Matrix Compose by DSP
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰの初期化とマトリックス合成処理プログラムのロード。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 *
 *************************************************************************
 */
void    SPR_3MatrixCompoInit(void)
{
    /** BEGIN ************************************************************/
    DSP_LoadProgram(0, dspProgram, 256);
}


/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoExec  -  Execute MatrixCompose by DSP
 *
 * PARAMETERS
 *
 *     (1) SprDspMatrixParm　 *dspMatrixParm  
 *                             - <i/o>  マトリックス合成パラメータテーブル
 *
 * DESCRIPTION
 *
 *      以下のマトリックス合成処理をＤＳＰにて行う。
 *
 *      ・出力マトリックスエリアのクリア
 *      ・Ｚの符号反転合成              
 *      ・平行移動合成                  
 *      ・Ｘ回転移動合成                
 *      ・Ｙ回転移動合成                
 *      ・Ｚ回転移動合成                
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    SPR_3MatrixCompoExec(SprDspMatrixParm *dspMatrixParm)
{
    Uint32 w[2];

    /** BEGIN ************************************************************/
    w[0] = ((Uint32)dspMatrixParm->inputMatrix  ) >> 2;
    w[1] = ((Uint32)dspMatrixParm->outputMatrix ) >> 2;
    outputMatrix = dspMatrixParm->outputMatrix;

    DSP_WriteData(DSP_RAM_0 | 0, w, 2);
    DSP_WriteData(DSP_RAM_0 | 2, (Uint32*)dspMatrixParm->op,
                                              sizeof(SprDspMatrixParm)/4 - 2);
    DSP_Start(0);
}


/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoCheck  -  Check Matrix Compose Complete
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰによるマトリックス合成処理が完了するまで待つ。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    SPR_3MatrixCompoCheck(void)
{
    /** BEGIN ************************************************************/
    while(DSP_CheckEnd() == DSP_NOT_END);
    CSH_Purge(outputMatrix, sizeof(MthMatrix));
}

#else
/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoInit  -  Initialize Matrix Compose by DSP
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰの初期化とマトリックス合成処理プログラムのロード。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 *
 *************************************************************************
 */
void    SPR_3MatrixCompoInit(void)
{
    /** BEGIN ************************************************************/
}


/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoExec  -  Execute MatrixCompose by DSP
 *
 * PARAMETERS
 *
 *     (1) SprDspMatrixParm　 *dspMatrixParm  
 *                             - <i/o>  マトリックス合成パラメータテーブル
 *
 * DESCRIPTION
 *
 *      以下のマトリックス合成処理をＤＳＰにて行う。
 *
 *      ・出力マトリックスエリアのクリア
 *      ・Ｚの符号反転合成              
 *      ・平行移動合成                  
 *      ・Ｘ回転移動合成                
 *      ・Ｙ回転移動合成                
 *      ・Ｚ回転移動合成                
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    SPR_3MatrixCompoExec(SprDspMatrixParm *dspMatrixParm)
{
    int  i;
    MthMatrixTbl matrixTbl;

    /** BEGIN ************************************************************/
    matrixTbl.stackSize = 1;
    matrixTbl.current   = dspMatrixParm->outputMatrix;
    matrixTbl.stack     = dspMatrixParm->outputMatrix;
    sprMemCpyL(dspMatrixParm->outputMatrix, dspMatrixParm->inputMatrix, 12);
    for(i=0; i<15; i++) {
        switch(dspMatrixParm->op[i].opCode) {

            case MTX_OP_CLEAR	:    /*　出力マトリックスエリアのクリア      */
                 MTH_ClearMatrix(&matrixTbl);
                 break;

            case MTX_OP_REV_Z	:    /*　Ｚの符号反転合成                    */
                 MTH_ReverseZ(&matrixTbl);
                 break;

            case MTX_OP_MOV_XYZ	:    /*　平行移動合成                        */
                 MTH_MoveMatrix(&matrixTbl,
                                dspMatrixParm->op[i].x,
                                dspMatrixParm->op[i].y,
                                dspMatrixParm->op[i].z);
                 break;

            case MTX_OP_ROT_X	:    /*　Ｘ回転移動合成                      */
                 MTH_RotateMatrixX(&matrixTbl,
                                   dspMatrixParm->op[i].x);
                 break;

            case MTX_OP_ROT_Y	:    /*　Ｙ回転移動合成                      */
                 MTH_RotateMatrixY(&matrixTbl,
                                   dspMatrixParm->op[i].y);
                 break;

            case MTX_OP_ROT_Z	:    /*　Ｚ回転移動合成                      */
                 MTH_RotateMatrixZ(&matrixTbl,
                                   dspMatrixParm->op[i].z);
                 break;

            case MTX_OP_END	:    /*　処理終了                            */
                 goto exit;
                 break;

        }
    }
exit: ;
}


/*************************************************************************
 *
 * NAME : SPR_3MatrixCompoCheck  -  Check Matrix Compose Complete
 *
 * PARAMETERS
 *
 *     No exist.
 *
 * DESCRIPTION
 *
 *	ＤＳＰによるマトリックス合成処理が完了するまで待つ。
 *
 * POSTCONDITIONS
 *
 *     No exist.
 *
 * CAVEATS
 * 
 *************************************************************************
 */
void    SPR_3MatrixCompoCheck(void)
{
    /** BEGIN ************************************************************/
}
#endif

/*  end of file */
