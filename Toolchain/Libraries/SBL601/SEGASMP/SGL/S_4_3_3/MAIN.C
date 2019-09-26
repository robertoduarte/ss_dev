/*----------------------------------------------------------------------*/
/*	Parallel Translation of 1 Polygon [X axis]			*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"

#define		POS_Z		60.0				/* 半径	*/

extern PDATA PD_PLANE1;

void main()
{
	static ANGLE	ang[XYZ];
	static FIXED	pos[XYZ];
	static ANGLE	tmp = DEGtoANG(0.0);	/* 角度変数 */

	/* 画面モードの設定	*/
	slInitSystem(TV_320x224,NULL,1);

	/* プログラムのタイトル表示 */
/*	slPrint("Sample program 4.3.3+" , slLocate(8,2));	*/

	/* 初期角度、位置の設定		*/
	ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
	pos[X] = toFIXED(  0.0);
	pos[Y] = toFIXED(  0.0);
	pos[Z] = toFIXED(220.0);

	/* 初期位置の修正	*/
	pos[X] = slMulFX(toFIXED(POS_Z), slCos(tmp));	
	pos[Y] = slMulFX(toFIXED(POS_Z), slSin(tmp));	

	/* メインループ */
	while(-1){
		slPushMatrix();						/* 演算マトリクス確保 */
		{
			slTranslate(pos[X] , pos[Y] , pos[Z]);
			slRotX(ang[X]);
			slRotY(ang[Y]);
			slRotZ(ang[Z]);

		/* 角度を変化させる	*/
			tmp += DEGtoANG(2.0);			

		/* Ｚ軸の回転	*/
			ang[Z] = tmp;

		/* ＸＹ座標の移動	*/
			pos[X] = slMulFX(toFIXED(POS_Z), slCos(tmp));	
			pos[Y] = slMulFX(toFIXED(POS_Z), slSin(tmp));	
			slPutPolygon(&PD_PLANE1);		/* ポリゴン描画関数 */
		}
		slPopMatrix();						/* 演算マトリクス呼び出し */

		slSynch();
	}
}
