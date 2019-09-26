/*----------------------------------------------------------------------------
 *  spr_2c.c -- SPR ライブラリ 2C モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *  Updated by H.O on 1995-11-14 Ver.1.00a SlaveOFFコマンド対策
 *
 *  このライブラリはスプライト表示拡張（２Ｄ）処理モジュールで、以下のルーチン
 *  を含む。
 *
 *  SPR_2Initial           -  ２Ｄスプライト表示の初期化処理(初回専用)
 *  SPR_2Initial2          -  ２Ｄスプライト表示の再初期化処理(二度目以降)
 *  SPR_2SetTvMode         -  ＴＶモードのセット
 *  SPR_2FrameChgIntr      -  フレームチェンジV-BLANKインターバル数セット
 *  SPR_2FrameEraseData    -  フレームバッファイレーズデータのセット
 *  SPR_2SetGourTbl        -  グーローテーブルのセット
 *  SPR_2SetLookupTbl      -  ルックアップテーブルのセット
 *  SPR_2SetChar           -  キャラクタのセット
 *  SPR_2ClrChar           -  キャラクタエリアの解放
 *  SPR_2ClrAllChar        -  全キャラクタエリアの解放
 *  SPR_2GourTblNo         -  グーローテーブル番号をＶＲＡＭアドレスに変換
 *  SPR_2LookupTblNo       -  ルックアップテーブル番号をＶＲＡＭアドレスに変換
 *  SPR_2CharNo            -  キャラクタ番号をＶＲＡＭアドレスに変換
 *  SPR_2GetAreaSizeInfo   -  各ワークエリアのサイズ及び使用サイズを返す
 *  SPR_2OpenCommand       -  コマンド書き込みのオープン処理
 *  SPR_2CloseCommand      -  コマンド書き込みのクローズ処理
 *  SPR_2FlushDrawPrty     -  コマンド描画プライオリティチェインのフラッシュ
 *  SPR_2LocalCoord        -  ローカル座標のセット
 *  SPR_2SysClip           -  システムクリッピングエリアのセット
 *  SPR_2UsrClip           -  ユーザークリッピングエリアのセット
 *  SPR_2Line              -  ライン描画
 *  SPR_2PolyLine          -  ポリラインの描画
 *  SPR_2Polygon           -  ポリゴンの描画
 *  SPR_2NormSpr           -  ノーマルスプライトの描画
 *  SPR_2ScaleSpr          -  スケールスプライトの描画
 *  SPR_2DistSpr           -  変形スプライトの描画
 *  SPR_2Cmd               -  コマンドセット
 *
 *  このライブラリを使用するには次のインクルードファイルとワークエリアを定義す
 *  る必要がある。
 *
 *   #include "sega_scl.h"
 *   #define  _SPR2_
 *   #include "sega_spr.h"
 *
 *   #define CommandMax    100           // 最大コマンド数
 *   #define GourTblMax    100           // 最大グーローテーブル数
 *   #define LookupTblMax  100           // 最大ルックアップテーブル数
 *   #define CharMax       100           // 最大キャラクタ数
 *   #define DrawPrtyMax   256           // 最大プライオリティブロック数
 *   SPR_2DefineWork(work2D,
 *                 CommandMax, GourTblMax, LookupTblMax, CharMax, DrawPrtyMax)
 *                                       // ２Ｄワークエリア定義
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#ifdef __GNUC__
#include <machine.h>
#else
#include <machine2.h>
#endif

#define  _SPR2_
#include "sega_spr.h"
#ifdef _SH
#include <string.h>		/*	95-7-27	*/
#include "sega_dma.h"
#endif

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* #define  SP2_DEBUG */

#if	1
/*
**■1995-09-11	高橋智延
**	＜追加＞
*/
#define	FAILURE	0
#define	SUCCESS	1
#endif

#define  VRAM_GRAN_MAX         16384     /* VRAM Granularity Max Count       */
#define  SP_SYS_CMD_MAX        4         /* System Command Area Count        */

#define  GetSpCmdArea(CMD)                          \
            SPR_2SetDrawPrty(drawPrty);            \
            CMD = mstCmd++;  SpMstCmdPos++;

#define  SetCharPara(CMD)                                                \
            CMD->charAddr = SpCharTbl[charNo].addr;                      \
            CMD->charSize = SpCharTbl[charNo].xySize;                    \
            drawMode = (drawMode & ~DRAW_COLOR) | SpCharTbl[charNo].mode;

#define  SetDrawPara(CMD)                                              \
            CMD->drawMode = drawMode;                                  \
            if((drawMode & DRAW_COLOR) == COLOR_1)                     \
                CMD->color = vramInfo.lookupTblR + color*4;            \
            else                                                       \
                CMD->color = color;

#define  SetDrawParaTex(CMD)                                           \
            CMD->drawMode = drawMode;                                  \
            if(color == 0xffff) color = SpCharTbl[charNo].color;       \
            if((drawMode & DRAW_COLOR) == COLOR_1)                     \
                CMD->color = vramInfo.lookupTblR + color*4;            \
            else                                                       \
                CMD->color = color;

#define  SetGourPara(CMD)                                              \
            if(gourTblNo == NO_GOUR)                                   \
                CMD->grshAddr = 0;                                     \
            else                                                       \
                CMD->grshAddr = crGourTblR + gourTblNo;

/*
 * DATA TYPE DECLARATIONS
 */
struct VramInfo {                           /* VRAM Area Entry Info.         */
    Uint16     cmdTblCnt;                   /* Command Area Max Count        */
    Uint16     cmdTblSize;                  /* Command Area Size (32 byte)   */
    Uint16     gourTblCnt;                  /* Gouraud Shading Table Max Cnt */
    Uint16     gourTblSize;                 /* Gouraud Shading Table Area Siz*/
    Uint16     lookupTblCnt;                /* Color Lookup Table Max Count  */
    Uint16     lookupTblSize;               /* Color Lookup Table Area Size  */
    SprLookupTbl *lookupTbl;                /* Color Lookup Table Top Addr   */
    Vaddr        lookupTblR;                /*      "      Relative Address  */
    Uint8      *blockPool;                  /* Block Pool Area Top Addr      */
    Uint16     blockPoolSize;               /* Block Pool Area Size          */
};

struct VramTbl {                            /* VRAM Area Address Info.       */
    SprSpCmd     *cmdTbl;                   /* Command Table Top Address     */
    Vaddr        cmdTblR;                   /*     "       Relative Address  */
    SprGourTbl   *gourTbl;                  /* Gouraud Shading Table Top Addr*/
    Vaddr        gourTblR;                  /*      "      Relative Address  */
};

struct NullBlock {                          /* Block Null Entry              */
    struct NullBlock *next;                 /* Next Null Block Pointer       */
    Uint16            size;                 /* Block Size (by the 32 byte)   */
};

typedef struct NullBlock NullBlock;

/*
 * GLOBAL DECLARATIONS
 */
Sint32         Sp2OpenFlag;           /* Open Command Exec Flag        */
Sint32         SpLCoordX;             /* Local Origin X Position       */
Sint32         SpLCoordY;             /* Local Origin Y Position       */
extern  Sint32 SpScreenX;             /* TV Screen X Size              */
extern  Sint32 SpScreenY;             /* TV Screen Y Size              */
SprCharTbl     *SpCharTbl;            /* Character Table Pointer       */
Sint32         SpCharMax;             /* Character Table Entry Count   */
Sint32         SpDrawPrtyBlkMax;      /* Draw Priority Block Max Count */
Sint32         SpCmdNo;
Sint32         SpDrawPrtyProcFlag;    /* Draw Priority Process Flag         */
SprDrawPrtyBlk *SpDrawPrtyBlk;        /* 描画プライオリティブロックテーブル */
SprSpCmd       *SpCmdTbl;
Vaddr          SpCmdTblR;
SprDrawPrtyBlk *SpMstDrawPrtyBlk;
SprSpCmd       *SpMstCmd;        
int            SpMstCmdPos;        
SprDrawPrtyBlk *SpSlvDrawPrtyBlk;
SprSpCmd       *SpSlvCmd;        
int            SpSlvCmdPos;        
Vaddr          SpLookupTblR;
Sint32         SpGourTblMax;
SprGourTbl     *SpGourTbl;


/*
 * STATIC DECLARATIONS
 */
static SprSpCmd        *mstCmd;        
static SprSpCmd        *slvCmd;        

static Uint8           *VRAM;
static Sint32          vramSW;
static SprSpCmd        *topSpCmd;
static struct VramInfo vramInfo;
static struct VramTbl  vramTbl[2];
static NullBlock       *nullBlockTop;
static Uint16          eraseData;
static Uint16          displayInterval;
static Vaddr           crGourTblR;
static SprGourTbl      *crGourTbl;


static Vaddr allocBlock(Uint16 size);
static void  freeBlock(Vaddr addr, Uint16 size);
Vaddr flushDrawPrtyBlock(int entryCnt, SprDrawPrtyBlk *mstZBlk,
           SprDrawPrtyBlk *slvZBlk, SprSpCmd **rtnChainLastCmd);
void  SPR_2SetDrawPrty(int drawPrtyNo);
void  SPR_2FlushDrawPrtyBlock(void);


/*****************************************************************************
 *
 * NAME:  SPR_2Initial()   - 2D Sprite表示環境の初期化(SlaveCPU起動付き)
 *        SPR_2Initial2()  - 2D Sprite表示環境の再初期化(SlaveCPU起動無し)
 *
 * PARAMETERS :
 *
 *        (1) Spr2WorkArea *workArea - <i> ２Ｄスプライトワークエリア定義
 *
 * DESCRIPTION:
 *
 *     ２Ｄスプライト表示環境の初期化処理
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *       SlaveCPUが外部アクセス中にSSHOFFコマンドを発効しないように
 *     新たにSPR_2Initial2を追加しました。
 *       アプリケーション起動時、クロックチェンジ、システムリセットの
 *     あとのSlaveCPU停止時には、従来通りSPR_Initialを、そうでない
 *     場合には、新しいSPR_2Initial2を使用してください。('95-11/14 H.O)
 *****************************************************************************
 */
void	SPR_2Initial(Spr2WorkArea  *workArea){
    SPR_InitSlaveSH();
	SPR_2Initial2(workArea);
}

void
SPR_2Initial2(Spr2WorkArea  *workArea)
{
    Uint16    w;
    Uint8     *wp;
    Uint16    gourTblCnt, cmdTblCnt;

    /** BEGIN ***************************************************************/
/*
 *	SPR_2Initial2新設に伴い削除。('95-11/14 H.O)
 *	SPR_InitSlaveSH();
 */
    SpCharMax = workArea->charMax;
    SpCharTbl = workArea->charTbl;

    gourTblCnt   = (workArea->gourTblMax + 3) & 0xfffc;

    SPR_Initial(&VRAM);

    cmdTblCnt = workArea->commandMax + SP_SYS_CMD_MAX;
    vramInfo.cmdTblCnt     = cmdTblCnt;
    vramInfo.cmdTblSize    = cmdTblCnt;
    vramInfo.gourTblCnt    = gourTblCnt;
    vramInfo.gourTblSize   = gourTblCnt / 4;
    vramInfo.lookupTblCnt  = workArea->lookupTblMax;
    vramInfo.lookupTblSize = workArea->lookupTblMax;
    w = (vramInfo.cmdTblSize + vramInfo.gourTblSize) * 2 + 
         vramInfo.lookupTblSize + 1;
    vramInfo.blockPoolSize = VRAM_GRAN_MAX - w;
    
    vramTbl[0].cmdTbl    = (SprSpCmd*)VRAM + 1;
    vramTbl[0].gourTbl   = (SprGourTbl*)  (vramTbl[0].cmdTbl    +
                                        vramInfo.cmdTblSize);
    vramTbl[1].cmdTbl    = (SprSpCmd*)    (vramTbl[0].gourTbl +
                                        vramInfo.gourTblSize * 4);
    vramTbl[1].gourTbl   = (SprGourTbl*)  (vramTbl[1].cmdTbl    +
                                        vramInfo.cmdTblSize);
    vramInfo.lookupTbl   = (SprLookupTbl*)(vramTbl[1].gourTbl   +
                                        vramInfo.gourTblSize * 4);
    wp                   = (Uint8*)    (vramInfo.lookupTbl +
                                        vramInfo.lookupTblSize);
    vramInfo.blockPool   = wp;

    vramTbl[0].cmdTblR   = 4;
    vramTbl[0].gourTblR  = vramTbl[0].cmdTblR    + vramInfo.cmdTblSize*4;
    vramTbl[1].cmdTblR   = vramTbl[0].gourTblR   + vramInfo.gourTblSize*4;
    vramTbl[1].gourTblR  = vramTbl[1].cmdTblR    + vramInfo.cmdTblSize*4;
    vramInfo.lookupTblR  = vramTbl[1].gourTblR   + vramInfo.gourTblSize*4;

    SPR_2ClrAllChar();

    topSpCmd = (SprSpCmd*)VRAM;
    topSpCmd->control = CTRL_END;

    SpMstCmd           = workArea->mstCmd;        
    SpSlvCmd           = workArea->slvCmd;        
    SpMstDrawPrtyBlk   = workArea->mstDrawPrtyBlk;
    SpSlvDrawPrtyBlk   = workArea->slvDrawPrtyBlk;
    SpDrawPrtyBlkMax   = workArea->drawPrtyBlkMax;
    SpGourTblMax       = workArea->gourTblMax;
    SpGourTbl          = workArea->gourTbl;
    SpDrawPrtyProcFlag = 0;
    memset(SpMstDrawPrtyBlk,0xff,sizeof(SprDrawPrtyBlk)*SpDrawPrtyBlkMax);
    if(SpSlvDrawPrtyBlk)
        memset(SpSlvDrawPrtyBlk,0xff,sizeof(SprDrawPrtyBlk)*SpDrawPrtyBlkMax);
    SpLookupTblR  = vramInfo.lookupTblR;

    vramSW = 0;
    SpCmdNo  = SP_SYS_CMD_MAX;
    SPR_2FrameEraseData(RGB16_COLOR(0,0,0));
    SpLCoordX = SpLCoordY = 0;

#if 0
xprintf("\n*** VramInfo ***\n");
xprintf("  cmdTblCnt     = %d\n",   vramInfo.cmdTblCnt);
xprintf("  cmdTblSize    = %d\n",   vramInfo.cmdTblSize);
xprintf("  gourTblCnt    = %d\n",   vramInfo.gourTblCnt);
xprintf("  gourTblSize   = %d\n",   vramInfo.gourTblSize);
xprintf("  lookupTblCnt  = %d\n",   vramInfo.lookupTblCnt);
xprintf("  lookupTblSize = %d\n",   vramInfo.lookupTblSize);
xprintf("  blockPool     = %lxH\n", vramInfo.blockPool);
xprintf("  blockPoolSize = %d\n",   vramInfo.blockPoolSize);
xprintf("\n*** VramTbl - 0 ***\n");
xprintf("  cmdTbl     = %lxH\n", vramTbl[0].cmdTbl);
xprintf("  cmdTblR    = %xH\n",  vramTbl[0].cmdTblR);
xprintf("  gourTbl    = %lxH\n", vramTbl[0].gourTbl);
xprintf("  gourTblR   = %xH\n",  vramTbl[0].gourTblR);
xprintf("  lookupTbl  = %lxH\n", vramTbl[0].lookupTbl);
xprintf("  lookupTblR = %xH\n",  vramTbl[0].lookupTblR);
xprintf("\n*** VramTbl - 1 ***\n");
xprintf("  cmdTbl     = %lxH\n", vramTbl[1].cmdTbl);
xprintf("  cmdTblR    = %xH\n",  vramTbl[1].cmdTblR);
xprintf("  gourTbl    = %lxH\n", vramTbl[1].gourTbl);
xprintf("  gourTblR   = %xH\n",  vramTbl[1].gourTblR);
xprintf("  lookupTbl  = %lxH\n", vramTbl[1].lookupTbl);
xprintf("  lookupTblR = %xH\n",  vramTbl[1].lookupTblR);
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetTvMode()  - Set TV Mode
 *
 * PARAMETERS :
 *
 *     (1) Uint16  mode            - <i>  ＴＶモード定義値
 *     (2) Uint16  screenSize      - <i>  画面の解像度
 *     (3) Uint16  doubleInterlace - <i>  倍密インタレースフラグ
 *
 * DESCRIPTION:
 *
 *     ＴＶモードを設定する
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
SPR_2SetTvMode(Uint16 mode, Uint16 screenSize, Uint16 doubleInterlace)
{

    /** BEGIN ***************************************************************/
    SPR_SetTvMode(mode, screenSize, doubleInterlace);
}


/*****************************************************************************
 *
 * NAME:  SPR_2FrameChgIntr()  - Set Frame Change Interval Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16  interval   - <i>  Ｖ－ＢＬＡＮＫインターバル数
 *                                0      = スプライトのフレーム切り替えをオート
 *                                         にして、インターバルを１にする
 *                                         SCL_DisplayFrame()による同期なし
 *                                1      = スプライトのフレーム切り替えをオート
 *                                         にして、インターバルを１にする
 *                                         SCL_DisplayFrame()による同期あり
 *                                0xffff = インターバルを無効にしてフレーム切り
 *                                         替え要求時に即切り替える
 *                                0xfffe = インターバルを無効にしてフレーム切り
 *                                         替え要求時にＶブランクイレースを行
 *                                         って即切り替える
 *                                その他 = 指定インターバルでフレーム切り替え
 *                                         を行う
 *                                         b15 = 0 : イレースライトを行う
 *                                             = 1 : イレースライトを行わない
*
 * DESCRIPTION:
 *
 *     フレームチェンジＶ－ＢＬＡＮＫインターバル数のセット
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
SPR_2FrameChgIntr(Uint16 interval)
{
    extern void SCL_SetFrameInterval(Uint16 count);	/*	95-7-27	*/

    /** BEGIN ***************************************************************/
    displayInterval = interval;
    SCL_SetFrameInterval(interval);
}


/*****************************************************************************
 *
 * NAME:  SPR_2FrameEraseData()  - Set Frame Buffer Erase Data
 *
 * PARAMETERS :
 *
 *     (1) Uint16  rgbColor      - <i> RGB イレーズデータ
 *                                     0 = 透明コード
 *
 * DESCRIPTION:
 *
 *     フレームバッファイレーズデータのセット
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
SPR_2FrameEraseData(Uint16 rgbColor)
{

    /** BEGIN ***************************************************************/
    SPR_SetEraseData(rgbColor, 0, 0, SpScreenX-1, SpScreenY-1);
    eraseData = rgbColor;
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetGourTbl()  - Set Gouraud Shading Table in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16     gourTblNo  - <i> グーローテーブル番号
 *     (2) SprGourTbl *gourTbl   - <i> グーローテーブルポインタ
 *
 * DESCRIPTION:
 *
 *     グーローテーブルをＶＲＡＭにセット
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
SPR_2SetGourTbl(Uint16 gourTblNo, SprGourTbl *gourTbl)
{
#if	0
	/*
	**■1995-08-28	高橋智延
	**	メモリのブロックコピーには memcpy() を使いましょう。
	*/
    Uint16      *ws, *wd;

    /** BEGIN ***************************************************************/
    wd = (Uint16*)(SpGourTbl + gourTblNo);
    ws = (Uint16*)gourTbl;
    *wd++ = *ws++;
    *wd++ = *ws++;
    *wd++ = *ws++;
    *wd   = *ws  ;
#else
	memcpy( &SpGourTbl[gourTblNo], gourTbl, sizeof( SprGourTbl ));
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetLookupTbl()  - Set Color Lookup Table in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16        lookupTblNo   - <i>  ルックアップテーブル番号
 *     (2) SprLookupTbl *lookupTbl     - <i>  ルックアップテーブルポインタ
 *
 * DESCRIPTION:
 *
 *     カラールックアップテーブルをＶＲＡＭにセット
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
SPR_2SetLookupTbl(Uint16 lookupTblNo, SprLookupTbl *lookupTbl)
{
    SprLookupTbl  *dLookupTbl;

    /** BEGIN ***************************************************************/
    dLookupTbl = vramInfo.lookupTbl + lookupTblNo;
    DMA_ScuMemCopy(dLookupTbl, lookupTbl, sizeof(SprLookupTbl));
    DMA_ScuResult();
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetChar()  - Set Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo       - <i>  キャラクタ番号
 *     (2) Uint16  colorMode    - <i>  カラーモード（０－５）
 *     (3) Uint16  color        - <i>  カラーデータ（ｶﾗｰﾊﾞﾝｸorﾙｯｸｱｯﾌﾟﾃｰﾌﾞﾙNo）
 *     (4) Uint16  width        - <i>  Ｘサイズ
 *     (5) Uint16  height       - <i>  Ｙサイズ
 *     (6) Uint8  *charImage    - <i>  キャラクタデータ
 *
 * DESCRIPTION:
 *
 *     キャラクタデータをＶＲＡＭにセット
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
#if	0
/*
**■1995-09-11	高橋智延
**	＜仕様変更＞
**	領域の確保に失敗したら、あるいは、確保されている領域のサイズよりもセッ
**	トしようとするデータのサイズが大きい場合には、エラーコードを返すように
**	する。
*/
void
SPR_2SetChar(Uint16 charNo, Uint16 colorMode, Uint16 color,
                       Uint16 width, Uint16 height, Uint8 *charImage)
{
    Uint32   w, charSize;
    Uint8    *charPtr;

    /** BEGIN ***************************************************************/
    w = (Uint32)width * (Uint32)height;
    if(colorMode >= COLOR_5)
       w <<= 1;
    else
    if(colorMode <= COLOR_1)
       w >>= 1;
    w = (w + 31) >> 5;
    charSize = w << 4;
    if(SpCharTbl[charNo].addr == 0) {
        SpCharTbl[charNo].addr = allocBlock(w);
        SpCharTbl[charNo].size = w;
    }
    if(w <= SpCharTbl[charNo].size) {
        charPtr = VRAM + (((Uint32)SpCharTbl[charNo].addr)<<3);
        if(charImage) {
            DMA_ScuMemCopy(charPtr, charImage, charSize*2);
            DMA_ScuResult();
        }
        SpCharTbl[charNo].mode   = colorMode;
        SpCharTbl[charNo].color  = color;
	SpCharTbl[charNo].xySize = (width << 5) + height;
    } else {
    }
}
#else
int	SPR_2SetChar( Uint16	charNo, Uint16	colorMode, Uint16	color,
				Uint16	width, Uint16	height, Uint8	*charImage ){
	SprCharTbl	*current = &SpCharTbl[charNo];
	Uint32	charSize = ( Uint32 )width * ( Uint32 )height;
	Uint32	blockSize;
	int		ret = SUCCESS;
	
	if( colorMode >= COLOR_5 )
		blockSize = charSize * 2;
	else if( colorMode <= COLOR_1 )
		blockSize = charSize / 2;
	else
		blockSize = charSize;
	blockSize = ( blockSize + 32-1 ) / 32;
	
	if( current->addr == 0 ){
		Vaddr	p;
		
		if(( p = allocBlock( ( Uint16 )blockSize )) != ( Vaddr )NULL ){
			current->addr = p;
			current->size = blockSize;
		}
		else{
			ret = FAILURE;
			goto	end;	/*	注意！	*/
		}
	}
	
	if( blockSize <= current->size ){
		Uint32	offset = ( Uint32 )current->addr << 3;
		Uint8	*charPtr = ( Uint8 * )( VRAM + offset );
		
		if( charImage ){
/* 1995-11-09 N.K */
#if 0
			DMA_ScuMemCopy( charPtr, charImage, charSize );
#else
			DMA_ScuMemCopy( charPtr, charImage, charSize * 2);
#endif
			DMA_ScuResult();
		}
		current->mode   = colorMode;
		current->color  = color;
		current->xySize = ( width * 32 ) + height;
	}
	else{
		ret = FAILURE;
	}
	
end:;
	return	ret;
}
#endif



/*****************************************************************************
 *
 * NAME:  SPR_2ClrChar()  - Clear Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo       - <i>  キャラクタ番号
 *
 * DESCRIPTION:
 *
 *     ＶＲＡＭからキャラクタデータエリアを解放
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
SPR_2ClrChar(Uint16 charNo)
{

    /** BEGIN ***************************************************************/
    if(SpCharTbl[charNo].addr) {
       freeBlock(SpCharTbl[charNo].addr, SpCharTbl[charNo].size);
       SpCharTbl[charNo].addr = 0;
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_2ClrAllChar()  - Clear All Character Data in the VRAM
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ＶＲＡＭから全キャラクタデータエリアを解放
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
SPR_2ClrAllChar(void)
{
    Sint32    i;

    /** BEGIN ***************************************************************/
    for(i=0; i<SpCharMax; i++)
        SpCharTbl[i].addr = 0;

    if(vramInfo.blockPoolSize) {
        nullBlockTop       = (struct NullBlock*)vramInfo.blockPool;
        nullBlockTop->next = 0;
        nullBlockTop->size = vramInfo.blockPoolSize;
    } else
        nullBlockTop = 0;
}


/*****************************************************************************
 *
 * NAME: SPR_2GourTblNoToVram() - Convert Gouraud Shading Table No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  gourTblNo    - <i>  グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     グーローテーブル番号をＶＲＡＭアドレスに変換
 *
 * POSTCONDITIONS:
 *
 *     ＶＲＡＭ内相対アドレス／８
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2GourTblNoToVram(Uint16 gourTblNo)
{

    /** BEGIN ***************************************************************/
    return vramTbl[vramSW].gourTblR + gourTblNo;
}


/*****************************************************************************
 *
 * NAME:  SPR_2LookupTblNoToVram()  - Convert Color Lookup Table No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  lookupTblNo  - <i>  ルックアップテーブル番号
 *
 * DESCRIPTION:
 *
 *     ルックアップテーブル番号をＶＲＡＭアドレスに変換
 *
 * POSTCONDITIONS:
 *
 *     ＶＲＡＭ内相対アドレス／８
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2LookupTblNoToVram(Uint16 lookupTblNo)
{

    /** BEGIN ***************************************************************/
    return  vramInfo.lookupTblR + lookupTblNo*4;
}


/*****************************************************************************
 *
 * NAME:  SPR_2CharNoTblNoToVram()  - Convert Character No to VRAM Addr
 *
 * PARAMETERS :
 *
 *     (1) Uint16  charNo    - <i>  キャラクタ番号
 *
 * DESCRIPTION:
 *
 *     キャラクタ番号をＶＲＡＭアドレスに変換
 *
 * POSTCONDITIONS:
 *
 *     ＶＲＡＭ内相対アドレス／８
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Uint16
SPR_2CharNoToVram(Uint16 charNo)
{

    /** BEGIN ***************************************************************/
    return  SpCharTbl[charNo].addr; 
}


/*****************************************************************************
 *
 * NAME:  SPR_2GetAreaSizeInfo()  - Get Work Area Size Info.
 *
 * PARAMETERS :
 *
 *     (1) SprAreaSize  *areaSizeInfo   - <o>  エリアサイズ情報リターンエリア
 *
 * DESCRIPTION:
 *
 *     各ワークエリアのサイズ及び使用サイズを返す
 *
 * POSTCONDITIONS:
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void
SPR_2GetAreaSizeInfo(SprAreaSize  *areaSizeInfo)
{
    NullBlock  *nullBlock;
    Uint16     nullBlkMaxSize, totalNullBlkCount;

    /** BEGIN ***************************************************************/
    nullBlock         = nullBlockTop;
    nullBlkMaxSize    = 0;
    totalNullBlkCount = 0;
    while(nullBlock) {
        if(nullBlkMaxSize < nullBlock->size)
            nullBlkMaxSize = nullBlock->size;
        totalNullBlkCount += nullBlock->size;
        nullBlock = nullBlock->next;
    }
    areaSizeInfo->charRemainBlkCount = totalNullBlkCount;
    areaSizeInfo->charNullBlkMaxSize = nullBlkMaxSize;
    areaSizeInfo->commandMax         = vramInfo.cmdTblCnt;
    areaSizeInfo->gourTblMax         = vramInfo.gourTblCnt;
    areaSizeInfo->lookupTblMax       = vramInfo.lookupTblCnt;
    areaSizeInfo->charMax            = SpCharMax;
    areaSizeInfo->drawPrtyBlkMax     = SpDrawPrtyBlkMax;
    areaSizeInfo->useCommandCount    = SpCmdNo;
}


/*****************************************************************************
 *
 * NAME:  SPR_2OpenCommand()  - Open Sprite Command Write
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     コマンド書き込みのオープン処理
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
SPR_2OpenCommand(Uint16 drawPrtyFlag)
{
    SprSpCmd  *spCmd;
    /** BEGIN ***************************************************************/
    Sp2OpenFlag         = 1;
    vramSW             ^= 1;
    SpCmdNo             = SP_SYS_CMD_MAX;
    SpDrawPrtyProcFlag  = drawPrtyFlag;
    SpCmdTbl            = vramTbl[vramSW].cmdTbl;
    SpCmdTblR           = vramTbl[vramSW].cmdTblR;
    crGourTblR          = vramTbl[vramSW].gourTblR;
    crGourTbl           = vramTbl[vramSW].gourTbl;
    spCmd               = SpCmdTbl + SpCmdNo;
    spCmd->control      = CTRL_SKIP;
    SpCmdNo++;
    mstCmd              = SpMstCmd;        
    slvCmd              = SpSlvCmd;
    SpMstCmdPos         = 0;        
    SpSlvCmdPos         = 0;        
}


/*****************************************************************************
 *
 * NAME:  SPR_2CloseCommand()  - Close Sprite Command Write
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     コマンド書き込みのクローズ処理
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
SPR_2CloseCommand(void)
{
    SprSpCmd    *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Uint32      xy[4];
    Uint16      *w;
#endif
    /** BEGIN ***************************************************************/
    SPR_2FlushDrawPrty();

    spCmd = SpCmdTbl + SpCmdNo;
    spCmd->control = CTRL_END;
    topSpCmd->control = CTRL_SKIP | JUMP_ASSIGN;
    if(displayInterval == 0xffff) {
        SpCmdNo = 0;
#if	0
		/*
		**■1995-08-28	高橋智延
		**	構造体にはメンバでアクセスしましょう。
		*/
	w = (Uint16*)xy;
        *w++ = 0;
        *w++ = 0;
        *w++ = SpScreenX-1;
        *w++ = 0;
        *w++ = SpScreenX-1;
        *w++ = SpScreenY-1;
        *w++ = 0;
        *w   = SpScreenY-1;
#endif
        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control = JUMP_NEXT | FUNC_LCOORD;
        spCmd->ax      = 0;
        spCmd->ay      = 0;
        
        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control  = JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON;
        spCmd->drawMode =  ECDSPD_DISABLE | COLOR_5 | COMPO_REP;
        spCmd->color    = eraseData;
#if	0
		/*
		**■1995-08-28	高橋智延
		**	構造体にはメンバでアクセスしましょう。
		*/
        sprMemCpyL(&spCmd->ax, xy, 4);
#else
		spCmd->ax = 0;
		spCmd->ax = 0;
		spCmd->bx = SpScreenX-1;
		spCmd->bx = 0;
		spCmd->cx = SpScreenX-1;
		spCmd->cx = SpScreenY-1;
		spCmd->dx = 0;
		spCmd->dx = SpScreenY-1;
#endif
        spCmd->grshAddr = 0;

        spCmd = SpCmdTbl + SpCmdNo++;
        spCmd->control = JUMP_NEXT | FUNC_LCOORD;
        spCmd->ax      = SpLCoordX;
        spCmd->ay      = SpLCoordY;

        spCmd          = SpCmdTbl + SpCmdNo;
        spCmd->control = CTRL_SKIP | JUMP_ASSIGN;
        spCmd->link    = SpCmdTblR + SP_SYS_CMD_MAX*4;
        topSpCmd->link = SpCmdTblR;
    } else
        topSpCmd->link = SpCmdTblR + SP_SYS_CMD_MAX*4;

    if(SpGourTblMax) {
        DMA_ScuMemCopy(crGourTbl, SpGourTbl, sizeof(SprGourTbl)*SpGourTblMax);
        DMA_ScuResult();
    }
}


/*****************************************************************************
 *
 * NAME:  SPR_2FlushDrawPrty()  - Flush Draw Priority Command Chain
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     コマンド描画プライオリティチェインのフラッシュ
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
SPR_2FlushDrawPrty(void)
{
/*    SprSpCmd       *spCmd;	95-7-27	unuse	*/

    /** BEGIN ***************************************************************/
    SPR_2FlushDrawPrtyBlock();
}


/*****************************************************************************
 *
 * NAME:  SPR_2LocalCoord()  - Set Local Coord
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) XyInt   xy         - <i> ローカル座標の相対座標　
 *
 * DESCRIPTION:
 *
 *     ローカル座標のセット
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
SPR_2LocalCoord(Sint32 drawPrty, XyInt *xy)
{
    SprSpCmd  *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Uint16    *w;
#endif

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN | FUNC_LCOORD;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	構造体にはメンバでアクセスしましょう。
	*/
    w = (Uint16*)xy;
    SpLCoordX = spCmd->ax = *w++;
    SpLCoordY = spCmd->ay = *w;
#else
	spCmd->ax = xy->x;
	spCmd->ay = xy->y;
	
	SpLCoordX = xy->x;
	SpLCoordY = xy->y;
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2SysClip()  - Set System Cliping Area
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) XyInt   xy         - <i> 右下座標
 *
 * DESCRIPTION:
 *
 *     システムクリッピングエリアのセット
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
SPR_2SysClip(Sint32 drawPrty, XyInt *xy)
{
    SprSpCmd  *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Uint16    *w;
#endif

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN  | FUNC_SCLIP;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	構造体にはメンバでアクセスしましょう。
	*/
    w = (Uint16*)xy;
    spCmd->cx = *w++;
    spCmd->cy = *w;
#else
	spCmd->cx = xy->x;
	spCmd->cy = xy->y;
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2UserClip()  - Set User Cliping Area
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty    - <i>  コマンド描画プライオリティ番号
 *     (2) XyInt   xy[2]       - <i>  xy[0] = 左上座標
 *                                    xy[1] = 右下座標
 *
 * DESCRIPTION:
 *
 *     ユーザークリッピングエリアのセット
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
SPR_2UserClip(Sint32 drawPrty, XyInt xy[2])
{
    SprSpCmd  *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Uint16 *w;
#endif

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN  | FUNC_UCLIP;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	構造体にはメンバでアクセスしましょう。
	*/
    w = (Uint16*)xy;
    spCmd->ax = *w++;
    spCmd->ay = *w++;
    spCmd->cx = *w++;
    spCmd->cy = *w;
#else
    spCmd->ax = xy->x;
    spCmd->ay = xy->y;
    spCmd->cx = xy->x;
    spCmd->cy = xy->y;
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2Line()  - Draw Line
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  drawMode   - <i> 描画モード
 *     (3) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (4) XyInt   xy[2]      - <i> ラインの２点
 *     (5) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     ラインの描画
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
SPR_2Line(Sint32 drawPrty,
          Uint16 drawMode, Uint16 color, XyInt xy[2], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Sint16 *s, *d;
#endif

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN | FUNC_LINE;
    SetDrawPara(spCmd);
#if	0
	/*
	**■1995-08-28	高橋智延
	**	構造体にはメンバでアクセスしましょう。
	*/
    s = (Sint16*)xy;
    d = (Sint16*)&spCmd->ax;
    *d++ = *s++;
    *d++ = *s++;
    *d++ = *s++;
    *d   = *s  ;
#else
	spCmd->ax = ( Sint16 )xy[0].x;
	spCmd->ay = ( Sint16 )xy[0].y;
	spCmd->bx = ( Sint16 )xy[1].x;
	spCmd->by = ( Sint16 )xy[1].y;
#endif
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2PolyLine()  - Draw Polyline
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  drawMode   - <i> 描画モード
 *     (3) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (4) XyInt   xy[4]      - <i> ポリラインの４点
 *     (5) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     ポリラインの描画
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
SPR_2PolyLine(Sint32 drawPrty, 
              Uint16 drawMode, Uint16 color, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN | FUNC_POLYLINE;
    SetDrawPara(spCmd);
    sprMemCpyW(&spCmd->ax, xy, 8);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2Polygon()  - Draw Polygon
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  drawMode   - <i> 描画モード
 *     (3) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (4) XyInt   xy[4]      - <i> ポリゴンの４点
 *     (5) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     ポリゴンの描画
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
SPR_2Polygon(Sint32 drawPrty, 
             Uint16 drawMode, Uint16 color, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON;
    SetDrawPara(spCmd);
    sprMemCpyW(&spCmd->ax, xy, 8);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2NormSpr()  - Draw Normal Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  dir        - <i> キャラクタ反転指示
 *     (3) Uint16  drawMode   - <i> 描画モード
 *     (4) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (5) Uint16  charNo     - <i> キャラクタ番号
 *     (6) XyInt   xy　　     - <i> 左上座標
 *     (7) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     ノーマルスプライトの描画
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
SPR_2NormSpr(Sint32 drawPrty, Uint16 dir, Uint16 drawMode, Uint16 color, 
                  Uint16 charNo, XyInt *xy, Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control =  ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV | FUNC_NORMALSP)
                        & ~CTRL_DIR) | dir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
    spCmd->ax = xy->x;
    spCmd->ay = xy->y;
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2ScaleSpr()  - Draw Scale Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  zoomDir    - <i> ズームとキャラクタ反転指示
 *     (3) Uint16  drawMode   - <i> 描画モード
 *     (4) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (5) Uint16  charNo     - <i> キャラクタ番号
 *     (6) XyInt   xy[2]      - <i> ２頂点座標、または不動点座標・表示幅
 *     (7) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     スケールスプライトの描画
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
SPR_2ScaleSpr(Sint32 drawPrty, Uint16 zoomDir, Uint16 drawMode,
              Uint16 color, Uint16 charNo, XyInt xy[2], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;
#if	0
	/*
	**■1995-08-28	高橋智延
	**	削除（理由は下記を参照）
	*/
    Sint16 *w;
#endif

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control =  ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP)
                       & ~CTRL_ZOOM & ~CTRL_DIR ) | zoomDir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
#if	0
	/*
	**■1995-08-28	高橋智延
	**	構造体にはメンバでアクセスしましょう。
	*/
    w = (Sint16*)xy;
    if(zoomDir & CTRL_ZOOM) {
        spCmd->ax = *w++;
        spCmd->ay = *w++;
        spCmd->bx = *w++;
        spCmd->by = *w;
    } else {
        spCmd->ax = *w++;
        spCmd->ay = *w++;
        spCmd->cx = *w++;
        spCmd->cy = *w;
    }
#else
	if( zoomDir & CTRL_ZOOM ){
		spCmd->ax = xy[0].x;
		spCmd->ay = xy[0].y;
		spCmd->bx = xy[1].x;
		spCmd->by = xy[1].y;
	}
	else{
		spCmd->ax = xy[0].x;
		spCmd->ay = xy[0].y;
		spCmd->cx = xy[1].x;
		spCmd->cy = xy[1].y;
	}
#endif
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2DistSpr()  - Draw Distorted Sprite
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty   - <i>  コマンド描画プライオリティ番号
 *     (2) Uint16  dir        - <i> キャラクタ反転指示
 *     (3) Uint16  drawMode   - <i> 描画モード
 *     (4) Uint16  color      - <i> カラーコードまたはルックアップテーブル番号
 *     (5) Uint16  charNo     - <i> キャラクタ番号
 *     (6) XyInt   xy[4]      - <i> ４頂点座標
 *     (7) Uint16　gourTblNo  - <i> グーローテーブル番号
 *
 * DESCRIPTION:
 *
 *     変形スプライトの描画
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
SPR_2DistSpr(Sint32 drawPrty, Uint16 dir, Uint16 drawMode, Uint16 color,
                       Uint16 charNo, XyInt xy[4], Uint16 gourTblNo)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    spCmd->control = ((JUMP_ASSIGN | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP)
                      & ~CTRL_DIR) | dir;
    SetCharPara(spCmd);
    SetDrawParaTex(spCmd);
    sprMemCpyW(&spCmd->ax, xy, 8);
    SetGourPara(spCmd);
}


/*****************************************************************************
 *
 * NAME:  SPR_2Cmd()  - Set Sprite Command
 *
 * PARAMETERS :
 *
 *     (1) Sint32  drawPrty    - <i>  コマンド描画プライオリティ番号
 *     (2) SprSpCmd  *spCmd    - <i> ３２バイトのスプライトコマンド
 *
 * DESCRIPTION:
 *
 *     スプライトコマンドをＶＲＡＭにセット
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
SPR_2Cmd(Sint32 drawPrty, SprSpCmd *pSpCmd)
{
    SprSpCmd  *spCmd;

    /** BEGIN ***************************************************************/
    GetSpCmdArea(spCmd);
    sprMemCpyW(spCmd, pSpCmd, 16);
}


/*****************************************************************************
 *
 * NAME:  allocBlock()  - Allocate Block in The VRAM Area
 *
 * PARAMETERS :
 *
 *     (1) Uint16  size    - <i>  ブロックサイズ（３２バイトを１単位とする）
 *
 * DESCRIPTION:
 *
 *     ＶＲＡＭ内のブロックプールエリアから指定サイズのブロックを獲得する
 *
 * POSTCONDITIONS:
 *
 *     ＶＲＡＭ内の獲得ブロックエリア位置を相対アドレス／８で返す
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
#if	0
/*
**■1995-09-11	高橋智延
**	＜修正＞
**	まだ空きエリアがあるにもかかわらず、NULL を返してしまうのを修正。
*/
static Vaddr
allocBlock(Uint16 size)
{
    NullBlock  *nullBlock, *minNullBlock;
    NullBlock  *befNullBlock, *befMinNullBlock;
    Uint16     minSize;
    Uint32     w;
    Vaddr      addr;

    /** BEGIN ***************************************************************/
    nullBlock = nullBlockTop;
    addr = 0;
    minSize = 0xffff;
    minNullBlock = 0;
    befNullBlock = 0;
    befMinNullBlock = 0;	/* 95-7-27	*/
    while(nullBlock) {
        if(size <= nullBlock->size)
            if(minSize > nullBlock->size) {
                minSize      = nullBlock->size;
                minNullBlock = nullBlock;
                befMinNullBlock = befNullBlock;
            }
        befNullBlock = nullBlock;
        nullBlock = nullBlock->next;
    }

    if(minNullBlock) {
        minNullBlock->size -= size;
        w  = (Uint32)minNullBlock + (Uint32)minNullBlock->size * 32;
        w -= (Uint32)VRAM;
        addr = w >> 3;
        if(minNullBlock->size == 0)
            if(befMinNullBlock)  befMinNullBlock->next = minNullBlock->next;
    }
    return addr;
}
#else
static Vaddr	allocBlock( Uint16	size ){
	NullBlock	*memblk = nullBlockTop;
	NullBlock	*prev = NULL;
	NullBlock	*second_best = NULL;
	NullBlock	*prev_second_best = NULL;
	Vaddr		p = ( Vaddr )NULL;
	
	while( memblk != NULL ){
		if( memblk->size == size )
			break;
		
		if( memblk->size > size ){
			if(( second_best == NULL )
			||(( second_best != NULL )&&( memblk->size < second_best->size ))){
				second_best = memblk;
				prev_second_best = prev;
			}
		}
		
		prev = memblk;
		memblk = memblk->next;
	}
	
	if( memblk == NULL ){
		memblk = second_best;
		prev   = prev_second_best;
	}
	
	if( memblk != NULL ){
		Uint32	blocksize;
		
		memblk->size -= size;
		blocksize = ( Uint32 )memblk->size*32;
		p = ( Vaddr )(( Uint32 )memblk + blocksize - ( Uint32 )VRAM );
		
		if(( memblk->size == 0 )&&( prev != NULL ))
			prev->next = memblk->next;
	}
	
	return	p;
}
#endif

/*****************************************************************************
 *
 * NAME:  freeBlock()  - Free Block in The VRAM Area
 *
 * PARAMETERS :
 *
 *     (1) Vaddr  addr    - <i>  ブロックアドレス
 *     (2) Uint16 size    - <i>  ブロックサイズ（３２バイトを１単位とする）
 *
 * DESCRIPTION:
 *
 *     ＶＲＡＭブロックプール内の指定ブロックエリアを解放する
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
freeBlock(Vaddr addr, Uint16 size)
{
/*    NullBlock  *nullBlock;	95-7-27	unuse	*/
    NullBlock  *blockTop, *blockNext;
    NullBlock  *vramTop, *vramBot;
    NullBlock  *pt1, *pt2, *pt1Next;
/*    Uint32     w;		95-7-27	unuse	*/

    /** BEGIN ***************************************************************/
    blockTop  = (NullBlock*)((Uint32)VRAM     + ((Uint32)addr)*8);
    blockNext = (NullBlock*)((Uint32)blockTop + ((Uint32)size)*32);
    
    vramTop = (NullBlock*)VRAM;
    vramBot = (NullBlock*)((Uint32)VRAM       + (Uint32)VRAM_GRAN_MAX*32);
    pt1 = vramTop;
    pt2 = nullBlockTop;
    while(1) {
        if((pt1 <= blockTop) && (blockTop < pt2)) {
            if(pt1 == vramTop) {
                if(pt2 == blockNext) {
                    blockTop->next = pt2->next;
                    blockTop->size = pt2->size + size;
                    nullBlockTop   = blockTop;
                } else {
                    blockTop->next = nullBlockTop;
                    blockTop->size = size;
                    nullBlockTop   = blockTop;
                }
            } else
            if(pt2 == vramBot) {
                pt1Next = (NullBlock*)((Uint32)pt1 + ((Uint32)(pt1->size))*32);
                if(pt1Next == blockTop) {
                    pt1->size += size;
                } else {
                    pt1->next      = blockTop;
                    blockTop->next = 0;
                    blockTop->size = size;
                } 
            } else {
                pt1Next = (NullBlock*)((Uint32)pt1 + ((Uint32)(pt1->size))*32);
                if((pt1Next == blockTop) && (blockNext <  pt2)) {
                    pt1->size += size;
                } else
                if((pt1Next <  blockTop) && (blockNext == pt2)) {
                    blockTop->next = pt2->next;
                    blockTop->size = pt2->size + size;
                    pt1->next      = blockTop;
                } else
                if((pt1Next <  blockTop) && (blockNext <  pt2)) {
                    blockTop->next = pt2;
                    blockTop->size = size;
                    pt1->next      = blockTop;
                } else {
                    pt1->next  = pt2->next;
                    pt1->size += size;
                    pt1->size += pt2->size;
                }
            }
            break;
        }
        if(pt2 == vramBot) break;
        pt1 = pt2;
        pt2 = pt2->next;
        if(pt2 == 0)  pt2 = vramBot;
    }

#if 0
    nullBlock = nullBlockTop;
    xprintf("\n");
    while(nullBlock) {
       xprintf("(%lxH,%d)->",nullBlock,nullBlock->size);
       nullBlock = nullBlock->next;
    }
#endif
}


/*****************************************************************************
 *
 * NAME:  SPR_2FlushDrawPrtyBlock()  - Flush Draw Priority Block
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     コマンド描画プライオリティブロックのフラッシュ
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */

static SprSpCmd *s_chainLastCmd;
static Vaddr s_chainTopCmdR;

void
SPR_2FlushDrawPrtyBlock(void)
{

    extern void SlaveFlushDrawPrtyBlock(void);
/*    extern void *SlaveCommand;	95-7-27	unuse	*/
    SprSpCmd    *spCmd, *chainLastCmd;
    Vaddr       chainTopCmdR;
    int         wCmdNo;

    /** BEGIN ***************************************************************/
    /* ＶＲＡＭスプライトコマンドのプライオリティによるリンク処理  */

    /* スレーブＣＰＵによる処理 */
    SPR_RunSlaveSH((PARA_RTN*)SlaveFlushDrawPrtyBlock, 0);
    
    /* マスタＣＰＵによる処理 */
    chainTopCmdR = flushDrawPrtyBlock(
                       SpDrawPrtyBlkMax - (SpDrawPrtyBlkMax >> 1),
                       SpMstDrawPrtyBlk + (SpDrawPrtyBlkMax >> 1),
                       SpSlvDrawPrtyBlk + (SpDrawPrtyBlkMax >> 1),
                       &chainLastCmd);

    SPR_WaitEndSlaveSH();

    if(s_chainLastCmd) {
        s_chainLastCmd->link = chainTopCmdR;
        chainTopCmdR = s_chainTopCmdR;
        if(chainLastCmd == 0)
            chainLastCmd = s_chainLastCmd;
    }

    if(chainLastCmd) {
	wCmdNo          = SpCmdNo;

	spCmd           = SpCmdTbl + SpCmdNo - 1;
	spCmd->control |= JUMP_ASSIGN;
	spCmd->link     = chainTopCmdR;

	SpCmdNo            = SpCmdNo + SpMstCmdPos + SpSlvCmdPos;
	chainLastCmd->link = SpCmdTblR + (SpCmdNo<<2);

	spCmd              = SpCmdTbl + SpCmdNo++;
	spCmd->control     = CTRL_SKIP | JUMP_NEXT;

        if(SpMstCmdPos) {
	    spCmd = SpCmdTbl + wCmdNo;
            DMA_ScuMemCopy(spCmd, SpMstCmd, sizeof(SprSpCmd)*SpMstCmdPos);
            DMA_ScuResult();
        }
        if(SpSlvCmdPos) {
	    spCmd = spCmd + SpMstCmdPos;
            DMA_ScuMemCopy(spCmd, SpSlvCmd, sizeof(SprSpCmd)*SpSlvCmdPos);
            DMA_ScuResult();
        }
    }
    mstCmd          = SpMstCmd;        
    slvCmd          = SpSlvCmd;
    SpMstCmdPos     = 0;        
    SpSlvCmdPos     = 0;        
}


/*****************************************************************************
 *
 * NAME:  slaveFlushDrawPrtyBlock()  - Flush Draw Priority Block for Slave
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     スレーブＳＨによるコマンド描画プライオリティブロックのフラッシュ
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
void SlaveFlushDrawPrtyBlock(void)
{
    /** BEGIN ***************************************************************/
    /* ＶＲＡＭスプライトコマンドのプライオリティによるリンク処理  */
    s_chainTopCmdR = flushDrawPrtyBlock(SpDrawPrtyBlkMax >> 1,
                       SpMstDrawPrtyBlk, SpSlvDrawPrtyBlk, &s_chainLastCmd);
}


/*  以下、高速化のためにアセンブラルーチンあり */
#if 0

/*****************************************************************************
 *
 * NAME:  flushDrawPrtyBlock()  - Flush Draw Priority Block
 *
 *     Vaddr chainTopCmdR = flushDrawPrtyBlock(entryCnt, mstZBlk,
 *                                             slvZBlk, rtnChainLastCmd);
 *
 * PARAMETERS :
 *
 *     (1) int            entryCnt      - <i>  コマンド描画プライオリティ
 *                                             テーブルのエントリ数
 *     (2) SprDrawPrtyBlk *mstZBlk      - <i>  マスタ側コマンド描画
 *                                             プライオリティテーブル
 *     (3) SprDrawPrtyBlk *slvZBlk      - <i>  スレーブ側コマンド描画
 *                                             プライオリティテーブル
 *     (4) SprSpCmd       **rtnChainLastCmd   - <i>  最終コマンドエリアの
 *                                                   ポインタリターンエリア
 *
 * DESCRIPTION:
 *
 *     コマンド描画プライオリティブロックのフラッシュ
 *
 * POSTCONDITIONS:
 *
 *         int            chainTopCmdR  - <o>  VRAM内次の先頭コマンドアドレス
 *
 * CAVEATS:
 *
 *****************************************************************************
 */
Vaddr flushDrawPrtyBlock(int entryCnt, SprDrawPrtyBlk *mstZBlk,
           SprDrawPrtyBlk *slvZBlk, SprSpCmd **rtnChainLastCmd)
{
    SprSpCmd       *spCmd, *chainLastCmd;
    Vaddr          chainTopCmdR, mstCmdR, slvCmdR;
    register int   mstTopCmdNo, mstBotCmdNo;
    register int   slvTopCmdNo, slvBotCmdNo;
    int            i, wCmdNo;

    /** BEGIN ***************************************************************/
    /* ＶＲＡＭスプライトコマンドのプライオリティによるリンク処理  */
    chainLastCmd = 0;
    chainTopCmdR = 0;
    mstCmdR = SpCmdTblR + (SpCmdNo<<2);
    slvCmdR = mstCmdR + (SpMstCmdPos<<2);

    if(SpSlvCmd == 0) {
        for(i = 0; i < entryCnt; i++) {
  	    mstTopCmdNo    = mstZBlk->topNo;
	    mstBotCmdNo    = mstZBlk->botNo;
	    mstZBlk->topNo = 0xffff;
	    mstZBlk++;
	    if(mstTopCmdNo != 0xffff) {
	        for(wCmdNo=mstTopCmdNo; wCmdNo != mstBotCmdNo; ) {
		    spCmd  = SpMstCmd + wCmdNo;
		    wCmdNo = spCmd->link;
		    spCmd->link = mstCmdR + (wCmdNo<<2);
	        }
		if(chainLastCmd)
		    chainLastCmd->link = mstCmdR + (mstTopCmdNo<<2);
		if(chainTopCmdR == 0)
		    chainTopCmdR = mstCmdR + (mstTopCmdNo<<2);
		chainLastCmd = SpMstCmd + mstBotCmdNo;
            }
        }
    } else {
        for(i = 0; i < entryCnt; i++) {
  	    mstTopCmdNo    = mstZBlk->topNo;
	    mstBotCmdNo    = mstZBlk->botNo;
	    mstZBlk->topNo = 0xffff;
	    mstZBlk++;
	    if(mstTopCmdNo != 0xffff) {
	        for(wCmdNo=mstTopCmdNo; wCmdNo != mstBotCmdNo; ) {
		    spCmd  = SpMstCmd + wCmdNo;
		    wCmdNo = spCmd->link;
		    spCmd->link = mstCmdR + (wCmdNo<<2);
	        }
	    }
	    slvTopCmdNo    = slvZBlk->topNo;
	    slvBotCmdNo    = slvZBlk->botNo;
 	    slvZBlk->topNo = 0xffff;
	    slvZBlk++;
	    if(slvTopCmdNo != 0xffff) {
	        for(wCmdNo=slvTopCmdNo; wCmdNo != slvBotCmdNo; ) {
		    spCmd  = SpSlvCmd + wCmdNo;
		    wCmdNo = spCmd->link;
		    spCmd->link = slvCmdR + (wCmdNo<<2);
	        }
	    }
	    if(mstTopCmdNo != 0xffff) {
	        if(slvTopCmdNo != 0xffff) {
		    if(chainLastCmd)
		        chainLastCmd->link = mstCmdR + (mstTopCmdNo<<2);
		    spCmd  = SpMstCmd + mstBotCmdNo;
		    spCmd->link = slvCmdR + (slvTopCmdNo<<2);
		    if(chainTopCmdR == 0)
		        chainTopCmdR = mstCmdR + (mstTopCmdNo<<2);
		    chainLastCmd = SpSlvCmd + slvBotCmdNo;
	        } else {
		    if(chainLastCmd)
		        chainLastCmd->link = mstCmdR + (mstTopCmdNo<<2);
		    if(chainTopCmdR == 0)
		        chainTopCmdR = mstCmdR + (mstTopCmdNo<<2);
		    chainLastCmd = SpMstCmd + mstBotCmdNo;
	        }
	    } else {
	        if(slvTopCmdNo != 0xffff) {
		    if(chainLastCmd)
		        chainLastCmd->link = slvCmdR + (slvTopCmdNo<<2);
		    if(chainTopCmdR == 0)
		        chainTopCmdR = slvCmdR + (slvTopCmdNo<<2);
		    chainLastCmd = SpSlvCmd + slvBotCmdNo;
	        }
	    }
        }
    }
    *rtnChainLastCmd = chainLastCmd;
    return chainTopCmdR;
}


/*****************************************************************************
 *
 * NAME:  SPR_2SetDrawPrty - Set Draw Priority Command Chain
 *
 * PARAMETERS :
 *
 *     (1) int   drawPrtyNo   - <i>  コマンド描画プライオリティ番号
 *
 * DESCRIPTION:
 *
 *     コマンド描画プライオリティ管理ブロックにコマンド番号を設定する
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
SPR_2SetDrawPrty(int drawPrtyNo)
{
    Sint32      j;
    SprSpCmd    *wSpCmd;

    /** BEGIN ***************************************************************/
    if(SpDrawPrtyBlkMax <= drawPrtyNo) drawPrtyNo = SpDrawPrtyBlkMax - 1;
    if(SpMstDrawPrtyBlk[drawPrtyNo].topNo == 0xffff)
        SpMstDrawPrtyBlk[drawPrtyNo].topNo = SpMstCmdPos;
    else {
        j = SpMstDrawPrtyBlk[drawPrtyNo].botNo;
        wSpCmd = SpMstCmd + j;
        wSpCmd->link = SpMstCmdPos;
    }
    SpMstDrawPrtyBlk[drawPrtyNo].botNo = SpMstCmdPos;

}

#endif

/*  end of file */
