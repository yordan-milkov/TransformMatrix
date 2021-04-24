#include "StdAfx.h"

#include "DlgTransformMatrix.h"

#include "ToolPickObject.h"

using namespace CreateTransformMatrix;
// dialog control IDs
enum ETransformMatrixDlg {
	kOK                      = 1,
	kCancel                  = 2,
	kGenerateTranGrp         = 4,
	kOriginStatic            = 5,
	kOriginPopup             = 6,
	kTransformsList          = 7,
	kAddButton               = 8,
	kRenameButton            = 9,
	kRemoveButton            = 10,
	kTransformGrp            = 11,
	kResultSwap              = 12,
	kTransformPane           = 13,
	kNameStatic              = 14,
	kTransformTypeGrp        = 15,
	kTypePopup               = 16,
	kTransformSwap           = 17,
	kRatationPane            = 18,
	kRotationAsixGrp         = 19,
	kXRotRadio               = 20,
	kYRotRadio               = 21,
	kZRotRadio               = 22,
	kAsixRotRadio            = 23,
	kAxisRotStatic           = 24,
	kXRotStatic              = 25,
	kXRotEdit                = 26,
	kYRotStatic              = 27,
	kYRotEdit                = 28,
	kZRotStatic              = 29,
	kZRotEdit                = 30,
	kNormAsixBtn             = 31,
	kRotAngleStatic          = 32,
	kRotAngleEdit            = 33,
	kTranslationPane         = 34,
	kTranslationCoordsStatic = 35,
	kXTranslateStatic        = 36,
	kXTranslateEdit          = 37,
	kYTranslateStatic        = 38,
	kYTranslateEdit          = 39,
	kZTranslateStatic        = 40,
	kZTranslateEdit          = 41,
	kScalePane               = 42,
	kScaleFactorsStatic      = 43,
	kXScaleStatic            = 44,
	kXScaleEdit              = 45,
	kYScaleStatic            = 46,
	kYScaleEdit              = 47,
	kZScaleStatic            = 48,
	kZScaleEdit              = 49,
	kSymetricScale           = 50,
	kObjectPane              = 51,
	kObjectNameStatic        = 52,
	kPickObjectButton        = 53,
	kObjectRotationCheck     = 54,
	kObjectTranslationCheck  = 55,
	kEulerPane               = 56,
	kEulerEditGroup          = 57,
	kXEulerStatic            = 58,
	kXEulerEdit              = 59,
	kYEulerStatic            = 60,
	kYEulerEdit              = 61,
	kZEulerStatic            = 62,
	kZEulerEdit              = 63,
	kResultPane              = 64,
	kPropertiesStatic        = 65,
	kEulerResultGroup        = 66,
	kXEulerResultStatic      = 67,
	kYEulerResultStatic      = 68,
	kZEulerResultStatic      = 69,
	kInvertCheck             = 70,
	kMatrixView              = 71,
	kMatrix0RC               = 72,
	kMatrixXCol              = 73,
	kMatrixYCol              = 74,
	kMatrixZCol              = 75,
	kMatrixURow              = 76,
	kMatrixUX                = 77,
	kMatrixUY                = 78,
	kMatrixUZ                = 79,
	kMatrixVRow              = 80,
	kMatrixVX                = 81,
	kMatrixVY                = 82,
	kMatrixVZ                = 83,
	kMatrixWRow              = 84,
	kMatrixWX                = 85,
	kMatrixWY                = 86,
	kMatrixWZ                = 87,
	kMatrixORow              = 88,
	kMatrixOX                = 89,
	kMatrixOY                = 90,
	kMatrixOZ                = 91,
	kFormulaView             = 92,
	kPreviewGrp              = 93,
	kPreview                 = 94,
	kViewStatic              = 95,
	kViewPopup               = 96,
	kRenderStatic            = 97,
	kRenderPopup             = 98,
	kDetailedPreviewCheck    = 99,
	kSlider                  = 100,
};


/*static*/ bool						CDlgTransformMatrix::Context::sIsToolActive			= false;
/*static*/ Uint32					CDlgTransformMatrix::Context::sRefCnt				= 0;
/*static*/ CDlgTransformMatrix*		CDlgTransformMatrix::Context::sDlgTransformMatrix	= nullptr;

CDlgTransformMatrix::Context::Context()
{
	if	(	sDlgTransformMatrix == nullptr
		&&	VERIFYN( kYKostadinov, sRefCnt == 0 ) )
	{
		sDlgTransformMatrix	= new CDlgTransformMatrix();
	}
	CDlgTransformMatrix::Context::AddRef();
}

CDlgTransformMatrix::Context::~Context()
{
	this->Release();
}

bool CDlgTransformMatrix::Context::IsToolActive()
{
	return sIsToolActive;
}

void CDlgTransformMatrix::Context::SetIsToolActive( bool isTempToolActive )
{
	if ( isTempToolActive )
	{
		if ( sIsToolActive == false )
		{
			CDlgTransformMatrix::Context::AddRef();
			gSDK->RunTempTool( CExtToolPickObject::_GetIID() );
		}
		else
		{
			gSDK->PopLastToolPt();
		}
	}

	if ( !isTempToolActive && VERIFYN( kYKostadinov, sIsToolActive == true ) )
	{
		CDlgTransformMatrix::Context::Release();
	}
	sIsToolActive	= isTempToolActive;
}


void CDlgTransformMatrix::Context::AddRef()
{
	sRefCnt++;
}

void CDlgTransformMatrix::Context::Release()
{
	if ( sRefCnt > 0 )
		sRefCnt--;

	if ( sRefCnt == 0 && VERIFYN( kYKostadinov, sDlgTransformMatrix ) )
	{
		delete sDlgTransformMatrix;
		sDlgTransformMatrix = nullptr;
	}
}

void CDlgTransformMatrix::Context::RunDialog()
{
	if ( VERIFYN( kYKostadinov, sDlgTransformMatrix ) )
	{
		if ( gSDK->NumSelectedObjects() > 0 )
		{
			gSDK->SupportUndoAndRemove();
			gSDK->SetUndoMethod( kUndoSwapObjects );
			if ( sDlgTransformMatrix->RunDialogLayout( "" ) == kDialogButton_Ok )
			{
				sDlgTransformMatrix->TransformObject();
				gSDK->RedrawRect( WorldRect() );
				gSDK->EndUndoEvent();
			}
			else
			{
				gSDK->UndoAndRemove();
			}
		}
		else
		{
			gSDK->AlertCritical( TXResStr( "MenuTransformMatrix", "SelectObjectAlert" ) );
		}
	}
}

void CDlgTransformMatrix::Context::RunDialog( InternalIndex pickedObjectIndex )
{
	if ( VERIFYN( kYKostadinov, sDlgTransformMatrix ) )
	{
		auto& currentDDX	= sDlgTransformMatrix->GetCurrentDDX();
		if ( VERIFYN( kYKostadinov, currentDDX.fTransformType == (Sint32) ETransformType::ObjectMat ) )
		{
			currentDDX.fObjectIndex	= pickedObjectIndex;
		}
		this->RunDialog();
	}
}



// ------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
const Sint32	kColNumber	= 0;
const Sint32	kColUse		= 1;
const Sint32	kColName	= 2;
const Sint32	kRowSep		= 100;
const Sint32	kRowResult	= 101;

const TXString kChecked("Yes");
const TXString kUnchecked("No");

EVENT_DISPATCH_MAP_BEGIN( CDlgTransformMatrix );
ADD_DISPATCH_EVENT( kTransformsList	, OnListBrowser );
ADD_DISPATCH_EVENT( kAddButton		, OnAddButton );
ADD_DISPATCH_EVENT( kRenameButton	, OnRenameButton );
ADD_DISPATCH_EVENT( kRemoveButton	, OnRemoveButton );

ADD_DISPATCH_EVENT( kTypePopup		, OnTypePopup );
ADD_DISPATCH_EVENT( kXRotRadio		, OnRotationRadio );
ADD_DISPATCH_EVENT( kYRotRadio		, OnRotationRadio );
ADD_DISPATCH_EVENT( kZRotRadio		, OnRotationRadio );
ADD_DISPATCH_EVENT( kNormAsixBtn	, OnNormalizeAsix );
ADD_DISPATCH_EVENT( kXScaleEdit		, OnScaleEdit );
ADD_DISPATCH_EVENT( kYScaleEdit		, OnScaleEdit );
ADD_DISPATCH_EVENT( kZScaleEdit		, OnScaleEdit );
ADD_DISPATCH_EVENT( kSymetricScale	, OnSymetricCheck );
ADD_DISPATCH_EVENT( kPickObjectButton,OnPickObjectButton );

ADD_DISPATCH_EVENT( kEulerResultGroup,OnResultPaneChage );
ADD_DISPATCH_EVENT( kViewPopup		, OnRenderChnage );
ADD_DISPATCH_EVENT( kRenderPopup	, OnRenderChnage );
ADD_DISPATCH_EVENT( kDetailedPreviewCheck, OnDetailedPreviewChnage );
EVENT_DISPATCH_MAP_END;

CDlgTransformMatrix::CDlgTransformMatrix()
	: fResultDDX()
	, fSelectedIndex( (size_t) -1 )
	, fFormulaView( false )
	, fViewMarker( standardViewLeftIso )
	, fRenderMarker( renderOpenGL )
	, fDetailedPreview( true )
	, fOriginIndex( 0 )
	, fSliderValue( 100 )
{
	// Assign settings tag to this dialog so it would save it's position and size automatically
	this->SetSavedSettingsTag( "TransformMatrix", "TransformMatrixDlg" );
}

CDlgTransformMatrix::~CDlgTransformMatrix()
{
}

TransformMatrixAdvanced CDlgTransformMatrix::GetTransform( bool useSlider )
{
	TransformMatrixAdvanced	result;
	
	WorldPt3	offset;
	if ( fOriginIndex )
	{
		WorldCube	boundCube	= fPreviewGeometry.GetCacheCube();
		switch ( fOriginIndex )
		{
			case 1:
			{
				offset	= boundCube.PointXYZ();
				break;
			}
			case 2:
			{
				offset	= boundCube.PointXYz();
				break;
			}
			case 3:
			{
				offset	= boundCube.PointXyZ();
				break;
			}
			case 4:
			{
				offset	= boundCube.PointXyz();
				break;
			}
			case 5:
			{
				offset	= boundCube.PointxYZ();
				break;
			}
			case 6:
			{
				offset	= boundCube.PointxYz();
				break;
			}
			case 7:
			{
				offset	= boundCube.PointxyZ();
				break;
			}
			case 8:
			{
				offset	= boundCube.Pointxyz();
				break;
			}
			case 9:
			{
				offset	= boundCube.Center();
				break;
			}
			default:
				break;
		}
		result.SetMatrix( -offset );
	}
	
	for ( const SDDXData& item : fDDXControls )
	{
		if ( item.fUse )
		{
			result	*= this->GetMatrixForDDX( item, useSlider );
		}
	}

	if ( fOriginIndex )
	{
		result	*= VWTransformMatrix( offset );
	}

	if ( fResultDDX.fInvert )
	{
		result.Invert();
	}

	return result;
}

void CDlgTransformMatrix::TransformObject( VWObject handle /*= nullptr*/, bool useSlider /*= false*/ )
{
	TransformMatrixAdvanced	transform	= this->GetTransform( useSlider );
	bool	isOrthogonalTransform	= transform.fMatrix.IsOrthogonal();

	if ( handle )
	{
		this->TransformObjectReq( handle, transform, isOrthogonalTransform );
	}
	else
	{
		if ( !isOrthogonalTransform )
		{
			gSDK->AlertInform( TXResStr( "TransformMatrixDlg", "NonOrtogonalAlert" ), "", false, fstrDialogCategory, "NonOrthogonalInform" );
		}

		gSDK->ForEachObjectN( allSelectedAndEditable,
			[ & ] ( VWObject obj )
			{
				gSDK->AddBothSwapObject( obj );
				this->TransformObjectReq( obj, transform, isOrthogonalTransform );
			} );
	}
}

void CDlgTransformMatrix::TransformObjectReq( VWObject& object, const TransformMatrixAdvanced& transform, bool isOrthogonal )
{
	short prop = gSDK->GetObjTypeProperties( object.GetType() );
	if ( ( prop & prGraphicObject ) != 0 )
	{
		if ( isOrthogonal )
		{
			if ( object.GetType() == kGroupNode )
			{
				for ( VWObject currObj : VWGroupObj( object ) )
				{
					this->TransformObjectReq( currObj, transform, isOrthogonal );
				}
			}
#if 1 //prior VW2021
			else if ( object.GetType() == kSolidNode )
			{
				object.ApplyObjectMatrix( transform );
			}
			else
			{
				object.TransformObject( transform, true, true );
			}
#else
			else
			{
				object.TransformObjectN( transform );
			}
#endif
		}
		else
		{
			if ( object.GetType() != kMeshNode )
			{
				object	= gSDK->ConvertTo3DPolygons( object );
				gSDK->AddAfterSwapObject( object );
				gSDK->GroupToMesh( object );
			}

			if ( object.GetType() == kMeshNode )
			{
				size_t	vertCount	= gSDK->GetMeshVertexCount( object );
				for ( size_t vertexIndex = 0; vertexIndex < vertCount; vertexIndex++ )
				{
					VWPoint3D	vertexPt	= gSDK->GetMeshVertexAt( object, vertexIndex );
					transform.TransformPoint( vertexPt );
					gSDK->SetMeshVertexAt( object, vertexIndex, vertexPt );
				}
			}
		}
		object.ResetObject();
	}
}

bool CDlgTransformMatrix::CreateDialogLayout()
{
	return this->CreateDialogLayoutFromVWR( "DebugModule/DialogLayout/TransformMatrixDlg.vs" );
}

void CDlgTransformMatrix::OnInitializeContent()
{
	VWDialog::OnInitializeContent();

	// Generate
	VWListBrowserCtrl*	pListBrowser	= this->GetListBrowserCtrlByID( kTransformsList );
	pListBrowser->EnableDragAndDrop( true );
	pListBrowser->EnableSingleLineSelection( true );
	pListBrowser->EnableSorting( false );

	VWListBrowserColumn	orderColumn	= pListBrowser->AddColumn( TXResStr( "TransformMatrixDlg", "#Col" ), 20 );
	orderColumn.SetColumnType		( EListBrowserControlType::kListBrowserControlNumber );
	orderColumn.SetAsDragAndDropColumn();

	VWListBrowserColumn useColumn	= pListBrowser->AddColumn( TXResStr( "TransformMatrixDlg", "Use" ), 40 );
	useColumn.SetColumnType			( EListBrowserControlType::kListBrowserControlMultiState );
	useColumn.SetItemDisplayType	( EListBrowserDisplayType::kListBrowserDisplayImageOnly );
	fLBBlankImageID					= pListBrowser->AddImageStandard( EStandardImageID::kStandardImageID_Blank );
	fLBCheckMarkImageID				= pListBrowser->AddImageStandard( EStandardImageID::kStandardImageID_CheckMark );
	useColumn.InsertColumnChoice	( kChecked	, fLBCheckMarkImageID );
	useColumn.InsertColumnChoice	( kUnchecked, fLBBlankImageID );

	VWListBrowserColumn	nameColumn	= pListBrowser->AddColumn( TXResStr( "TransformMatrixDlg", "NameCol" ), 80 );

	this->RebuildListBrowser();

	VWPullDownMenuCtrl*	pTranformTypePopup	= this->GetPullDownMenuCtrlByID( kTypePopup );
	size_t	append	= size_t( -1 );
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "Rotation" )	, append, (Sint32)ETransformType::Rotation		);
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "Translation" ), append, (Sint32)ETransformType::Translation	);
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "Scaling" )	, append, (Sint32)ETransformType::Scaling		);
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "EulerAngles" ), append, (Sint32)ETransformType::EulerAngles	);
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "ObjectMatrix" ),append, (Sint32)ETransformType::ObjectMat		);

	fEulerIndexPopup	= 0;
	auto fillEulerPulldown = [ & ] ( TControlID controlID )
	{
		VWPullDownMenuCtrl* pEulerPopup	= this->GetPullDownMenuCtrlByID( controlID );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "XYZ" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "XZY" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "YXZ" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "YZX" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "ZXY" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "ZYX" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "X0YX1" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "X0ZX1" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Y0XY1" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Z0YZ1" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Y0ZY1" ) );
		pEulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Z0XZ1" ) );
	};
	fillEulerPulldown( kEulerEditGroup );
	fillEulerPulldown( kEulerResultGroup );

	VWPullDownMenuCtrl* pViewPopup	= this->GetPullDownMenuCtrlByID( kViewPopup );
	for ( Sint32 currViewMarker = standardViewFront; currViewMarker <= standardViewBottomLeftRearIso; currViewMarker++ )
	{
		pViewPopup->AddItem( TXResStr( "TransformMatrixDlg", TXString( "View" ) << currViewMarker ), append, currViewMarker );
	}

	VWPullDownMenuCtrl* pRenderPopup	= this->GetPullDownMenuCtrlByID( kRenderPopup );
	pRenderPopup->AddItem( TXResStr( "TransformMatrixDlg", "RenderOGL" )	, append, renderOpenGL );
	pRenderPopup->AddItem( TXResStr( "TransformMatrixDlg", "RenderWire" )	, append, renderWireFrame );
	pRenderPopup->AddItem( TXResStr( "TransformMatrixDlg", "RenderHidden" )	, append, renderFinalHiddenLine );

	VWPullDownMenuCtrl* pOriginPopup	= this->GetPullDownMenuCtrlByID( kOriginPopup );
	for ( short currOrigin = 0; currOrigin <= 9; currOrigin++ )
	{
		pOriginPopup->AddItem( TXResStr( "TransformMatrixDlg", TXString( "Bound" ) << currOrigin ) );
	}

	fRenderChange	= true;
	fDetailChange	= true;
	this->GetSliderCtrlByID( kSlider )->SetSliderLiveUpdate( true );
}

void CDlgTransformMatrix::OnDDXInitialize()
{
	this->RebuildDDX();
}

void CDlgTransformMatrix::OnUpdateUI()
{
	const SDDXData& currentData	= this->GetCurrentDDX();

	bool	isMatrixSelected	= ( fSelectedIndex != (size_t) -1 );
	this->EnableControl( kRenameButton	, isMatrixSelected );
	this->EnableControl( kRemoveButton	, isMatrixSelected );
	this->EnableControl( kFormulaView	, isMatrixSelected );

	ETransformType		type	= ETransformType(currentData.fTransformType);
	switch ( type )
	{
		case ETransformType::Result:
		{
			break;
		}
		case ETransformType::Rotation:
		{
			this->EnableControl( kXRotEdit, currentData.fAsixRotRadio );
			this->EnableControl( kYRotEdit, currentData.fAsixRotRadio );
			this->EnableControl( kZRotEdit, currentData.fAsixRotRadio );
			this->EnableControl( kNormAsixBtn, currentData.fAsixRotRadio );
			break;
		}
		case ETransformType::Translation:
		{
			break;
		}
		case ETransformType::Scaling:
		{
			break;
		}
		default:
			break;
	}

	this->UpdateMatrixView();
	this->UpdatePreview();

	VWDialog::OnUpdateUI();
}

void CDlgTransformMatrix::OnSetDownEvent()
{
	fPreviewGeometry.Clear();
	VWDialog::OnSetDownEvent();
}

void CDlgTransformMatrix::OnListBrowser( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	VWListBrowserCtrl*	pListBrowser	= this->GetListBrowserCtrlByID( kTransformsList );

	size_t	rowIndex, colIndex;
	EListBrowserEventType	eventType	= eventArgs.GetListBrowserEvent( rowIndex, colIndex );
	if ( eventArgs.IsListBrowserDrawNDropEnd() )
	{
#if 0
		// the rowIndex holds the dragged index, the first selected is the drop index
		size_t	dragIndex	= rowIndex;
		size_t	dropIndex	= pListBrowser->GetFirstSelected();
		if ( dragIndex < fDDXControls.size() )
		{
			dropIndex	= ( dropIndex < fDDXControls.size() ) ? dropIndex : fDDXControls.size() - 1;
			if ( dropIndex != dragIndex )
			{
				auto&	moveFrom	= fDDXControls.begin() + dragIndex;
				auto&	moveTo		= fDDXControls.begin() + dropIndex;
				if ( moveFrom < moveTo )
				{
					std::rotate( moveFrom, moveFrom + 1, moveTo + 1 );
				}
				else if ( moveFrom > moveTo )
				{
					std::rotate( moveTo, moveFrom, moveFrom + 1 );
				}
				fSelectedIndex	= dropIndex;
				this->RebuildDDX();
				this->UpdateData( true );
			}
		}
#else
		size_t	currSelIndex	= pListBrowser->GetFirstSelected();
		TTransformDXXArr	copyDDX;
		size_t				rowsCount	= pListBrowser->GetRowsCount();
		for ( size_t index = 0; index < rowsCount; index++ )
		{
			TXString	transformName	= pListBrowser->GetItem( index, 2 ).GetItemText();
			for ( auto item : fDDXControls )
			{
				if ( item.fName == transformName )
				{
					if ( index == currSelIndex )
					{
						fSelectedIndex	= copyDDX.size();
						currSelIndex	= size_t( -1 );
					}
					copyDDX.push_back( item );
					break;

				}
			}
		}
		fDDXControls	= copyDDX;

		this->RebuildDDX();
		this->UpdateData( true );
#endif
		this->RebuildListBrowser();
	}
	else if ( eventType == kListBrowserEventType_SelectionChangeClick )
	{
		Sint32	rowMarker	= pListBrowser->GetRowMarker( rowIndex );
		if ( rowMarker == kRowResult || rowMarker == kRowSep )
		{
			if ( rowMarker == kRowSep )
			{
				pListBrowser->DeselectAll();
				pListBrowser->SelectRow( pListBrowser->GetRowWithMarker( kRowResult ), true );
			}
			fSelectedIndex	= (size_t) -1;
		}
		else
		{
			fSelectedIndex	= rowIndex;
		}
		this->RebuildDDX();
		this->UpdateData( true );
	}
	else if ( eventType == kListBrowserEventType_DataChangeClick )
	{
		Sint32	rowMarker	= pListBrowser->GetRowMarker( rowIndex );
		if ( rowMarker == kRowResult || rowMarker == kRowSep )
		{
			if ( rowMarker == kRowSep )
			{
				pListBrowser->DeselectAll();
				pListBrowser->SelectRow( pListBrowser->GetRowWithMarker( kRowResult ), true );
			}
			fSelectedIndex	= (size_t) -1;
			pListBrowser->GetItem( rowIndex, kColUse ).SetItemImage( fLBBlankImageID );
		}
		else
		{
			fSelectedIndex				= rowIndex;
			this->GetCurrentDDX().fUse	= ( pListBrowser->GetItem( rowIndex, kColUse ).GetItemImage() == fLBCheckMarkImageID );
		}
		this->RebuildDDX();
		this->UpdateData( true );
	}
	else if ( eventType == 50 )
	{
	}
}

void CDlgTransformMatrix::OnTypePopup( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	this->UpdatePanes();
}

void CDlgTransformMatrix::OnRotationRadio( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	SDDXData& currentData		= this->GetCurrentDDX();
	currentData.fAsixRotEdit.x	= ( controlID == kXRotRadio ) ? 1 : 0;
	currentData.fAsixRotEdit.y	= ( controlID == kYRotRadio ) ? 1 : 0;
	currentData.fAsixRotEdit.z	= ( controlID == kZRotRadio ) ? 1 : 0;
	this->UpdateData( true );
}

void CDlgTransformMatrix::OnNormalizeAsix( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	SDDXData& currentData		= this->GetCurrentDDX();
	currentData.fAsixRotEdit.Normalize();
	this->UpdateData( true );
}

void CDlgTransformMatrix::OnScaleEdit( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	SDDXData& currentData		= this->GetCurrentDDX();
	if ( currentData.fSymetricScale )
	{
		double	value	= this->GetEditRealCtrlByID( controlID )->GetEditReal( VWEditRealCtrl::kEditControlReal );
		currentData.fXScaleEdit		= value;
		currentData.fYScaleEdit		= value;
		currentData.fZScaleEdit		= value;
		this->UpdateData( true );
	}
}

void CDlgTransformMatrix::OnSymetricCheck( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	this->OnScaleEdit( kXScaleEdit, eventArgs );
}

void CDlgTransformMatrix::OnPickObjectButton( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	Context::SetIsToolActive( true );

	this->SetDialogClose( false );
}

void CDlgTransformMatrix::OnResultPaneChage( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	this->CalculateResultPane();
}

void CDlgTransformMatrix::OnRenderChnage( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	fRenderChange	= true;
}

void CDlgTransformMatrix::OnDetailedPreviewChnage( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	fDetailChange	= true;
}

void CDlgTransformMatrix::OnAddButton(Sint32 controlID, VWDialogEventArgs& eventArgs)
{
	TXString	newName	= this->GetNewName( "" );
	if ( !newName.IsEmpty() )
	{
		fSelectedIndex		= fDDXControls.size();
		fDDXControls.push_back( SDDXData() );
		SDDXData&	newData = fDDXControls.back();
		newData.fName			= newName;
		newData.fTransformType	= (Sint32) ETransformType::Rotation;

		this->RebuildDDX();
		this->RebuildListBrowser();
		this->UpdateData( true );
	}
}

void CDlgTransformMatrix::OnRenameButton( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	if ( fSelectedIndex != (size_t) -1 )
	{
		TXString&	matrixName	= fDDXControls[ fSelectedIndex ].fName;
		TXString	newName	= this->GetNewName( matrixName );
		if ( !newName.IsEmpty() )
		{
			matrixName	= newName;
			this->RebuildDDX();
			this->RebuildListBrowser();
		}
	}
}

void CDlgTransformMatrix::OnRemoveButton(Sint32 controlID, VWDialogEventArgs& eventArgs)
{
	if ( EDialogButton::kDialogButton_Ok == VWDialog::AlertQuestion
			( TXResStr( "TransformMatrixDlg", "DeleteAlert" ), TXResStr( "TransformMatrixDlg", "DeleteInform" ) ) )
	{
		fDDXControls.erase( fDDXControls.begin() + fSelectedIndex );
		fSelectedIndex	= (size_t) -1;

		this->RebuildDDX();
		this->RebuildListBrowser();
		this->UpdateData( true );
	}
}

CDlgTransformMatrix::SDDXData& CDlgTransformMatrix::GetCurrentDDX()
{
	return ( fSelectedIndex == (size_t) -1 || fSelectedIndex > fDDXControls.size() ) ? fResultDDX : fDDXControls.at( fSelectedIndex );
}

void CDlgTransformMatrix::RebuildDDX()
{
	this->ClearDDX();

	SDDXData& currentData	= this->GetCurrentDDX();

	if ( ETransformType( currentData.fTransformType ) != ETransformType::Result )
	{
		this->AddDDX_PulldownMenu(kTypePopup			, &currentData.fTransformType	);
		this->AddDDX_RadioButton( kXRotRadio			, &currentData.fXRotRadio		);
		this->AddDDX_RadioButton( kYRotRadio			, &currentData.fYRotRadio		);
		this->AddDDX_RadioButton( kZRotRadio			, &currentData.fZRotRadio		);
		this->AddDDX_RadioButton( kAsixRotRadio			, &currentData.fAsixRotRadio	);
		this->AddDDX_EditReal	( kXRotEdit				, &currentData.fAsixRotEdit.x	, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kYRotEdit				, &currentData.fAsixRotEdit.y	, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kZRotEdit				, &currentData.fAsixRotEdit.z	, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kRotAngleEdit			, &currentData.fRotAngleEdit	, VWEditRealCtrl::kEditControlAngle );
		this->AddDDX_EditReal	( kXTranslateEdit		, &currentData.fXTranslateEdit	, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kYTranslateEdit		, &currentData.fYTranslateEdit	, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kZTranslateEdit		, &currentData.fZTranslateEdit	, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kXScaleEdit			, &currentData.fXScaleEdit		, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kYScaleEdit			, &currentData.fYScaleEdit		, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kZScaleEdit			, &currentData.fZScaleEdit		, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_CheckButton( kSymetricScale		, &currentData.fSymetricScale	);
		this->AddDDX_CheckButton( kObjectRotationCheck	, &currentData.fObjectRotation	);
		this->AddDDX_CheckButton( kObjectTranslationCheck,&currentData.fObjectTrans		);
		this->AddDDX_EditReal	( kXEulerEdit			, &currentData.fEulerAnglesEdit.x, VWEditRealCtrl::kEditControlAngle );
		this->AddDDX_EditReal	( kYEulerEdit			, &currentData.fEulerAnglesEdit.y, VWEditRealCtrl::kEditControlAngle );
		this->AddDDX_EditReal	( kZEulerEdit			, &currentData.fEulerAnglesEdit.z, VWEditRealCtrl::kEditControlAngle );
		this->AddDDX_PulldownMenu(kEulerEditGroup		, &currentData.fEulerAnglesNotation );
	}

	this->AddDDX_PulldownMenu( kEulerResultGroup, &fEulerIndexPopup );
	this->AddDDX_CheckButton ( kInvertCheck		, &currentData.fInvert );
	this->AddDDX_CheckButton ( kFormulaView		, &fFormulaView );

	this->UpdatePanes();

	this->AddDDX_PulldownMenu(kViewPopup			, &fViewMarker );
	this->AddDDX_PulldownMenu(kRenderPopup			, &fRenderMarker );
	this->AddDDX_CheckButton( kDetailedPreviewCheck	, &fDetailedPreview );
	this->AddDDX_PulldownMenu(kOriginPopup			, &fOriginIndex );
	this->AddDDX_Slider		( kSlider				, &fSliderValue );
}

void CDlgTransformMatrix::RebuildListBrowser()
{
	VWListBrowserCtrl*	pListBrowser	= this->GetListBrowserCtrlByID( kTransformsList );
	pListBrowser->DeleteAllRows();

	for ( size_t i = 0; i < fDDXControls.size(); i++ )
	{
		const SDDXData& currItem	= fDDXControls[ i ];
		size_t rowIndex	= pListBrowser->AddRow( TXString::Formatted( "%d", i ) );
		pListBrowser->GetItem( rowIndex, kColUse )	.SetItemImage	( currItem.fUse ? fLBCheckMarkImageID : fLBBlankImageID );
		pListBrowser->GetItem( rowIndex, kColName )	.SetItemText	( currItem.fName );
	}

	pListBrowser->AddRow( "-", kRowSep );

	size_t	resultRowIndex	= pListBrowser->AddRow( " ", kRowResult );
	pListBrowser->GetItem( resultRowIndex, kColName ).SetItemText( TXResStr( "TransformMatrixDlg", "ResultRow" ) );

	pListBrowser->SelectRow( ( ( fSelectedIndex == (size_t) -1 ) ? resultRowIndex : fSelectedIndex ), true );
}

void CDlgTransformMatrix::UpdatePanes()
{
	VWSwapCtrl*		resultSwap		= this->GetSwapCtrlByID( kResultSwap );
	if ( fSelectedIndex == (size_t) -1 )
	{
		resultSwap->SetActivePaneIndex( 1 );

		TXString	groupLabel	= TXResStr( "TransformMatrixDlg", "ResultRow" );
		this->GetGroupBoxCtrlByID( kTransformGrp )->SetControlText( groupLabel );

		this->CalculateResultPane();
	}
	else
	{
		resultSwap->SetActivePaneIndex( 0 );

		TXString	groupLabel	= TXResStr( "TransformMatrixDlg", "kTransformGrp" );
		this->GetGroupBoxCtrlByID( kTransformGrp )->SetControlText( groupLabel );

		SDDXData&		currentData			= this->GetCurrentDDX();
		TXString		transformNameLabel	= TXResStr( "TransformMatrixDlg", "kNameStatic" );
		transformNameLabel.Replace( "^1", currentData.fName );
		this->GetStaticTextCtrlByID( kNameStatic )->SetControlText( transformNameLabel );

		ETransformType	transformType		= ETransformType( currentData.fTransformType );
		if ( transformType == ETransformType::ObjectMat )
		{
			TXString		objNameLabel	= TXResStr( "TransformMatrixDlg", "kObjectNameStatic" );
			MCObjectHandle	object;
			TXString		replaceText		=	(	currentData.fObjectIndex > 0
												&&	( object = VWObject::InternalIndex2Handle( currentData.fObjectIndex ) ) != nullptr )
											?	VWObject( object ).GetObjectName()
											:	"";
			if ( replaceText.IsEmpty() )
			{
				replaceText	= TXResStr( "TransformMatrixDlg", "NoPickObject" );
			}
			objNameLabel.Replace( "^1", replaceText );
			this->GetStaticTextCtrlByID( kObjectNameStatic )->SetControlText( objNameLabel );
		}

		VWSwapCtrl*		transformSwap		= this->GetSwapCtrlByID( kTransformSwap );

		transformSwap->SetActivePaneIndex( (size_t) transformType );
	}
}

TransformMatrixAdvanced CDlgTransformMatrix::GetMatrixForDDX( const SDDXData & data, bool useSlider )
{
	TransformMatrixAdvanced	result;
	ETransformType		type	= ETransformType( data.fTransformType );
	switch ( type )
	{
		case ETransformType::Result:
		{
			result.SetIdentity();
			break;
		}
		case ETransformType::Rotation:
		{
			double		angle	= data.fRotAngleEdit;
			if ( useSlider )
			{
				angle	*= this->SliderPercent();
			}
			result.SetRotation( angle, data.fAsixRotEdit );
			if ( DoublesAreNotNearlyEqual( data.fAsixRotEdit.MagnitudeSquared(), 1 ) )
			{
				VWPoint3D	normal	= data.fAsixRotEdit.GetNormalized();

				VWPoint3D	scale;
				scale.x		= DoubleIsNearlyZero( normal.x ) ? 1 : data.fAsixRotEdit.x / normal.x;
				scale.y		= DoubleIsNearlyZero( normal.y ) ? 1 : data.fAsixRotEdit.y / normal.y;
				scale.z		= DoubleIsNearlyZero( normal.z ) ? 1 : data.fAsixRotEdit.z / normal.z;
				if ( useSlider )
				{
					VWPoint3D	one( 1, 1, 1 );
					scale	=  ( scale - one )*this->SliderPercent() + one;
				}
				result.ScaleAfter( scale.x, scale.y, scale.z );
			}
			break;
		}
		case ETransformType::Translation:
		{
			VWPoint3D	offset( data.fXTranslateEdit, data.fYTranslateEdit, data.fZTranslateEdit );
			if ( useSlider )
			{
				offset	*= this->SliderPercent();
			}
			result.SetTranslation( offset );
			break;
		}
		case ETransformType::Scaling:
		{
			VWPoint3D	scale( data.fXScaleEdit, data.fYScaleEdit, data.fZScaleEdit );
			if ( useSlider )
			{
				VWPoint3D	one( 1, 1, 1 );
				scale	=  ( scale - one )*this->SliderPercent() + one;
			}
			result.ScaleAfter( scale.x, scale.y, scale.z );
			break;
		}
		case ETransformType::ObjectMat:
		{
			VWObject	object;
			if	(	data.fObjectIndex > 0
				&&	( object = VWObject::InternalIndex2Handle( data.fObjectIndex ) ) != nullptr )
			{
				TransformMatrixAdvanced	objectMat;
				objectMat.fMatrix	= object.GetObjectModelMatrix();

				if ( data.fObjectRotation )
				{
					if ( useSlider && DoublesAreNotNearlyEqual( this->SliderPercent(), 1 ) )
					{
						VWPoint3D angles	= objectMat.GetEulerAngles();
						angles	*= this->SliderPercent();
						result.SetEulerAnglesRotation( angles );
					}
					else
					{
						result	= objectMat;
					}
					result.SetOffset( 0, 0, 0 );
				}
				if ( data.fObjectTrans )
				{
					VWPoint3D offset	= objectMat.GetOffset();
					if ( useSlider && DoublesAreNotNearlyEqual( this->SliderPercent(), 1 ) )
					{
						offset	*= this->SliderPercent();
					}
					result.SetOffset( offset );
				}
			}
			break;
		}
		case ETransformType::EulerAngles:
		{
			VWPoint3D	angles	= data.fEulerAnglesEdit;
			if ( useSlider )
			{
				angles *= this->SliderPercent();
			}
			result.SetEulerAnglesRotation( angles, TransformMatrixAdvanced::EEulerAnglesOrder( data.fEulerAnglesNotation ) );
			break;
		}
		default:
			break;
	}

	if ( data.fInvert )
	{
		result.Invert();
	}

	return result;
}

void CDlgTransformMatrix::CalculateResultPane()
{
	TransformMatrixAdvanced	resultMat	= this->GetTransform( false );

	TXString	labelProperties	= TXResStr( "TransformMatrixDlg", "kPropertiesStatic" );
	labelProperties.Replace( "^1"	, resultMat.IsAffine()
									? TXResStr( "TransformMatrixDlg", "Affine" )
									: TXResStr( "TransformMatrixDlg", "NotAffine" ) );
	labelProperties.Replace( "^2"	, resultMat.fMatrix.IsOrthogonal()
									? TXResStr( "TransformMatrixDlg", "Orthogonal" )
									: TXResStr( "TransformMatrixDlg", "NotOrthogonal" ) );
	this->GetStaticTextCtrlByID( kPropertiesStatic )->SetControlText( labelProperties );

	VWPoint3D	angle	= resultMat.GetEulerAngles( TransformMatrixAdvanced::EEulerAnglesOrder( fEulerIndexPopup ) );

	TXString	labelX	= TXResStr( "TransformMatrixDlg", "kXEulerResultStatic" );
	labelX.Replace( "^1", VWStringConv( angle.x ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kXEulerResultStatic )->SetControlText( labelX );

	TXString	labelY	= TXResStr( "TransformMatrixDlg", "kYEulerResultStatic" );
	labelY.Replace( "^1", VWStringConv( angle.y ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kYEulerResultStatic )->SetControlText( labelY );

	TXString	labelZ	= TXResStr( "TransformMatrixDlg", "kZEulerResultStatic" );
	labelZ.Replace( "^1", VWStringConv( angle.z ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kZEulerResultStatic )->SetControlText( labelZ );
}

void CDlgTransformMatrix::UpdateMatrixView()
{
	auto SetEditItemText = [ & ] ( TControlID control, const TXString& label )
	{
		this->GetEditTextCtrlByID( control )->SetControlText( label );
	};
	auto SetEditItem = [ & ] ( TControlID control, double value )
	{
		TXString	label;
		gSDK->DoubleToString( nsGeneral, 2, 0, value, label );
		SetEditItemText( control, label );
	};

	const SDDXData&	currentDDX	= this->GetCurrentDDX();
	bool	enableFormulaView
		= !setmember( ETransformType( currentDDX.fTransformType ), ETransformType::Result, ETransformType::ObjectMat );
	this->EnableControl( kFormulaView, enableFormulaView );

	if ( fFormulaView && enableFormulaView )
	{
		SetEditItem( kMatrixUX, 1 );
		SetEditItem( kMatrixUY, 0 );
		SetEditItem( kMatrixUZ, 0 );
		SetEditItem( kMatrixVX, 0 );
		SetEditItem( kMatrixVY, 1 );
		SetEditItem( kMatrixVZ, 0 );
		SetEditItem( kMatrixWX, 0 );
		SetEditItem( kMatrixWY, 0 );
		SetEditItem( kMatrixWZ, 1 );
		SetEditItem( kMatrixOX, 0 );
		SetEditItem( kMatrixOY, 0 );
		SetEditItem( kMatrixOZ, 0 );
		
		switch ( ETransformType( currentDDX.fTransformType ) )
		{
			case ETransformType::Rotation:
			{
				TXString	sin		( TXResStr( "TransformMatrixDlg", "formulaSin" ) );
				TXString	cos		( TXResStr( "TransformMatrixDlg", "formulaCos" ) );

				if ( currentDDX.fAsixRotRadio )
				{
					auto formatAsixRot = [ & ] ( const TXString& label ) -> TXString
					{
						TXString result	= label;
						result.Replace( "^1", TXResStr( "TransformMatrixDlg", "kXRotRadio" ) );
						result.Replace( "^2", TXResStr( "TransformMatrixDlg", "kYRotRadio" ) );
						result.Replace( "^3", TXResStr( "TransformMatrixDlg", "kZRotRadio" ) );
						result.Replace( "^4", sin );
						result.Replace( "^5", cos );
						return result;
					};

					SetEditItemText( kMatrixUX, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotUX" ) ) );
					SetEditItemText( kMatrixUY, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotUY" ) ) );
					SetEditItemText( kMatrixUZ, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotUZ" ) ) );
					SetEditItemText( kMatrixVX, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotVX" ) ) );
					SetEditItemText( kMatrixVY, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotVY" ) ) );
					SetEditItemText( kMatrixVZ, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotVZ" ) ) );
					SetEditItemText( kMatrixWX, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotWX" ) ) );
					SetEditItemText( kMatrixWY, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotWY" ) ) );
					SetEditItemText( kMatrixWZ, formatAsixRot( TXResStr( "TransformMatrixDlg", "formulaRotWZ" ) ) );
				}
				else
				{

					if ( currentDDX.fXRotRadio )
					{
						SetEditItemText( kMatrixVY, cos );
						SetEditItemText( kMatrixVZ, "-" + sin );
						SetEditItemText( kMatrixWY, sin );
						SetEditItemText( kMatrixWZ, cos );
					}
					else if ( currentDDX.fYRotRadio )
					{
						SetEditItemText( kMatrixUX, cos );
						SetEditItemText( kMatrixUZ, sin );
						SetEditItemText( kMatrixWX, "-" + sin );
						SetEditItemText( kMatrixWZ, cos );

					}
					else if ( currentDDX.fZRotRadio )
					{
						SetEditItemText( kMatrixUX, cos );
						SetEditItemText( kMatrixUY, "-" + sin );
						SetEditItemText( kMatrixVX, sin );
						SetEditItemText( kMatrixVY, cos );
					}
				}
				break;
			}
			case ETransformType::Translation:
			{
				SetEditItemText( kMatrixOX, TXResStr( "TransformMatrixDlg", "kXRotRadio" ) );
				SetEditItemText( kMatrixOY, TXResStr( "TransformMatrixDlg", "kYRotRadio" ) );
				SetEditItemText( kMatrixOZ, TXResStr( "TransformMatrixDlg", "kZRotRadio" ) );
				break;
			}
			case ETransformType::Scaling:
			{
				SetEditItemText( kMatrixUX, TXResStr( "TransformMatrixDlg", "kXRotRadio" ) + "*1" );
				SetEditItemText( kMatrixVY, TXResStr( "TransformMatrixDlg", "kYRotRadio" ) + "*1" );
				SetEditItemText( kMatrixWZ, TXResStr( "TransformMatrixDlg", "kZRotRadio" ) + "*1" );
				break;
			}
			default:
				break;
		}
	}
	else
	{
		TransformMatrixAdvanced	matrix;
		if ( fSelectedIndex == (size_t) -1 )
		{
			matrix	= this->GetTransform( false );
		}
		else
		{
			matrix	= this->GetMatrixForDDX( this->GetCurrentDDX(), false );
		}

		auto&	matrixData	= matrix.fMatrix.mat;
		SetEditItem( kMatrixUX, matrixData[ 0 ][ 0 ] );
		SetEditItem( kMatrixUY, matrixData[ 0 ][ 1 ] );
		SetEditItem( kMatrixUZ, matrixData[ 0 ][ 2 ] );
		SetEditItem( kMatrixVX, matrixData[ 1 ][ 0 ] );
		SetEditItem( kMatrixVY, matrixData[ 1 ][ 1 ] );
		SetEditItem( kMatrixVZ, matrixData[ 1 ][ 2 ] );
		SetEditItem( kMatrixWX, matrixData[ 2 ][ 0 ] );
		SetEditItem( kMatrixWY, matrixData[ 2 ][ 1 ] );
		SetEditItem( kMatrixWZ, matrixData[ 2 ][ 2 ] );
		SetEditItem( kMatrixOX, matrixData[ 3 ][ 0 ] );
		SetEditItem( kMatrixOY, matrixData[ 3 ][ 1 ] );
		SetEditItem( kMatrixOZ, matrixData[ 3 ][ 2 ] );
	}
}

void CDlgTransformMatrix::UpdatePreview()
{
	TransformMatrixAdvanced	transform		= this->GetTransform( true );
	MCObjectHandle			hPreviewSymbol	= gSDK->GetNamedObject( "____SymbolDefPreview" );
	if	(	fDetailChange
		||	!fLastTransform.IsEqual( transform )
		||	hPreviewSymbol == nullptr )
	{
		if ( hPreviewSymbol == nullptr )
		{
			hPreviewSymbol	= VWSymbolDefObj( "____SymbolDefPreview" );
			gSDK->AddAfterSwapObject( hPreviewSymbol );
		}

		VWGroupObj	previewDef( hPreviewSymbol );
		previewDef.DeleteAllInnerObjects( true );

		bool	isOrthogonal	= transform.fMatrix.IsOrthogonal();
		VWObject	transformObj( isOrthogonal
			? fPreviewGeometry.AddCacheObjectCopy( previewDef )
			: fPreviewGeometry.AddMeshCopy( previewDef ) );
		this->TransformObjectReq( transformObj, transform, isOrthogonal );

		if ( fDetailedPreview )
		{
			fPreviewGeometry.AddCacheObjectCopy( previewDef );
		}
		else
		{
			fPreviewGeometry.AddSimplePreviewCopy( previewDef );
		}

		previewDef.ResetObject();
		fLastTransform	= transform;
		fDetailChange	= false;
		fRenderChange	= true;
	}

	if ( fRenderChange )
	{
		SymbolImgInfo imgInfoIn( (TStandardView) fViewMarker, (TRenderMode) fRenderMarker, EImageViewComponent::NotSet, ELevelsOfDetail::All, false/*scaleByZoom*/ );
		gSDK->UpdateSymbolDisplayControl( this->GetDialogID()
										, kPreview
										, "____SymbolDefPreview"
										, imgInfoIn );
		fRenderChange	= false;
	}
}

double CDlgTransformMatrix::SliderPercent()
{
	return fSliderValue / 100.0;
}

TXString CDlgTransformMatrix::GetNewName( const TXString & oldName )
{
	bool	doAdd	= oldName.IsEmpty();
	CStandardEditTextBoxDlg dlgSetName
		( doAdd ? TXResStr( "TransformMatrixDlg", "AddTitle" ) : TXResStr( "TransformMatrixDlg", "RenameTitle" )
		, doAdd ? TXResStr( "TransformMatrixDlg", "AddLabel" ) : TXResStr( "TransformMatrixDlg", "RenameLabel" )
		, oldName
		, TXResStr( "TransformMatrixDlg", "ok_button" )
		, TXResStr( "TransformMatrixDlg", "cancel_button" )
	);

	TXString	newName;
	do
	{
		if ( dlgSetName.RunDialogLayout( "" ) == EDialogButton::kDialogButton_Ok )
		{
			newName	= dlgSetName.GetValue();
			if ( !newName.IsEmpty() && newName != TXResStr( "TransformMatrixDlg", "ResultRow" ) )
			{
				for ( auto&& currentData : fDDXControls )
				{
					if ( newName == currentData.fName )
					{
						newName.Clear();
						break;
					}
				}
			}

		}
		else
		{
			break;
		}
	}
	while ( newName.IsEmpty() );

	return newName;
}

CDlgTransformMatrix::CPreviewGeometry::CPreviewGeometry()
	: fCacheObject( nullptr )
	, fPreviewObject( nullptr )
	, fMeshObject( nullptr )
{
}

CDlgTransformMatrix::CPreviewGeometry::~CPreviewGeometry()
{
}

MCObjectHandle CDlgTransformMatrix::CPreviewGeometry::AddCacheObjectCopy( MCObjectHandle inContainer )
{
	MCObjectHandle	result	= gSDK->DuplicateObject( this->GetOrGenerateCache() );
	gSDK->AddObjectToContainer( result, inContainer );

	return result;
}

MCObjectHandle CDlgTransformMatrix::CPreviewGeometry::AddSimplePreviewCopy( MCObjectHandle inContainer )
{
	if ( !fPreviewObject )
	{
		WorldCube		boundCube	= this->GetCacheCube();
		VWRectangle2D	baseRect	( boundCube.MinX(), boundCube.MaxX(), boundCube.MinY(), boundCube.MaxY() );
		VWPolygon2DObj	basePoly	( baseRect );

		VWExtrudeObj	extrude		( basePoly, boundCube.MinZ(), boundCube.MaxZ() - boundCube.MinZ() );
		gSDK->AddAfterSwapObject( extrude );
		fPreviewObject	= gSDK->ConvertTo3DPolygons( extrude );
		gSDK->AddAfterSwapObject( fPreviewObject );

		VWObjectAttr( fPreviewObject ).SetFillNone();
		gSDK->ResetObject( fPreviewObject );
	}
	
	MCObjectHandle	result	= gSDK->DuplicateObject( fPreviewObject );
	gSDK->AddObjectToContainer( result, inContainer );
	return result;
}

MCObjectHandle CDlgTransformMatrix::CPreviewGeometry::AddMeshCopy( MCObjectHandle inContainer )
{
	if ( !fMeshObject )
	{
		VWGroupObj	meshGroup;
		gSDK->AddAfterSwapObject( meshGroup );
		MCObjectHandle	mesh = gSDK->ConvertTo3DPolygons( this->AddCacheObjectCopy( meshGroup ) );
		gSDK->GroupToMesh( mesh );
		fMeshObject	= meshGroup;
	}

	MCObjectHandle	result	= gSDK->DuplicateObject( gSDK->FirstMemberObj( fMeshObject ) );
	gSDK->AddObjectToContainer( result, inContainer );
	return result;
}

WorldCube CDlgTransformMatrix::CPreviewGeometry::GetCacheCube()
{
	WorldCube	boundCube;
	gSDK->GetObjectCube( this->GetOrGenerateCache(), boundCube );
	return boundCube;
}

void CDlgTransformMatrix::CPreviewGeometry::Clear()
{
	auto clearObject	= [] ( MCObjectHandle& handle )
	{
		if ( handle )
		{
			gSDK->AddBeforeSwapObject( handle );
			gSDK->DeleteObject( handle );
		}
		handle	= nullptr;
	};
	clearObject( fCacheObject );
	clearObject( fPreviewObject );
	clearObject( fMeshObject );
	MCObjectHandle	symbol	= gSDK->GetNamedObject( "____SymbolDefPreview" );
	clearObject( symbol );
}

MCObjectHandle CDlgTransformMatrix::CPreviewGeometry::GetOrGenerateCache()
{
	if ( !fCacheObject )
	{
		VWGroupObj	selGroup;
		gSDK->AddAfterSwapObject( selGroup );
		gSDK->ForEachObjectN
		( allSelectedAndEditable
			, [ & ] ( MCObjectHandle h )
			{
				selGroup.AddObject( gSDK->DuplicateObject( h ) );
			}
		);
		selGroup.ResetObject();
		fCacheObject	= selGroup ;
	}
	return fCacheObject;
}
