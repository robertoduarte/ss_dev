/*----------------------------------------------------------------------------
 *  spr_3ct0.c -- SPR ライブラリ 3C TENGEN モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-09-15 Ver.1.00
 *
 *  このライブラリはスプライト３Ｄ表示処理モジュールで、以下のルーチンを含む。
 *
 *  SPR_3Initial            -  ３Ｄスプライト表示の初期化
 *  SPR_3SetLight           -  光源ベクタのセット        
 *  SPR_3SetView            -  視点のセット              
 *  SPR_3CallAllCluster     -  全クラスタに対するファンクションコール
 *  SPR_3MoveCluster        -  クラスタの移動
 *  SPR_3DrawModel          -  モデルの登録
 *  SPR_3Flush              -  モデルの描画              
 *  SPR_3SetTexture         -  テクスチャのセット
 *  SPR_3ChangeTexColor     -  テクスチャカラーデータの変更
 *  SPR_3ClrTexture         -  テクスチャエリアの解放
 *  SPR_3SetClipLevel       -  クリッピングレベルのセット    
 *  SPR_3SetZSortMinMax     -  視点座標系Ｚソート範囲の最小・最大値をセット
 *  SPR_3SetPixelCount      -  単位ピクセル数のセット    
 *  SPR_3SetTransPostRtn    -  座標変換結果通知ルーチンのセット    
 *  SPR_3GetStatus          -  現３Ｄ環境データの取得
 *  SPR_3SetSurfNormVect    -  ３Ｄオブジェクトの面法線を計算しセット
 *  SPR_3SetDrawSpeed       -  ＶＤＰ１の高速描画パラメータをセット
 *
 *  このライブラリを使用するには次のインクルードファイルとワークエリアを定義す
 *  る必要がある。
 *
 *   #include "sega_scl.h"
 *   #define  _SPR3_
 *   #include "sega_spr.h"
 *
 *   #define CommandMax    100          // 最大コマンド数
 *   #define GourTblMax    100          // 最大グーローテーブル数
 *   #define LookupTblMax  100          // 最大ルックアップテーブル数
 *   #define CharMax       100          // 最大キャラクタ数
 *   #define DrawPrtyMax   256          // 最大プライオリティブロック数
 *   SPR_2DefineWork(work2D,
 *                CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)
 *                                      // ２Ｄワークエリア定義
 *
 *   #define ObjSurfMax     16          // ３Ｄオブジェクトの最大面数
 *   #define ObjVertMax     16          // ３Ｄオブジェクトの最大頂点数
 *   SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)
 *                                      // ３Ｄワークエリア定義
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#define  _SPR3_
#include "sega_spr.h"

#ifdef _SH
#include <machine.h>
#endif
#ifdef USE_DSP
#include "sega_dsp.h"
#endif

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define   SCREEN_W_SIZE     256
#define   SCREEN_H_SIZE     256

#define   CLIP_MIN_Z       FIXED32_MIN
#define   CLIP_MAX_Z       MTH_FIXED(-0.005)
#define   ZSORT_MIN_Z      MTH_FIXED(-10.0)
#define   ZSORT_MAX_Z      MTH_FIXED(0)

#define	WORLD_MATRIX_STACK_SIZE	32

typedef struct TransParm TransParm;
struct TransParm {
   Sint32    *vertBright;
   MthXyz    *coordView3D;
   XyInt     *coord2D;
   Sint32    *surfBright;
   MthXyz    *coordWorld3D;
   TComTbl   *comTbl;
   T3Dto2D   *mst3Dto2D;
   T3Dto2D   *slv3Dto2D;
   T2DtoCmd  *mst2DtoCmd;
   T2DtoCmd  *slv2DtoCmd;
};

/*
 * GLOBAL DECLARATIONS
 */
#ifdef USE_DEBUG_INFO
int    dbgComputePol = 0;
int    dbgDrawPol    = 0;
#endif

extern Sint32      Sp2OpenFlag;             /* Open Command Exec Flag        */
extern SprCharTbl *SpCharTbl;               /* Character Table Pointer       */
extern Sint32      SpCharMax;               /* Character Table Entry Count   */
extern Sint32      SpDrawPrtyBlkMax;        /* Draw Priority Block Max Count */
extern SprDrawPrtyBlk  *SpMstDrawPrtyBlk;
extern SprSpCmd    *SpMstCmd;        
extern SprDrawPrtyBlk  *SpSlvDrawPrtyBlk;
extern SprSpCmd    *SpSlvCmd;        
extern Vaddr       SpLookupTblR;
extern int         SpMstCmdPos;        
extern int         SpSlvCmdPos;
extern Sint32      SpGourTblMax;
extern SprGourTbl  *SpGourTbl;

extern void *SPR_SlaveParam;
extern void (*SPR_SlaveCommand)(void);
extern void SPR_3CosSinToDsp(Fixed32 degree);	/*	95-7-27	*/

/*
 * STATIC DECLARATIONS
 */
       TComTbl    SpTComTbl; 
static T3Dto2D    tMst3Dto2D; 
static T3Dto2D    tSlv3Dto2D; 
static T2DtoCmd   tMst2DtoCmd; 
static T2DtoCmd   tSlv2DtoCmd;
static GetSurfNormPara  mstSurfNormPara;
static GetSurfNormPara  slvSurfNormPara;
static TransParm  transParm;
 
static MthMatrix    viewMatrix;
static MthMatrix    lightMatrix;
static MthMatrix    worldMatrix [WORLD_MATRIX_STACK_SIZE];
static MthMatrixTbl viewMatrixTbl;
static MthMatrixTbl lightMatrixTbl;
static MthMatrixTbl worldMatrixTbl;

static Sint32    firstDrawFlag;    /* フラッシュ後最初のモデル描画フラグ     */

static MthXyz    lightAngle;       /* 光源アングル                           */
static MthXyz	 worldLightVector; /* ワールド座標系での光源ベクトル         */
static MthXyz	 viewLightVector;  /* 視点座標系での光源ベクトル             */
static MthXyz    viewCoordPoint;   /* 視点座標系内の視点位置                 */
static MthXyz    viewPoint;        /* 視点座標                               */
static MthXyz    viewAngle;        /* 視点アングル                           */
static Sint32	 viewAngleSeq;     /* 視点アングル回転操作                   */
static Sint32    *surfBright;      /* 面輝度テーブル                         */
static MthXyz    *coordView3D;     /* ３Ｄ視点座標変換後テーブル             */
static XyInt     *coord2D;         /* ２Ｄ透視変換後テーブル                 */
static Sint32    *vertBright;      /* 頂点輝度テーブル                       */
static MthXyz    *coordWorld3D;    /* ３Ｄワールド座標変換後テーブル         */
/* static Sint32    gourTblNo;  	95-7-27	unuse	*/
static Fixed32   zSortZMinOrg;     /* フラッシュ単位元Ｚ座標最小値           */
static Fixed32   zSortZMaxOrg;     /* フラッシュ単位元Ｚ座標最大値           */
static Fixed32   zSortZMin;        /* フラッシュ単位現Ｚ座標最小値           */
static Fixed32   zSortZMax;        /* フラッシュ単位現Ｚ座標最大値           */
static Fixed32   zSortBZMin;       /* フラッシュ単位前Ｚ座標最小値           */
static Fixed32   zSortBZMax;       /* フラッシュ単位前Ｚ座標最大値           */
static Uint16    zSortBlkCnt;      /* Ｚソートブロック数                     */
static Sint32    zSortMode;        /* ＺソートＺ座標値の採用モード           */
static int       blkSftCnt;        /* Ｚソートブロックハッシュ用シフト数     */
static int       zSftCnt;          /* ＺソートＺシフト数                     */
static Sint32    clipLevel;        /* クリッピングレベル                     */
static Sint32   spriteDrawFlag;    /* 変形スプライト描画フラグ               */
static Sint32   otherDrawFlag;     /* その他ポリゴン描画フラグ               */
static Sint32   unitPixel[2];      /* X,Y 1.0 に対するスクリーンのピクセル数 */
static Fixed32  clipRatio[2];      /* ３Ｄクリッピング率 [0]=X, [1]=Y        */
static Fixed32  clipZ[2];          /* 視点座標系クリップＺ [0]=min, [1]=max  */

static int        gourTblSize;     /* グーロテーブルサイズ                   */
static SprGourTbl *mstGourTblTop;  /* マスタ  グーロテーブル先頭アドレス     */
static SprGourTbl *slvGourTblTop;  /* スレーブグーロテーブル先頭アドレス     */

static	MthXyz	orgLightVect =
	    { MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(-1.0)};

#ifdef USE_DSP
static volatile Uint32 *dspData = (Uint32*)(DSP_REG_BASE + DSP_RW_DDAT);
#endif

static SprCluster *befCluster;
static MthMatrix  clusterViewMatrix[2];
static MthMatrix  clusterWorldMatrix[2];
static int        clusterBufSw, firstClusterFlag;

void  SPR_3Tran3Dto2D(T3Dto2D *t3Dto2D);
void  SPR_3Tran2DtoCmd(T2DtoCmd *t2DtoCmd);
void  SPR_3CosSin(Fixed32 degree);

static void   initDraw(void);
static void   transCluster(SprCluster *cluster);
static void   drawCluster(SprCluster *cluster,
                          MthMatrix *viewMatrix, MthMatrix *worldMatrix);
static void   viewCoordMatrix(void);
static void   setLightAngle(void);
static void   setInbetPolygons(SprCluster *cluster, SprObject3D  *obj);
static void   setSurfNormVect(GetSurfNormPara  *para);
int SPR_3SetTransParm(SprObject3D *obj, MthMatrix *viewMatrix,
                                   MthMatrix *worldMatrix, TransParm *tParm);


/*****************************************************************************
 *
 * NAME:  SPR_3Initial()  - Initial 3D Sprite Display Environment
 *
 * PARAMETERS :
 *
 * 　　(1) Spr3WorkArea *workArea - <i> ３Ｄスプライトワークエリア定義
 *
 * DESCRIPTION:
 *
 *     ３Ｄスプライト表示の初期化処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3Initial(Spr3WorkArea  *workArea)
{
    MthXyz  w0,w1;
    int     i, j;

    /** BEGIN ***************************************************************/
    SPR_3MatrixCompoInit();
    
    SPR_3SetDrawSpeed(0, 0, 0);
    
    surfBright   = workArea->surfBright;
    coordView3D  = workArea->coordView3D;
    coord2D      = workArea->coord2D;
    vertBright   = workArea->vertBright;
    coordWorld3D = workArea->coordWorld3D;

    zSortBlkCnt = SpDrawPrtyBlkMax;
    for(blkSftCnt=0; blkSftCnt<16; blkSftCnt++)
       if((zSortBlkCnt<<=1) & 0x8000) break;
    zSortBlkCnt = SpDrawPrtyBlkMax;
    zSortZMin   = ZSORT_MIN_Z;
    zSortZMax   = ZSORT_MAX_Z;
    zSortMode   = ZSORT_FLOAT_MODE;

    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    SPR_3SetLight(1, &w0);
    w0.x = MTH_FIXED(0.0);   w0.y = MTH_FIXED(0.0);     w0.z = MTH_FIXED(0.0);
    w1.x = MTH_FIXED(0.0);   w1.y = MTH_FIXED(0.0);     w1.z = MTH_FIXED(0.0);
    SPR_3SetView(1, &w0, &w1, ROT_SEQ_XYZ, &w0);

    MTH_InitialMatrix(&worldMatrixTbl, WORLD_MATRIX_STACK_SIZE,	worldMatrix);
    SPR_3SetClipLevel(2, CLIP_MIN_Z, CLIP_MAX_Z);
    SPR_3SetPixelCount(SCREEN_W_SIZE, SCREEN_H_SIZE);

    gourTblSize = (SpGourTblMax - 32) / 2;
    mstGourTblTop  = SpGourTbl      + 32;
    slvGourTblTop  = mstGourTblTop  + gourTblSize;
    for(i=0; i<32; i++) 
        for(j=0; j<4; j++) 
            SpGourTbl[i].entry[j] = RGB16_COLOR(i,i,i);
    firstDrawFlag = 1;

    transParm.vertBright   = vertBright;
    transParm.coordView3D  = coordView3D;
    transParm.coord2D      = coord2D;
    transParm.surfBright   = surfBright;
    transParm.coordWorld3D = coordWorld3D;
    transParm.comTbl       = &SpTComTbl;
    transParm.mst3Dto2D    = &tMst3Dto2D;
    transParm.slv3Dto2D    = &tSlv3Dto2D;
    transParm.mst2DtoCmd   = &tMst2DtoCmd;
    transParm.slv2DtoCmd   = &tSlv2DtoCmd;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetLight()  - Set Light Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16      moveKind  - <i> 移動量の種別ーブルポインタ
 *                       　　　　　    0 = 相対移動、1 = 絶対移動タ
 *     (2) MthXyz *pLightAngler  - <i>  平行光源のアングル
 *
 * DESCRIPTION:
 *
 *     光源アングルセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3SetLight(Uint16 moveKind, MthXyz *pLightAngle)
{
    Fixed32 angleX, angleY, angleZ;
    
    /** BEGIN ***************************************************************/
    if(moveKind & 0x0002) {
        angleX = MTH_Atan(pLightAngle->y, pLightAngle->z);
        angleY = MTH_Atan(pLightAngle->z, pLightAngle->x);
        angleZ = MTH_Atan(pLightAngle->y, pLightAngle->x);
    } else  {
        angleX = pLightAngle->x;
        angleY = pLightAngle->y;
        angleZ = pLightAngle->z;
    }
    if(moveKind & 0x0001) {
	lightAngle.x  = angleX;
	lightAngle.y  = angleY;
	lightAngle.z  = angleZ;
    } else       {
	lightAngle.x  += angleX;
	lightAngle.y  += angleY;
	lightAngle.z  += angleZ;
    }
    if(lightAngle.x > MTH_FIXED( 180)) lightAngle.x -= MTH_FIXED(360);  else
    if(lightAngle.x < MTH_FIXED(-180)) lightAngle.x += MTH_FIXED(360);
    if(lightAngle.y > MTH_FIXED( 180)) lightAngle.y -= MTH_FIXED(360);  else
    if(lightAngle.y < MTH_FIXED(-180)) lightAngle.y += MTH_FIXED(360);
    if(lightAngle.z > MTH_FIXED( 180)) lightAngle.z -= MTH_FIXED(360);  else
    if(lightAngle.z < MTH_FIXED(-180)) lightAngle.z += MTH_FIXED(360);
    setLightAngle();
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetView()  - Set View Point and Angle
 *
 * PARAMETERS :
 *
 *     (1) Uint16  moveKind         - <i> 移動量の種別ーブルポインタ
 *                       　　　　　       0 = 相対移動、1 = 絶対移動タ
 *     (2) MthXyz *pViewPoint       - <i> 視点座標
 *     (3) MthXyz *pViewAngle       - <i> 視点のアングル（度）
 *     (4) Uint16  pViewAngleSeq    - <i> アングルの回転操作
 *　　　　　　　　　　　    　            ROT_SEQ_ZYX
 *　　　　　　　　　　　    　            ROT_SEQ_ZXY
 *　　　　　　　　　　　    　            ROT_SEQ_YZX
 *　　　　　　　　　　　    　            ROT_SEQ_YXZ
 *　　　　　　　　　　　    　            ROT_SEQ_XYZ
 *　　　　　　　　　　　    　            ROT_SEQ_XZY
 *     (5) MthXyz *pViewCoordPoint  - <i> 視点座標系内の視点位置
 *
 * DESCRIPTION:
 *
 *     視点のセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3SetView(Uint16 moveKind, MthXyz *pViewPoint, MthXyz *pViewAngle,
                              Uint16 pViewAngleSeq, MthXyz *pViewCoordPoint)
{
    Fixed32 angleX, angleY, angleZ;
    
    /** BEGIN ***************************************************************/
    viewAngleSeq = pViewAngleSeq;

    if(pViewCoordPoint) {
        if(moveKind & 0x0001) {
	    viewCoordPoint.x  = pViewCoordPoint->x;
	    viewCoordPoint.y  = pViewCoordPoint->y;
	    viewCoordPoint.z  = pViewCoordPoint->z;
        } else       {
	    viewCoordPoint.x += pViewCoordPoint->x;
	    viewCoordPoint.y += pViewCoordPoint->y;
	    viewCoordPoint.z += pViewCoordPoint->z;
        }
    }

    if(pViewPoint) {
        if(moveKind & 0x0001) {
	    viewPoint.x  = pViewPoint->x;
	    viewPoint.y  = pViewPoint->y;
	    viewPoint.z  = pViewPoint->z;
        } else       {
	    viewPoint.x += pViewPoint->x;
	    viewPoint.y += pViewPoint->y;
	    viewPoint.z += pViewPoint->z;
        }
    }

    if(pViewAngle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(pViewAngle->y, pViewAngle->z);
            angleY = MTH_Atan(pViewAngle->z, pViewAngle->x);
            angleZ = MTH_Atan(pViewAngle->y, pViewAngle->x);
        } else  {
            angleX = pViewAngle->x;
            angleY = pViewAngle->y;
            angleZ = pViewAngle->z;
        }
        if(moveKind & 0x0001) {
	    viewAngle.x  = angleX;
	    viewAngle.y  = angleY;
	    viewAngle.z  = angleZ;
        } else       {
	    viewAngle.x += angleX;
	    viewAngle.y += angleY;
	    viewAngle.z += angleZ;
        }
        if(viewAngle.x > MTH_FIXED( 180)) viewAngle.x -= MTH_FIXED(360);  else
        if(viewAngle.x < MTH_FIXED(-180)) viewAngle.x += MTH_FIXED(360);
        if(viewAngle.y > MTH_FIXED( 180)) viewAngle.y -= MTH_FIXED(360);  else
        if(viewAngle.y < MTH_FIXED(-180)) viewAngle.y += MTH_FIXED(360);
        if(viewAngle.z > MTH_FIXED( 180)) viewAngle.z -= MTH_FIXED(360);  else
        if(viewAngle.z < MTH_FIXED(-180)) viewAngle.z += MTH_FIXED(360);
    }

}


/*****************************************************************************
 *
 * NAME:  SPR_3CallAllCluster()  - Call User Function with all Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster           - <i> ルートクラスタテーブルのポインタ
 *     (2) void (*userFunc)(SprCluster*) - <i> ユーザファンクション
 *
 * DESCRIPTION:
 *
 *     ルートクラスタにつながる全クラスタをパラメータとして指定ファンクション
 *     を呼び出す
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3CallAllCluster(SprCluster *cluster, void (*userFunc)(SprCluster*))
{
    /** BEGIN ***************************************************************/
    while(cluster) {
        userFunc(cluster);
	/* 子クラスタの処理 */
	if(cluster->child) SPR_3CallAllCluster(cluster->child, userFunc);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3MoveCluster()  - Move Specify Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *cluster   - <i> 移動すべきクラスタテーブルのポインタ
 *     (2) Uint16      moveKind  - <i> 移動量の種別ーブルポインタ
 *                       　　　　　    0 = 相対移動、1 = 絶対移動タ
 *     (3) MthXyz     *angle     - <i> 回転移動量（度）
 *     (4) MthXyz     *point     - <i> 平行移動量テーブルポインタ
 *
 * DESCRIPTION:
 *
 *     クラスタの移動
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3MoveCluster(SprCluster *cluster, Uint16 moveKind,
				      MthXyz *angle, MthXyz *point)
{
    Fixed32 angleX, angleY, angleZ;
    
    /** BEGIN ***************************************************************/
    if(angle) {
        if(moveKind & 0x0002) {
            angleX = MTH_Atan(angle->y, angle->z);
            angleY = MTH_Atan(angle->z, angle->x);
            angleZ = MTH_Atan(angle->y, angle->x);
        } else  {
	    angleX = angle->x;
	    angleY = angle->y;
	    angleZ = angle->z;
        }
        if(moveKind & 0x0001) {
   	    cluster->angle.x  = angleX;
	    cluster->angle.y  = angleY;
	    cluster->angle.z  = angleZ;
        } else       {
	    cluster->angle.x += angleX;
	    cluster->angle.y += angleY;
	    cluster->angle.z += angleZ;
        }
        if(cluster->angle.x > MTH_FIXED( 180)) 
            cluster->angle.x -= MTH_FIXED(360);
        else
        if(cluster->angle.x < MTH_FIXED(-180))
            cluster->angle.x += MTH_FIXED(360);
        if(cluster->angle.y > MTH_FIXED( 180))
            cluster->angle.y -= MTH_FIXED(360);
        else
        if(cluster->angle.y < MTH_FIXED(-180))
            cluster->angle.y += MTH_FIXED(360);
        if(cluster->angle.z > MTH_FIXED( 180))
            cluster->angle.z -= MTH_FIXED(360);
        else
        if(cluster->angle.z < MTH_FIXED(-180))
            cluster->angle.z += MTH_FIXED(360);
    }
    if(point)
        if(moveKind & 0x0001) {
	    cluster->point.x  = point->x;
	    cluster->point.y  = point->y;
	    cluster->point.z  = point->z;
        } else       {
	    cluster->point.x += point->x;
	    cluster->point.y += point->y;
	    cluster->point.z += point->z;
        }
}


/*****************************************************************************
 *
 * NAME:  SPR_3DrawModel()  - Set Specify Model
 *
 * PARAMETERS :
 *
 *     (1) SprCluster *rootCluster  - <i>  表示すべきモデルのルートクラスタ
 *                                         テーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     モデルの登録
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3DrawModel(SprCluster *rootCluster)
{
    /** BEGIN ***************************************************************/
    if(Sp2OpenFlag) {
	Sp2OpenFlag = 0;
        /* マスタ、スレーブ共通パラメータのセット */
        SpTComTbl.gourGrayTopR  = SPR_2GourTblNoToVram(0);
        /* マスタ側　２Ｄ－＞Ｃｍｄ　パラメータセット */
        tMst2DtoCmd.gourTblCur  = mstGourTblTop;    
        tMst2DtoCmd.gourTblCurR = SpTComTbl.gourGrayTopR + 32;    
        /* スレーブ側　２Ｄ－＞Ｃｍｄ　パラメータセット */
        tSlv2DtoCmd.gourTblCur  = slvGourTblTop;
        tSlv2DtoCmd.gourTblCurR = tMst2DtoCmd.gourTblCurR + gourTblSize;
    }

    if(firstDrawFlag) {
	firstDrawFlag = 0;
	viewCoordMatrix();
	initDraw();
    }

    firstClusterFlag = 0;
    clusterBufSw = 0;
    transCluster(rootCluster);
    drawCluster(befCluster, &clusterViewMatrix[clusterBufSw^1],
                            &clusterWorldMatrix[clusterBufSw^1]);

    SpMstCmdPos = tMst2DtoCmd.outCmdPos; 
    SpSlvCmdPos = tSlv2DtoCmd.outCmdPos; 
}


/*****************************************************************************
 *
 * NAME:  SPR_3Flush()  - Draw End All Seted Cluster
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     登録された全モデルの描画終了処理
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3Flush(void)
{
    /** BEGIN ***************************************************************/
    SPR_2FlushDrawPrty();
    firstDrawFlag = 1;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetTexture()  - Set Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  テクスチャテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャのセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3SetTexture(SprTexture *texture)
{
    SprTexture   *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++) {
	SPR_2SetChar(wTex->charNo, wTex->colorMode, wTex->color,
                     wTex->width, wTex->height, wTex->charData);
	if(wTex->lookupTbl)
	    SPR_2SetLookupTbl(wTex->color, wTex->lookupTbl);
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3ChangeTexColor()  - Change Texture Color
 *
 * PARAMETERS :
 *
 *     (1) Uint16      charNo    - <i>  キャラクタ番号
 *     (2) Uint16      color     - <i>  カラーデータ（ｶﾗｰﾊﾞﾝｸorﾙｯｸｱｯﾌﾟﾃｰﾌﾞﾙNo）
 *     (2) SprLookupTbl *lookupTbl   - <i>  ルックアップテーブルポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャカラーデータの変更
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3ChangeTexColor(Uint16 charNo, Uint16 color, SprLookupTbl *lookupTbl)
{
    /** BEGIN ***************************************************************/
    SpCharTbl[charNo].color = color;
    if(lookupTbl)
        SPR_2SetLookupTbl(color, lookupTbl);
}


/*****************************************************************************
 *
 * NAME:  SPR_3ClrTexture()  - Clear Texture Data
 *
 * PARAMETERS :
 *
 *     (1) SprTexture *texture  - <i>  テクスチャテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     テクスチャエリアの解放
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3ClrTexture(SprTexture *texture)
{
    SprTexture  *wTex;

    /** BEGIN ***************************************************************/
    for(wTex = texture; wTex->charNo != 0xffff; wTex++)
        SPR_2ClrChar(wTex->charNo);
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetZSortMinMax()  - Set Z Sort Min/Max Z Value
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pZSortMode  - <i>  ＺソートＺ座標値の採用モード
 *                                     ０＝浮動モード
 *                                     １＝固定モード
 *     (2) Fixed32  pZSortMin   - <i>  視点座標系ＺソートＺ最小値座標
 *     (3) Fixed32  pZSortMax   - <i>  視点座標系ＺソートＺ最大値座標
 *
 *
 * DESCRIPTION:
 *
 *     視点座標系Ｚソート範囲の最小・最大値をセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3SetZSortMinMax(Uint16 pZSortMode, Fixed32 pZSortMin, Fixed32 pZSortMax)
{

    /** BEGIN ***************************************************************/
    zSortMode    = pZSortMode;
    zSortZMin    = pZSortMin;
    zSortZMax    = pZSortMax;
    zSortZMinOrg = pZSortMin;
    zSortZMaxOrg = pZSortMax;
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetClipLevel()  - Set Cliping Level
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pClipLevel  - <i>  クリッピングのレベル番号
 *                                     ０＝クリッピングなし
 *                                     １＝視点座標系のＺ範囲でクリップ
 *                                     ２＝フレームバッファに対する３Ｄ
 *                                         ピラミッドでクリップ
 *     (2) Fixed32  pClipZMin   - <i>  視点座標系クリップＺ最小値座標
 *     (3) Fixed32  pClipZMax   - <i>  視点座標系クリップＺ最大値座標
 *
 * DESCRIPTION:
 *
 *     クリッピングレベルのセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_3SetClipLevel(Uint16 pClipLevel, Fixed32 pClipZMin, Fixed32 pClipZMax)
{

    /** BEGIN ***************************************************************/
    clipLevel    = pClipLevel;
    clipZ[0]     = FIXED32_MIN;
    clipZ[1]     = FIXED32_MAX;
    clipRatio[0] = 0;
    clipRatio[1] = 0;
    if(clipLevel >= 1) {
        clipZ[0] = pClipZMin;
        clipZ[1] = pClipZMax;
        if(clipLevel >= 2) {
            clipRatio[0] = MTH_Div(MTH_FIXED(-1000.0),
                                   MTH_IntToFixed(unitPixel[0]));
            clipRatio[1] = MTH_Div(MTH_FIXED(-1000.0),
                                   MTH_IntToFixed(unitPixel[1]));
        }
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetPixelCount()  - Set Screen Pixel Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16   pixelCountX  - <i>  スクリーンＸの単位ピクセル数
 *     (2) Uint16   pixelCountY  - <i>  スクリーンＹの単位ピクセル数
 *
 * DESCRIPTION:
 *
 *     スクリーンへの透視変換時のＸＹ各々 1.0 に対するスクリーンのピクセル数
 *     を設定する。
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void SPR_3SetPixelCount(Uint16 pixelCountX, Uint16 pixelCountY)
{

    /** BEGIN ***************************************************************/
    unitPixel[0] = pixelCountX;
    unitPixel[1] = pixelCountY;
    if(clipLevel >= 2) {
        clipRatio[0] = MTH_Div(MTH_FIXED(-1000.0),
                                   MTH_IntToFixed(unitPixel[0]));
        clipRatio[1] = MTH_Div(MTH_FIXED(-1000.0),
                                   MTH_IntToFixed(unitPixel[1]));
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3GetStatus()  - Get 3D Environment Value
 *
 * PARAMETERS :
 *
 *     (1) Spr3dStatus *spr3dStatus  - <o>  ３Ｄ環境データテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     現３Ｄ環境データの取得。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3GetStatus(Spr3dStatus *spr3dStatus)
{
    /** BEGIN ***************************************************************/
    spr3dStatus->lightAngle      = lightAngle;
    spr3dStatus->viewCoordPoint  = viewCoordPoint;
    spr3dStatus->viewPoint       = viewPoint;     
    spr3dStatus->viewAngle       = viewAngle;     
    spr3dStatus->viewAngleSeq    = viewAngleSeq;  
    spr3dStatus->zSortMode       = zSortMode;     
    spr3dStatus->zSortZMin       = zSortZMin;     
    spr3dStatus->zSortZMax       = zSortZMax;     
    spr3dStatus->clipZMin        = clipZ[0];      
    spr3dStatus->clipZMax        = clipZ[1];      
    spr3dStatus->clipLevel       = clipLevel;     
    spr3dStatus->unitPixelX      = unitPixel[0];
    spr3dStatus->unitPixelY      = unitPixel[1];
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetSurfNormVect() - Compute Surface Normal Vector in the Object
 *
 * PARAMETERS :
 *
 *     (1)  SprObject3D  *obj  - <i/o>  ３Ｄオブジェクトのポインタ
 *
 * DESCRIPTION:
 *
 *     指定３Ｄオブジェクトの面の法線を計算し、セットする。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3SetSurfNormVect(SprObject3D  *obj)
{
    /** BEGIN ***************************************************************/
    mstSurfNormPara.surfaceCount = obj->surfaceCount >> 1;
    slvSurfNormPara.surfaceCount = obj->surfaceCount - 
                                   mstSurfNormPara.surfaceCount;

    /* スレーブ側　面の法線を計算　*/
    slvSurfNormPara.surface       = obj->surface +
                                    mstSurfNormPara.surfaceCount;
    slvSurfNormPara.vertPoint     = obj->vertPoint;
    slvSurfNormPara.surfaceNormal = obj->surfaceNormal +
                                    mstSurfNormPara.surfaceCount;
    slvSurfNormPara.surfNormK     = obj->surfNormK;
    /* invoke Slave */
    SPR_RunSlaveSH((PARA_RTN*)setSurfNormVect, &slvSurfNormPara);

    /* マスタ側　面の法線を計算　*/
    mstSurfNormPara.surface       = obj->surface;
    mstSurfNormPara.vertPoint     = obj->vertPoint;
    mstSurfNormPara.surfaceNormal = obj->surfaceNormal;
    mstSurfNormPara.surfNormK     = obj->surfNormK;
    setSurfNormVect(&mstSurfNormPara);

    /* check slave end */
    SPR_WaitEndSlaveSH();
}


/*****************************************************************************
 *
 * NAME:  setSurfNormVect() - Compute Surface Normal Vector
 *
 * PARAMETERS :
 *
 *     (1)  GetSurfNormPara  *para - <i/o>  パラメータテーブルのポインタ
 *
 * DESCRIPTION:
 *
 *     指定パラメータの面の法線を計算し、セットする。
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void   setSurfNormVect(GetSurfNormPara  *para)
{
    Sint32      i, p1, p2, p3;
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
    MthXyz      *wSurfNormal;

    /** BEGIN ***************************************************************/
    wSurface    = para->surface;
    wVertPoint  = para->vertPoint;
    wSurfNormal = para->surfaceNormal;
    for(i = 0; i < para->surfaceCount; i++) {
	p1 = wSurface->vertNo[0];
	p2 = wSurface->vertNo[1];
	p3 = wSurface->vertNo[2];
	MTH_ComputeNormVect(para->surfNormK, &wVertPoint[p1], &wVertPoint[p2],
			                     &wVertPoint[p3], wSurfNormal);
	wSurface++;
	wSurfNormal++;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_3SetDrawSpeed() - Set VDP1 Drawing Speed up Parameter
 *
 * PARAMETERS :
 *
 *     (1)  Sint32    hssFlag  - <i>  ハイスピードシュリンクフラグ
 *                                    テクスチャポリゴンの描画モード
 *                                    ０＝精度優先描画（デフォルト）
 *                                    １＝速度優先描画
 *     (2)  Sint32    eosFlag  - <i>  hss=1 で速度優先描画を指定した時の
 *                                    元絵テクスチャのサンプリング座標
 *                                    ０＝偶数座標をサンプリング（デフォルト）
 *                                    １＝奇数座標をサンプリング
 *     (3)  Sint32    pclpFlag - <i>  プリクリッピング有効・無効フラグ
 *                                    ０＝有効（デフォルト）
 *                                    １＝無効
 *
 *
 * DESCRIPTION:
 *
 *     ＶＤＰ１の高速描画パラメータをセット
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void   SPR_3SetDrawSpeed(Sint32 hssFlag, Sint32 eosFlag, Sint32 pclpFlag)
{
/*    Sint32      i, p1, p2, p3;	95-7-27	unuse
    MthXyz      *wVertPoint;
    SprSurface  *wSurface;
   MthXyz      *wSurfNormal;
*/
    /** BEGIN ***************************************************************/
    if(hssFlag) 
       spriteDrawFlag  = HSS_ENABLE;
    else
       spriteDrawFlag  = 0;

    if(pclpFlag) {
       spriteDrawFlag |= PCLP_ENABLE;
       otherDrawFlag   = PCLP_ENABLE;
    } else {
       spriteDrawFlag &= ~PCLP_ENABLE;
       otherDrawFlag   = 0;
    }

    SPR_SetEosMode(eosFlag);
}


/*****************************************************************************
 *
 * NAME:  initDraw()  - Initial Draw Environment
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ３Ｄ表示のテーブル初期化
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
initDraw(void)
{
    Fixed32   ztMax;
    Sint32    i;

    /** BEGIN ***************************************************************/
    if(zSortMode) {
        zSortBZMin    = zSortZMinOrg;
        zSortBZMax    = zSortZMaxOrg;
    } else        {
        zSortBZMin    = zSortZMin;
        zSortBZMax    = zSortZMax;
    }
    zSortZMin     = FIXED32_MAX;
    zSortZMax     = FIXED32_MIN;
    ztMax = zSortBZMax - zSortBZMin;
    for(i=0; i<32; i++)
	if((ztMax<<=1) & 0x80000000) break;
    zSftCnt = 17 - i + blkSftCnt;
    if(zSftCnt < 0) zSftCnt = 0;

    /* マスタ、スレーブ共通パラメータのセット */
    SpTComTbl.lightVector    = &viewLightVector;
    SpTComTbl.zSortBZMin     = zSortBZMin;   
    SpTComTbl.zSftCnt        = zSftCnt;      
    SpTComTbl.zSortBlkCnt    = zSortBlkCnt;  
    SpTComTbl.spriteDrawFlag = spriteDrawFlag;
    SpTComTbl.otherDrawFlag  = otherDrawFlag;
    SpTComTbl.unitPixel      = unitPixel;      
    SpTComTbl.clipRatio      = clipRatio;     
    SpTComTbl.clipZ          = clipZ;     
    SpTComTbl.charTbl        = SpCharTbl;    
    SpTComTbl.lookupTblR     = SpLookupTblR;
    SpTComTbl.coordView3D    = coordView3D;  
    SpTComTbl.coord2D        = coord2D;      
    SpTComTbl.vertBright     = vertBright;      

    /* マスタ側　２Ｄ－＞Ｃｍｄ　パラメータセット */
    tMst2DtoCmd.zSortZMin    = zSortZMin;    
    tMst2DtoCmd.zSortZMax    = zSortZMax;    
    tMst2DtoCmd.prtyTbl      = SpMstDrawPrtyBlk;
    tMst2DtoCmd.cmdBuf       = SpMstCmd;        
    tMst2DtoCmd.outCmdPos    = SpMstCmdPos;

    /* スレーブ側　２Ｄ－＞Ｃｍｄ　パラメータセット */
    tSlv2DtoCmd.zSortZMin    = zSortZMin;    
    tSlv2DtoCmd.zSortZMax    = zSortZMax;    
    tSlv2DtoCmd.prtyTbl      = SpSlvDrawPrtyBlk;
    tSlv2DtoCmd.cmdBuf       = SpSlvCmd;        
    tSlv2DtoCmd.outCmdPos    = SpSlvCmdPos; 
}



/*****************************************************************************
 *
 * NAME:  transCluster()  - Transfer Cluster Coord
 *
 * PARAMETERS :
 *
 *     (1) SprCluster  *cluster  - <i>  ルートクラスタのポインタ
 *
 * DESCRIPTION:
 *
 *     クラスタ単位の隠面、座標変換処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
transCluster(SprCluster *cluster)
{
/*    SprDspMatrixParm mtxParm;		95-7-27	unuse
    SprDspMatrixOp   *wOp;
*/    
    Uint32           wl;


    /** BEGIN ***************************************************************/
    while(cluster) {
        if(cluster->transStart)  cluster->transStart(cluster);

        MTH_PushMatrix(&worldMatrixTbl);

#ifdef USE_DSP
        DSP_WRITE_REG(DSP_RW_CTRL, 0);      /* DSP Stop                     */
        DSP_WRITE_REG(DSP_W_DADR, 0);       /* Write Data Address           */
        wl = ((Uint32)worldMatrixTbl.current) >> 2;
        *dspData = wl;			   /* Write Data                 */
        *dspData = wl;

        *dspData = (Uint32)cluster->point.x;
        *dspData = (Uint32)cluster->point.y;
        *dspData = (Uint32)cluster->point.z;
        *dspData = (Uint32)MTX_OP_MOV_XYZ;

        switch(cluster->angleSeq) {
            case ROT_SEQ_ZYX :
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
	         break;
            case ROT_SEQ_ZXY :
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
	         break;
            case ROT_SEQ_YZX :
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
	         break;
            case ROT_SEQ_YXZ :
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
	         break;
            case ROT_SEQ_XYZ :
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
	         break;
            case ROT_SEQ_XZY :
                 SPR_3CosSinToDsp(cluster->angle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(cluster->angle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(cluster->angle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
	         break;
        }
        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;

#else
        mtxParm.inputMatrix  = worldMatrixTbl.current;
        mtxParm.outputMatrix = worldMatrixTbl.current;
        wOp = mtxParm.op;

        wOp  ->opCode = MTX_OP_MOV_XYZ;
        wOp  ->x      = cluster->point.x;
        wOp  ->y      = cluster->point.y;
        wOp++->z      = cluster->point.z;

        switch(cluster->angleSeq) {
            case ROT_SEQ_ZYX :
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
	         break;
            case ROT_SEQ_ZXY :
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
	         break;
            case ROT_SEQ_YZX :
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
	         break;
            case ROT_SEQ_YXZ :
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
	         break;
            case ROT_SEQ_XYZ :
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
	         break;
            case ROT_SEQ_XZY :
                 wOp  ->opCode = MTX_OP_ROT_Y;
                 wOp++->y      = cluster->angle.y;
                 wOp  ->opCode = MTX_OP_ROT_Z;
                 wOp++->z      = cluster->angle.z;
                 wOp  ->opCode = MTX_OP_ROT_X;
                 wOp++->x      = cluster->angle.x;
	         break;
        }

        wOp  ->opCode = MTX_OP_END;
#endif

#ifdef USE_DSP
        DSP_WRITE_REG(DSP_RW_CTRL, 0);           /* DSP Stop                 */
        DSP_WRITE_REG(DSP_RW_CTRL, 0x000018000); /* Set pc & start DSP       */
#else
        SPR_3MatrixCompoExec(&mtxParm);
#endif
        if(firstClusterFlag)
            drawCluster(befCluster, &clusterViewMatrix[clusterBufSw^1],
                                    &clusterWorldMatrix[clusterBufSw^1]);
#ifdef USE_DSP
        while(((*(volatile Uint32*)0x25fe00a4) & 0x20) == 0);
        while(DSP_READ_REG(DSP_RW_CTRL) & 0x800000); 
        *(Uint32*)0x25fe00a4  = ~0x20;
        *(Uint16*)0xfffffe92 |= 0x10;   /* cash parge */ 
#else
        SPR_3MatrixCompoCheck();
#endif

	if(coordWorld3D) {
	    MTH_MulMatrix(&viewMatrix, worldMatrixTbl.current,
				     &clusterViewMatrix[clusterBufSw]);
            sprMemCpyL(&clusterWorldMatrix[clusterBufSw],
                                            worldMatrixTbl.current, 12);
	} else
            sprMemCpyL(&clusterViewMatrix[clusterBufSw],
                                            worldMatrixTbl.current, 12);
        firstClusterFlag = 1;
        clusterBufSw ^= 1;
        befCluster = cluster;

	/* 子クラスタの処理 */
	if(cluster->child) transCluster(cluster->child);

	MTH_PopMatrix(&worldMatrixTbl);
        cluster = cluster->next;
    }
}


/*****************************************************************************
 *
 * NAME:  drawCluster()  - Draw Cluster
 *
 * PARAMETERS :
 *
 *     (1) SprCluster  *cluster      - <i>  描画クラスタのポインタ
 *     (2) MthMatrix   *viewMatrix   - <i>  視点座標系変換用マトリックス
 *     (3) MthMatrix   *worldMatrix  - <i>  ワールド座標系変換用マトリックス
 *
 * DESCRIPTION:
 *
 *     クラスタの隠面、座標変換処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
drawCluster(SprCluster *cluster, MthMatrix *viewMatrix, MthMatrix *worldMatrix)
{
    SprObject3D  *obj;
    int          mstBoutCmdPos;
    int          slvBoutCmdPos;
    int          computePoly;

/*	95-7-27
    const static MthMatrix	unitMatrix
	= { MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0), MTH_FIXED(0.0),
	    MTH_FIXED(0.0), MTH_FIXED(0.0), MTH_FIXED(1.0), MTH_FIXED(0.0)};
*/

    static const MthMatrix	unitMatrix
	= { 
		{
		{MTH_FIXED(1.0),MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
	    {MTH_FIXED(0.0),MTH_FIXED(1.0),MTH_FIXED(0.0),MTH_FIXED(0.0)},
	    {MTH_FIXED(0.0),MTH_FIXED(0.0),MTH_FIXED(1.0),MTH_FIXED(0.0)},
		}
	  };

    /** BEGIN ***************************************************************/

	obj = cluster->object;
	while(obj) {
#ifdef USE_INBETWEEN_OBJECT
            if(obj->dispFlag & INBETWEEN_OBJECT) {
		sprMemCpyL(viewMatrix, &unitMatrix, 12);
		SPR_3SetSurfNormVect(obj);
            }
#endif
            computePoly = SPR_3SetTransParm(obj, viewMatrix, worldMatrix,
                                                              &transParm);

            mstBoutCmdPos = tMst2DtoCmd.outCmdPos; 
            slvBoutCmdPos = tSlv2DtoCmd.outCmdPos; 

            /* invoke Slave */
            SPR_RunSlaveSH((PARA_RTN*)SPR_3Tran3Dto2D, &tSlv3Dto2D);
            SPR_3Tran3Dto2D(&tMst3Dto2D);
            /* check slave end */
            SPR_WaitEndSlaveSH();

            /* invoke Slave */
            SPR_RunSlaveSH((PARA_RTN*)SPR_3Tran2DtoCmd, &tSlv2DtoCmd);
            SPR_3Tran2DtoCmd(&tMst2DtoCmd);
            /* check slave end */
            SPR_WaitEndSlaveSH();

            if(cluster->transEnd)
                cluster->transEnd(cluster, obj, worldMatrix, coordWorld3D);

            zSortZMin = MIN(tMst2DtoCmd.zSortZMin, tSlv2DtoCmd.zSortZMin);
            zSortZMax = MAX(tMst2DtoCmd.zSortZMax, tSlv2DtoCmd.zSortZMax);

#ifdef USE_DEBUG_INFO
            dbgDrawPol    += ((tMst2DtoCmd.outCmdPos - mstBoutCmdPos) + 
                              (tSlv2DtoCmd.outCmdPos - slvBoutCmdPos)); 
#endif

#ifdef USE_INBETWEEN_OBJECT
            dbgComputePol += computePoly;
	    setInbetPolygons(cluster, obj);
#endif
            obj = obj->next;
	}

}


/*****************************************************************************
 *
 * NAME:  SPR_3SetTransParm()  - Set 3D->2D->Cmd Transfer Parm
 *
 * PARAMETERS :
 *
 *     (1) SprObject3D *obj          - <i>  描画オブジェクトのポインタ
 *     (2) MthMatrix   *viewMatrix   - <i>  視点座標系変換用マトリックス
 *     (3) MthMatrix   *worldMatrix  - <i>  ワールド座標系変換用マトリックス
 *     (4) TransParm   *tParm        - <i>  変換パラメータエリア
 *
 * DESCRIPTION:
 *
 *     3D->2D->Cmd 変換パラメータのセット
 *
 *
 * POSTCONDITIONS:
 *
 *     int  polyCount 計算ポリゴン数
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
int SPR_3SetTransParm(SprObject3D *obj, MthMatrix *viewMatrix,
                                     MthMatrix *worldMatrix, TransParm *tParm)
{
    int          mstVertCount;
    int          mstSurfCount;
    int          slvVertCount;
    int          slvSurfCount;
    Sint32       *wVertBright;

    /** BEGIN ***************************************************************/

   /* グーロー用頂点輝度テーブルを得るためのパラメータセット  */
   wVertBright = 0;
   if((obj->dispFlag & SHADING_MASK) == GOURAUD_SHADING)
       if(obj->dispFlag & INBETWEEN_OBJECT)
           sprMemCpyL(tParm->vertBright, obj->vertNormal,
                                         obj->vertCount);
       else
           wVertBright  = tParm->vertBright;

   /* マスタ、スレーブ共通パラメータのセット */
   tParm->comTbl->viewMatrix     = viewMatrix;
   tParm->comTbl->worldMatrix    = worldMatrix;
   tParm->comTbl->dispFlag       = obj->dispFlag;
   tParm->comTbl->shdIdxTbl      = obj->shdIdxTbl;

   mstVertCount                  = obj->vertCount >> 1;
   mstSurfCount                  = obj->surfaceCount >> 1;
   slvVertCount                  = obj->vertCount    - mstVertCount;
   slvSurfCount                  = obj->surfaceCount - mstSurfCount;

   /* ３Ｄ－＞２Ｄ　*/
   tParm->mst3Dto2D->vertCount     = mstVertCount;      
   tParm->mst3Dto2D->surfaceCount  = mstSurfCount;      
   tParm->mst2DtoCmd->surfaceCount = mstSurfCount; 
   tParm->slv3Dto2D->vertCount     = slvVertCount;
   tParm->slv2DtoCmd->surfaceCount = slvSurfCount; 
   tParm->slv3Dto2D->surfaceCount  = slvSurfCount;
   tParm->mst3Dto2D->surfInfo      = obj->surface;
   tParm->slv3Dto2D->surfInfo      = tParm->mst3Dto2D->surfInfo  +
                                     mstSurfCount;
   tParm->mst2DtoCmd->surface      = tParm->mst3Dto2D->surfInfo; 
   tParm->slv2DtoCmd->surface      = tParm->slv3Dto2D->surfInfo; 

   tParm->mst3Dto2D->vertPoint     = obj->vertPoint;    
   tParm->slv3Dto2D->vertPoint     = tParm->mst3Dto2D->vertPoint +
                                     mstVertCount;

   tParm->mst3Dto2D->surfaceNormal = obj->surfaceNormal;
   tParm->slv3Dto2D->surfaceNormal = tParm->mst3Dto2D->surfaceNormal +
                                     mstSurfCount;

   tParm->mst3Dto2D->coordView3D   = tParm->coordView3D;       
   tParm->slv3Dto2D->coordView3D   = tParm->mst3Dto2D->coordView3D   +
                                     mstVertCount;

   tParm->mst3Dto2D->coord2D       = tParm->coord2D;           
   tParm->slv3Dto2D->coord2D       = tParm->mst3Dto2D->coord2D     +
                                     mstVertCount;

   tParm->mst3Dto2D->surfBright    = tParm->surfBright;        
   tParm->slv3Dto2D->surfBright    = tParm->mst3Dto2D->surfBright +
                                     mstSurfCount;
   tParm->mst2DtoCmd->surfBright   = tParm->mst3Dto2D->surfBright;
   tParm->slv2DtoCmd->surfBright   = tParm->slv3Dto2D->surfBright;

   tParm->mst3Dto2D->vertNormal    = obj->vertNormal;
   if(obj->vertNormal)
      tParm->slv3Dto2D->vertNormal = tParm->mst3Dto2D->vertNormal +
                                     mstVertCount;
   else
      tParm->slv3Dto2D->vertNormal = 0;

   tParm->mst3Dto2D->vertBright    = wVertBright;
   tParm->slv3Dto2D->vertBright    = tParm->mst3Dto2D->vertBright  +
                                     mstVertCount;

   tParm->mst3Dto2D->coordWorld3D  = tParm->coordWorld3D;
   if(tParm->coordWorld3D)
      tParm->slv3Dto2D->coordWorld3D = tParm->mst3Dto2D->coordWorld3D +
                                       mstVertCount;
   else
      tParm->slv3Dto2D->coordWorld3D = 0;

   return (mstSurfCount + slvSurfCount);
}


/*****************************************************************************
 *
 * NAME:  setInbetPolygons  - Set Inbetween Polygon Data to Inbet Object
 *
 * PARAMETERS :
 *
 *     (1) DoubleBufArea  *buf    - <i>  カレント座標変換バッファ
 *
 * DESCRIPTION:
 *
 *     ３Ｄオブジェクト間接続ポリゴン視点座標系頂点データの登録処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
static void
setInbetPolygons(SprCluster *cluster, SprObject3D  *obj)
{
    Sint32       i;
/*    SprObject3D  *inbetObj;		95-7-27	unuse	*/
    SprInbetInf  *wInbetInf;
    MthXyz       *fromVertPoint;
    Sint32       *fromVertBright;
    Uint16       *fromVertNo;
    MthXyz       *toVertPoint;
    Sint32       *toVertBright;
    Uint16       *toVertNo;

    /** BEGIN ***************************************************************/
    /*  ３Ｄオブジェクト間接続ポリゴン情報がある場合     */
    if(cluster->inbetInf) {
	 wInbetInf = cluster->inbetInf;
	 while(wInbetInf) {
	     if(obj == wInbetInf->fromObj) {
		 fromVertPoint = coordView3D;
		 fromVertNo    = wInbetInf->fromVertNo;
		 toVertPoint   = wInbetInf->toObj->vertPoint;
		 toVertNo      = wInbetInf->toVertNo;
		 for(i=0 ; i<wInbetInf->vertCount; i++)
		     toVertPoint[toVertNo[i]] = fromVertPoint[fromVertNo[i]];
		 toVertBright  = (Sint32*)wInbetInf->toObj->vertNormal;
		 if(toVertBright) {
		     fromVertBright = vertBright;
		     for(i=0 ; i<wInbetInf->vertCount; i++)
		         toVertBright[toVertNo[i]] = 
		                              fromVertBright[fromVertNo[i]];
		 }
                 break;
             }
             wInbetInf = wInbetInf->next;
         }
    }
}


/*****************************************************************************
 *
 * NAME:  viewCoordMatrix()  - set View Coord Convert Matrix To
 *                                                    current matrix
 *
 * PARAMETERS :
 *
 * 　　なし
 *
 *
 * DESCRIPTION:
 *
 *     カレントマトリックスに視点座標系への変換マトリックスをセット
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
*/
static void viewCoordMatrix(void)
{
    Uint32           wl;
    /** BEGIN ***************************************************************/
    MTH_InitialMatrix(&viewMatrixTbl, 1, &viewMatrix);

#ifdef USE_DSP
        DSP_WRITE_REG(DSP_RW_CTRL, 0);      /* DSP Stop                     */
        DSP_WRITE_REG(DSP_W_DADR, 0);       /* Write Data Address           */
        wl = ((Uint32)viewMatrixTbl.current) >> 2;
        *dspData = wl;			   /* Write Data                 */
        *dspData = wl;

        *dspData = (Uint32)MTX_OP_REV_Z;
        *dspData = (Uint32)MTX_OP_REV_Z;
        *dspData = (Uint32)MTX_OP_REV_Z;
        *dspData = (Uint32)MTX_OP_REV_Z;

        *dspData = (Uint32)viewCoordPoint.x;
        *dspData = (Uint32)viewCoordPoint.y;
        *dspData = (Uint32)viewCoordPoint.z;
        *dspData = (Uint32)MTX_OP_MOV_XYZ;

        switch(viewAngleSeq) {
            case ROT_SEQ_ZYX :
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
	         break;
            case ROT_SEQ_ZXY :
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
	         break;
            case ROT_SEQ_YZX :
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
	         break;
            case ROT_SEQ_YXZ :
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
	         break;
            case ROT_SEQ_XYZ :
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
	         break;
            case ROT_SEQ_XZY :
                 SPR_3CosSinToDsp(-viewAngle.y);
                 *dspData = (Uint32)MTX_OP_ROT_Y;
                 SPR_3CosSinToDsp( viewAngle.z);
                 *dspData = (Uint32)MTX_OP_ROT_Z;
                 SPR_3CosSinToDsp(-viewAngle.x);
                 *dspData = (Uint32)MTX_OP_ROT_X;
	         break;
        }
        *dspData = (Uint32)-viewPoint.x;
        *dspData = (Uint32)-viewPoint.y;
        *dspData = (Uint32)-viewPoint.z;
        *dspData = (Uint32)MTX_OP_MOV_XYZ;

        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;
        *dspData = (Uint32)MTX_OP_END;

        DSP_WRITE_REG(DSP_RW_CTRL, 0);           /* DSP Stop                 */
        DSP_WRITE_REG(DSP_RW_CTRL, 0x000018000); /* Set pc & start DSP       */

        while(((*(volatile Uint32*)0x25fe00a4) & 0x20) == 0);
        while(DSP_READ_REG(DSP_RW_CTRL) & 0x800000); 
        *(Uint32*)0x25fe00a4  = ~0x20;
        *(Uint16*)0xfffffe92 |= 0x10;   /* cash parge */ 
#else

    MTH_ReverseZ(&viewMatrixTbl);
    MTH_MoveMatrix(&viewMatrixTbl, viewCoordPoint.x,
				   viewCoordPoint.y,
				   viewCoordPoint.z);
    switch(viewAngleSeq) {
        case ROT_SEQ_ZYX :
   	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_ZXY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     break;
        case ROT_SEQ_YZX :
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_YXZ :
             MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     break;
        case ROT_SEQ_XYZ :
  	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
        case ROT_SEQ_XZY :
	     MTH_RotateMatrixY(&viewMatrixTbl, -viewAngle.y);
	     MTH_RotateMatrixZ(&viewMatrixTbl,  viewAngle.z);
	     MTH_RotateMatrixX(&viewMatrixTbl, -viewAngle.x);
	     break;
    }
    MTH_MoveMatrix(&viewMatrixTbl, -viewPoint.x, -viewPoint.y, -viewPoint.z);
#endif

    MTH_NormalTrans(&viewMatrix, &worldLightVector, &viewLightVector);
    if(coordWorld3D) {
        MTH_ClearMatrix(&worldMatrixTbl);
    } else {
	sprMemCpyL(worldMatrixTbl.current, &viewMatrix, 12);
    }
}


/*****************************************************************************
 *
 * NAME:  lightAngle()  -
 *
 * PARAMETERS :
 *
 * 　　MthXyz *lightAngle
 *
 *
 * DESCRIPTION:
 *
 *     光源のアングルを設定する
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
*/
static void setLightAngle(void)
{
    MTH_InitialMatrix(&lightMatrixTbl, 1, &lightMatrix);
    MTH_RotateMatrixZ(&lightMatrixTbl, lightAngle.z);
    MTH_RotateMatrixY(&lightMatrixTbl, lightAngle.y);
    MTH_RotateMatrixX(&lightMatrixTbl, lightAngle.x);
    MTH_NormalTrans(&lightMatrix, &orgLightVect, &worldLightVector);
}

/*  end of file */
