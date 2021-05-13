#include "StdAfx.h"

#include "ExtToolPickObject.h"
#include "DlgTransformMatrix.h"

using namespace CreateTransformMatrix;

// --------------------------------------------------------------------------------------------------------
static SToolDef		gToolDef = {
	/*ToolType*/					eExtensionToolType_Normal,
	/*ParametricName*/				"",
	/*PickAndUpdate*/				ToolDef::pickAndUpdate,
	/*NeedScreenPlane*/				ToolDef::needsScreenPlane,
	/*Need3DProjection*/			ToolDef::doesntNeed3DView,
	/*Use2DCursor*/					ToolDef::use2DCursor,
	/*ConstrainCursor*/				ToolDef::screenBasedCursor,
	/*NeedPerspective*/				ToolDef::doesntNeedPerspective,
	/*ShowScreenHints*/				ToolDef::showScreenHints,
	/*NeedsPlanarContext*/			ToolDef::needsPlanarContext,
	/*Message*/						{"ExtToolPickObject", "tool_message"},
	/*WaitMoveDistance*/			0,
	/*ConstraintFlags*/				0,
	/*BarDisplay*/					kToolBarDisplay_XYClLaZo,
	/*MinimumCompatibleVersion*/	900,
	/*Title*/						{"ExtToolPickObject",	"tool_title"	},
	/*Category*/					{"ExtToolPickObject",	"tool_category"	},
	/*HelpText*/					{"ExtToolPickObject",	"tool_help"		},
	/*VersionCreated*/				25,
	/*VersoinModified*/				0,
	/*VersoinRetired*/				0,
	/*OverrideHelpID*/				"",
	/*IconSpec*/					"",
	/*CursorImageSpec*/				"",
};

// --------------------------------------------------------------------------------------------------------
// {7217DCBF-4DBB-4760-A923-894F719B5B07}
IMPLEMENT_VWToolExtension(
	/*Extension class*/	CExtToolPickObject,
	/*Event sink*/		CExtToolPickObject_EventSink,
	/*Universal name*/	"PickObjectTempTool",
	/*Version*/			1,
	/*UUID*/			 0x7217dcbf, 0x4dbb, 0x4760, 0xa9, 0x23, 0x89, 0x4f, 0x71, 0x9b, 0x5b, 0x7 );


// --------------------------------------------------------------------------------------------------------
CExtToolPickObject::CExtToolPickObject(CallBackPtr cbp)
	: VWExtensionTool( cbp, gToolDef )
{
}

CExtToolPickObject::~CExtToolPickObject()
{
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
CExtToolPickObject_EventSink::CExtToolPickObject_EventSink(IVWUnknown* parent)
	: VWTool_EventSink( parent )
{
	this->Clear();
}

CExtToolPickObject_EventSink::~CExtToolPickObject_EventSink()
{
}

bool CExtToolPickObject_EventSink::DoSetUp(bool bRestore, const IToolModeBarInitProvider* pModeBarInitProvider)
{
	this->SetModeBarHelpText( TXResStr( "ExtToolPickObject", "tool_message" ) );
	return VWTool_EventSink::DoSetUp( bRestore, pModeBarInitProvider );
}

void CExtToolPickObject_EventSink::DoSetDown(bool bRestore, const IToolModeBarInitProvider* pModeBarInitProvider)
{
	if ( bRestore == false )
	{
		CDlgTransformMatrix::Context::SetIsToolActive( false );
		this->Clear();
	}

	VWTool_EventSink::DoSetDown( bRestore, pModeBarInitProvider );
}

void CExtToolPickObject_EventSink::PointAdded()
{
	if ( !fPickObject )
	{
		this->PopLastToolPt();
	}
	else
	{
		this->RunMenuCommand();
	}
}

void CExtToolPickObject_EventSink::MouseMove()
{
	gSDK->EmptyToolHighlightingList();

	short overPart;
	SintptrT code;
	gSDK->TrackTool( fPickObject, overPart, code );
	if ( fPickObject )
	{
		gSDK->AddToolHighlightingObject( fPickObject );
	}
}

void CExtToolPickObject_EventSink::HandleComplete()
{

}

void CExtToolPickObject_EventSink::RunMenuCommand()
{
	if ( fPickObject )
	{
		gSDK->SetUndoMethod( kUndoSwapObjects );

		VWObject	pickObj	( fPickObject );
		TXString	name	= pickObj.GetObjectName();
		if ( name.IsEmpty() )
		{
			CStandardEditTextBoxDlg dlgSetName
				( TXResStr( "ExtToolPickObject", "NameTitle" )
				, TXResStr( "ExtToolPickObject", "NameLabel" )
				, name
				, TXResStr( "ExtToolPickObject", "ok_Name" )
				, TXResStr( "ExtToolPickObject", "cancel_Name" )
			);

			do
			{
				if ( dlgSetName.RunDialogLayout( "" ) == EDialogButton::kDialogButton_Ok )
				{
					name	= dlgSetName.GetValue();
					if ( !name.IsEmpty() )
					{
						if ( gSDK->GetNamedObject( name ) == nullptr )
						{
							gSDK->AddBothSwapObject( pickObj );
							pickObj.SetObjectName( name );
							break;
						}
					}

				}
				else
				{
					break;
				}
			}
			while ( name.IsEmpty() );
		}
		gSDK->EndUndoEvent();

		if ( pickObj.GetObjectName().IsEmpty() == false )
		{
			CDlgTransformMatrix::Context transfromDialog;
			transfromDialog.RunDialog( pickObj.GetInternalIndex() );
		}
	}
	this->Clear();
	//gSDK->DoMenuName( "CreateTransformMatrix", 0 );
}

void CExtToolPickObject_EventSink::Clear()
{
	fPickObject		= nullptr;
	gSDK->EmptyToolHighlightingList();
}
