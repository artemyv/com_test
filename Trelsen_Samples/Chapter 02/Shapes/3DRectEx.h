// 3DRectEx.h: interface for the C3DRectEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DRECTEX_H__C058B644_D4ED_11D2_B8CF_0020781238D4__INCLUDED_)
#define AFX_3DRECTEX_H__C058B644_D4ED_11D2_B8CF_0020781238D4__INCLUDED_

#pragma once

#include "interfaces.h"

class C3DRectEx: public IDraw3  // Brings in layout of IDraw2 & IDraw
{
public:
	C3DRectEx();
	virtual ~C3DRectEx();

	// IDraw
	void Draw() override;

	// IDraw2
	void DrawToMemory() override;
	void DrawToPrinter() override;

	// IDraw3
	void DrawToMetaFile() override;
};

#endif // !defined(AFX_3DRECTEX_H__C058B644_D4ED_11D2_B8CF_0020781238D4__INCLUDED_)
