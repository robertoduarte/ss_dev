
/*--------------------------------------------------------------*/
/*	Virtua Gun Control Sample				*/
/*--------------------------------------------------------------*/

#include	"sgl.h"

#define		BACK_COL_ADR			((void*)(VDP2_VRAM_B1 + 0x1fffe))
#define		MEGA_ID_StnShooting		0x0a

extern Sint8 SynchCount;
extern Uint16 VDP2_EXTEN;

static Uint8 GunCtrlFlag1P, GunCtrlFlag2P;

static Uint8 getMegaDriveId(Uint8 pd1, Uint8 pd0)
{
	Uint8 mid;

	pd1  = (pd1 & 0x0a) | ((pd1 & 0x05) << 1);
	mid  = (pd1 & 0x08) | ((pd1 & 0x02) << 1);
	pd0  = (pd0 & 0x05) | ((pd0 & 0x0a) >> 1);
	mid |= (pd0 & 0x01) | ((pd0 & 0x04) >> 1);

	return(mid);
}

static void gunCtrlTrigger()
{
	PerPoint* pPerPoint;
	Uint8 pd1, pd0, mid;

	if(SynchCount) return;
	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[0]));
	if((GunCtrlFlag1P)&&(pPerPoint->id == PER_ID_StnShooting)) {
		slSetPortDir1(0x40);
		pd1 = slGetPortData1();
		slSetPortDir1(0x00);
		pd0 = slGetPortData1();
		mid = getMegaDriveId(pd1, pd0);
		if(mid != MEGA_ID_StnShooting) {
			GunCtrlFlag1P = FALSE;
			slIntBackCancel();
		}
	}

	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[15]));
	if((GunCtrlFlag2P)&&(pPerPoint->id == PER_ID_StnShooting)) {
		slSetPortDir2(0x40);
		pd1 = slGetPortData2();
		slSetPortDir2(0x00);
		pd0 = slGetPortData2();
		mid = getMegaDriveId(pd1, pd0);
		if(mid != MEGA_ID_StnShooting) {
			GunCtrlFlag2P = FALSE;
			slIntBackCancel();
		}
	}
}

static void gunCtrlInit()
{
	VDP2_EXTEN = 0x0200;
	GunCtrlFlag1P = GunCtrlFlag2P = FALSE;
	slGetStatus();
	slIntFunction(gunCtrlTrigger);
}

static void gunCtrlPortMode()
{
	PerPoint* pPerPoint;
	static Uint8 id1 = PER_ID_StnShooting;
	static Uint8 id2 = PER_ID_StnShooting;

	if(!(slCheckIntBackSet())) {
		if(GunCtrlFlag1P) {
			slSetPortDir1(0x00);
			slSetPortData1(0x7f);
			slSetPortSelect1(SMPC_SH2_DIRECT);
		} else {
			slSetPortSelect1(SMPC_CONTROL);
			slSetPortExt1(SMPC_EXL_DIS);
		}
		if(GunCtrlFlag2P) {
			slSetPortDir2(0x00);
			slSetPortData2(0x7f);
			slSetPortSelect2(SMPC_SH2_DIRECT);
		} else {
			slSetPortSelect2(SMPC_CONTROL);
			slSetPortExt2(SMPC_EXL_DIS);
		}
		slGetStatus();
		return;
	}

	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[0]));
	if((id1 != PER_ID_NotConnect)&&
		(pPerPoint->id == (MEGA_ID_StnShooting | 0xf0))) {
		GunCtrlFlag1P = TRUE;
		slIntBackCancel();
	}
	id1 = pPerPoint->id;

	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[15]));
	if((id2 != PER_ID_NotConnect)&&
		(pPerPoint->id == (MEGA_ID_StnShooting | 0xf0))) {
		GunCtrlFlag2P = TRUE;
		slIntBackCancel();
	}
	id2 = pPerPoint->id;
}

static void getGunPointData()
{
	PerPoint* pPerPoint;

	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[0]));
	slPrint("Player 1    ID =", slLocate(4, 4));
	slPrint("Data =", slLocate(14, 5));
	slPrintHex(pPerPoint->id, slLocate(24, 4));
	slPrintHex(pPerPoint->data, slLocate(24, 5));
	if(pPerPoint->id == PER_ID_StnShooting) {
		slPrint("X =", slLocate(17, 6));
		slPrint("Y =", slLocate(17, 7));
		slPrintHex(slHex2Dec(pPerPoint->x), slLocate(24, 6));
		slPrintHex(slHex2Dec(pPerPoint->y), slLocate(24, 7));
	}

	pPerPoint = (PerPoint*)(&(Smpc_Peripheral[15]));
	slPrint("Player 2    ID =", slLocate(4, 10));
	slPrint("Data =", slLocate(14, 11));
	slPrintHex(pPerPoint->id, slLocate(24, 10));
	slPrintHex(pPerPoint->data, slLocate(24, 11));
	if(pPerPoint->id == PER_ID_StnShooting) {
		slPrint("X =", slLocate(17, 12));
		slPrint("Y =", slLocate(17, 13));
		slPrintHex(slHex2Dec(pPerPoint->x), slLocate(24, 12));
		slPrintHex(slHex2Dec(pPerPoint->y), slLocate(24, 13));
	}
}

void ss_main(void)
{
	slInitSystem(TV_352x240, NULL, 2);
	slBack1ColSet(BACK_COL_ADR, CD_Magenta);

	gunCtrlInit();

	while(TRUE) {
		gunCtrlPortMode();
		getGunPointData();

		slSynch();
	}
}

