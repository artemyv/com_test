// Shapes.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "3drect.h"
#include "3drectEx.h"
#include "rectfunctions.h"
#include <print>

int main(void)
{
	bool retVal = false;

	std::println( "******* Interfaces *******" );
	

	// Create a rect to work with.
	CreateThe3DRect();

	IDraw* pDraw = NULL;
	IDraw2* pDraw2= NULL;
	IDraw3* pDraw3= NULL;
	IShapeEdit* pShapeEdit= NULL;

	// Get IDraw from Object.
	retVal = GetInterfaceFrom3DRect(INTERFACEID::IDRAW, (void**)&pDraw);

	if(retVal)
	{
		pDraw->Draw();

		// Send it a rect.
		ExersiceRect(pDraw);
	}

	// Get IShapeEdit from object.
	retVal = GetInterfaceFrom3DRect(INTERFACEID::ISHAPEEDIT, (void**)&pShapeEdit);
	
	if(retVal)
	{
		pShapeEdit->Fill(FILLTYPE::POLKADOT);
		pShapeEdit->Inverse();
		pShapeEdit->Stretch(90);
	}

	retVal = GetInterfaceFrom3DRect(INTERFACEID::IDRAW3, (void**)&pDraw3);

	if(retVal)
	{
		pDraw3->DrawToMetaFile();
	}

	// Done.
	DestroyThe3DRect();

	// Make a RectEx
	CreateThe3DRectEx();

	// Get interfaces
	GetInterfaceFrom3DRectEx(INTERFACEID::IDRAW,  (void**)&pDraw);
	GetInterfaceFrom3DRectEx(INTERFACEID::IDRAW2, (void**)&pDraw2);
	GetInterfaceFrom3DRectEx(INTERFACEID::IDRAW3, (void**)&pDraw3);
	GetInterfaceFrom3DRectEx(INTERFACEID::ISHAPEEDIT, (void**)&pShapeEdit);

	pDraw2->DrawToMemory();
	pDraw2->DrawToPrinter();
	pDraw3->DrawToMetaFile();

	ExersiceRect((IDraw*)pDraw2);

	DestroyThe3DRectEx();
	return 0;
}
