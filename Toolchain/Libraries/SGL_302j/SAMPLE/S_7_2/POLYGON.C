#include	"sgl.h"

POINT point_PLANE[] = {
	POStoFIXED(-20.0, -20.0, 0.0),
	POStoFIXED( 20.0, -20.0, 0.0),
	POStoFIXED( 20.0,  20.0, 0.0),
	POStoFIXED(-20.0,  20.0, 0.0),
};

POLYGON polygon_PLANE[] = {
	NORMAL(0.0,0.0,1.0), VERTICES(0,1,2,3),
};

ATTR attribute_PLANE[] = {
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31,31,0),No_Gouraud,MESHoff,sprPolygon,No_Option),
};

PDATA PD_PLANE = {
	point_PLANE , sizeof(point_PLANE)/sizeof(POINT) ,
	polygon_PLANE , sizeof(polygon_PLANE)/sizeof(POLYGON) ,
	attribute_PLANE
};

