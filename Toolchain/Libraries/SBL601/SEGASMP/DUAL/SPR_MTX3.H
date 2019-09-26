/*----------------------------------------------------------------------------
 *  spr_mtx3.h -- SPR ライブラリ MTX3 インクルードファイル
 *  Copyright(c) 1994 SEGA
 *  Written by K.M on 1993-12-14 Ver.0.80
 *  Updated by H.E on 1994-02-15 Ver.0.80
 *
 *  このインクルードファイルはスプライト３Ｄ表示のマトリックス演算ルーチン
 *  定義。
 *
 *----------------------------------------------------------------------------
 */

#define	PRODUCT(a, b)\
(MUL_FIXED((a).x, (b).x) + MUL_FIXED((a).y, (b).y) + MUL_FIXED((a).z, (b).z))

void sp3InitialMatrix(void);

void sp3ClearCurrentMatrix(Void);

void sp3ViewPointAngle(GlbXYZ *pivotViewPoint, GlbXYZ *viewPoint,
		       GlbXYZ *viewAngle);

void sp3LightAngle(GlbXYZ *lightAngle);

void sp3ViewCoordMatrix(Void);

void sp3PushMatrix(Void);

void sp3PopMatrix(Void);

void sp3RotateMatrixX(Fixed32 angle);

void sp3RotateMatrixY(Fixed32 angle);

void sp3RotateMatrixZ(Fixed32 angle);

void sp3ReversMatrixZ(void);

void sp3MoveMatrix(Fixed32 x, Fixed32 y, Fixed32 z);

void sp3SurfaceBright(Uint16 entryCount, GlbXYZ *surfacePoint,
		      GlbXYZ *surfaceNormal, Uint8 *surfaceBright);

void sp3CoordTrans(Uint16 entryCount, GlbXYZ *srcPoint, GlbXYZ *ansPoint);

void sp3VertBright(Uint16 entryCount, GlbXYZ *vertNormal, Uint8 *pointBright);

void sp3CoordTransSub(GlbXYZ *src, GlbXYZ *ans);

void sp3NormalTrans(GlbXYZ *src, GlbXYZ *ans);

void sp3ComputeBrightness(GlbXYZ *view, GlbXYZ *norm, Uint8 *colorNo);

/*  end of file */
