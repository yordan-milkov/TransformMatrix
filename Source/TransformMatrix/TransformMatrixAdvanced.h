#pragma once
#include "StdAfx.h"

namespace CreateTransformMatrix
{
	class TransformMatrixAdvanced : public VWTransformMatrix
	{
	public:
		TransformMatrixAdvanced();
		~TransformMatrixAdvanced();

	public:
		bool		IsAffine();

		enum class	EEulerAnglesOrder
		{
			XYZ = 0,
			XZY,
			YXZ,
			YZX,
			ZXY,
			ZYX,
			X0YX1,
			X0ZX1,
			Y0XY1,
			Y0ZY1,
			Z0XZ1,
			Z0YZ1
		};
		VWPoint3D	GetEulerAngles( EEulerAnglesOrder order = EEulerAnglesOrder::XYZ );
		void		SetEulerAnglesRotation( const VWPoint3D& angles, EEulerAnglesOrder notation = EEulerAnglesOrder::XYZ );
	};
}