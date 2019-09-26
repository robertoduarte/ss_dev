/*****************************************************************************
 *
 * Copyright © Sega Europe Ltd. 1995
 *
 * Contents:
 *
 * This is a developer sample. While every effort has been made to check this 
 * material  carefully, Sega accepts no responsibility implied or otherwise 
 * for it's use.
 *
 *****************************************************************************
 *
 * $Log$
 *
 ****************************************************************************/

/*
 * Include files.
 */
#include	<stdio.h>
#include	<stdlib.h>

#define		_SPR2_
#include	<sega_xpt.h>
#include	<sega_int.h>
#include	<sega_spr.h>
#include	<sega_scl.h>
#include	<sega_per.h>
#include	<sega_gfs.h>
#include	<string.h>
#include	<sega_tim.h>

#include	<machine.h>
#include	"sega_scl.h"
#include	"../../v_blank/v_blank.h"
#include	"../../per/smpclib\per_x.h"

#include	"palette.h"
#include	"fade.h"
#include	"vdp2.h"


/*
 * #defines and macros.
 */
#define LEFTX	10
#define LPP_TOP	10
#define	RPP_TOP	120
#define	LP_ADJ	(28*8)


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
 * Global/Static variables.
 */

static SprSpCmd  spCmd16_320_240[] = {
   {/* [ 0]     */
    /* control  */ (JUMP_NEXT | FUNC_SCLIP),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ DISPLAY_PIXEL_WIDTH-1, DISPLAY_PIXEL_HEIGHT-1,
    /* dx, dy   */ 0, 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [ 1]     */
    /* control  */ (JUMP_NEXT | FUNC_LCOORD),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ 0, 0,
    /* dx, dy   */ 0, 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* END      */
    /* control  */ CTRL_END,
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0,
    /* bx, by   */ 0,
    /* cx, cy   */ 0,
    /* dx, dy   */ 0,
    /* grshAddr */ 0,
    /* dummy    */ 0}
};

SclVramConfig	vramConfig;
SclConfig		config;

/*
 * Setup VDP2 VRAM with two screens as follows :
 *
 *	NBG0 - 512x512 bitmap - 256 colours - bank A
 *	NBG1 - 512x512 bitmap - 256 colours - bank B
 *
 *	Support 2x to 1/2x scaling (why not ?).
 */
Uint16			cyclePat[] =
      			{
     				0x4444, 0xeeee,		/* NBG0 - 512x512 bitmap - 256 colours - bank A */
					0x4444, 0xeeee,
					0x5555, 0xeeee,		/* NBG1 - 512x512 bitmap - 256 colours - bank B */
					0x5555, 0xeeee
				};

Uint16	black = 0;

Uint16	dummy[1];

/* DIAGNOSTIC */
#define MAX_STRING_BUFFER_SIZE 256
static 	Uint8 string_buffer[MAX_STRING_BUFFER_SIZE];
/* DIAGNOSTIC */

Uint32	Timer0handlerCounter = 0;
Uint32	Timer0handlerCompare = 0;

Uint32	Timer1handlerCounter = 0;

Uint32	HblankhandlerCounter = 0;

Uint32	FramesDisplayedCounter = 0;

/*****************************************************************************
 *
 * void NuVblDisplayFrame(void)
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
 *	When this routine has finished, you can guarantee that you are
 *	just commencing VSCAN.
 *
 ****************************************************************************/

void NuVblDisplayFrame(void)
{
	/* Variables. */

	CdcStat stats;

	/* Code. */

	CDC_GetCurStat(&stats);
	if (CDC_STAT_STATUS(&stats)==CDC_ST_OPEN)
	{
		 SYS_EXECDMP();
	}

	SCL_DisplayFrame();
}

void Timer0handler(void)
{
	/* Paranoia save of MACH/MACL */
	asm("
		sts.l	macl,@-r15
		sts.l	mach,@-r15
	");

   /* Update the timer 0 counter */
	Timer0handlerCounter += 1;

   /* Force multiple Timer 0 interrupts during this VSCAN */
	Timer0handlerCompare += DISPLAY_PIXEL_HEIGHT / 5;
	if (Timer0handlerCompare >= DISPLAY_PIXEL_HEIGHT)
   {
		Timer0handlerCompare = DISPLAY_PIXEL_HEIGHT / 5;
   }

	TIM_T0_SET_CMP(Timer0handlerCompare);

	/* Restore the Paranoia save of MACH/MACL */
	asm("

		lds.l	@r15+,mach
		lds.l	@r15+,macl
	");
}

void Timer1handler(void)
{
	/* Paranoia save of MACH/MACL */
	asm("
		sts.l	macl,@-r15
		sts.l	mach,@-r15
	");

   /* Update the timer 1 counter */
	Timer1handlerCounter += 1;
	TIM_T1_SET_DATA(1000);

	/* Restore the Paranoia save of MACH/MACL */
	asm("

		lds.l	@r15+,mach
		lds.l	@r15+,macl
	");
}

void Hblankhandler(void)
{
	/* Paranoia save of MACH/MACL */
	asm("
		sts.l	macl,@-r15
		sts.l	mach,@-r15
	");

   /* Update the timer 0 counter */
	HblankhandlerCounter += 1;

	/* Restore the Paranoia save of MACH/MACL */
	asm("

		lds.l	@r15+,mach
		lds.l	@r15+,macl
	");
}

/*****************************************************************************
 *
 * void TestDrawing(void)
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
 *	TESTBED FUNCTION ONLY !!!
 *
 ****************************************************************************/

void TestDrawing(void)
{
	/* Variables. */

	Sint32	innerloop;

	/* Code. */

	string_buffer[MAX_STRING_BUFFER_SIZE-1] = 0;

	/* Reset display */

	VDP2_PrintStringAt(LEFTX,LPP_TOP,   "Timer 0/1 - Simple demo sample V1.00");

   /* Print info section */
	VDP2_PrintStringAt(LEFTX,LPP_TOP+20,"Screen width x height = 320 x 240   ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+30,"                                    ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+40,"Snapshots are taken of all counters ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+50,"every 1000 display frames (the      ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+60,"bracketed numbers).                 ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+70,"                                    ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+80,"Sample illustrates Timer 0 / 1 and  ");
	VDP2_PrintStringAt(LEFTX,LPP_TOP+90,"Hblank interrupt handlers.          ");

	/* Print diagnostic line */
	VDP2_PrintStringAt(LEFTX,RPP_TOP,   "Value of timer 0 counter = ");
	VDP2_PrintStringAt(LEFTX,RPP_TOP+20,"Value of timer 1 counter = ");
	VDP2_PrintStringAt(LEFTX,RPP_TOP+40,"Frames displayed counter = ");
	VDP2_PrintStringAt(LEFTX,RPP_TOP+60,"Value of Hblank  counter = ");

	FadeFromBlack();

   /* Fire up Timer 0 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_TIM0);
	INT_SetScuFunc(INT_SCU_TIM0,Timer0handler);
	INT_ChgMsk(INT_MSK_TIM0,INT_MSK_NULL);

   /* Fire up Timer 1 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_TIM1);
	INT_SetScuFunc(INT_SCU_TIM1,Timer1handler);
	INT_ChgMsk(INT_MSK_TIM1,INT_MSK_NULL);

   /* Fire up the Hblank handler */

	INT_ChgMsk(INT_MSK_NULL,INT_MSK_HBLK_IN);
	INT_SetScuFunc(INT_SCU_HBLK_IN,Hblankhandler);
	INT_ChgMsk(INT_MSK_HBLK_IN,INT_MSK_NULL);

   /* Fire up Timer 0 */
	Timer0handlerCompare = DISPLAY_PIXEL_HEIGHT / 5;
	TIM_T0_SET_CMP(Timer0handlerCompare);

	TIM_T1_SET_DATA(1000);
/*   TIM_T1_SET_MODE(TIM_TENB_ON | TIM_T1MD_CST_LINE); */

   TIM_T1_SET_MODE(TIM_TENB_ON);

 	/* Display frames */

	while (1)
	{
		/* Update the screen display */

      /* Value of timer 0 counter */
		sprintf(string_buffer,"%08d",Timer0handlerCounter);
		VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP,string_buffer);

      /* Value of timer 1 counter */
		sprintf(string_buffer,"%08d",Timer1handlerCounter);
		VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+20,string_buffer);

      /* Value of Frames displayed counter */
		sprintf(string_buffer,"%08d",FramesDisplayedCounter);
		VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+40,string_buffer);

      /* Value of Hblank counter */
		sprintf(string_buffer,"%08d",HblankhandlerCounter);
		VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+60,string_buffer);

      /* Snapshot counters every thousand frames */
      if (FramesDisplayedCounter == ((FramesDisplayedCounter / 1000) * 1000))
      {
			sprintf(string_buffer,"(%08d)",Timer0handlerCounter);
			VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+10,string_buffer);

	      /* Value of timer 1 counter */
			sprintf(string_buffer,"(%08d)",Timer1handlerCounter);
			VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+30,string_buffer);

	      /* Value of Frames displayed counter */
			sprintf(string_buffer,"(%08d)",FramesDisplayedCounter);
			VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+50,string_buffer);

	      /* Value of Hblank counter */
			sprintf(string_buffer,"(%08d)",HblankhandlerCounter);
			VDP2_PrintStringAt(LEFTX+LP_ADJ,RPP_TOP+70,string_buffer);
      }

		FramesDisplayedCounter += 1;

		/* Now display the screen update */

		NuVblDisplayFrame();
	}
}

/*
 * The dummy call below has been created in response to changes made for
 * the following compiler :
 *
 * 		cygnus-2.6-95q1-SOA-950317
 *
 * In GCC.LOG the following extract is listed
 *
 * C++ is now enabled. If you're using straight C, and you'd like to
 * avoid the 5k of overhead for C++ global constructors, insert a dummy
 * function called "__main() {}" to prevent the C++ constructor code
 * from being linked. For reference, __main() is defined in libgcc.a.
 * (g++/6438)
 */
void __main(void)
{
}

/*****************************************************************************
 *
 *
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

void main()
{
	/* Variables. */

    Uint8  	*VRAM;
    Uint16 	*colAddr;

	/* Code. */

    set_imask(0);

	SCL_Vdp2Init();
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
		SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
	SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,SCL_SP_WINDOW);
	SCL_SetColRamMode(SCL_CRM15_2048);

    SetVblank();

    SPR_Initial(&VRAM);
    SCL_SetFrameInterval(-1);

    SCL_DisplayFrame();
    SCL_DisplayFrame();

	/* Essentially a dummy call to set transparency */
	SCL_AllocColRam(SCL_SPR,512,OFF);
	SCL_SetColRam(SCL_SPR,0,512,&dummy[0]);

	SCL_AllocColRam(SCL_SPR,512,OFF);
	SCL_SetColRam(SCL_SPR,512,512,&dummy[0]);

	/* Set priorities explicity - Don't assume any defaults */

	SCL_SetPriority(SCL_NBG1,5);
	SCL_SetPriority(SCL_NBG0,6);
    SCL_SetPriority(SCL_SPR,7);

	SCL_SetBack(BACK_COL_ADR, 1, &black);

	/* Configure VDP2 VRAM */
	SCL_InitVramConfigTb(&vramConfig);
	vramConfig.vramModeA = ON;
	vramConfig.vramModeB = ON;
	SCL_SetVramConfig(&vramConfig);

	/*
	 * Setup NBG0/1 to be 512*512 256 colour paletted bitmaps.
	 */
	SCL_InitConfigTb(&config);
	config.dispenbl 	 = ON;
	config.bmpsize 		 = SCL_BMP_SIZE_512X512;
	config.coltype 		 = SCL_COL_TYPE_256;
	config.datatype 	 = SCL_BITMAP;
	config.mapover       = SCL_OVER_0;
	config.plate_addr[0] = NBG0_VDP2_VRAM_BASE_ADDR;
	SCL_SetConfig(SCL_NBG0, &config);

    SCL_InitConfigTb(&config);
    config.dispenbl      = ON;
	config.bmpsize 		 = SCL_BMP_SIZE_512X512;
	config.coltype 		 = SCL_COL_TYPE_256;
	config.datatype 	 = SCL_BITMAP;
	config.mapover       = SCL_OVER_0;
    config.plate_addr[0] = NBG1_VDP2_VRAM_BASE_ADDR;
	SCL_SetConfig(SCL_NBG1, &config);

	SCL_SetCycleTable(cyclePat);

	/* Set up default palette(s) */
	PaletteDefaultSelect();

	/* Select DISPLAY_PIXEL_WIDTH*DISPLAY_PIXEL_HEIGHT non-interlaced display */
    SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
    SPR_SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);

    SPR_SetEraseData(0x8000,0,0,DISPLAY_PIXEL_WIDTH-1,DISPLAY_PIXEL_HEIGHT-1);

	SCL_SetFrameInterval(1);

	/* Initialise VDP1 VRAM */
    memcpy(VRAM,spCmd16_320_240,sizeof(spCmd16_320_240));

	/* Initialise VDP2 VRAM */
	VDP2_InitVRAM();

	/* Initialise screen display */
	FadeInit();
	FadeDisplay(-100);
	PaletteDefaultSelect();

	/* Note - No return from following routine */
	TestDrawing();
}

