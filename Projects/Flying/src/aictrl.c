//------------------------------------------------------------------------
//
//	AICTRL.C
//	Controller input-handler actor
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
#include "ai.h"
#include "sega_per.h"

void AISaucerRestart(ACTOR* act);
void AISaucerAntiGrav(ACTOR* act, FIXED v);
void AISaucerAccelerate(ACTOR* act, FIXED x, FIXED y, FIXED z);
void AISaucerShoot(ACTOR* act);

#define CONTROL_SAUCER ((actControl->scratch.l)[0])
#define CONTROL_CAMERA ((actControl->scratch.l)[1])

#define PAD_START  PER_DGT_ST
#define PAD_S   (PAD_START) 
#define PAD_RIGHT  PER_DGT_KR
#define PAD_R   (PAD_RIGHT)
#define PAD_LEFT  PER_DGT_KL
#define PAD_L   (PAD_LEFT)
#define PAD_UP   PER_DGT_KU
#define PAD_U   (PAD_UP)
#define PAD_DOWN  PER_DGT_KD
#define PAD_D   (PAD_DOWN)
#define PAD_A   PER_DGT_TA
#define PAD_B   PER_DGT_TB
#define PAD_C   PER_DGT_TC
#define PAD_X   PER_DGT_TX
#define PAD_Y   PER_DGT_TY
#define PAD_Z   PER_DGT_TZ
#define PAD_TRIG_RIGHT PER_DGT_TR
#define PAD_TR   (PAD_TRIG_RIGHT)
#define PAD_TRIG_LEFT PER_DGT_TL
#define PAD_TL   (PAD_TRIG_LEFT)

int ctrlPadStatusCurrent = 0;
int ctrlPadStatusOld = 0;
int ctrlPadStatusNew = 0;
int ctrlPadStatusDelta = 0;

void AIControlGetSaturnPad(void)
{
    ctrlPadStatusOld = ctrlPadStatusCurrent;

    if (Per_Connect1)
    {
        ctrlPadStatusCurrent = ~(Smpc_Peripheral[0].data);
    }

    ctrlPadStatusNew = ctrlPadStatusCurrent & (~ctrlPadStatusOld);
    ctrlPadStatusDelta = ctrlPadStatusCurrent ^ ctrlPadStatusOld;
}

void AIControlProcess1(ACTOR* act, PROCESS* proc)
{
    ACTOR* sa;
    ACTOR* ca;

    sa = (ACTOR*) CONTROL_SAUCER;
    ca = (ACTOR*) CONTROL_CAMERA;

    AIControlGetSaturnPad();

    if (ctrlPadStatusCurrent & PAD_S) AISaucerRestart(sa);
    if (ctrlPadStatusCurrent & PAD_X) AISaucerAntiGrav(sa, toFIXED(1.0));
    if (ctrlPadStatusCurrent & PAD_A) AISaucerAntiGrav(sa, toFIXED(-1.0));
    if (ctrlPadStatusCurrent & PAD_L) AISaucerTurn(sa, toFIXED(-0.1));
    if (ctrlPadStatusCurrent & PAD_R) AISaucerTurn(sa, toFIXED(0.1));
    if (ctrlPadStatusCurrent & PAD_TL) AISaucerSlip(sa, toFIXED(-1.5));
    if (ctrlPadStatusCurrent & PAD_TR) AISaucerSlip(sa, toFIXED(1.5));
    if (ctrlPadStatusCurrent & PAD_U) AISaucerForward(sa, toFIXED(0.2));
    if (ctrlPadStatusCurrent & PAD_D) AISaucerForward(sa, toFIXED(-0.1));

    if (ctrlPadStatusNew & PAD_B) AISaucerShoot(sa);

}

int AIControlMsgHand(ACTOR* act, MESSAGE_FLAGS mess, long data)
{
    switch (mess)
    {
    case ASYNC_MESS_CREATE:
        ActAddProcess(act, AIControlProcess1, DELAY_60HZ, DELAY_60HZ, 1);
        return ( TRUE);

    default:
        return ( FALSE);
    }
}

