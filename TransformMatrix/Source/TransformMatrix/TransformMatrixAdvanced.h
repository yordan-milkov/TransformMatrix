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
		VWPoint3D	GetOulerAngles( size_t notation = 0 );
		void		SetOulerAnglesRotation( const VWPoint3D& angles, size_t notation = 0 );
	};
}