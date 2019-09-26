#include <stdio.h>
#include <string.h>
#include <machine.h>

#include "sega_xpt.h"
#define _SPR2_
#include "sega_spr.h"
#include "spr_mtx3.h"
#include "slv_mtx3.h"

#define VBR   0x06000000
#define SCU_BASE         0x25FE0000
#define SCU_INTR_MASK    (Uint32 *)(SCU_BASE + 0x000000a0)
#define SCU_INTR_STATUS  (Uint32 *)(SCU_BASE + 0x000000a4)
#define SCU_DMA_LENGTH   (Uint32 *)(SCU_BASE + 0x00000008)
#define SCU_DMA_FIRE     (Uint32 *)(SCU_BASE + 0x00000010)
#define SCU_DMA_MAGIC    (Uint32 *)(SCU_BASE + 0x00000014)
#define SETUINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x300)))
#define GETUINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x304)))
#define SETSINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x310)))
#define GETSINT   ((Sint32 (*)()) *((Uint32*)(VBR+0x314)))
#define VBIVN	0x40
#define VBOVN	0x41
#define HBIVN	0x42
extern volatile Uint8 *SMPC_COM;  /* SMPC command register */
extern volatile Uint8 *SMPC_RET;  /* SMPC result register */
extern volatile Uint8 *SMPC_SF;   /* SMPC status flag */
extern const Uint8 SMPC_SSHON;    /* SMPC slave SH on command */
extern const Uint8 SMPC_SSHOFF;   /* SMPC slave SH off command */

void SpFlipVram(void);

#define SCRN_W 320
#define SCRN_H 224

Uint16 drawCount = 0;
/* データのよみこみ */

/* VDPI のコマンドテーブルの初期化部分 */
static GlbSpCmd  spCmdS[] = {
   {/* [-3]     */  /* システムクリップ設定 */
    /* control  */ (JUMP_NEXT | FUNC_SCLIP),
    /* link     */  0,
    /* drawMode */  0,
    /* color    */  0,
    /* charAddr */  0,
    /* charSize */  0,
    /* ax, ay   */  0, 0,
    /* bx, by   */  0, 0,
    /* cx, cy   */  639, 223,
    /* dx, dy   */  0, 0,
    /* grshAddr */  0,
    /* dummy    */  0},

   {/* [-2]     */  /* ローカル座標系設定 */
    /* control  */ (JUMP_NEXT | FUNC_LCOORD),
    /* link     */  0,
    /* drawMode */  0,
    /* color    */  0,
    /* charAddr */  0,
    /* charSize */  0,
    /* ax, ay   */  0, 0,
    /* bx, by   */  0, 0,
    /* cx, cy   */  0, 0,
    /* dx, dy   */  0, 0,
    /* grshAddr */  0,
    /* dummy    */  0},

   {/* [-1]     */ /* ユーザクリップ設定 */
    /* control  */ (JUMP_NEXT | FUNC_UCLIP),
    /* link     */  0,
    /* drawMode */  0,
    /* color    */  0,
    /* charAddr */  0,
    /* charSize */  0,
    /* ax, ay   */  100, 100,
    /* bx, by   */  0, 0,
    /* cx, cy   */  250, 170,
    /* dx, dy   */  0, 0,
    /* grshAddr */  0,
    /* dummy    */  0},

   {/* [ -1]     */ /* Frame Buffer Erase Polygon */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (COLOR_0 | ECDSPD_DISABLE),
    /* color    */ 0x0000,  /* 透明 */
    /* charAddr */ 0x0000,
    /* charSize */ 0x0000,
    /* ax, ay   */   0,   0,
    /* bx, by   */   639, 0,
    /* cx, cy   */   639, 255,
    /* dx, dy   */   0,   255,
    /* grshAddr */   0,
    /* dummy    */   0}
};

/* VDPI のコマンドテーブルのテンプレート */

#define SPRITE_COMMAND \
   {/* [ 0]     */ \
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_NORMALSP),\
    /* link     */ 0, \
    /* drawMode */ (COLOR_5 | COMPO_REP), /* 256 color mode */\
    /* color    */ 0x801f,  /* RED */\
    /* charAddr */ 0x0000,  /*  2048 byte pos */\
    /* charSize */ 0x0000,  /* x * y = 128 * 176 */\
    /* ax, ay   */   0,   0,\
    /* bx, by   */   0,   0,\
    /* cx, cy   */   0,   0,\
    /* dx, dy   */   0,   0,\
    /* grshAddr */   0,\
    /* dummy    */   0}

static GlbSpCmd  spCmdTbl[] = {
/* /////////////////////////////////////// */
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND,
   SPRITE_COMMAND
};

/* VDPI のエンドコマンドのテンプレート */
static GlbSpCmd  spCmdE[] = {
   {/* [ E]     */
    /* control  */ (CTRL_END),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */   0,   0,
    /* bx, by   */   0,   0,
    /* cx, cy   */   0,   0,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0}
};

Uint16	extCtrl = JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON;
Uint16	extMode = COLOR_0 | ECDSPD_DISABLE | COMPO_REP;
Uint16	extSize = 0;
Uint16  PolygonColor = 0xC210; /* 初期化用のポリゴンの色 */

static Uint16 *Spbase[2]; /* VDPI コマンドテーブル(FW, BK)アドレスのテーブル */
static Sint16 FWBank = 0, BKBank = 1;

#define VRAM1		0x25C00000 /* VDPI コマンドテーブルの先頭(FWBank) */
#define VRAM2		0x25C10000 /* VDPI コマンドテーブルの先頭(BKBank) */
#define VRAMSize	   0x10000

Void /* VDPI のレジスタの初期化 */
  VDPI_INIT(void)
{
    *(Uint16 *)(0x25D00000) = 0x0;    /* NTSC, no-turn-around, 16bit/pixel */
    *(Uint16 *)(0x25D00002) = 0x3;    /* change buf to manual change mode  */
    *(Uint16 *)(0x25D00004) = 0x2;    /* trig set to auto start drawing */
    *(Uint16 *)(0x25D00006) = 0x0000;/* set erase write color to see through */
    *(Uint16 *)(0x25D00008) = 0x0;    /* set e/w left top point to 0 */
    *(Uint16 *)(0x25D0000a) = 0xffff; /* set e/w right bottom point to ffff */
}

void /* VDPI のコマンドテーブルに関する各種初期化 */
  sp_init(void)
{
	Sint32 i;
/*	95-7-26 unuse
	Sint32 init_seq;
*/
	Uint16 *wp;

	/*======== Initialize SpCmd BKBank0 ==========*/
	wp = (Uint16 *)VRAM1;
	dmemcpy(wp,spCmdS,sizeof(spCmdS)/2);
	wp += sizeof(spCmdS)/2;
	Spbase[0] = wp;
	/* FB erase polygon command */
	for (i = 0; i < sizeof(spCmdTbl)/sizeof(GlbSpCmd); i++)
	{
	    dmemcpy(wp,&spCmdTbl[i],sizeof(spCmdTbl[i])/2);
	    *(wp) /* control */ = extCtrl;
	    *(wp+2) /* drawmode */ = extMode;
	    *(wp+3) /* color */ = PolygonColor;
	    *(wp+4) /* addr */ =  0x0000;
	    wp += sizeof(spCmdTbl[i])/2;
	}
	dmemcpy(wp,spCmdE,sizeof(spCmdE)/2);

	/*======== Initialize SpCmd BKBank1 =========*/
	wp = (Uint16 *)VRAM2;
	dmemcpy(wp,spCmdS,sizeof(spCmdS)/2);
	wp += sizeof(spCmdS)/2;
	Spbase[1] = wp;
	/* FB erase polygon command */
	for (i = 0; i < sizeof(spCmdTbl)/sizeof(GlbSpCmd); i++)
	{
	    dmemcpy(wp,&spCmdTbl[i],sizeof(spCmdTbl[i])/2);
	    *(wp) /* control */ = extCtrl;
	    *(wp+2) /* drawmode */ = extMode;
	    *(wp+3) /* color bank */ = PolygonColor;
	    *(wp+4) /* addr */ = 0x0000;
	    wp += sizeof(spCmdTbl[i])/2;
	}
	dmemcpy(wp,spCmdE,sizeof(spCmdE)/2);
	VDPI_INIT();
    }

#define MAX_POLYGON_NUM 12 /* 描画を行なうポリゴンの最大数 */

/* マスタ CPU が処理するポリゴン数 */
#define MASTR_POLYGON_NUM MAX_POLYGON_NUM/2 

/* スレーブ CPU が処理するポリゴン数 */
#define SLAV_POLYGON_NUM MAX_POLYGON_NUM/2

#define NUM_OF_MODEL0_SURF 6

#define SCALE 16.0 /* 立方体の一辺の大きさ */
GlbXYZ model0[4*NUM_OF_MODEL0_SURF] = {
    /* polygon 0 */
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    /* polygon 1 */
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)},
    /* polygon 2 */
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)},
    /* polygon 3 */
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)},
    /* polygon 4 */
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED( 1.0*SCALE), FIXED(-1.0*SCALE)},
    /* polygon 5 */
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)},
    {FIXED(-1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED( 1.0*SCALE)},
    {FIXED( 1.0*SCALE), FIXED(-1.0*SCALE), FIXED(-1.0*SCALE)}
}; /* モデルのデータ */

#define EYEDIST FIXED(328.0) /* 視点の距離 */

void /* Z ソートを行なう */
  zSort(Fixed32 *zBuff, Uint16 *SortBuff, Uint16 NumOfElement)
{
    Uint16 i, j, count;

    for(i = 0; i < NumOfElement; i++){
	count = 0;
	for(j = 0; j < NumOfElement; j++){
	    if(zBuff[j] > zBuff[i]){
		count++;
	    }
	}
	SortBuff[count] = i;
    }
}

Uint16 Color[] = {   /* 各ポリゴンの色のテーブル */
    0x801F,
    0xC210,
    0xFC00,
    0x83E0,
    0x821F,
    0xFC1F,

    0x801F,
    0xC210,
    0xFC00,
    0x83E0,
    0x821F,
    0xFC1F
  };

Uint16 orig_x = 160, orig_y = 112; /* 原点のスクリーン上での位置 */

void
  PutSpCommand(GlbXYint FourPoints[], Uint16 *SortBuff, Uint16 CommandNumber)
{
    Uint16 *wp;
    
    wp = Spbase[BKBank]+sizeof(GlbSpCmd)*CommandNumber/2+0x06/2;
    *wp++ = Color[SortBuff[CommandNumber]]; /* Polygon Color */
    wp += 2;
    *wp++ =  orig_x+FourPoints[0].x; /* XA */  /* 左上座標 */
    *wp++ =  orig_y+FourPoints[0].y; /* YA */
    *wp++ =  orig_x+FourPoints[1].x; /* XB */  /* 右上座標 */
    *wp++ =  orig_y+FourPoints[1].y; /* YB */
    *wp++ =  orig_x+FourPoints[2].x; /* XC */  /* 右下座標 */
    *wp++ =  orig_y+FourPoints[2].y; /* YC */
    *wp++ =  orig_x+FourPoints[3].x; /* XD */  /* 左下座標 */
    *wp++ =  orig_y+FourPoints[3].y; /* YD */
}

void
  s_PutSpCommand(GlbXYint FourPoints[], Uint16 *SortBuff, Uint16 CommandNumber)
{
    Uint16 *wp;

    *(Uint16 *)((Uint32)&BKBank + 0x40000000) = 0x0000; /* chache parse */

    wp = Spbase[BKBank]+sizeof(GlbSpCmd)*(CommandNumber+MASTR_POLYGON_NUM)/2+0x06/2;
    *wp++ = Color[SortBuff[CommandNumber]+MASTR_POLYGON_NUM]; /* Polygon Color */
    wp += 2;
    *wp++ =  orig_x+FourPoints[0].x; /* XA */  /* 左上座標 */
    *wp++ =  orig_y+FourPoints[0].y; /* YA */
    *wp++ =  orig_x+FourPoints[1].x; /* XB */  /* 右上座標 */
    *wp++ =  orig_y+FourPoints[1].y; /* YB */
    *wp++ =  orig_x+FourPoints[2].x; /* XC */  /* 右下座標 */
    *wp++ =  orig_y+FourPoints[2].y; /* YC */
    *wp++ =  orig_x+FourPoints[3].x; /* XD */  /* 左下座標 */
    *wp++ =  orig_y+FourPoints[3].y; /* YD */
}

void
  PutSpEndCommand(Uint16 CommandNumber)
{
    Uint16 *wp;
    
    wp = Spbase[BKBank]+sizeof(GlbSpCmd)*CommandNumber/2+0x00/2;
    *wp = 0x8000;
}

void slave(void);
void master(void);

#define CLOCKCHANGE 1

#if	1
/*
**■1995-09-21	高橋智延
**	スレーブ側とのＩ／Ｆは cache through かつ volatile にすること。
*/
	extern volatile void	*SlaveCommand;
	#define	slave_command	*( volatile Uint32 * )(( Uint32 )&SlaveCommand + 0x20000000 )
#endif
void /* マスタ CPU のメイン関数(スレーブ CPU に関数 slave の実行を依頼する) */
  main(void)
{
#if	0
/*
**■1995-09-21	高橋智延
**	スレーブ側とのＩ／Ｆは cache through かつ volatile にすること。
*/
    extern void *SlaveCommand;
#endif
    extern void InitSlaveSH(void);

#if CLOCKCHANGE
    (*(void (*)(Uint32))(*(void **)0x6000320))(0x00000001);
/* clock is 28MHz */
    *(Uint16 *)0x25F80000 = 0x8001;/* screen 352 mode */
#else
    (*(void (*)(Uint32))(*(void **)0x6000320))(0x00000000);
/* clock is 26MHz */
    *(Uint16 *)0x25F80000 = 0x8000;/* screen 320 mode */
#endif
    *(Uint16 *)0x25E00000 = 0xa3e3; /* back color is green */
    *(Uint16 *)0x25F80020 = 0x0000;/* scroll surface disable */
    *(Uint16 *)0x25F800EC = 0x0000;/* Color Mix disable */
    *(Uint16 *)0x25F800D0 = 0x0000;/* nbg0,1 Window disable */
    *(Uint16 *)0x25F800D2 = 0x0000;/* nbg2,3 Window disable */
    *(Uint16 *)0x25F800D4 = 0x0000;/* rbg0,SP Window disable */
    *(Uint16 *)0x25F800D6 = 0x0000;/* Window disable */
    *(Uint16 *)0x25F800e6 = 0x0000;/* SPCAOS disable */
    *(Uint16 *)0x25F800e2 = 0x0000;/* shadow disable */
    *(Uint16 *)0x25F800f0 = 0x0707;/* sprite priority */
    *(Uint16 *)0x25F800f2 = 0x0707;/* sprite priority */
    *(Uint16 *)0x25F800f4 = 0x0707;/* sprite priority */
    *(Uint16 *)0x25F800f6 = 0x0707;/* sprite priority */
    *(Uint16 *)0x25F800f8 = 0x0000;/* scroll priority */
    *(Uint16 *)0x25F800fA = 0x0000;
    *(Uint16 *)0x25F800E0 = 0x0020;

    InitSlaveSH();

    sp_init();
    sp3InitialMatrix();
    s_sp3InitialMatrix();

#if	0
/*
**■1995-09-21	高橋智延
**	スレーブ側とのＩ／Ｆは cache through かつ volatile にすること。
*/
    while(1){
	SlaveCommand = slave;
	*(Uint16 *)0x21000000 = 0xffff; /* slave FRT inp invoke */
	master();
/* 	1995-07-11 xxx */
/*	while(*(Uint32 *)((Uint32)&SlaveCommand + 0x20000000) != 0);	*/
	while(*(volatile Uint32 *)((Uint32)&SlaveCommand + 0x20000000) != 0);
	SpFlipVram();
	*(Uint16 *)0x25D00002 = 0x0003;
    }
#else
	for(;;){
		slave_command = ( Uint32 )slave;
		*( Uint16 * )0x21000000 = 0xffff;
		
		master();
		
		while( slave_command != 0 )
			;
		
		SpFlipVram();
		*( Uint16 * )0x25d00002 = 0x0003;
	}
#endif
}

void /* マスタ CPU が実行する関数 */
  master(void)
{	
/*	95-7-26	unuse
    Uint16 *wp;
*/  
    static Sint16 arg = 45;
    static Fixed32 cube0_x = FIXED(0.0), cube0_dx = -FIXED(1.7);
    static Fixed32 cube0_y = FIXED(0.0), cube0_dy = -FIXED(1.3);
    static Fixed32 cube0_z = FIXED(413.0), cube0_dz = FIXED(2.7);
    Uint16 i, j;
    Uint16 PolyNumber;
    GlbXYZ Coord3D[4*MASTR_POLYGON_NUM]; /* 回転後の座標データバッファ */
    GlbXYint  Coord2D[4*MASTR_POLYGON_NUM]; /* 透視変換後のデータバッファ */
    Fixed32 zBuff[MASTR_POLYGON_NUM];    /* 各頂点の Z 値のバッファ    */
    Uint16 SortBuff[MASTR_POLYGON_NUM];  /* Z 値でソートした頂点番号の並び */

    arg++;
    /* Control cube0 movements. */
    cube0_x += cube0_dx;
    cube0_y += cube0_dy;
    cube0_z += cube0_dz;
    if(180 <= arg){
	arg = -180;
    }
    if((cube0_x >= FIXED(0.0)) || (cube0_x <= -FIXED(150.0))){
	cube0_dx = -cube0_dx;
    }
    if((cube0_y >= FIXED(100.0)) || (cube0_y <= -FIXED(100.0))){
	cube0_dy = -cube0_dy;
    }
    if((cube0_z >= FIXED(600.2)) || (cube0_z <= FIXED(300.0))){
	cube0_dz = -cube0_dz;
    }
    
    /* model0 Matrix calc */
    sp3PushMatrix();
    sp3MoveMatrix(cube0_x, cube0_y, cube0_z);
    sp3RotateMatrixX(FIXED(arg));
    sp3RotateMatrixY(FIXED(arg));
    sp3RotateMatrixZ(FIXED(arg));
    /* model 0 coord trans */
    sp3CoordTrans(4*NUM_OF_MODEL0_SURF, &model0[0], &Coord3D[0]);
    
    /* ポリゴンの Z sort のための z 値（4 頂点の平均 * 4) を得る */
    for(PolyNumber = 0;
	PolyNumber < MASTR_POLYGON_NUM;
	PolyNumber++){
	zBuff[PolyNumber]
	  = Coord3D[PolyNumber*4].z
	    + Coord3D[PolyNumber*4+1].z
	      + Coord3D[PolyNumber*4+2].z
		+ Coord3D[PolyNumber*4+3].z;
	SortBuff[PolyNumber] = PolyNumber;
    }
    
    /* 昇順に Z sort */
    zSort(zBuff, SortBuff, MASTR_POLYGON_NUM);
    
    /* ソートされた順番に 透視変換 */
    for(i = 0; i < MASTR_POLYGON_NUM; i++){
	PolyNumber = SortBuff[i];
	for(j = 0; j < 4; j++){
	    Coord2D[i*4+j].x
	      = FIXED_TO_INT(MUL_FIXED(DIV_FIXED(Coord3D[PolyNumber*4+j].x
						 ,Coord3D[PolyNumber*4+j].z)
				       ,EYEDIST));
	    Coord2D[i*4+j].y
	      = FIXED_TO_INT(MUL_FIXED(DIV_FIXED(Coord3D[PolyNumber*4+j].y
						 ,Coord3D[PolyNumber*4+j].z)
				       ,EYEDIST));
	}
    }
    /* スプライトコマンド生成, 転送 */
    for(i = 0; i < MAX_POLYGON_NUM/2; i++){
	PutSpCommand(&Coord2D[i*4], SortBuff, i);
    }
    PutSpEndCommand(MAX_POLYGON_NUM);
    
    sp3PopMatrix();
}

void /* スレーブ CPU が、実行する関数 */
  slave(void)
{	
/*	95-7-26	unuse
    Uint16 *wp;
*/
    static Sint16 arg = 45;
    static Fixed32 cube1_x = FIXED(3.0), cube1_dx = FIXED(2.7);
    static Fixed32 cube1_y = FIXED(12.0), cube1_dy = -FIXED(2.3);
    static Fixed32 cube1_z = FIXED(412.7), cube1_dz = FIXED(3.7);
    Uint16 i, j;
    Uint16 PolyNumber;
    GlbXYZ Coord3D[4*SLAV_POLYGON_NUM]; /* 回転後の座標データバッファ */
    GlbXYint  Coord2D[4*SLAV_POLYGON_NUM]; /* 透視変換後のデータバッファ */
    Fixed32 zBuff[SLAV_POLYGON_NUM];    /* 各頂点の Z 値のバッファ    */
    Uint16 SortBuff[SLAV_POLYGON_NUM];  /* Z 値でソートした頂点番号の並び */

    arg++;
    /* Control cube1 movements. */
    cube1_x += cube1_dx;
    cube1_y += cube1_dy;
    cube1_z += cube1_dz;
    if(180 <= arg){
	arg = -180;
    }
    if((cube1_x >= FIXED(150.0)) || (cube1_x <= -FIXED(0.0))){
	cube1_dx = -cube1_dx;
    }
    if((cube1_y >= FIXED(100.0)) || (cube1_y <= -FIXED(100.0))){
	cube1_dy = -cube1_dy;
    }
    if((cube1_z >= FIXED(600.2)) || (cube1_z <= FIXED(300.0))){
	cube1_dz = -cube1_dz;
    }

    /* cube 1 Matrix calc */
    s_sp3PushMatrix();
    s_sp3MoveMatrix(cube1_x, cube1_y, cube1_z);
    s_sp3RotateMatrixX(FIXED(-arg));
    s_sp3RotateMatrixY(FIXED(arg));
    s_sp3RotateMatrixZ(FIXED(-arg));
    /* cube 1 coord trans */
    s_sp3CoordTrans(4*NUM_OF_MODEL0_SURF, &model0[0], &Coord3D[0]);
    
    /* ポリゴンの Z sort のための z 値（4 頂点の平均 * 4) を得る */
    for(PolyNumber = 0;
	PolyNumber < SLAV_POLYGON_NUM;
	PolyNumber++){
	zBuff[PolyNumber]
	  = Coord3D[PolyNumber*4].z
	    + Coord3D[PolyNumber*4+1].z
	      + Coord3D[PolyNumber*4+2].z
		+ Coord3D[PolyNumber*4+3].z;
	SortBuff[PolyNumber] = PolyNumber;
    }
    
    /* 昇順に Z sort */
    zSort(zBuff, SortBuff, SLAV_POLYGON_NUM);
    
    /* ソートされた順番に 透視変換 */
    for(i = 0; i < SLAV_POLYGON_NUM; i++){
	PolyNumber = SortBuff[i];
	for(j = 0; j < 4; j++){
	    Coord2D[i*4+j].x
	      = FIXED_TO_INT(MUL_FIXED(DIV_FIXED(Coord3D[PolyNumber*4+j].x
						 ,Coord3D[PolyNumber*4+j].z)
				       ,EYEDIST));
	    Coord2D[i*4+j].y
	      = FIXED_TO_INT(MUL_FIXED(DIV_FIXED(Coord3D[PolyNumber*4+j].y
						 ,Coord3D[PolyNumber*4+j].z)
				       ,EYEDIST));
	}
    }
    /* スプライトコマンド生成, 転送 */
    for(i = 0; i < SLAV_POLYGON_NUM; i++){
	s_PutSpCommand(&Coord2D[i*4], SortBuff, i);
    }
    s_sp3PopMatrix();
}

/* =================================================================== */
void
  SpFlipVram(void)
{
    /* コマンドテーブルを切替える */
    if (BKBank == 1) {
	*(Uint16 *)(VRAM1) = 0x9 /*(JUMP_NEXT | FUNC_SCLIP)*/;
	*(Uint16 *)(VRAM1+0x2) = 0;
    } else {
	*(Uint16 *)(VRAM1) = 0x1009 /*(JUMP_ASSIGN | FUNC_SCLIP)*/;
	*(Uint16 *)(VRAM1+0x2) = (0xfffe0 & (Uint32)VRAM2)/8;
    }
    FWBank = 1 - FWBank; /* 1 -> 0; 0 -> 1 */
    BKBank = 1 - BKBank; /* 1 -> 0; 0 -> 1 */
}
