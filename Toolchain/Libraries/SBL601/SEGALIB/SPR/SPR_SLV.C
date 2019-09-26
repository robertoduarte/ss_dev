/*
 *  Slave SH2 Driver Module
 *
 *  <1st version>: FRT inputcapture is not supported. (1994-04-01)
 *                 SOUND RAM is used for MasterSH SlaveSH Handshake flag.
 *
 *  <2nd version>: FRT inputcapture is supported. (1994-04-01)
 *                 work RAM is used for Master Slave parameter passing.
 *
 */

#include "sega_xpt.h"
#include "sega_spr.h"
#include <stdlib.h>
#include <machine.h>


Uint32
  *SPR_SlaveCommand  = (Uint32 *)0;          /* MASTER to SLAVE command AREA */

Uint32
  SPR_SlaveState    = (Uint32)0;             /* SLAVE to MASTER state  AREA */

Uint32
  SPR_SlaveParam    = (Uint32)0;             /* MASTER to SLAVE parameter  AREA */

#ifdef USE_SLAVE

volatile void **SPR_SlaveSHEntry
/*  = (void **)0x6000250; 			 95-7-27	*/
  = (volatile void **)0x6000250;  /* 95-7-27  BOOT ROMs dispatch address */

volatile Uint8 *SPR_SMPC_COM = (Uint8 *)0x2010001F;   /* SMPC command register */
volatile Uint8 *SPR_SMPC_RET = (Uint8 *)0x2010005f;   /* SMPC result register */
volatile Uint8 *SPR_SMPC_SF  = (Uint8 *)0x20100063;   /* SMPC status flag */

const Uint8 SPR_SMPC_SSHON  = 0x02;          /* SMPC slave SH on command */
const Uint8 SPR_SMPC_SSHOFF = 0x03;          /* SMPC slave SH off command */

/* ============ DECLARE FUNCTIONS =========== */

void SPR_InitSlaveSH(void);
void SPR_SlaveSHmain(void);

/* ============ DEFINE FUNCTIONS ============ */

/* -Change BOOT ROMs "slave SH dummy loop entry" To "SlaveSHmain entry" */
/* -Enable Slave SH with SMPC command SSHON                             */
/* -Then Slave SH start and Initialized by BOOT ROM code                */
/* -Finally Slave SH round a flag checking loop in SlaveSHmain          */


void /* slave SH Initialize (RUNS on main SH) */
  SPR_InitSlaveSH(void)
{
    volatile Uint16 i;

    *(volatile Uint8 *)0xfffffe10  = 0x01;    /* TIER FRT INT disable */
    SPR_SlaveState = 0;                /* set RUNNING state */
    /* SlaveSH のリセット状態を設定する */
    while((*SPR_SMPC_SF & 0x01) == 0x01);
    *SPR_SMPC_SF = 1;                 /* --- SMPC StatusFlag SET */
    *SPR_SMPC_COM = SPR_SMPC_SSHOFF;      /* --- Slave SH OFF SET */
    while((*SPR_SMPC_SF & 0x01) == 0x01);
    for(i = 0 ; i < 1000; i++);   /* slave reset assert length */
    *(void **)SPR_SlaveSHEntry = (void *)&SPR_SlaveSHmain; /* dispatch address set */
    /* SlaveSH のリセット状態を解除する */
    *SPR_SMPC_SF = 1;                 /* --- SMPC StatusFlag SET */
    *SPR_SMPC_COM = SPR_SMPC_SSHON;       /* --- Slave SH ON SET */
    while((*SPR_SMPC_SF & 0x01) == 0x01);
}

void /* Slave SH2 main loop (RUNS on slave SH) */
  SPR_SlaveSHmain(void)
{
/*    const Uint32 RUNNING = 1;		95-7-27	unuse
    const Uint32 WAITING = 0;
*/

    /* Wait until SlaveSHReqCode is set */
    /* then call function for SlaveSHReqCode */
    set_imask(0xf);
    *(volatile Uint16 *)0xfffffee2 = 0x0000;  /* IPRA int disable */
    *(volatile Uint16 *)0xfffffe60 = 0x0000;  /* IPRB int disable */
    *(volatile Uint8 *)0xfffffe10  = 0x01;    /* TIER FRT INT disable */
    while(1){
	/* Use "FRT InputCaptureFlag" Poling for wait command from Master */
        if((*(volatile Uint8 *)0xfffffe11 & 0x80) == 0x80){
	   *(Uint8 *)0xfffffe11 = 0x00; /* FTCSR clear */
	   if((*(void (*)(void*))*(void **)((Uint32)&SPR_SlaveCommand+0x20000000)))
           {
             /* chache parse all */
             *(volatile Uint16 *)0xfffffe92 |= 0x10;
	     (*(void (*)(void*))*(void **)((Uint32)&SPR_SlaveCommand+0x20000000))
                                                         ((void*)SPR_SlaveParam);
             /* frt inp to master */
             *(volatile Uint16 *)0x21800000 = 0xffff;
	   }
	}
    }
}


void  SPR_RunSlaveSH(PARA_RTN *routine, void *parm)
{
    SPR_SlaveCommand = (Uint32*)routine;
    SPR_SlaveParam   = (Uint32)parm;
    *(volatile Uint16 *)0x21000000 = 0xffff;
}


void  SPR_WaitEndSlaveSH(void)
{
    while((*(volatile Uint8 *)0xfffffe11 & 0x80) != 0x80);
    *(volatile Uint8 *)0xfffffe11 = 0x00; /* FTCSR clear */
    *(volatile Uint16 *)0xfffffe92 |= 0x10; /* chache parse all */
}

#else

void /* slave SH Initialize (RUNS on main SH) */
  SPR_InitSlaveSH(void)
{
}

void  SPR_RunSlaveSH(PARA_RTN *routine, void *parm)
{
    SPR_SlaveParam   = (Uint32)parm;
    routine((void*)SPR_SlaveParam);
}


void  SPR_WaitEndSlaveSH(void)
{
}

#endif
