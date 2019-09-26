/****************************************************************************/
/*  filename "smpdma1.c"                                                    */
/*  ＤＭＡサンプルプログラム                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by N.T on 1994-05-14 Ver.0.90                                   */
/*  Updated by N.T on 1994-05-14 Ver.0.90                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_csh.h"
#include "sega_dma.h"

static Sint8 *pbuf;                         /*  結果格納バッファポインタ    */

Uint32 hikaku(void *, void *, Uint32, Uint32, Sint8 *); /*  結果確認関数    */
void rev(void *, void *, Uint32, Uint32);   /*  転送先データ破壊関数        */

void main(void)
{
    void *src, *dst;
    Uint32 cnt;
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
    Uint32 status;
    Uint32 *work;
#endif

    pbuf = (Sint8 *)0x06040000;             /*  結果格納先頭アドレス        */

/*  BOOT to WORK    */
    src = (void *)0x00000000;   /*  WORK address    */
    dst = (void *)0x6050000;   /*  WORK address    */
    cnt = 0x100;

    rev(src, dst, cnt, 1);
    DMA_CpuMemCopy1(dst, src, cnt);
    CSH_Purge(dst, cnt);

    hikaku(src, dst, cnt, 1, "BOOT to WORK");

    for(;;) ;
}
/****************************************************************************/
/*  結果比較関数                                                            */
/****************************************************************************/
Uint32 hikaku(void *src, void *dst, Uint32 cnt, Uint32 size, Sint8 *msg)
{
#if	0
	/*
	**■1995-07-28	高橋智延
	**	使ってないので削除
	*/
    Uint32 i, p;
#else
	Uint32	i;
#endif

#if	0
	/*
	**■1995-07-28	高橋智延
	**	long なオブジェクトは %X ではなく %lX
	**	ポインタの値をそのまま表示しようとしているので ( Uint32 ) にキャスト
	*/
    sprintf(pbuf, "[[ \'%s\' %08X >> %08X : %08X Count, %01X Byte ",
     msg, src, dst, cnt, size);
#else
    sprintf(pbuf, "[[ \'%s\' %08lX >> %08lX : %08lX Count, %01lX Byte ",
     msg, ( Uint32 )src, ( Uint32 )dst, cnt, size);
#endif
    pbuf += strlen(pbuf);
    for(i = 0; i < cnt; i++) {
        switch(size) {
            case 1:
                if(*(Uint8 *)dst != *(Uint8 *)src) {
#if	0
	/*
	**■1995-07-28	高橋智延
	**	long なオブジェクトは %X ではなく %lX
	**	ポインタの値をそのまま表示しようとしているので ( Uint32 ) にキャスト
	*/
                    sprintf(pbuf, "Error %08X(%02X,%02X) ]] \n",
                     dst, *(Uint8 *)src, *(Uint8 *)dst);
#else
                    sprintf(pbuf, "Error %08lX(%02X,%02X) ]] \n",
                     ( Uint32 )dst, *(Uint8 *)src, *(Uint8 *)dst);
#endif
                    pbuf += strlen(pbuf);
                    return 1;
                }
                break;
            case 2:
                if(*(Uint16 *)dst != *(Uint16 *)src) {
#if	0
	/*
	**■1995-07-28	高橋智延
	**	long なオブジェクトは %X ではなく %lX
	**	ポインタの値をそのまま表示しようとしているので ( Uint32 ) にキャスト
	*/
                    sprintf(pbuf, "Error %08X(%04X,%04X) ]] \n",
                     dst, *(Uint16 *)src, *(Uint16 *)dst);
#else
                    sprintf(pbuf, "Error %08lX(%04X,%04X) ]] \n",
                     ( Uint32 )dst, *(Uint16 *)src, *(Uint16 *)dst);
#endif
                    pbuf += strlen(pbuf);
                    return 1;
                }
                break;
            case 4:
                if(*(Uint32 *)dst != *(Uint32 *)src) {
#if	0
	/*
	**■1995-07-28	高橋智延
	**	long なオブジェクトは %X ではなく %lX
	**	ポインタの値をそのまま表示しようとしているので ( Uint32 ) にキャスト
	*/
                    sprintf(pbuf, "Error %08X(%08X,%08X) ]] \n",
                     dst, *(Uint32 *)src, *(Uint32 *)dst);
#else
                    sprintf(pbuf, "Error %08lX(%08lX,%08lX) ]] \n",
                     ( Uint32 )dst, *(Uint32 *)src, *(Uint32 *)dst);
#endif
                    pbuf += strlen(pbuf);
                    return 1;
                }
                break;
            case 16:
                if((*(Uint32 *)dst != *(Uint32 *)src)
                 || (*((Uint32 *)dst + 1) != *((Uint32 *)src + 1))
                 || (*((Uint32 *)dst + 2) != *((Uint32 *)src + 2))
                 || (*((Uint32 *)dst + 3) != *((Uint32 *)src + 3))) {
#if	0
	/*
	**■1995-07-28	高橋智延
	**	long なオブジェクトは %X ではなく %lX
	**	ポインタの値をそのまま表示しようとしているので ( Uint32 ) にキャスト
	*/
	                    sprintf(pbuf,
                     "Error %08X(%08X%08X%08X%08X,%08X%08X%08X%08X) ]] \n",
                      dst, *(Uint32 *)src, *(Uint32 *)((Uint32)src + 4),
                      *(Uint32 *)((Uint32)src + 8),
                      *(Uint32 *)((Uint32)src + 12),
                      *(Uint32 *)dst, *(Uint32 *)((Uint32)dst + 4),
                      *(Uint32 *)((Uint32)dst + 8),
                      *(Uint32 *)((Uint32)dst + 12));
#else
                    sprintf(pbuf,
                     "Error %08lX(%08lX%08lX%08lX%08lX,%08lX%08lX%08lX%08lX) ]] \n",
                      ( Uint32 )dst, *(Uint32 *)src, *(Uint32 *)((Uint32)src + 4),
                      *(Uint32 *)((Uint32)src + 8),
                      *(Uint32 *)((Uint32)src + 12),
                      *(Uint32 *)dst, *(Uint32 *)((Uint32)dst + 4),
                      *(Uint32 *)((Uint32)dst + 8),
                      *(Uint32 *)((Uint32)dst + 12));
#endif
                    pbuf += strlen(pbuf);
                    return 1;
                }
                i += 3;
                break;
            default:
                break;
        }
        dst = (void *)((Uint32)dst + size);
        src = (void *)((Uint32)src + size);
    }
    sprintf(pbuf, "OK ]] \n");
    pbuf += strlen(pbuf);
    return 0;
}

/****************************************************************************/
/*  転送先内容破壊関数                                                      */
/****************************************************************************/
void rev(void *src, void *dst, Uint32 cnt, Uint32 size)
{
    Uint32 i;
    
    for(i = 0; i < cnt; i++) {
        switch(size) {
            case 1:
                *(Uint8 *)dst = ~*(Uint8 *)src;
                break;
            case 2:
                *(Uint16 *)dst = ~*(Uint16 *)src;
                break;
            case 4:
                *(Uint32 *)dst = ~*(Uint32 *)src;
                break;
            case 16:
                *(Uint32 *)dst = ~*(Uint32 *)src;
                *((Uint32 *)dst + 1) = ~*((Uint32 *)src + 1);
                *((Uint32 *)dst + 2) = ~*((Uint32 *)src + 2);
                *((Uint32 *)dst + 3) = ~*((Uint32 *)src + 3);
                i += 3;
                break;
            default:
                break;
        }
        src = (void *)((Uint32)src + size);
        dst = (void *)((Uint32)dst + size);
    }
}

/****************************************************************************/
/*  End of File                                                             */
/****************************************************************************/
