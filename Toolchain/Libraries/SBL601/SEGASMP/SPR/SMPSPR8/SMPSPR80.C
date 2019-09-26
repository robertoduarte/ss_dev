/*----------------------------------------------------------------------------
 *  smpspr80.c -- スプライトサンプル１
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1995-03-15 Ver.1.20
 *  Updated by H.E on 1995-03-15 Ver.1.20
 *
 *  スプライト３Ｄテスト表示。
 *  ３階層クラスタモデルの回転移動表示をする。
 *
 *----------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>

#include	<machine.h>
#define		_SPR3_
#include        "sega_spr.h"
#include        "sega_scl.h"
#include        "sega_dbg.h"
#include        "sega_sys.h"
#include        "sega_tim.h"
#include	"../../v_blank/v_blank.h"

/* スプライトシステムワークエリア定義 */
#define CommandMax    2000
#define GourTblMax    200
#define LookupTblMax  100
#define CharMax       16
#define DrawPrtyMax   256
SPR_2DefineWork(work2D, CommandMax, GourTblMax, LookupTblMax,
						  CharMax, DrawPrtyMax)

#define ObjSurfMax     100
#define ObjVertMax     100
SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)

/* ３Ｄモデルの定義 */
#include	"model.dat"

#define   MTH_FIXED_MIN        0x80000000
#define   MTH_FIXED_MAX        0x7fffffff
#define   CLIP_MIN_Z       MTH_FIXED_MIN
#define   CLIP_MAX_Z       MTH_FIXED(0.0) /* -0.005 */
#define   ZSORT_MIN_Z      MTH_FIXED(-10.0)
#define   ZSORT_MAX_Z      MTH_FIXED(0)

#define   DISP_XSIZE       352
#define   DISP_YSIZE       240

			      /*    X            Y             Z      */
static MthXyz oLightAngle = {MTH_FIXED(0.0),  MTH_FIXED(180.0),MTH_FIXED(0.0)};
static MthXyz oVeiwPoint  = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(2.5)};
static MthXyz oVeiwAngle  = {MTH_FIXED(0.0),  MTH_FIXED(180.0),MTH_FIXED(0.0)};
/*	95-7-26	unuse
static MthXyz oModelPoint = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(0.0)};
static MthXyz oModelAngle = {MTH_FIXED(0.0),  MTH_FIXED(0.0),  MTH_FIXED(0.0)};
*/

static MthXyz	lightAngle;
static MthXyz	viewPoint;
static MthXyz	viewAngle;
/* 95-7-26	unuse
static MthXyz	modelPoint;
static MthXyz	modelAngle;
static char     mc;
*/

/* #define TIME_DISP */
#ifdef USE_DEBUG_INFO
extern int  dbgVblCount;
#endif


typedef struct  {
    SprCluster *cluster;
    MthXyz     pos;
    MthXyz     angle;
    MthXyz     speed;
} Obj3D;

#define SPEED  0.5


/* 95-7-26	*/
#if	0
static Obj3D obj3D[] = {
  /* [ 0 ]   */
  /* cluster */ &ClusterKyu0,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(10.0),MTH_FIXED(20.0),MTH_FIXED(30.0)},
  /* speed   */ {MTH_FIXED(0.025*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.023*SPEED)},

  /* [ 1 ]   */
  /* cluster */ &ClusterKyu1,
  /* pos     */ {MTH_FIXED(0.5),MTH_FIXED(0.8),MTH_FIXED(0.8)},
  /* angle   */ {MTH_FIXED(40.0),MTH_FIXED(50.0),MTH_FIXED(60.0)},
  /* speed   */ {MTH_FIXED(0.031*SPEED),MTH_FIXED(0.02*SPEED),
                 MTH_FIXED(-0.02*SPEED)},

  /* [ 2 ]   */
  /* cluster */ &ClusterKyu2,
  /* pos     */ {MTH_FIXED(-0.7),MTH_FIXED(-0.8),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(70.0),MTH_FIXED(80.0),MTH_FIXED(90.0)},
  /* speed   */ {MTH_FIXED(0.033*SPEED),MTH_FIXED(-0.025*SPEED),
                 MTH_FIXED(0.023*SPEED)},

  /* [ 3 ]   */
  /* cluster */ &ClusterKyu3,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.8),MTH_FIXED(-0.7)},
  /* angle   */ {MTH_FIXED(100.0),MTH_FIXED(110.0),MTH_FIXED(120.0)},
  /* speed   */ {MTH_FIXED(0.03*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.027*SPEED)},

  /* [ 4 ]   */
  /* cluster */ &ClusterKyu4,
  /* pos     */ {MTH_FIXED(-0.2),MTH_FIXED(-0.2),MTH_FIXED(0.5)},
  /* angle   */ {MTH_FIXED(130.0),MTH_FIXED(140.0),MTH_FIXED(150.0)},
  /* speed   */ {MTH_FIXED(-0.026*SPEED),MTH_FIXED(-0.018*SPEED),
                 MTH_FIXED(0.029*SPEED)},

  /* [ 5 ]   */
  /* cluster */ &ClusterKyu5,
  /* pos     */ {MTH_FIXED(0.5),MTH_FIXED(0.2),MTH_FIXED(-0.6)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.019*SPEED),MTH_FIXED(-0.026*SPEED),
                 MTH_FIXED(-0.034*SPEED)},

  /* [ 6 ]   */
  /* cluster */ &ClusterKyu6,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.022*SPEED),MTH_FIXED(0.023*SPEED),
                 MTH_FIXED(-0.031*SPEED)},

  /* [ 7 ]   */
  /* cluster */ &Cluster10,
  /* pos     */ {MTH_FIXED(0.8),MTH_FIXED(0.8),MTH_FIXED(-0.7)},
  /* angle   */ {MTH_FIXED(100.0),MTH_FIXED(110.0),MTH_FIXED(120.0)},
  /* speed   */ {MTH_FIXED(0.027*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.027*SPEED)},

  /* [ 8 ]   */
  /* cluster */ &Cluster11,
  /* pos     */ {MTH_FIXED(-0.2),MTH_FIXED(-0.6),MTH_FIXED(0.5)},
  /* angle   */ {MTH_FIXED(130.0),MTH_FIXED(140.0),MTH_FIXED(150.0)},
  /* speed   */ {MTH_FIXED(-0.026*SPEED),MTH_FIXED(-0.021*SPEED),
                 MTH_FIXED(-0.029*SPEED)},

  /* [ 9 ]   */
  /* cluster */ &Cluster12,
  /* pos     */ {MTH_FIXED(0.7),MTH_FIXED(0.5),MTH_FIXED(-0.6)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.022*SPEED),MTH_FIXED(0.026*SPEED),
                 MTH_FIXED(-0.034*SPEED)},

  /* cluster */ 0,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* speed   */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
};

#else
static Obj3D obj3D[] = {
  /* [ 0 ]   */	{
  /* cluster */ &ClusterKyu0,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(10.0),MTH_FIXED(20.0),MTH_FIXED(30.0)},
  /* speed   */ {MTH_FIXED(0.025*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.023*SPEED)},
				},
				
  /* [ 1 ]   */	{
  /* cluster */ &ClusterKyu1,
  /* pos     */ {MTH_FIXED(0.5),MTH_FIXED(0.8),MTH_FIXED(0.8)},
  /* angle   */ {MTH_FIXED(40.0),MTH_FIXED(50.0),MTH_FIXED(60.0)},
  /* speed   */ {MTH_FIXED(0.031*SPEED),MTH_FIXED(0.02*SPEED),
                 MTH_FIXED(-0.02*SPEED)},
				},
				
  /* [ 2 ]   */	{
  /* cluster */ &ClusterKyu2,
  /* pos     */ {MTH_FIXED(-0.7),MTH_FIXED(-0.8),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(70.0),MTH_FIXED(80.0),MTH_FIXED(90.0)},
  /* speed   */ {MTH_FIXED(0.033*SPEED),MTH_FIXED(-0.025*SPEED),
                 MTH_FIXED(0.023*SPEED)},
				},
				
  /* [ 3 ]   */	{
  /* cluster */ &ClusterKyu3,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.8),MTH_FIXED(-0.7)},
  /* angle   */ {MTH_FIXED(100.0),MTH_FIXED(110.0),MTH_FIXED(120.0)},
  /* speed   */ {MTH_FIXED(0.03*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.027*SPEED)},
				},
				
  /* [ 4 ]   */	{
  /* cluster */ &ClusterKyu4,
  /* pos     */ {MTH_FIXED(-0.2),MTH_FIXED(-0.2),MTH_FIXED(0.5)},
  /* angle   */ {MTH_FIXED(130.0),MTH_FIXED(140.0),MTH_FIXED(150.0)},
  /* speed   */ {MTH_FIXED(-0.026*SPEED),MTH_FIXED(-0.018*SPEED),
                 MTH_FIXED(0.029*SPEED)},
				},
				
  /* [ 5 ]   */	{
  /* cluster */ &ClusterKyu5,
  /* pos     */ {MTH_FIXED(0.5),MTH_FIXED(0.2),MTH_FIXED(-0.6)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.019*SPEED),MTH_FIXED(-0.026*SPEED),
                 MTH_FIXED(-0.034*SPEED)},
				},
				
  /* [ 6 ]   */	{
  /* cluster */ &ClusterKyu6,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.022*SPEED),MTH_FIXED(0.023*SPEED),
                 MTH_FIXED(-0.031*SPEED)},
				},
				
  /* [ 7 ]   */	{
  /* cluster */ &Cluster10,
  /* pos     */ {MTH_FIXED(0.8),MTH_FIXED(0.8),MTH_FIXED(-0.7)},
  /* angle   */ {MTH_FIXED(100.0),MTH_FIXED(110.0),MTH_FIXED(120.0)},
  /* speed   */ {MTH_FIXED(0.027*SPEED),MTH_FIXED(0.032*SPEED),
                 MTH_FIXED(-0.027*SPEED)},
				},
				
  /* [ 8 ]   */	{
  /* cluster */ &Cluster11,
  /* pos     */ {MTH_FIXED(-0.2),MTH_FIXED(-0.6),MTH_FIXED(0.5)},
  /* angle   */ {MTH_FIXED(130.0),MTH_FIXED(140.0),MTH_FIXED(150.0)},
  /* speed   */ {MTH_FIXED(-0.026*SPEED),MTH_FIXED(-0.021*SPEED),
                 MTH_FIXED(-0.029*SPEED)},
				},
				
  /* [ 9 ]   */	{
  /* cluster */ &Cluster12,
  /* pos     */ {MTH_FIXED(0.7),MTH_FIXED(0.5),MTH_FIXED(-0.6)},
  /* angle   */ {MTH_FIXED(160.0),MTH_FIXED(170.0),MTH_FIXED(180.0)},
  /* speed   */ {MTH_FIXED(0.022*SPEED),MTH_FIXED(0.026*SPEED),
                 MTH_FIXED(-0.034*SPEED)},
				},
				
				{
  /* cluster */ 0,
  /* pos     */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* angle   */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
  /* speed   */ {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
				},
};
#endif



void moveObj3D(Obj3D *obj3D);


/* main()		95-7-26  +"void"	*/
void main()	 /*	95-7-26				*/
{
XyInt	xy;
/* Uint16  timeCnt1, timeCnt2;	95-7-26 unuse	*/
MthXyz  mAngle, mPoint;

        SYS_CHGSYSCK(1);              /* システムクロックを２８Ｍに切替え  */

        set_imask(0);
        SetVblank();
        SCL_Vdp2Init();
        SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                        SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,6);
        SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);

	SPR_2Initial(&work2D);		/* ２Ｄスプライト表示初期化 */
	SPR_2FrameChgIntr(0);

        SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_B);
        SPR_2SetTvMode(SPR_TV_NORMAL,SPR_TV_352X240,OFF);
        SPR_SetEraseData(RGB16_COLOR(0,0,0), 0, 0, DISP_XSIZE-1, DISP_YSIZE-1);

#ifdef TIME_DISP
        DBG_Initial(&PadData1,RGB16_COLOR(31,31,31),0);
        DBG_DisplayOn();
	DBG_SetCursol(0,0);
        TIM_FRT_INIT(TIM_CKS_128);
        SCL_DisplayFrame();
        SCL_DisplayFrame();
#endif

	SPR_3Initial(&work3D);		/* ３Ｄスプライト表示初期化 */
	SPR_3SetTexture((SprTexture *)&Texture);
					/* ３Ｄ用テクスチャデータのセット */
	SPR_3SetLight(1,&oLightAngle);
	SPR_3SetView(1,&oVeiwPoint,&oVeiwAngle,ROT_SEQ_XYZ,0);
	SPR_3SetZSortMinMax(0,ZSORT_MIN_Z, ZSORT_MAX_Z);
	SPR_3SetClipLevel (2,  CLIP_MIN_Z,  CLIP_MAX_Z);

	mPoint.x  = MTH_FIXED(0.0);
	mPoint.y  = MTH_FIXED(0.0);
	mPoint.z  = MTH_FIXED(0.0);
	for(;;) {
	    mAngle.x = MTH_FIXED(0.0);
	    mAngle.y = MTH_FIXED(0.0);
	    mAngle.z = MTH_FIXED(0.0);
            switch(PadData1) {
	        case PAD_X:
	             mAngle.x = MTH_FIXED(1.0);
		     break;
	        case PAD_Y:
	             mAngle.y = MTH_FIXED(1.0);
                     break;
	        case PAD_Z:
	             mAngle.z = MTH_FIXED(1.0);
                     break;
	        case PAD_L:
	             mPoint.x -= MTH_FIXED(0.05);
                     break;
	        case PAD_R:
	             mPoint.x += MTH_FIXED(0.05);
                     break;
	        case PAD_U:
	             mPoint.z -= MTH_FIXED(0.05);
                     break;
	        case PAD_D:
	             mPoint.z += MTH_FIXED(0.05);
                     break;
            }

	    SPR_2OpenCommand(SPR_2DRAW_PRTY_ON);
	              	          /* スプライトコマンド書き込みのオープン */
	    xy.x = DISP_XSIZE-1; xy.y = DISP_YSIZE-1;
	    SPR_2SysClip(0, &xy);	/* システムクリップエリアコマンド */

	    xy.x = DISP_XSIZE/2; xy.y = DISP_YSIZE/2;
	    SPR_2LocalCoord(0, &xy);

	    SPR_3SetLight(0,&lightAngle);
	    SPR_3SetView(0,&viewPoint,&viewAngle,ROT_SEQ_XYZ,0);

            moveObj3D(obj3D);
	    SPR_3MoveCluster(&RootCluster, 0,&mAngle,0);
	    SPR_3MoveCluster(&RootCluster, 1,0,&mPoint);

#ifdef TIME_DISP
            TIM_FRT_SET_16(0);
#endif
	    SPR_3DrawModel(&RootCluster);	/* ３Ｄモデルの登録 */

#ifdef TIME_DISP
            timeCnt1 = TIM_FRT_GET_16();
#endif
	    SPR_3Flush();		/* ３Ｄスプライトコマンドセット */

#ifdef TIME_DISP
            timeCnt2 = TIM_FRT_GET_16();
#endif

	    SPR_2CloseCommand();     /* スプライトコマンド書き込みのクローズ */

	    SCL_DisplayFrame();    /* Ｖ－ＢＬＡＮＫ割り込みを待ち、       */
				     /* スプライト表示とスクロール動作を行う */
#ifdef TIME_DISP
	    DBG_SetCursol(3,2);
            DBG_Printf("V-Blank Count=%2d  \n", dbgVblCount);
	    DBG_SetCursol(3,3);
            DBG_Printf("Compute Poly =%4d  \n", dbgComputePol);
	    DBG_SetCursol(3,4);
            DBG_Printf("Draw    Poly =%4d  \n", dbgDrawPol);
            dbgComputePol = 0;
            dbgDrawPol = 0;
            dbgVblCount = 0;
	    DBG_SetCursol(3,5);
            DBG_Printf("Time Counter1=%8d  \n", timeCnt1);
	    DBG_SetCursol(3,6);
            DBG_Printf("Time Counter2=%8d  \n", timeCnt2);
#endif

    }
}


#define  BORDER     MTH_FIXED(0.9)
#define  COLLISION  MTH_FIXED(0.2)

void moveObj3D(Obj3D *topObj3D)
{
static MthXyz  m1Angle = { MTH_FIXED(1.0), MTH_FIXED(4.0), MTH_FIXED(1.0)};
static MthXyz  m2Angle = { MTH_FIXED(0.0), MTH_FIXED(-5.0), MTH_FIXED(0.0)};
static MthXyz  m3Angle = { MTH_FIXED(3.0), MTH_FIXED(0.0), MTH_FIXED(0.0)};
Obj3D *obj3D, *tObj3D;
Fixed32     w, x, y, z;
int         colFlag;

    for(obj3D = topObj3D; obj3D->cluster != 0; obj3D++) {
        for(tObj3D = obj3D+1; tObj3D->cluster != 0; tObj3D++) {
            if(obj3D != tObj3D) {
                colFlag = 0;
                w = x = obj3D->pos.x - tObj3D->pos.x;
                if(w < 0) w = -w;
                if(w < COLLISION) colFlag++;
                w = y = obj3D->pos.y - tObj3D->pos.y;
                if(w < 0) w = -w;
                if(w < COLLISION) colFlag++;
                w = z = obj3D->pos.z - tObj3D->pos.z;
                if(w < 0) w = -w;
                if(w < COLLISION) colFlag++;
                if(colFlag >= 3) {
                     tObj3D->speed.x = - tObj3D->speed.x;
                     obj3D->speed.x  = - obj3D->speed.x;
                     if(x >= 0)
                         obj3D->pos.x += (COLLISION - x);
                     else
                         obj3D->pos.x -= (COLLISION + x);
                     tObj3D->speed.y = - tObj3D->speed.y;
                     obj3D->speed.y  = - obj3D->speed.y;
                     if(y >= 0)
                         obj3D->pos.y += (COLLISION - y);
                     else
                         obj3D->pos.y -= (COLLISION + y);
                     tObj3D->speed.z = - tObj3D->speed.z;
                     obj3D->speed.z  = - obj3D->speed.z;
                     if(z >= 0)
                         obj3D->pos.z += (COLLISION - z);
                     else
                         obj3D->pos.z -= (COLLISION + z);
                }
            }
        }
    }
    for(obj3D = topObj3D; obj3D->cluster != 0; obj3D++) {
        obj3D->pos.x += obj3D->speed.x;
        obj3D->pos.y += obj3D->speed.y;
        obj3D->pos.z += obj3D->speed.z;
        if(obj3D->pos.x < -BORDER) {
            obj3D->speed.x = -obj3D->speed.x;
            obj3D->pos.x   = -BORDER;
        }
        if(obj3D->pos.x >  BORDER) {
            obj3D->speed.x = -obj3D->speed.x;
            obj3D->pos.x   = BORDER;
        }
        if(obj3D->pos.y < -BORDER) {
            obj3D->speed.y = -obj3D->speed.y;
            obj3D->pos.y   = -BORDER;
        }
        if(obj3D->pos.y >  BORDER) {
            obj3D->speed.y = -obj3D->speed.y;
            obj3D->pos.y   = BORDER;
        }
        if(obj3D->pos.z < -BORDER) {
            obj3D->speed.z = -obj3D->speed.z;
            obj3D->pos.z   = -BORDER;
        }
        if(obj3D->pos.z >  BORDER) {
            obj3D->speed.z = -obj3D->speed.z;
            obj3D->pos.z   = BORDER;
        }
        obj3D->angle.x += m1Angle.x;
        obj3D->angle.y += m1Angle.y;
        obj3D->angle.z += m1Angle.z;
        SPR_3MoveCluster(obj3D->cluster,1,&obj3D->angle,&obj3D->pos);
        obj3D->angle.x = obj3D->cluster->angle.x;
        obj3D->angle.y = obj3D->cluster->angle.y;
        obj3D->angle.z = obj3D->cluster->angle.z;
    }
    SPR_3MoveCluster(&Cluster2a,0,&m2Angle,0);
    SPR_3MoveCluster(&Cluster2b,0,&m2Angle,0);
    SPR_3MoveCluster(&Cluster3a,0,&m3Angle,0);
    SPR_3MoveCluster(&Cluster3b,0,&m3Angle,0);
}

/*  end of file */
