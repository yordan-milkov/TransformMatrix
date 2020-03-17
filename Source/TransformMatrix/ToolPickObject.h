#pragma once
#include "StdAfx.h"

#include "DlgTransformMatrix.h"

namespace CreateTransformMatrix
{
	using namespace VWFC::PluginSupport;

	class CExtToolPickObject_EventSink : public VWTool_EventSink
	{
	public:
					CExtToolPickObject_EventSink(IVWUnknown* parent);
		virtual		~CExtToolPickObject_EventSink();

	public:
		//virtual bool		DoSetUp(bool bRestore, const IToolModeBarInitProvider* pModeBarInitProvider);
		virtual void		DoSetDown(bool bRestore, const IToolModeBarInitProvider* pModeBarInitProvider);
		virtual void		PointAdded();
		virtual void		MouseMove();
		virtual void		HandleComplete();

	public:
		virtual void		CallbackObjectIndex( InternalIndex& index );

	protected:
		void				Clear();

	protected:
		InternalIndex*		fpResultIndex;
		MCObjectHandle		fPickObject;
	};

	// ------------------------------------------------------------------------------------------------------
	class CExtToolPickObject : public VWExtensionTool
	{
		DEFINE_VWToolExtension;
	public:
		CExtToolPickObject(CallBackPtr cbp);
		virtual			~CExtToolPickObject();

	};
}