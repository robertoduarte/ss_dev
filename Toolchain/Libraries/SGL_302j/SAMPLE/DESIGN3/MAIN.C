/*----------------------------------------------------------------------*/
/*	Design Check 3							*/
/*----------------------------------------------------------------------*/
#include	"sgl.h"
#include	"sega_sys.h"

#define		max_texture		2

extern OBJECT	object_sample[];
extern TEXTURE	tex_sample[];
extern PICTURE	pic_sample[];

typedef struct {
	FIXED pos[XYZ];
	ANGLE ang[XYZ];
	FIXED sca[XYZ];
	MATRIX base;
} CAMERA;

static CAMERA cam;

int	check_pad_change( CAMERA *cam )
{
    static	FIXED	old_pos[XYZ];
    static	FIXED	old_scl[XYZ];
    static	int	fast=1;
    int		code = 0;
    int		i;

    if ( fast )
      {
	  fast = 0;
	  code = 1;
	  memcpy( old_pos, cam->pos, sizeof(FIXED)*XYZ );
	  memcpy( old_scl, cam->sca, sizeof(FIXED)*XYZ );
      }
    else
      {
	  for( i=0; i<XYZ; i++ )
	    {
		if ( (DEGtoANG(0)!= cam->ang[i]) ||
		     (old_pos[i] != cam->pos[i]) ||
		     (old_scl[i] != cam->sca[i])    )
		  {
		      code = 1;
		      memcpy( old_pos, cam->pos, sizeof(FIXED)*XYZ );
		      memcpy( old_scl, cam->sca, sizeof(FIXED)*XYZ );
		      break;
		  }
	    }
      }

    return code;
}

static void init_cam()
{
	cam.pos[X] = cam.pos[Y] = toFIXED(0);
	cam.pos[Z] = toFIXED(220.0);
	cam.ang[X] = cam.ang[Y] = cam.ang[Z] = DEGtoANG(0);
	cam.sca[X] = cam.sca[Y] = cam.sca[Z] = toFIXED(1);
	slUnitMatrix(cam.base);
}

static void set_texture(PICTURE* pcptr, Uint32 NbPicture)
{
	TEXTURE* txptr;

	for(;NbPicture-- > 0;pcptr++) {
		txptr = tex_sample + pcptr->texno;
		slDMACopy((void*)pcptr->pcsrc, (void*)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcptr->cmode)));
	}
}

static void get_pad()
{
	Uint16 data;

	if(!Per_Connect1) return;
	data = Smpc_Peripheral[0].data;

	if((data & PER_DGT_TA) == 0) {
		if((data & PER_DGT_KR) == 0) cam.pos[X] += toFIXED(2);
		if((data & PER_DGT_KL) == 0) cam.pos[X] -= toFIXED(2);
		if((data & PER_DGT_KU) == 0) cam.pos[Y] -= toFIXED(2);
		if((data & PER_DGT_KD) == 0) cam.pos[Y] += toFIXED(2);
		if((data & PER_DGT_TR) == 0) cam.pos[Z] -= toFIXED(2);
		if((data & PER_DGT_TL) == 0) cam.pos[Z] += toFIXED(2);
	}

	cam.ang[X] = cam.ang[Y] = cam.ang[Z] = DEGtoANG(0);
	if((data & PER_DGT_TB) == 0) {
		if((data & PER_DGT_KU) == 0) cam.ang[X] = DEGtoANG( 2);
		if((data & PER_DGT_KD) == 0) cam.ang[X] = DEGtoANG(-2);
		if((data & PER_DGT_KR) == 0) cam.ang[Y] = DEGtoANG( 2);
		if((data & PER_DGT_KL) == 0) cam.ang[Y] = DEGtoANG(-2);
		if((data & PER_DGT_TR) == 0) cam.ang[Z] = DEGtoANG(-2);
		if((data & PER_DGT_TL) == 0) cam.ang[Z] = DEGtoANG( 2);
	}

	if((data & PER_DGT_TC) == 0) {
		if((data & PER_DGT_KU) == 0) {
			cam.sca[X] += toFIXED(0.01);
			cam.sca[Y] += toFIXED(0.01);
			cam.sca[Z] += toFIXED(0.01);
		}
		if((data & PER_DGT_KD) == 0) {
			cam.sca[X] -= toFIXED(0.01);
			cam.sca[Y] -= toFIXED(0.01);
			cam.sca[Z] -= toFIXED(0.01);
		}
	}

	if((data & PER_DGT_TX) == 0) {
		cam.pos[X] = cam.pos[Y] = toFIXED(0);
		cam.pos[Z] = toFIXED(220);
	}
	if((data & PER_DGT_TY) == 0) {
		cam.ang[X] = cam.ang[Y] = cam.ang[Z] = DEGtoANG(0);
		slUnitMatrix(cam.base);
	}
	if((data & PER_DGT_TZ) == 0) {
		cam.sca[X] = cam.sca[Y] = cam.sca[Z] = toFIXED(1);
	}
	if((data & PER_DGT_ST) == 0) init_cam();
}

static void drawpolygon(OBJECT *obj)
{
    slPushMatrix();
    {
	slTranslate(obj->pos[X],obj->pos[Y],obj->pos[Z]);
	slRotZ(obj->ang[Z]);
	slRotY(obj->ang[Y]);
	slRotX(obj->ang[X]);
	slScale(obj->scl[X],obj->scl[Y],obj->scl[Z]);

	if(obj->pat != NULL){
	    slPutPolygon(obj->pat);
	}
	if(obj->child != NULL){
	    drawpolygon(obj->child);
	}


	if(obj->sibling != NULL){

	    slPopMatrix();
	    {
		drawpolygon(obj->sibling);
	    }
	    slPushMatrix();
	}
    }
    slPopMatrix();
}

void ss_main(void)
{
	slInitSystem(TV_320x224, tex_sample, 1);
	set_texture(pic_sample, max_texture);
	slPrint("Sample program design check 3", slLocate(6,2));
	init_cam();
	while(-1) {
		get_pad();

		if ( check_pad_change( &cam ) )
		  {
		      slPushMatrix();
		      {
			  slLoadMatrix(cam.base);
			  slTransposeMatrix();
			  slRotX(cam.ang[X]);
			  slRotY(cam.ang[Y]);
			  slRotZ(cam.ang[Z]);
			  slTransposeMatrix();
			  slGetMatrix(cam.base);
		      }
		      slPopMatrix();
		  }

		slPushMatrix();
		{
			slTranslate(cam.pos[X], cam.pos[Y], cam.pos[Z]);
			slMultiMatrix(cam.base);
			slScale(cam.sca[X], cam.sca[Y], cam.sca[Z]);
			drawpolygon(object_sample);
		}
		slPopMatrix();

		slSynch();
	}
}

