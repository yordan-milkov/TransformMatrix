#pragma once

#include "TransformMatrixAdvanced.h"

namespace CreateTransformMatrix
{
	class CDlgTransformMatrix : public VWDialog
	{
	public:
		class Context
		{
		public:
			Context();
			virtual		~Context();

			static bool	IsToolActive();
			static void	SetIsToolActive( bool isTempToolActive );

			void		RunDialog();
			void		RunDialog( InternalIndex pickedObjectIndex );

		private:
			static void	AddRef();
			static void	Release();

			static bool						sIsToolActive;
			static Uint32					sRefCnt;
			static CDlgTransformMatrix*		sDlgTransformMatrix;
		};

	protected:
								CDlgTransformMatrix();
		virtual					~CDlgTransformMatrix();

		TransformMatrixAdvanced	GetTransform( bool useSlider );
		void					TransformObject( VWObject handle = nullptr, bool useSlider = false );

	protected:
		void					TransformObjectReq( VWObject& object, const TransformMatrixAdvanced& transform, bool isOrthogonal );

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
		void					OnRenderChnage( Sint32 controlID, VWDialogEventArgs& eventArgs );
		void					OnDetailedPreviewChnage( Sint32 controlID, VWDialogEventArgs& eventArgs );

		// DDX
	protected:
		enum class ETransformType : Sint32
		{
			Rotation	= 0,
			Translation	= 1,
			Scaling		= 2,
			ObjectMat	= 3,
			EulerAngles	= 4,
			Result		= 5
		};

		struct SDDXData
		{
			bool			fUse;
			TXString		fName;
			Sint32			fTransformType;
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
			size_t			fEulerAnglesNotation;
			VWPoint3D		fEulerAnglesEdit;
			bool			fInvert;

			SDDXData()
				: fAsixRotEdit( 1, 0, 0 )
			{
				fUse			= true;
				fTransformType	= (Sint32) ETransformType::Result;
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
				fEulerAnglesNotation = 0;
				fInvert			= false;
			}
		};
		using	TTransformDXXArr	= std::vector< SDDXData >;
		TTransformDXXArr	fDDXControls;
		SDDXData			fResultDDX;
		size_t				fEulerIndexPopup;
		bool				fFormulaView;
		Sint32				fViewMarker;
		Sint32				fRenderMarker;
		bool				fDetailedPreview;
		short				fOriginIndex;
		Sint32				fSliderValue;

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

		// Inner data
	private:
		class CPreviewGeometry
		{
		public:
			CPreviewGeometry();
			~CPreviewGeometry();

		public:
			MCObjectHandle		AddCacheObjectCopy(MCObjectHandle inContainer);
			MCObjectHandle		AddSimplePreviewCopy(MCObjectHandle inContainer);
			MCObjectHandle		AddMeshCopy(MCObjectHandle inContainer);

			WorldCube			GetCacheCube();

			void				Clear();

		protected:
			MCObjectHandle		GetOrGenerateCache();

		private:
			MCObjectHandle		fCacheObject;
			MCObjectHandle		fPreviewObject;
			MCObjectHandle		fMeshObject;
		};

		size_t					fSelectedIndex;
		size_t					fLBBlankImageID;
		size_t					fLBCheckMarkImageID;

		CPreviewGeometry		fPreviewGeometry;
		TransformMatrixAdvanced	fLastTransform;
		bool					fDetailChange;
		bool					fRenderChange;
	};
}