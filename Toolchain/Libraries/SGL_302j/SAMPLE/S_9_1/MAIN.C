/*----------------------------------------------------------------------*/
/*	Pad Control							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"sega_sys.h"

#define		NBG1_CEL_ADR		( VDP2_VRAM_B1 + 0x02000 )
#define		NBG1_MAP_ADR		( VDP2_VRAM_B1 + 0x12000 )
#define		NBG1_COL_ADR		( VDP2_COLRAM + 0x00200 )
#define		BACK_COL_ADR		( VDP2_VRAM_A1 + 0x1fffe )
#define		PAD_NUM			13

static Uint16 pad_asign[] = {
	PER_DGT_KU,
	PER_DGT_KD,
	PER_DGT_KR,
	PER_DGT_KL,
	PER_DGT_TA,
	PER_DGT_TB,
	PER_DGT_TC,
	PER_DGT_ST,
	PER_DGT_TX,
	PER_DGT_TY,
	PER_DGT_TZ,
	PER_DGT_TR,
	PER_DGT_TL,
};

extern pad_cel[];
extern pad_map[];
extern pad_pal[];
extern TEXTURE tex_spr[];
extern PICTURE pic_spr[];
extern FIXED stat[][XYZS];
extern SPR_ATTR attr[];
extern ANGLE angz[];

static void set_sprite(PICTURE *pcptr , Uint32 NbPicture)
{
	TEXTURE *txptr;
 
	for(; NbPicture-- > 0; pcptr++){
		txptr = tex_spr + pcptr->texno;
		slDMACopy((void *)pcptr->pcsrc,
			(void *)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcptr->cmode)));
	}
}

static void disp_sprite()
{
	static Sint32 i;
	Uint16 data;

	if(!Per_Connect1) return;
	data = Smpc_Peripheral[0].data;

	for(i=0;i<PAD_NUM;i++){
		if((data & pad_asign[i])==0){
			slDispSprite((FIXED *)stat[i],
				(SPR_ATTR *)(&attr[i].texno),(ANGLE)angz[i]);
		}
	}
}

void ss_main(void)
{
	slInitSystem(TV_320x224,tex_spr,1);
	slTVOff();
	set_sprite(pic_spr,1);
	slPrint("Sample program 9.1" , slLocate(9,2));
	
	slColRAMMode(CRM16_1024);
	slBack1ColSet((void *)BACK_COL_ADR , 0);

	slCharNbg1(COL_TYPE_256 , CHAR_SIZE_1x1);
	slPageNbg1((void *)NBG1_CEL_ADR , 0 , PNB_1WORD|CN_12BIT);
	slPlaneNbg1(PL_SIZE_1x1);
	slMapNbg1((void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR , (void *)NBG1_MAP_ADR);
	Cel2VRAM(pad_cel , (void *)NBG1_CEL_ADR , 483*64);
	Map2VRAM(pad_map , (void *)NBG1_MAP_ADR , 32 , 19 , 1 , 256);
	Pal2CRAM(pad_pal , (void *)NBG1_COL_ADR , 256);

	slScrPosNbg1(toFIXED(-32.0) , toFIXED(-36.0));
	slScrAutoDisp(NBG0ON | NBG1ON);
	slTVOn();
	
	while(1) {
		disp_sprite();
		slSynch();
	} 
}

