/********************************************************************
     < SGL Ver3.00 Sample Program >
  Sample Name : 3D Driving Sample(Near Clipping & Depth Cueing)
  Author      : H.S
  Date        : 09/10/1996
********************************************************************/

#include	"sgl.h"
#include	"sgl_cd.h"
#include	"sega_sys.h"

#include	"sglcolli.h"

#define FLASH_SATURN_CD

#define	pol_map_x	64
#define	pol_map_y	48

extern	Uint16	TotalPolygons;
extern	Uint16	DispPolygons;
extern	TEXTURE	tex_sample[];
extern	PICTURE	pic_sample[];
extern	PDATA	*map_data[];
extern	CDATA	*collison_data[];
extern	XPDATA	*xpdata_Kirby_of_the_Star[];

#include	"init.h"

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
#define		LINECOLOR				(VDP2_VRAM_A0   + 0x04000)

static	void set_matrix(FIXED *, ANGLE *);
static	void init_matrix(FIXED *, ANGLE *, FIXED *);
void	Collison_put(FIXED,FIXED,FIXED);
void	pos_move(void);
void	main_came(EVENT *);
void	main_pad(EVENT *);
void	main_Pmap(EVENT *);
void	main_char(EVENT *);
void	init_main(void);

#define	GRTBL(r,g,b)	(((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )
static	Uint16	DepthData[32] = {
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
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 16, 16, 16 ),
	GRTBL( 15, 15, 15 ),
	GRTBL( 14, 14, 14 ),
	GRTBL( 13, 13, 13 ),
	GRTBL( 12, 12, 12 ),
	GRTBL( 11, 11, 11 ),
	GRTBL( 10, 10, 10 ),
	GRTBL(  9,  9,  9 ),
	GRTBL(  8,  8,  8 ),
	GRTBL(  7,  7,  7 ),
	GRTBL(  6,  6,  6 ),
	GRTBL(  5,  5,  5 ),
	GRTBL(  4,  4,  4 ),
	GRTBL(  3,  3,  3 ),
	GRTBL(  2,  2,  2 ),
	GRTBL(  1,  1,  1 ),
	GRTBL(  0,  0,  0 ),
};

Sint32	tak_data;
FIXED	pos_add[XYZ];
FIXED	pos_add2[XYZ];
Sint16	pos_add2_xx;
ANGLE	ang_add;
FIXED	pos[XYZ], scl[XYZ], gpos[XYZ];
ANGLE	ang[XYZ];
FIXED	pos2[XYZ], scl2[XYZ];
ANGLE	ang2[XYZ];
Sega3D	sega3D;
Sint32	eg_speed;
int		col_hight;
int		col_att;
int		col_grp;
/*-----------------------------------------------------------------------*/
void	init_camera(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->cpos[X]	= toFIXED(0.0);
	segaPtr->cpos[Y]	= toFIXED(0.0);
	segaPtr->cpos[Z]	= 0xffce0000;
	segaPtr->ctarget[X]	= toFIXED(0.0);
	segaPtr->ctarget[Y]	= toFIXED(0.0);
	segaPtr->ctarget[Z]	= toFIXED(0.0);
	segaPtr->cangle[X]	= DEGtoANG(0.0);
	segaPtr->cangle[Y] 	= DEGtoANG(0.0);
	segaPtr->cangle[Z] 	= DEGtoANG(0.0);
}
/*--------------------------------------------------------------------------*/
void	init_object(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->pos[X] =	toFIXED(0.0);
	segaPtr->pos[Y] =	toFIXED(0.0);
	segaPtr->pos[Z] =	toFIXED(0.0);
	segaPtr->scl[X] =	toFIXED(1.0);
	segaPtr->scl[Y] =	toFIXED(1.0);
	segaPtr->scl[Z] =	toFIXED(1.0);
	segaPtr->ang[X] = 	DEGtoANG(90.0);
	segaPtr->ang[Y] =	DEGtoANG(0.0);
	segaPtr->ang[Z] = 	DEGtoANG(0.0);
}
/*--------------------------------------------------------------------------*/
static void init_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	pos[X] = toFIXED(800.0);
	pos[Y] = toFIXED(800.0);
	pos[Z] = toFIXED(110.0);
	ang[X] = 0x4008;
	ang[Y] = DEGtoANG(0.0);
	ang[Z] = DEGtoANG(-90.0);
	scl[X] = toFIXED(1.0);
	scl[Y] = toFIXED(1.0);
	scl[Z] = toFIXED(1.0);
}
/*--------------------------------------------------------------------------*/
void	init_main()
{
	eg_speed	=0;
	tak_data	=-40;
	pos_add[X]	=toFIXED(0.0);
	pos_add[Y]	=toFIXED(0.0);
	pos_add[Z]	=toFIXED(0.0);
	pos_add2[X]	=toFIXED(0.0);
	pos_add2[Y]	=toFIXED(0.0);
	pos_add2[Z]	=toFIXED(0.0);
	pos_add2_xx	=0;
	ang_add		=DEGtoANG(0.0);
	pos[X]		=toFIXED(3327.0);
	pos[Y]		=toFIXED( 355.0);
	ang[Z]		=DEGtoANG(90.0);
}
/*--------------------------------------------------------------------------*/
static void set_matrix(FIXED *pos, ANGLE *ang)
{
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
	slTranslate(pos[X], pos[Y], pos[Z]);
}
#define	set_polygon	set_matrix
/*--------------------------------------------------------------------------*/
static	GOURAUDTBL	gr_tab[300];
static	Uint16	point_temp[300];
void	ss_main(void)
{
	slInitSystem(TV_352x224,tex_sample,2);
	slTVOff();
	slPrint("[3D Driving Sample2]" , slLocate(9,2));
	slPrint("TotalPolygons", slLocate(30,3));
	slPrint("DispPolygons", slLocate(30,5));
	set_texture(pic_sample,11);
	slTVOn();
/*--------------------------------------------------------------------------*/
	init_camera((void*)&sega3D);
	init_object((void*)&sega3D);
	init_matrix(pos, ang, scl);	
	init_main();
	pos_move();
	slZdspLevel(5);
/*--------------------------------------------------------------------------*/
	slInitEvent();							/*  イベントの初期化		*/
	slSetEvent((void *)main_pad);			/*  パッド入力関係			*/
	slSetEvent((void *)main_came);			/*  カメラ位置指定関係		*/
	slSetEvent((void *)main_Pmap);			/*  ポリゴンマップ表示関係	*/
/*--------------------------------------------------------------------------*/
	slPerspective(DEGtoANG(80));
	slSetDepthLimit(0,11,5);
	slSetDepthTbl(DepthData,0xf000,32);
	/*--------------------------------------*/
	slInitGouraud((GOURAUDTBL *)gr_tab,(Uint32)300,0xe000,(Uint8 *)point_temp);
	slIntFunction(slGouraudTblCopy);
	/*--------------------------------------*/
	slWindowClipLevel(50);
	slNearClipFlag(1);
	while(1)
	{
#ifdef FLASH_SATURN_CD
		if ( CDC_GetHirqReq() & CDC_HIRQ_DCHG ) {
			SYS_Exit(1);
		}
#endif
		slExecuteEvent();					/*	イベントの実行			*/
		slPrintFX(TotalPolygons<<16, slLocate(30,4));
		slPrintFX(DispPolygons<<16, slLocate(30,6));
		slSynch();							/*	シンク					*/
	} 
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------*/
/*	カメラ関係							*/
/*--------------------------------------*/
void	main_came(EVENT *evptr)
{
	slUnitMatrix(CURRENT);
	slLookAt(sega3D.cpos, sega3D.ctarget, sega3D.cangle[Z]);
}
void	main_Pmap(EVENT *evptr)
{
	Sint32	MyAddX,MyAddY;
	FIXED	pom[XYZ];
	Sint32	MyAbsX,MyAbsY;
	Sint32	MapAngX,MapAngY;
	PDATA	*pol;
	Sint32	bb;
	Sint32	ax,ay;
	Sint32	my_xx,my_yy;
	Sint32	X_MAX,X_MIN;
	Sint32	Y_MAX,Y_MIN;
	Sint32	gg_cc;

	my_xx	=((Sint16)((pos[X]+256*slSin(ang[Z]))>>16))>>5;
	my_yy	=((Sint16)((pos[Y]+256*slCos(ang[Z]))>>16))>>5;
	MapAngX	= slCos(ang[Z])*256;
	MapAngY	=-slSin(ang[Z])*256;
	gg_cc	=0;
	slPushMatrix();
	{
		X_MIN	=(my_xx>>2)-18;
		X_MAX	=(my_xx>>2)+19;
		if(X_MIN < 0)X_MIN=0;
		if(X_MAX > pol_map_x)X_MAX=pol_map_x;
		Y_MIN	=(my_yy>>2)-18;
		Y_MAX	=(my_yy>>2)+19;
		if(Y_MIN < 0)Y_MIN=0;
		if(Y_MAX > pol_map_y)Y_MAX=pol_map_y;

		set_matrix(pos, ang);
		for(ax = X_MIN ; ax < X_MAX ; ax++)
		{
			for(ay = Y_MIN ; ay < Y_MAX ; ay++)
			{
				pol	=map_data[ax+ay*pol_map_x];
				if(pol->nbPolygon != 0){
					MyAbsX	=abs((ax<<2)-my_xx);
					MyAbsY	=abs((ay<<2)-my_yy);
					if((MyAbsX < 16) && (MyAbsY<16))
					{
						slPutPolygonS(pol);
					}
					else
					{
						pom[X]	=(-ax<<23)-toFIXED(64)+MapAngX-MapAngY*2;
						pom[Y]	=(-ay<<23)-toFIXED(64)+MapAngX*2+MapAngY;
						pom[Z]	=0;
						if(slCheckOnScreen(pom,toFIXED(128.0)) >= 0){
							slPutPolygonS(map_data[ax+ay*pol_map_x]);
						}
					}
				}
			}
		}
	}
	slPopMatrix();
}
/*--------------------------------------*/
/*	位置保存							*/
/*--------------------------------------*/
void	pos_move()
{
	pos2[X]	=pos[X];
	pos2[Y]	=pos[Y];
	pos2[Z]	=pos[Z];
	ang2[X]	=ang[X];
	ang2[Y]	=ang[Y];
	ang2[Z]	=ang[Z];
}
/*--------------------------------------*/
/*	パッド関係							*/
/*--------------------------------------*/
void	main_pad(EVENT *evptr)
{
	Uint16		point_l,point_r;
	FIXED		ax,ay,az;
	FIXED		bx,by,bz;
	Sint16		aa;
	Uint16		data;

	data = Smpc_Peripheral[15].data;					//パッド２
	if((data & PER_DGT_TC) == 0)tak_data+=10;
	if((data & PER_DGT_TZ) == 0)tak_data-=10;
	if((data & PER_DGT_KL) == 0)ang[Z]+=DEGtoANG(4.0);
	if((data & PER_DGT_KR) == 0)ang[Z]-=DEGtoANG(4.0);
	if((data & PER_DGT_KU) == 0)ang[X]+=DEGtoANG(4.0);
	if((data & PER_DGT_KD) == 0)ang[X]-=DEGtoANG(4.0);
	/*---------------------------------------------*/
	data = Smpc_Peripheral[0].data;						//パッド１
#ifdef FLASH_SATURN_CD
	if((data & (PER_DGT_ST | PER_DGT_TA | PER_DGT_TB | PER_DGT_TC)) == 0){
		SYS_Exit(0);
	}
#endif
	if((data & PER_DGT_ST) == 0)init_main();
/*---------------------------------------------*/
/*車の挙動はアバウトに作っているので参考にしないでください。*/
/*---------------------------------------------*/
	/*加速減速*/
	aa	=0;
	if((data & PER_DGT_TB) == 0)
	{
		if(eg_speed < 0)
		{
			eg_speed	=0;
		}
		else
		{
			if(++eg_speed > 45)eg_speed=45;
		}
		aa	=1;
	}
	if((data & PER_DGT_TY) == 0)
	{
		if(eg_speed > 0)
		{
			eg_speed	=0;
		}
		else
		{
			if(--eg_speed < -30)eg_speed=-30;
		}
		aa	=1;
	}
	if(aa == 0)
	{
		if(eg_speed > 0)eg_speed--;
		if(eg_speed < 0)eg_speed++;
	}
	pos_add[X]	-=eg_speed*slSin(ang[Z])/10;
	pos_add[Y]	-=eg_speed*slCos(ang[Z])/10;
	/*---------------------------------------------*/
	/*サイド移動*/
	if((data & PER_DGT_TL) == 0)
	{
		pos[X]	+=3*slCos(ang[Z]);
		pos[Y]	-=3*slSin(ang[Z]);
	}
	if((data & PER_DGT_TR) == 0){
		pos[X]	-=3*slCos(ang[Z]);
		pos[Y]	+=3*slSin(ang[Z]);
	}
	/*---------------------------------------------*/
	/*左右の回転*/
	aa	=0;
	if((data & PER_DGT_KR) == 0)
	{
		ang_add	+=DEGtoANG(1.0);
		aa	=1;
		if(pos_add2_xx == 0)
		{
			pos_add[X]	=pos_add[X]/2;
			pos_add[Y]	=pos_add[Y]/2;
			pos_add[Z]	=pos_add[Z]/2;
			pos_add2[X]	=pos_add[X];
			pos_add2[Y]	=pos_add[Y];
			pos_add2[Z]	=pos_add[Z];
		}
		pos_add2_xx	=1;
	}
	if((data & PER_DGT_KL) == 0)
	{
		ang_add	-=DEGtoANG(1.0);
		aa	=1;
		if(pos_add2_xx == 0)
		{
			pos_add[X]	=pos_add[X]/2;
			pos_add[Y]	=pos_add[Y]/2;
			pos_add[Z]	=pos_add[Z]/2;
			pos_add2[X]	=pos_add[X];
			pos_add2[Y]	=pos_add[Y];
			pos_add2[Z]	=pos_add[Z];
		}
		pos_add2_xx	=1;
	}
	if(aa == 0)pos_add2_xx	=0;
	/*---------------------------------------------*/
	ax			=pos[X]+pos_add[X]+pos_add2[X];
	ay			=pos[Y]+pos_add[Y]+pos_add2[Y];
	az			=pos[Z]+pos_add[Z]+pos_add2[Z];
	pos_add[X]	=pos_add[X]*7/8;
	pos_add[Y]	=pos_add[Y]*7/8;
	pos_add[Z]	=pos_add[Z]*7/8;
	pos_add2[X]	=pos_add2[X]*9/10;
	pos_add2[Y]	=pos_add2[Y]*9/10;
	pos_add2[Z]	=pos_add2[Z]*9/10;

	bx	=ax+30*slCos(ang[Z]);
	by	=ay-30*slSin(ang[Z]);
	Collison_put((bx),(by),pos[Z]);
	point_l	=col_att;
	
	bx	=ax-30*slCos(ang[Z]);
	by	=ay+30*slSin(ang[Z]);
	Collison_put((bx),(by),pos[Z]);
	point_r	=col_att;

	if(point_r == 1 || point_l == 1)
	{
		if(point_r == 1 && point_l == 1)
		{
			pos[X]	=ax;
			pos[Y]	=ay;
		}
		if(point_r == 2)
		{
			ang_add	-=DEGtoANG(2.0);
			pos[X]	=ax+20*slCos(ang[Z]);
			pos[Y]	=ay-20*slSin(ang[Z]);
		}
		if(point_l == 2)
		{
			ang_add	+=DEGtoANG(2.0);
			pos[X]	=ax-20*slCos(ang[Z]);
			pos[Y]	=ay+20*slSin(ang[Z]);
		}
		if(point_r == 3)
		{
			ang_add	-=DEGtoANG(3.0);
			pos[X]	=ax+25*slCos(ang[Z]);
			pos[Y]	=ay-25*slSin(ang[Z]);
		}
		if(point_l == 3)
		{
			ang_add	+=DEGtoANG(3.0);
			pos[X]	=ax-25*slCos(ang[Z]);
			pos[Y]	=ay+25*slSin(ang[Z]);
		}
	}
	else
	{
		pos_add[X]	=-pos_add[X];
		pos_add[Y]	=-pos_add[Y];
		pos_add2[X]	=-pos_add2[X];
		pos_add2[Y]	=-pos_add2[Y];
		ax			=pos[X]+pos_add[X]+pos_add2[X];
		ay			=pos[Y]+pos_add[Y]+pos_add2[Y];
		eg_speed	=0;
	}
	Collison_put(pos[X],pos[Y],pos[Z]);
	pos[Z]	=toFIXED(col_hight+tak_data);
	/*---------------------------------------------*/
	ang[Y]	=-(ang_add)/2;
	ang[Z]	=ang[Z]+(ang_add>>1);
	ang_add	=ang_add*4/5;
	/*---------------------------------------------*/
}
/*-------------------------------------------------*/
/*	コリジョン	高さ抽出							*/
/*-------------------------------------------------*/
void	Collison_put(FIXED x,FIXED y,FIXED z)
{
	FIXED		li;
	FIXED		li_min;
	COLLISON	*GetCol;
	int			GetColNo;
	int			sx,sy,sz;
	int			A,B,C;
	FIXED		poa,pob,poc;
	int			aa_loop,aa;
	int			ax,ay,ux,uy;
	int			ax_min,ax_max;
	int			ay_min,ay_max;
	CDATA		*a_cdata;
	COLLISON	*a_collison;

	ux	=x>>(23);
	uy	=y>>(23);
	
	ax_min	=ux-1;
	ax_max	=ux+2;
	if(ax_min<0)ax_min=0;
	if(ax_max > pol_map_x)ax_max=pol_map_x;
	ay_min	=uy-1;
	ay_max	=uy+2;
	if(ay_min<0)ay_min=0;
	if(ay_max > pol_map_y)ay_max=pol_map_y;

	GetColNo	=100;
	li_min	=toFIXED(2000);
	for(ay = ay_min ; ay < ay_max ; ay++)
	{
		for(ax = ax_min ; ax < ax_max ; ax++)
		{
			a_cdata	=(CDATA *)collison_data[ax+ay*pol_map_x];
			aa_loop	=a_cdata->nbCo;
			if(aa_loop != 0)
			{
				a_collison	=(COLLISON *)a_cdata->cotbl;
				for(aa = 0 ; aa < aa_loop ; aa++)
				{
					sx	=(a_collison[aa].cen_x+x)>>16;
					sy	=(a_collison[aa].cen_y+y)>>16;
					sz	=(a_collison[aa].cen_z+z)>>16;
					li	=slSquart(sx*sx+sy*sy+sz*sz);
					if(li_min > li)
					{
						li_min		=li;
						GetCol		=(COLLISON *)a_collison;
						GetColNo	=aa;
					}
				}
			}
		}
	}
	if(GetColNo == 100)
	{
		col_hight		=0;
		col_att			=0;
		col_grp			=0;
	}
	else
	{
		A	=GetCol[GetColNo].norm[X];
		B	=GetCol[GetColNo].norm[Y];
		C	=GetCol[GetColNo].norm[Z];

		poa	=GetCol[GetColNo].cen_x;
		pob	=GetCol[GetColNo].cen_y;
		poc	=GetCol[GetColNo].cen_z;
		if( C != 0)
		{
			col_hight		=-(A*((-x-poa)>>16)+B*((-y-pob)>>16))/C-(poc>>16);
		}
		else
		{
			col_hight		=-poc>>16;
		}
		col_att		=GetCol[GetColNo].att;
		col_grp		=GetCol[GetColNo].gru;
	}
}
