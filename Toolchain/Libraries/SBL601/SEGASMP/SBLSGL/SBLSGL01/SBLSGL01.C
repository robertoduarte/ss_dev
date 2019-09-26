/*------------------------------------------------------------------------
 *  FILE:	sblsgl01.c
 *
 *  PURPOSE:
 *		ＳＧＬモードのテストサンプル
 * 
 *------------------------------------------------------------------------
 */
#include    <sgl.h> 				/* use SGL  */
extern PDATA PD_PLANE1, PD_PLANE2, PD_PLANE3;

#include	<machine.h>
#include	<sega_scl.h> 

#include	"..\..\v_blank\v_blank.h"

#include	"..\graphic\cosmo2u.cha"
#include	"..\graphic\cosmo2u.col"
#include	"..\graphic\cosmo2u.map"

/*----------------------------
SCROLL NBG0 Cycle Table
Pattern Name Table location:B0
Character location: B1
Color Mode: C256
Zoom Mode: 1
----------------------------*/
Uint16	CycleTb[]={
	0xffff,0xffff,
	0xffff,0xffff,
	0x0f44,0xffff,
	0xffff,0xffff
};

static	void	set_poly(ANGLE ang[XYZ],FIXED pos[XYZ])
{
	slTranslate(pos[X],pos[Y],pos[Z]);
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
}


/* メインルーチン */
void main()
{
	SclConfig	scfg;
	SclRgb		start,end;
	Uint16  	BackCol,i,PadData1EW;
	Uint8   	*VramWorkP;

	static ANGLE ang1[XYZ], ang2[XYZ], ang3[XYZ];
	static FIXED pos1[XYZ], pos2[XYZ], pos3[XYZ];
	static ANGLE tang, aang;

	slInitSystem(TV_320x224,NULL,1);

	/* Ｖブランクルーチンをユーザー登録する場合	*/
    SetVblank();

	SCL_SetColRamMode(SCL_CRM24_1024);

	BackCol = 0x0000;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	/******************************************************
	*	スクロールのｷｬﾗｸﾀﾊﾟﾀｰﾝの設定(VRAM B1に置く)   *
	******************************************************/
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1;
	memcpy(VramWorkP, cosmo2u_char, sizeof(cosmo2u_char));

	/*******************************************
	*	スクロールのﾊﾟﾀｰﾝﾈｰﾑﾃｰﾌﾞﾙの設定        *
	*******************************************/
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0;
	for(i=0;i<28;i++) {
		memcpy(VramWorkP, &cosmo2u_map[i*40],40*2);
		VramWorkP += 64*2;
	}

	/*******************************************
	*	パレットデータの設定                   *
	*******************************************/
	SCL_AllocColRam(SCL_NBG0,256,OFF);
	SCL_SetColRam(SCL_NBG0,0,sizeof(cosmo2u_col),(void*)cosmo2u_col);

	/*******************************************
	*	Scroll Configure Set               	   *
	*******************************************/
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_1X1;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_12BIT;
		scfg.platesize     = SCL_PL_SIZE_2X2;
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x000c;	/* VRAM B1 のオフセット */
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);

	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0);	/* Home Position */
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();

	SCL_SetCycleTable(CycleTb);

	/* プライオリティのセット */
	SCL_SetPriority(SCL_NBG0,7);

	start.red = start.green = start.blue = 0;
	end.red = end.green = end.blue = - 255;

	ang1[X] = ang1[Y] = ang1[Z] = DEGtoANG(0.0);
	ang2[X] = ang2[Y] = ang2[Z] = DEGtoANG(0.0);
	ang3[X] = ang3[Y] = ang3[Z] = DEGtoANG(0.0);
	pos1[X] = toFIXED(  0.0);
	pos1[Y] = toFIXED( 40.0);
	pos1[Z] = toFIXED(170.0);
	pos2[X] = toFIXED(  0.0);
	pos2[Y] = toFIXED(-40.0);
	pos2[Z] = toFIXED(  0.0);
	pos3[X] = toFIXED(  0.0);
	pos3[Y] = toFIXED(-40.0);
	pos3[Z] = toFIXED(  0.0);
	tang = DEGtoANG(0.0);
	aang = DEGtoANG(2.0);


	/******** メインループ ********/
	while(-1) {
		PadData1EW = PadData1E;
		PadData1E = 0;
		if(PadData1EW & PAD_U){
			SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,start.red,start.green,start.blue);
			SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&start,&end);
		}
		if(PadData1EW & PAD_D){
			SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,end.red,end.green,end.blue);
			SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&end,&start);
		}
		if((PadData1 & PAD_S)) 	SCL_SetMosaic(SCL_NBG0, 0, 0);
		if((PadData1 & PAD_LB))	SCL_SetMosaic(SCL_NBG0, 2, 2);
		if((PadData1 & PAD_RB))	SCL_SetMosaic(SCL_NBG0, 4, 4);
		if((PadData1 & PAD_X)) 	SCL_SetMosaic(SCL_NBG0, 6, 6);
		if((PadData1 & PAD_Y)) 	SCL_SetMosaic(SCL_NBG0, 8, 8);
		if((PadData1 & PAD_Z)) 	SCL_SetMosaic(SCL_NBG0,10,10);
		if((PadData1 & PAD_A)) 	SCL_SetMosaic(SCL_NBG0,12,12);
		if((PadData1 & PAD_B)) 	SCL_SetMosaic(SCL_NBG0,14,14);
		if((PadData1 & PAD_C)) 	SCL_SetMosaic(SCL_NBG0,16,16);

		slUnitMatrix(CURRENT);
		ang1[Z] = ang2[Z] = tang;
		tang += aang;
		if(tang < DEGtoANG(-90.0)) {
			aang = DEGtoANG(2.0);
		} else if(tang > DEGtoANG(90.0)) {
			aang = -DEGtoANG(2.0);
		}

		slPushMatrix();
		{
			set_poly(ang1, pos1);
			slPutPolygon(&PD_PLANE1);

			slPushMatrix();
			{
				set_poly(ang2, pos2);
				slPutPolygon(&PD_PLANE2);

				slPushMatrix();
				{
					set_poly(ang3, pos3);
					ang3[Y] += DEGtoANG(5.0);
					slPutPolygon(&PD_PLANE3);
				}
				slPopMatrix();
			}	
			slPopMatrix();
		}
		slPopMatrix();

		slSynch();
	}
}



