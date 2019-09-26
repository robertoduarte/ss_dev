#include	"cd_ctrl.h"
#include	"sega_gfs.h"
#include	"sega_snd.h"
#include	"sega_int.h"

/*****	Const	*****/
#define MAX_OPEN        1
#define MAX_DIR         10
#define	SDDRV_NAME	"SDDRVS.TSK"
#define	SDMAP_NAME	"SMPSND5.MP"
#define	SDDRV_ADDR	0x6080000
#define	SDDRV_SIZE	0x6000
#define	SDMAP_ADDR	0x6086000
#define	SDMAP_SIZE	0x14

/*****	Variable *****/
Uint32     lib_work[GFS_WORK_SIZE(MAX_OPEN) / sizeof(Uint32)];
GfsDirTbl  dirtbl;
GfsDirName dirname[MAX_DIR];
Sint32	   flag_snddrv = 0;

/***	Objects. ***/
static	CdcPly	playdata;
static	CdcPos	posdata;

/*****	Sound Interruppt *****/
void	SndIntr(void) {

    SND_RESET_INT();
}

/*** Initialize at first. ***/
void	InitSoundDriver(void) {

    SndIniDt snd_init;

    if (flag_snddrv == 0) {
	CDC_CdInit(0x00,0x00,0x05,0x0f);
	GFS_DIRTBL_TYPE(&dirtbl)    = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&dirtbl) = dirname;
	GFS_DIRTBL_NDIR(&dirtbl)    = MAX_DIR;
	GFS_Init(MAX_OPEN, lib_work, &dirtbl);
	GFS_Load(GFS_NameToId(SDDRV_NAME),0,(void *) SDDRV_ADDR,SDDRV_SIZE);
	GFS_Load(GFS_NameToId(SDMAP_NAME),0,(void *) SDMAP_ADDR,SDMAP_SIZE);

	flag_snddrv = 1;
    }
    set_imask(0);
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_SND);
    INT_SetScuFunc(INT_SCU_SND, SndIntr);
    INT_ChgMsk(INT_MSK_SND,INT_MSK_NULL);

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)SDDRV_ADDR;
    SND_INI_PRG_SZ(snd_init)  = (Uint16)  SDDRV_SIZE;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)SDMAP_ADDR;
    SND_INI_ARA_SZ(snd_init)  = (Uint16)  SDMAP_SIZE;
    SND_Init(&snd_init);
    SND_ChgMap(0);
    SND_SetCdDaLev(7,7);                        /* CD-DAレベルの設定         */
    SND_SetCdDaPan(0,0);                        /* CD-DAのPAN設定            */
    SND_SetTlVl(15);                            /* Total Volumeの設定        */
    SND_RESET_INT();                            /* 割込みステータスリセット  */
}

void	InitCDBlock( void ){

    InitSoundDriver();
    CDC_PLY_STYPE(&playdata) = CDC_PTYPE_TNO;	/* set by track number.*/
    CDC_PLY_STNO( &playdata) = 2;		/* start track number. */
    CDC_PLY_SIDX( &playdata) = 1;		/* start index number. */
    CDC_PLY_ETYPE(&playdata) = CDC_PTYPE_TNO;	/* set by track number.*/
    CDC_PLY_ETNO( &playdata) = 3;		/* start track number. */
    CDC_PLY_EIDX( &playdata) = 99;		/* start index number. */
    CDC_PLY_PMODE(&playdata) = CDC_PM_DFL + 1;	/* Play Mode. */
}

/*** Play Music. ***/
void	PlayMusic( int track ){

    CDC_PLY_STNO( &playdata ) = (Uint8) track;
    CDC_PLY_ETNO( &playdata ) = (Uint8) track;
    CDC_CdPlay(&playdata);
}

/* Stop Music. */
void	StopMusic( void ){
    CDC_POS_PTYPE( &posdata ) = CDC_PTYPE_DFL;	/* Stop Music. */
}

/*** CD Open Check ***/
void CheckCDOpen(void){

    CdcStat stat;

    CDC_GetPeriStat(&stat);
    /* 定期レスポンスとトレイが空いているをチェック */
    if ( stat.status == ( CDC_ST_PERI | CDC_ST_OPEN ) ) {
	SYS_EXECDMP();		/* ＣＤマルチプレイヤーへ */
    }
}
