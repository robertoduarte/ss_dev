//------------------------------------------------------------------------
//
//	TEXTURE.H
//	Texture definition structures
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

#include "sgl.h"
#include "texture.h"
#include "assets/test.dat"

TEXTURE tex_set[] ={
    TEXDEF(16, 16, 0),
    TEXDEF(16, 16, 16 * 16 * 1),
    TEXDEF(16, 16, 16 * 16 * 2),
    TEXDEF(16, 16, 16 * 16 * 3),
    TEXDEF(8, 8, 16 * 16 * 4),
    TEXDEF(8, 8, (16 * 16 * 4)+(8 * 8 * 1)), // oy, what a hassle
    TEXDEF(32, 32, (16 * 16 * 4)+(8 * 8 * 2)), // oy, what a hassle
    TEXDEF(16, 16, (32 * 32 * 1)+(16 * 16 * 4)+(8 * 8 * 2)), // oy, what a hassle
};

PICTURE pic_set[] ={
    PICDEF(0, COL_32K, test_tex),
    PICDEF(1, COL_32K, tile_tex),
    PICDEF(2, COL_32K, land0_tex),
    PICDEF(3, COL_32K, ball_tex),
    PICDEF(4, COL_32K, blip_tex),
    PICDEF(5, COL_32K, spark_tex),
    PICDEF(6, COL_32K, target_tex),
    PICDEF(7, COL_32K, spark16_tex),
};

void TextureDMA(PICTURE* pic, int num)
{
    TEXTURE* tex;

    while (num--)
    {
        tex = tex_set + pic->texno;
        slDMACopy((void*) pic->pcsrc,
                  (void*) (SpriteVRAM + (tex->CGadr << 3)),
                  tex->Hsize * tex->Vsize * 4 >> pic->cmode);

        pic++;
    }
}

void TextureInit(void)
{
    TextureDMA(pic_set, 8);
}

