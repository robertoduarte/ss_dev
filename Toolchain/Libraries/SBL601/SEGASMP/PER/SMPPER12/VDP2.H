/*
**■VDP2.H
**	VDP2 のハードウェアを定義する。
**	
**
*/
#if	!defined( __VDP2_H )
	#define	__VDP2_H
	
	/****************************************
	*			include files				*
	****************************************/
	#include	"sega_xpt.h"
	
	/****************************************
	*			define constants			*
	****************************************/
	#define	AD_VDP2			(0x20000000+0x05E00000)
	
	#define	AD_VDP2_VRAM	(AD_VDP2+0x00000000)
	#define	AD_VDP2_COLOR	(AD_VDP2+0x00100000)
	#define	AD_VDP2_REG		(AD_VDP2+0x00180000)
	
	#define	VDP2_REG		(( volatile Uint16 * )AD_VDP2_REG)
	#define	VDP2_TVMD		VDP2_REG[0]
	#define	VDP2_EXTEN		VDP2_REG[1]
	#define	VDP2_TVSTAT		VDP2_REG[2]
	#define	VDP2_VRSIZE		VDP2_REG[3]
	#define	VDP2_HCNT		VDP2_REG[4]
	#define	VDP2_VCNT		VDP2_REG[5]
	#define	VDP2_RAMCTL		VDP2_REG[7]
	#define	VDP2_CYCLE		(&VDP2_REG[8])
	#define	VDP2_CYCA0L		VDP2_CYCLE[0]
	#define	VDP2_CYCA0U		VDP2_CYCLE[1]
	#define	VDP2_CYCA1L		VDP2_CYCLE[2]
	#define	VDP2_CYCA1U		VDP2_CYCLE[3]
	#define	VDP2_CYCB0L		VDP2_CYCLE[4]
	#define	VDP2_CYCB0U		VDP2_CYCLE[5]
	#define	VDP2_CYCB1L		VDP2_CYCLE[6]
	#define	VDP2_CYCB1U		VDP2_CYCLE[7]
	#define	VDP2_BGON		VDP2_REG[16]
	
	#define	VDP2_TVMD_DISP		0x8000
	#define	VDP2_TVMD_BDCLMD	0x0100
	#define	VDP2_TVMD_LSMD		0x00c0
	#define	VDP2_TVMD_VRESO		0x0030
	#define	VDP2_TVMD_HRESO		0x0007
	
	#define	VDP2_EXTEN_EXLTEN	0x0200
	#define	VDP2_EXTEN_EXSYEN	0x0100
	#define	VDP2_EXTEN_DASEL	0x0002
	#define	VDP2_EXTEN_EXBGEN	0x0001
	
	#define	VDP2_TVSTAT_EXLTFG	0x0200
	#define	VDP2_TVSTAT_EXSYFG	0x0100
	#define	VDP2_TVSTAT_VBLANK	0x0008
	#define	VDP2_TVSTAT_HBLANK	0x0004
	#define	VDP2_TVSTAT_ODD		0x0002
	#define	VDP2_TVSTAT_PAL		0x0001
	
	#define	VDP2_VRSIZE_VRAMSZ	0x8000
	#define	VDP2_VRSIZE_VER		0x000f
	
	#define	VDP2_RAMCTL_CRKTE	0x8000
	#define	VDP2_RAMCTL_CRMD	0x3000
	#define	VDP2_RAMCTL_VRBMD	0x0300
	#define	VDP2_RAMCTL_RDBSB1	0x00c0
	#define	VDP2_RAMCTL_RDBSB0	0x0030
	#define	VDP2_RAMCTL_RDBSA1	0x000c
	#define	VDP2_RAMCTL_RDBSA0	0x0003
	
	#define	VDP2_CYCLE_T0		0xf000
	#define	VDP2_CYCLE_T1		0x0f00
	#define	VDP2_CYCLE_T2		0x00f0
	#define	VDP2_CYCLE_T3		0x000f
	#define	VDP2_CYCLE_T4		0xf000
	#define	VDP2_CYCLE_T5		0x0f00
	#define	VDP2_CYCLE_T6		0x00f0
	#define	VDP2_CYCLE_T7		0x000f
	
#endif
