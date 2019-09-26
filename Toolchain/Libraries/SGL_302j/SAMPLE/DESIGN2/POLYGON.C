#include	"sgl.h"
enum texno{
	AM2_64X32,
	SONIC_64X64,
};

POINT point_sample[] = {
	POStoFIXED(-50.0, -50.0, -50.0),
	POStoFIXED(-50.0, -50.0,  50.0),
	POStoFIXED(-50.0,  50.0, -50.0),
	POStoFIXED(-50.0,  50.0,  50.0),
	POStoFIXED( 50.0, -50.0, -50.0),
	POStoFIXED( 50.0, -50.0,  50.0),
	POStoFIXED( 50.0,  50.0, -50.0),
	POStoFIXED( 50.0,  50.0,  50.0),
};

POLYGON polygon_sample[] = {
	NORMAL(-1.0,  0.0,  0.0), VERTICES(0, 1, 3, 2),
	NORMAL( 0.0,  0.0,  1.0), VERTICES(1, 5, 7, 3),
	NORMAL( 1.0,  0.0,  0.0), VERTICES(5, 4, 6, 7),
	NORMAL( 0.0,  0.0, -1.0), VERTICES(4, 0, 2, 6),
	NORMAL( 0.0, -1.0,  0.0), VERTICES(4, 5, 1, 0),
	NORMAL( 0.0,  1.0,  0.0), VERTICES(2, 3, 7, 6),
};


ATTR attribute_sample[] = {
	ATTRIBUTE(Single_Plane, SORT_MIN, AM2_64X32, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, SONIC_64X64, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, SONIC_64X64, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, AM2_64X32, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, AM2_64X32, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
	ATTRIBUTE(Single_Plane, SORT_MIN, SONIC_64X64, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
};

PDATA pdata_sample[] = {
	point_sample, sizeof(point_sample) / sizeof(POINT),
	polygon_sample, sizeof(polygon_sample) / sizeof(POLYGON),
	attribute_sample
};
