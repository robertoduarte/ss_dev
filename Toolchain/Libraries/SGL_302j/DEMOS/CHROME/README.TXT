Chrome/Bump demo
10/11/96


This demo simulates a chrome/bump mapping effect
through the use of gouraud shading on a paletted polygon
and setting up the palette in a specific fashion.



How it works:

First of all, ignore the all Saturn documentation stating that
Gouraud shading does not work with paletted textures/polygons.




Steps to follow:

1) Setup a 32 color gradient in sequential order in a palette bank
	in VDP2 color ram - this will be your gouraud lookup table.
	
2) Setup a paletted polygon referencing the middle color of your
	32 color gradient.
	
3) Apply additive/subtractive RED gouraud shading to have gouraud
	processing reference higher/lower indexes to create 
	your gouraud polygon
	
4) That's it! Easy! 


<< PRECAUTIONS	>>

- Make sure you set the appropriate polygon flags

- Using RED gouraud shading on paletted textures causes the gouraud
	processing to interpolate palette index numbers(not RGB color data).

- Do not use GREEN or BLUE Gouraud offset values

- Here are the flags you need to set, based on whether or not it is textured


	// NON TEXTURED FLAGS
	#define		Chrome_ATR	(MESHoff | CL_Gouraud | CL256Bnk)
	#define		Chrome_OPT	(UseGouraud | UsePalette)
	#define		Chrome_COL	(16)						// middle color in gouraud palette
	#define		Chrome_TEX 	(No_Texture)
	#define		Chrome_POLY	(sprPolygon)


	// TEXTURED FLAGS
	#define		Chrome_ATR	(CL256Bnk|CL_Gouraud|MESHoff|SPdis|ECdis)
	#define		Chrome_OPT	(UseGouraud)
	#define		Chrome_COL	(1024)						// color palette bank offset from base of VDP2 colorram
	#define		Chrome_TEX	(0)
	#define		Chrome_POLY	(sprNoflip)


- This technique works with 16/64/128/256 palette modes but be aware that indexes may
	over/underflow if the index values being used are near the palette boundaries.


<< ADDITIONAL BENEFITS >>

- Since the polygon data is paletted there are a number of nice features that spring up.

- Faster drawing than RGB polys

- Transparency with Scroll screens

- Priority / Color calculation capability

- Highlight can now fall into the interior of the polygon, creating a 
	nice phong-shading style effect.

- Based on your palette setup, your gouraud shading can be non-linear
	since your palette defines the gouraud shading color values.


<< PSUEDO-BUMP MAP >>

Another way to represent the paletted gouraud is what I call a bump-light table.

- Since the neighboring index values (above/below) the one used in a 
	texture are set to represent a brighter or darker version of the one 
	in the texture map being referenced, you can create a bump map effect
	by having your texture reference different gradient sections of your palette
	that ramp in different directions that represent a different reaction
	to the gouraud(light) being applied across the polygon. (Did that make any sense?)


Here's how the bump map demo texture was created:


- It uses 3 index values in a 256 color palette (112)(208)(240)

- The gradients the texture references extend (+/-)16 when RED gouraud is applied.

- The gradients each ramp in a different direction, one gradient having
	its highlight lie in the middle of the table, causing the phong style
	highlight to fall in the polygon's interior.

- The texture has been included with the demo so you can see how the palette
	is setup. (demo_tex.pcx)




More to come.


Chris Coffin
Advanced Development Engineer
SEGA 

Revision A




