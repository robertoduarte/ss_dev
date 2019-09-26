/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:Mask table, Memcpy, and so on.
 * File		:pcm_etc.c
 * Date		:1994-09-29
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include "sega_xpt.h"
#include "pcm_msub.h"

const Uint32 pcm_bit_msk[32] = {
		0x00000001, 0x00000002, 0x00000004, 0x00000008, 
		0x00000010, 0x00000020, 0x00000040, 0x00000080, 
		0x00000100, 0x00000200, 0x00000400, 0x00000800, 
		0x00001000, 0x00002000, 0x00004000, 0x00008000, 
		0x00010000, 0x00020000, 0x00040000, 0x00080000, 
		0x00100000, 0x00200000, 0x00400000, 0x00800000, 
		0x01000000, 0x02000000, 0x04000000, 0x08000000, 
		0x10000000, 0x20000000, 0x40000000, 0x80000000, 
};

const Uint8 pcm_bittbl[8] = {
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

void pcm_MemsetWord(Uint16 *dst, Uint16 value, Sint32 cnt)
{
	dst += cnt;
	while (--cnt >= 0) {
		*--dst = value;
	}
}

void pcm_MemsetDword(Uint32 *dst, Uint32 value, Sint32 cnt)
{
	dst += cnt;
	while (--cnt >= 0) {
		*--dst = value;
	}
}

/* void pcm_aaa(void){} */

void pcm_MemcpyDword(Uint32 *dst, Uint32 *src, Sint32 dwsize)
{
/*	if ((*(src+0) == 0)&&(*(src+1) == 0)&&(*(src+2) == 0)&&(*(src+3) == 0)&&
		(*(src+4) == 0)&&(*(src+5) == 0)&&(*(src+6) == 0)&&(*(src+7) == 0)
		) {
		pcm_aaa();
	}
*/
	dst += dwsize;
	src += dwsize;
	while (--dwsize >= 0) {
		*--dst = *--src;
	}
}

void pcm_MemmoveByte(Sint8 *dst, Sint8 *src, Sint32 bsize)
{
	if (dst < src) {
		while (--bsize >= 0) {
			*dst++ = *src++;
		}
	} else if (dst > src) {
		dst += bsize;
		src += bsize;
		while (--bsize >= 0) {
			*--dst = *--src;
		}
	}
}

void pcm_MemmoveDword(Sint32 *dst, Sint32 *src, Sint32 dwsize)
{
	if (dst < src) {
		while (--dwsize >= 0) {
			*dst++ = *src++;
		}
	} else if (dst > src) {
		dst += dwsize;
		src += dwsize;
		while (--dwsize >= 0) {
			*--dst = *--src;
		}
	}
}

/*******************************************************************
【機　能】
	bsize_shift 分のバッファ中のデータの回転。
					<-- bsize_pcmbuf ------------------->
					          <--> bsize_shift
		処理前：	aaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbb
		処理後：	aaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbaaaa
【引　数】
	addr_pwork		(入力)　ポーズ作業用バッファ
	addr_pcmbuf		(入出力)処理対象バッファ
	bsize_pcmbuf	(入力)	処理対象バッファサイズ[byte]
	dwsize_pcmbuf	(入力)	処理対象バッファサイズ[dword]
	bsize_shift		(入力)　シフトサイズ(回転させる量)[byte]
	dwsize_shift	(入力)　シフトサイズ(回転させる量)[dword]
【戻り値】
	なし
【注　意】
	シフトサイズは、ポーズ作業用バッファサイズ以下のサイズでなければならない。
*******************************************************************/
void pcm_RotateByte(Sint8 *addr_pwork, Sint8 *addr_pcmbuf, Sint32 bsize_pcmbuf, Sint32 bsize_shift)
{
	Sint32 	offset2 = bsize_pcmbuf - bsize_shift;

	pcm_MemmoveByte(addr_pwork, addr_pcmbuf, bsize_shift);
	pcm_MemmoveByte(addr_pcmbuf, addr_pcmbuf + bsize_shift, offset2);
	pcm_MemmoveByte(addr_pcmbuf + offset2, addr_pwork, bsize_shift);
}
void pcm_RotateDword(Sint32 *addr_pwork, Sint32 *addr_pcmbuf, Sint32 dwsize_pcmbuf, Sint32 dwsize_shift)
{
	Sint32 	offset2 = dwsize_pcmbuf - dwsize_shift;

	pcm_MemmoveDword(addr_pwork, addr_pcmbuf, dwsize_shift);
	pcm_MemmoveDword(addr_pcmbuf, addr_pcmbuf + dwsize_shift, offset2);
	pcm_MemmoveDword(addr_pcmbuf + offset2, addr_pwork, dwsize_shift);
}

