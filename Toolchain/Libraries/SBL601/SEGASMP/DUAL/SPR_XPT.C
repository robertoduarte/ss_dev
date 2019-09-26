/*----------------------------------------------------------------------------
 *  spr_xpt.c -- SPR ライブラリ XPT モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-02-15 Ver.0.80
 *
 *  このライブラリはスプライト表示処理のマシンに依存するルーチンを含む。
 *
 *  fxsin                   -  ＳＩＮの計算
 *  fxcos                   -  ＣＯＳの計算
 *  dmemcpy                 -  メモリコピー
 *
 *  このライブラリを使用するには次のインクルードファイルを定義する必要がある。
 *
 *   #include "sega_xpt.h"
 *   #include "sega_spr.h"
 *
 *----------------------------------------------------------------------------
 */

#include "sega_xpt.h"
#include "sega_spr.h"

#include	<machine.h>

extern Fixed32 fsin(Fixed32 degree);
extern Fixed32 fcos(Fixed32 degree);

/*
 *    -180 <= degree <= 180
 */
Fixed32 fxsin(Fixed32 degree)
{
    Sint16   sign;
    Fixed32  val;

    sign = 0;
    if(degree < FIXED(0)) {
        sign = 1;
        degree = -degree;
    }
    if(degree >= FIXED(180))
        degree = FIXED(0);
    else
    if(degree > FIXED(90))
	degree = FIXED(180) - degree;
    val = fsin(degree);
    if(sign) val = -val;
    return val;
}

/*
 *    -180 <= degree <= 180
 */
Fixed32 fxcos(Fixed32 degree)
{
    Sint16   sign;
    Fixed32  val;

    if(degree < FIXED(0)) degree = -degree;
    sign = 1;
    if(degree >= FIXED(180))
	degree = FIXED(0);
    else
    if(degree > FIXED(90))
	degree = FIXED(180) - degree;
    else
	sign = 0;
    val = fcos(degree);
    if(sign) val = -val;
    return val;
}

Void dmemcpy(Void *dst, Void *src, Uint32 wsize)
{
    memcpy(dst, src, wsize*2);
}

/*  end of file */



