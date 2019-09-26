/*----------------------------------------------------------------------------
 *  V_Blank.c -- サンプルゲームＶブランク割り込み処理内ルーチン
 *----------------------------------------------------------------------------
 */
#include "sgl.h"
#include "machine.h"
#include "sega_scl.h"
#include "sega_dbg.h"
#include "sega_per.h"
#include "sega_xpt.h"
#include "sega_int.h"
#include "sample.h"


extern SysPort	*PortData;

void   GameVblankIn(void);
void   GameVblankOut(void);

static Uint16	dummy_pad=0;


void SetVblank(void){
	/* V-Blank割り込みルーチンの登録 */
	INT_ChgMsk(INT_MSK_NULL,INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
	INT_SetScuFunc(INT_SCU_VBLK_IN,GameVblankIn);
	INT_SetScuFunc(INT_SCU_VBLK_OUT,GameVblankOut);
	INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT,INT_MSK_NULL);

	DBG_Initial(&dummy_pad,RGB16_COLOR(31,31,31),0);
	DBG_DisplayOn();
/*	DBG_SetCursol(9, 3 );
	DBG_Printf("SBL6 SAMPLE GAME XXX");
*/
	PortData = PER_OpenPort();
}


void   GameVblankIn(void){
	/* ＳＧＬモードの場合 */
	BlankIn();
}


void   GameVblankOut(void){
	/* ＳＧＬモードの場合 */
	BlankOut();

	/* パッドデータ取得 */
	PER_GetPort(PortData);
}
