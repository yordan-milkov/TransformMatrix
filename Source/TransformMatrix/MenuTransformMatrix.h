#pragma once
#include "StdAfx.h"

#include "DlgTransformMatrix.h"

namespace CreateTransformMatrix
{
	using namespace VWFC::PluginSupport;

	class CMenuTransformMatrix_EventSink : public VWMenu_EventSink
	{
	public:
						CMenuTransformMatrix_EventSink( IVWUnknown* parent );
		virtual			~CMenuTransformMatrix_EventSink();


		virtual void	DoInterface();
	
	private:
		CDlgTransformMatrix::Context	fTransfromDialog; //ensure we use the same instnace for the life of the menu.
	};

	class CExtMenuTransformMatrix : public VWExtensionMenu
	{
		DEFINE_VWMenuExtension;

	public:
					CExtMenuTransformMatrix( CallBackPtr cbp );

		virtual		~CExtMenuTransformMatrix();
	};
}