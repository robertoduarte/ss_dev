/*
**■SMPC.H
**	SMPC のハードウェアを定義する。
**	
**	インターフェイスレジスタ
**		COMREG	-/W	b/-/-	1Uint8
**		SR		R/-	b/-/-	1Uint8
**		SF		R/W	b/-/-	1bit
**		IREG	-/W	b/-/-	1Uint8*7
**		OREG	R/-	b/-/-	1Uint8*32
**	
**	パラレルＩ／Ｏレジスタ
**		PDR		R/W	b/-/-	7bit*2
**		DDR		-/W	b/-/-	7bit*2
**		IOSEL	-/W	b/-/-	2bit
**		EXLE	-/W	b/-/-	2bit
**	
**	リセット系コマンド
**		MSHON		00H
**		SSHON		02H
**		SSHOFF		03H
**		SNDON		06H
**		SNDOFF		07H
**		CDON		08H
**		CDOFF		09H
**		SYSRES		0DH
**		CKCHG352	0EH
**		CKCHG320	0FH
**		NMIREQ		18H
**		RESENAB		19H
**		RESDISA		1AH
**	
**	非リセット系コマンド
**		INTBACK		10H
**		SETSMEM		17H
**	
**	リアルタイムクロック系コマンド
**		SETTIME		16H
**
*/
#if		!defined( __SMPC_H )
	#define	__SMPC_H
	
	/****************************************
	*			include files				*
	****************************************/
	#include	"sega_xpt.h"
	
	/****************************************
	*			define constants			*
	****************************************/
	#define	_MAX_SMPC_IREG	7
	#define	_MAX_SMPC_OREG	32
	#define	_MAX_SMPC_PDR	2
	#define	_MAX_SMPC_DDR	2
	#define	_MAX_SMPC_SMEM	4
	
	#define	AD_SMPC_COMREG	0x2010001F
	#define	AD_SMPC_SR		0x20100061
	#define	AD_SMPC_SF		0x20100063
	#define	AD_SMPC_IREG	0x20100001
	#define	AD_SMPC_OREG	0x20100021
	#define	AD_SMPC_PDR		0x20100075
	#define	AD_SMPC_DDR		0x20100079
	#define	AD_SMPC_IOSEL	0x2010007D
	#define	AD_SMPC_EXLE	0x2010007F
	
	#define	SMPC_COMREG		(*( volatile Uint8 * )AD_SMPC_COMREG)
	#define	SMPC_SR			(*( volatile const Uint8 * )AD_SMPC_SR)
	#define	SMPC_SF			(*( volatile Uint8 * )AD_SMPC_SF)
	#define	SMPC_IREG(n)	(*(( volatile Uint8 * )AD_SMPC_IREG+(n)*2))
	#define	SMPC_OREG(n)	(*(( volatile const Uint8 * )AD_SMPC_OREG+(n)*2))
	#define	SMPC_PDR(n)		(*(( volatile Uint8 * )AD_SMPC_PDR+(n)*2))
	#define	SMPC_DDR(n)		(*(( volatile Uint8 * )AD_SMPC_DDR+(n)*2))
	#define	SMPC_IOSEL		(*( volatile Uint8 * )AD_SMPC_IOSEL)
	#define	SMPC_EXLE		(*( volatile Uint8 * )AD_SMPC_EXLE)
	
	#define	SMPC_COMREG_MSHON		0x00
	#define	SMPC_COMREG_SSHON		0x02
	#define	SMPC_COMREG_SSHOFF		0x03
	#define	SMPC_COMREG_SNDON		0x06
	#define	SMPC_COMREG_SNDOFF		0x07
	#define	SMPC_COMREG_CDON		0x08
	#define	SMPC_COMREG_CDOFF		0x09
	#define	SMPC_COMREG_SYSRES		0x0D
	#define	SMPC_COMREG_CKCHG352	0x0E
	#define	SMPC_COMREG_CKCHG320	0x0F
	#define	SMPC_COMREG_NMIREQ		0x18
	#define	SMPC_COMREG_RESENAB		0x19
	#define	SMPC_COMREG_RESDISA		0x1A
	
	#define	SMPC_COMREG_INTBACK		0x10
	#define	SMPC_COMREG_SETSMEM		0x17
	
	#define	SMPC_COMREG_SETTIME		0x16
	
	#define	SMPC_SF_BUSY		0x01
	
	#define	SMPC_SR_PDL			0x40
	#define	SMPC_SR_NPE			0x20
	#define	SMPC_SR_PDE			0x20
	#define	SMPC_SR_RESB		0x10
	#define	SMPC_SR_P2MD		0x0c
	#define	SMPC_SR_P1MD		0x03
	
	#define	SMPC_IREG1_P1MD_15	0x00
	#define	SMPC_IREG1_P1MD_255	0x10
	#define	SMPC_IREG1_P1MD_0	0x30
	#define	SMPC_IREG1_P2MD_15	0x00
	#define	SMPC_IREG1_P2MD_255	0x40
	#define	SMPC_IREG1_P2MD_0	0xc0
	#define	SMPC_IREG1_PEN		0x08
	#define	SMPC_IREG1_OPE		0x02
	
	#define	SMPC_IREG0_CONTINUE	0x80
	#define	SMPC_IREG0_BREAK	0x40
	
	#define	SMPC_OREG0_STE		0x80
	#define	SMPC_OREG0_RESD		0x40
	
	#define	SMPC_OREG9_NTSC		0x00
	#define	SMPC_OREG9_PAL		0x08
	#define	SMPC_OREG9_JAPAN				(0x01|SMPC_OREG9_NTSC)
	#define	SMPC_OREG9_KOREA				(0x02|SMPC_OREG9_NTSC)
	#define	SMPC_OREG9_ASIA_NTSC			(0x02|SMPC_OREG9_NTSC)
	#define	SMPC_OREG9_NORTH_AMERICA		(0x04|SMPC_OREG9_NTSC)
	#define	SMPC_OREG9_LATIN_AMERICA_NTSC	(0x04|SMPC_OREG9_NTSC)
	#define	SMPC_OREG9_ASIA_PAL				(0x04|SMPC_OREG9_PAL)
	#define	SMPC_OREG9_EU					(0x04|SMPC_OREG9_PAL)
	#define	SMPC_OREG9_LATIN_AMERICA_PAL	(0x04|SMPC_OREG9_PAL)
	
	#define	SMPC_OREG10_SNDRES		0x01
	#define	SMPC_OREG10_SYSRES		0x02
	#define	SMPC_OREG10_MSHNMI		0x08
	#define	SMPC_OREG10_DOTSEL		0x40
	#define	SMPC_OREG11_CDRES		0x40
	
	#define	SMPC_OREG12_LANGUAGE	0x0f
	#define	SMPC_OREG13_SE			0x01
	#define	SMPC_OREG13_SOUND		0x02
	#define	SMPC_OREG13_HELPWIN		0x04
	
	#define	SMPC_OREG12_ENGLISH		0x00
	#define	SMPC_OREG12_DEUTSCH		0x01
	#define	SMPC_OREG12_FRANCAIS	0x02
	#define	SMPC_OREG12_ESPNOL		0x03
	#define	SMPC_OREG12_ITALIANO	0x04
	#define	SMPC_OREG12_JAPANESE	0x05
	
	/****************************************
	*			define macro				*
	****************************************/
	#define	_smpc_busy()		(SMPC_SF=SMPC_SF_BUSY)
	#define	_smpc_isbusy()		(SMPC_SF&SMPC_SF_BUSY)
	#define	_smpc_sendcommand( command )	\
								(SMPC_COMREG=( Uint8 )(command))
	
	#define	_smpc_MSHON()		_smpc_sendcommand( SMPC_COMREG_MSHON )
	#define	_smpc_SSHON()		_smpc_sendcommand( SMPC_COMREG_SSHON )
	#define	_smpc_SSHOFF()		_smpc_sendcommand( SMPC_COMREG_SSHOFF )
	#define	_smpc_SNDON()		_smpc_sendcommand( SMPC_COMREG_SNDON )
	#define	_smpc_SNDOFF()		_smpc_sendcommand( SMPC_COMREG_SNDOFF )
	#define	_smpc_CDON()		_smpc_sendcommand( SMPC_COMREG_CDON )
	#define	_smpc_CDOFF()		_smpc_sendcommand( SMPC_COMREG_CDOFF )
	#define	_smpc_SYSRES()		_smpc_sendcommand( SMPC_COMREG_SYSRES )
	#define	_smpc_CKCHG352()	_smpc_sendcommand( SMPC_COMREG_CKCHG352 )
	#define	_smpc_CKCHG320()	_smpc_sendcommand( SMPC_COMREG_CKCHG320 )
	#define	_smpc_NMIREQ()		_smpc_sendcommand( SMPC_COMREG_NMIREQ )
	#define	_smpc_RESENAB()		_smpc_sendcommand( SMPC_COMREG_RESENAB )
	#define	_smpc_RESDISA()		_smpc_sendcommand( SMPC_COMREG_RESDISA )
	#define	_smpc_INTBACK()		_smpc_sendcommand( SMPC_COMREG_INTBACK )
	#define	_smpc_SETSMEM()		_smpc_sendcommand( SMPC_COMREG_SETSMEM )
	#define	_smpc_SETTIME()		_smpc_sendcommand( SMPC_COMREG_SETTIME )
	
#endif
