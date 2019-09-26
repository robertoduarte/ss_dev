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

void s_sp3InitialMatrix(void);

void s_sp3ClearCurrentMatrix(Void);

void s_sp3ViewPointAngle(GlbXYZ *pivotViewPoint, GlbXYZ *viewPoint,
		       GlbXYZ *viewAngle);

void s_sp3LightAngle(GlbXYZ *lightAngle);

void s_sp3ViewCoordMatrix(Void);

void s_sp3PushMatrix(Void);

void s_sp3PopMatrix(Void);

void s_sp3RotateMatrixX(Fixed32 angle);

void s_sp3RotateMatrixY(Fixed32 angle);

void s_sp3RotateMatrixZ(Fixed32 angle);

void s_sp3ReversMatrixZ(void);

void s_sp3MoveMatrix(Fixed32 x, Fixed32 y, Fixed32 z);

void s_sp3SurfaceBright(Uint16 entryCount, GlbXYZ *surfacePoint,
		      GlbXYZ *surfaceNormal, Uint8 *surfaceBright);

void s_sp3CoordTrans(Uint16 entryCount, GlbXYZ *srcPoint, GlbXYZ *ansPoint);

void s_sp3VertBright(Uint16 entryCount, GlbXYZ *vertNormal, Uint8 *pointBright);

void s_sp3CoordTransSub(GlbXYZ *src, GlbXYZ *ans);

void s_sp3NormalTrans(GlbXYZ *src, GlbXYZ *ans);

void s_sp3ComputeBrightness(GlbXYZ *view, GlbXYZ *norm, Uint8 *colorNo);

/*  end of file */
