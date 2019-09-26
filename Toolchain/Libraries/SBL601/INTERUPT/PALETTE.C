/*****************************************************************************
 *
 * Copyright © Sega Europe Ltd. 1995
 *
 * Contents:
 *	
 *
 *****************************************************************************
 *
 * $Log$
 *
 ****************************************************************************/

/*
 * Include files.
 */

#include	<machine.h>
#include	<sega_xpt.h>
#include	<sega_def.h> 

#include	"sega_spr.h" 
#include	"sega_scl.h"

#include	"palette.h"
#include	"fade.h"


/*
 * #defines and macros.
 */


/*
 * User type and enumerated type definitions.
 */


/*
 * Structure definitions.
 */


/*
 * Function prototypes.
 */


/*
 * Global variables.
 */

/*****************************************************************************
 *
 * static void PaletteSetPaletteBlock(Uint16 palette_block)
 *
 * Parameters:
 *										 
 * Returns:
 *
 * Globals and side effects:
 *
 * Runtime requirements:
 *
 * Critical cross references:
 *
 * Notes:
 *
 *
 ****************************************************************************/

static void PaletteSetPaletteBlock(Uint16 palette_block)
{
	/* Variables. */

	int	colour, i, j, r, g, b;

	/* Code. */

	/*
	 *  Palette 0 - Create a 256-colour blue-scale palette.
	 *  Palette 1 - Create a 256-colour red-scale palette.
	 */
	for (i = r = g = b = 0; i < 512; i += 16, ++r, ++g, ++b)
	{
		if (palette_block == 0)
		{
			colour = (b << 10) | 0x8000;
		}
		else
		{
			colour = r | 0x8000;
		}

		for (j = 0; j < 16; j += 2)
			*((Uint16 *) (SCL_COLRAM_ADDR + i + j + (palette_block << 1))) = colour;
	}

	/*
	 * Override some of the colours (specifically colour numbers 1 to 15)
	 */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_BLUE + palette_block) << 1))) 		
		= (Uint16)RGB16_COLOR(  0,  0, 15);	/* 1 Low Blue       */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_GREEN + palette_block) << 1))) 		
		= (Uint16)RGB16_COLOR(  0, 15,  0);	/* 2 Low Green      */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_CYAN + palette_block) << 1))) 		
		= (Uint16)RGB16_COLOR(  0, 15, 15);	/* 3 Low Cyan       */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_RED + palette_block) << 1))) 		
		= (Uint16)RGB16_COLOR( 15,  0,  0);	/* 4 Low Red        */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_MAGENTA + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR( 15,  0, 15);	/* 5 Low Magenta    */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_YELLOW + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR( 15, 15,  0);	/* 6 Low Yellow     */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_LOW_WHITE + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR( 15, 15, 15);	/* 7 Low White      */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_GREY + palette_block) << 1)))			
		= (Uint16)RGB16_COLOR(  8,  8,  8);	/* 8 Grey           */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_BLUE + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR(  0,  0, 31);	/* 9 Bright Blue    */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_GREEN + palette_block) << 1)))	
		= (Uint16)RGB16_COLOR(  0, 31,  0);	/* A Bright Green   */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_CYAN + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR(  0, 31, 31);	/* B Bright Cyan    */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_RED + palette_block) << 1)))		
		= (Uint16)RGB16_COLOR( 31,  0,  0);	/* C Bright Red     */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_MAGENTA + palette_block) << 1)))	
		= (Uint16)RGB16_COLOR( 31,  0, 31);	/* D Bright Magenta */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_YELLOW + palette_block) << 1)))	
		= (Uint16)RGB16_COLOR( 31, 31,  0);	/* E Bright Yellow  */
	*((Uint16 *) (SCL_COLRAM_ADDR + ((COLOUR_BRIGHT_WHITE + palette_block) << 1)))	
		= (Uint16)RGB16_COLOR( 31, 31, 31);	/* F Bright White   */
}

/*****************************************************************************
 *
 * void PaletteDefaultSelect(void)
 *
 * Parameters:
 *
 * Returns:
 *
 * Globals and side effects:
 *
 * Runtime requirements:
 *
 * Critical cross references:
 *
 * Notes:
 *
 *
 ****************************************************************************/

void PaletteDefaultSelect(void)
{
	/* Code. */

	PaletteSetPaletteBlock(0);
	PaletteSetPaletteBlock(256);
}
