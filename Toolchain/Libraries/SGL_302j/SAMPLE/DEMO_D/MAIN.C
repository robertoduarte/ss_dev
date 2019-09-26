#include	"sgl.h"
#include	"ss_scrol.h"

#define		RBG0RB_CEL_ADR			(VDP2_VRAM_A0            )
#define		RBG0RB_MAP_ADR			(VDP2_VRAM_B0            )
#define		RBG0RB_COL_ADR			(VDP2_COLRAM    + 0x00200)
#define		RBG0RA_CEL_ADR			(RBG0RB_CEL_ADR + 0x06e80)
#define		RBG0RA_MAP_ADR			(RBG0RB_MAP_ADR + 0x02000)
#define		RBG0RA_COL_ADR			(RBG0RB_COL_ADR + 0x00200)
#define		RBG0_KTB_ADR			(VDP2_VRAM_A1            )
#define		RBG0_PRA_ADR			(VDP2_VRAM_A1   + 0x1fe00)
#define		RBG0_PRB_ADR			(RBG0_PRA_ADR   + 0x80   )
#define		BACK_COL_ADR			(VDP2_VRAM_A1   + 0x1fffe)

extern	PDATA PD_CUBE;

extern	void get_pad2(FIXED * , ANGLE * , FIXED * , ANGLE *);

MATRIX poly_mt;

static	ANGLE	pang[XYZ] = {DEGtoANG(0.0) , DEGtoANG(0.0) , DEGtoANG(0.0)};
static	FIXED	ppos[XYZ] = {toFIXED(0.0) , toFIXED(0.0) , toFIXED(400.0)};
static	FIXED	pscl[XYZ] = {toFIXED(1.0) , toFIXED(1.0) , toFIXED(  1.0)};
static	ANGLE	kang[XYZ];
static	ANGLE	wang[XYZ];
static	FIXED	wpos[XYZ];
static	FIXED	wscl[XYZ];

int	check_pad_change(FIXED ppos[XYZ],ANGLE pang[XYZ],FIXED pscl[XYZ],ANGLE kang[XYZ])
{
    static	ANGLE	old_pang[XYZ];
    static	FIXED	old_ppos[XYZ];
    static	FIXED	old_pscl[XYZ];
    static	ANGLE	old_kang[XYZ];
    static	int	fast=1;
    int		code = 0;
    int		i;

    if ( fast )
      {
	  fast = 0;
	  code = 1;
	  memcpy( old_pang, pang, sizeof(ANGLE)*XYZ );
	  memcpy( old_ppos, ppos, sizeof(FIXED)*XYZ );
	  memcpy( old_pscl, pscl, sizeof(FIXED)*XYZ );
	  memcpy( old_kang, kang, sizeof(ANGLE)*XYZ );
      }
    else
      {
	  for( i=0; i<XYZ; i++ )
	    {
		if ( (old_pang[i] != pang[i]) ||
		     (old_ppos[i] != ppos[i]) ||
		     (old_pscl[i] != pscl[i]) ||
		     (old_kang[i] != kang[i])    )
		  {
		      code = 1;
		      memcpy( old_pang, pang, sizeof(ANGLE)*XYZ );
		      memcpy( old_ppos, ppos, sizeof(FIXED)*XYZ );
		      memcpy( old_pscl, pscl, sizeof(FIXED)*XYZ );
		      memcpy( old_kang, kang, sizeof(ANGLE)*XYZ );
		      break;
		  }
	    }
      }

    return code;
}

void init_scroll()
{
	slColRAMMode(CRM16_1024);

	slRparaInitSet((void*)RBG0_PRA_ADR);
	slMakeKtable((void*)RBG0_KTB_ADR);
	slCharRbg0(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageRbg0((void*)RBG0RB_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);
	slPlaneRA(PL_SIZE_1x1);
	sl1MapRA((void*)RBG0RA_MAP_ADR);
	slOverRA(0);
	slKtableRA((void*)RBG0_KTB_ADR , K_FIX | K_DOT | K_2WORD | K_ON);
	Cel2VRAM(tuti_cel , (void*)RBG0RA_CEL_ADR , 65536);
	Map2VRAM(tuti_map , (void*)RBG0RA_MAP_ADR , 64 , 64 , 2 , 884);
	Pal2CRAM(tuti_pal , (void*)RBG0RA_COL_ADR , 160);

	slPlaneRB(PL_SIZE_1x1);
	sl1MapRB((void*)RBG0RB_MAP_ADR);
	slOverRB(0);
	slKtableRB((void*)RBG0_KTB_ADR , K_FIX | K_DOT | K_2WORD | K_ON);
	Cel2VRAM(sora_cel , (void*)RBG0RB_CEL_ADR , 28288);
	Map2VRAM(sora_map , (void*)RBG0RB_MAP_ADR , 64 , 20 , 1 , 0);
	Pal2CRAM(sora_pal , (void*)RBG0RB_COL_ADR , 256);

	slRparaMode(K_CHANGE);

	slBack1ColSet((void*)BACK_COL_ADR , 0);

	slScrAutoDisp(NBG0ON | RBG0ON);
	slTVOn();
}


void get_limit()
{
	if(ppos[X]<toFIXED(-150.0)) ppos[X] = toFIXED(-150.0);
	else if(ppos[X]>toFIXED(150.0)) ppos[X] = toFIXED(150.0);
	if(ppos[Y]<toFIXED(-120.0)) ppos[Y] = toFIXED(-120.0);
	else if(ppos[Y]>toFIXED(100.0)) ppos[Y] = toFIXED(100.0);
	if(ppos[Z]<toFIXED(300.0)) ppos[Z] = toFIXED(300.0);
	else if(ppos[Z]>toFIXED(600.0)) ppos[Z] = toFIXED(600.0);
	if(DEGtoANG(-80.0)<=(pang[X]+kang[X]) &&
		(pang[X]+kang[X])<=DEGtoANG(80.0)) pang[X] += kang[X];
	else kang[X] = DEGtoANG(0.0);
	if(DEGtoANG(-80.0)<=(pang[Y]+kang[Y]) &&
		(pang[Y]+kang[Y])<=DEGtoANG(80.0)) pang[Y] += kang[Y];
	else kang[Y] = DEGtoANG(0.0);
	if(DEGtoANG(-80.0)<=(pang[Z]+kang[Z]) &&
		(pang[Z]+kang[Z])<=DEGtoANG(80.0)) pang[Z] += kang[Z];
	else kang[Z] = DEGtoANG(0.0);
	if(pscl[X] < toFIXED(0.5)) pscl[X] = toFIXED(0.5);
	else if(pscl[X] > toFIXED(2.0)) pscl[X] = toFIXED(2.0);
	if(pscl[Y] < toFIXED(0.5)) pscl[Y] = toFIXED(0.5);
	else if(pscl[Y] > toFIXED(2.0)) pscl[Y] = toFIXED(2.0);
	if(pscl[Z] < toFIXED(0.5)) pscl[Z] = toFIXED(0.5);
	else if(pscl[Z] > toFIXED(2.0)) pscl[Z] = toFIXED(2.0);
}


void disp_poly()
{
	wpos[X] = ppos[X] / 2.0;
	wpos[Y] = ppos[Y] / 2.0;
	wpos[Z] = ppos[Z] / 2.0;
	wang[X] = kang[X] / 2.0;
	wang[Y] = kang[Y] / 2.0;
	wang[Z] = kang[Z] / 4.0;
	if ( check_pad_change(ppos , pang , pscl , kang) )
	  {
	      slPushMatrix();
	      {
		  slLoadMatrix(poly_mt);
		  slInversMatrix();
		  slRotZ(wang[Z]);
		  slRotY(wang[Y]);
		  slRotX(wang[X]);
		  slInversMatrix();
		  slGetMatrix(poly_mt);
	      }
	      slPopMatrix();
	  }
	slPushMatrix();
	{
		slTranslate(toFIXED(0.0)+wpos[X] , toFIXED(0.0)+wpos[Y] , toFIXED(0.0)+wpos[Z]);
		slMultiMatrix(poly_mt);
		slScale(pscl[X] , pscl[Y] , pscl[Z]);
		wscl[X] = wscl[Y] = toFIXED(0.0);
		wscl[Z] = slDivFX(pscl[Z] , toFIXED(1.0));
		slLight(wscl);
		slPutPolygon(&PD_CUBE);
	}
	slPopMatrix();
}


void disp_scroll()
{
	/* tuti */
	wpos[X] = ppos[X] / 2.0;
	wpos[Y] = -(ppos[Z] / 2.0);
	wpos[Z] = -(ppos[Y] / 2.0 + pang[X] * 64.0);
	wang[X] =		ppos[Y]/ 16384.0 - (pang[X]/64.0);
	wang[Y] = pang[Y] / 16.0;
	wang[Z] = - (pang[Z] / 20.0);
	slCurRpara(RA);
	slUnitMatrix(CURRENT);
	slTranslate(toFIXED(0.0)+wpos[X] , toFIXED(0.0)+wpos[Y] , toFIXED(100.0)+wpos[Z]);
	slRotX(DEGtoANG(-90.0)+wang[X]);
	slRotY(wang[Y]);
	slRotZ(wang[Z]);
	slScrMatSet();

	/* sora */
	wpos[X] = ppos[X] / 20.0;
	wpos[Y] = ppos[Y] / 20.0 - pang[X] * 12.0;
	wpos[Z] = ppos[Z] / 5.0;
	wang[Y] = pang[Y] / 20.0;
	wang[Z] = - (pang[Z] / 20.0);
	slCurRpara(RB);
	slUnitMatrix(CURRENT);
	slTranslate(toFIXED(160.0)+wpos[X] , toFIXED(150.0)+wpos[Y] , toFIXED(100.0)+wpos[Z]);
	slRotY(wang[Y]);
	slRotZ(wang[Z]);
	slScrMatSet();
}

void ss_main()
{
	slInitSystem(TV_320x224,NULL,1);
	slTVOff();
	slPrint("demo D" , slLocate(6,2));
	slUnitMatrix(poly_mt);
	init_scroll();
	while(1) {
		get_pad2(ppos , pang , pscl , kang);
		get_limit();
		slUnitMatrix(CURRENT);
		disp_poly();
		disp_scroll();
		slSynch();
	} 
}

