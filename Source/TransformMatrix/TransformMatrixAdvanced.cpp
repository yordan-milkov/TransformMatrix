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
	const auto&		matrixData	= fMatrix.v2;
	return	!DoubleIsNearlyZero( Abs( DotProduct( CrossProduct( matrixData.i, matrixData.j ), matrixData.k ) ) );
}

VWPoint3D TransformMatrixAdvanced::GetEulerAngles( EEulerAnglesOrder order /*= EEulerAnglesOrder::XYZ*/ )
{
	const auto&		matrixData	= fMatrix.v1;
	VWPoint3D	result;

	switch ( order )
	{
		case EEulerAnglesOrder::XYZ:
		{
			if ( matrixData.a02 < +1 )
			{
				if ( matrixData.a02 > -1 )
				{
					result.x = asin ( matrixData.a02 );
					result.y = atan2( -matrixData.a12, matrixData.a22 );
					result.z = atan2( -matrixData.a01, matrixData.a00 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a10, matrixData.a11 )
					result.x = -PI / 2;
					result.y = -atan2( matrixData.a10, matrixData.a11 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a10, matrixData.a11 )
				result.x = +PI / 2;
				result.y = +atan2( matrixData.a10, matrixData.a11 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::XZY:
		{
			if ( matrixData.a01 < +1 )
			{
				if ( matrixData.a01 > -1 )
				{
					result.x = asin ( -matrixData.a01 );
					result.y = atan2( matrixData.a21, matrixData.a11 );
					result.z = atan2( matrixData.a02, matrixData.a00 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a20, matrixData.a22 )
					result.x = +PI / 2;
					result.y = -atan2( -matrixData.a20, matrixData.a22 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a20, matrixData.a22 )
				result.x = -PI / 2;
				result.y = +atan2( -matrixData.a20, matrixData.a22 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::YXZ:
		{
			if ( matrixData.a12 < +1 )
			{
				if ( matrixData.a12 > -1 )
				{
					result.x = asin ( -matrixData.a12 );
					result.y = atan2( matrixData.a02, matrixData.a22 );
					result.z = atan2( matrixData.a10, matrixData.a11 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a01, matrixData.a00 )
					result.x = +PI / 2;
					result.y = -atan2( -matrixData.a01, matrixData.a00 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a01, matrixData.a00 )
				result.x = -PI / 2;
				result.y = +atan2( -matrixData.a01, matrixData.a00 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::YZX:
		{
			if ( matrixData.a10 < +1 )
			{
				if ( matrixData.a10 > -1 )
				{
					result.x = asin ( matrixData.a10 );
					result.y = atan2( -matrixData.a20, matrixData.a00 );
					result.z = atan2( -matrixData.a12, matrixData.a11 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a21, matrixData.a22 )
					result.x = -PI / 2;
					result.y = -atan2( matrixData.a21, matrixData.a22 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a21, matrixData.a22 )
				result.x = +PI / 2;
				result.y = +atan2( matrixData.a21, matrixData.a22 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::ZXY:
		{
			if ( matrixData.a21 < +1 )
			{
				if ( matrixData.a21 > -1 )
				{
					result.x = asin ( matrixData.a21 );
					result.y = atan2( -matrixData.a01, matrixData.a11 );
					result.z = atan2( -matrixData.a20, matrixData.a22 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a02, matrixData.a00 )
					result.x = -PI / 2;
					result.y = -atan2( matrixData.a02, matrixData.a00 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a02, matrixData.a00 )
				result.x = +PI / 2;
				result.y = +atan2( matrixData.a02, matrixData.a00 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::ZYX:
		{
			if ( matrixData.a20 < +1 )
			{
				if ( matrixData.a20 > -1 )
				{
					result.x = asin ( -matrixData.a20 );
					result.y = atan2( matrixData.a10, matrixData.a00 );
					result.z = atan2( matrixData.a21, matrixData.a22 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a12, matrixData.a11 )
					result.x = +PI / 2;
					result.y = -atan2( -matrixData.a12, matrixData.a11 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a12, matrixData.a11 )
				result.x = -PI / 2;
				result.y = +atan2( -matrixData.a12, matrixData.a11 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::X0YX1:
		{
			if ( matrixData.a00 < +1 )
			{
				if ( matrixData.a00 > -1 )
				{
					result.x = acos ( matrixData.a00 );
					result.y = atan2( matrixData.a10, -matrixData.a20 );
					result.z = atan2( matrixData.a01, matrixData.a02 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a12, matrixData.a11 )
					result.x = +PI;
					result.y = -atan2( -matrixData.a12, matrixData.a11 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a12, matrixData.a11 )
				result.x = 0;
				result.y = +atan2( -matrixData.a12, matrixData.a11 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::X0ZX1:
		{
			if ( matrixData.a00 < +1 )
			{
				if ( matrixData.a00 > -1 )
				{
					result.x = acos ( matrixData.a00 );
					result.y = atan2( matrixData.a20, matrixData.a10 );
					result.z = atan2( matrixData.a02, -matrixData.a01 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a21, matrixData.a22 )
					result.x = +PI;
					result.y = -atan2( matrixData.a21, matrixData.a22 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a21, matrixData.a22 )
				result.x = 0;
				result.y = atan2( matrixData.a21, matrixData.a22 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::Y0XY1:
		{
			if ( matrixData.a11 < +1 )
			{
				if ( matrixData.a11 > -1 )
				{
					result.x = acos ( matrixData.a11 );
					result.y = atan2( matrixData.a01, matrixData.a21 );
					result.z = atan2( matrixData.a10, -matrixData.a12 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a02, matrixData.a00 )
					result.x = +PI;
					result.y = -atan2( matrixData.a02, matrixData.a00 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a02, matrixData.a00 )
				result.x = 0;
				result.y = atan2( matrixData.a02, matrixData.a00 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::Y0ZY1:
		{
			if ( matrixData.a11 < +1 )
			{
				if ( matrixData.a11 > -1 )
				{
					result.x = acos ( matrixData.a11 );
					result.y = atan2( matrixData.a21, -matrixData.a01 );
					result.z = atan2( matrixData.a12, matrixData.a10 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a20, matrixData.a22 )
					result.x = +PI;
					result.y = -atan2( -matrixData.a20, matrixData.a22 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a20, matrixData.a22 )
				result.x = 0;
				result.y = atan2( -matrixData.a20, matrixData.a22 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::Z0XZ1:
		{
			if ( matrixData.a22 < +1 )
			{
				if ( matrixData.a22 > -1 )
				{
					result.x = acos ( matrixData.a22 );
					result.y = atan2( matrixData.a02, -matrixData.a12 );
					result.z = atan2( matrixData.a20, matrixData.a21 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( -matrixData.a01, matrixData.a00 )
					result.x = +PI;
					result.y = -atan2( -matrixData.a01, matrixData.a00 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( -matrixData.a01, matrixData.a00 )
				result.x = 0;
				result.y = atan2( -matrixData.a01, matrixData.a00 );
				result.z = 0;
			}
			break;
		}
		case EEulerAnglesOrder::Z0YZ1:
		{
			if ( matrixData.a22 < +1 )
			{
				if ( matrixData.a22 > -1 )
				{
					result.x = acos ( matrixData.a22 );
					result.y = atan2( matrixData.a12, matrixData.a02 );
					result.z = atan2( matrixData.a21, -matrixData.a20 );
				}
				else
				{
					// Not a unique solution: thetaZ-thetaY = atan2( matrixData.a10, matrixData.a11 )
					result.x = +PI;
					result.y = -atan2( matrixData.a10, matrixData.a11 );
					result.z = 0;
				}
			}
			else
			{
				// Not a unique solution: thetaZ + thetaY = atan2( matrixData.a10, matrixData.a11 )
				result.x = 0;
				result.y = atan2( matrixData.a10, matrixData.a11 );
				result.z = 0;
			}
			break;
		}
		default:
			break;
	}

	result.x	= MathUtils::Rad2Deg( result.x );
	result.y	= MathUtils::Rad2Deg( -result.y );
	std::swap( result.x, result.y ); //to match the VW matrix creation.
	result.z	= MathUtils::Rad2Deg( -result.z );

	return result;
}

void TransformMatrixAdvanced::SetEulerAnglesRotation( const VWPoint3D& angles, EEulerAnglesOrder notation /*= EEulerAnglesOrder::XYZ*/ )
{
	MajorAxisSpec	firstAxis	= kXAxis;
	MajorAxisSpec	secondAxis	= kYAxis;
	MajorAxisSpec	thirdAxis	= kZAxis;

	switch ( notation )
	{
		case EEulerAnglesOrder::XYZ:
			firstAxis	= kXAxis;
			secondAxis	= kYAxis;
			thirdAxis	= kZAxis;
			break;
		case EEulerAnglesOrder::XZY:
			firstAxis	= kXAxis;
			secondAxis	= kZAxis;
			thirdAxis	= kYAxis;
			break;
		case EEulerAnglesOrder::YXZ:
			firstAxis	= kYAxis;
			secondAxis	= kXAxis;
			thirdAxis	= kZAxis;
			break;
		case EEulerAnglesOrder::YZX:
			firstAxis	= kYAxis;
			secondAxis	= kZAxis;
			thirdAxis	= kXAxis;
			break;
		case EEulerAnglesOrder::ZXY:
			firstAxis	= kZAxis;
			secondAxis	= kXAxis;
			thirdAxis	= kYAxis;
			break;
		case EEulerAnglesOrder::ZYX:
			firstAxis	= kZAxis;
			secondAxis	= kYAxis;
			thirdAxis	= kXAxis;
			break;
		case EEulerAnglesOrder::X0YX1:
			firstAxis	= kXAxis;
			secondAxis	= kYAxis;
			thirdAxis	= kXAxis;
			break;
		case EEulerAnglesOrder::X0ZX1:
			firstAxis	= kXAxis;
			secondAxis	= kZAxis;
			thirdAxis	= kXAxis;
			break;
		case EEulerAnglesOrder::Y0XY1:
			firstAxis	= kYAxis;
			secondAxis	= kXAxis;
			thirdAxis	= kYAxis;
			break;
		case EEulerAnglesOrder::Y0ZY1:
			firstAxis	= kYAxis;
			secondAxis	= kZAxis;
			thirdAxis	= kYAxis;
			break;
		case EEulerAnglesOrder::Z0XZ1:
			firstAxis	= kZAxis;
			secondAxis	= kXAxis;
			thirdAxis	= kZAxis;
			break;
		case EEulerAnglesOrder::Z0YZ1:
			firstAxis	= kZAxis;
			secondAxis	= kYAxis;
			thirdAxis	= kZAxis;
			break;
		default:
			break;
	}

	VWPoint3D	savedOffset	= this->GetOffset();
	::SetMultiAxisSpinMatrix( fMatrix, firstAxis, secondAxis, thirdAxis, angles.x, -angles.y, angles.z, WorldPt3( 0, 0, 0 ), true );
	if ( !VERIFYN( kYKostadinov, savedOffset == this->GetOffset() ) )
	{
		this->SetOffset( savedOffset );
	}
}

