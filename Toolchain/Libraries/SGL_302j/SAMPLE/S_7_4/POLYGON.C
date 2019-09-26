#include	"sgl.h"

#define		PN_SONIC		0
#define		PN_AM2			1

POINT point_plane[] = {
        POStoFIXED(-40.0 , -40.0 , 0.0) ,
        POStoFIXED( 40.0 , -40.0 , 0.0) ,
        POStoFIXED( 40.0 ,  40.0 , 0.0) ,
        POStoFIXED(-40.0 ,  40.0 , 0.0)
};

POLYGON polygon_plane[] = {
        NORMAL(0.0,0.0,1.0), VERTICES(0 , 1 , 2 , 3)
};

ATTR attribute_plane[] = {
	ATTRIBUTE(Single_Plane, SORT_CEN, PN_SONIC, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, No_Option),
};

PDATA PD_PLANE = {
        point_plane , sizeof(point_plane)/sizeof(POINT),
        polygon_plane, sizeof(polygon_plane)/sizeof(POLYGON),
        attribute_plane
};
