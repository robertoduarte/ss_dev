/*------------------------------------------------------------------------
  取り敢えず自分で作ってみた回転スクロールのサンプル

  1995.9.7 c.yoshida
------------------------------------------------------------------------*/

/************** include file ************************************/

#include	<stdio.h>
#include	<machine.h>
#include	<sega_scl.h> 
#include	"..\..\v_blank\v_blank.h"

#include	<sgl.h>
#include	<sgl_work.h>		/* use SGL */

/************** define ******************************************/

#define		X_SIZE			512
#define		Y_SIZE			256
#define		MAP_OFFSET		0x0800
#define		MAP_NUM			16

/*************** 関数プロトタイプ *******************************/

void	ClrVram(Uint8 *buff);

/*************** 外部参照 ***************************************/

extern	Uint32	BackPalette[];		/* 鉄板風タイルのパレット   */
extern	Uint8	BackCharPatData[];	/* 鉄板風タイルのデータ本体 */

	Uint16		PadData1EW,PadData2EW;
	SclConfig	Rbg0Scfg;

/*************** 以下 プログラム本体 ****************************/

int	main( void ){
	SclVramConfig	tp;
	Uint32		i,j;

	Uint16		*wp,BackCol;

	ANGLE	x = DEGtoANG(0.0);
	ANGLE	y = DEGtoANG(0.0);
	ANGLE	z = DEGtoANG(0.0);


	SCL_SglOn();
						/*	各種初期化処理				*/
	slInitSystem(TV_320x224,NULL,1);

	SetVblank();

	SCL_SetColRamMode(SCL_CRM24_1024);

						/*	バック画面の色を黒に設定	*/
	BackCol = RGB16_COLOR(0,0,0) & 0x7fff;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

						/*	回転ﾊﾟﾗﾒｰﾀﾃｰﾌﾞﾙをVRAM中のどこに置くかを指定	*/
	SCL_InitRotateTable(SCL_VDP2_VRAM_A1+0x1fe00,2,SCL_RBG0,SCL_RBG0);
	slRparaInitSet(((void *)(SCL_VDP2_VRAM_A1+0x1fe00)));

						/*	ＶＲＡＭの使用方法の設定	*/
	SCL_InitVramConfigTb(&tp);
		tp.ktboffsetA	= 0x0000;
		tp.ktboffsetB	= 0x0000;
		tp.vramModeA	= ON;
		tp.vramModeB	= ON;
		tp.vramA0		= SCL_NON;
		tp.vramA1		= SCL_RBG0_K;
		tp.vramB0		= SCL_RBG0_CHAR;
		tp.vramB1		= SCL_RBG0_PN;
		tp.colram		= SCL_NON;
	SCL_SetVramConfig(&tp);

						/*	回転面0の設定				*/
	SCL_InitConfigTb(&Rbg0Scfg);
	    Rbg0Scfg.dispenbl      = ON;
	    Rbg0Scfg.charsize      = SCL_CHAR_SIZE_2X2;
	    Rbg0Scfg.pnamesize     = SCL_PN1WORD;
	    Rbg0Scfg.flip          = SCL_PN_12BIT;
	    Rbg0Scfg.platesize     = SCL_PL_SIZE_1X1;
	    Rbg0Scfg.coltype       = SCL_COL_TYPE_256;
	    Rbg0Scfg.datatype      = SCL_CELL;
	    Rbg0Scfg.mapover       = SCL_OVER_0;
	    for(i=0;i<MAP_NUM;i++)
			Rbg0Scfg.plate_addr[i] = SCL_VDP2_VRAM_B1;
	    for(i=0;i<MAP_NUM;i++)
			Rbg0Scfg.plate_addr[16+i] = SCL_VDP2_VRAM_B1;
	SCL_SetConfig(SCL_RBG0, &Rbg0Scfg);


	slMakeKtable((void *)SCL_VDP2_VRAM_A1);
	slKtableRA((void *)SCL_VDP2_VRAM_A1 , K_FIX | K_DOT | K_2WORD | K_ON);
	slKtableRB((void *)SCL_VDP2_VRAM_A1 , K_FIX | K_DOT | K_2WORD | K_ON);

						/*	プライオリティの設定		*/
	SCL_SetPriority(SCL_RBG0,4);

						/*	背景のタイルのカラー転送	*/
	SCL_AllocColRam(SCL_RBG0,256,ON);
	SCL_SetColRam(SCL_RBG0,1,256,BackPalette);

						/*	背景タイルのCG転送			*/
	memcpy((void *)SCL_VDP2_VRAM_B0,(void *)BackCharPatData,1024);

						/*	背景タイルのPN dataを転送	*/
	for(i=0;i<16;i++)	for(j=0;j<16;j++) {
		wp  = (Uint16 *)(SCL_VDP2_VRAM_B1 + i*2*64 + j*2*2);
		*wp = MAP_OFFSET+0;
		wp++;
		*wp = MAP_OFFSET+2;
		wp += 31;
		*wp = MAP_OFFSET+4;
		wp++;
		*wp = MAP_OFFSET+6;
	}

/*	slDispCenterR( 320/2, 224/2 );	*/
	slRparaMode(K_CHANGE);
	slScrAutoDisp(RBG0ON);

						/*	main loop  padをつかった画面操作	*/
	while(1){
		PadData1EW=PadData1E;
		PadData1E=0;
		PadData2EW=PadData2E;
		PadData2E=0;
		
		if(PadData1 || PadData1EW)	{
			if( PadData1 & PAD_X )	{
				x=x+0x0080;
			}
			if( PadData1 & PAD_Y )	{
				y=y+0x0080;
			}
			if((PadData1 & PAD_Z))	{
				z=z+0x0080;
			}
			if((PadData1 & PAD_S)) {
				x=(DEGtoANG(0.0));
				y=(DEGtoANG(0.0));
				z=(DEGtoANG(0.0));
			}
		}
		
		slCurRpara(RA);
		slUnitMatrix(CURRENT);
		slRotX(x);
		slRotY(y);
		slRotZ(z);
		slTranslate(toFIXED(0.0) , toFIXED(0.0), toFIXED(170.0));
		slScrMatSet();
		
		slCurRpara(RB);
		slUnitMatrix(CURRENT);
		slTranslate(toFIXED(0.0) , toFIXED(0.0) , toFIXED(170.0));
		slScrMatSet();
		
		slSynch();
	}
	
	return	0;
}


void	ClrVram(Uint8 *buff)	{
	Uint32	i;
	for(i=0;i<(X_SIZE * Y_SIZE);i++)	buff[i]=0x00;
}
