#include	"sgl.h"

POINT point_CUBE[] = {
        POStoFIXED(-15.0,-15.0,-15.0),
        POStoFIXED(-15.0,-15.0, 15.0),
        POStoFIXED(-15.0, 15.0,-15.0),
        POStoFIXED(-15.0, 15.0, 15.0),
        POStoFIXED( 15.0,-15.0,-15.0),
        POStoFIXED( 15.0,-15.0, 15.0),
        POStoFIXED( 15.0, 15.0,-15.0),
        POStoFIXED( 15.0, 15.0, 15.0),
};

POLYGON polygon_CUBE[] = {
        NORMAL(-1.0, 0.0, 0.0), VERTICES(0,1,3,2),
        NORMAL( 0.0, 0.0, 1.0), VERTICES(1,5,7,3),
        NORMAL( 1.0, 0.0, 0.0), VERTICES(5,4,6,7),
        NORMAL( 0.0, 0.0,-1.0), VERTICES(4,0,2,6),
        NORMAL( 0.0,-1.0, 0.0), VERTICES(4,5,1,0),
        NORMAL( 0.0, 1.0, 0.0), VERTICES(2,3,7,6),
};

ATTR attribute_CUBE[] = {
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(31,31,00),No_Gouraud,MESHoff,sprPolygon,UseLight),
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(31,00,00),No_Gouraud,MESHoff,sprPolygon,UseLight),
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(00,31,00),No_Gouraud,MESHoff,sprPolygon,UseLight),
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(00,00,31),No_Gouraud,MESHoff,sprPolygon,UseLight),
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(31,00,31),No_Gouraud,MESHoff,sprPolygon,UseLight),
        ATTRIBUTE(Single_Plane,SORT_MIN,No_Texture,C_RGB(00,31,31),No_Gouraud,MESHoff,sprPolygon,UseLight),
};

PDATA PD_CUBE = {
        point_CUBE,sizeof(point_CUBE)/sizeof(POINT),
        polygon_CUBE,sizeof(polygon_CUBE)/sizeof(POLYGON),
        attribute_CUBE
};
