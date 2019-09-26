//------------------------------------------------------------------------
//
//	AISAUCER.C
//	Flying saucer actor
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

ATTR nose_attr =
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(0, 31, 0), No_Gouraud, MESHoff, sprPolygon, UseClip);
ATTR vect_attr =
        ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(31, 0, 0), No_Gouraud, MESHoff, sprPolygon, UseClip);

VECTOR3F dummy = {toFIXED(0.0), toFIXED(0.0), toFIXED(0.0)};

void AIPointerProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* sobj;
    EOBJECT* obj = ActGetFirstObject(act);
    VECTOR3F* nose = &dummy;
    VECTOR3F* vect = &dummy;
    SATPOLY* sg;
    PDATA* pdata;
    POINT* pt;

    if (actSaucer != NULL)
    {
        sobj = ActGetFirstObject(actSaucer);
        if (sobj != NULL)
        {
            obj->pos.x = sobj->pos.x;
            obj->pos.y = sobj->pos.y;
            obj->pos.z = sobj->pos.z;

            vect = &(sobj->vel);
        }
        nose = ((VECTOR3F*) ((actSaucer->scratch.f) + 2));
    }

    sg = (SATPOLY*) (obj->graph);
    pdata = &(sg->pdata);
    pt = pdata->pntbl;

    pt[1][X] = pt[2][X] = slMulFX(nose->x, toFIXED(200.0));
    pt[1][Y] = pt[2][Y] = slMulFX(nose->y, toFIXED(200.0));
    pt[1][Z] = pt[2][Z] = slMulFX(nose->z, toFIXED(200.0));

    pt[4][X] = pt[5][X] = slMulFX(vect->x, toFIXED(20.0));
    pt[4][Y] = pt[5][Y] = slMulFX(vect->y, toFIXED(20.0));
    pt[4][Z] = pt[5][Z] = slMulFX(vect->z, toFIXED(20.0));
}

void AIPointerBuild(ACTOR* act)
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

    obj = ActAddObject(act, OFLAG_SATPOLY, 6, 2);

    obj->pos.x = toFIXED(0.0);
    obj->pos.y = toFIXED(0.0);
    obj->pos.z = toFIXED(0.0);
    obj->rot.x = 0;
    obj->rot.y = 0;
    obj->rot.z = 0;
    obj->rotvel.x = (ANGLE) 0;
    obj->rotvel.y = (ANGLE) 0;
    obj->rotvel.z = (ANGLE) 0;
    obj->vel.x = toFIXED(0.0);
    obj->vel.y = toFIXED(0.0);
    obj->vel.z = toFIXED(0.0);

    sg = (SATPOLY*) (obj->graph);
    pdata = &(sg->pdata);
    pg = pdata->pltbl;
    pt = pdata->pntbl;
    at = pdata->attbl;

    pt[0][X] = toFIXED(0.0);
    pt[0][Y] = toFIXED(20.0);
    pt[0][Z] = toFIXED(0.0);

    pt[1][X] = pt[2][X] = pt[4][X] = pt[5][X] = toFIXED(200.0);
    pt[1][Y] = pt[2][Y] = pt[4][Y] = pt[5][Y] = toFIXED(0.0);
    pt[1][Z] = pt[2][Z] = pt[4][Z] = pt[5][Z] = toFIXED(0.0);

    pt[3][X] = toFIXED(0.0);
    pt[3][Y] = toFIXED(-20.0);
    pt[3][Z] = toFIXED(0.0);

    pg[0].norm[X] = toFIXED(0.0);
    pg[0].norm[Y] = toFIXED(0.0);
    pg[0].norm[Z] = toFIXED(-1.0);
    pg[0].Vertices[0] = 0;
    pg[0].Vertices[1] = 1;
    pg[0].Vertices[2] = 2;
    pg[0].Vertices[3] = 3;

    pg[1].norm[X] = toFIXED(0.0);
    pg[1].norm[Y] = toFIXED(0.0);
    pg[1].norm[Z] = toFIXED(-1.0);
    pg[1].Vertices[0] = 0;
    pg[1].Vertices[1] = 4;
    pg[1].Vertices[2] = 5;
    pg[1].Vertices[3] = 3;

    at[0] = nose_attr;
    at[1] = vect_attr;
}

int AIPointerMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        AIPointerBuild(act);
        ActAddProcess(act, AIPointerProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

VECTOR3F ai_blip_offset;

SPR_ATTR blip_attr =
        SPR_ATTRIBUTE(TEX_BLIP, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip);
#define BLIP_AGE ((act->scratch.l)[0])

void AIBlipProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;
    EOBJECT* dad;
    SATSPRITE* ss;

    obj = ActGetFirstObject(act);
    ss = (SATSPRITE*) (obj->graph);

    BLIP_AGE++;
    if (BLIP_AGE > 30)
        ActSendMessage(act, ASYNC_MESS_DESTROY, 0);

    ss->zrot += obj->rotvel.z;
}

void AIBlipBuild(ACTOR* act)
{
    EOBJECT* obj;
    EOBJECT* dad;
    SATSPRITE* ss;

    dad = ActGetFirstObject(act->parent);
    obj = ActAddObject(act, OFLAG_SATSPRITE | OFLAG_AUTOMOVE, 0, 0);
    ss = (SATSPRITE*) (obj->graph);
    ss->attr = &blip_attr;
    ss->scale = toFIXED(2.0);
    ss->zrot = (ANGLE) slRandom();

    obj->pos.x = dad->pos.x + ai_blip_offset.x;
    obj->pos.y = dad->pos.y + ai_blip_offset.y;
    obj->pos.z = dad->pos.z + ai_blip_offset.z;

    obj->vel.x = FRANDOM(8.0) - toFIXED(4.0);
    obj->vel.y = FRANDOM(8.0) - toFIXED(4.0);
    obj->vel.z = FRANDOM(8.0) - toFIXED(4.0);

    obj->rotvel.z = (ANGLE) ((slRandom() - toFIXED(0.5)) >> 3);

    BLIP_AGE = 0;
}

int AIBlipMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        AIBlipBuild(act);
        ActAddProcess(act, AIBlipProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

SPR_ATTR shot_attr =
        SPR_ATTRIBUTE(TEX_BALL, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip);

#define SHOT_AGE ((act->scratch.l)[0])

void AIShotProcess1(ACTOR* act, PROCESS* proc)
{
    EOBJECT* obj;
    SATSPRITE* ss;

    obj = ActGetFirstObject(act);
    ss = (SATSPRITE*) (obj->graph);

    SHOT_AGE++;
    if (SHOT_AGE > 200)
        ActSendMessage(act, ASYNC_MESS_DESTROY, 0);

    ss->zrot += (ANGLE) 20;

}

void AIShotBuild(ACTOR* act)
{
    EOBJECT* obj;
    EOBJECT* dad;
    SATSPRITE* ss;

    dad = ActGetFirstObject(act->parent);
    obj = ActAddObject(act, OFLAG_SATSPRITE | OFLAG_AUTOMOVE, 0, 0);
    ss = (SATSPRITE*) (obj->graph);
    ss->attr = &shot_attr;
    ss->scale = toFIXED(3.0);
    ss->zrot = (ANGLE) 0;

    obj->pos.x = dad->pos.x;
    obj->pos.y = dad->pos.y;
    obj->pos.z = dad->pos.z;

    obj->vel.x = dad->vel.x + slMulFX(((VECTOR3F*) ((act->parent->scratch.f) + 2))->x, toFIXED(25.0));
    obj->vel.y = dad->vel.y + slMulFX(((VECTOR3F*) ((act->parent->scratch.f) + 2))->y, toFIXED(25.0));
    obj->vel.z = dad->vel.z + slMulFX(((VECTOR3F*) ((act->parent->scratch.f) + 2))->z, toFIXED(25.0));

    // collision features...

    act->flags |= AFLAG_ATTACK_MASK; // muchly hitful
    obj->flags |= OFLAG_ATTACKER;
    obj->radius = toFIXED(12.0); // fairly random
    SHOT_AGE = 0;
}

int AIShotMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        AIShotBuild(act);
        ActAddProcess(act, AIShotProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    case ASYNC_MESS_COLLIDE_ATT:
        ActSendMessage(act, ASYNC_MESS_DESTROY, 0); // kill self
        return ( FALSE); // do default processing as well

    default:
        return ( FALSE);
    }
}

#define SAUCER_ANTIGRAV ((act->scratch.f)[0])
#define SAUCER_THROTTLE ((act->scratch.f)[1])
#define SAUCER_FACING  ((VECTOR3F*)((act->scratch.f)+2))

ATTR saucer_attr =
        ATTRIBUTE(Single_Plane, SORT_CEN, TEX_SAUCER, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip, UseClip);

ATTR xsaucer_attr =
        ATTRIBUTE(Single_Plane, SORT_CEN, TEX_SAUCER, No_Palet, No_Gouraud, CL32KRGB | MESHoff, sprNoflip, UseClip);

void AISaucerBlips(ACTOR* act)
{
    int i;

    if (slRandom() > toFIXED(0.7))
        AICreateActor(AT_KEY_BLIP, act);
}

void AISaucerShoot(ACTOR* act)
{
    ACTOR* shot;

    shot = AICreateActor(AT_KEY_SHOT, act);
}

void AISaucerRestart(ACTOR* act)
{
    EOBJECT* obj = ActGetFirstObject(act);

    obj->pos.x = obj->pos.y = obj->pos.z = toFIXED(0.0);
    obj->vel.x = obj->vel.y = obj->vel.z = toFIXED(0.0);
    obj->rot.x = (ANGLE) 0;
    obj->rot.y = (ANGLE) 0;
    obj->rot.z = (ANGLE) 0;
    obj->rotvel.x = (ANGLE) 0;
    obj->rotvel.y = (ANGLE) 199;
    obj->rotvel.z = (ANGLE) 0;

    SAUCER_ANTIGRAV = toFIXED(20.0);
    SAUCER_THROTTLE = toFIXED(0.0);

    SAUCER_FACING->x = toFIXED(0.0);
    SAUCER_FACING->y = toFIXED(0.0);
    SAUCER_FACING->z = toFIXED(1.0);
}

void AISaucerAntiGrav(ACTOR* act, FIXED v)
{
    SAUCER_ANTIGRAV += slMulFX(v, toFIXED(1.0));

    if (SAUCER_ANTIGRAV < toFIXED(5.0))
        SAUCER_ANTIGRAV = toFIXED(5.0);

    if (SAUCER_ANTIGRAV > toFIXED(40.0))
        SAUCER_ANTIGRAV = toFIXED(40.0);

    ai_blip_offset.x = toFIXED(0.0);
    ai_blip_offset.y = -slMulFX(v, toFIXED(30.0));
    ai_blip_offset.z = toFIXED(0.0);

    //	AISaucerBlips( act );
}

void AISaucerForward(ACTOR* act, FIXED thrust)
{
    SAUCER_THROTTLE += thrust;

    if (SAUCER_THROTTLE > toFIXED(5.0))
        SAUCER_THROTTLE = toFIXED(5.0);

    if (SAUCER_THROTTLE < toFIXED(-3.0))
        SAUCER_THROTTLE = toFIXED(-3.0);

    ai_blip_offset.x = -slMulFX(SAUCER_FACING->x, toFIXED(30.0));
    ai_blip_offset.y = -slMulFX(SAUCER_FACING->y, toFIXED(30.0));
    ai_blip_offset.z = -slMulFX(SAUCER_FACING->z, toFIXED(30.0));

    //	AISaucerBlips( act );
}

void AISaucerSlip(ACTOR* act, FIXED thrust)
{
    VECTOR3F up, right;
    EOBJECT* obj = ActGetFirstObject(act);

    up.x = toFIXED(0.0);
    up.y = toFIXED(1.0);
    up.z = toFIXED(0.0);
    VecCross(&right, SAUCER_FACING, &up);
    VecSetMag(&right, thrust);

    ai_blip_offset.x = -slMulFX(right.x, toFIXED(30.0));
    ai_blip_offset.y = -slMulFX(right.y, toFIXED(30.0));
    ai_blip_offset.z = -slMulFX(right.z, toFIXED(30.0));

    //	AISaucerBlips( act );

    obj->vel.x += right.x;
    assert(right.y == toFIXED(0.0));
    obj->vel.z += right.z;

    if (thrust > toFIXED(0.0))
    {
        if ((obj->rot.x > 0xE000u) || (obj->rot.x < 0x2000u))
            obj->rot.x += SAUCER_FACING->x >> 8;
        if ((obj->rot.z > 0xE000u) || (obj->rot.z < 0x2000u))
            obj->rot.z += SAUCER_FACING->z >> 8;
    }
    else
    {
        if ((obj->rot.x > 0xE000u) || (obj->rot.x < 0x2000u))
            obj->rot.x -= SAUCER_FACING->x >> 8;
        if ((obj->rot.z > 0xE000u) || (obj->rot.z < 0x2000u))
            obj->rot.z -= SAUCER_FACING->z >> 8;
    }
}

void AISaucerTurn(ACTOR* act, FIXED thrust)
{
    ANGLE a;

    AISaucerSlip(act, thrust);
    a = VecAzimuth(SAUCER_FACING);
    if (thrust > toFIXED(0.0))
        a += DEGtoANG(360.0 / 64.0);
    else
        a -= DEGtoANG(360.0 / 64.0);

    SAUCER_FACING->x = slCos(a);
    SAUCER_FACING->z = slSin(a);
    VecSetMag(SAUCER_FACING, toFIXED(1.0));
}

void AISaucerAccelerate(ACTOR* act, FIXED x, FIXED y, FIXED z)
{
    VECTOR3F a;
    EOBJECT* obj = ActGetFirstObject(act);

    a.x = x;
    a.y = y;
    a.z = z;

    VecAdd(&(obj->vel), &a);
}

#define PATCH_LAYERS 6
#define VERTEX_LAYERS (PATCH_LAYERS+1)
#define PATCH_SECTORS 16
#define VERTEX_SECTORS (PATCH_SECTORS)

#define SAUCER_VERTS (VERTEX_LAYERS*VERTEX_SECTORS)
#define SAUCER_PATCHES (PATCH_LAYERS*PATCH_SECTORS)

int vy_coords[ VERTEX_LAYERS ] ={
    45, 36, 27, 0, -18, -36, -40
};

int vx_coords[ VERTEX_LAYERS ] ={
    1, 27, 40, 114, 40, 27, 1
};

void AISaucerBuild(ACTOR* act)
{
    EOBJECT* obj;
    int patch, sector, layer, vert, v0, v1, v2, v3;
    FIXED sint, cost;
    ANGLE theta;

    SATPOLY* sg;
    PDATA* pdata;
    POLYGON* pg;
    POINT* pt;
    ATTR* at;
    VECTOR3F edge1, edge2, norm;

    obj = ActAddObject(act, OFLAG_SATPOLY | OFLAG_AUTOMOVE | OFLAG_AUTOROTATE | OFLAG_GRAVITY, SAUCER_VERTS, SAUCER_PATCHES);

    sg = (SATPOLY*) (obj->graph);
    sg->rotorder = act_xzy_order;
    pdata = &(sg->pdata);
    pg = pdata->pltbl;
    pt = pdata->pntbl;
    at = pdata->attbl;

    // create vertices

    for (vert = 0, sector = 0; sector < VERTEX_SECTORS; sector++)
    {
        for (layer = 0; layer < VERTEX_LAYERS; layer++, vert++)
        {
            theta = (ANGLE) ((65536 * sector) / VERTEX_SECTORS);
            sint = slSin(theta);
            cost = slCos(theta);
            pt[ vert ][X] = slMulFX(toFIXED(vx_coords[ layer ]), sint);
            pt[ vert ][Y] = toFIXED(vy_coords[ layer ]);
            pt[ vert ][Z] = slMulFX(toFIXED(vx_coords[ layer ]), cost);
        }
    }

    // create patches

    for (sector = 0, patch = 0; sector < PATCH_SECTORS; sector++)
    {
        for (layer = 0; layer < PATCH_LAYERS; layer++, patch++)
        {
            v0 = pg[patch].Vertices[0] = (sector * VERTEX_LAYERS) + layer;
            v1 = pg[patch].Vertices[1] = (((sector + 1) % PATCH_SECTORS) * VERTEX_LAYERS) + layer;
            v2 = pg[patch].Vertices[2] = (((sector + 1) % PATCH_SECTORS) * VERTEX_LAYERS) + layer + 1;
            v3 = pg[patch].Vertices[3] = (sector * VERTEX_LAYERS) + layer + 1;

            edge1.x = pt[v0][X] - pt[v1][X];
            edge1.y = pt[v0][Y] - pt[v1][Y];
            edge1.z = pt[v0][Z] - pt[v1][Z];

            edge2.x = pt[v3][X] - pt[v0][X];
            edge2.y = pt[v3][Y] - pt[v0][Y];
            edge2.z = pt[v3][Z] - pt[v0][Z];

            VecCross(&norm, &edge1, &edge2);
            VecNormalize(&norm);

            pg[patch].norm[X] = norm.x;
            pg[patch].norm[Y] = norm.y;
            pg[patch].norm[Z] = norm.z;

            if (sector == 0)
                at[patch] = xsaucer_attr;
            else
                at[patch] = saucer_attr;

        }
    }

    act->flags |= AFLAG_COLLIDE_ATT;
    obj->flags |= OFLAG_ATTACKER;
    obj->radius = toFIXED(55.0);
}

void AISaucerCruise(ACTOR* act, FIXED drag, FIXED stat)
{
    ANGLE face_az;
    ANGLE move_az;
    FIXED v;

    EOBJECT* obj = ActGetFirstObject(act);

    obj->vel.x += slMulFX(SAUCER_FACING->x, SAUCER_THROTTLE);
    obj->vel.y += slMulFX(SAUCER_FACING->y, SAUCER_THROTTLE);
    obj->vel.z += slMulFX(SAUCER_FACING->z, SAUCER_THROTTLE);

    obj->rot.x *= 7;
    obj->rot.x /= 8;
    obj->rot.z *= 7;
    obj->rot.z /= 8;

    VecScale(&(obj->vel), drag);
    v = VecMag(&(obj->vel));

    if (v > stat)
        v -= stat;
    else
        v = toFIXED(0.0);

    VecSetMag(&(obj->vel), v);

    slMulFX(SAUCER_THROTTLE, toFIXED(0.9));
    if (SAUCER_THROTTLE > toFIXED(0.0))
    {
        if (SAUCER_THROTTLE > stat)
            SAUCER_THROTTLE -= stat;
        else
            SAUCER_THROTTLE = toFIXED(0.0);
    }
    else
    {
        if (SAUCER_THROTTLE < -stat)
            SAUCER_THROTTLE += stat;
        else
            SAUCER_THROTTLE = toFIXED(0.0);
    }
}

void AISaucerProcess2(ACTOR* act, PROCESS* proc)
{
    FIXED alt;
    EOBJECT* landObj = ActGetFirstObject(actLand);
    EOBJECT* obj = ActGetFirstObject(act);

    alt = obj->pos.y - landObj->pos.y;

    if (alt < toFIXED(5.0))
        alt = toFIXED(5.0);

    AISaucerAccelerate(act, toFIXED(0.0), slDivFX(slSquartFX(alt), SAUCER_ANTIGRAV), toFIXED(0.0));
    AISaucerCruise(act, toFIXED(0.9), toFIXED(0.01));

}

void AISaucerProcess1(ACTOR* act, PROCESS* proc)
{
    AISaucerRestart(act);

    ProcSetFunction(proc, AISaucerProcess2);
}

int AISaucerMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        AISaucerBuild(act);
        ActAddProcess(act, AISaucerProcess1, 1, 1, 1);
        return ( TRUE);

    default:
        return ( FALSE);

    }
}

