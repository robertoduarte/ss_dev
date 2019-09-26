//------------------------------------------------------------------------
//
//	RENDER.C
//	Enigma actor engine rendering stuff
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
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
#include "render.h"
#include "mem_mgr.h"
#include "vector.h"
#include "texture.h"
#include "assert.h"

Tree* graphic_tree;
EOBJECT* rendCamera = NULL;
EOBJECT* rendCameraTarget = NULL;

int rendActorsHit;
int rendObjectsHit;
int rendPatchesRendered;
int rendFramesRendered = 0;

FIXED rendGrossDistanceClip = toFIXED(30000.0);

extern long demoClock;

void RendDebug( void )
{
	slPrintHex( SatGetTime(), slLocate( 2, 4 ) );
	slPrintHex( demoClock, slLocate( 2, 5 ) );

	slPrintHex( rendActorsHit, slLocate( 7, 17 ) );
	slPrintHex( rendObjectsHit, slLocate( 7, 18 ) );
	slPrintHex( rendPatchesRendered, slLocate( 7, 19 ) );
	slPrint( "Actors  :", slLocate( 2, 17 ) );
	slPrint( "Objects :", slLocate( 2, 18 ) );
	slPrint( "Polygons:", slLocate( 2, 19 ) );


}

void RendSatPolyObject( EOBJECT* obj )
{
	SATPOLY* sg = (SATPOLY*)obj->graph;
	PDATA* sgp = &(sg->pdata);
	char* ro;

	rendPatchesRendered += sgp->nbPolygon;

	slPushMatrix();

	// translate object coords

	slTranslate( obj->pos.x, obj->pos.y, obj->pos.z );

	// rotate object local coords

	for (ro = sg->rotorder; *ro != '\0'; ro++)
	{
		switch(*ro)
		{
			case 'x':	slRotX( obj->rot.x );	break;
			case 'y':	slRotY( obj->rot.y );	break;
			case 'z':	slRotZ( obj->rot.z );	break;
			default:	assert( TRUE==FALSE );	break;
		}
	}	

	slPutPolygon( sgp );

	slPopMatrix();
}	

void RendSatSpriteObject( EOBJECT* obj )
{
	SATSPRITE* ss = (SATSPRITE*)obj->graph;
	FIXED pos[XYZS];

	rendPatchesRendered++;

	slPushMatrix();

	pos[X] = obj->pos.x;
	pos[Y] = obj->pos.y;
	pos[Z] = obj->pos.z;
	pos[S] = ss->scale;

	slPutSprite( pos, ss->attr, ss->zrot );

	slPopMatrix();
}	

void RendSatBackObject( EOBJECT* obj )
{

}	

int RendObjectCallback( Node* n )
{
	EOBJECT* o = (EOBJECT*)n;
	FIXED dst, dx, dy, dz;

	rendObjectsHit++;

	dx = ABS( o->pos.x - rendCamera->pos.x );
	dy = ABS( o->pos.y - rendCamera->pos.y );
	dz = ABS( o->pos.z - rendCamera->pos.z );

	if (dy > dx)
		SWAP( dx, dy );
	if (dz > dx)
		SWAP( dx, dz );
	dst = dx + (dy+dz>>2);

	if (dst > rendGrossDistanceClip)
		return( CONTINUE );	

	switch( o->flags & OFLAG_MASK_GRAPHIC )
	{
		case OFLAG_SATPOLY:
		RendSatPolyObject( o );
		break;

		case OFLAG_SATSPRITE:
		RendSatSpriteObject( o );
		break;

		case OFLAG_SATBACKGR:
		RendSatBackObject( o );
		break;

		case OFLAG_NOGRAPH:
		case OFLAG_NVGRAPH:
		default:
		break;
	}

	return( CONTINUE );	
}

int RendActorCallback( Tree** node )
{
	ACTOR* a = (ACTOR*)(*node);

	rendActorsHit++;

	if (a->flags & AFLAG_OBJECT)
	{
		LstIterate( &(a->object), RendObjectCallback );
	}

	return( CONTINUE );
}


int RendInit( void )
{
	slInitSystem( TV_320x224, tex_set, 1 );
	TextureInit();
	slInitSynch();
	slZdspLevel( 1 );
	slWindow( 0, 0, 319, 223, 0x7FFF, 160, 112 );
	slPrint( "Irrelevant Text Display", slLocate( 2, 2 ) );
	return( TRUE );
}

void RendUpdate( int clock )
{
	FIXED light[XYZ];
	FIXED campos[XYZ];
	FIXED camtarg[XYZ];

	rendActorsHit = rendObjectsHit = rendPatchesRendered = 0;

	// clear the screen

	// tear down the tree
	TreeInit( &graphic_tree );

	// make sure textures are ready
	slDMAWait();

	// transform all the objects and rebuild the tree

	if (rendCamera != NULL)
	{
		campos[X] = rendCamera->pos.x;
		campos[Y] = rendCamera->pos.y;
		campos[Z] = rendCamera->pos.z;

		if (rendCameraTarget != NULL)
		{
			camtarg[X] = rendCameraTarget->pos.x;
			camtarg[Y] = rendCameraTarget->pos.y;
			camtarg[Z] = rendCameraTarget->pos.z;
		}
		else
		{
			camtarg[X] = toFIXED( 0.0 );
			camtarg[Y] = toFIXED( 0.0 );
			camtarg[Z] = toFIXED( 0.0 );
		}

		slUnitMatrix( CURRENT );
		slLookAt( campos, camtarg, DEGtoANG(180.0) );

		slPushUnitMatrix();
			slRotY( (ANGLE)clock*180 );
			slRotX( DEGtoANG(-5.0) );
			slRotZ( DEGtoANG(-5.0) );
			slCalcPoint( toFIXED(0.0), toFIXED(0.0), toFIXED(1.0), light );
		slPopMatrix();
		slLight( light );

		TreeIterate( &actor_tree, RendActorCallback );
	}

	// update

	RendDebug();
	slSynch();
	rendFramesRendered++;
}

void RendShutdown( void )
{
}
