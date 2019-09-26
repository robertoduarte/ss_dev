//------------------------------------------------------------------------
//
//	VECTOR.C
//  VECTOR3F math routines
//
//	CONFIDENTIAL
//	Copyright (c) 1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 2/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  2/8/96 - RAB - Initial version
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#include "vector.h"

VECTOR3F vpxu = {  1.0f,  0.0f,  0.0f };
VECTOR3F vpyu = {  0.0f,  1.0f,  0.0f };
VECTOR3F vpzu = {  0.0f,  0.0f,  1.0f };
VECTOR3F vnxu = { -1.0f,  0.0f,  0.0f };
VECTOR3F vnyu = {  0.0f, -1.0f,  0.0f };
VECTOR3F vnzu = {  0.0f,  0.0f, -1.0f };

void VecCopy( VECTOR3F* dest, VECTOR3F* source )
{
	dest->x = source->x;
	dest->y = source->y;
	dest->z = source->z;
}

void VecAdd( VECTOR3F* dest, VECTOR3F* source )
{
	dest->x += source->x;
	dest->y += source->y;
	dest->z += source->z;
}

void VecAAdd( VECTOR3A* dest, VECTOR3A* source )
{
	dest->x += source->x;
	dest->y += source->y;
	dest->z += source->z;
}

void VecSub( VECTOR3F* dest, VECTOR3F* source )
{
	dest->x -= source->x;
	dest->y -= source->y;
	dest->z -= source->z;
}

void VecNeg( VECTOR3F* dest )
{
	dest->x = -dest->x;
	dest->y = -dest->y;
	dest->z = -dest->z;
}

FIXED VecMag( VECTOR3F* source )
{
	FIXED m2;

	m2  = slMulFX( source->x, source->x );
	m2 += slMulFX( source->y, source->y );
	m2 += slMulFX( source->z, source->z );

	return( slSquartFX( m2 ) );
}

void VecNormalize( VECTOR3F* dest )
{
	FIXED mag = VecMag( dest );

	if (mag != toFIXED(0.0))
	{
		dest->x = slDivFX( mag, dest->x );
		dest->y = slDivFX( mag, dest->y );
		dest->z = slDivFX( mag, dest->z );
	}
	else
	{
		dest->x = toFIXED(0.0);
		dest->y = toFIXED(0.0);
		dest->z = toFIXED(0.0);
	}
}

void VecScale( VECTOR3F* dest, FIXED scale )
{
	dest->x = slMulFX( dest->x, scale );
	dest->y = slMulFX( dest->y, scale );
	dest->z = slMulFX( dest->z, scale );
}

void VecSetMag( VECTOR3F* dest, FIXED scale )
{
	VecNormalize( dest );
	VecScale( dest, scale );
}

void VecCross( VECTOR3F* dest, VECTOR3F* a, VECTOR3F* b )
{
	dest->x = slMulFX( a->y, b->z ) - slMulFX( a->z, b->y );
	dest->y = slMulFX( a->z, b->x ) - slMulFX( a->x, b->z );
	dest->z = slMulFX( a->x, b->y ) - slMulFX( a->y, b->x );
}

FIXED VecDot( VECTOR3F* a, VECTOR3F* b )
{
	// this is slInnerProduct, btw

	return( slMulFX( a->x, b->x ) + slMulFX( a->y, b->y ) + slMulFX( a->z, b->z ) );
}

ANGLE VecAzimuth( VECTOR3F* a )
{
	if ((a->x == toFIXED(0.0)) && (a->z == toFIXED(0.0)))
		return( (ANGLE)0 );

	return(slAtan( a->x, a->z ));
}

ANGLE VecElevation( VECTOR3F* a )
{
	FIXED xz;

	xz = slSquartFX( slMulFX( a->x, a->x ) + slMulFX( a->z, a->z ) );
	if ((xz == toFIXED( 0.0 )) && (a->y == toFIXED(0.0)))
		return( (ANGLE)0 );
	
	return( slAtan( xz, a->y ));
}

void VecPolarUnit( VECTOR3F* dest, ANGLE azimuth, ANGLE elevation )
{
	FIXED c = slCos( elevation );
	dest->x = slMulFX( slCos( azimuth ), c ) ;
	dest->z = slMulFX( slSin( azimuth ), c );
	dest->y = slSin( elevation );
}
