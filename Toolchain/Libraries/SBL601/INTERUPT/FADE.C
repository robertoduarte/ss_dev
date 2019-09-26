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
#include	<sega_mth.h> 
#include	<string.h>

#include	"sega_spr.h" 
#include	"sega_scl.h"
#include	"sega_dma.h"
#include	"sega_gfs.h"

#include	"palette.h"
#include	"fade.h"


/*
 * #defines and macros.
 */

/* Color offset register addresses */
#define 		CLOFEN		0x5f80110
#define 		COAR   		0x5f80114
#define 		COAG		0x5f80116
#define 		COAB		0x5f80118
#define 		CLOFSL		0x5f80112


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
 * void FadeInit(void)
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

void FadeInit(void)
{
	/* Code. */

	/* Assign all screens to group A and enable fade */
	*(Uint16 *)CLOFEN=0x007f;					   
	*(Uint16 *)CLOFSL=0x0000;

	/* Set fade adjust level to none */
	*(Uint16 *)COAR=0;
	*(Uint16 *)COAB=0;
	*(Uint16 *)COAG=0;
}

/*****************************************************************************
 *
 * void FadeDisplay(Sint16 fade_adjust_level)
 *
 * Parameters:
 *	level_to_fade_to : Range -100 <= 0 <= 100 (percent)
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

void FadeDisplay(Sint16 fade_adjust_level)
{
	/* Variables. */

	Sint16 fade_calc;

	/* Code. */

	/* Filter range */
	if (fade_adjust_level < -100)
	{
		fade_adjust_level = -100;
	}

	if (fade_adjust_level > 100)
	{
		fade_adjust_level = 100;
	}

	fade_calc = (255 * fade_adjust_level) / 100;
	 	
	*(Uint16 *)COAR=fade_calc;
	*(Uint16 *)COAB=fade_calc;
	*(Uint16 *)COAG=fade_calc;
}

/*****************************************************************************
 *
 * void FadeToBlack(void)
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

void FadeToBlack(void)
{
	/* Variables. */

	Sint32 loop;

	/* Code. */

	loop = 0;
	while (loop >= -100)
	{
		FadeDisplay((Sint16)loop);
		NuVblDisplayFrame();
		loop--;
	}
}

/*****************************************************************************
 *
 * void FadeFromBlack(void)
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

void FadeFromBlack(void)
{
	/* Variables. */

	Sint32 loop;

	/* Code. */

	loop = -100;
	while (loop <= 0)
	{
		FadeDisplay((Sint16)loop);
		NuVblDisplayFrame();
		loop++;
	}
}
