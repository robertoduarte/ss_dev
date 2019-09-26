#include	"sgl.h"

POINT point_sample[]={
	POStoFIXED(-50.0, -50.0, -50.0),
	POStoFIXED(-50.0, -50.0,  50.0),
	POStoFIXED(-50.0,  50.0, -50.0),
	POStoFIXED(-50.0,  50.0,  50.0),
	POStoFIXED( 50.0, -50.0, -50.0),
	POStoFIXED( 50.0, -50.0,  50.0),
	POStoFIXED( 50.0,  50.0, -50.0),
	POStoFIXED( 50.0,  50.0,  50.0),
};

POLYGON polygon_sample[]={
	NORMAL(-1.0,  0.0,  0.0), VERTICES(0, 1, 3, 2),
	NORMAL( 0.0,  0.0,  1.0), VERTICES(1, 5, 7, 3),
	NORMAL( 1.0,  0.0,  0.0), VERTICES(5, 4, 6, 7),
	NORMAL( 0.0,  0.0, -1.0), VERTICES(4, 0, 2, 6),
	NORMAL( 0.0, -1.0,  0.0), VERTICES(4, 5, 1, 0),
	NORMAL( 0.0,  1.0,  0.0), VERTICES(2, 3, 7, 6),
};


ATTR attribute_sample[]={
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB( 0, 20, 28), No_Gouraud, MESHoff, sprPolygon, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(28,  8,  0), No_Gouraud, MESHoff, sprPolygon, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB( 8, 28,  0), No_Gouraud, MESHoff, sprPolygon, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(28, 20,  0), No_Gouraud, MESHoff, sprPolygon, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(16,  0, 28), No_Gouraud, MESHoff, sprPolygon, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, No_Texture, C_RGB(28, 20, 16), No_Gouraud, MESHoff, sprPolygon, No_Option),
};

PDATA pdata_sample[] = {
	point_sample, sizeof(point_sample) / sizeof(POINT),
	polygon_sample, sizeof(polygon_sample) / sizeof(POLYGON),
	attribute_sample
};
