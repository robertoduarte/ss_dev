//------------------------------------------------------------------------
//
//	COLLIDE.C
//	Enigma actor collisions and attacks
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
#include "collide.h"
#include "assert.h"

ACTOR** collide_alist;
ACTOR** collide_dlist;

enum
{
	DIST_MANHATTAN,
	DIST_QUARTERS
};

extern int actor_tree_count;

int collide_mode = DIST_QUARTERS;

int collide_alist_count;
int collide_dlist_count;

EOBJECT* collide_attack_object;
EOBJECT* collide_defend_object;
VECTOR3F collide_loc;

int _CollideBuildListCallback( Tree** node )
{
	ACTOR* a = (ACTOR*)(*node);

	if (a->flags & AFLAG_ATTACK_MASK)
		collide_alist[ collide_alist_count++ ] = a;

	if ((a->flags & AFLAG_DEFEND_MASK) && (a->invuln == 0))
		collide_dlist[ collide_dlist_count++ ] = a;

	return( CONTINUE );	
}

void CollideBuildLists( void )
{
	collide_alist_count = 0;
	collide_dlist_count = 0;
	TreeIterate( &actor_tree, _CollideBuildListCallback );
}

int _CollideDefendCheckCallback( Node* n, long v1, long* v2 )
{
	EOBJECT* o = (EOBJECT*)n;
	ACTOR* a = (ACTOR*)v1;
	ACTOR* d = (ACTOR*)v2;
	FIXED dst, dx, dy, dz;

	collide_defend_object = o;

	if (o->flags & OFLAG_DEFENDER)
	{
		// okay, they are fully eligible here... I think.

		if (collide_mode == DIST_MANHATTAN)
		{
			dst  = ABS( o->pos.x - collide_attack_object->pos.x );
			dst += ABS( o->pos.y - collide_attack_object->pos.y );
			dst += ABS( o->pos.z - collide_attack_object->pos.z );
		}
		else
		{

			dx = ABS( o->pos.x - collide_attack_object->pos.x );
			dy = ABS( o->pos.y - collide_attack_object->pos.y );
			dz = ABS( o->pos.z - collide_attack_object->pos.z );

			if (dy > dx)
				SWAP( dx, dy );

			if (dz > dx)
				SWAP( dx, dz );

			dst = dx + (dy+dz>>2);
		}

		if (dst < (o->radius+collide_attack_object->radius))
		{
			// collision has occurred, notify all concerned

			collide_loc.x = (o->pos.x + collide_attack_object->pos.x)/2;
			collide_loc.y = (o->pos.y + collide_attack_object->pos.y)/2;
			collide_loc.z = (o->pos.z + collide_attack_object->pos.z)/2;

			ActSendMessage( d, AMESS_COLLIDE_DEF, a );
			ActSendMessage( a, AMESS_COLLIDE_ATT, d );
		}
	}
}

int _CollideAttackCheckCallback( Node* n, long v1, long* v2 )
{
	EOBJECT* o = (EOBJECT*)n;
	ACTOR* a = (ACTOR*)v1;
	ACTOR* d = (ACTOR*)v2;

	if (o->flags & OFLAG_ATTACKER)
	{
		collide_attack_object = o;
		LstIterateValue( &(d->object), _CollideDefendCheckCallback, (long)a, (long*)d );
	}	
	
	return( CONTINUE );
}

void CollidePositionCheck( ACTOR* a, ACTOR* d )
{
	LstIterateValue( &(a->object), _CollideAttackCheckCallback, (long)a, (long*)d );
}

void CollideScan( void )
{
	int anum, dnum;
	ACTOR* a;
	ACTOR* d;

	for (anum = 0; anum < collide_alist_count; anum++)
	{
		a = collide_alist[anum];

		for (dnum = 0; dnum < collide_dlist_count; dnum++)
		{
			d = collide_dlist[dnum];

			// try and avoid family disputes

			if ((d->parent != a) && (a->parent != d))
			{
				// for each attacker-defender pairing
				// if the attacker's attack-bits match the defender's
				// defense-bits, do position checks

				if (((a->flags & AFLAG_ATTACK_MASK)<<1) & d->flags)
					CollidePositionCheck( a, d );
			}
		}
	}
}

void CollideUpdate( void )
{
	collide_alist = (ACTOR**)COLLIDE_MEM_ALLOC( actor_tree_count * sizeof( ACTOR* ) );
	collide_dlist = (ACTOR**)COLLIDE_MEM_ALLOC( actor_tree_count * sizeof( ACTOR* ) );

	assert( collide_alist != NULL );
	assert( collide_dlist != NULL );

	CollideBuildLists();
	CollideScan();

	COLLIDE_MEM_FREE( collide_alist );
	COLLIDE_MEM_FREE( collide_dlist );
}
