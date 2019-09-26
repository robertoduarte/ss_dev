//------------------------------------------------------------------------
//
//	AI.H
//	Actor "intelligence"
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

extern void AIDemoInit( void );

extern ACTOR* actMaster;
extern ACTOR* actSaucer;
extern ACTOR* actCamera;
extern ACTOR* actControl;
extern ACTOR* actLand;
extern VECTOR3F ai_blip_offset;

typedef struct _actor_template_
{
	int				key;
	char*			name;
	ACTOR_FLAGS		flags;
	short			pri;
	MESSAGE_HAND	mh;
	int				scr;
} ACTOR_TEMPLATE;

enum
{
	AT_KEY_NULL = 0,
	AT_KEY_MASTER,		
	AT_KEY_SAUCER, 
	AT_KEY_CAMERA, 
	AT_KEY_TILE, 
	AT_KEY_CONTROL, 
	AT_KEY_LAND, 
	AT_KEY_SHOT, 
	AT_KEY_POINTER, 
	AT_KEY_BLIP, 
	AT_KEY_SPARK, 
	AT_KEY_TARGET, 
};

extern ACTOR* AICreateActor( int key, ACTOR* parent );
void AISparks( void );
