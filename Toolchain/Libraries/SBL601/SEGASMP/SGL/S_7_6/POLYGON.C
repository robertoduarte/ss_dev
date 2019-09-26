#include	"sgl.h"

#define		GRaddr		0xe000

static POINT point_CUBE[] = {
	POStoFIXED(-20.0 , -20.0 ,  20.0) ,
	POStoFIXED( 20.0 , -20.0 ,  20.0) ,
	POStoFIXED( 20.0 ,  20.0 ,  20.0) ,
	POStoFIXED(-20.0 ,  20.0 ,  20.0) ,
	POStoFIXED(-20.0 , -20.0 , -20.0) ,
	POStoFIXED( 20.0 , -20.0 , -20.0) ,
	POStoFIXED( 20.0 ,  20.0 , -20.0) ,
	POStoFIXED(-20.0 ,  20.0 , -20.0)
};

static POLYGON polygon_CUBE[] = {
	NORMAL( 0.0 , 0.0 , 1.0), VERTICES(0 , 1 , 2 , 3),
	NORMAL(-1.0 , 0.0 , 0.0), VERTICES(4 , 0 , 3 , 7),
	NORMAL( 0.0 , 0.0 ,-1.0), VERTICES(5 , 4 , 7 , 6),
	NORMAL( 1.0 , 0.0 , 0.0), VERTICES(1 , 5 , 6 , 2),
	NORMAL( 0.0 ,-1.0 , 0.0), VERTICES(4 , 5 , 1 , 0),
	NORMAL( 0.0 , 1.0 , 0.0), VERTICES(3 , 2 , 6 , 7)
};

static ATTR attribute_CUBE[] = {
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr,   MESHoff|CL_Gouraud, sprPolygon,No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr+1, MESHoff|CL_Gouraud, sprPolygon,No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr+2, MESHoff|CL_Gouraud, sprPolygon,No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr+3, MESHoff|CL_Gouraud, sprPolygon,No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr+4, MESHoff|CL_Gouraud, sprPolygon,No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(31,16,31), GRaddr+5, MESHoff|CL_Gouraud, sprPolygon,No_Option),
};

PDATA PD_CUBE = {
	point_CUBE , sizeof(point_CUBE)/sizeof(POINT),
	polygon_CUBE , sizeof(polygon_CUBE)/sizeof(POLYGON) ,
	attribute_CUBE
};
