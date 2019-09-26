
/*							*/
/*  Sound control sample				*/
/*							*/

#include    "sl_def.h"
#include    "sddrvs.dat"

/*------------------------------------------------------------------------*/

extern char s_16[];
extern Uint32 s_16_size;
extern PCM s_16_dat ;
extern char s_8[];
extern Uint32 s_8_size;
extern PCM s_8_dat ;
extern char m_16[];
extern Uint32 m_16_size;
extern PCM m_16_dat ;

extern PDATA PD_Cube ;

/*------------------------------------------------------------------------*/

void init_sound(){
    char sound_map[] = {0xff , 0xff} ;
    slInitSound(sddrvstsk , sizeof(sddrvstsk) , (Uint8 *)sound_map , sizeof(sound_map)) ;
}

sound_test(EVENT *evptr){
    PerDigital* pptr ;
    Sint16	 pad ;

    pptr = Smpc_Peripheral;
    pad = ~pptr->push;   /* Push data */

    if(pad & PER_DGT_TX){
	slPCMOn(&s_16_dat , s_16 , s_16_size) ;
    }
    if(pad & PER_DGT_TY){
	slPCMOn(&s_8_dat , s_8 , s_8_size) ;
    }
    if(pad & PER_DGT_TZ){
	slPCMOn(&m_16_dat , m_16 , m_16_size) ;
    }
}

/*------------------------------------------------------------------------*/

typedef struct{
    ANGLE	angy ;
} CUBE_MAN ;

void disp_cube(EVENT *evptr){
    CUBE_MAN	*cbptr ;

    cbptr = (CUBE_MAN *)evptr->user ;
    slPushUnitMatrix() ;
    {
	slTranslate(toFIXED(0) , toFIXED(0) , toFIXED(500)) ;
	slRotY(cbptr->angy) ;
	cbptr->angy += 0x0100 ;
	slPutPolygon(&PD_Cube) ;
    }
    slPopMatrix() ;
}

/*------------------------------------------------------------------------*/

sample(){
    init_sound() ;
    slInitEvent() ;
    slSetEvent((void *)sound_test) ;
    slSetEvent((void *)disp_cube) ;

    slScrAutoDisp(NBG0ON) ;
    slPrint("Sound PCM test" , slLocate(10,2)) ;
    while(-1){
	slExecuteEvent() ;
	slSynch() ;
    }
}
/*-----------------------------------------------------------------------*/

void ss_main(void){
    slInitSystem(TV_352x224,NULL,1) ;
    sample() ;
}

/*-----------------------------------------------------------------------*/
