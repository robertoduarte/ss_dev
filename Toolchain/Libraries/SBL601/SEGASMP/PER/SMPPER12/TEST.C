/************************************************************************
*	取り敢えずバーチャガンのサンプルをつくる							*
*	1995.10.11	c.yoshida												*
************************************************************************/

#include	<machine.h>
#include	<sega_scl.h> 

#include	"..\..\v_blank\v_blank.h"
#include	"..\..\scl\graphic\cosmo2u.cha"
#include	"..\..\scl\graphic\cosmo2u.col"
#include	"..\..\scl\graphic\cosmo2u.map"

/*********** 関数プロトタイプ *************/
void scl_main(void);


/******************************************/

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


void scl_main()	{
	
	SclConfig	scfg;
	SclRgb		start,end;
	Uint16  	BackCol,i;
	Uint8   	*VramWorkP;
	
	/*	バック画面の色を黒に設定           */
	BackCol = 0x0000;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);
	
	/*	スクロールのｷｬﾗｸﾀﾊﾟﾀｰﾝの設定(VRAM B1に置く)   */
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1;
	memcpy(VramWorkP, cosmo2u_char, sizeof(cosmo2u_char));
	
	/*	スクロールのﾊﾟﾀｰﾝﾈｰﾑﾃｰﾌﾞﾙの設定    */
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0;
	for(i=0;i<28;i++) {
		memcpy(VramWorkP, &cosmo2u_map[i*40],40*2);
		VramWorkP += 64*2;
	}
	
	/*	パレットデータの設定               */
	SCL_AllocColRam(SCL_NBG0,256,OFF);
	SCL_SetColRam(SCL_NBG0,0,sizeof(cosmo2u_col),(void *)cosmo2u_col);
	
	/*	Scroll Configuration Set               */
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_1X1;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_12BIT;
		scfg.platesize     = SCL_PL_SIZE_2X2;
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x000c;/* VRAM B1 のオフセット */
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);
	
	/*	VRAM Access Pattern Set            */
	SCL_SetCycleTable(CycleTb);
	
	SCL_SetPriority(SCL_NBG0,7);
	SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
					SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);

	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
	
	start.red = start.green = start.blue = 0;
	end.red = end.green = end.blue = - 255;
}
