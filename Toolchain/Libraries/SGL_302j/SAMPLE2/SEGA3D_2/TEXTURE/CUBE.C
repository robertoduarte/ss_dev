#include	".\texture\saturn.c"
#include	".\texture\tails.c"
#include	".\texture\mika.c"

TEXTURE	tex_cube[] = {
	{320, 82, 0x2000, 0x2852},
	{96, 96, 0x39a0, 0x0c60},
	{128, 68, 0x42a0, 0x1044},
};

PICTURE	pic_cube[] = {
	PICDEF(0, COL_32K, saturn_320x82),
	PICDEF(1, COL_32K, tails_96x96),
	PICDEF(2, COL_32K, mika_128x68),
};

