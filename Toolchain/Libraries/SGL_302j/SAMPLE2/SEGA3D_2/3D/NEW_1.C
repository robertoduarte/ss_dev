POINT point_new_1[] = {
	POStoFIXED(-1.53787e-02, 6.02842e+00,-4.32139e+00),
	POStoFIXED(-1.53787e-02, 6.02842e+00, 4.32139e+00),
	POStoFIXED(-1.53787e-02,-2.61436e+00, 4.32139e+00),
	POStoFIXED(-1.53787e-02,-2.61436e+00,-4.32139e+00),
	POStoFIXED(-8.65816e+00, 6.02842e+00, 4.32139e+00),
	POStoFIXED(-8.65816e+00, 6.02842e+00,-4.32139e+00),
	POStoFIXED(-8.65816e+00,-2.61436e+00,-4.32139e+00),
	POStoFIXED(-8.65816e+00,-2.61436e+00, 4.32139e+00),
};

POLYGON polygon_new_1[] = {
	NORMAL( 1.00000e+00,-2.44667e-12, 2.20961e-11), VERTICES(  0,  1,  2,  3),
	NORMAL(-1.00000e+00, 1.66863e-08,-2.86883e-09), VERTICES(  4,  5,  6,  7),
	NORMAL( 5.26996e-09, 1.00000e+00, 7.67126e-09), VERTICES(  0,  5,  4,  1),
	NORMAL( 5.35681e-09,-1.00000e+00, 2.98170e-09), VERTICES(  2,  7,  6,  3),
	NORMAL(-7.45374e-09,-1.83187e-09, 1.00000e+00), VERTICES(  1,  4,  7,  2),
	NORMAL( 7.45374e-09,-1.83187e-09,-1.00000e+00), VERTICES(  5,  0,  3,  6),
};

ATTR attribute_new_1[] = {
	ATTRIBUTE(Single_Plane,SORT_CEN,TAILS,No_Palet,GRaddr+0,0|CL32KRGB|CL_Gouraud,sprHVflip,No_Option),
	ATTRIBUTE(Single_Plane,SORT_CEN,TAILS,No_Palet,GRaddr+1,0|CL32KRGB|CL_Gouraud,sprHVflip,No_Option),
	ATTRIBUTE(Single_Plane,SORT_CEN,MIKA,No_Palet,GRaddr+2,0|CL32KRGB|CL_Gouraud,sprNoflip,No_Option),
	ATTRIBUTE(Single_Plane,SORT_CEN,MIKA,No_Palet,GRaddr+3,0|CL32KRGB|CL_Gouraud,sprNoflip,No_Option),
	ATTRIBUTE(Single_Plane,SORT_CEN,SATURN,No_Palet,GRaddr+4,0|CL32KRGB|CL_Gouraud,sprHVflip,No_Option),
	ATTRIBUTE(Single_Plane,SORT_CEN,SATURN,No_Palet,GRaddr+5,0|CL32KRGB|CL_Gouraud,sprHVflip,No_Option),
};

PDATA PD_NEW_1[] = {
	point_new_1, sizeof(point_new_1)/sizeof(POINT),
	polygon_new_1, sizeof(polygon_new_1)/sizeof(POLYGON),
	attribute_new_1
};

