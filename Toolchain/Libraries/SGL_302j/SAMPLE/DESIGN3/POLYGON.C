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

/***	CHILD 2	***/
OBJECT object_children2[]={
  pdata_sample,
  {toFIXED(-100),toFIXED(100),toFIXED(0)},
  {DEGtoANG(0),DEGtoANG(0),DEGtoANG(0)},
  {toFIXED(1),toFIXED(1),toFIXED(1)},
  NULL,
  NULL,
};
OBJECT object_child2[]={
  pdata_sample,
  {toFIXED(-100),toFIXED(100),toFIXED(0)},
  {DEGtoANG(0),DEGtoANG(0),DEGtoANG(0)},
  {toFIXED(1),toFIXED(1),toFIXED(1)},
  object_children2,
  NULL,
};

/***	CHILD 1	***/
OBJECT object_children1[]={
  pdata_sample,
  {toFIXED(100),toFIXED(100),toFIXED(0)},
  {DEGtoANG(0),DEGtoANG(0),DEGtoANG(0)},
  {toFIXED(1),toFIXED(1),toFIXED(1)},
  NULL,
  NULL,
};
OBJECT object_child1[]={
  pdata_sample,
  {toFIXED(100),toFIXED(100),toFIXED(0)},
  {DEGtoANG(0),DEGtoANG(0),DEGtoANG(0)},
  {toFIXED(1),toFIXED(1),toFIXED(1)},
  object_children1,
  object_child2,
};

/***	PARENT	***/
OBJECT object_sample[]={
  pdata_sample,
  {toFIXED(0),toFIXED(0),toFIXED(0)},
  {DEGtoANG(0),DEGtoANG(0),DEGtoANG(0)},
  {toFIXED(1),toFIXED(1),toFIXED(1)},
  object_child1,
  NULL,
};
