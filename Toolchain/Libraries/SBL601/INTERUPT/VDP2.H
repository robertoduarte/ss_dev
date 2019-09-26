/*****************************************************************************
 *
 * Copyright © Sega Europe Ltd. 1995
 *
 * Contents:
 *
 *
 *
 *****************************************************************************
 *
 * $Log$
 *
 ****************************************************************************/

#ifndef _VDP2_H_INCLUDED
#define _VDP2_H_INCLUDED

/*
 * Include files.
 */


/*
 * #defines and macros.
 */
#define BACK_COL_ADR 				(SCL_VDP2_VRAM_A1 + 0x1fffe)
#define BITMAP_PIXEL_WIDTH			512
#define	BITMAP_PIXEL_HEIGHT		512
#define	DISPLAY_PIXEL_WIDTH		320
#define	DISPLAY_PIXEL_HEIGHT		240

/* NBG0 / 1 control */
#define	NBG0_VDP2_VRAM_BASE_ADDR	SCL_VDP2_VRAM_A0
#define	NBG1_VDP2_VRAM_BASE_ADDR	SCL_VDP2_VRAM_B0

/* VDP2 print control */
#define	VDP2_FONT_8x8_MONO	0
#define	VDP2_FONT_MAX		VDP2_FONT_8x8_MONO

#define	VDP2_PRINT_NBG0		0
#define	VDP2_PRINT_NBG1		1
#define	VDP2_PRINT_MAX		VDP2_PRINT_NBG1
/*
 * User type and enumerated type definitions.
 */


/*
 * Structure definitions.
 */


/*
 * Function prototypes.
 */

/* VDP2 - Print control */
void VDP2_PrintHomeCursor(void);
void VDP2_PrintString(char *ascii_string);
void VDP2_PrintStringAt(Sint16 xpos,Sint16 ypos,char *ascii_string);
void VDP2_SelectCurrentPrintScreen(Uint16 select_screen);
void VDP2_SelectCurrentFont(Uint16 select_font);

void VDP2_InitVRAM(void);

/*
 * External variables.
 */


#endif
