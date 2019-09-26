/*----------------------------------------------------------------------------
 *  sega_spr.h -- SPR ライブラリ インクルードファイル
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-02-15 Ver.0.80
 *
 *  スプライト表示用インクルードファイルで使用スプライト表示ライブラリの種別に
 *  より以下のように定義します。
 *
 *  （１）スプライト表示基本処理のみの場合
 *
 *        #include "sega_spr.h"
 *
 *  （２）スプライト表示拡張処理の場合
 *
 *        #define  _SPR2_
 *        #include "sega_spr.h"
 *
 *  （３）スプライト３Ｄ表示処理の場合
 *
 *        #define  _SPR3_
 *        #include "sega_spr.h"
 *
 *
 *----------------------------------------------------------------------------
 */

#ifndef SEGA_SPR_H
#define SEGA_SPR_H

/*-----------------------------------
 * Select 3D Execute Machine
 *---------------------------------*/
#define  _PC_         0
#define  _INDIGO2_    1
#define  _SH_         2


/*
 * Set Machine Environment
 */
#define NO_EXIST    0
#define    EXIST    1

#if defined(_PC)
   #define _MACHINE  _PC_
   #define _DSP      NO_EXIST
   #define _MATH        EXIST
#endif

#if defined(_INDIGO2)
   #define _MACHINE  _INDIGO2_
   #define _DSP      NO_EXIST
   #define _MATH        EXIST
#endif

#if defined(_SH)
   #define _MACHINE  _SH_
   #define _DSP         EXIST
   #define _MATH     NO_EXIST
#endif

#ifndef SSV_XPORT_H
#define SSV_XPORT_H
/*
 * C VIRTUAL TYPES DECLARATIONS
 */
typedef char                  Char;
typedef int                   Int;
typedef long                  Long;
typedef float                 Float;
typedef double                Double;
typedef Sint32                Fixed32;    /* Fixed-point representation         */
					  /* Radix point is between bit 15 & 16 */

typedef struct  GlbXY {
	 Fixed32         x;
	 Fixed32         y;
} GlbXY;

typedef struct  GlbXYZ {
	 Fixed32         x;
	 Fixed32         y;
	 Fixed32         z;
} GlbXYZ;

/*
 * CONSTANT DATA DECLARATIONS
 */
#define BOOLEAN               Uint8
#define Boolean               Uint8
/*
#ifndef FALSE
#define FALSE                 0
#endif
*/
#ifndef TRUE
#define TRUE                  1
#endif

#ifndef NIL
#define NIL                   0
#endif

#ifndef VOID
#define VOID                  void
#endif

#ifndef Void
#define Void                  void
#endif

#ifndef OK
#define OK                    0
#endif

#ifndef NG
#define NG                    -1
#endif

/*
 * MACROS DECLARATIONS
 */
#define PROTOTYPE(returnType, fName, pList) returnType fName pList

#define FIXED(x)	      ((Fixed32)((x) * 65536.0))
#define FIXED_TO_FLOAT(x)     ((Float32)((long)(x) / 65536.0))

#if(_MACHINE == _SH_)
#define INT_TO_FIXED(x)       SPR_ITOF(x)
#define FIXED_TO_INT(x)       SPR_FTOI(x)
#define MUL_FIXED(a, b)       SPR_MULF(a, b)
#define DIV_FIXED(a, b)       SPR_DIVF(a, b)
PROTOTYPE(Fixed32, SPR_ITOF, (int x));
PROTOTYPE(int,     SPR_FTOI, (Fixed32 x));
PROTOTYPE(Fixed32, SPR_MULF, (Fixed32 a, Fixed32 b));
PROTOTYPE(Fixed32, SPR_DIVF, (Fixed32 a, Fixed32 b));
#else
#define INT_TO_FIXED(x)       ((Fixed32)((x) * 65536))
#define FIXED_TO_INT(x)       ((Sint32)((x) / 65536))
#define MUL_FIXED(a, b)       (((a) / 256) * ((b) / 256))
#define DIV_FIXED(a, b)       (((a) / ((b) / 256)) * 256)
#endif

PROTOTYPE(Fixed32, fxsin, (Fixed32 degree));
PROTOTYPE(Fixed32, fxcos, (Fixed32 degree));
PROTOTYPE(Void, dmemcpy, (Void *dst, Void *src, Uint32 wsize));

#endif /* ifndef SSV_XPORT_H */


#ifdef _SPR3_
   #define _SPR2_
#endif


/**************** SEGA_SP1.H *************************************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

#define  RGB(r,g,b)  ((b<<10) + (g<<5) + r + 0x8000)

#define  VRAM_SIZE           512*1024   /* 512 Kbyte                        */

#define  AUTO_FRAME_CHG      0          /* auto frame change mode           */
#define  MANUAL_FRAME_CHG    1          /* manual frame change mode         */
#define  NO_INTERVAL_FRAME_CHG   2      /* no interval frame change mode    */
#define  VRAM_0              0          /* VRAM Bank Change Parm to VRAM_0  */
#define  VRAM_1              1          /* VRAM Bank Change Parm to VRAM_1  */
#define  VRAM_REVERSE        2          /* VRAM Bank Change Parm to Reverse */

#define  DISP_XSIZE          320        /* TV Display X Size                */
#define  DISP_YSIZE          224        /* TV Display Y Size                */

#define  TV_MODE             0x00000000 /* TV mode selection                */
#define  FRAME_BUF_MODE      0x00000002 /* Frame buf change mode            */
#define  PLOT_TRIGGER        0x00000004 /* Draw start control               */
#define  ERASE_DATA          0x00000006 /* Erase write data                 */
#define  ERASE_LT_POINT      0x00000008 /* Erase write area left top point  */
#define  ERASE_RB_POINT      0x0000000A /* Erase write area right bot point */
#define  END_STATUS          0x00000010 /* Transfer complete status         */
#define  END_TABLE_ADDR      0x00000012 /* End command table address        */

#define  VRAM_ADDR           0x25C00000 /* Sprite IC VRAM area address      */
#define  VDP1_REG_BASE       0x25D00000 /* Sprite IC Register base address  */
#define  VDP2_REG_BASE       0x25F80000 /* Sprite IC Register base address  */

struct GlbSpStatus {
    Uint16  SpFrameChgMode;             /* Frame Change Mode                 */
    Uint16  SpFrameEraseMode;           /* Frame Erase  Mode                 */
    Uint16  VBInterval;                 /* V-Blank Interval for Frame Change */
    Uint16  eraseData;
    Uint16  eraseLeftX;
    Uint16  eraseTopY;
    Uint16  eraseRightX;
    Uint16  eraseBotY;
};

typedef struct GlbSpStatus GlbSpStatus;

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern Sint16  SpScreenX;               /* TV Screen X Size                  */
extern Sint16  SpScreenY;               /* TV Screen Y Size                  */
extern Uint16  SpFrameChgMode;          /* Frame Change Mode                 */
extern Uint16  SpFrameEraseMode;        /* Frame Erase  Mode                 */
extern Uint16  VBInterval;              /* V-Blank Interval for Frame Change */
extern Uint16  VBIntervalCounter;       /* V-Blank Interval Counter          */
extern Uint16  ReqDisplayFlag;          /* Display Request flag              */

/*
 * EXTERNAL FUNCTION PROTOTYPE DECLARATIONS
 */
extern PROTOTYPE(VOID, SPR_Initial, (Uint8 **VRAM, Uint8 **CharVRAM));
extern PROTOTYPE(VOID, SPR_ChangeFrame, (Void));
extern PROTOTYPE(VOID, SPR_GetStatus, (GlbSpStatus *spStatus));
extern PROTOTYPE(VOID, SPR_SetEraseData, (Uint16 eraseData,
                                   Uint16 leftX,  Uint16 topY,
                                   Uint16 rightX, Uint16 botY));
extern PROTOTYPE(VOID, SPR_SetEraseMode, (Uint16 mode));
extern PROTOTYPE(VOID, SPR_SetNextFrameErase, (Void));
extern PROTOTYPE(VOID, SPR_WaitDrawEnd, (Void));


#ifdef _SPR2_
/**************** SEGA_SP2.H *************************************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define  CTRL_END            0x8000     /* control word end bit mask        */
#define  CTRL_SKIP           0x4000     /* control word skip bit mask       */
#define  CTRL_JUMP           0x3000     /* control word jump bit mask       */
#define  CTRL_ZOOM           0x0f00     /* control word zoom point bit mask */
#define  CTRL_FUNC           0x000f     /* control word function bit mask   */
#define  CTRL_DIR            0x0030     /* control word char read bit mask  */

#define  DRAW_COMPO          0x0003     /* draw mode word color compose mask*/
#define  DRAW_GOURAU         0x0004     /* draw mode word gouraud shading   */
#define  DRAW_GOURAU_GRAY    0x0804     /* draw mode word gray gour shading */
#define  DRAW_COLOR          0x0038     /* draw mode word color mode mask   */
#define  DRAW_MESH           0x0100     /* draw mode word mesh on           */

#define  COMPO_REP           0x0000     /* color compose reprace mode       */
#define  COMPO_SHADOW        0x0001     /* color compose shadow mode        */
#define  COMPO_HARF          0x0002     /* color compose harf luminance mode*/
#define  COMPO_TRANS         0x0003     /* color compose trunslucent mode   */

#define  FUNC_NORMALSP       0x0000     /* draw normal sprite function      */
#define  FUNC_SCALESP        0x0001     /* draw scaled sprite function      */
#define  FUNC_DISTORSP       0x0002     /* draw distorted sprite function   */
#define  FUNC_POLYGON        0x0004     /* draw polygon function            */
#define  FUNC_POLYLINE       0x0005     /* draw polyline function           */
#define  FUNC_LINE           0x0006     /* draw line function               */
#define  FUNC_SCLIP          0x0009     /* set system clipping function     */
#define  FUNC_UCLIP          0x0008     /* set user clipping function       */
#define  FUNC_LCOORD         0x000a     /* set local coordinate function    */
#define  FUNC_TEXTURE        0x0004     /* texture command group mask       */

#define  JUMP_NEXT           0x0000     /* jump next command                */
#define  JUMP_ASSIGN         0x1000     /* jump assign command              */
#define  JUMP_CALL           0x2000     /* call assign command              */
#define  JUMP_RETURN         0x3000     /* return command subroutine        */

#define  ZOOM_NOPOINT        0x0000     /* zoom no point                    */
#define  ZOOM_TL             0x0500     /* zoom point top left              */
#define  ZOOM_TM             0x0600     /* zoom point top middle            */
#define  ZOOM_TR             0x0700     /* zoom point top right             */
#define  ZOOM_ML             0x0900     /* zoom point middle left           */
#define  ZOOM_MM             0x0a00     /* zoom point center                */
#define  ZOOM_MR             0x0b00     /* zoom point middle right          */
#define  ZOOM_BL             0x0c00     /* zoom point bottom left           */
#define  ZOOM_BM             0x0e00     /* zoom point bottom middle         */
#define  ZOOM_BR             0x0f00     /* zoom point bottom right          */

#define  DIR_NOREV           0x0000     /* char read not reverse            */
#define  DIR_TBREV           0x0020     /* char read top/bottom reverse     */
#define  DIR_LRREV           0x0010     /* char read left/right reverse     */
#define  DIR_LRTBREV         0x0030     /* char read left/right/top/bot rev */

#define  ECD_DISABLE         0x0080     /* ECD disabe & SPD enable          */
#define  SPD_DISABLE         0x0040     /* ECD enable & SPD disable         */
#define  ECDSPD_DISABLE      0x00c0     /* ECD & SPD disable                */
#define  UCLPIN_ENABLE       0x0400     /* CLIP IN enable                   */
#define  UCLPOUT_ENABLE      0x0600     /* CLIP OUT enable                  */

#define  COLOR_0             0x0000     /* 4 bit/pixel & 16 color bank mode */
#define  COLOR_1             0x0008     /* 4 bit/pixel & 16 color lookup tbl*/
#define  COLOR_2             0x0010     /* 8 bit/pixel &  64 color bank mode*/
#define  COLOR_3             0x0018     /* 8 bit/pixel & 128 color bank mode*/
#define  COLOR_4             0x0020     /* 8 bit/pixel & 256 color bank mode*/
#define  COLOR_5             0x0028     /* 16 bit/pixel & RGB mode          */

#define  NO_GOUR             0xffff     /* no use Gouraud Shading Table No. */


/*
 * STRUCTURE DECLARATIONS
 */
struct GlbSpCmd {                       /* Sprite Command Table             */
    Uint16  control;                    /* control word                     */
    Uint16  link;                       /* command link                     */
    Uint16  drawMode;                   /* draw mode                        */
    Uint16  color;                      /* color info.                      */
    Uint16  charAddr;                   /* character address                */
    Uint16  charSize;                   /* character size                   */
    Uint16  ax;                         /* point A x                        */
    Uint16  ay;                         /* point A y                        */
    Uint16  bx;                         /* point B x                        */
    Uint16  by;                         /* point B y                        */
    Uint16  cx;                         /* point C x                        */
    Uint16  cy;                         /* point C y                        */
    Uint16  dx;                         /* point D x                        */
    Uint16  dy;                         /* point D y                        */
    Uint16  grshAddr;                   /* gouraud shading table address    */
    Uint16  dummy;                      /* dummy area                       */
};

struct GlbGourTbl {
    Uint16  entry[4];
};

struct GlbLookupTbl {
    Uint16  entry[16];
};

struct GlbXYint {
    Sint16  x;
    Sint16  y;
};

/*
 * TYPEDEFS
 */
typedef Uint16 Vaddr;                       /* Relative Address in The VRAM  */
typedef struct GlbSpCmd     GlbSpCmd;
typedef struct GlbGourTbl   GlbGourTbl;
typedef struct GlbLookupTbl GlbLookupTbl;
typedef struct GlbXYint     GlbXYint;
/* internal data type */
typedef	struct GlbCharTbl   GlbCharTbl;
typedef	struct Glb2WorkArea Glb2WorkArea;

struct GlbCharTbl {                         /* Character Entry Table         */
    Vaddr     addr;                         /* Character Relative Address    */
    Uint16    xySize;                       /* Charcter Size (X * Y)         */
    Uint16    mode;                         /* Charcter Mode (0-5)           */
    Uint16    size;                         /* Charcter Size (by the 32 byte)*/
};

struct Glb2WorkArea {                       /* SP2 Work Area Define          */
    Uint16    commandMax;                   /* Command Max                   */
    Uint16    gourTblMax;                   /* Gour Tbl Max                  */
    Uint16    lookupTblMax;                 /* Lookup Tbl Max                */
    Uint16    charMax;                      /* Char Max                      */
    GlbCharTbl *charTbl;                    /* Character Tbl Pointer         */
};

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern Sint16  SpLCoordX;                   /* Local Origin X Position       */
extern Sint16  SpLCoordY;                   /* Local Origin Y Position       */


#define   SPR_2DefineWork(sp2WorkArea, sp2CommandMax, sp2GourTblMax, sp2LookupTblMax,sp2CharMax) \
static GlbCharTbl  charTbl[sp2CharMax]; \
static Glb2WorkArea sp2WorkArea  = { sp2CommandMax, sp2GourTblMax, \
                             sp2LookupTblMax, sp2CharMax, charTbl };


/*
 * PROTOTYPE DECLARATIONS
 */
PROTOTYPE(Void  , SPR_2Initial, (Glb2WorkArea  *workArea));
PROTOTYPE(Void  , SPR_2FrameChgIntr, (Uint16));
PROTOTYPE(Void  , SPR_2FrameEraseData, (Uint16));
PROTOTYPE(Void  , SPR_2SetGourTbl, (Uint16, GlbGourTbl *));
PROTOTYPE(Void  , SPR_2SetLookupTbl, (Uint16, GlbLookupTbl *));
PROTOTYPE(Void  , SPR_2SetChar, (Uint16, Uint16, Uint16, Uint16, Uint8 *));
PROTOTYPE(Void  , SPR_2ClrChar, (Uint16));
PROTOTYPE(Void  , SPR_2ClrAllChar, (Void));
PROTOTYPE(Uint16, SPR_2GourTblNoToVram, (Uint16));
PROTOTYPE(Uint16, SPR_2LookupTblNoToVram, (Uint16));
PROTOTYPE(Uint16, SPR_2CharNoToVram, (Uint16));
PROTOTYPE(Void  , SPR_2OpenCommand, (Void));
PROTOTYPE(Void  , SPR_2CloseCommand, (Void));
PROTOTYPE(Void  , SPR_2LocalCoord, (GlbXYint *));
PROTOTYPE(Void  , SPR_2SysClip, (GlbXYint *));
PROTOTYPE(Void  , SPR_2UserClip, (GlbXYint *));
PROTOTYPE(Void  , SPR_2Line, (Uint16, Uint16, GlbXYint *, Uint16));
PROTOTYPE(Void  , SPR_2PolyLine, (Uint16, Uint16, GlbXYint *, Uint16));
PROTOTYPE(Void  , SPR_2Polygon, (Uint16, Uint16, GlbXYint *, Uint16));
PROTOTYPE(Void  , SPR_2NormSpr, (Uint16, Uint16, Uint16, 
				       Uint16, GlbXYint *, Uint16));
PROTOTYPE(Void  , SPR_2ScaleSpr, (Uint16, Uint16, Uint16,
					Uint16, GlbXYint *xy, Uint16));
PROTOTYPE(Void  , SPR_2DistSpr, (Uint16, Uint16, Uint16,
                                       Uint16, GlbXYint *xy, Uint16));
PROTOTYPE(Void  , SPR_2Cmd, (GlbSpCmd *));
PROTOTYPE(Uint16, SPR_2GetNextCmdNo, (void));
PROTOTYPE(Void  , SPR_2SetCmdLink, (Uint16 cmdNo, Uint16 linkNo));

#endif /* _SPR2_ */

#ifdef _SPR3_
/**************** SEGA_SP3.H *************************************************/
/*
 * TYPEDEFS
 */
typedef	struct GlbCluster  GlbCluster;
typedef	struct GlbObject3D GlbObject3D;
typedef	struct GlbSurface  GlbSurface;
typedef	struct GlbTexture  GlbTexture;
/* internal data type */
typedef	struct GlbCmdChain  GlbCmdChain;
typedef	struct Glb3WorkArea Glb3WorkArea;


/*
 * STRUCTURE DECLARATIONS
 */
struct GlbCluster {
    GlbObject3D	*object;        /* ３Ｄオブジェクト                          */
    GlbCluster	*next;       	/* 次クラスタ                                */
    GlbCluster	*child;      	/* 子クラスタ                                */
    Uint16	angleSeq;       /* 回転順序                                  */
    GlbXYZ	angle;	        /* 親クラスタの座標系における回転            */
    GlbXYZ	point;	        /* 親クラスタの座標系における原点の座標      */
    GlbXYZ	worldPoint;     /* ワールド座標系における原点の座標          */
};

struct GlbObject3D {
    Uint16      dispFlag;       /* 表示フラグ                                */
    Uint16      vertCount;      /* 頂点数                                    */
    Uint16      surfaceCount;   /* 面数                                      */
    GlbXYZ      *vertPoint;     /* 頂点座標テーブル                          */
    GlbXYZ      *vertNormal;    /* 頂点の法線ベクトルテーブル                */
    GlbSurface  *surface;       /* 面定義テーブル                            */
    GlbXYZ      *surfaceNormal; /* 面の法線ベクトルテーブル                  */
    Uint16      **shdIdxTbl;    /* シェーディングインデックステーブル        */
    GlbObject3D *next;          /* 次３Ｄオブジェクト                        */
};

struct GlbSurface {
    Uint16     vertNo[4];       /* 面を構成する頂点番号                      */
    Uint16     drawMode;        /* 描画モード                                */
    Uint16     color;           /* カラーデータ                              */
};

struct GlbTexture {
    Uint16       charNo;        /* 面を構成する頂点番号                      */
    Uint16       colorMode;     /* カラーモード                              */
    Uint16       width;         /* キャラクタの幅                            */
    Uint16       height;        /* キャラクタの高さ                          */
    Uint8        *charData;     /* キャラクタデータのポインタ                */
    GlbLookupTbl *lookupTbl;    /* ルックアップテーブルのポインタ            */
};

struct GlbCmdChain {
    Fixed32     z;              /* Ｚソート代表値                            */
    GlbCmdChain *next;          /* 次のコマンドチェインエントリポインタ      */
    Uint16      cmdNo;          /* コマンド番号                              */
};

struct Glb3WorkArea {
    Uint16      drawPolMax;     /* 描画ポリゴン最大数                        */
    Uint16      objSurfMax;     /* オブジェクト内最大面数                    */
    Uint16      objVertMax;     /* オブジェクト内最大頂点数                  */
    Uint16      sortBlkMax;     /* ソートブロック最大エントリ数              */
    Uint8       *surfBright;    /* 面輝度テーブル                            */
    Uint8       *vertBright;    /* 頂点輝度テーブル                          */
    GlbXYZ      *coord3D;       /* ３Ｄ座標変換後テーブル                    */
    GlbXYint    *coord2D;       /* ２Ｄ透視変換後テーブル                    */
    GlbCmdChain *cmdChain;      /* 描画ポリゴンコマンドチェインテーブル      */
    GlbCmdChain **zSortBlk;     /* Ｚソートブロックエリア                    */
};


/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define   ROT_SEQ_ZYX       0x0000
#define   ROT_SEQ_ZXY       0x0001
#define   ROT_SEQ_YZX       0x0002
#define   ROT_SEQ_YXZ       0x0003
#define   ROT_SEQ_XYZ       0x0004
#define   ROT_SEQ_XZY       0x0005

#define   DISP_POLYGON      0x0000
#define   DISP_POLYLINE     0x8000
#define   DISP_MASK         0xc000

#define   NO_SHADING        0x0000
#define   FLAT_SHADING      0x0100
#define   GOURAUD_SHADING   0x0200
#define   SHADING_MASK      0x0300

#define   MATE_COLOR        0x0000
#define   MATE_TEXTURE      0x4000
#define   MATE_SHADING_TBL  0x8000
#define   MATE_AUTO_SHADING 0xc000
#define   MATE_MASK         0xc000

#define   ZSORT_MIN         0x0000
#define   ZSORT_MAX         0x1000
#define   ZSORT_MID         0x2000
#define   ZSORT_MASK        0x3000

#define   DRAW_MODE_MASK    0x0fff

#define   SPR_3mac1(sp3ObjSurfMax) \
static Uint8     sp3SurfBright  [sp3ObjSurfMax]; \
static GlbXYZ    sp3SurfCenter  [sp3ObjSurfMax];

#define   SPR_3mac2(sp3ObjVertMax) \
static Uint8     sp3VertBright  [sp3ObjVertMax];   \
static GlbXYZ    sp3Coord3D     [sp3ObjVertMax];   \
static GlbXYint  sp3Coord2D     [sp3ObjVertMax];

#define   SPR_3mac3(sp3DrawPolMax) \
static GlbCmdChain  sp3CmdChain   [sp3DrawPolMax];

#define   SPR_3mac4(sp3SortBlkMax) \
static GlbCmdChain *sp3ZSortBlk   [sp3SortBlkMax];

#define   SPR_3mac5(sp3WorkArea,   sp3DrawPolMax, sp3ObjSurfMax, sp3ObjVertMax, sp3SortBlkMax)  \
static Glb3WorkArea  sp3WorkArea = {                          \
			      sp3DrawPolMax , sp3ObjSurfMax , \
                              sp3ObjVertMax , sp3SortBlkMax , \
			      sp3SurfBright , sp3VertBright , \
                              sp3Coord3D    , sp3Coord2D    , \
			      sp3CmdChain   , sp3ZSortBlk   };

#define   SPR_3DefineWork(sp3WorkArea,   sp3DrawPolMax, sp3ObjSurfMax, sp3ObjVertMax, sp3SortBlkMax) \
 SPR_3mac1(sp3ObjSurfMax) \
 SPR_3mac2(sp3ObjVertMax) \
 SPR_3mac3(sp3DrawPolMax) \
 SPR_3mac4(sp3SortBlkMax) \
 SPR_3mac5(sp3WorkArea, sp3DrawPolMax, sp3ObjSurfMax, \
			sp3ObjVertMax, sp3SortBlkMax)

/*
 * PROTOTYPE DECLARATIONS
 */
PROTOTYPE(void  , SPR_3Initial,       (Glb3WorkArea  *workArea));
PROTOTYPE(void  , SPR_3SetLight,      (Uint16 moveKind, GlbXYZ *lightAngle));
PROTOTYPE(void  , SPR_3SetView,       (Uint16 moveKind, GlbXYZ *pivotViewPoint,
						      GlbXYZ *viewPoint,
						      GlbXYZ *viewAngle));
PROTOTYPE(void  , SPR_3MoveCluster,   (GlbCluster *cluster, Uint16 moveKind,
					   GlbXYZ *angle, GlbXYZ *point));
PROTOTYPE(void  , SPR_3DrawModel,     (GlbCluster *rootCluster));
PROTOTYPE(void  , SPR_3Flush,         (void));
PROTOTYPE(void  , SPR_3SetTexture,    (GlbTexture *texture));
PROTOTYPE(void  , SPR_3ClrTexture,    (GlbTexture *texture));
PROTOTYPE(void  , SPR_3SetZSortLevel, (Uint16 pZSortLevel,
				       Fixed32 pZSortZMin, Fixed32 pZSortZMax));
PROTOTYPE(void  , SPR_3SetClipLevel,  (Uint16 pClipLevel,
				       Fixed32 pClipZMin, Fixed32 pClipZMax));
PROTOTYPE(void  , SPR_3TranClustWorld, (GlbCluster *cluster));

#endif /* _SPR3_ */

#endif /* ifndef SEGA_SPR_H */

/*  end of file */
