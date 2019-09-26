/****************************************
*			include files				*
****************************************/
#include	<string.h>
#include	"sega_scl.h"
#define	_SPR2_
#include	"sega_spr.h"

/****************************************
*			define constants			*
****************************************/
#define   COLOR_BANK	   0x0000

/****************************************
*		declare private objects			*
****************************************/
/* コマンドテーブルデータ */
static SprSpCmd  spCmd16_320_224[] = {

   {/* [ 0] システムクリッピングコマンド	*/
	/* control  */ (JUMP_NEXT | FUNC_SCLIP),
	/* link	 */ 0,
	/* drawMode */ 0,
	/* color	*/ 0,
	/* charAddr */ 0,
	/* charSize */ 0,
	/* ax, ay   */ 0, 0,
	/* bx, by   */ 0, 0,
	/* cx, cy   */ 320-1, 224-1,
	/* dx, dy   */ 0, 0,
	/* grshAddr */ 0,
	/* dummy	*/ 0},

   {/* [ 1] 相対座標設定コマンド   */
	/* control  */ (JUMP_NEXT | FUNC_LCOORD),
	/* link	 */ 0,
	/* drawMode */ 0,
	/* color	*/ 0,
	/* charAddr */ 0,
	/* charSize */ 0,
	/* ax, ay   */ 0, 0,
	/* bx, by   */ 0, 0,
	/* cx, cy   */ 0, 0,
	/* dx, dy   */ 0, 0,
	/* grshAddr */ 0,
	/* dummy	*/ 0},

   {/* [ 2] スプライト描画コマンド   */
	/* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_NORMALSP),
	/* link	 */ 0,
	/* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_REP),
	/* color	*/ 0,
	/* charAddr */ 0x0080,  /* 1024 byte pos */
	/* charSize */ 0x0110,  /* x*y = 8*16	 */
	/* ax, ay   */ 320/2, 224/2,
	/* bx, by   */   0,   0,
	/* cx, cy   */   0,   0,
	/* dx, dy   */   0,   0,
	/* grshAddr */   0,
	/* dummy	*/   0},

   {/* END 描画終了コマンド	*/
	/* control  */ CTRL_END,
	/* link	 */ 0,
	/* drawMode */ 0,
	/* color	*/ 0,
	/* charAddr */ 0,
	/* charSize */ 0,
	/* ax, ay   */ 0,
	/* bx, by   */ 0,
	/* cx, cy   */ 0,
	/* dx, dy   */ 0,
	/* grshAddr */ 0,
	/* dummy	*/ 0}
};
static Uint8	*VRAM;

void	_spr2_transfercommand( void ){
	memcpy( VRAM, spCmd16_320_224, sizeof( spCmd16_320_224 ) );
}
void	_spr2_transferimage( void	*image, Uint32	size ){
	memcpy( &VRAM[1024], image, size );
}
void	_spr2_initialize( void ){
	SPR_Initial(&VRAM);
	/* このイニシャル関数ではデフォルトのセットをする。			*/
	/* 例えば ＴＶモードのセットでは次のような設定をしている。	*/
	/* NTSC, no-turn-around, 16bit/pixel 						*/
	/* SPR_SetTvMode(SPR_TV_NORMAL, SPR_TV_320X224, 0);			*/
	
	SCL_SetFrameInterval( -1 );
	
	SCL_DisplayFrame();
	
	SCL_SetDisplayMode( SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A );
	SPR_SetTvMode( SPR_TV_NORMAL, SPR_TV_320X224, OFF );
	SPR_SetEraseData( 0x0000, 0, 0, 320-1, 224-1 );
}
void	_spr2_setspritecoord( int	x, int	y ){
	spCmd16_320_224[2].ax = x;
	spCmd16_320_224[2].ay = y;
}
void	_spr2_getspritecoord( int	*x, int	*y ){
	if( x != NULL )
		*x = spCmd16_320_224[2].ax;
	if( y != NULL )
		*y = spCmd16_320_224[2].ay;
}
