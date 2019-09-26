/*-----------------------------------------------------------------------*/

typedef struct{
	FIXED	cpos[XYZ];
	FIXED	ctarget[XYZ];
	FIXED	pos[XYZ];
	FIXED	scl[XYZ];
	ANGLE	cangle[XYZ];
	ANGLE	ang[XYZ];
}Sega3D, *Sega3DPtr;

/*-----------------------------------------------------------------------*/
void set_texture(PICTURE *pcptr , Uint32 NbPicture)
{
	TEXTURE *txptr;

	for(; NbPicture-- > 0; pcptr++){
		txptr = tex_sample + pcptr->texno;
		slDMACopy((void *)pcptr->pcsrc,
			(void *)(SpriteVRAM + ((txptr->CGadr) << 3)),
			(Uint32)((txptr->Hsize * txptr->Vsize * 4) >> (pcptr->cmode)));
	}
}
/*-----------------------------------------------------------------------*/
void Cel2VRAM( Uint8 *Cel_Data , void *Cell_Adr , Uint32 suu )
{
	Uint32 i;
	Uint8 *VRAM;

	VRAM = (Uint8 *)Cell_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Cel_Data++);
}
void Map2VRAM( Uint16 *Map_Data , void *Map_Adr , Uint16 suuj , Uint16 suui , Uint16 palnum ,Uint32 mapoff)
{
	Uint16 i , j;
	Uint16 paloff;
	Uint16 *VRAM;

	paloff= palnum << 12;
	VRAM = (Uint16 *)Map_Adr;
	

	for( i = 0; i < suui; i++ ) {
		for( j = 0; j < suuj; j++ ) {
			*VRAM++ = (*Map_Data | paloff) + mapoff;
			Map_Data++;
		}
		VRAM += (64 - suuj);
	}
}
void Pal2CRAM( Uint16 *Pal_Data , void *Col_Adr , Uint32 suu )
{
	Uint16 i;
	Uint16 *VRAM;

	VRAM = (Uint16 *)Col_Adr;

	for( i = 0; i < suu; i++ )
		*(VRAM++) = *(Pal_Data++);
}
