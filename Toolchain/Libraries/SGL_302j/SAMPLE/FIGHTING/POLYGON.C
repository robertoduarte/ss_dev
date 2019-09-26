#include "sgl.h"

#include	"./polygon/miti.c"
#include	"./polygon/i1_rob.c"
#include	"./polygon/i2_rob.c"

POINT	point_BOX[]={
	POStoFIXED( -16.00,  16.00, -16.00),
	POStoFIXED( -16.00,  16.00,  16.00),
	POStoFIXED(  16.00,  16.00,  16.00),
	POStoFIXED(  16.00,  16.00, -16.00),
	POStoFIXED( -16.00, -16.00,  16.00),
	POStoFIXED(  16.00, -16.00,  16.00),
	POStoFIXED( -16.00, -16.00, -16.00),
	POStoFIXED(  16.00, -16.00, -16.00),
};

POLYGON polygon_BOX[]={
	NORMAL(    0,   50,    0), VERTICES(  0,  1,  2,  3),
	NORMAL(    0,    0,   50), VERTICES(  1,  4,  5,  2),
	NORMAL(    0,  -50,    0), VERTICES(  4,  6,  7,  5),
	NORMAL(    0,    0,  -50), VERTICES(  6,  0,  3,  7),
	NORMAL(   50,    0,    0), VERTICES(  3,  2,  5,  7),
	NORMAL(  -50,    0,    0), VERTICES(  1,  0,  6,  4),
};

ATTR attribute_BOX[]={
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31, 0, 8),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31,23, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31, 0, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31,23, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(10,10,31),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(10,10,31),No_Gouraud,MESHoff,sprPolygon,UseClip),
};

PDATA PD_BOX = {
	point_BOX,sizeof(point_BOX)/sizeof(POINT),
	polygon_BOX,sizeof(polygon_BOX)/sizeof(POLYGON),
	attribute_BOX
};
POINT	point_box2[]={
	POStoFIXED(  24.00,  16.00, -24.00),
	POStoFIXED(  24.00,  16.00,  24.00),
	POStoFIXED(   0.00, -16.00,   0.00),
	POStoFIXED( -24.00,  16.00,   0.00),
};

POLYGON polygon_box2[]={
	NORMAL(   46,  -35,    0), VERTICES(  0,  1,  2,  2),
	NORMAL(  -23,  -17,   46), VERTICES(  1,  3,  2,  2),
	NORMAL(  -23,  -17,  -46), VERTICES(  3,  0,  2,  2),
	NORMAL(    0,   70,    0), VERTICES(  3,  1,  0,  0),
};

ATTR attribute_box2[]={
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31, 8, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31,23, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(15,31, 0),No_Gouraud,MESHoff,sprPolygon,UseClip),
	ATTRIBUTE(Single_Plane,SORT_CEN,No_Texture,C_RGB(31,15,27),No_Gouraud,MESHoff,sprPolygon,UseClip),
};

PDATA PD_box2 = {
	point_box2,sizeof(point_box2)/sizeof(POINT),
	polygon_box2,sizeof(polygon_box2)/sizeof(POLYGON),
	attribute_box2
};

#define	miss_pal	0xff1f

POLYGON polygon_box[]={
	NORMAL(    0,    0,   -1), VERTICES(  0,  1,  2,  3),
};


