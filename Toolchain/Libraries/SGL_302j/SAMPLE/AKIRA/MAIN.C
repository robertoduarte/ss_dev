/*----------------------------------------------------------------------*/
/*	Walking Akira							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"ss_akira.h"

extern MOTION walk[];

extern XPDATA AKI_ASHI_L;
extern XPDATA AKI_ASHI_R;
extern XPDATA AKI_HIZA_L;
extern XPDATA AKI_HIZA_R;
extern XPDATA AKI_MOMO_L;
extern XPDATA AKI_MOMO_R;
extern XPDATA AKI_HARA;
extern XPDATA AKI_KOSHI;
extern XPDATA AKI_MUNE;
extern XPDATA AKI_KAO;
extern XPDATA AKI_KUBI;
extern XPDATA AKI_ATAMA;
extern XPDATA AKI_KATA_L;
extern XPDATA AKI_KATA_R;
extern XPDATA AKI_TE_L;
extern XPDATA AKI_YUBI_L;
extern XPDATA AKI_OYAYUBI_L;
extern XPDATA AKI_TE_R;
extern XPDATA AKI_YUBI_R;
extern XPDATA AKI_OYAYUBI_R;
extern XPDATA AKI_UDE_L;
extern XPDATA AKI_UDE_R;

#define	GRaddr	0xe000
static	GOURAUDTBL	gour[1024];
static	Uint8	vwork[1024];

#define	GRTBL(r,g,b)	(((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )

static	Uint16	GourTbl[32] = {
	GRTBL(  0,  0,  0 ),
	GRTBL(  1,  1,  1 ),
	GRTBL(  2,  2,  2 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 10, 10, 10 ),
	GRTBL( 11, 11, 11 ),
	GRTBL( 12, 12, 12 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 14, 14, 14 ),
	GRTBL( 15, 15, 15 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 17, 17, 17 ),
	GRTBL( 18, 18, 18 ),
	GRTBL( 19, 19, 19 ),
	GRTBL( 20, 20, 20 ),
	GRTBL( 21, 21, 21 ),
	GRTBL( 22, 22, 22 ),
	GRTBL( 23, 23, 23 ),
	GRTBL( 24, 24, 24 ),
	GRTBL( 25, 25, 25 ),
	GRTBL( 26, 26, 26 ),
	GRTBL( 27, 27, 27 ),
	GRTBL( 28, 28, 28 ),
	GRTBL( 29, 29, 29 ),
	GRTBL( 30, 30, 30 ),
	GRTBL( 31, 31, 31 ),
};

static	Uint16	GourTbl2[32] = {
	GRTBL(  0,  0,  0 ),
	GRTBL(  1,  1,  1 ),
	GRTBL(  2,  2,  2 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 10, 10, 10 ),
	GRTBL( 11, 11, 11 ),
	GRTBL( 12, 12, 12 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 14, 14, 14 ),
	GRTBL( 15, 15, 15 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 17, 17, 17 ),
	GRTBL( 19, 19, 19 ),
	GRTBL( 22, 22, 22 ),
	GRTBL( 26, 26, 26 ),
	GRTBL( 31, 31, 31 ),
};

static	Uint16	GourTbl3[32] = {
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  9,  9,  9 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 18, 18, 18 ),
	GRTBL( 24, 24, 24 ),
	GRTBL( 31, 31, 31 ),
};

void calcModelVector( XPDATA *model )
{
	int	i,j,p,s;
	FIXED	x,y,z,l,x2,y2,z2;
	FIXED	*vect;
	POLYGON	*poly;
	
	p = model->nbPoint;
	s = model->nbPolygon;
	vect = (FIXED*)model->vntbl;

	for( i=0; i<p; i++ ) {
		x=y=z=0;
		poly = model->pltbl;
		for( j=0; j<s; j++ ) {
			if( poly->Vertices[0] == i || poly->Vertices[1] == i ||
				poly->Vertices[2] == i || poly->Vertices[3] == i ) {
				x += poly->norm[0];
				y += poly->norm[1];
				z += poly->norm[2];
			}
			poly++;
		}
		x2 = slMulFX(x,x);
		y2 = slMulFX(y,y);
		z2 = slMulFX(z,z);
		l = slSquartFX(x2+y2+z2);
		*vect++ = slDivFX(l,x);
		*vect++ = slDivFX(l,y);
		*vect++ = slDivFX(l,z);
	}
}

void setgstb( XPDATA *model )
{
	Sint32	i,s;
	static  Uint16  g = 0;
	ATTR	*attr;
	
	s = model->nbPolygon;
	attr = model->attbl;
	

	for( i=0; i<s; i++ ) {
		attr->gstb = GRaddr+g;
		attr++;
		g++;
	}
}

void ss_main(void)
{
	Uint16 data;
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
		{ toFIXED(0.0), toFIXED(-200.0), toFIXED(350.0) };
	static ANGLE aki_ang[XYZ] =
		{ DEGtoANG(0.0), DEGtoANG(90.0)  , DEGtoANG(0.0) };
	static ANGLE wlk_ang[XYZ] =
		{ DEGtoANG(0.0), DEGtoANG(0.0)	, DEGtoANG(0.0) };






	calcModelVector( &AKI_ASHI_L );
	calcModelVector( &AKI_ASHI_R );
	calcModelVector( &AKI_HIZA_L );
	calcModelVector( &AKI_HIZA_R );
	calcModelVector( &AKI_MOMO_L );
	calcModelVector( &AKI_MOMO_R );
	calcModelVector( &AKI_HARA );
	calcModelVector( &AKI_KOSHI );
	calcModelVector( &AKI_MUNE );
	calcModelVector( &AKI_KAO );
	calcModelVector( &AKI_KUBI );
	calcModelVector( &AKI_ATAMA );
	calcModelVector( &AKI_KATA_L );
	calcModelVector( &AKI_KATA_R );
	calcModelVector( &AKI_TE_L );
	calcModelVector( &AKI_YUBI_L );
	calcModelVector( &AKI_OYAYUBI_L );
	calcModelVector( &AKI_TE_R );
	calcModelVector( &AKI_YUBI_R );
	calcModelVector( &AKI_OYAYUBI_R );
	calcModelVector( &AKI_UDE_L );
	calcModelVector( &AKI_UDE_R );


	setgstb( &AKI_ASHI_L );
	setgstb( &AKI_ASHI_R );
	setgstb( &AKI_HIZA_L );
	setgstb( &AKI_HIZA_R );
	setgstb( &AKI_MOMO_L );
	setgstb( &AKI_MOMO_R );
	setgstb( &AKI_HARA );
	setgstb( &AKI_KOSHI );
	setgstb( &AKI_MUNE );
	setgstb( &AKI_KAO );
	setgstb( &AKI_KUBI );
	setgstb( &AKI_ATAMA );
	setgstb( &AKI_KATA_L );
	setgstb( &AKI_KATA_R );
	setgstb( &AKI_TE_L );
	setgstb( &AKI_YUBI_L );
	setgstb( &AKI_OYAYUBI_L );
	setgstb( &AKI_TE_R );
	setgstb( &AKI_YUBI_R );
	setgstb( &AKI_OYAYUBI_R );
	setgstb( &AKI_UDE_L );
	setgstb( &AKI_UDE_R );


	slInitSystem(TV_352x224,NULL,1);
	slInitGouraud( gour,1024, GRaddr, vwork );
	slIntFunction( slGouraudTblCopy );
	slZdspLevel(7);

	slPrint("akira", slLocate(4,2));

	i = 0;

	slPushUnitMatrix();
	{
		slRotY(DEGtoANG(50.0));
		slRotX(DEGtoANG(50.0));
		slRotZ(DEGtoANG(50.0));
		slCalcPoint(toFIXED(0.0), toFIXED(0.0), toFIXED(1.0), light);
	}
	slPopMatrix();
#if 0
	light[X] = toFIXED(0.707);
	light[Y] = toFIXED(0.707);
	light[Z] = toFIXED(0.0);
#endif
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

/*		slScale( toFIXED(1.0), toFIXED(2.0), toFIXED(1.0) );*/
		
		motptr = &walk[i];
		slPushMatrix();
		{
			slTranslate(aki_pos[X], aki_pos[Y]+motptr->HARApos[Y], aki_pos[Z]);
			slRotZ(motptr->HARAang[Z] + aki_ang[X]);
			slRotY(motptr->HARAang[Y] + aki_ang[Y]);
			slRotX(motptr->HARAang[X] + aki_ang[Z]);
			slPutPolygonX(&AKI_HARA,light);

			slPushMatrix();
			{
				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->MUNEang[Z]);
				slRotY(motptr->MUNEang[Y]);
				slRotX(motptr->MUNEang[X]);
				slPutPolygonX(&AKI_MUNE,light);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slPushMatrix();
				{
					mcnt += 1;
					slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
					slRotZ(motptr->KAOang[Z]);
					slRotY(motptr->KAOang[Y]);
					slRotX(motptr->KAOang[X]);
					slPutPolygonX(&AKI_ATAMA,light);
					slPutPolygonX(&AKI_KUBI,light);
					slPutPolygonX(&AKI_KAO,light);
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
					slPutPolygonX(&AKI_KATA_R,light);
					slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

					slRotZ(motptr->R_UDEang);
					slPutPolygonX(&AKI_UDE_R,light);
					slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

					slRotZ(motptr->R_TEang[Z]);
					slRotY(motptr->R_TEang[Y]);
					slRotX(motptr->R_TEang[X]);
					slPutPolygonX(&AKI_TE_R,light);
					slPutPolygonX(&AKI_YUBI_R,light);
					slPutPolygonX(&AKI_OYAYUBI_R,light);
				}
				slPopMatrix();

				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->L_KATAang[Z]);
				slRotY(motptr->L_KATAang[Y]);
				slRotX(motptr->L_KATAang[X]);
				slPutPolygonX(&AKI_KATA_L,light);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->L_UDEang);
				slPutPolygonX(&AKI_UDE_L,light);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->L_TEang[Z]);
				slRotY(motptr->L_TEang[Y]);
				slRotX(motptr->L_TEang[X]);
				slPutPolygonX(&AKI_TE_L,light);
				slPutPolygonX(&AKI_YUBI_L,light);
				slPutPolygonX(&AKI_OYAYUBI_L,light);
			}
			slPopMatrix();

			mcnt += 1;
			slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
			slRotZ(motptr->KOSHIang[Z]);
			slRotY(motptr->KOSHIang[Y]);
			slRotX(motptr->KOSHIang[X]);
			slPutPolygonX(&AKI_KOSHI,light);
			slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

			slPushMatrix();
			{
				mcnt += 1;
				slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
				slRotZ(motptr->R_MOMOang[Z]);
				slRotY(motptr->R_MOMOang[Y]);
				slRotX(motptr->R_MOMOang[X]);
				slPutPolygonX(&AKI_MOMO_R,light);
				slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

				slRotZ(motptr->R_HIZAang);
				slPutPolygonX(&AKI_HIZA_R,light);
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
				slPutPolygonX(&AKI_ASHI_R,light);
			}
			slPopMatrix();

			mcnt += 1;
			slTranslate(root_pos[mcnt][X], root_pos[mcnt][Y], root_pos[mcnt][Z]);
			slRotZ(motptr->L_MOMOang[Z]);
			slRotY(motptr->L_MOMOang[Y]);
			slRotX(motptr->L_MOMOang[X]);
			slPutPolygonX(&AKI_MOMO_L,light);
			slTranslate(bone_leng[bcnt++], toFIXED(0.0), toFIXED(0.0));

			slRotZ(motptr->L_HIZAang);
			slPutPolygonX(&AKI_HIZA_L,light);
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
			slPutPolygonX(&AKI_ASHI_L,light);
		}
		slPopMatrix();

		data = Smpc_Peripheral[0].data;

		if( !(data & PER_DGT_TA) ) slSetGouraudTbl( GourTbl );
		if( !(data & PER_DGT_TB) ) slSetGouraudTbl( GourTbl2 );
		if( !(data & PER_DGT_TC) ) slSetGouraudTbl( GourTbl3 );
		if( !(data & PER_DGT_TX) ) slSetGouraudColor( C_RGB( 31,16,16 ) );
		if( !(data & PER_DGT_TY) ) slSetGouraudColor( C_RGB( 16,31,16 ) );
		if( !(data & PER_DGT_TZ) ) slSetGouraudColor( C_RGB( 16,16,31 ) );
		if( !(data & PER_DGT_TA) ) slSetGouraudTbl( GourTbl );
#if 0
		if( data & PER_DGT_TR ) aki_pos[Z] += toFIXED(1.0);
		if( data & PER_DGT_TL ) aki_pos[Z] -= toFIXED(1.0);
#endif		
		slSynch();

		if(++i >= WALK_CNT) i = 0;

		aki_ang[Y] += DEGtoANG(0.5);
		wlk_ang[Y] += DEGtoANG(0.5);
	}
}

