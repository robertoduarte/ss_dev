/*----------------------------------------------------------------------*/
/*	Walking Akira							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"ss_akira.h"

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

void main()
{
	Sint16 i;
	Sint16 mcnt, bcnt;
	MOTION *motptr;
	static MATRIX mtrx;
	static FIXED light[XYZ];
	static FIXED cm_pos[XYZ] =
		{toFIXED(0.0), toFIXED(-200.0), toFIXED(-1500.0) };
	static FIXED cm_target[XYZ] =
		{toFIXED(0.0), toFIXED(0.0), toFIXED(0.0)};
	static ANGLE cm_ang[XYZ] =
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

/*	slPrint("demo C", slLocate(6,2));	*/

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

	while(-1){
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

		slSynch();

		if(++i >= WALK_CNT) i = 0;

		aki_ang[Y] += DEGtoANG(0.5);
		wlk_ang[Y] += DEGtoANG(0.5);
	}
}

