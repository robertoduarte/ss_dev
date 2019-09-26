//------------------------------------------------------------------------
//
//	AI.C
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

#include "enigma.h"
#include "actor.h"
#include "ai.h"
#include "vector.h"
#include "render.h"
#include "texture.h"
#include "collide.h"
#include "assert.h"

ACTOR* actMaster;
ACTOR* actSaucer;
ACTOR* actCamera;
ACTOR* actControl;
ACTOR* actLand;

#define MASTER_CAMERA ((actMaster->scratch.l)[0])
#define MASTER_SAUCER ((actMaster->scratch.l)[1])
#define CAMERA_TARGET ((actCamera->scratch.l)[0])
#define CAMERA_CLOCK ((actCamera->scratch.l)[1])
#define TILE_AGE  ((act->scratch.l)[0])
#define CONTROL_SAUCER ((actControl->scratch.l)[0])
#define CONTROL_CAMERA ((actControl->scratch.l)[1])

ATTR tile_attr =
        ATTRIBUTE(Dual_Plane, SORT_CEN, TEX_TILE, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip, UseClip);

SPR_ATTR spark_attr =
        SPR_ATTRIBUTE(TEX_SPARK16, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip);

#define SPARK_AGE ((act->scratch.l)[0])

void AISparkProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;
    EOBJECT* dad;
    SATSPRITE* ss;

    obj = ActGetFirstObject(act);
    ss = (SATSPRITE*) (obj->graph);

    SPARK_AGE++;
    if (SPARK_AGE > 30)
        ActSendMessage(act, ASYNC_MESS_DESTROY, 0);

    ss->zrot += obj->rotvel.z;
}

void AISparkBuild(ACTOR* act)
{
    EOBJECT* obj;
    EOBJECT* dad;
    SATSPRITE* ss;

    obj = ActAddObject(act, OFLAG_SATSPRITE | OFLAG_AUTOMOVE | OFLAG_GRAVITY, 0, 0);
    ss = (SATSPRITE*) (obj->graph);
    ss->attr = &spark_attr;
    ss->scale = toFIXED(4.0);
    ss->zrot = (ANGLE) slRandom();

    obj->pos.x = collide_loc.x;
    obj->pos.y = collide_loc.y;
    obj->pos.z = collide_loc.z;

    obj->vel.x = FRANDOM(40.0) - toFIXED(20.0);
    obj->vel.y = FRANDOM(40.0) - toFIXED(20.0);
    obj->vel.z = FRANDOM(40.0) - toFIXED(20.0);

    SPARK_AGE = 0;
}

int AISparkMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        AISparkBuild(act);
        ActAddProcess(act, AISparkProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

void AISparks(void)
{
    int i;

    for (i = 0; i < 4; i++)
        AICreateActor(AT_KEY_SPARK, NULL);
}
//------------------------------------------------------

void AITileProcess2(ACTOR* act, PROCESS* proc)
{
    TILE_AGE++;
    if (TILE_AGE > 60)
        ActSendMessage(act, ASYNC_MESS_DESTROY, 0);
}

void AITileProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;
    EOBJECT* sobj;
    SATPOLY* sg;
    PDATA* pdata;
    POLYGON* pg;
    POINT* pt;
    ATTR* at;

    int x, y, v;
    FIXED fx, fy;

    obj = ActGetFirstObject(act);

    obj->pos.x = FRANDOM(1000.0) - toFIXED(500.0);
    obj->pos.y = FRANDOM(1000.0) - toFIXED(500.0);
    obj->pos.z = FRANDOM(1000.0) - toFIXED(500.0);

    if (actSaucer != NULL)
    {
        sobj = ActGetFirstObject(actSaucer);
        if (sobj != NULL)
        {
            obj->pos.x += sobj->pos.x;
            obj->pos.y += sobj->pos.y;
            obj->pos.z += sobj->pos.z;
        }
    }

    obj->rot.x = (ANGLE) slRandom();
    obj->rot.y = (ANGLE) slRandom();
    obj->rot.z = (ANGLE) slRandom();
    obj->rotvel.x = ((ANGLE) slRandom()) >> 5;
    obj->rotvel.y = ((ANGLE) slRandom()) >> 5;
    obj->rotvel.z = ((ANGLE) slRandom()) >> 5;
    obj->vel.x = toFIXED(0.0);
    obj->vel.y = toFIXED(0.0);
    obj->vel.z = toFIXED(0.0);
    TILE_AGE = 0;

    sg = (SATPOLY*) (obj->graph);
    pdata = &(sg->pdata);
    pg = pdata->pltbl;
    pt = pdata->pntbl;
    at = pdata->attbl;

    pt[0][X] = toFIXED(-50.0);
    pt[0][Y] = toFIXED(-50.0);
    pt[0][Z] = toFIXED(0.0);

    pt[1][X] = toFIXED(50.0);
    pt[1][Y] = toFIXED(-50.0);
    pt[1][Z] = toFIXED(0.0);

    pt[2][X] = toFIXED(50.0);
    pt[2][Y] = toFIXED(50.0);
    pt[2][Z] = toFIXED(0.0);

    pt[3][X] = toFIXED(-50.0);
    pt[3][Y] = toFIXED(50.0);
    pt[3][Z] = toFIXED(0.0);

    pg[0].norm[X] = toFIXED(0.0);
    pg[0].norm[Y] = toFIXED(0.0);
    pg[0].norm[Z] = toFIXED(-1.0);
    pg[0].Vertices[0] = 0;
    pg[0].Vertices[1] = 1;
    pg[0].Vertices[2] = 2;
    pg[0].Vertices[3] = 3;

    at[0] = tile_attr;

    ProcSetFunction(proc, AITileProcess2);
}

int AITileMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        ActAddObject(act, OFLAG_SATPOLY | OFLAG_AUTOMOVE | OFLAG_AUTOROTATE, 4, 1);
        ActAddProcess(act, AITileProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);

    }
}

//------------------------------------------------------

void AICameraProcess2(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;

    obj = ActGetFirstObject(act);

    obj->pos.x = (rendCameraTarget->pos.x >> 1);
    obj->pos.y = (rendCameraTarget->pos.y >> 1) + toFIXED(200.0);
    obj->pos.z = (rendCameraTarget->pos.z >> 1) - toFIXED(1000.0);
}

void AICameraProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;

    obj = ActGetFirstObject(act);
    obj->pos.x = toFIXED(0.0);
    obj->pos.y = toFIXED(200.0);
    obj->pos.z = toFIXED(-1000.0);
    obj->rot.x = obj->rot.y = obj->rot.z = (ANGLE) 0;
    obj->rotvel.x = obj->rotvel.y = obj->rotvel.z = (ANGLE) 0;
    obj->vel.x = obj->vel.y = obj->vel.z = toFIXED(0.0);

    rendCamera = obj;
    rendCameraTarget = ActGetFirstObject((ACTOR*) CAMERA_TARGET);

    ProcSetFunction(proc, AICameraProcess2);
}

int AICameraMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        ActAddObject(act, OFLAG_NOGRAPH, 0, 0);
        ActAddProcess(act, AICameraProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

//------------------------------------------------------

extern int AISaucerProcess1(ACTOR* act, PROCESS* proc);
extern int AISaucerMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AILandMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AIControlMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AIShotMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AISparkMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AIBlipMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AIPointerMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);
extern int AITargetMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data);

//------------------------------------------------------

void AIMasterProcess1(ACTOR* act, PROCESS* proc)
{
    AICreateActor(AT_KEY_TILE, act);
}

int AIMasterMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        actSaucer = AICreateActor(AT_KEY_SAUCER, act);
        actCamera = AICreateActor(AT_KEY_CAMERA, act);
        actLand = AICreateActor(AT_KEY_LAND, act);
        CAMERA_TARGET = (long) actSaucer;
        actControl = AICreateActor(AT_KEY_CONTROL, act);
        AICreateActor(AT_KEY_POINTER, actSaucer);
        CONTROL_CAMERA = (long) actCamera;
        CONTROL_SAUCER = (long) actSaucer;
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        AICreateActor(AT_KEY_TARGET, act);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

//------------------------------------------------------

void AIDemoInit(void)
{
    actMaster = AICreateActor(AT_KEY_MASTER, NULL);
}

ACTOR_TEMPLATE aiTemplates[] ={
    { AT_KEY_MASTER, "Master", AFLAG_RS_RUN, 10, AIMasterMsgHand, 8},
    { AT_KEY_SAUCER, "Saucer", AFLAG_RS_RUN, 20, AISaucerMsgHand, 32},
    { AT_KEY_CAMERA, "Camera", AFLAG_RS_RUN, 30, AICameraMsgHand, 8},
    { AT_KEY_TILE, "Tile", AFLAG_RS_RUN, 40, AITileMsgHand, 8},
    { AT_KEY_LAND, "Land", AFLAG_RS_RUN, 99, AILandMsgHand, 8},
    { AT_KEY_CONTROL, "Control", AFLAG_RS_RUN, 5, AIControlMsgHand, 8},
    { AT_KEY_SHOT, "Shot", AFLAG_RS_RUN, 25, AIShotMsgHand, 8},
    { AT_KEY_BLIP, "Blip", AFLAG_RS_RUN, 50, AIBlipMsgHand, 8},
    { AT_KEY_POINTER, "Pointer", AFLAG_RS_RUN, 27, AIPointerMsgHand, 8},
    { AT_KEY_SPARK, "Spark", AFLAG_RS_RUN, 27, AISparkMsgHand, 8},
    { AT_KEY_TARGET, "Target", AFLAG_RS_RUN, 27, AITargetMsgHand, 8},
    { AT_KEY_NULL, "(null)", -1, -1, NULL, 0},
};

ACTOR* AICreateActor(int key, ACTOR* parent)
{
    ACTOR_TEMPLATE* at = aiTemplates;

    while (at->key != AT_KEY_NULL)
    {
        if (at->key == key)
        {
            return ( ActCreateActor(at->name, at->flags, at->pri, at->mh, parent, at->scr));
        }
        at++;
    }
    return ( NULL);
}

