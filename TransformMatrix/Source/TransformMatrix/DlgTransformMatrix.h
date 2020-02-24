#pragma once

#include "TransformMatrixAdvanced.h"

namespace CreateTransformMatrix
{
	class CDlgTransformMatrix : public VWDialog
	{
	public:
								CDlgTransformMatrix();
		virtual					~CDlgTransformMatrix();

		TransformMatrixAdvanced	GetTransform( bool useSlider );
		void					TransformObjects( MCObjectHandle targetContainer = nullptr, bool useSlider = false );

	public :


		// virtuals
	protected:
		virtual bool			CreateDialogLayout();
		virtual void			OnInitializeContent();
		virtual void			OnDDXInitialize();
		virtual void			OnUpdateUI();

		virtual void			OnSetDownEvent();
			

		// dispatch map
	protected:
		DEFINE_EVENT_DISPATH_MAP;
		void					OnListBrowser( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnAddButton( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnRenameButton( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnRemoveButton( Sint32 controlID, VWDialogEventArgs& eventArgs );

		void					OnTypePopup( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnRotationRadio( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnNormalizeAsix( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnScaleEdit( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnSymetricCheck( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnPickObjectButton( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnResultPaneChage( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnOriginPullDown( Sint32 controlID, VWDialogEventArgs& eventArgs );

		// DDX
	protected:
		enum class ETransformType : Sint32
		{
			Rotation	= 0,
			Translation	= 1,
			Scaling		= 2,
			ObjectMat	= 3,
			Result		= 4
		};

		struct SDDXData
		{
			bool			fUse;
			TXString		fName;
			Sint32			fTransformType;
			bool			fInvert;
			bool			fXRotRadio;
			bool			fYRotRadio;
			bool			fZRotRadio;
			bool			fAsixRotRadio;
			VWPoint3D		fAsixRotEdit;
			double			fRotAngleEdit;
			double			fXTranslateEdit;
			double			fYTranslateEdit;
			double			fZTranslateEdit;
			double			fXScaleEdit;
			double			fYScaleEdit;
			double			fZScaleEdit;
			bool			fSymetricScale;
			InternalIndex	fObjectIndex;
			bool			fObjectRotation;
			bool			fObjectTrans;

			SDDXData()
				: fAsixRotEdit( 1, 0, 0 )
			{
				fUse			= true;
				fTransformType	= (Sint32) ETransformType::Result;
				fInvert			= false;
				fXRotRadio		= true;
				fYRotRadio		= false;
				fZRotRadio		= false;
				fAsixRotRadio	= false;
				fRotAngleEdit	= 0;
				fXTranslateEdit	= 0;
				fYTranslateEdit	= 0;
				fZTranslateEdit	= 0;
				fXScaleEdit		= 1;
				fYScaleEdit		= 1;
				fZScaleEdit		= 1;
				fSymetricScale	= false;
				fObjectIndex	= -1;
				fObjectRotation	= true;
				fObjectTrans	= true;
			}
		};
		std::vector< SDDXData >	fDDXControls;
		SDDXData	fResultDDX;
		size_t		fOulerIndexPopup;
		bool		fFormulaView;
		Sint32		fViewMarker;
		Sint32		fRenderMarker;
		bool		fDetailedPreview;
		short		fOriginIndex;
		Sint32		fSliderValue;

		// local functions
	private:
		SDDXData&				GetCurrentDDX();
		void					RebuildDDX();
		void					RebuildListBrowser();
		void					UpdatePanes();
		TransformMatrixAdvanced	GetMatrixForDDX( const SDDXData& data, bool useSlider );
		void					CalculateResultPane();
		void					UpdateMatrixView();
		void					UpdatePreview();
		double					SliderPercent();
		TXString				GetNewName( const TXString& oldName );
		void					ClearPreviewGeometry();

		// Inner data
	private:
		size_t					fSelectedIndex;
		MCObjectHandle			fCacheObject;
		MCObjectHandle			fBoundObject;
		MCObjectHandle			fPreviewObject;
		size_t					fLBBlankImageID;
		size_t					fLBCheckMarkImageID;
	};
}