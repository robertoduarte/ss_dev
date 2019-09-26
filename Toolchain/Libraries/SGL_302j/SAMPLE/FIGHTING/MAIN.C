/********************************************************************
     < SGL Ver3.00 Sample Program >
  Sample Name : 3D Fighting Sample(RealTime Gouraud)
  Author      : H.S
  Date        : 09/10/1996
********************************************************************/
#include	"sgl.h"
#include	"sgl_cd.h"
#include	"ss_scrol.h"
#include	"sega_sys.h"
#define FLASH_SATURN_CD

#define	CAMERA_ZDEF	-200.0
#define	OFFSET_SCL	1.0

#define		VRAMaddr		(SpriteVRAM+0x70000)

#define		SEGA3D
#define		game_speed	2
#define		game_ang	4

typedef struct{
	FIXED	cpos[XYZ];
	FIXED	ctarget[XYZ];
	ANGLE	cangle[XYZ];
	FIXED	pos[XYZ];
	ANGLE	ang[XYZ];
	FIXED	scl[XYZ];
}Sega3D, *Sega3DPtr;

extern	Uint16	TotalPolygons;
extern	Uint16	DispPolygons;
extern	Uint16	TotalVertices;
extern	Uint8	sora_cel[];
extern	Uint16	sora_map[];
extern	Uint16	sora_pal[];
extern	Uint8	tuti_cel[];
extern	Uint16	tuti_map[];
extern	Uint16	tuti_pal[];

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

#define		NBG2_CEL_ADR            (VDP2_VRAM_B1+0x02000)
#define		NBG2_MAP_ADR            (VDP2_VRAM_B1+0x18000)
#define		NBG2_COL_ADR            ( VDP2_COLRAM  + 0x00800 )

#define		LINE_SCR_TABL		(VDP2_VRAM_A0 + 0x1f000)
#define		LINE_COLOR_TABLE		(VDP2_VRAM_A0	+ 0x1f400)
#define		COLOR_RAM_ADR			(VDP2_COLRAM	+ 0x00600)
#define		CEL_LINE_TABLE			( VDP2_VRAM_B1 + 0x1f000)

static void set_matrix(FIXED *, ANGLE *, FIXED *);
static void init_matrix(FIXED *, ANGLE *, FIXED *);
static	Sint32	LINE_TE1[224];
static	Sint32	LINE_TE2[224];
static	Sint32	LINE_TE;
TEXTURE tex_sample[]={
	TEXDEF(64,64,0),				/*0*/
	TEXDEF(64,64,64*64),
	TEXDEF(64,64,64*64*2),
};
extern	PDATA  PD_miti;
extern	PDATA  PD_BOX;
extern	PDATA  PD_box2;
extern	PDATA  PD_kage;
extern	XPDATA XPD_i_rob00;
extern	XPDATA XPD_i_rob01;
extern	XPDATA XPD_i_rob02;
extern	XPDATA XPD_i_rob03;
extern	XPDATA XPD_i_rob04;
extern	XPDATA XPD_i_rob05;
extern	XPDATA XPD_i_rob06;
extern	XPDATA XPD_i_rob07;
extern	XPDATA XPD_i_rob08;
extern	XPDATA XPD_i_rob09;

extern	XPDATA XPD_i2_rob00;
extern	XPDATA XPD_i2_rob01;
extern	XPDATA XPD_i2_rob02;
extern	XPDATA XPD_i2_rob03;
extern	XPDATA XPD_i2_rob04;
extern	XPDATA XPD_i2_rob05;
extern	XPDATA XPD_i2_rob06;
extern	XPDATA XPD_i2_rob07;
extern	XPDATA XPD_i2_rob08;
extern	XPDATA XPD_i2_rob09;

extern	Uint8	ami_map[];
extern	Uint8	ami_chip[];
extern	Uint16	ami_pal[];

static	GOURAUDTBL	gr_tab[512];
static	Uint16	point_temp[300];
static	Sint32	obj_xx;
typedef struct{
	Sint32	no;
	FIXED	X;
	FIXED	Y;
	FIXED	X_add;
	FIXED	Y_add;
}object;
object	obj[40];
FIXED	light_d[XYZ];
extern	Uint16	gg_data[];
FIXED	pos_kk[XYZ];
FIXED	pos[XYZ], scl[XYZ], gpos[XYZ];
ANGLE	ang[XYZ];
FIXED	pos2[XYZ], scl2[XYZ];
ANGLE	ang2[XYZ];
Sega3D	sega3D;
ANGLE	l_ang;
ANGLE	m_ang;

typedef struct{
	FIXED	posX;
	FIXED	posY;
	FIXED	posZ;
	FIXED	posX_add;
	FIXED	posY_add;
	FIXED	posZ_add;
	ANGLE	ang;		/*アングル*/
	FIXED	sc;			/*スコアー*/
	Sint16	miss;		/*ミサイル*/
	Sint16	xx;			/*	*/
	Uint16	data;		/*  前回のコマンドテーブル*/
	Uint16	bot1;		/*１個前のコマンドテーブル*/
	Uint16	bot2;		/*２個前のコマンドテーブル*/
	Uint16	bot3;		/*３個前のコマンドテーブル*/
} robo_xx;
static	robo_xx	robo1;
static	robo_xx	robo2;


void	main_came(EVENT *);
void	main_pad(EVENT *);
void	main_Pmap(EVENT *);
void	main_map(EVENT *);
void	main_obj(EVENT *);
void	robo_put(robo_xx*,XPDATA*);
void	obj_set(Sint32,FIXED,FIXED,FIXED,FIXED);
void	pad_ck(robo_xx*,FIXED,Uint32,Uint16);

void Cel2VRAM( Uint8 *Cel_Data , void *Cell_Adr , Uint32 suu )
{
	Uint32 i;
	Uint8 *VRAM;

	VRAM = (Uint8 *)Cell_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Cel_Data++);
}
void Map2VRAM( Uint16 *Map_Data , void *Map_Adr , Uint16 suuj , Uint16 suui , Uint16 palnum ,Uint32 mapoff)
{
	Uint16 i , j;
	Uint16 paloff;
	Uint16 *VRAM;

	paloff= palnum << 12;
	VRAM = (Uint16 *)Map_Adr;
	

	for( i = 0; i < suui; i++ )
		{
		for( j = 0; j < suuj; j++ )
		{
			*VRAM++ = (*Map_Data | paloff) + mapoff;
			Map_Data++;
		}
		VRAM += (64 - suuj);
	}
}
void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu )
{
	Uint16 i;
	Uint16 *VRAM;

	VRAM = (Uint16 *)Col_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Pal_Data++);
}

void	init_camera(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->cpos[X]	= toFIXED(0.0);
	segaPtr->cpos[Y]	= toFIXED(0.0);
	segaPtr->cpos[Z]	= toFIXED(CAMERA_ZDEF);
	segaPtr->ctarget[X]	= toFIXED(0.0);
	segaPtr->ctarget[Y]	= toFIXED(0.0);
	segaPtr->ctarget[Z]	= toFIXED(0.0);
	segaPtr->cangle[X]	= DEGtoANG(0.0);
	segaPtr->cangle[Y] 	= DEGtoANG(0.0);
	segaPtr->cangle[Z] 	= DEGtoANG(0.0);
}

void	init_object(void *ptr)
{
	Sega3DPtr	segaPtr = (Sega3DPtr)ptr;

	segaPtr->pos[X] =	toFIXED(0.0);
	segaPtr->pos[Y] =	toFIXED(0.0);
	segaPtr->pos[Z] =	toFIXED(0.0);
	segaPtr->scl[X] =	toFIXED(1.0 * OFFSET_SCL);
	segaPtr->scl[Y] =	toFIXED(1.0 * OFFSET_SCL);
	segaPtr->scl[Z] =	toFIXED(1.0 * OFFSET_SCL);
	segaPtr->ang[X] = 	DEGtoANG(90.0);
	segaPtr->ang[Y] =	DEGtoANG(0.0);	/*for Scroll to Sprite*/
	segaPtr->ang[Z] = 	DEGtoANG(0.0);
}

static void init_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	pos[X] = toFIXED(-100.0);
	pos[Y] = toFIXED(100.0);
	pos[Z] = toFIXED(115.0);
	ang[X]	=-12744;
	ang[Y] = DEGtoANG(0.0);
	ang[Z] = DEGtoANG(0.0);
	scl[X] = toFIXED(1.0);
	scl[Y] = toFIXED(1.0);
	scl[Z] = toFIXED(1.0);
}

static void set_matrix(FIXED *pos, ANGLE *ang, FIXED *scl)
{
	slRotX(ang[X]);
	slRotY(ang[Y]);
	slRotZ(ang[Z]);
	slTranslate(pos[X], pos[Y], pos[Z]);
	slScale(scl[X], scl[Y], scl[Z]);
}
void	init_VDP2()
{
	Uint16	*line_tt;
	Sint32	aa,bx;
	Uint16	*Line_Color_Pal0;

	slTVOff();
	slColRAMMode(CRM16_2048);
/*------------------------------------------------------*/
/*		ラインカラー画面設定  							*/
/*------------------------------------------------------*/
	Line_Color_Pal0	=(Uint16 *)COLOR_RAM_ADR;
	for(aa = 0; aa < 224; aa++)
	{
		Line_Color_Pal0[aa+32] = 0x8000;
	}
	for(aa = 0; aa < 32; aa++)
	{
		Line_Color_Pal0[80-aa+32] = (aa << 10 | aa << 5 | aa | 0x8000);
		Line_Color_Pal0[81+aa-32] = (aa << 10 | aa << 5 | aa | 0x8000);
	}
	Line_Color_Pal0	=(Uint16 *)LINE_COLOR_TABLE;
	for(aa = 0; aa < 224; aa++)
	{
		Line_Color_Pal0[aa] = aa+256*3+32;
	}
	slLineColDisp( RBG0ON | NBG2ON);
	slLineColTable((void *)LINE_COLOR_TABLE);
 	slColorCalc( CC_ADD | CC_TOP | RBG0ON | NBG2ON);
	slColorCalcOn( RBG0ON |NBG2ON);
	slColRateLNCL(0x1f);
/*------------------------------------------------------*/
/*------------------------------------------------------*/
	slRparaMode(W_CHANGE);				/*回転スクロールのモード*/

	slRparaInitSet((void *)RBG0_PRA_ADR);
	slMakeKtable((void *)RBG0_KTB_ADR);
	slCharRbg0(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageRbg0((void *)RBG0RB_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);

	slPlaneRA(PL_SIZE_1x1);
	sl1MapRA((void *)RBG0RA_MAP_ADR);
	slOverRA(0);
	slKtableRA((void *)RBG0_KTB_ADR,K_FIX|K_DOT|K_2WORD|K_ON);
	Cel2VRAM(tuti_cel , (void *)RBG0RA_CEL_ADR , 65536);
	Map2VRAM(tuti_map , (void *)RBG0RA_MAP_ADR , 64 , 64 , 2 , 884);
	Pal2CRAM(tuti_pal , (void *)RBG0RA_COL_ADR , 160);
/*------------------------------------------------------*/
	slPlaneRB(PL_SIZE_1x1);
	sl1MapRB((void *)RBG0RB_MAP_ADR);
	slOverRB(0);
	slKtableRB((void *)RBG0_KTB_ADR , K_FIX | K_DOT | K_2WORD | K_ON );
	Cel2VRAM(sora_cel , (void *)RBG0RB_CEL_ADR , 28288);
	Map2VRAM(sora_map , (void *)RBG0RB_MAP_ADR , 64 , 64 , 1 , 0);
	Pal2CRAM(sora_pal , (void *)RBG0RB_COL_ADR , 256);
/*------------------------------------------------------*/
	slCharNbg2(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageNbg2((void *)NBG2_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);
    slPlaneNbg2(PL_SIZE_1x1);
    slMapNbg2((void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR , (void *)NBG2_MAP_ADR);

    Cel2VRAM(sora_cel , (void *)NBG2_CEL_ADR , 28288) ;
    Map2VRAM(sora_map , (void *)NBG2_MAP_ADR , 64 , 64 , 4 ,256) ;
    Pal2CRAM(sora_pal , (void *)NBG2_COL_ADR , 256) ;
/*------------------------------------------------------*/
/*ウインドウ設定*/
	slScrLineWindow0((void *)(LINE_SCR_TABL | 0x80000000));/**/
	slScrWindowMode(scnROT,win0_IN);		/*ウインド関係*/

	slScrTransparent(RBG0ON);
	slColRAMOffsetNbg0(7);

	slBack1ColSet((void *)BACK_COL_ADR , 0);
	slScrAutoDisp(NBG0ON | NBG2ON | RBG0ON);
	slTVOn();
}
void	init_ss_main()
{
	Sint32	aa;

	m_ang	=DEGtoANG(0.0);
	l_ang	=DEGtoANG(0.0);
	
	robo1.posX		=toFIXED(500.0);
	robo1.posY		=toFIXED(-500.0);
	robo1.posZ		=toFIXED(-60.0);
	robo1.posX_add	=toFIXED(0.0);
	robo1.posY_add	=toFIXED(0.0);
	robo1.posZ_add	=toFIXED(0.0);
	robo1.sc		=0;
	robo1.xx		=0;
	robo1.miss		=0;
	robo1.data		=0;
	robo1.bot1		=0;
	robo1.bot2		=0;
	robo1.bot3		=0;

	robo2.posX		=toFIXED(200.0);
	robo2.posY		=toFIXED(-200.0);
	robo2.posZ		=toFIXED(-60.0);
	robo2.posX_add	=toFIXED(0.0);
	robo2.posY_add	=toFIXED(0.0);
	robo2.posZ_add	=toFIXED(0.0);
	robo2.sc		=0;
	robo2.xx		=0;
	robo2.miss		=0;
	robo2.data		=0;
	robo2.bot1		=0;
	robo2.bot2		=0;
	robo2.bot3		=0;

	for(aa = 0 ; aa < 40 ; aa++)obj[aa].no	=0;		/*オブジェクト初期化*/
	obj_xx	=0;

	pos2[X]	=pos[X];
	pos2[Y]	=pos[Y];
	pos2[Z]	=pos[Z];
	scl2[X]	=scl[X];
	scl2[Y]	=scl[Y];
	scl2[Z]	=scl[Z];
	ang2[X]	=ang[X];
	ang2[Y]	=ang[Y];
	ang2[Z]	=ang[Z];

	light_d[X]	=toFIXED(0.0);
	light_d[Y]	=toFIXED(0.0);
	light_d[Z]	=toFIXED(0.0);
	
	slZdspLevel(7);
}
void	ss_main(void)
{
	Uint16	*line_tab;
	Sint16	ax;
	Uint16	*pal_add;

	slInitSystem(TV_352x224,tex_sample,1);
	slPrint("Fighting Sample" , slLocate(15,2));
	slPrint("TotalPolygons", slLocate(30,3));
	slPrint("DispPolygons", slLocate(30,5));
	slPrint("TotalVertices", slLocate(30,7));
	slPrint("Ble SCORE", slLocate(3,1));
	slPrint("Red SCORE", slLocate(30,1));

slDMACopy((Uint16 *)gg_data,(void *)(SpriteVRAM+0x10000),(Uint32)64*64*2*3);
slInitGouraud((GOURAUDTBL *)gr_tab,(Uint32)512,0xe000,(Uint8 *)point_temp);
	slIntFunction(slGouraudTblCopy);
	
	init_VDP2();							/*ＶＤＰ２初期設定*/
	init_ss_main();							/*初期値設定*/
	init_camera((void*)&sega3D);
	init_object((void*)&sega3D);
	init_matrix(pos, ang, scl);
	LINE_TE	=0;

	slInitEvent();							/*  イベントの初期化		*/
	slSetEvent((void *)main_pad);			/*  パッド入力関係			*/
	slSetEvent((void *)main_came);			/*  カメラ位置指定関係		*/
	slSetEvent((void *)main_obj);			/* 	ポリゴンオジェ表示関係	*/
	slSetEvent((void *)main_Pmap);			/*  ポリゴンマップ表示関係	*/
	slSetEvent((void *)main_map);			/*  ポリゴンマップ表示関係	*/

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
		slPrintFX(TotalVertices<<16, slLocate(30,8));
		slPrintFX(robo1.sc, slLocate(3,2));
		slPrintFX(robo2.sc, slLocate(30,2));
		slSynch();							/*	シンク					*/
	} 
}

/*--------------------------------------*/
/*	カメラ関係							*/
/*--------------------------------------*/
void	main_came(EVENT *evptr)
{
	slUnitMatrix(CURRENT);
	slLookAt(sega3D.cpos, sega3D.ctarget, sega3D.cangle[Z]);
}
/*--------------------------------------*/
/*	ポリゴンマップ関係の表示			*/
/*--------------------------------------*/
void	main_Pmap(EVENT *evptr)
{
	Sint32	*LINE_T;
static	FIXED	PointL[16][XYZ];
	Sint32	an[XY];
	FIXED	point_pos[XYZ];
	FIXED	temp;
	Sint32	Po_MAX,Po_MIN;
	Sint32	PoyMAX,PoyMIN;
	Sint32	Po_LINE,End;
	Sint32	POS_X1,POS_X2;
	Sint32	pos_x1,pos_x2;
	Sint32	POS_ADDX1,POS_ADDX2;
	Sint32	PO1,PO2,POU;
	Sint32	PO1Y,PO2Y;
static	XPDATA	*my1_pd_data[]	={&XPD_i_rob00,&XPD_i_rob01,
								&XPD_i_rob02,&XPD_i_rob03,
								&XPD_i_rob04,&XPD_i_rob05,
								&XPD_i_rob06,&XPD_i_rob07,
								&XPD_i_rob08,&XPD_i_rob09};
static	XPDATA	*my2_pd_data[]	={&XPD_i2_rob00,&XPD_i2_rob01,
								&XPD_i2_rob02,&XPD_i2_rob03,
								&XPD_i2_rob04,&XPD_i2_rob05,
								&XPD_i2_rob06,&XPD_i2_rob07,
								&XPD_i2_rob08,&XPD_i2_rob09};
	if(LINE_TE == 0)
	{
		LINE_T	=(Sint32 *)LINE_TE1;
	} else {
		LINE_T	=(Sint32 *)LINE_TE2;
	}
	robo_put(&robo1,(XPDATA *)my1_pd_data);
	robo_put(&robo2,(XPDATA *)my2_pd_data);
	light_d[Y]	=slSin(ang[Z]+l_ang);
	light_d[Z]	=slCos(ang[Z]+l_ang);
	m_ang+=DEGtoANG(4.0);
	l_ang+=DEGtoANG(2.0);
	slPushMatrix();
	{
		set_matrix(pos, ang, scl);
		slPutPolygon(&PD_miti);
/*------------------------------------------------------------------*/
/*	ﾗｲﾝｳｲﾝﾄﾞｳの計算をします。										*/
/*	今回は暫定的な計算のしかたなのでこれから最適化はします			*/
/*	最終的にはテーブルにより、こんな大きなﾌﾟﾛｸﾞﾗﾑにはなりません		*/
/*------------------------------------------------------------------*/
		point_pos[X]	=toFIXED(0.0);
		point_pos[Y]	=toFIXED(0.0);
		point_pos[Z]	=toFIXED(0.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[0][X]	=(an[X]<<1)+toFIXED(352);
			PointL[0][Y]	=(an[Y]>>16)+112;
			PointL[0][Z]	=0;
		} else {
			PointL[0][Y]	=512;
			PointL[0][Z]	=1;
		}
		point_pos[X]	=toFIXED(100.0);
		point_pos[Y]	=toFIXED(0.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[1][X]	=(an[X]<<1)+toFIXED(352);
			PointL[1][Y]	=(an[Y]>>16)+112;
			PointL[1][Z]	=0;
		} else {
			PointL[1][Y]	=512;
			PointL[1][Z]	=1;
		}

		point_pos[X]	=toFIXED(1000.0);
		point_pos[Y]	=toFIXED(0.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[2][X]	=(an[X]<<1)+toFIXED(352);
			PointL[2][Y]	=(an[Y]>>16)+112;
			PointL[2][Z]	=0;
		} else {
			PointL[2][Y]	=512;
			PointL[2][Z]	=1;
		}

		point_pos[X]	=toFIXED(1900.0);
		point_pos[Y]	=toFIXED(0.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[3][X]	=(an[X]<<1)+toFIXED(352);
			PointL[3][Y]	=(an[Y]>>16)+112;
			PointL[3][Z]	=0;
		} else {
			PointL[3][Y]	=512;
			PointL[3][Z]	=1;
		}

		point_pos[X]	=toFIXED(2000.0);
		point_pos[Y]	=toFIXED(0.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[4][X]	=(an[X]<<1)+toFIXED(352);
			PointL[4][Y]	=(an[Y]>>16)+112;
			PointL[4][Z]	=0;
		} else {
			PointL[4][Y]	=512;
			PointL[4][Z]	=1;
		}

		point_pos[X]	=toFIXED(2000.0);
		point_pos[Y]	=toFIXED(-100.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[5][X]	=(an[X]<<1)+toFIXED(352);
			PointL[5][Y]	=(an[Y]>>16)+112;
			PointL[5][Z]	=0;
		} else {
			PointL[5][Y]	=512;
			PointL[5][Z]	=1;
		}

		point_pos[X]	=toFIXED(2000.0);
		point_pos[Y]	=toFIXED(-1000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[6][X]	=(an[X]<<1)+toFIXED(352);
			PointL[6][Y]	=(an[Y]>>16)+112;
			PointL[6][Z]	=0;
		} else {
			PointL[6][Y]	=512;
			PointL[6][Z]	=1;
		}


		point_pos[X]	=toFIXED(2000.0);
		point_pos[Y]	=toFIXED(-1900.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[7][X]	=(an[X]<<1)+toFIXED(352);
			PointL[7][Y]	=(an[Y]>>16)+112;
			PointL[7][Z]	=0;
		} else {
			PointL[7][Y]	=512;
			PointL[7][Z]	=1;
		}

		point_pos[X]	=toFIXED(2000.0);
		point_pos[Y]	=toFIXED(-2000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[8][X]	=(an[X]<<1)+toFIXED(352);
			PointL[8][Y]	=(an[Y]>>16)+112;
			PointL[8][Z]	=0;
		} else {
			PointL[8][Y]	=512;
			PointL[8][Z]	=1;
		}

		point_pos[X]	=toFIXED(1900.0);
		point_pos[Y]	=toFIXED(-2000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[9][X]	=(an[X]<<1)+toFIXED(352);
			PointL[9][Y]	=(an[Y]>>16)+112;
			PointL[9][Z]	=0;
		} else {
			PointL[9][Y]	=512;
			PointL[9][Z]	=1;
		}

		point_pos[X]	=toFIXED(1000.0);
		point_pos[Y]	=toFIXED(-2000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[10][X]	=(an[X]<<1)+toFIXED(352);
			PointL[10][Y]	=(an[Y]>>16)+112;
			PointL[10][Z]	=0;
		} else {
			PointL[10][Y]	=512;
			PointL[10][Z]	=1;
		}

		point_pos[X]	=toFIXED(100.0);
		point_pos[Y]	=toFIXED(-2000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[11][X]	=(an[X]<<1)+toFIXED(352);
			PointL[11][Y]	=(an[Y]>>16)+112;
			PointL[11][Z]	=0;
		} else {
			PointL[11][Y]	=512;
			PointL[11][Z]	=1;
		}

		point_pos[X]	=toFIXED(0.0);
		point_pos[Y]	=toFIXED(-2000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[12][X]	=(an[X]<<1)+toFIXED(352);
			PointL[12][Y]	=(an[Y]>>16)+112;
			PointL[12][Z]	=0;
		} else {
			PointL[12][Y]	=512;
			PointL[12][Z]	=1;
		}

		point_pos[X]	=toFIXED(0.0);
		point_pos[Y]	=toFIXED(-1900.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[13][X]	=(an[X]<<1)+toFIXED(352);
			PointL[13][Y]	=(an[Y]>>16)+112;
			PointL[13][Z]	=0;
		} else {
			PointL[13][Y]	=512;
			PointL[13][Z]	=1;
		}

		point_pos[X]	=toFIXED(0.0);
		point_pos[Y]	=toFIXED(-1000.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[14][X]	=(an[X]<<1)+toFIXED(352);
			PointL[14][Y]	=(an[Y]>>16)+112;
			PointL[14][Z]	=0;
		} else {
			PointL[14][Y]	=512;
			PointL[14][Z]	=1;
		}

		point_pos[X]	=toFIXED(0.0);
		point_pos[Y]	=toFIXED(-100.0);
		if(slConvert3Dto2DFX(point_pos,an)>toFIXED(100.0))
		{
			PointL[15][X]	=(an[X]<<1)+toFIXED(352);
			PointL[15][Y]	=(an[Y]>>16)+112;
			PointL[15][Z]	=0;
		} else {
			PointL[15][Y]	=512;
			PointL[15][Z]	=1;
		}
	}
	slPopMatrix();
	Po_MAX	=0;
	if(PointL[0     ][Y] < PointL[1][Y])Po_MAX=1;/**/
	if(PointL[Po_MAX][Y] < PointL[2][Y])Po_MAX=2;/**/
	if(PointL[Po_MAX][Y] < PointL[3][Y])Po_MAX=3;
	if(PointL[Po_MAX][Y] < PointL[4][Y])Po_MAX=4;/**/
	if(PointL[Po_MAX][Y] < PointL[5][Y])Po_MAX=5;/**/
	if(PointL[Po_MAX][Y] < PointL[6][Y])Po_MAX=6;
	if(PointL[Po_MAX][Y] < PointL[7][Y])Po_MAX=7;/**/
	if(PointL[Po_MAX][Y] < PointL[8][Y])Po_MAX=8;/**/
	if(PointL[Po_MAX][Y] < PointL[9][Y])Po_MAX=9;
	if(PointL[Po_MAX][Y] < PointL[10][Y])Po_MAX=10;/**/
	if(PointL[Po_MAX][Y] < PointL[11][Y])Po_MAX=11;/**/
	if(PointL[Po_MAX][Y] < PointL[12][Y])Po_MAX=12;/**/
	if(PointL[Po_MAX][Y] < PointL[13][Y])Po_MAX=13;
	if(PointL[Po_MAX][Y] < PointL[14][Y])Po_MAX=14;/**/
	if(PointL[Po_MAX][Y] < PointL[15][Y])Po_MAX=15;/**/
	Po_MIN	=0;
	if(PointL[0     ][Y] > PointL[1][Y])Po_MIN=1;/**/
	if(PointL[Po_MIN][Y] > PointL[2][Y])Po_MIN=2;/**/
	if(PointL[Po_MIN][Y] > PointL[3][Y])Po_MIN=3;
	if(PointL[Po_MIN][Y] > PointL[4][Y])Po_MIN=4;/**/
	if(PointL[Po_MIN][Y] > PointL[5][Y])Po_MIN=5;/**/
	if(PointL[Po_MIN][Y] > PointL[6][Y])Po_MIN=6;
	if(PointL[Po_MIN][Y] > PointL[7][Y])Po_MIN=7;/**/
	if(PointL[Po_MIN][Y] > PointL[8][Y])Po_MIN=8;/**/
	if(PointL[Po_MIN][Y] > PointL[9][Y])Po_MIN=9;
	if(PointL[Po_MIN][Y] > PointL[10][Y])Po_MIN=10;/**/
	if(PointL[Po_MIN][Y] > PointL[11][Y])Po_MIN=11;/**/
	if(PointL[Po_MIN][Y] > PointL[12][Y])Po_MIN=12;/**/
	if(PointL[Po_MIN][Y] > PointL[13][Y])Po_MIN=13;
	if(PointL[Po_MIN][Y] > PointL[14][Y])Po_MIN=14;/**/
	if(PointL[Po_MIN][Y] > PointL[15][Y])Po_MIN=15;/**/
	PoyMAX	=PointL[Po_MAX][Y];
	PoyMIN	=PointL[Po_MIN][Y];

/*	if(Po_MAX < 0 || Po_MIN > 223)*/

	Po_LINE	=0;

	while(Po_LINE < PoyMIN)
	{
		LINE_T[Po_LINE]=0;
		Po_LINE++;
	}
	POS_X1	=PointL[Po_MIN][X];
	POS_X2	=PointL[Po_MIN][X];
	PO1	=Po_MIN-1;
	PO2	=Po_MIN+1;
	if(PO1 == -1)PO1=15;
	if(PO2 == 16)PO2=0;
	PO1Y	=PointL[PO1][Y];
	PO2Y	=PointL[PO2][Y];
	POS_ADDX1	=(PointL[PO1][X]-PointL[Po_MIN][X])/
				(PointL[PO1][Y]-PointL[Po_MIN][Y]);
	POS_ADDX2	=(PointL[PO2][X]-PointL[Po_MIN][X])/
				(PointL[PO2][Y]-PointL[Po_MIN][Y]);
	while(Po_LINE != 224 && Po_LINE != PoyMAX)
	{
		POS_X1	+=POS_ADDX1;
		POS_X2	+=POS_ADDX2;
		pos_x1	=POS_X2>>16;
		pos_x2	=POS_X1>>16;
		if(pos_x1 > 352*2 || pos_x2 < 0)
		{
			pos_x1	=0;
			pos_x2	=0;
		} else {
			if(pos_x1 < 0)pos_x1 = 0;
			if(pos_x2 > 352*2 )pos_x2=352*2;
		}
		LINE_T[Po_LINE]=(pos_x1<<16)+pos_x2;
		Po_LINE++;
		if(Po_LINE >= PO1Y)
		{
			POU	=PO1;
			if(--PO1 == -1)PO1=15;
			if(PointL[PO1][Z] ==0)POS_ADDX1=(PointL[PO1][X]-PointL[POU][X])/
						(PointL[PO1][Y]-PointL[POU][Y]);
			PO1Y	=PointL[PO1][Y];
			POS_X1	=PointL[POU][X];
		}
		if(Po_LINE >= PO2Y)
		{
			POU	=PO2;
			if(++PO2 == 16)PO2=0;
			if(PointL[PO2][Z] ==0)POS_ADDX2=(PointL[PO2][X]-PointL[POU][X])/
						(PointL[PO2][Y]-PointL[POU][Y]);
			POS_X2	=PointL[POU][X];
			PO2Y	=PointL[PO2][Y];
		}
	}
	while(Po_LINE < 224)
	{
		LINE_T[Po_LINE]=0;
		Po_LINE++;
	}
}
/*--------------------------------------*/
/*	回転ＢＧの表示関係					*/
/*--------------------------------------*/
void	main_map(EVENT *evptr)
{
	slScrPosNbg2(-((FIXED)ang[Z]<<11)&0x01ffffff,toFIXED(45.0));
	slPushMatrix();
	{
		set_matrix(pos2, ang2, scl2);
		slRotZ(-DEGtoANG(90.0));
		slCurRpara(RA);
		slScrMatConv();
		slScrMatSet();
	}
	slPopMatrix();
	slPushMatrix();
	{
		slRotX(ang2[X]);
		slRotY(ang2[Y]);
		slRotZ(ang2[Z]);
		slTranslate(pos2[X], pos2[Y], pos2[Z]+toFIXED(200.0));
		slScale(scl2[X], scl2[Y], scl2[Z]);
		slRotZ(-DEGtoANG(90.0));
		slCurRpara(RB);
		slScrMatConv();
		slScrMatSet();
	}
	slPopMatrix();
	/*--------------------------------------*/
	pos2[X]	=pos[X];
	pos2[Y]	=pos[Y];
	pos2[Z]	=pos[Z];
	scl2[X]	=scl[X];
	scl2[Y]	=scl[Y];
	scl2[Z]	=scl[Z];
	ang2[X]	=ang[X];
	ang2[Y]	=ang[Y];
	ang2[Z]	=ang[Z];
	/*--------------------------------------*/
	if(LINE_TE == 0)
	{
		slDMACopy((Uint16 *)LINE_TE2,(void *)LINE_SCR_TABL,(Uint32)448*2);
		LINE_TE	=1;
	} else {
		slDMACopy((Uint16 *)LINE_TE1,(void *)LINE_SCR_TABL,(Uint32)448*2);
		LINE_TE	=0;
	}
}
/*--------------------------------------*/
/*	パッド関係							*/
/*--------------------------------------*/
void	main_pad(EVENT *evptr)
{
	
	ANGLE	kk_ang;
	FIXED	aa,bb,ax,ay;
	
kk_ang	=slAtan(robo1.posX-robo2.posX,robo1.posY-robo2.posY)+DEGtoANG(90.0);
	robo1.ang	=kk_ang;
	robo2.ang	=kk_ang+DEGtoANG(180.0);
	
	ax	=((-(robo1.posX+robo2.posX)>>1)-pos[X])>>4;
	ay	=((-(robo1.posY+robo2.posY)>>1)-pos[Y])>>4;
	pos[X]	+=ax;
	pos[Y]	+=ay;
	kk_ang	=(-kk_ang+DEGtoANG(90.0)-ang[Z]);
	ang[Z]	+=(kk_ang)>>4;

	ax	=(robo1.posX-robo2.posX)>>16;
	ay	=(robo1.posY-robo2.posY)>>16;
	bb	=aa	=-slSquartFX(ax*ax+ay*ay)*200;
	if(aa > -toFIXED(200.0))aa=-toFIXED(200.0);
	sega3D.cpos[Z]	=aa;

	pad_ck(&robo1,bb,1,Smpc_Peripheral[ 0].data);
	pad_ck(&robo2,bb,2,Smpc_Peripheral[15].data);
}
void	main_obj(EVENT *evptr)
{
	FIXED	ax,ay;
	FIXED	bx,by;
	Sint32	aa,object_xx;
	Sint32	suu;

	object_xx	=obj_xx;
	if(object_xx != 0)
	{
		slPushMatrix();
		{
			set_matrix(pos, ang, scl);
			suu	=0;
			aa	=0;
			while(aa != object_xx)
			{
				if(obj[suu].no != 0)
				{
					aa++;
					slPushMatrix();
					{
						slTranslate(obj[suu].X,obj[suu].Y,toFIXED(-60.0));
						slRotX(m_ang);
						if(obj[suu].no == 1)
						{
							slPutPolygon(&PD_BOX);
							ax	=robo2.posX-obj[suu].X>>16;
							ay	=robo2.posY-obj[suu].Y>>16;
							if((ax*ax+ay*ay) < 640 && robo2.xx < 3)
							{
								obj[suu].no		=0; 
								robo2.posX_add	=obj[suu].X_add*2;
								robo2.posY_add	=obj[suu].Y_add*2;
								robo2.sc		=robo2.sc+toFIXED(1.0);
							}
						} else {
							slPutPolygon(&PD_box2);
							ax	=robo1.posX-obj[suu].X>>16;
							ay	=robo1.posY-obj[suu].Y>>16;
							if((ax*ax+ay*ay) < 640 && robo1.xx < 3)
							{
								obj[suu].no=0; 
								robo1.posX_add	=obj[suu].X_add*2;
								robo1.posY_add	=obj[suu].Y_add*2;
								robo1.sc		=robo1.sc+toFIXED(1.0);
							}
						}
						obj[suu].X	+=obj[suu].X_add;
						obj[suu].Y	+=obj[suu].Y_add;
						obj[suu].X_add	+=ax<<5;
						obj[suu].Y_add	+=ay<<5;
						if(obj[suu].X < toFIXED(80.0) || obj[suu].X > toFIXED(2018))obj[suu].no	=0;
						if(obj[suu].Y > toFIXED(-80.0) || obj[suu].Y < toFIXED(-2018))obj[suu].no	=0;
					}
					slPopMatrix();
				}
				suu++;
				if(suu == 39)aa=object_xx;
			}
		}
		slPopMatrix();
	}
}
void	obj_set(Sint32 kk,FIXED ax,FIXED ay,FIXED axx,FIXED ayy)
{
	Sint32 aa;
	aa	=0;
	while(obj[aa].no != 0)aa++;

	if(aa < 39)
	{
		obj[aa].no		=kk;
		obj[aa].X		=ax;
		obj[aa].Y		=ay;
		obj[aa].X_add	=axx;
		obj[aa].Y_add	=ayy;
		obj_xx++;
	}
}
void	robo_put(robo_xx *mmm,XPDATA *pd_da)
{
	FIXED	my_x,my_y,my_z;
	ANGLE	my_ang;
	Sint32	*pd_data;

	my_x	=mmm->posX;
	my_y	=mmm->posY;
	my_z	=mmm->posZ;
	my_ang	=mmm->ang;

	pd_data	=(Sint32 *)pd_da;
	slPushMatrix();
	{
		set_matrix(pos, ang, scl);
		slTranslate(my_x,my_y,toFIXED(0.0));
		slRotZ(my_ang);
		slPutPolygon(&PD_kage);
		slTranslate(toFIXED(0.0),toFIXED(0.0),my_z);
		slPutPolygonX((XPDATA *)pd_data[0],(FIXED *)light_d);
		slPushMatrix();
		{
			slTranslate(toFIXED(32.0),toFIXED(0.0),toFIXED(-60.0));
			slRotX(DEGtoANG(90.0));
			slRotY(DEGtoANG(-90.0));
			slRotZ(m_ang);
			slPutPolygonX((XPDATA *)pd_data[1],(FIXED *)light_d);
			slPushMatrix();
			{
				slTranslate(toFIXED(0.0),toFIXED(35.0),toFIXED(0.0));
				slRotZ(slSin(m_ang)>>3);
				slPutPolygonX((XPDATA *)pd_data[2],(FIXED *)light_d);
			}
			slPopMatrix();
		}
		slPopMatrix();
		slPushMatrix();
		{
			slTranslate(toFIXED(-32.0),toFIXED(0.0),toFIXED(-60.0));
			slRotX(DEGtoANG(90.0));
			slRotY(DEGtoANG(90.0));
			slRotZ(slSin(m_ang)>>3);
			slPutPolygonX((XPDATA *)pd_data[3],(FIXED *)light_d);
			slPushMatrix();
			{
				slTranslate(toFIXED(0.0),toFIXED(35.0),toFIXED(0.0));
				slRotZ((slSin(m_ang)>>3)-DEGtoANG(30.0));
				slPutPolygonX((XPDATA *)pd_data[4],(FIXED *)light_d);
			}
			slPopMatrix();
		}
		slPopMatrix();
		slPushMatrix();
		{
			slTranslate(toFIXED(0.0),toFIXED(0.0),toFIXED(-70.0));
			slRotZ((slCos(m_ang)>>4));
			slPutPolygonX((XPDATA *)pd_data[5],(FIXED *)light_d);
		}
		slPopMatrix();
		slPushMatrix();
		{
			slTranslate(toFIXED(16.0),toFIXED(0.0),toFIXED(-5.0));
			slRotX(slSin(m_ang)>>3);
			slPutPolygonX((XPDATA *)pd_data[6],(FIXED *)light_d);
			slPushMatrix();
			{
				slTranslate(toFIXED(0.0),toFIXED(0.0),toFIXED(36.0));
				slRotX((slSin(m_ang)>>3)+DEGtoANG(30.0));
				slPutPolygonX((XPDATA *)pd_data[8],(FIXED *)light_d);
			}
			slPopMatrix();
		}
		slPopMatrix();
		slPushMatrix();
		{
			slTranslate(toFIXED(-16.0),toFIXED(0.0),toFIXED(-5.0));
			slRotX(slSin(-m_ang)>>3);
			slPutPolygonX((XPDATA *)pd_data[7],(FIXED *)light_d);
			slPushMatrix();
			{
				slTranslate(toFIXED(0.0),toFIXED(0.0),toFIXED(36.0));
				slRotX((-slSin(m_ang)>>3)+DEGtoANG(30.0));
				slPutPolygonX((XPDATA *)pd_data[9],(FIXED *)light_d);
			}
			slPopMatrix();
		}
		slPopMatrix();
	}
	slPopMatrix();
}
void	pad_ck(robo_xx *robo,FIXED bb,Uint32 nn,Uint16 data)
{
	Uint16	data_xx,dd,ee;
	Uint16	PER_data1,PER_data2;
	Uint16	PER_data3,PER_data4;

	if(nn == 1)	{
#ifdef FLASH_SATURN_CD
		if((data & (PER_DGT_ST | PER_DGT_TA | PER_DGT_TB | PER_DGT_TC)) == 0){
			SYS_Exit(0);
		}
#endif
		PER_data1	=PER_DGT_KR;		/*パッド１*/
		PER_data2	=PER_DGT_KL;
		PER_data3	=PER_DGT_KD;
		PER_data4	=PER_DGT_KU;
	} else {
		PER_data1	=PER_DGT_KL;		/*パッド２*/
		PER_data2	=PER_DGT_KR;
		PER_data3	=PER_DGT_KU;
		PER_data4	=PER_DGT_KD;
	}
/*=======================================================================*/
/*	コマンド入力			*/
	data_xx	=data & (PER_DGT_KR | PER_DGT_KL | PER_DGT_KU | PER_DGT_KD);
	if(data_xx != robo->data)
	{
		dd	=0;
		ee	=data_xx & robo->data;
		if((ee & PER_data2) == 0 && robo->bot1 != PER_DGT_KL)dd	=PER_DGT_KL;
		if((ee & PER_data1) == 0 && robo->bot1 != PER_DGT_KR)dd	=PER_DGT_KR;
		if((ee & PER_data3) == 0 && robo->bot1 != PER_data3 )dd	=PER_data3;
		if((ee & PER_data4) == 0 && robo->bot1 != PER_data4 )dd	=PER_data4;
		robo->data	=data_xx;
		if(dd != 0 && dd != robo->bot1)
		{
			robo->bot3	=robo->bot2;
			robo->bot2	=robo->bot1;
			robo->bot1	=dd;
		}
	}
/*	slPrintFX(robo->bot1, slLocate( 3,20+nn));
	slPrintFX(robo->bot2, slLocate(13,20+nn));
	slPrintFX(robo->bot3, slLocate(23,20+nn));
/*=======================================================================*/
	if((data & PER_DGT_ST) == 0)
	{
		robo1.sc	=0;
		robo2.sc	=0;
	}
	if((data & PER_data1) == 0)
	{
		robo->posX_add		+=game_speed*slSin(robo->ang);
		robo->posY_add		-=game_speed*slCos(robo->ang);
	}
	if((data & PER_data2) == 0)
	{
		if(bb < -toFIXED(60.0))
		{
			robo->posX_add		-=game_speed*slSin(robo->ang);
			robo->posY_add		+=game_speed*slCos(robo->ang);
		}
	}
	if(bb > -toFIXED(60.0))
	{
		robo->posX_add		= 8*game_speed*slSin(robo->ang);
		robo->posY_add		=-8*game_speed*slCos(robo->ang);
	}
	if((data & PER_data3) == 0)
	{
		robo->posX_add		+=game_speed*slCos(robo->ang);
		robo->posY_add		+=game_speed*slSin(robo->ang);
	}
	if((data & PER_data4) == 0)
	{
		robo->posX_add		-=game_speed*slCos(robo->ang);
		robo->posY_add		-=game_speed*slSin(robo->ang);
	}
	if((data & PER_DGT_TB) == 0)
	{
		if(robo->xx < 8)robo->posZ_add	-=toFIXED(2.0);
		robo->xx++;
	} else {
		if(robo->xx > 0)robo->xx=21;
	}
	robo->posX		+=robo->posX_add;
	robo->posY		+=robo->posY_add;
	robo->posZ		+=robo->posZ_add;
	robo->posX_add	=robo->posX_add*9/10;
	robo->posY_add	=robo->posY_add*9/10;
	if(robo->posX < toFIXED(80.0))
	{
		robo->posX		=toFIXED(80.0);
		robo->posX_add	=-robo->posX_add;
	}
	if(robo->posX > toFIXED(1960.0))
	{
		robo->posX		=toFIXED(1960.0);
		robo->posX_add	=-robo->posX_add;
	}
	if(robo->posY > -toFIXED(80.0))
	{
		robo->posY		=-toFIXED(80.0);
		robo->posY_add	=-robo->posY_add;
	}
	if(robo->posY < -toFIXED(1960.0))
	{
		robo->posY		=-toFIXED(1960.0);
		robo->posY_add	=-robo->posY_add;
	}
	if(robo->posZ > toFIXED(-60.0))
	{
		robo->posZ		=toFIXED(-60.0);
		robo->xx			=0;
		robo->posZ_add	=-(robo->posZ_add/4);
	}
	if(robo->posZ < toFIXED(-60.0))robo->posZ_add	+=toFIXED(0.5);
	if((data & PER_DGT_TA ) == 0 || (data & PER_DGT_TC) == 0)
	{
		if(robo->miss == 0 && robo->xx == 0)
		{
			if((robo->bot1 == PER_DGT_KL) &&
				(robo->bot2	== PER_DGT_KD) && (robo->bot3	== PER_DGT_KR))
			{
				obj_set(nn,robo->posX,robo->posY,
						-(FIXED)4*slSin(robo->ang),(FIXED)4*slCos(robo->ang));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)4*slSin(robo->ang+DEGtoANG( 30.0)),
					 (FIXED)4*slCos(robo->ang+DEGtoANG( 30.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)6*slSin(robo->ang+DEGtoANG( 60.0)),
					 (FIXED)6*slCos(robo->ang+DEGtoANG( 60.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)8*slSin(robo->ang+DEGtoANG( 90.0)),
					 (FIXED)8*slCos(robo->ang+DEGtoANG( 90.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)10*slSin(robo->ang+DEGtoANG(120.0)),
					 (FIXED)10*slCos(robo->ang+DEGtoANG(120.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)12*slSin(robo->ang+DEGtoANG(150.0)),
					 (FIXED)12*slCos(robo->ang+DEGtoANG(150.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)14*slSin(robo->ang+DEGtoANG(180.0)),
					 (FIXED)14*slCos(robo->ang+DEGtoANG(180.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)12*slSin(robo->ang+DEGtoANG(210.0)),
					 (FIXED)12*slCos(robo->ang+DEGtoANG(210.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)10*slSin(robo->ang+DEGtoANG(240.0)),
					 (FIXED)10*slCos(robo->ang+DEGtoANG(240.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)8*slSin(robo->ang+DEGtoANG(270.0)),
					 (FIXED)8*slCos(robo->ang+DEGtoANG(270.0)));
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)6*slSin(robo->ang+DEGtoANG(300.0)),
					 (FIXED)6*slCos(robo->ang+DEGtoANG(300.0)));
			} else {
				obj_set(nn,robo->posX,robo->posY,
					-(FIXED)4*slSin(robo->ang),(FIXED)4*slCos(robo->ang));
			}
			robo->bot1	=0;
			robo->bot2	=0;
			robo->bot3	=0;
		}
		robo->miss	=1;
	} else {
		robo->miss	=0;
	}
}
