#include	"sgl.h"

POINT point_CUBE[] = {
	POStoFIXED(-10.0, -10.0,  10.0),	/* 頂点 00 */
	POStoFIXED( 10.0, -10.0,  10.0),	/*		01 */
	POStoFIXED( 10.0,  10.0,  10.0),	/*		02 */
	POStoFIXED(-10.0,  10.0,  10.0),	/*		03 */
	POStoFIXED(-10.0, -10.0, -10.0),	/*		04 */
	POStoFIXED( 10.0, -10.0, -10.0),	/*		05 */
	POStoFIXED( 10.0,  10.0, -10.0),	/*		06 */
	POStoFIXED(-10.0,  10.0, -10.0),	/*		07 */
};

POLYGON polygon_CUBE[] = {
	NORMAL( 0.0, 0.0, 1.0), VERTICES(0,1,2,3),	/* 法線ﾍﾞｸﾄﾙ :面 00	*/
	NORMAL(-1.0, 0.0, 0.0), VERTICES(4,0,3,7),	/*				 01 */
	NORMAL( 0.0, 0.0,-1.0), VERTICES(5,4,7,6),	/*				 02 */
	NORMAL( 1.0, 0.0, 0.0), VERTICES(1,5,6,2),	/*				 03 */
	NORMAL( 0.0,-1.0, 0.0), VERTICES(4,5,1,0),	/*				 04 */
	NORMAL( 0.0, 1.0, 0.0), VERTICES(3,2,6,7),	/*				 05 */
};

ATTR attribute_CUBE1[] = {
	/* 面属性 : 面 00 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,16,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 01 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,6,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 02 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,16,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 03 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,6,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 04 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(20,20,10),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 05 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(20,20,10),No_Gouraud,MESHoff,sprPolygon,No_Option),
};

ATTR attribute_CUBE2[] = {
	/* 面属性 : 面 00 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,16,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 01 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,6,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 02 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,16,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 03 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(6,6,25),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 04 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(20,20,10),No_Gouraud,MESHoff,sprPolygon,No_Option),
	/* 面属性 : 面 05 */
	ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(20,20,10),No_Gouraud,MESHoff,sprPolygon,No_Option),
};

PDATA PD_CUBE1 = {
	point_CUBE,sizeof(point_CUBE)/sizeof(POINT),
	polygon_CUBE,sizeof(polygon_CUBE)/sizeof(POLYGON),
	attribute_CUBE1
};
PDATA PD_CUBE2 = {
	point_CUBE,sizeof(point_CUBE)/sizeof(POINT),
	polygon_CUBE,sizeof(polygon_CUBE)/sizeof(POLYGON),
	attribute_CUBE2
};


/*-------	ショット弾	----------*/ 
POINT point_PLANE1[] = {
	POStoFIXED( -3.0,  -3.0,   0.0),	/*		00 */
	POStoFIXED(  3.0,  -3.0,   0.0),	/*		01 */
	POStoFIXED(  3.0,   3.0,   0.0),	/*		02 */
	POStoFIXED( -3.0,   3.0,   0.0),	/*		03 */
};

POLYGON polygon_PLANE1[] = {
        NORMAL(0.0, 0.0,-1.0), VERTICES(0, 1, 2, 3),
};

ATTR attribute_PLANE1[] = {
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(31,31,31), No_Gouraud, MESHoff, sprPolygon, No_Option),
};

PDATA PD_PLANE1 = {
        point_PLANE1, sizeof(point_PLANE1) / sizeof(POINT),
        polygon_PLANE1, sizeof(polygon_PLANE1) / sizeof(POLYGON),
        attribute_PLANE1
};


/*----------- 壊れたパターン -------------*/

ATTR attribute_BREAK[] = {
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(6,6,25), No_Gouraud, MESHoff, sprPolygon, No_Option),
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(6,16,25), No_Gouraud, MESHoff, sprPolygon, No_Option),
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(20,20,10), No_Gouraud, MESHoff, sprPolygon, No_Option),
};


POLYGON polygon_BREAK1[] = {
	NORMAL( 0.0, 0.0, 1.0), VERTICES(0,1,2,3),	/* 法線ﾍﾞｸﾄﾙ :面 00	*/
};
POLYGON polygon_BREAK2[] = {
	NORMAL(-1.0, 0.0, 0.0), VERTICES(4,0,3,7),	/*				 01 */
};
POLYGON polygon_BREAK3[] = {
	NORMAL( 0.0, 0.0,-1.0), VERTICES(5,4,7,6),	/*				 02 */
};
POLYGON polygon_BREAK4[] = {
	NORMAL( 1.0, 0.0, 0.0), VERTICES(1,5,6,2),	/*				 03 */
};
POLYGON polygon_BREAK5[] = {
	NORMAL( 0.0,-1.0, 0.0), VERTICES(4,5,1,0),	/*				 04 */
};
POLYGON polygon_BREAK6[] = {
	NORMAL( 0.0, 1.0, 0.0), VERTICES(3,2,6,7),	/*				 05 */
};

PDATA PD_BREAK1 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK1, sizeof(polygon_BREAK1) / sizeof(POLYGON),
        &attribute_BREAK[1]
};
PDATA PD_BREAK2 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK2, sizeof(polygon_BREAK2) / sizeof(POLYGON),
        &attribute_BREAK[0]
};
PDATA PD_BREAK3 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK3, sizeof(polygon_BREAK3) / sizeof(POLYGON),
        &attribute_BREAK[1]
};
PDATA PD_BREAK4 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK4, sizeof(polygon_BREAK4) / sizeof(POLYGON),
        &attribute_BREAK[0]
};
PDATA PD_BREAK5 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK5, sizeof(polygon_BREAK5) / sizeof(POLYGON),
        &attribute_BREAK[2]
};
PDATA PD_BREAK6 = {
        point_CUBE, sizeof(point_CUBE) / sizeof(POINT),
        polygon_BREAK6, sizeof(polygon_BREAK6) / sizeof(POLYGON),
        &attribute_BREAK[2]
};








