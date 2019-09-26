//------------------------------------------------------------------------
//
//	ACTOR.C
//	"actor engine" stuff
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
//	  8/7/96 - RAB - cleanup for demo release
//
//------------------------------------------------------------------------

#include "enigma.h"
#include "assert.h"
#include "mem_mgr.h"
#include "list.h"
#include "tree.h"
#include "vector.h"
#include "actor.h"

static int num_actors_executing;
static int actor_tree_extant = FALSE;

char* act_xyz_order = "xyz";
char* act_xzy_order = "xzy";
char* act_yxz_order = "yxz";
char* act_yzx_order = "yzx";
char* act_zxy_order = "zxy";
char* act_zyx_order = "zyx";

Tree* actor_tree;
int actor_tree_count = 0;

List actor_kill_list;
List* actor_kill_ptr = &actor_kill_list;

VECTOR3F actGravVect = { toFIXED(0.0), toFIXED(-1.0), toFIXED(0.0) };

int _ActFreeProcessCallback( Node* n, long v1, long* v2 )
{
	PROCESS* p = (PROCESS*)n;
	List* l = &(((ACTOR*)v2)->process);

	LstUnlinkNode( l, n );	
	ACTOR_MEM_FREE( p );
	return( CONTINUE );
}

int _ActFreeObjectCallback( Node* n, long v1, long* v2 )
{
	EOBJECT* o = (EOBJECT*)n;
	List* l = &(((ACTOR*)v2)->object);

	LstUnlinkNode( l, n );	
	
	ACTOR_MEM_FREE( o );	// deletes object and graphic
	return( CONTINUE );
}

void _ActDestroyActor( ACTOR* victim )
{
		// free all processes for this actor
		LstIterateValue( &(victim->process), _ActFreeProcessCallback, 0, (long*)victim );

		// free all objects for this actor
		LstIterateValue( &(victim->object), _ActFreeObjectCallback, 0, (long*)victim );

		// free the actor & scratch memory

		// 'twould be nice if we could get to a Tree** from the node
		// we're currently working on without having to search the tree.

		TreeDeleteByKey( &actor_tree, victim->tnode.key );		
		actor_tree_count--;
		ACTOR_MEM_FREE( victim );
}

int _ActProcessKillsCallback( Node* n )
{
	_ActDestroyActor( (ACTOR*)(n->key) );
	LstUnlinkNode( actor_kill_ptr, n );
	ACTOR_MEM_FREE( n );
	return( CONTINUE );
}

void ActProcessKills( void )
{
	LstIterate( actor_kill_ptr, _ActProcessKillsCallback );
}

void _ActDefaultMessageHandler( ACTOR* current, MESSAGE_FLAGS message, long data )
{
	Node* actor_kill;

	switch ((int)message)
	{
		case AMESS_DESTROY		:	
		actor_kill = ACTOR_MEM_ALLOC( sizeof (Node) );
		if (actor_kill != NULL)
		{
			actor_kill->key = (long)current;
			LstAddNodeToTail( actor_kill_ptr, actor_kill );
		}
		else
		{
			current->flags |= AFLAG_DESTROY;		// mark for eventual destruction
		}
		break;

		case AMESS_RS_RUN		:	
		case AMESS_RS_WAIT		:
		case AMESS_RS_SLEEP		:
		case AMESS_RS_SUSPEND	:
		current->flags &= ~AFLAG_RS_MASK;
		current->flags |= (message & AFLAG_RS_MASK);
		break;

		case AMESS_CREATE		:
		break;

		case AMESS_COLLIDE_ATT:
		AISparks();
		break;
	
		case AMESS_COLLIDE_DEF:
		current->invuln = 15;
		break;
		
		default:
		break;
	}
}

int _ActBroadcastCallback( Tree** node, long v1, long* v2 )
{
	ACTOR* a = (ACTOR*)(*node);
	
	ActSendMessage( a, v1, (long) v2 );
	return( CONTINUE );
}

void ActSendMessage( ACTOR* receive, MESSAGE_FLAGS mess, long data )
{
	int block = FALSE;

	if (mess & AMESS_BROADCAST)
	{
		assert( receive == NULL );		// required for broadcast

		// resend to each

		TreeIterateValue( &actor_tree, _ActBroadcastCallback, mess & ~AMESS_BROADCAST, (long*)data );
		return;
	}
	assert( receive != NULL );

	switch (receive->flags & AFLAG_RS_MASK)
	{
		case AFLAG_RS_RUN:
		case AFLAG_RS_WAIT:
		block = FALSE;
		break;

		case AFLAG_RS_SLEEP:
		if (mess > AMESS_PRIORITY_2)
		block = TRUE; 
		break;
		
		case AFLAG_RS_SUSPEND:
		if (mess > AMESS_PRIORITY_1)
		block = TRUE; 
		break;

		default:
		assert( TRUE == FALSE );
		break;
	}

	if (!block)
	{
		// if the receiver has a message handler, invoke it
		if (receive->message)
			if ((*receive->message)( receive, mess, data ) == TRUE)
				return;

		// if no message handler, or if handler returns FALSE,
		// invoke the default handler

		_ActDefaultMessageHandler( receive, mess, data );
	}
}

//
// void ActCreateActorTree( void )
//
// call to initialize the balanced tree maintained for actors
//

void ActCreateActorTree( void )
{
	TreeInit( &actor_tree );
	actor_tree_count = 0;
	LstInitList( actor_kill_ptr );
	actor_tree_extant = TRUE;
}

void ActKillActorTree( void )
{
	// if tree exists, kill all actors

	if (actor_tree_extant)
		ActSendMessage( NULL, AMESS_BROADCAST | AMESS_DESTROY, 0 );

	actor_tree_extant = FALSE;
}

// 
// ACTOR* ActCreateActor( 
// 	char* actor_name, 
// 	ACTOR_FLAGS flags, 
// 	short priority, 
// 	void (*message_handler)(),
// 	ACTOR* parent,
// 	int scratch  ) 
// 
// Allocates and initializes an actor with no objects or processes.
// priority is shifted left by 16 bits then incremented until a unique
// ID is created to serve as the index tag in the actor tree.
// scratch is the number of bytes of scratch space to allocate; the 
// actor scratch pointer is set to a buffer of this size.
// Once the actor is created, an AMESS_CREATE message is sent to that
// actor - the message handler should be prepared for this, and will 
// probably create the processes and objects for the actor.
//
// returns pointer to new actor or NULL
//

ACTOR* ActCreateActor( 
	char* actor_name, 
	ACTOR_FLAGS flags, 
	short priority, 
	MESSAGE_HAND message_handler,
	ACTOR* parent,
	int scratch  ) 
{
	Tree** scan;
	ACTOR* new_actor;
	long key;

	assert( message_handler != NULL );

	// upper 16 bits of key is priority; low 16 is individual ID
	key = priority << 16;

	do
	{
		scan = TreeSearch( &actor_tree, key );
		if (scan != NULL)
			key++;
	}
	while (scan != NULL);

	// we have a unique key here

	new_actor = ACTOR_MEM_ALLOC( sizeof( ACTOR )+scratch );
	if (new_actor == NULL)
		return( NULL );

	new_actor->tnode.key = key;
	new_actor->clock = -1;
	new_actor->actor_name = actor_name;
	new_actor->flags = flags;
	LstInitList( &new_actor->process );
	LstInitList( &new_actor->object );
	new_actor->parent = parent;
	new_actor->message = message_handler;
	if (scratch != 0)
		new_actor->scratch.v = ((char*)new_actor)+sizeof( ACTOR );
	else
		new_actor->scratch.v = NULL;

	new_actor->invuln = 0;
	
	TreeAdd( &actor_tree, &(new_actor->tnode) );
	actor_tree_count++;

	ActSendMessage( new_actor, AMESS_CREATE, (long)parent );

	return( new_actor );
}

// 
// PROCESS* ActAddProcess( 
// 	ACTOR* actor, 
// 	void (*function)(), 
// 	int delay, 
// 	int autodelay, 
// 	long priority )		
// 
// adds a process to the actor's process list.
// delay is the number of ticks before it will execute (0-1 = ASAP); 
// autodelay, if not 0, is the number of ticks to recycle 
// the delay with
// priority is the execution priority within the actor's processes
// only
// 
// returns pointer to new process or NULL
//

PROCESS* ActAddProcess( 
	ACTOR* actor, 
	PROCESS_FUNC function,
	int delay, 
	int autodelay, 
	long priority )		
{
	PROCESS* new_process;

	assert( actor != NULL );
	assert( function != NULL );

	new_process = ACTOR_MEM_ALLOC( sizeof( PROCESS ) );
	if (new_process != NULL)
	{
		new_process->lnode.key = priority;
		new_process->delay = delay;
		new_process->autodelay = autodelay;
		new_process->function = function;
		LstAddNodeByKey( &(actor->process), &(new_process->lnode) );
	}
	return( new_process );
}		

//
// EOBJECT* ActAddObject( 
//	ACTOR* actor, 
//	OBJECT_FLAGS flags, 
//	int num_verts, 
//	int num_patches )
//
// adds an object with the specified flags
// if object is graphical according to flags, allocates
// the specified number of vertexes and patches for the
// appropriate graphic model
//
// returns a pointer to new object or NULL
//

EOBJECT* ActAddObject( 
	ACTOR* actor, 
	OBJECT_FLAGS flags, 
	int num_verts, 
	int num_patches )
{
	EOBJECT* new_object;
	int size;
	
	assert( actor != NULL );
	
	switch	(flags & OFLAG_MASK_GRAPHIC)
	{
		case OFLAG_NVGRAPH:
		size =	sizeof( EOBJECT ) + 
				sizeof( NVGRAPH ) + 
				(num_verts * sizeof( NVVERTEX )) +
				(num_patches * sizeof( NVPATCH ));

		new_object = ACTOR_MEM_ALLOC( size );
		if (new_object == NULL)
			return( NULL );
		new_object->graph = (GRAPHIC*)(((char*)new_object) + sizeof( EOBJECT ));
		new_object->graph->nv.type = OBJECT_NV_DEFAULT_TYPE;
		new_object->graph->nv.num_verts = num_verts;
		new_object->graph->nv.num_patches = num_patches;
		new_object->graph->nv.vert_array = (NVVERTEX*)(((char*)new_object) + sizeof( EOBJECT ) + sizeof( NVGRAPH ));
		new_object->graph->nv.patch_array = (NVPATCH*)(((char*)new_object) + sizeof( EOBJECT ) + sizeof( NVGRAPH ) + (num_verts * sizeof( NVVERTEX )));
		new_object->graph->nv.format = OBJECT_NV_DEFAULT_FORMAT;
		break;

		case OFLAG_SATPOLY:
		size =	sizeof( EOBJECT ) + 
				sizeof( SATPOLY ) + 
				(num_verts * sizeof( POINT )) +
				(num_patches * (sizeof( POLYGON )+sizeof( ATTR )));

		new_object = ACTOR_MEM_ALLOC( size );
		if (new_object == NULL)
			return( NULL );
		new_object->graph = (GRAPHIC*)(((char*)new_object) + sizeof( EOBJECT ));
		new_object->graph->satply.pdata.nbPoint = num_verts;
		new_object->graph->satply.pdata.nbPolygon = num_patches;
		new_object->graph->satply.pdata.pntbl = (POINT*)(((char*)new_object) + sizeof( EOBJECT ) + sizeof( SATPOLY ));
		new_object->graph->satply.pdata.pltbl = (POLYGON*)(((char*)new_object->graph->satply.pdata.pntbl) + (num_verts*sizeof(POINT)));
		new_object->graph->satply.pdata.attbl = (ATTR*)(((char*)new_object->graph->satply.pdata.pltbl) + (num_patches*sizeof(POLYGON)));
		new_object->graph->satply.rotorder = act_yzx_order;
		break;

		case OFLAG_SATSPRITE:
		size =	sizeof( EOBJECT ) + 
				sizeof( SATSPRITE );

		new_object = ACTOR_MEM_ALLOC( size );
		if (new_object == NULL)
			return( NULL );
		new_object->graph = (GRAPHIC*)(((char*)new_object) + sizeof( EOBJECT ));
		new_object->graph->satspr.attr = NULL;
		new_object->graph->satspr.scale = toFIXED( 1.0 );
		new_object->graph->satspr.zrot = (ANGLE)0;
		break;

		case OFLAG_SATBACKGR:
		size =	sizeof( EOBJECT ) + 
				sizeof( SATBACKGR );

		new_object = ACTOR_MEM_ALLOC( size );
		if (new_object == NULL)
			return( NULL );
		new_object->graph = (GRAPHIC*)(((char*)new_object) + sizeof( EOBJECT ));
		break;

		case OFLAG_NOGRAPH:
		size = sizeof( EOBJECT );
		new_object = ACTOR_MEM_ALLOC( size );
		if (new_object == NULL)
			return( NULL );
		new_object->graph = NULL;
		break;
	}

	new_object->flags = flags;
	LstAddNodeToHead( &(actor->object), &(new_object->lnode) );
	actor->flags |= AFLAG_OBJECT;
	new_object->radius = toFIXED( 5.0 );	// a reasonable default?
	return( new_object );
}

int _ActUpdateProcessCallback( Node* n, long v1, long* v2 )
{
	PROCESS* p = (PROCESS*)n;
	
	if (p->delay <= 1)
	{
		(*p->function)( (ACTOR*)v2, p );
		if (p->autodelay >= 1)
			p->delay = p->autodelay;
	}
	else
	{
		p->delay--;
	}

	return( CONTINUE );
}

void _ActAutoMove( EOBJECT* obj )
{
	VecAdd( &(obj->pos), &(obj->vel) );
}

void _ActAutoRotate( EOBJECT* obj )
{
	SATSPRITE* ss;
	ss = (SATSPRITE*)(obj->graph);

	VecAAdd( &(obj->rot), &(obj->rotvel) );

	if (obj->flags & OFLAG_SATSPRITE)
		ss->zrot += obj->rotvel.z;
}

void _ActAutoGravity( EOBJECT* obj )
{
	VecAdd( &(obj->vel), &actGravVect );
}

void _ActAutoUpdate( ACTOR* curr )
{
	// if the actor is slated for destruction, try and add it to the kill list
	EOBJECT* obj;

	if (curr->flags & AFLAG_DESTROY)
	{
		ActSendMessage( curr, AMESS_DESTROY, 0 );
	}

	if (curr->invuln != 0)
		curr->invuln--;

	obj = ActGetFirstObject(curr);
	while (obj != NULL)
	{
		if (obj->flags & OFLAG_AUTOMOVE)
			_ActAutoMove( obj );
		if (obj->flags & OFLAG_AUTOROTATE)
			_ActAutoRotate( obj );
		if (obj->flags & OFLAG_GRAVITY)
			_ActAutoGravity( obj );
		obj = ActGetNextObject(curr, obj);
	}
}

int _ActUpdateCallback( Tree** node, long v1, long* v2 )
{
	ACTOR* a = (ACTOR*)(*node);

	a->clock = v1;

	_ActAutoUpdate( a );
	if ((a->flags & AFLAG_RS_MASK) == AFLAG_RS_RUN)
	{
	 	LstIterateValue( &(a->process), _ActUpdateProcessCallback, 0, (long*)a );
		num_actors_executing++;
	}		

	return( CONTINUE );
}

int ActUpdate( long clock )
{
	num_actors_executing = 0;

	TreeIterateValue( &actor_tree, _ActUpdateCallback, clock, NULL );

	ActProcessKills();

	return( num_actors_executing );
}

EOBJECT* ActGetFirstObject( ACTOR* act )
{
	return ((EOBJECT*)LstFirstNode( (&(act->object)) ));
}

EOBJECT* ActGetNextObject( ACTOR* act, EOBJECT* obj )
{
	return ((EOBJECT*)LstNextNode( (&(act->object)), ((Node*)&(obj->lnode)) ));
}

void ProcSetDelay( PROCESS* proc, int delay, int autodelay )
{
	proc->delay = delay;
	proc->autodelay = autodelay;
}

void ProcSetFunction( PROCESS* proc, PROCESS_FUNC function )
{
	proc->function = function;
}

void ProcSetPriority( ACTOR* actor, PROCESS* proc, int priority )
{
	LstChangeKey( &(actor->process), &(proc->lnode), priority );
}
