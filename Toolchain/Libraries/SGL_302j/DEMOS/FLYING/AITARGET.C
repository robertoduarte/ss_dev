//------------------------------------------------------------------------
//
//	AITARGET.C
//	Target actors
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

#include "enigma.h"
#include "actor.h"
#include "vector.h"
#include "ai.h"
#include "texture.h"
#include "assert.h"

ATTR target_attr = 
	ATTRIBUTE(Dual_Plane, SORT_CEN, TEX_TARGET, No_Palet, No_Gouraud, CL32KRGB|MESHoff, sprNoflip, UseClip);

void AITargetProcess1( ACTOR* act, PROCESS* proc )
{
}

void AITargetBuild( ACTOR* act )
{
	EOBJECT* obj;
	EOBJECT* sobj;
	SATPOLY* sg;
	PDATA* pdata;
	POLYGON* pg;
	POINT* pt;
	ATTR* at;
	int v, f;

	obj = ActAddObject( act, OFLAG_AUTOMOVE|OFLAG_AUTOROTATE|OFLAG_SATPOLY, 8, 6 );
	
	obj->pos.x = FRANDOM( 4000.0 ) - toFIXED(2000.0);
	obj->pos.y = FRANDOM( 300.0 ) + toFIXED( 100.0 );
	obj->pos.z = FRANDOM( 4000.0 ) + toFIXED(200.0);
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

	for (v = 0; v < 8; v++)
	{
		pt[v][X] = (v&1) ? toFIXED(  50.0 ) : toFIXED( -50.0 );
		pt[v][Y] = (v&2) ? toFIXED( -50.0 ) : toFIXED(  50.0 );
		pt[v][Z] = (v&4) ? toFIXED(  50.0 ) : toFIXED( -50.0 );
	}

	for (f = 0; f < 6; f++)
	{
		pg[f].norm[X] = toFIXED(  0.0 );
		pg[f].norm[Y] = toFIXED(  0.0 );
		pg[f].norm[Z] = toFIXED( -1.0 );
		at[f] = target_attr;	 
	}

	pg[0].Vertices[0] = 0;
	pg[0].Vertices[1] = 1;
	pg[0].Vertices[2] = 3;
	pg[0].Vertices[3] = 2;

	pg[1].Vertices[0] = 1;
	pg[1].Vertices[1] = 5;
	pg[1].Vertices[2] = 7;
	pg[1].Vertices[3] = 3;

	pg[2].Vertices[0] = 0;
	pg[2].Vertices[1] = 4;
	pg[2].Vertices[2] = 5;
	pg[2].Vertices[3] = 1;

	pg[3].Vertices[0] = 4;
	pg[3].Vertices[1] = 5;
	pg[3].Vertices[2] = 7;
	pg[3].Vertices[3] = 6;

	pg[4].Vertices[0] = 0;
	pg[4].Vertices[1] = 4;
	pg[4].Vertices[2] = 6;
	pg[4].Vertices[3] = 2;

	pg[5].Vertices[0] = 2;
	pg[5].Vertices[1] = 6;
	pg[5].Vertices[2] = 7;
	pg[5].Vertices[3] = 3;

	// collision features...

	act->flags |= AFLAG_DEFEND_MASK;	// muchly hittable
	obj->flags |= OFLAG_DEFENDER;
	obj->radius = toFIXED(62.0);		// vol of 62 radius sphere = 100 edge cube
}

int AITargetMsgHand( ACTOR* act, MESSAGE_FLAGS mess, long data )
{
	EOBJECT* obj;

	switch( mess )
	{
		case AMESS_CREATE:
		AITargetBuild( act );
		ActAddProcess( act, AITargetProcess1, DELAY_60HZ, DELAY_60HZ, 1 );
		return( TRUE );

		case AMESS_COLLIDE_DEF:

		ActSendMessage( act, AMESS_DESTROY, 0 );	// kill self
		AICreateActor( AT_KEY_TARGET, NULL );
		return( FALSE );		// do default processing as well


		default:
		return( FALSE );
	}
}

