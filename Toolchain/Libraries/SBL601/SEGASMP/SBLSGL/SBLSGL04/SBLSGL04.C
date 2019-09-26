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

#define		RBG0RB_CEL_ADR			(VDP2_VRAM_A0            )
#define		RBG0RB_MAP_ADR			(VDP2_VRAM_B0            )
#define		RBG0RB_COL_ADR			(VDP2_COLRAM    + 0x00200)
#define		RBG0RA_CEL_ADR			(RBG0RB_CEL_ADR + 0x06e80)
#define		RBG0RA_MAP_ADR			(RBG0RB_MAP_ADR + 0x02000)
#define		RBG0RA_COL_ADR			(RBG0RB_COL_ADR + 0x00200)
#define		RBG0_KTB_ADR			(VDP2_VRAM_A1            )
#define		RBG0_PRA_ADR			(VDP2_VRAM_A1   + 0x1fe00)
#define		RBG0_PRB_ADR			(RBG0_PRA_ADR   + 0x00080)
#define		BACK_COL_ADR			(VDP2_VRAM_A1   + 0x1fffe)

#define		MAP_NUM					16

/*************** 関数プロトタイプ *******************************/

void	ClrVramAll(void);

/*************** 外部参照 ***************************************/

extern	Uint8	tuti_cel[];
extern	Uint16	tuti_map[],tuti_pal[];
extern	Uint8	sora_cel[];
extern	Uint16	sora_map[],sora_pal[];

extern	void Cel2VRAM( Uint8 *Cel_Data , void *Cell_Adr , Uint32 suu );
extern	void Map2VRAM( Uint16 *Map_Data , void *Map_Adr , Uint16 suuj , Uint16 suui , Uint16 palnum ,Uint32 mapoff);
extern	void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu );

/*************** *      * ***************************************/


	SclConfig	Rbg0Scfg;

/*************** 以下 プログラム本体 ****************************/

void main()	{

	SclVramConfig	tp;

	Uint16		BackCol;


	FIXED	posy = toFIXED(0.0);
	ANGLE	angz = DEGtoANG(0.0);
	ANGLE	angz_up = DEGtoANG(0.0);


	SCL_SglOn();
						/*	各種初期化処理				*/
	slInitSystem(TV_320x224,NULL,1);

	SetVblank();

	SCL_SetColRamMode(SCL_CRM15_1024);
	slTVOff();

	ClrVramAll();

						/*	バック画面の色を黒に設定	*/
	BackCol = RGB16_COLOR(0,0,0) & 0x7fff;
	SCL_SetBack(BACK_COL_ADR,1,&BackCol);

						/*	回転ﾊﾟﾗﾒｰﾀﾃｰﾌﾞﾙをVRAM中のどこに置くかを指定	*/
	SCL_InitRotateTable(RBG0_PRA_ADR,(Uint16)2 ,SCL_RBG0,SCL_RBG0);
	slRparaInitSet( (void *)RBG0_PRA_ADR );

						/*	ＶＲＡＭの使用方法の設定	*/
	SCL_InitVramConfigTb(&tp);
		tp.ktboffsetA		= 0x0000;
		tp.ktboffsetB		= 0x0000;
		tp.vramModeA		= ON;
		tp.vramModeB		= ON;
		tp.vramA0			= SCL_RBG0_CHAR;
		tp.vramA1			= SCL_RBG0_K;
		tp.vramB0			= SCL_RBG0_PN;
		tp.vramB1			= SCL_NON;
		tp.colram			= SCL_NON;
	SCL_SetVramConfig(&tp);

						/*	回転面0の設定				*/
	SCL_InitConfigTb(&Rbg0Scfg);
		Rbg0Scfg.dispenbl      = ON;
		Rbg0Scfg.charsize      = SCL_CHAR_SIZE_1X1;
		Rbg0Scfg.pnamesize     = SCL_PN1WORD;
		Rbg0Scfg.flip          = SCL_PN_12BIT;
		Rbg0Scfg.platesize     = SCL_PL_SIZE_1X1;
		Rbg0Scfg.coltype       = SCL_COL_TYPE_256;
		Rbg0Scfg.datatype      = SCL_CELL;
		Rbg0Scfg.mapover       = SCL_OVER_0;
		Rbg0Scfg.plate_addr[0] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[1] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[2] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[3] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[4] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[5] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[6] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[7] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[8] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[9] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[10] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[11] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[12] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[13] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[14] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[15] = RBG0RA_MAP_ADR;
		Rbg0Scfg.plate_addr[16] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[17] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[18] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[19] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[20] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[21] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[22] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[23] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[24] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[25] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[26] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[27] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[28] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[29] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[30] = RBG0RB_MAP_ADR;
		Rbg0Scfg.plate_addr[31] = RBG0RB_MAP_ADR;
	SCL_SetConfig(SCL_RBG0, &Rbg0Scfg);

	slMakeKtable((void *)RBG0_KTB_ADR);
	slKtableRA((void *)RBG0_KTB_ADR , K_FIX | K_DOT | K_2WORD | K_ON);
	slKtableRB((void *)RBG0_KTB_ADR , K_FIX | K_DOT | K_2WORD | K_ON);

						/*	プライオリティの設定		*/
	SCL_SetPriority(SCL_RBG0,4);

						/*	地面の土のデータ転送	*/
	Cel2VRAM(tuti_cel,(void *)RBG0RA_CEL_ADR,65536);
	Map2VRAM(tuti_map,(void *)RBG0RA_MAP_ADR,64,64,2,884);
	Pal2CRAM(tuti_pal,(void *)RBG0RA_COL_ADR,160);

						/*	空のデータ転送	*/
	Cel2VRAM(sora_cel,(void *)RBG0RB_CEL_ADR,28288);
	Map2VRAM(sora_map,(void *)RBG0RB_MAP_ADR,64,20,1,0);
	Pal2CRAM(sora_pal,(void *)RBG0RB_COL_ADR,256);

	slTVOn();

	slRparaMode(K_CHANGE);
	slScrAutoDisp(RBG0ON);




						/*	main loop  画面操作		*/
	while(1){
		slCurRpara(RA);
		slUnitMatrix(CURRENT);
		slTranslate(toFIXED(0.0) , toFIXED(0.0) + posy , toFIXED(100.0));
		posy -= toFIXED(5.0);
		slRotX(DEGtoANG(-90.0));
		slRotZ(angz);
		slScrMatSet();

		slCurRpara(RB);
		slUnitMatrix(CURRENT);
		slTranslate(toFIXED(160.0) , toFIXED(155.0) , toFIXED(100.0));
		slRotZ(angz);
		slScrMatSet();

		angz_up += DEGtoANG(0.5);
		angz = (slSin(angz_up) >> 4);

		slSynch();
	}

}


void	ClrVramAll(void)	{

	Uint32	*vr = (Uint32 *)0x25e00000;
	Uint32	i;

	for(i=0;i<20000;i++)	{
		vr[i]=0x00000000;
	}
}
