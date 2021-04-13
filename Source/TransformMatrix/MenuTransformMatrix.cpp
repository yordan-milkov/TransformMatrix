#include "StdAfx.h"

#include "MenuTransformMatrix.h"

using namespace CreateTransformMatrix;

static SMenuDef		gMenuItemTransformMatrix = {
	/*Needs*/				1,				/* docIsActive */
	/*NeedsNot*/			0,
	/*Title*/				{"MenuTransformMatrix", "menu_title"}, 
	/*Category*/			{"MenuTransformMatrix", "menu_category"},
	/*HelpText*/			{"MenuTransformMatrix", "menu_helptext"},
	/*VersionCreated*/		24,
	/*VersoinModified*/		0,
	/*VersoinRetired*/		0,
	/*OverrideHelpID*/		""
};

// {D017BD46-3E2B-4A98-99E3-C19029314C26}
IMPLEMENT_VWMenuExtension(
	/*Extension class*/		CExtMenuTransformMatrix,
	/*Event sink*/			CMenuTransformMatrix_EventSink,
	/*Universal name*/		"CreateTransformMatrix",
	/*Version*/				1,
	/*UUID*/				0xd017bd46, 0x3e2b, 0x4a98, 0x99, 0xe3, 0xc1, 0x90, 0x29, 0x31, 0x4c, 0x26 );


CExtMenuTransformMatrix::CExtMenuTransformMatrix( CallBackPtr cbp )
	: VWExtensionMenu( cbp, gMenuItemTransformMatrix, NULL )
{
}

CExtMenuTransformMatrix::~CExtMenuTransformMatrix()
{
}


CMenuTransformMatrix_EventSink::CMenuTransformMatrix_EventSink( IVWUnknown * parent )
	: VWMenu_EventSink( parent )
{
}

CMenuTransformMatrix_EventSink::~CMenuTransformMatrix_EventSink()
{
}

void CMenuTransformMatrix_EventSink::DoInterface()
{
	fTransfromDialog.RunDialog();
}

