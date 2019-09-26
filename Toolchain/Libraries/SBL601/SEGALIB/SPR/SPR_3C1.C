/*----------------------------------------------------------------------------
 *  spr_3c1.c -- SPR ライブラリ 3C 1 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1995-02-20 Ver.1.00
 *  Updated by H.E on 1995-02-20 Ver.1.00
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#define  _SPR3_
#include "sega_spr.h"

#ifdef _SH
#include <machine.h>
#endif

/*
 * GLOBAL DECLARATIONS
 */
extern TComTbl SpTComTbl;

/*
 * PROTOTYPE DEFINES
 */
void  SPR_3Tran3Dto2D(T3Dto2D *t3Dto2D);
void  SPR_3Tran2DtoCmd(T2DtoCmd *t2DtoCmd);
extern Uint16 SPR_3GetShdColor(Uint16 baseRgbColor, Uint16 bright);


/*****************************************************************************
 *
 * NAME:  SPR_3Tran3Dto2D  - Transfer 3D to 2D coord
 *
 * PARAMETERS :
 *
 *     (1) T3Dto2D *t3Dto2D       - <i/o>  ３Ｄ－＞２Ｄ　パラメータ
 *
 * DESCRIPTION:
 *
 *     ３Ｄ－＞２Ｄ 座標変換、透視変換、面の輝度計算
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void  SPR_3Tran3Dto2D(T3Dto2D *t3Dto2D)
{
    /** BEGIN ***************************************************************/

    if(t3Dto2D->vertCount) {
        /* 頂点データの視点座標変換後テーブルを得る */
        SPR_3AllCoordTransClipPers(
               SpTComTbl.viewMatrix,
               t3Dto2D->vertPoint,
	       t3Dto2D->coordView3D,
	       t3Dto2D->coord2D,
               t3Dto2D->vertCount,
               SpTComTbl.unitPixel,
               SpTComTbl.clipRatio,
               SpTComTbl.clipZ);

        /* グーロー用頂点輝度テーブルを得る  */
        if(t3Dto2D->vertNormal)
            SPR_3AllVertBright(
               SpTComTbl.viewMatrix,
               t3Dto2D->vertNormal,
               t3Dto2D->vertBright,
               SpTComTbl.lightVector,
               t3Dto2D->vertCount);

        /* 頂点データのワールド座標変換後テーブルを得る */
        if(t3Dto2D->coordWorld3D)
	    SPR_3AllCoordTrans(
               SpTComTbl.worldMatrix,
               t3Dto2D->vertPoint,
               t3Dto2D->vertCount,
               t3Dto2D->coordWorld3D);
    }

    /* 面法線テーブルから面輝度テーブルを得る */
    if(t3Dto2D->surfaceCount)
        SPR_3AllSurfaceBright(
               SpTComTbl.viewMatrix,
               t3Dto2D->surfInfo,
               t3Dto2D->surfaceNormal,
               t3Dto2D->surfBright,
               SpTComTbl.lightVector,
               t3Dto2D->surfaceCount,
	       SpTComTbl.coordView3D);

}


/*  以下、高速化のためにアセンブラルーチンあり */
#if 0
/*****************************************************************************
 *
 * NAME:   SPR_3Tran2DtoCmd  -  Transfer 2D data to VDP1 Command
 *
 * PARAMETERS :
 *
 *     (1) T2DtoCmd *t2DtoCmd   - <i/o>  ２Ｄ－＞Ｃｍｄ　パラメータ
 *
 * DESCRIPTION:
 *
 *     ２Ｄ－＞Ｃｍｄ  ポリゴン描画コマンドの登録処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void  SPR_3Tran2DtoCmd(T2DtoCmd *t2DtoCmd)
{
    Sint32         i, cmdNo;
    Sint32         shadingKind, mateKind, dispFlag, sBright, bothFaceFlag;
    Sint32         dispKind, drawMode, drawColor, colOrTex, textCtrl;
    Sint32         *wSurfBright, *vertBright;
    Uint32         zSortKind, drawPrtyBlkNo;
    Uint16         *wVertNo, *shadingTbl, *gour, *wPt;
    Uint32         color, surfBright, w;
    XyInt          *coord2D;
    Fixed32        zSum, zMin, zMax, z;
    MthXyz         *coordView3D;
    SprSurface     *wSurface;
    SprDrawPrtyBlk *prtyTbl;
    SprSpCmd       *spCmd, *wSpCmd;
    SprCharTbl     *charTbl;

    static Uint16    grayCode[32] = {
        RGB16_COLOR( 0, 0, 0), RGB16_COLOR( 1, 1, 1),
        RGB16_COLOR( 2, 2, 2), RGB16_COLOR( 3, 3, 3),
        RGB16_COLOR( 4, 4, 4), RGB16_COLOR( 5, 5, 5),
        RGB16_COLOR( 6, 6, 6), RGB16_COLOR( 7, 7, 7),
        RGB16_COLOR( 8, 8, 8), RGB16_COLOR( 9, 9, 9),
        RGB16_COLOR(10,10,10), RGB16_COLOR(11,11,11),
        RGB16_COLOR(12,12,12), RGB16_COLOR(13,13,13),
        RGB16_COLOR(14,14,14), RGB16_COLOR(15,15,15),
        RGB16_COLOR(16,16,16), RGB16_COLOR(17,17,17),
        RGB16_COLOR(18,18,18), RGB16_COLOR(19,19,19),
        RGB16_COLOR(20,20,20), RGB16_COLOR(21,21,21),
        RGB16_COLOR(22,22,22), RGB16_COLOR(23,23,23),
        RGB16_COLOR(24,24,24), RGB16_COLOR(25,25,25),
        RGB16_COLOR(26,26,26), RGB16_COLOR(27,27,27),
        RGB16_COLOR(28,28,28), RGB16_COLOR(29,29,29),
        RGB16_COLOR(30,30,30), RGB16_COLOR(31,31,31)
    };


    /** BEGIN ***************************************************************/
    coordView3D  = SpTComTbl.coordView3D;
    coord2D      = SpTComTbl.coord2D;
    vertBright   = SpTComTbl.vertBright;
    shadingKind  = SpTComTbl.dispFlag & SHADING_MASK;
    bothFaceFlag = SpTComTbl.dispFlag & BOTH_FACE;
    dispKind     = SpTComTbl.dispFlag & DISP_MASK;
    wSurfBright  = t2DtoCmd->surfBright;
    wSurface     = t2DtoCmd->surface;
    prtyTbl      = t2DtoCmd->prtyTbl;

    for(i = 0; i < t2DtoCmd->surfaceCount; i++, wSurfBright++, wSurface++) {
	sBright = *wSurfBright & 0xff;
	dispFlag = 1;
	if(*wSurfBright & 0x80000000)
	    if(bothFaceFlag) {
	       sBright = 31 - sBright;
	       dispFlag = 2;
	    } else
	       dispFlag = 0;

	if(dispFlag) {
	    /* 代表Ｚ値の取り出し */
	    wVertNo  = wSurface->vertNo;
	    z = coordView3D[*wVertNo++].z;
	    zSum = zMin = zMax = z;
	    z = coordView3D[*wVertNo++].z;
            zSum += z;
            if(zMin > z) zMin = z;
            if(zMax < z) zMax = z;
	    z = coordView3D[*wVertNo++].z;
            zSum += z;
            if(zMin > z) zMin = z;
            if(zMax < z) zMax = z;
	    z = coordView3D[*wVertNo].z;
            zSum += z;
            if(zMin > z) zMin = z;
            if(zMax < z) zMax = z;
            if(zMax >= 0) goto next_surf;
	    zSortKind = wSurface->drawMode & ZSORT_MASK;
	    if(zSortKind == ZSORT_MIN)
		z = zMin;
	    else
	    if(zSortKind == ZSORT_MAX)
		z = zMax;
	    else
		z = MTH_Mul(zSum, MTH_FIXED(0.25));

            /* Ｚ値ブロックソートの最大最小値を得る */
            if(z < t2DtoCmd->zSortZMin) t2DtoCmd->zSortZMin = z;
            if(z > t2DtoCmd->zSortZMax) t2DtoCmd->zSortZMax = z;
	    if(z > SpTComTbl.zSortBZMin) {
	        drawPrtyBlkNo = 
	            (int)((z - SpTComTbl.zSortBZMin) >> SpTComTbl.zSftCnt);
	        if(drawPrtyBlkNo >= SpTComTbl.zSortBlkCnt)
	            drawPrtyBlkNo = SpTComTbl.zSortBlkCnt - 1;
	    } else
	        drawPrtyBlkNo = 0;

	    /* コマンドエリアの獲得とコマンドプライオリティテーブルのセット */
            cmdNo   = t2DtoCmd->outCmdPos++;
            spCmd   = t2DtoCmd->cmdBuf + cmdNo;
            if(prtyTbl[drawPrtyBlkNo].topNo == 0xffff)
                prtyTbl[drawPrtyBlkNo].topNo = cmdNo;
            else {
		wSpCmd = t2DtoCmd->cmdBuf + prtyTbl[drawPrtyBlkNo].botNo;
                wSpCmd->link = cmdNo;
            }
            prtyTbl[drawPrtyBlkNo].botNo = cmdNo;

	    /* ２Ｄデータの取り出し */
	    wVertNo = wSurface->vertNo;
	    wPt    = (Uint16*)&spCmd->ax;
	    *wPt++ = coord2D[*wVertNo  ].x;
	    *wPt++ = coord2D[*wVertNo++].y;
	    *wPt++ = coord2D[*wVertNo  ].x;
	    *wPt++ = coord2D[*wVertNo++].y;
	    *wPt++ = coord2D[*wVertNo  ].x;
	    *wPt++ = coord2D[*wVertNo++].y;
	    *wPt++ = coord2D[*wVertNo  ].x;
	    *wPt   = coord2D[*wVertNo  ].y;

            mateKind  = wSurface->drawMode & MATE_MASK;
            drawMode  = wSurface->drawMode & DRAW_MODE_MASK;
            drawColor = wSurface->drawMode & DRAW_COLOR;
            colOrTex  = wSurface->color;
            spCmd->grshAddr = 0;
            
	    if(shadingKind == NO_SHADING) {
                if(mateKind == MATE_TEXTURE) {
		 /* SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                  drawMode | spriteDrawFlag,
		   	          0xffff, colOrTex, xy, NO_GOUR); */
                    textCtrl   = colOrTex & 0xc000;
                    textCtrl >>= 10;
                    colOrTex  &= 0x3fff;
                    charTbl = &SpTComTbl.charTbl[colOrTex];
                    spCmd->control = ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                     | FUNC_DISTORSP) & ~CTRL_DIR) | textCtrl;
                    spCmd->charAddr = charTbl->addr;
                    spCmd->charSize = charTbl->xySize;
                    spCmd->drawMode = (drawMode & ~DRAW_COLOR)
			             | charTbl->mode
			             | SpTComTbl.spriteDrawFlag;
                    color = charTbl->color;
                    if(drawColor == COLOR_1)
                        spCmd->color = SpTComTbl.lookupTblR + color*4;
                    else
			spCmd->color = color;
                } else {
    	            if(dispKind == DISP_POLYGON) {
	             /* SPR_2Polygon(drawPrtyBlkNo,
                            drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR);*/
		       spCmd->control = JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                         | FUNC_POLYGON;
                        spCmd->drawMode = drawMode | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + colOrTex*4;
                        else
                            spCmd->color = colOrTex;
                    } else {
	            /*  SPR_2PolyLine(drawPrtyBlkNo,
	                  drawMode | otherDrawFlag, colOrTex, xy, NO_GOUR); */
                        spCmd->control = JUMP_ASSIGN | FUNC_POLYLINE;
                        spCmd->drawMode = drawMode | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + colOrTex*4;
                        else
                            spCmd->color = colOrTex;
                    }
                }
            } else
	    if(shadingKind == FLAT_SHADING) {  /* Color Mode is Only RGB */
                if(mateKind == MATE_TEXTURE) {
		    if(SpTComTbl.shdIdxTbl) {
                        spCmd->grshAddr = t2DtoCmd->gourTblCurR++;
                        gour            = (Uint16*)t2DtoCmd->gourTblCur++;
		        shadingTbl = SpTComTbl.shdIdxTbl[0];
		        w = shadingTbl[sBright];
		        *gour++ = w;
		        *gour++ = w;
		        *gour++ = w;
		        *gour   = w;
		    } else {
		        w = (sBright >> 1) + 8;
                        spCmd->grshAddr = SpTComTbl.gourGrayTopR + w;
                    }
                 /* SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                             drawMode  | spriteDrawFlag,
                              0xffff, colOrTex, xy, gourTblNo); */
                    textCtrl   = colOrTex & 0xc000;
                    textCtrl >>= 10;
                    colOrTex  &= 0x3fff;
                    charTbl = &SpTComTbl.charTbl[colOrTex];
                    spCmd->control = ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                     | FUNC_DISTORSP) & ~CTRL_DIR) | textCtrl;
                    spCmd->charAddr = charTbl->addr;
                    spCmd->charSize = charTbl->xySize;
                    spCmd->drawMode = (drawMode & ~DRAW_COLOR)
                                     | DRAW_GOURAU_GRAY
                                     | charTbl->mode
                                     | SpTComTbl.spriteDrawFlag;
                    color = charTbl->color;
                    if(drawColor == COLOR_1)
                        spCmd->color = SpTComTbl.lookupTblR + color*4;
                    else
                        spCmd->color = color;
                } else {
		    if(mateKind == MATE_SHADING_TBL) {
		        shadingTbl = SpTComTbl.shdIdxTbl[wSurface->color];
		        surfBright = shadingTbl[sBright];
		    } else
		        surfBright = SPR_3GetShdColor(wSurface->color,sBright);
                    if(dispKind == DISP_POLYGON) {
		       /* SPR_2Polygon(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                 surfBright, xy, NO_GOUR); */
                        spCmd->control = JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                         | FUNC_POLYGON;
                        spCmd->drawMode = drawMode | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + surfBright*4;
                        else
                            spCmd->color = surfBright;
                    } else {
                      /* SPR_2PolyLine(drawPrtyBlkNo, drawMode | otherDrawFlag,
                                  surfBright, xy, NO_GOUR); */
                        spCmd->control = JUMP_ASSIGN | FUNC_POLYLINE;
                        spCmd->drawMode = drawMode | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + surfBright*4;
                        else
                            spCmd->color = surfBright;
	            }
	        }
            } else
            if(shadingKind == GOURAUD_SHADING) { /* Color Mode is Only RGB */
                spCmd->grshAddr = t2DtoCmd->gourTblCurR++;
                gour            = (Uint16*)t2DtoCmd->gourTblCur++;
		wVertNo = wSurface->vertNo;
		if(SpTComTbl.shdIdxTbl)
		    shadingTbl = SpTComTbl.shdIdxTbl[0];
		else
		    shadingTbl = grayCode;
                if(dispFlag == 1) {
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[vertBright[*wVertNo++]];
		    *gour   = shadingTbl[vertBright[*wVertNo  ]];
                } else {
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour++ = shadingTbl[31 - vertBright[*wVertNo++]];
		    *gour   = shadingTbl[31 - vertBright[*wVertNo  ]];
                }
                if(mateKind == MATE_TEXTURE) {
                 /* SPR_2DistSpr(drawPrtyBlkNo, textCtrl,
                                 drawMode | DRAW_GOURAU | spriteDrawFlag,
		   	                0xffff, colOrTex, xy, gourTblNo); */
                    textCtrl   = colOrTex & 0xc000;
                    textCtrl >>= 10;
                    colOrTex  &= 0x3fff;
                    charTbl = &SpTComTbl.charTbl[colOrTex];
                    spCmd->control = ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                     | FUNC_DISTORSP) & ~CTRL_DIR) | textCtrl;
                    spCmd->charAddr = charTbl->addr;
                    spCmd->charSize = charTbl->xySize;
                    spCmd->drawMode = (drawMode & ~DRAW_COLOR)
                                     | DRAW_GOURAU
			             | charTbl->mode
			             | SpTComTbl.spriteDrawFlag;
                    color = charTbl->color;
                    if(drawColor == COLOR_1)
                        spCmd->color = SpTComTbl.lookupTblR + color*4;
                    else
			spCmd->color = color;
                } else     {
	            if(dispKind == DISP_POLYGON) {
                     /* SPR_2Polygon(drawPrtyBlkNo,
                                 drawMode | DRAW_GOURAU | otherDrawFlag,
						colOrTex, xy, gourTblNo); */
		        spCmd->control = JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV
                                         | FUNC_POLYGON;
                        spCmd->drawMode = drawMode
                                        | DRAW_GOURAU
                                        | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + colOrTex*4;
                        else
                            spCmd->color = colOrTex;
                    } else {
		     /* SPR_2PolyLine(drawPrtyBlkNo,
                                  drawMode | DRAW_GOURAU | otherDrawFlag,
				          	colOrTex, xy, gourTblNo); */
                        spCmd->control = JUMP_ASSIGN | FUNC_POLYLINE;
                        spCmd->drawMode = drawMode
                                        | DRAW_GOURAU
                                        | SpTComTbl.otherDrawFlag;
                        if(drawColor == COLOR_1)
                            spCmd->color = SpTComTbl.lookupTblR + colOrTex*4;
                        else
                            spCmd->color = colOrTex;
                    }
                }
            }
	}
next_surf: ;
    }
    return;
stop:
    goto stop;
}
#endif

/*  end of file */
