/*------------------------------------------------------------------------
 *  FILE:	sblsgl02.c
 *
 *  PURPOSE:
 *		ＳＧＬモードのテストサンプル
 * 
 *------------------------------------------------------------------------
 */

#include    "sgl.h" 					/* use SGL  */
#include	"ss_akira.h"

#include	<machine.h>
#include	<sega_xpt.h>
#include	<sega_def.h> 
#include	<sega_mth.h> 
#include	<sega_scl.h> 
#include	<sega_dbg.h>
#include	"botan.h"
#include	"../../v_blank/v_blank.h"


extern MOTION walk[];

extern PDATA AKI_ASHI_L;
extern PDATA AKI_ASHI_R;
extern PDATA AKI_HIZA_L;
extern PDATA AKI_HIZA_R;
extern PDATA AKI_MOMO_L;
extern PDATA AKI_MOMO_R;
extern PDATA AKI_HARA;
extern PDATA AKI_KOSHI;
extern PDATA AKI_MUNE;
extern PDATA AKI_KAO;
extern PDATA AKI_KUBI;
extern PDATA AKI_ATAMA;
extern PDATA AKI_KATA_L;
extern PDATA AKI_KATA_R;
extern PDATA AKI_TE_L;
extern PDATA AKI_YUBI_L;
extern PDATA AKI_OYAYUBI_L;
extern PDATA AKI_TE_R;
extern PDATA AKI_YUBI_R;
extern PDATA AKI_OYAYUBI_R;
extern PDATA AKI_UDE_L;
extern PDATA AKI_UDE_R;


void SetAutoLinePara(Uint16 SclNum);
void ClrLinePara(Uint16 SclNum);

/* extern SetVblank(void);	*/			/*	追加 95.7.11 Chikahiro Yoshida	*/
extern FirstData(Uint32 sclnum);	/*	追加 95.7.11 Chikahiro Yoshida	*/
extern SecondData(Uint32 sclnum);	/*	追加 95.7.11 Chikahiro Yoshida	*/

/********************************
 * ｻｲｸﾙﾊﾟﾀｰﾝﾃｰﾌﾞﾙ               *
 ********************************/
Uint16	CycleTb[]={
	0x01ff,0x4455,	/* VRAM A (VRAM A0)   1:NBG1ﾊﾟﾀｰﾝﾈｰﾑ(ﾏｯﾌﾟ)ﾘｰﾄﾞ */
			/*                    0:NBG0ﾊﾟﾀｰﾝﾈｰﾑ(ﾏｯﾌﾟ)ﾘｰﾄﾞ */
			/*                    5:NBG1ｷｬﾗｸﾀﾊﾟﾀｰﾝ(ｾﾙ)ﾘｰﾄﾞ */
			/*                    4:NBG0ｷｬﾗｸﾀﾊﾟﾀｰﾝ(ｾﾙ)ﾘｰﾄﾞ */
			/*                    f:ｱｸｾｽしない             */
	0xffff,0xffff,  /*        (VRAM A1)   無効（分割していない）   */
	0xffff,0xffff,  /* VRAM B (VRAM B0)   未使用                   */
	0xffff,0xffff   /*        (VRAM B1)   無効（分割していない）   */
};


/* メインルーチン */
void main()
{
    SclConfig	scfg;
	Uint32	sclnum =SCL_NBG1;
    Uint32	sclnum2=SCL_NBG0;
    Uint32	StartAddr2,Sflag;
    Uint8	Key;
    Uint8	sw;
    Uint16	dummy_pad=0;

	Sint16 i;
	Sint16 mcnt, bcnt;
	MOTION *motptr;
	static MATRIX mtrx;
	static FIXED light[XYZ];
	static 	FIXED cm_pos[XYZ] =
		{toFIXED(0.0), toFIXED(-200.0), toFIXED(-1500.0) };
	static 	FIXED cm_target[XYZ] =
		{toFIXED(0.0), toFIXED(0.0), toFIXED(0.0)};
	static 	ANGLE cm_ang[XYZ] =
		{DEGtoANG(0.0), DEGtoANG(0.0), DEGtoANG(0.0) };

	static FIXED bone_leng[] = {
		toFIXEDa(0.38  ) ,
		toFIXEDa(0.21  ) ,
		toFIXEDa(0.2627) ,
		toFIXEDa(0.2124) ,
		toFIXEDa(0.2627) ,
		toFIXEDa(0.2124) ,
		toFIXEDa(0.22  ) ,
		toFIXEDa(0.3905) ,
		toFIXEDa(0.441 ) ,
		toFIXEDa(0.3905) ,
		toFIXEDa(0.441 )
	};

	static FIXED root_pos[][XYZ] = {
		{ toFIXED ( 0.0 ), toFIXED ( 0.0 ), toFIXED ( 0.0 ) } ,
		{ toFIXED ( 0.0 ), toFIXEDa(-0.01), toFIXED ( 0.0 ) } ,
		{ toFIXEDa( 0.02), toFIXED ( 0.0 ), toFIXED ( 0.0 ) } ,
		{ toFIXEDa(-0.09), toFIXED ( 0.0 ), toFIXEDa( 0.24) } ,
		{ toFIXEDa(-0.09), toFIXED ( 0.0 ), toFIXEDa(-0.24) } ,
		{ toFIXED ( 0.0 ), toFIXEDa(-0.01), toFIXED ( 0.0 ) } ,
		{ toFIXEDa(-0.04), toFIXED ( 0.0 ), toFIXEDa( 0.09) } ,
		{ toFIXEDa(-0.04), toFIXED ( 0.0 ), toFIXEDa(-0.09) } 
	};

	static FIXED aki_pos[XYZ] =
		{ toFIXED(0.0), toFIXED(-100.0), toFIXED(500.0) };
	static ANGLE aki_ang[XYZ] =
		{ DEGtoANG(0.0), DEGtoANG(90.0)  , DEGtoANG(0.0) };
	static ANGLE wlk_ang[XYZ] =
		{ DEGtoANG(0.0), DEGtoANG(0.0)	, DEGtoANG(0.0) };

	slInitSystem(TV_320x224,NULL,1);

	/* Ｖブランクルーチンをユーザー登録する場合	*/
    SetVblank();

    SCL_SetColRamMode(SCL_CRM24_1024);
	slSynch();

   /*******************************************
    *	スクロールデータのセット              *
    *******************************************/
    FirstData(sclnum);
    StartAddr2 = SecondData(sclnum2);

   /*******************************************
    *	ｽｸﾛｰﾙｺﾝﾌｨｸﾞﾚｰｼｮﾝの設定                *
    *******************************************/
    SCL_InitConfigTb(&scfg);
    scfg.dispenbl      = ON;
    scfg.charsize      = SCL_CHAR_SIZE_1X1;
    scfg.pnamesize     = SCL_PN2WORD;
    scfg.platesize     = SCL_PL_SIZE_1X1;
    scfg.coltype       = SCL_COL_TYPE_256;
    scfg.datatype      = SCL_CELL;
    scfg.plate_addr[0] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[1] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[2] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[3] = SCL_VDP2_VRAM_A1;
    SCL_SetConfig(sclnum, &scfg);

    scfg.plate_addr[0] = StartAddr2;
    scfg.plate_addr[1] = StartAddr2;
    scfg.plate_addr[2] = StartAddr2;
    scfg.plate_addr[3] = StartAddr2;
    SCL_SetConfig(sclnum2, &scfg);

   /*******************************************
    *	サイクルパターンの設定                *
    *******************************************/
    SCL_SetCycleTable(CycleTb);

   /*******************************************
    *	ﾌﾟﾗｲｵﾘﾃｨを設定(0～7)                  *
    *******************************************/
    SCL_SetPriority(sclnum2,7);
    SCL_SetPriority(sclnum,6);

    SetAutoLinePara(sclnum);

    SCL_Open(sclnum2);
	SCL_MoveTo(FIXED(0), FIXED(50),0);/* Home Position */
	SCL_Scale(FIXED(1.0), FIXED(1.0));
    SCL_Close();

    SCL_Open(sclnum);
	SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
	SCL_Scale(FIXED(1.0), FIXED(1.0));
    SCL_Close();
/*    SCL_DisplayFrame();	*/

    Key=1;
    sw=1;
    Sflag = 0;

	i = 0;

	slPushUnitMatrix();
	{
		slRotY(DEGtoANG(20.0));
		slRotX(DEGtoANG(20.0));
		slRotZ(DEGtoANG(20.0));
		slCalcPoint(toFIXED(0.0), toFIXED(0.0), toFIXED(1.0), light);
	}
	slPopMatrix();
	slLight(light);


    while(-1) {

		/* SGL Sprite Controll	*/
		mcnt = bcnt = 0;
		
		slUnitMatrix(CURRENT);
		slLookAt(cm_pos, cm_target, cm_ang[Z] );

		slPushUnitMatrix();
		{
			slTranslate(aki_pos[X], aki_pos[Y], aki_pos[Z]);
			slRotY(wlk_ang[Y]);
			slCalcPoint(toFIXED(6.0), toFIXED(0.0), toFIXED(0.0), aki_pos);
		}
		slPopMatrix();

		motptr = &walk[i];
		slPushMatrix();
		{
			slTranslate(aki_pos[X], aki_pos[Y]+motptr->HARApos[Y], aki_pos[Z]);
			slRotZ(motptr->HARAang[Z] + aki_ang[X]);
			slRotY(motptr->HARAang[Y] + aki_ang[Y]);
			slRotX(motptr->HARAang[X] + aki_ang[Z]);
			slPutPolygon(&AKI_HARA);

			slPushMatrix();
			{
				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->MUNEang[Z]);
				slRotY(motptr->MUNEang[Y]);
				slRotX(motptr->MUNEang[X]);
				slPutPolygon(&AKI_MUNE);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slPushMatrix();
				{
					mcnt += 1;
					slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
					slRotZ(motptr->KAOang[Z]);
					slRotY(motptr->KAOang[Y]);
					slRotX(motptr->KAOang[X]);
					slPutPolygon(&AKI_ATAMA);
					slPutPolygon(&AKI_KUBI);
					slPutPolygon(&AKI_KAO);
					slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));
				}
				slPopMatrix();

				slPushMatrix();
				{
					mcnt += 1;
					slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
					slRotZ(motptr->R_KATAang[Z]);
					slRotY(motptr->R_KATAang[Y]);
					slRotX(motptr->R_KATAang[X]);
					slPutPolygon(&AKI_KATA_R);
					slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

					slRotZ(motptr->R_UDEang);
					slPutPolygon(&AKI_UDE_R);
					slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

					slRotZ(motptr->R_TEang[Z]);
					slRotY(motptr->R_TEang[Y]);
					slRotX(motptr->R_TEang[X]);
					slPutPolygon(&AKI_TE_R);
					slPutPolygon(&AKI_YUBI_R);
					slPutPolygon(&AKI_OYAYUBI_R);
				}
				slPopMatrix();

				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->L_KATAang[Z]);
				slRotY(motptr->L_KATAang[Y]);
				slRotX(motptr->L_KATAang[X]);
				slPutPolygon(&AKI_KATA_L);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->L_UDEang);
				slPutPolygon(&AKI_UDE_L);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->L_TEang[Z]);
				slRotY(motptr->L_TEang[Y]);
				slRotX(motptr->L_TEang[X]);
				slPutPolygon(&AKI_TE_L);
				slPutPolygon(&AKI_YUBI_L);
				slPutPolygon(&AKI_OYAYUBI_L);
			}
			slPopMatrix();

			mcnt += 1;
			slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
			slRotZ(motptr->KOSHIang[Z]);
			slRotY(motptr->KOSHIang[Y]);
			slRotX(motptr->KOSHIang[X]);
			slPutPolygon(&AKI_KOSHI);
			slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

			slPushMatrix();
			{
				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->R_MOMOang[Z]);
				slRotY(motptr->R_MOMOang[Y]);
				slRotX(motptr->R_MOMOang[X]);
				slPutPolygon(&AKI_MOMO_R);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->R_HIZAang);
				slPutPolygon(&AKI_HIZA_R);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slGetMatrix(mtrx);
				mtrx[0][0] = mtrx[1][1] = mtrx[2][2] = toFIXED(1.0);
				mtrx[0][1] = mtrx[0][2] = toFIXED(0.0);
				mtrx[1][0] = mtrx[1][2] = toFIXED(0.0);
				mtrx[2][0] = mtrx[2][1] = toFIXED(0.0);
				slLoadMatrix(mtrx);
				slRotZ(aki_ang[Z]);
				slRotY(aki_ang[Y]);
				slRotX(aki_ang[X]);
				slRotZ(motptr->R_ASHIang[Z]);
				slRotY(motptr->R_ASHIang[Y]);
				slRotX(motptr->R_ASHIang[X]);
				slPutPolygon(&AKI_ASHI_R);
			}
			slPopMatrix();

			mcnt += 1;
			slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
			slRotZ(motptr->L_MOMOang[Z]);
			slRotY(motptr->L_MOMOang[Y]);
			slRotX(motptr->L_MOMOang[X]);
			slPutPolygon(&AKI_MOMO_L);
			slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

			slRotZ(motptr->L_HIZAang);
			slPutPolygon(&AKI_HIZA_L);
			slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

			slGetMatrix(mtrx);
			mtrx[0][0] = mtrx[1][1] = mtrx[2][2] = toFIXED(1.0);
			mtrx[0][1] = mtrx[0][2] = toFIXED(0.0);
			mtrx[1][0] = mtrx[1][2] = toFIXED(0.0);
			mtrx[2][0] = mtrx[2][1] = toFIXED(0.0);
			slLoadMatrix(mtrx);
			slRotZ(aki_ang[Z]);
			slRotY(aki_ang[Y]);
			slRotX(aki_ang[X]);
			slRotZ(motptr->L_ASHIang[Z]);
			slRotY(motptr->L_ASHIang[Y]);
			slRotX(motptr->L_ASHIang[X]);
			slPutPolygon(&AKI_ASHI_L);
		}
		slPopMatrix();



	/* SBL Scroll Controll	*/
	switch(Key) {
	    case UP_LEFT:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), FIXED(2),0);
		SCL_Close();
		if(sw==0) Key = 8;
		break;
	    case UP_RIGHT:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), FIXED(2), 0);
		SCL_Close();
		if(sw==0) Key = 1;
		break;
	    case UP_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(0, FIXED(2), 0);
		SCL_Close();
		if(sw==0) Key = 9;
		break;
	    case DOWN_LEFT:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), -FIXED(2), 0);
		SCL_Close();
		if(sw==0) Key = 2;
		break;
	    case DOWN_RIGHT:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), -FIXED(2), 0);
		SCL_Close();
		if(sw==0) Key = 4;
		break;
	    case DOWN_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(0, -FIXED(2), 0);
		SCL_Close();
		if(sw==0) Key = 3;
		break;
	    case LEFT_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), 0, 0);
		SCL_Close();
		if(sw==0) Key = 6;
		break;
	    case RIGHT_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), 0, 0);
		SCL_Close();
		if(sw==0) Key = 7;
		break;
	}
	sw++;

		slSynch();

		if(++i >= WALK_CNT) i = 0;

		aki_ang[Y] += DEGtoANG(0.5);
		wlk_ang[Y] += DEGtoANG(0.5);

    }	/* while	*/

}


/********************************************************************
 * ラインパラメータテーブルのダミーを作成する                       *
 ********************************************************************/
#define X_SIZE		320
#define Y_SIZE		224
#define Y_SIZE_H	112

void SetAutoLinePara(Uint16 SclNum)
{
    Fixed32 count;
    SclLineparam lp;

    SCL_InitLineParamTb(&lp);

    lp.delta_enbl=ON;
    lp.v_enbl=ON;
    lp.h_enbl=ON;
    lp.line_addr=SCL_VDP2_VRAM_B0;
    lp.interval=0;

    for(count=0;count<Y_SIZE;count++)
    {
    	lp.line_tbl[count].h = FIXED(count)/2;
	lp.line_tbl[count].v = FIXED(count)*2;
	lp.line_tbl[count].dh
	= DIV_FIXED((FIXED(X_SIZE)-(2* lp.line_tbl[count].h)),FIXED(X_SIZE));
    }

    SCL_Open(SclNum);
	SCL_SetLineParam(&lp);
    SCL_Close();
	slSynch();
	/* SCL_DisplayFrame();	*/
}

/********************************************************************
 * ラインパラメータテーブルを無効にする                             *
 ********************************************************************/
void ClrLinePara(Uint16 SclNum)
{
    SclLineparam lp;

    SCL_InitLineParamTb(&lp);

    SCL_Open(SclNum);
	SCL_SetLineParam(&lp);
    SCL_Close();
	slSynch();
	/* SCL_DisplayFrame();	*/
}
