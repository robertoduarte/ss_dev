/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994,1995 SEGA
 *
 * Library	:ＰＣＭ・ＡＤＰＣＭ再生ライブラリ
 * Module 	:Debug aide
 * File		:pcm_msub.h
 * Date		:1994-09-29
 * Version	:1.00
 * Auther	:Y.T
 *
 ****************************************************************************/
#include "sega_xpt.h"


/*
#define ADDR_START		(0x06050000)
#define ADDR_END		(0x06060000)

#define ADDR_START_0	(0x04000000)
#define ADDR_START		(0x04000010)
#define ADDR_END		(0x04FFFFF0)
*/
#define ADDR_START_0	(0x04FF0000)
#define ADDR_START		(0x04FF0010)
#define ADDR_END		(0x04FFFFF0)

void debug_write_data(Uint32 data);

Sint32	cnt_retur_start = 0;

static Uint32 addr_write = ADDR_START;
void debug_write_str(char *str)
{
#ifndef _PCMD
	return;
#else
	char *w;

	w = (char *)addr_write;
	while (*str != '\0') {
		*w++ = *str++;
	}
	addr_write = (Uint32)w;
	addr_write = (addr_write + 3) & 0xFFFFFFFC;
	if (addr_write >= ADDR_END) {
		addr_write = ADDR_START;
		cnt_retur_start++;
		debug_write_str("cnt_return_s");
		debug_write_data(cnt_retur_start);
	}
#endif
}

void debug_write_data(Uint32 data)
{
#ifndef _PCMD
	return;
#else
	*(Uint32 *)ADDR_START_0 = addr_write;
	*(Uint32 *)addr_write = data;
	addr_write += 4;
	if (addr_write >= ADDR_END) {
		addr_write = ADDR_START;
		cnt_retur_start++;
		debug_write_str("cnt_return_s");
		debug_write_data(cnt_retur_start);
	}
#endif
}
