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

		enum class	EOulerAnglesOrder
		{
			XYZ,
			XZY,
			YXZ,
			YZX,
			ZXY,
			ZYX
		};
		VWPoint3D	GetOulerAngles( EOulerAnglesOrder order = EOulerAnglesOrder::XYZ );
		void		SetOulerAnglesRotation( const VWPoint3D& angles, EOulerAnglesOrder notation = EOulerAnglesOrder::XYZ );
	};
}