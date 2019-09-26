//------------------------------------------------------------------------
//
//	AILAND.C
//	Land surface actor
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 4/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------
#include <stdlib.h>

// Floating point math package is used *only* to precalculate an 
// interestingly curved "ground" surface. Not used during main run loop.

#include <math.h>

// engima.h includes sl_def.h which defines M_PI, also defined in math.h.
// this suppresses a redefinition warning.

#undef M_PI

#include "enigma.h"
#include "actor.h"
#include "texture.h"

#define random(range)		(rand()%(range))

ATTR land_attr = 
	ATTRIBUTE(Single_Plane, SORT_CEN, TEX_LAND, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, UseClip);

#define LAND_X_PATCHES		6
#define LAND_Y_PATCHES		6
#define LAND_PATCHES		(LAND_X_PATCHES*LAND_Y_PATCHES)
#define LAND_X_VERTS		(LAND_X_PATCHES+1)
#define LAND_Y_VERTS		(LAND_Y_PATCHES+1)
#define LAND_VERTS			(LAND_X_VERTS*LAND_Y_VERTS)

#define LAND_PATCH_SIZE		(toFIXED(1024.0/LAND_X_PATCHES))
#define LAND_Y_POS			toFIXED(-500.0)
#define DISTORT				toFIXED(0.0)

void AILandBuild( ACTOR* act )
{
	EOBJECT* obj;
	SATPOLY* sg;
	PDATA* pdata;
	POLYGON* pg;
	POINT* pt;
	ATTR* at;
	int v0, v1, v2, v3;
	int i, j, v, p;
	VECTOR3F edge1, edge2, norm;

	double xx, yy, zz;

	obj = ActAddObject( act, OFLAG_SATPOLY|OFLAG_HUGE|OFLAG_AUTOROTATE, LAND_VERTS, LAND_PATCHES );

	obj->pos.x = toFIXED(0.0);
	obj->pos.y = LAND_Y_POS;
	obj->pos.z = toFIXED(0.0);
	obj->rot.x = (ANGLE)0; 
	obj->rot.y = (ANGLE)0; 
	obj->rot.z = (ANGLE)0; 
	obj->rotvel.x = (ANGLE)0; 
	obj->rotvel.y = (ANGLE)0; 
	obj->rotvel.z = (ANGLE)0; 
	obj->vel.x = toFIXED(0.0);
	obj->vel.y = toFIXED(0.0);
	obj->vel.z = toFIXED(0.0);
	
	sg = (SATPOLY*)(obj->graph);
	pdata = &(sg->pdata);
	pg = pdata->pltbl;
	pt = pdata->pntbl;
	at = pdata->attbl;

	for (i = 0, v = 0; i < LAND_X_VERTS; i++)
	{
		for (j = 0; j < LAND_Y_VERTS; j++, v++)
		{
			pt[v][X] = slMulFX( toFIXED(i-(LAND_X_PATCHES/2)), LAND_PATCH_SIZE );
			pt[v][Z] = slMulFX( toFIXED(j-(LAND_Y_PATCHES/2)), LAND_PATCH_SIZE );
			
			xx = (double)pt[v][X];
			xx /= 65536.0;				
			xx /= 2.0;
			zz = (double)pt[v][Z];
			zz /= 65536.0;
			zz /= 2.0;
			yy = sqrt( 250000.0 - (xx*xx + zz*zz) );

			pt[v][Y] = toFIXED( yy - 500.0 );

			pt[v][Y] += DISTORT;
		}
	}

	for (i = 0, p = 0; i < LAND_X_PATCHES; i++)
	{
		for (j = 0; j < LAND_Y_PATCHES; j++, p++)
		{
			v0 = pg[p].Vertices[0] = ( i   *LAND_Y_VERTS)+j;
			v1 = pg[p].Vertices[1] = ((i+1)*LAND_Y_VERTS)+j;
			v2 = pg[p].Vertices[2] = ((i+1)*LAND_Y_VERTS)+j+1;
			v3 = pg[p].Vertices[3] = ( i   *LAND_Y_VERTS)+j+1;

			edge1.x = pt[v0][X] - pt[v1][X];
			edge1.y = pt[v0][Y] - pt[v1][Y];
			edge1.z = pt[v0][Z] - pt[v1][Z];

			edge2.x = pt[v3][X] - pt[v0][X];
			edge2.y = pt[v3][Y] - pt[v0][Y];
			edge2.z = pt[v3][Z] - pt[v0][Z];

			VecCross( &norm, &edge1, &edge2 );
			VecNormalize( &norm );

			pg[p].norm[X] = norm.x;
			pg[p].norm[Y] = norm.y;
			pg[p].norm[Z] = norm.z;

			at[p] = land_attr;	 
		}
	}
}

int AILandMsgHand( ACTOR* act, MESSAGE_FLAGS mess, long data )
{
	switch( mess )
	{
		case AMESS_CREATE:
		AILandBuild( act );
		return( TRUE );

		default:
		return( FALSE );
	}
}

