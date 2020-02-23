#include "StdAfx.h"

#include "DlgTransformMatrix.h"

#include "ToolPickObject.h"

using namespace CreateTransformMatrix;
// dialog control IDs
enum ETransformMatrixDlg {
	kOK                      = 1,
	kCancel                  = 2,
	kGenerateTranGrp         = 4,
	kTransformsList          = 5,
	kAddButton               = 6,
	kRenameButton            = 7,
	kRemoveButton            = 8,
	kTransformGrp            = 9,
	kResultSwap              = 10,
	kTransformPane           = 11,
	kNameStatic              = 12,
	kTransformTypeGrp        = 13,
	kTypePopup               = 14,
	kInvertCheck             = 15,
	kTransformSwap           = 16,
	kRatationPane            = 17,
	kRotationAsixGrp         = 18,
	kXRotRadio               = 19,
	kYRotRadio               = 20,
	kZRotRadio               = 21,
	kAsixRotRadio            = 22,
	kAxisRotStatic           = 23,
	kXRotStatic              = 24,
	kXRotEdit                = 25,
	kYRotStatic              = 26,
	kYRotEdit                = 27,
	kZRotStatic              = 28,
	kZRotEdit                = 29,
	kNormAsixBtn             = 30,
	kRotAngleStatic          = 31,
	kRotAngleEdit            = 32,
	kTranslationPane         = 33,
	kTranslationCoordsStatic = 34,
	kXTranslateStatic        = 35,
	kXTranslateEdit          = 36,
	kYTranslateStatic        = 37,
	kYTranslateEdit          = 38,
	kZTranslateStatic        = 39,
	kZTranslateEdit          = 40,
	kScalePane               = 41,
	kScaleFactorsStatic      = 42,
	kXScaleStatic            = 43,
	kXScaleEdit              = 44,
	kYScaleStatic            = 45,
	kYScaleEdit              = 46,
	kZScaleStatic            = 47,
	kZScaleEdit              = 48,
	kSymetricScale           = 49,
	kObjectPane              = 50,
	kObjectNameStatic        = 51,
	kPickObjectButton        = 52,
	kObjectRotationCheck     = 53,
	kObjectTranslationCheck  = 54,
	kResultPane              = 55,
	kAffine                  = 56,
	kOulerGroup              = 57,
	kXOulerStatic            = 58,
	kYOulerStatic            = 59,
	kZOulerStatic            = 60,
	kMatrixView              = 61,
	kMatrix0RC               = 62,
	kMatrixXCol              = 63,
	kMatrixYCol              = 64,
	kMatrixZCol              = 65,
	kMatrixURow              = 66,
	kMatrixUX                = 67,
	kMatrixUY                = 68,
	kMatrixUZ                = 69,
	kMatrixVRow              = 70,
	kMatrixVX                = 71,
	kMatrixVY                = 72,
	kMatrixVZ                = 73,
	kMatrixWRow              = 74,
	kMatrixWX                = 75,
	kMatrixWY                = 76,
	kMatrixWZ                = 77,
	kMatrixORow              = 78,
	kMatrixOX                = 79,
	kMatrixOY                = 80,
	kMatrixOZ                = 81,
	kFormulaView             = 82,
	kPreviewGrp              = 83,
	kPreview                 = 84,
	kSlider                  = 85,

	// custom strings IDs
};

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

ADD_DISPATCH_EVENT( kOulerGroup		, OnResultPaneChage );
EVENT_DISPATCH_MAP_END;

CDlgTransformMatrix::CDlgTransformMatrix()
	: fSliderValue( 100 )
	, fResultDDX()
	, fSelectedIndex((size_t) -1)
{
	// Assign settings tag to this dialog so it would save it's position and size automatically
	this->SetSavedSettingsTag( "TransformMatrix", "TransformMatrixDlg" );
}

CDlgTransformMatrix::~CDlgTransformMatrix()
{
	gSDK->DeleteObjectNoNotify( fCacheObject );
}

TransformMatrixAdvanced CDlgTransformMatrix::GetTransform( bool useSlider )
{
	TransformMatrixAdvanced	result;
	for ( const SDDXData& item : fDDXControls )
	{
		if ( item.fUse )
		{
			result	*= this->GetMatrixForDDX( item, useSlider );
		}
	}
	return result;
}

void CDlgTransformMatrix::TransformObjects( MCObjectHandle targetContainer /*= nullptr*/, bool applySlider /*= false*/ )
{
	TransformMatrixAdvanced	transform	= this->GetTransform( applySlider );
	if ( targetContainer )
	{
		//VWMeshObj	mesh( gSDK->DuplicateObject( fMeshObject ) );

		VWObject	obj( gSDK->DuplicateObject( fCacheObject ) );
		gSDK->AddObjectToContainer( obj, targetContainer ? targetContainer : gSDK->GetActiveLayer() );
		obj.TransformObject( transform, true, true );
		obj.ResetObject();


		//VectorWorks::IMeshDataPtr	meshData = mesh.GetMesh();
		//size_t	vertCount	= meshData->GetVertexCount();
		//for ( size_t vertexIndex = 0; vertexIndex < vertCount; vertexIndex++ )
		//{
		//	TransformXMatrix	projectMatrix;
		//	MatrixToXMatrix( transform, projectMatrix );
		//	WorldPt3	vertexPt;
		//	meshData->GetVertexPosition( vertexIndex, vertexPt );
		//	vertexPt = XPointTransformN( vertexPt, projectMatrix );
		//	meshData->SetVertexPosition( vertexIndex, vertexPt );
		//}
		//meshData->Save();
		//mesh.ResetObject();
	}
	else
	{
		gSDK->ForEachObjectN( allSelectedAndEditable + descendIntoGroups,
			[ & ] ( MCObjectHandle h )
			{
				gSDK->AddBothSwapObject( h );
				VWObject	obj( h );
				obj.TransformObject( transform, true, true );
				obj.ResetObject();
			} );
	}
}

bool CDlgTransformMatrix::CreateDialogLayout()
{
	return this->CreateDialogLayoutFromVWR( "TransformMatrix/DialogLayout/TransformMatrixDlg.vs" );
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
	pTranformTypePopup->AddItem( TXResStr( "TransformMatrixDlg", "ObjectMatrix" ),append, (Sint32)ETransformType::ObjectMat		);

	fOulerIndexPopup	= 0;
	VWPullDownMenuCtrl* pOulerPopup	= this->GetPullDownMenuCtrlByID( kOulerGroup );
	pOulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Notation1" ) );
	pOulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Notation2" ) );
	pOulerPopup->AddItem( TXResStr( "TransformMatrixDlg", "Notation3" ) );

	this->GetSliderCtrlByID( kSlider )->SetSliderLiveUpdate( true );

	VWGroupObj	selGroup;
	gSDK->ForEachObjectN
		(	allSelectedAndEditable
		,	[ & ] ( MCObjectHandle h )
			{
				selGroup.AddObject( gSDK->DuplicateObject( h ) );
			}	
		);
	fCacheObject	= /*gSDK->ConvertTo3DPolygons( */selGroup /*)*/;
	//gSDK->GroupToMesh( fMeshObject );
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

void CDlgTransformMatrix::OnListBrowser( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	VWListBrowserCtrl*	pListBrowser	= this->GetListBrowserCtrlByID( kTransformsList );

	size_t	rowIndex, colIndex;
	EListBrowserEventType	eventType	= eventArgs.GetListBrowserEvent( rowIndex, colIndex );
	if ( eventArgs.IsListBrowserDrawNDropEnd() )
	{
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
	if ( this->GetCurrentDDX().fSymetricScale )
	{
		double	value	= this->GetEditRealCtrlByID( controlID )->GetEditReal( VWEditRealCtrl::kEditControlReal );
		auto setEditValue	= [ & ] ( Sint32 forControlID )
		{
			if ( controlID != forControlID )
			{
				this->GetEditRealCtrlByID( forControlID )->SetEditReal( value, VWEditRealCtrl::kEditControlReal );
			}
		};
		setEditValue( kXScaleEdit );
		setEditValue( kYScaleEdit );
		setEditValue( kZScaleEdit );
	}
}

void CDlgTransformMatrix::OnSymetricCheck( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	this->OnScaleEdit( kXScaleEdit, eventArgs );
}

void CDlgTransformMatrix::OnPickObjectButton( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	IExtendedPropsPtr	props( IID_ExtendedProps );
	IExtension*			extention	= props->GetExtensionByName( "PickObjectTempTool" );
	if ( extention )
	{
		VCOMSinkPtr< CExtToolPickObject_EventSink >	sink( extention, IID_ToolEventSink );
		if ( sink )
		{
			sink->CallbackObjectIndex( this->GetCurrentDDX().fObjectIndex );
		}
	}

	gSDK->RunTempTool( CExtToolPickObject::_GetIID() );
	this->SetDialogClose( false );
}

void CDlgTransformMatrix::OnResultPaneChage( Sint32 controlID, VWDialogEventArgs & eventArgs )
{
	this->CalculateResultPane();
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
		this->AddDDX_PulldownMenu(kTypePopup			, &currentData.fTransformType );
		this->AddDDX_CheckButton( kInvertCheck			, &currentData.fInvert );
		this->AddDDX_RadioButton( kXRotRadio			, &currentData.fXRotRadio );
		this->AddDDX_RadioButton( kYRotRadio			, &currentData.fYRotRadio );
		this->AddDDX_RadioButton( kZRotRadio			, &currentData.fZRotRadio );
		this->AddDDX_RadioButton( kAsixRotRadio			, &currentData.fAsixRotRadio );
		this->AddDDX_EditReal	( kXRotEdit				, &currentData.fAsixRotEdit.x, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kYRotEdit				, &currentData.fAsixRotEdit.y, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kZRotEdit				, &currentData.fAsixRotEdit.z, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kRotAngleEdit			, &currentData.fRotAngleEdit, VWEditRealCtrl::kEditControlAngle );
		this->AddDDX_EditReal	( kXTranslateEdit		, &currentData.fXTranslateEdit, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kYTranslateEdit		, &currentData.fYTranslateEdit, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kZTranslateEdit		, &currentData.fZTranslateEdit, VWEditRealCtrl::kEditControlDimension );
		this->AddDDX_EditReal	( kXScaleEdit			, &currentData.fXScaleEdit, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kYScaleEdit			, &currentData.fYScaleEdit, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_EditReal	( kZScaleEdit			, &currentData.fZScaleEdit, VWEditRealCtrl::kEditControlReal );
		this->AddDDX_CheckButton( kSymetricScale		, &currentData.fSymetricScale );
		this->AddDDX_CheckButton( kObjectRotationCheck	, &currentData.fObjectRotation );
		this->AddDDX_CheckButton( kObjectTranslationCheck,&currentData.fObjectTrans );
	}

	this->AddDDX_PulldownMenu( kOulerGroup	, &fOulerIndexPopup );
	this->AddDDX_CheckButton ( kFormulaView	, &fFormulaView );

	this->UpdatePanes();
	
	this->AddDDX_Slider( kSlider, &fSliderValue );
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
			TXString	objNameLabel	= TXResStr( "TransformMatrixDlg", "kObjectNameStatic" );
			TXString	replaceText		= VWObject( VWObject::InternalIndex2Handle( currentData.fObjectIndex ) ).GetObjectName();
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
				scale	*= this->SliderPercent();
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
						VWPoint3D angles	= objectMat.GetOulerAngles( 0 );
						angles	*= this->SliderPercent();
						result.SetOulerAnglesRotation( angles, 0 );
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

	TXString	labelAffine	= TXResStr( "TransformMatrixDlg", "kAffine" );
	labelAffine.Replace( "^1"
		, resultMat.IsAffine() ? TXResStr( "TransformMatrixDlg", "Affine" ) : TXResStr( "TransformMatrixDlg", "NotAffine" ) );
	this->GetStaticTextCtrlByID( kAffine )->SetControlText( labelAffine );

	VWPoint3D	angle	= resultMat.GetOulerAngles( fOulerIndexPopup );

	TXString	labelX	= TXResStr( "TransformMatrixDlg", "kXOulerStatic" );
	labelX.Replace( "^1", VWStringConv( angle.x ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kXOulerStatic )->SetControlText( labelX );

	TXString	labelY	= TXResStr( "TransformMatrixDlg", "kYOulerStatic" );
	labelY.Replace( "^1", VWStringConv( angle.y ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kYOulerStatic )->SetControlText( labelY );

	TXString	labelZ	= TXResStr( "TransformMatrixDlg", "kZOulerStatic" );
	labelZ.Replace( "^1", VWStringConv( angle.z ).GetAngleDegString() );
	this->GetStaticTextCtrlByID( kZOulerStatic )->SetControlText( labelZ );
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
	VWSymbolDefObj	previewDef( "____SymbolDefPreview" );
	previewDef.DeleteAllInnerObjects();
	this->TransformObjects( previewDef, true );
	previewDef.ResetObject();

	SymbolImgInfo imgInfoIn(standardViewLeftIso, renderOpenGL, EImageViewComponent::NotSet, false/*scaleByZoom*/);
	gSDK->UpdateSymbolDisplayControl( this->GetDialogID()
		, kPreview
		, previewDef.GetObjectName()
		, imgInfoIn);
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
			if ( !newName.IsEmpty() )
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


