/*----------------------------------------------------------------------------
 *  model.dat -- �X�v���C�g�T���v���P ���f����`
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-02-15 Ver.0.80
 *
 *  �X�v���C�g�R�c�e�X�g�\���p�f�[�^�B
 *
 *----------------------------------------------------------------------------
 */

#define DRAW16	MATE_AUTO_SHADING | COLOR_0 | ECDSPD_DISABLE | COMPO_REP
#define COLOR16 RGB16_COLOR(15,15,15)
#define DRAWTX	MATE_TEXTURE | COLOR_0 | ECDSPD_DISABLE | COMPO_REP
#define COLORTX 0
#define DRAW8	MATE_SHADING_TBL | COLOR_4 | ECDSPD_DISABLE | COMPO_REP
#define COLOR8  0

#define SHADING_KIND  GOURAUD_SHADING
/* #define SHADING_KIND  FLAT_SHADING */

#define  WIDTH  0.1
#define  HEIGHT 0.6
#define  HEIGHT_MIN 0.5
#define  HEIGHT_MAX 0.9
#define  Y0     0.0
#define  X0     WIDTH
#define  Y1     0.0
#define  X1     WIDTH*(4.0/5.0)
#define  Y2     0.0
#define  X2     WIDTH*(3.0/5.0)
#define  Y3     0.0
#define  X3     WIDTH*(2.0/5.0)
#define  Y4     0.0
#define  X4     WIDTH*(1.0/5.0)
#define  Y5     0.0
#define  X5     0.0

typedef	struct ClustData   ClustData;
struct ClustData {
    MthXyz   rot;
    Fixed32  posY;
    Fixed32  movY;
};
#define   POS_Y_0  (HEIGHT_MIN+0.0)/5.0
#define   MOV_Y_0  0.0005
#define   POS_Y_1  (HEIGHT_MIN+0.1)/5.0
#define   MOV_Y_1  0.001
#define   POS_Y_2  (HEIGHT_MIN+0.2)/5.0
#define   MOV_Y_2  -0.0005
#define   POS_Y_3  (HEIGHT_MIN+0.3)/5.0
#define   MOV_Y_3  -0.001
#define   POS_Y_4  (HEIGHT_MIN+0.4)/5.0
#define   MOV_Y_4  -0.0015
#define   POS_Y_5  (HEIGHT_MIN+0.2)/5.0
#define   MOV_Y_5  0.0015

static ClustData clustData20 = {
       { MTH_FIXED( 120), MTH_FIXED(0), MTH_FIXED(-180)},
         MTH_FIXED(POS_Y_0), MTH_FIXED(MOV_Y_0),
};
static ClustData clustData30 = {
       { MTH_FIXED( 135), MTH_FIXED(0), MTH_FIXED(-165)},
         MTH_FIXED(POS_Y_0), MTH_FIXED(MOV_Y_0),
};
static ClustData clustData40 = {
       { MTH_FIXED( 150), MTH_FIXED(0), MTH_FIXED(-150)},
         MTH_FIXED(POS_Y_0), MTH_FIXED(MOV_Y_0),
};
static ClustData clustData50 = {
       { MTH_FIXED( 165), MTH_FIXED(0), MTH_FIXED(-135)},
         MTH_FIXED(POS_Y_0), MTH_FIXED(MOV_Y_0),
};
static ClustData clustData60 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_0), MTH_FIXED(MOV_Y_0),
};
static ClustData clustData21 = {
       { MTH_FIXED(  60), MTH_FIXED(0), MTH_FIXED(-120)},
         MTH_FIXED(POS_Y_1), MTH_FIXED(MOV_Y_1),
};
static ClustData clustData31 = {
       { MTH_FIXED(  75), MTH_FIXED(0), MTH_FIXED(-105)},
         MTH_FIXED(POS_Y_1), MTH_FIXED(MOV_Y_1),
};
static ClustData clustData41 = {
       { MTH_FIXED(  90), MTH_FIXED(0), MTH_FIXED( -90)},
         MTH_FIXED(POS_Y_1), MTH_FIXED(MOV_Y_1),
};
static ClustData clustData51 = {
       { MTH_FIXED( 105), MTH_FIXED(0), MTH_FIXED( -75)},
         MTH_FIXED(POS_Y_1), MTH_FIXED(MOV_Y_1),
};
static ClustData clustData61 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_1), MTH_FIXED(MOV_Y_1),
};
static ClustData clustData22 = {
       { MTH_FIXED(   0), MTH_FIXED(0), MTH_FIXED( -60)},
         MTH_FIXED(POS_Y_2), MTH_FIXED(MOV_Y_2),
};
static ClustData clustData32 = {
       { MTH_FIXED(  15), MTH_FIXED(0), MTH_FIXED( -45)},
         MTH_FIXED(POS_Y_2), MTH_FIXED(MOV_Y_2),
};
static ClustData clustData42 = {
       { MTH_FIXED(  30), MTH_FIXED(0), MTH_FIXED( -30)},
         MTH_FIXED(POS_Y_2), MTH_FIXED(MOV_Y_2),
};
static ClustData clustData52 = {
       { MTH_FIXED(  45), MTH_FIXED(0), MTH_FIXED( -15)},
         MTH_FIXED(POS_Y_2), MTH_FIXED(MOV_Y_2),
};
static ClustData clustData62 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_2), MTH_FIXED(MOV_Y_2),
};
static ClustData clustData23 = {
       { MTH_FIXED( -60), MTH_FIXED(0), MTH_FIXED(   0)},
         MTH_FIXED(POS_Y_3), MTH_FIXED(MOV_Y_3),
};
static ClustData clustData33 = {
       { MTH_FIXED( -45), MTH_FIXED(0), MTH_FIXED(  15)},
         MTH_FIXED(POS_Y_3), MTH_FIXED(MOV_Y_3),
};
static ClustData clustData43 = {
       { MTH_FIXED( -30), MTH_FIXED(0), MTH_FIXED(  30)},
         MTH_FIXED(POS_Y_3), MTH_FIXED(MOV_Y_3),
};
static ClustData clustData53 = {
       { MTH_FIXED( -15), MTH_FIXED(0), MTH_FIXED(  45)},
         MTH_FIXED(POS_Y_3), MTH_FIXED(MOV_Y_3),
};
static ClustData clustData63 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_3), MTH_FIXED(MOV_Y_3),
};
static ClustData clustData24 = {
       { MTH_FIXED(-120), MTH_FIXED(0), MTH_FIXED(  60)},
         MTH_FIXED(POS_Y_4), MTH_FIXED(MOV_Y_4),
};
static ClustData clustData34 = {
       { MTH_FIXED(-105), MTH_FIXED(0), MTH_FIXED(  75)},
         MTH_FIXED(POS_Y_4), MTH_FIXED(MOV_Y_4),
};
static ClustData clustData44 = {
       { MTH_FIXED( -90), MTH_FIXED(0), MTH_FIXED(  90)},
         MTH_FIXED(POS_Y_4), MTH_FIXED(MOV_Y_4),
};
static ClustData clustData54 = {
       { MTH_FIXED( -75), MTH_FIXED(0), MTH_FIXED( 105)},
         MTH_FIXED(POS_Y_4), MTH_FIXED(MOV_Y_4),
};
static ClustData clustData64 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_4), MTH_FIXED(MOV_Y_4),
};
static ClustData clustData25 = {
       { MTH_FIXED(-180), MTH_FIXED(0), MTH_FIXED( 120)},
         MTH_FIXED(POS_Y_5), MTH_FIXED(MOV_Y_5),
};
static ClustData clustData35 = {
       { MTH_FIXED(-165), MTH_FIXED(0), MTH_FIXED( 135)},
         MTH_FIXED(POS_Y_5), MTH_FIXED(MOV_Y_5),
};
static ClustData clustData45 = {
       { MTH_FIXED(-150), MTH_FIXED(0), MTH_FIXED( 150)},
         MTH_FIXED(POS_Y_5), MTH_FIXED(MOV_Y_5),
};
static ClustData clustData55 = {
       { MTH_FIXED(-135), MTH_FIXED(0), MTH_FIXED( 165)},
         MTH_FIXED(POS_Y_5), MTH_FIXED(MOV_Y_5),
};
static ClustData clustData65 = {
       { MTH_FIXED(0), MTH_FIXED(0), MTH_FIXED(0)},
         MTH_FIXED(POS_Y_5), MTH_FIXED(MOV_Y_5),
};

static Uint16  colorData[] = {
/* 0x00 */  0x0000,
/* 0x01 */  RGB16_COLOR( 1, 1, 1),
/* 0x02 */  RGB16_COLOR( 1, 1, 1),
/* 0x03 */  RGB16_COLOR( 2, 2, 2),
/* 0x04 */  RGB16_COLOR( 3, 3, 3),
/* 0x05 */  RGB16_COLOR( 4, 4, 4),
/* 0x06 */  RGB16_COLOR( 5, 5, 5),
/* 0x07 */  RGB16_COLOR( 6, 6, 6),
/* 0x08 */  RGB16_COLOR( 7, 7, 7),
/* 0x09 */  RGB16_COLOR( 8, 8, 8),
/* 0x0a */  RGB16_COLOR( 9, 9, 9),
/* 0x0b */  RGB16_COLOR(10,10,10),
/* 0x0c */  RGB16_COLOR(11,11,11),
/* 0x0d */  RGB16_COLOR(12,12,12),
/* 0x0e */  RGB16_COLOR(13,13,13),
/* 0x0f */  RGB16_COLOR(14,14,14),
/* 0x10 */  RGB16_COLOR(15,15,15),
/* 0x11 */  RGB16_COLOR(16,16,16),
/* 0x12 */  RGB16_COLOR(17,17,17),
/* 0x13 */  RGB16_COLOR(18,18,18),
/* 0x14 */  RGB16_COLOR(19,19,19),
/* 0x15 */  RGB16_COLOR(20,20,20),
/* 0x16 */  RGB16_COLOR(21,21,21),
/* 0x17 */  RGB16_COLOR(22,22,22),
/* 0x18 */  RGB16_COLOR(23,23,23),
/* 0x19 */  RGB16_COLOR(24,24,24),
/* 0x1a */  RGB16_COLOR(25,25,25),
/* 0x1b */  RGB16_COLOR(26,26,26),
/* 0x1c */  RGB16_COLOR(27,27,27),
/* 0x1d */  RGB16_COLOR(28,28,28),
/* 0x1e */  RGB16_COLOR(29,29,29),
/* 0x1f */  RGB16_COLOR(30,30,30),
/* 0x20 */  RGB16_COLOR(31,31,31),
/* 0x21 */  RGB16_COLOR(31, 0, 0),
/* 0x22 */  RGB16_COLOR( 0, 0, 0),
/* 0x23 */  RGB16_COLOR( 0, 0, 0),
/* 0x24 */  RGB16_COLOR( 0, 0, 0),
/* 0x25 */  RGB16_COLOR( 0, 0, 0),
/* 0x26 */  RGB16_COLOR( 0, 0, 0),
/* 0x27 */  RGB16_COLOR( 0, 0, 0),
/* 0x28 */  RGB16_COLOR( 0, 0, 0),
/* 0x29 */  RGB16_COLOR( 0, 0, 0),
/* 0x2a */  RGB16_COLOR( 0, 0, 0),
/* 0x2b */  RGB16_COLOR( 0, 0, 0),
/* 0x2c */  RGB16_COLOR( 0, 0, 0),
/* 0x2d */  RGB16_COLOR( 0, 0, 0),
/* 0x2e */  RGB16_COLOR( 0, 0, 0),
/* 0x2f */  RGB16_COLOR(31, 0, 0),
};

static Uint16 shdIdxTbl8White[] = {
  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
};

static Uint16 *shdIdxTbl8[] = {
    /* 0 */  shdIdxTbl8White
};

/* �I�u�W�F�N�g6  ���_���W�e�[�u�� */
static MthXyz Obj6VertPoint[21];
static MthXyz Obj6VertNormal[21];

/* �I�u�W�F�N�g6  �ʂ̖@���x�N�g���e�[�u�� */
static MthXyz Obj6SurfaceNormal[20];

/* �I�u�W�F�N�g6  �ʒ�`�e�[�u�� */
static SprSurface Obj6Surface[] = {
	{	
		{
		0,4,5,1,
		},			/* �ʂ��\�����钸�_�ԍ�  0 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		1,5,6,2,
		},			/* �ʂ��\�����钸�_�ԍ�  1 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		2,6,7,3,
		},			/* �ʂ��\�����钸�_�ԍ�  2 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		3,7,4,0,
		},			/* �ʂ��\�����钸�_�ԍ�  3 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		4,8,9,5,
		},			/* �ʂ��\�����钸�_�ԍ�  4 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		5,9,10,6,      
		},		 /* �ʂ��\�����钸�_�ԍ�  5 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		6,10,11,7,     
		},		 /* �ʂ��\�����钸�_�ԍ�  6 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		7,11,8,4,      
		},		 /* �ʂ��\�����钸�_�ԍ�  7 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		8,12,13,9,
		},			/* �ʂ��\�����钸�_�ԍ�  8 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		9,13,14,10,    
		},		 /* �ʂ��\�����钸�_�ԍ�  9 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		10,14,15,11,   
		},		 /* �ʂ��\�����钸�_�ԍ� 10 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		11,15,12,8,    
		},		 /* �ʂ��\�����钸�_�ԍ� 11 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		12,16,17,13,   
		},		 /* �ʂ��\�����钸�_�ԍ� 12 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		13,17,18,14,   
		},		 /* �ʂ��\�����钸�_�ԍ� 13 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		14,18,19,15,   
		},		 /* �ʂ��\�����钸�_�ԍ� 14 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		15,19,16,12,   
		},		 /* �ʂ��\�����钸�_�ԍ� 15 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		16,20,17,16,   
		},		 /* �ʂ��\�����钸�_�ԍ� 16 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		17,20,18,17,   
		},		 /* �ʂ��\�����钸�_�ԍ� 17 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		18,20,19,18,   
		},		 /* �ʂ��\�����钸�_�ԍ� 18 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
	{	
		{
		19,20,16,19,   
		},		 /* �ʂ��\�����钸�_�ԍ� 19 */
		DRAW16 | ZSORT_MIN,
		COLOR16
	},
};
/* �I�u�W�F�N�g6  */
static SprObject3D Object6 = {
	6,                              /* �I�u�W�F�N�g�ԍ�  */
	INBETWEEN_OBJECT |
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	21,				/* ���_�� */
	20,				/* �ʐ� */
	(MthXyz *)&Obj6VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&Obj6VertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	(SprSurface *)&Obj6Surface,	/* �ʒ�`�e�[�u��*/
	(MthXyz *)&Obj6SurfaceNormal,	/* �ʂ̖@���x�N�g���e�[�u�� */
        NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(-4.0),                /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL				/* ���I�u�W�F�N�g */
};


static MthXyz ObjVertNormal[] = {
	{MTH_FIXED(-0.7071),MTH_FIXED( 0.0),MTH_FIXED( 0.7071)},
	{MTH_FIXED( 0.7071),MTH_FIXED( 0.0),MTH_FIXED( 0.7071)},
	{MTH_FIXED( 0.7071),MTH_FIXED( 0.0),MTH_FIXED(-0.7071)},
	{MTH_FIXED(-0.7071),MTH_FIXED( 0.0),MTH_FIXED(-0.7071)},
};

/* �I�u�W�F�N�g�O ���_���W�e�[�u�� */
static MthXyz Obj0VertPoint[] = {
	{MTH_FIXED(-X0),MTH_FIXED( Y0),MTH_FIXED( X0)},
	{MTH_FIXED( X0),MTH_FIXED( Y0),MTH_FIXED( X0)},
	{MTH_FIXED( X0),MTH_FIXED( Y0),MTH_FIXED(-X0)},
	{MTH_FIXED(-X0),MTH_FIXED( Y0),MTH_FIXED(-X0)},
};
/* �I�u�W�F�N�g�O */
static SprObject3D Object0 = {
	0,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	4,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj0VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&ObjVertNormal,       /* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(1.0),                 /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL, 				/* ���I�u�W�F�N�g */
};

/* �I�u�W�F�N�g�P ���_���W�e�[�u�� */
static MthXyz Obj1VertPoint[] = {
	{MTH_FIXED(-X1),MTH_FIXED( Y1),MTH_FIXED( X1)},
	{MTH_FIXED( X1),MTH_FIXED( Y1),MTH_FIXED( X1)},
	{MTH_FIXED( X1),MTH_FIXED( Y1),MTH_FIXED(-X1)},
	{MTH_FIXED(-X1),MTH_FIXED( Y1),MTH_FIXED(-X1)},
};
/* �I�u�W�F�N�g�P */
static SprObject3D Object1 = {
	1,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	GOURAUD_SHADING,
	4,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj1VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&ObjVertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(1.0),                 /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL				/* ���I�u�W�F�N�g */
};

/* �I�u�W�F�N�g�Q ���_���W�e�[�u�� */
static MthXyz Obj2VertPoint[] = {
	{MTH_FIXED(-X2),MTH_FIXED( Y2),MTH_FIXED( X2)},
	{MTH_FIXED( X2),MTH_FIXED( Y2),MTH_FIXED( X2)},
	{MTH_FIXED( X2),MTH_FIXED( Y2),MTH_FIXED(-X2)},
	{MTH_FIXED(-X2),MTH_FIXED( Y2),MTH_FIXED(-X2)},
};
/* �I�u�W�F�N�g�Q */
static SprObject3D Object2 = {
	2,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	4,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj2VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&ObjVertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(1.0),                 /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL				/* ���I�u�W�F�N�g */
};

/* �I�u�W�F�N�g3  ���_���W�e�[�u�� */
static MthXyz Obj3VertPoint[] = {
	{MTH_FIXED(-X3),MTH_FIXED( Y3),MTH_FIXED( X3)},
	{MTH_FIXED( X3),MTH_FIXED( Y3),MTH_FIXED( X3)},
	{MTH_FIXED( X3),MTH_FIXED( Y3),MTH_FIXED(-X3)},
	{MTH_FIXED(-X3),MTH_FIXED( Y3),MTH_FIXED(-X3)},
};
/* �I�u�W�F�N�g3  */
static SprObject3D Object3 = {
	3,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	4,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj3VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&ObjVertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(1.0),                 /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL				/* ���I�u�W�F�N�g */
};

/* �I�u�W�F�N�g4 ���_���W�e�[�u�� */
static MthXyz Obj4VertPoint[] = {
	{MTH_FIXED(-X4),MTH_FIXED( Y4),MTH_FIXED( X4)},
	{MTH_FIXED( X4),MTH_FIXED( Y4),MTH_FIXED( X4)},
	{MTH_FIXED( X4),MTH_FIXED( Y4),MTH_FIXED(-X4)},
	{MTH_FIXED(-X4),MTH_FIXED( Y4),MTH_FIXED(-X4)},
};
/* �I�u�W�F�N�g4  */
static SprObject3D Object4 = {
	4,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	4,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj4VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&ObjVertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(10.0),                /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	NULL				/* ���I�u�W�F�N�g */
};

/* �I�u�W�F�N�g5 ���_���W�e�[�u�� */
static MthXyz Obj5VertPoint[] = {
	{MTH_FIXED(X5),MTH_FIXED( Y5),MTH_FIXED(X5)},
};
static MthXyz Obj5VertNormal[] = {
	{MTH_FIXED( 0.0),   MTH_FIXED( 1.0),MTH_FIXED( 0.0)},
};

/* �I�u�W�F�N�g5  */
static SprObject3D Object5 = {
	5,                              /* �I�u�W�F�N�g�ԍ�  */
	DISP_POLYGON |                  /* �\���t���O */
	SHADING_KIND,
	1,				/* ���_�� */
	0,				/* �ʐ� */
	(MthXyz *)&Obj5VertPoint,	/* ���_���W�e�[�u�� */
	(MthXyz *)&Obj5VertNormal,	/* ���_�̖@���x�N�g���e�[�u�� */
	NULL,				/* �ʒ�`�e�[�u��*/
	NULL,				/* �ʂ̖@���x�N�g���e�[�u�� */
	NULL,                           /* �ʂ̑�\���_���W�e�[�u��  */
	NULL,                           /* �V�F�[�f�B���O�C���f�b�N�X�e�[�u��*/
        MTH_FIXED(10.0),                /* �ʖ@���x�N�g���̌v�Z�␳�l      */
	&Object6			/* ���I�u�W�F�N�g */
};


/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 0 */
Uint16 fromVertNo0[] = { 0, 1, 2, 3};
Uint16 toVertNo0[]   = { 0, 1, 2, 3};
struct SprInbetInf InbetInf0 = {
    4,				/* ���o�����_��                            */
    &Object0,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo0,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo0,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};

/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 1 */
Uint16 fromVertNo1[] = { 0, 1, 2, 3};
Uint16 toVertNo1[]   = { 4, 5, 6, 7};
struct SprInbetInf InbetInf1 = {
    4,				/* ���o�����_��                            */
    &Object1,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo1,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo1,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};

/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 2 */
Uint16 fromVertNo2[] = { 0, 1, 2, 3};
Uint16 toVertNo2[]   = { 8, 9,10,11};
struct SprInbetInf InbetInf2 = {
    4,				/* ���o�����_��                            */
    &Object2,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo2,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo2,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};

/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 3 */
Uint16 fromVertNo3[] = { 0, 1, 2, 3};
Uint16 toVertNo3[]   = {12,13,14,15};
struct SprInbetInf InbetInf3 = {
    4,				/* ���o�����_��                            */
    &Object3,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo3,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo3,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};

/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 4 */
Uint16 fromVertNo4[] = { 0, 1, 2, 3};
Uint16 toVertNo4[]   = {16,17,18,19};
struct SprInbetInf InbetInf4 = {
    4,				/* ���o�����_��                            */
    &Object4,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo4,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo4,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};

/* �R�c�I�u�W�F�N�g�Ԑڑ��|���S����� 5 */
Uint16 fromVertNo5[] = { 0};
Uint16 toVertNo5[]   = {20};
struct SprInbetInf InbetInf5 = {
    1,				/* ���o�����_��                            */
    &Object5,			/* �ڑ����_�f�[�^���o���I�u�W�F�N�g        */
    fromVertNo5,		/* ���o�����_�ԍ��z��                      */
    &Object6,			/* �ڑ����_�f�[�^�ݒ�I�u�W�F�N�g            */
    toVertNo5,			/* �ݒ蒸�_�ԍ��z��                          */
    NULL,			/* ���R�c�I�u�W�F�N�g�Ԑڑ��|���S�����      */
};


/* �N���X�^ 65 */
SprCluster Cluster65 = {
	65,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
		MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
		MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
		MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
		MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
		MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
		MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData65,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 55 */
SprCluster Cluster55 = {
	55,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster65,	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData55,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 45 */
SprCluster Cluster45 = {
	45,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster55,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData45,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 35 */
SprCluster Cluster35 = {
        35,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster45,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData35,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 25 */
SprCluster Cluster25 = {
        25,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster35,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData25,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 15 */
SprCluster Cluster15 = {
        15,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(90.0),            /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(-WIDTH),          /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster25,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 64 */
SprCluster Cluster64 = {
	64,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData64,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 54 */
SprCluster Cluster54 = {
	54,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster64,	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData54,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 44 */
SprCluster Cluster44 = {
	44,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster54,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData44,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 34 */
SprCluster Cluster34 = {
        34,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster44,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData34,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 24 */
SprCluster Cluster24 = {
        24,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster34,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData24,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 14 */
SprCluster Cluster14 = {
	14,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(-90.0),           /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(WIDTH),           /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	&Cluster15,		    /* ���N���X�^ */
	&Cluster24,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 63 */
SprCluster Cluster63 = {
	63,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData63,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 53 */
SprCluster Cluster53 = {
	53,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster63, 	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData53,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 43 */
SprCluster Cluster43 = {
	43,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster53,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData43,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 33 */
SprCluster Cluster33 = {
        33,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster43,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData33,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 23 */
SprCluster Cluster23 = {
        23,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster33,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData23,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 13 */
SprCluster Cluster13 = {
	13,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(-90.0),           /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(90.0),            /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(-WIDTH),          /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	&Cluster14,		    /* ���N���X�^ */
	&Cluster23,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 62 */
SprCluster Cluster62 = {
	62,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData62,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 52 */
SprCluster Cluster52 = {
	52,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster62,	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData52,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 42 */
SprCluster Cluster42 = {
	42,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster52,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData42,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 32 */
SprCluster Cluster32 = {
        32,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster42,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData32,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 22 */
SprCluster Cluster22 = {
        22,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster32,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData22,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 12 */
SprCluster Cluster12 = {
	12,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(-180.0),          /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(-WIDTH),          /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	&Cluster13,		    /* ���N���X�^ */
	&Cluster22,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 61 */
SprCluster Cluster61 = {
	61,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData61,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 51 */
SprCluster Cluster51 = {
	51,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster61,	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData51,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 41 */
SprCluster Cluster41 = {
	41,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster51,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData41,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 31 */
SprCluster Cluster31 = {
        31,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster41,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData31,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 21 */
SprCluster Cluster21 = {
        21,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster31,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData21,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 11 */
SprCluster Cluster11 = {
	11,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(90.0),            /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(90.0),            /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(WIDTH),           /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	&Cluster12,		    /* ���N���X�^ */
	&Cluster21,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 60 */
SprCluster Cluster60 = {
	60,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object5,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	NULL,  		   	    /* �q�N���X�^ */
        &InbetInf5,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData60,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 50 */
SprCluster Cluster50 = {
	50,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object4,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster60,	   	    /* �q�N���X�^ */
        &InbetInf4,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData50,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 40 */
SprCluster Cluster40 = {
	40,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object3,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster50,  	   	    /* �q�N���X�^ */
        &InbetInf3,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData40,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 30 */
SprCluster Cluster30 = {
        30,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object2,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster40,  		    /* �q�N���X�^ */
        &InbetInf2,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData30,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 20 */
SprCluster Cluster20 = {
        20,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(HEIGHT/5.0),      /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object1,		    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster30,  		    /* �q�N���X�^ */
        &InbetInf1,	 	    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        &clustData20,               /* ���[�U�R���e�L�X�g�G���A            */
};

/* �N���X�^ 10 */
SprCluster Cluster10 = {
	10,                         /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(WIDTH),           /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	&Object0,		    /* �R�c�I�u�W�F�N�g */
	&Cluster11,		    /* ���N���X�^ */
	&Cluster20,  		    /* �q�N���X�^ */
	&InbetInf0,		    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

/* ���[�g�N���X�^ */
SprCluster RootCluster = {
	0,                          /* �N���X�^�ԍ�  */
	ROT_SEQ_ZYX,		    /* ��]���� */
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����w��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����x��] */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ�����y��] */
	},
	{
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂w���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂x���W */
	MTH_FIXED(0.0),             /* ���[���h���W�n�ɂ����錴�_�̂y���W */
	},
	NULL,			    /* �R�c�I�u�W�F�N�g */
	NULL,			    /* ���N���X�^ */
	&Cluster10,  		    /* �q�N���X�^ */
        NULL, 			    /* �R�c�I�u�W�F�N�g�Ԑڑ��|���S�����  */
        NULL,                       /* ���W�ϊ��J�n�O�R�[���o�b�N���[�`��  */
        NULL,                       /* ���W�ϊ��I����R�[���o�b�N���[�`��  */
        NULL,                       /* ���[�U�R���e�L�X�g�G���A            */
};

#include "trape1.txc"

static SprTexture Texture[] = {
    {
/* character no.        */ 0,
/* color mode           */ COLOR_5,
/* color data           */ 0,
/* character width      */ 64,
/* character height     */ 32,
/* character pointer    */ (Uint8 *)trape1tex,
/* lookup table pointer */ NULL
    },
{
/*                      */ 0xffff,
/* stop data            */ COLOR_5,
/* color data           */ 0,
/*                      */ 0,
/*                      */ 0,
/*                      */ NULL,
/*                      */ NULL
    }
};


/*  end of file */
