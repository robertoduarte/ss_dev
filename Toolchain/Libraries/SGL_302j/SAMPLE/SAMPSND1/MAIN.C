
/*							*/
/*  Sound control sample				*/
/*							*/

#include    "sl_def.h"
#include    "sddrvs.dat"

/*------------------------------------------------------------------------*/

#define     SoundSeqBuf		0x25a0b000

extern char sound_map[];
extern char sound_dat[];
extern Uint32 mapsize;
extern Uint32 datsize;
extern PDATA PD_Cube ;

/*------------------------------------------------------------------------*/

#define		_Play		0x01
#define		_Pause		0x02
#define		_Fade		0x04

typedef struct{
    struct{
	Sint8	stat ;
	Sint16	tmp ;
	Sint8	pan ;
    }seq[8] ;
    Uint8 vol ;
    Uint8 banknum ;
    Uint8 songnum ;
    Uint8 sequence ;
} SOUND_MAN ;

/*------------------------------------------------------------------------*/

void init_sound(){
    slInitSound(sddrvstsk , sizeof(sddrvstsk) , (Uint8 *)sound_map , mapsize) ;
    slDMACopy(sound_dat , (void *)(SoundSeqBuf) , datsize) ;
}

void disp_vol(Uint16 vol){
    slSndVolume(vol << 3) ;
    slPrint("Volume : " , slLocate(2,6)) ;
    slPrintHex(vol , slLocate(11,6)) ;
}

sound_test(EVENT *evptr){
    SOUND_MAN	*smptr ;
    PerDigital	*pptr ;
    Sint16	padd ;
    Sint16	padp ;

    int i ;
    
    const Uint8 bank = 3 ;
    const Uint8 song[] = {1 , 7 , 0 , 1} ;
    
    smptr = (SOUND_MAN *)evptr->user ;
    pptr = Smpc_Peripheral;
    padd = ~pptr->data;
    padp = ~pptr->push;
    
    for (i = 7; i >= 0; i--){
	if(smptr->seq[i].stat && !(slSequenceStat(i))){
	    smptr->seq[i].stat = 0 ;
	}
	switch(smptr->seq[i].stat){
	  case 0 : slPrint("        ", slLocate(33,3+i)) ; break ;
	  case 1 : slPrint("Play    ", slLocate(33,3+i)) ; break ;
	  case 3 : slPrint("Pause   ", slLocate(33,3+i)) ; break ;
	  case 5 : slPrint("Fade Out", slLocate(33,3+i)) ; break ;
	  case 7 : slPrint("Pause   ", slLocate(33,3+i)) ; break ;
	}
    }

    if(padd & PER_DGT_ST){
	if(padp & PER_DGT_KU){
	    if(++smptr->banknum > bank) smptr->banknum = 0 ;
	    if(smptr->songnum > song[smptr->banknum]) smptr->songnum = song[smptr->banknum] ;
	} else if(padp & PER_DGT_KD){
	    if(--smptr->banknum > bank) smptr->banknum = bank ;
	    if(smptr->songnum > song[smptr->banknum]) smptr->songnum = song[smptr->banknum] ;
	}
	if(padp & PER_DGT_KR){
	    if(++smptr->songnum > song[smptr->banknum]) smptr->songnum = 0 ;
	} else if(padp & PER_DGT_KL){
	    if(--smptr->songnum > song[smptr->banknum]) smptr->songnum = song[smptr->banknum] ;
	}
    } else {
	if(padp & PER_DGT_KU){
	    if(smptr->vol < 15){
		disp_vol(++smptr->vol) ;
	    }
	} else if(padp & PER_DGT_KD){
	    if(smptr->vol > 0){
		disp_vol(--smptr->vol) ;
	    }
	}
	if(padp & PER_DGT_KR){
	    if(++smptr->sequence > 7) smptr->sequence = 1;
	} else if(padp & PER_DGT_KL){
	    if(--smptr->sequence < 1) smptr->sequence = 7;
	}
    }
 
    slPrintHex(slHex2Dec(smptr->banknum) , slLocate(7,10)) ;
    slPrint("Bank     : " , slLocate(2,10)) ;
    slPrintHex(slHex2Dec(smptr->songnum) , slLocate(7,11)) ;
    slPrint("Song     : " , slLocate(2,11)) ;
    slPrintHex(slHex2Dec(smptr->sequence) , slLocate(7,12)) ;
    slPrint("Sequence : " , slLocate(2,12)) ;

    if(padp & PER_DGT_TX){
	if(smptr->seq[0].stat & _Play){
	    slBGMOff() ;
	    smptr->seq[0].stat = 0 ;
	} else {
	    slBGMOn((smptr->banknum << 8) + smptr->songnum,0,127,0) ;
	    smptr->seq[0].stat |= _Play ;
	}
    }
    if(padp & PER_DGT_TA){
	if(smptr->seq[smptr->sequence].stat & _Play){
	    slSequenceOff(smptr->sequence) ;
	    smptr->seq[smptr->sequence].stat = 0 ;
	} else {
	    smptr->seq[slSequenceOn((smptr->banknum << 8) + smptr->songnum,0,127,0)].stat |= _Play ;
	}
    }
    if(smptr->seq[0].stat & _Play){
	if(padp & PER_DGT_TY){
	    if(smptr->seq[0].stat & _Pause){
		slBGMCont() ;
		smptr->seq[0].stat &= ~(_Pause) ;
	    } else {
		slBGMPause() ;
		smptr->seq[0].stat |= _Pause ;
	    }
	}
	if(!(smptr->seq[0].stat & _Pause)){
	    if(padp & PER_DGT_TZ){
		if(smptr->seq[0].stat & _Fade){
		    smptr->seq[0].stat &= ~(_Fade) ;
		    slBGMFade(127 , 25) ; /* Fade In */
		} else {
		    smptr->seq[0].stat |= _Fade ;
		    slBGMFade(1 , 25) ;	/* Fade Out */
		}
	    }
	}
    }
    if(smptr->seq[smptr->sequence].stat & _Play){
	if(padp & PER_DGT_TB){
	    if(smptr->seq[smptr->sequence].stat & _Pause){
		slSequenceCont(smptr->sequence) ;
		smptr->seq[smptr->sequence].stat &= ~(_Pause) ;
	    } else {
		slSequencePause(smptr->sequence) ;
		smptr->seq[smptr->sequence].stat |= _Pause ;
	    }
	}
	if(!(smptr->seq[smptr->sequence].stat & _Pause)){
	    if(padp & PER_DGT_TC){
		if(smptr->seq[smptr->sequence].stat & _Fade){
		    smptr->seq[smptr->sequence].stat &= ~(_Fade) ;
		    slSequenceFade(smptr->sequence , 127 , 25) ; /* Fade In */
		} else {
		    smptr->seq[smptr->sequence].stat |= _Fade ;
		    slSequenceFade(smptr->sequence , 1 , 25) ;	/* Fade Out */
		}
	    }
	}
    }
}

void init_sound_test(EVENT *evptr){
    SOUND_MAN	*smptr ;
    int i ;

    evptr->exad = (void *)sound_test ;
    smptr = (SOUND_MAN *)evptr->user ;
    smptr->vol = 15 ;
    smptr->sequence = 1 ;
    smptr->banknum = 0 ;
    smptr->songnum = 0 ;

    for(i = 7; i >= 0; i--){
	smptr->seq[i].stat = 0 ;
	slPrintHex(i , slLocate(24,3+i)) ;
    }		
    disp_vol(smptr->vol) ;
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
    slSetEvent((void *)init_sound_test) ;
    slSetEvent((void *)disp_cube) ;

    slScrAutoDisp(NBG0ON) ;
    slPrint("Sound test" , slLocate(10,2)) ;
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
