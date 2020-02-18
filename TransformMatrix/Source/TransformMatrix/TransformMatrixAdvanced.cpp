#include "StdAfx.h"

#include "TransformMatrixAdvanced.h"

using namespace CreateTransformMatrix;


TransformMatrixAdvanced::TransformMatrixAdvanced()
{
}

TransformMatrixAdvanced::~TransformMatrixAdvanced()
{
}

bool TransformMatrixAdvanced::IsAffine()
{
	return true;
}

VWPoint3D TransformMatrixAdvanced::GetOulerAngles( size_t notation /*= 0*/ )
{
	auto		matrixData	= fMatrix.mat;
	VWPoint3D	angles;

	size_t	i0	= ( 0 + notation ) % 3;
	size_t	i1	= ( 1 + notation ) % 3;
	size_t	i2	= ( 2 + notation ) % 3;

	/*bank*/
	if	(	DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], 1 )
		||	DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], -1 ) )
	{
		angles.x	= 0;
	}
	else
	{
		angles.x	= std::atan2( -matrixData[ i1 ][ i2 ], matrixData[ i1 ][ i1 ] );
	}

	/*heading*/
	if	(	DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], 1 )
		||	DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], -1 ) )
	{
		angles.y	= std::atan2( matrixData[ i0 ][ i2 ], matrixData[ i2 ][ i2 ] );
	}
	else
	{
		angles.y	= std::atan2( -matrixData[ i2 ][ i0 ], matrixData[ i0 ][ i0 ] );
	}

	/*attitude*/
	if ( DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], 1 ) )
	{
		angles.z	= PI / 2;
	}
	else if ( DoublesAreNearlyEqual( matrixData[ i1 ][ i0 ], -1 ) )
	{

		angles.z	= -PI / 2;
	}
	else
	{

		angles.z	= std::asin (  matrixData[ i1 ][ i0 ] );
	}

	angles.x	= MathUtils::Rad2Deg( angles.x );
	angles.y	= MathUtils::Rad2Deg( angles.y );
	angles.z	= MathUtils::Rad2Deg( angles.z );

	return angles;
}

void TransformMatrixAdvanced::SetOulerAnglesRotation( const VWPoint3D& angles, size_t notation )
{
	VWPoint3D	savedOffset	= this->GetOffset();
	::SetMultiAxisSpinMatrix( fMatrix, kXAxis, kYAxis, kZAxis, angles.x, angles.y, angles.z, WorldPt3( 0, 0, 0 ), true );
	if ( !VERIFYN( kYKostadinov, savedOffset == this->GetOffset() ) )
	{
		this->SetOffset( savedOffset );
	}
}

