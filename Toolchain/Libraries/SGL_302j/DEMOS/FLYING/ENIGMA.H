//------------------------------------------------------------------------
//
//	ENIGMA.H
//	Enigma engine stuff
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

#ifndef _ENIGMA_H_
#define _ENIGMA_H_

//------------------- PRE-INCLUDE ENVIRONMENT SETTINGS -------------------

#include "master.h"
#include "sgl.h"

//------------------------------- INCLUDES -------------------------------

#include "tree.h"
#include "list.h"

//-------------------------- MACROS & TYPEDEFS ---------------------------

#define RANDOM( range )		((slRandom() * (range)) >> 16)
#define FRANDOM( range )	(slMulFX( slRandom(), toFIXED( (range) ) ))

#define SWAP( a,b )			{ a ^= b; b ^= a; a ^= b; }

enum 
{
	AMESS_NULL			= 0x00000000,
	AMESS_CREATE		= 0x00000001,
	AMESS_DESTROY		= 0x00000002,
	AMESS_SET_RUNSTATE  = 0x00000004,
	AMESS_RS_RUN		= 0x00000004,
	AMESS_RS_WAIT		= 0x00000005,
	AMESS_RS_SLEEP		= 0x00000006,
	AMESS_RS_SUSPEND	= 0x00000007,

	AMESS_PRIORITY_1	= 0x000000FF, // above this, all actors receive

	AMESS_COLLIDE_ATT	= 0x00000100, 
	AMESS_COLLIDE_DEF	= 0x00000101, 

	AMESS_PRIORITY_2	= 0x0000FFFF, // above this, unsuspended actors
									  // below this, running/waiting actors

	AMESS_PRIVATE		= 0x01000000, // private message range
	AMESS_ENDPRIVATE	= 0x01FFFFFF, 
	AMESS_BROADCAST		= 0x80000000,

	
};

enum
{
	AFLAG_RS_RUN		= 0x00000000,
	AFLAG_RS_WAIT		= 0x00000001,
	AFLAG_RS_SLEEP		= 0x00000002,
	AFLAG_RS_SUSPEND	= 0x00000003,
	AFLAG_RS_MASK		= 0x00000003,
	AFLAG_OBJECT		= 0x00000004,

	AFLAG_SIDE_GRAY		= 0x00000000,
	AFLAG_SIDE_DOPP		= 0x00000008,
	AFLAG_SIDE_MIL		= 0x00000010,
	AFLAG_SIDE_CIV		= 0x00000018,

	AFLAG_COLLIDE_ATT	= 0x00000020,	// Things collide with me
	AFLAG_COLLIDE_DEF	= 0x00000040,	// I collide with things
	AFLAG_PROJECT_ATT	= 0x00000080,	// I'm a bullet
	AFLAG_PROJECT_DEF	= 0x00000100,	// Bullets hit me
	AFLAG_ENERGY_ATT	= 0x00000200,	// I'm a zap beam
	AFLAG_ENERGY_DEF	= 0x00000400,	// Zap beams hit me
	AFLAG_SPECIAL_ATT	= 0x00000800,	// I'm another attack form
	AFLAG_SPECIAL_DEF	= 0x00001000,	// Other attack forms hit me

	AFLAG_ATTACK_MASK	= 
		AFLAG_COLLIDE_ATT|AFLAG_PROJECT_ATT|AFLAG_ENERGY_ATT|AFLAG_SPECIAL_ATT,	// OR of all attack bits
	AFLAG_DEFEND_MASK	= 
		AFLAG_COLLIDE_DEF|AFLAG_PROJECT_DEF|AFLAG_ENERGY_DEF|AFLAG_SPECIAL_DEF,	// OR of all defense bits
	AFLAG_COLLIDE_MASK	= AFLAG_ATTACK_MASK|AFLAG_DEFEND_MASK,	// OR of all collide/attack bits

	AFLAG_DESTROY		= 0x80000000,
};

enum 
{
	OFLAG_NOGRAPH		= 0x00000000,		// object has no graphic
	OFLAG_NVGRAPH		= 0x00000001,		// object has NVidia graphic
	OFLAG_PCGRAPH		= 0x00000002,		// object has PC graphic (TBD)
	OFLAG_SATPOLY		= 0x00000004,		// object has SGL polygon graphic
	OFLAG_SATSPRITE		= 0x00000008,		// object has SGL sprite graphic
	OFLAG_SATBACKGR		= 0x00000010,		// object has SGL bg-plane graphic
	OFLAG_MASK_GRAPHIC	= 0x0000001F,		// mask of all graphic flags
	OFLAG_AUTOMOVE		= 0x00000020,		// pos = pos + vel every cycle
	OFLAG_AUTOROTATE	= 0x00000040,		// rot = rot + rotvel every cycle
	OFLAG_HUGE			= 0x00000080,		// graphic is too big to cull by center
	OFLAG_NOBACKFACE	= 0x00000100,		// graphic has no back faces
	OFLAG_GRAVITY		= 0x00000200,		// object is affected by standard gravity

	OFLAG_ATTACKER		= 0x00000400,		// this object hits things (subject to aflags)
	OFLAG_DEFENDER		= 0x00000800,		// things hit this object (subject to aflags)

	OFLAG_TRYMOVE		= 0x00001000,		// pos = trypos if no collide at trypos
	OFLAG_AUTOTRYMOVE	= 0x00002000,		// trypos = pos + vel every cycle

	OFLAG_CULLED		= 0x80000000,		// object will not be rendered this frame
};

enum
{
	ONV_TYPE_QTM		= 0x0001,
	ONV_TYPE_QTM_A		= 0x0002,
	ONV_TYPE_BTM		= 0x0003,
	ONV_TYPE_BTM_A		= 0x0004,
	ONV_RENDER_MASK		= 0x00FF,
	ONV_PROJECT_MASK	= 0xFF00,
	ONV_TYPE_SPRITE		= 0x8000,
};

#define OBJECT_NV_DEFAULT_TYPE		ONV_TYPE_QTM
#define OBJECT_NV_DEFAULT_FORMAT	1

typedef unsigned long ACTOR_FLAGS;
typedef unsigned long MESSAGE_FLAGS;
typedef unsigned long OBJECT_FLAGS;

struct _actor_;
struct _process_;

typedef int (*MESSAGE_HAND)( struct _actor_* current, MESSAGE_FLAGS message, long data );
typedef void (*PROCESS_FUNC)( struct _actor_* current_actor, struct _process_* current_process );		

typedef struct _process_
{
	Node	   		lnode;
	int		   		delay;
	int		   		autodelay;
	PROCESS_FUNC	function;
} PROCESS;

typedef struct _vector2i_
{
	int				x;		
	int				y;		
} VECTOR2I;

typedef struct _vector2f_
{
	FIXED			x;		
	FIXED			y;		
} VECTOR2F;

typedef struct _vector3i_
{
	int				x;		
	int				y;		
	int				z;  	
} VECTOR3I;

typedef struct _vector3f_
{
	FIXED			x;		
	FIXED			y;		
	FIXED			z;
} VECTOR3F;

typedef struct _vector3a_
{
	ANGLE			x;		
	ANGLE			y;		
	ANGLE			z;
} VECTOR3A;

typedef struct _nvvertex_
{
	VECTOR3F		world;	// pre xform
	VECTOR2I		screen;	// post projection
	int				sortz;	// post transform z
	short			beta;	
} NVVERTEX;					// vertex or qtm control point

typedef struct _nvtexture_
{
	void*			tex;		// texels or color
	char			width;		// tex dimensions
	char			height;		
	int				blend;		// beta blend info
} NVTEXTURE;

typedef struct _nvpatch_
{
	NVTEXTURE		texture;	// texture data
	short			vert[9];		// up to 9 vertices/control points
} NVPATCH;

typedef struct _nvdisplay_
{
	Tree			tnode;		// display tree node
	short			type;		// object type
	NVVERTEX*		vert_array;	// vertex context
	NVPATCH*		patch;		// patch data
} NVDISPLAY;

typedef struct _nvgraph_
{
	short			type;			// one patch type per graph object - is OK?
	short			num_verts;
	short			num_patches;
	NVVERTEX*		vert_array;
	NVPATCH*		patch_array;
	long			format;			// color format of color or texture
} NVGRAPH;

typedef struct _pcgraph_
{
	Tree			tnode;			// graphic is node in graphic tree
} PCGRAPH;

typedef struct _satpoly_
{
	Tree			tnode;			// graphic is node in graphic tree
	PDATA			pdata;
	char*			rotorder;		// rotation order string
} SATPOLY;

typedef struct _satsprite_
{
	Tree			tnode;			// graphic is node in graphic tree
	SPR_ATTR*		attr;			// sprite attributes
	FIXED			scale;			// sprite scale
	ANGLE			zrot;			// sprite z rotation
} SATSPRITE;

typedef struct _satbackgr_
{
	Tree			tnode;			// graphic is node in graphic tree
		
} SATBACKGR;

typedef union _graphic_
{
	NVGRAPH			nv;
	PCGRAPH			pc;
	SATPOLY			satply;
	SATSPRITE		satspr;
	SATBACKGR		satbgr;
} GRAPHIC;

typedef struct _object_
{
	Node			lnode;
	OBJECT_FLAGS	flags;
	VECTOR3F		pos;
	VECTOR3F		try_pos;
	VECTOR3A		rot;
	VECTOR3F		vel;
	VECTOR3A		rotvel;
	GRAPHIC*		graph;
	FIXED			radius;			// for collision purposes
} EOBJECT;

typedef union _actor_scratch_
{
	void*			v;
	char*			c;
	short*			s;
	long*			l;
	FIXED*			f;
	VECTOR3F*		v3;
	// other pointer types go here
} ACTOR_SCRATCH;

typedef struct _actor_
{
	Tree			tnode;		// actor is node in "cast" tree
	long			clock;		// last tick processed
	char*			actor_name;
	ACTOR_FLAGS		flags;
	List			process;	// list of PROCESS type
	List			object;		// list of EOBJECT type
	struct _actor_*	parent;		// parent actor if hierarchical - not tree parent!
	MESSAGE_HAND	message;
	ACTOR_SCRATCH	scratch;
	int				invuln;
} ACTOR;

//--------------------------- GLOBAL VARIABLES ---------------------------

extern Tree*		actor_tree;
extern Tree*		graphic_tree;
	
//--------------------------- GLOBAL FUNCTIONS ---------------------------

#endif
