// 3DRect.h: interface for the C3DRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DRECT_H__1E95D33C_D4B1_11D2_AAF2_00A0C9312D57__INCLUDED_)
#define AFX_3DRECT_H__1E95D33C_D4B1_11D2_AAF2_00A0C9312D57__INCLUDED_

#pragma once

class C3DRect: public IDraw,
	public IShapeEdit
{
public:
	C3DRect();
	virtual ~C3DRect();

	// IDraw
	virtual void Draw();

	// IShapeEdit
	void Fill(FILLTYPE fType) override;
	void Inverse() override;
	void Stretch(int factor) override;
};

#endif // !defined(AFX_3DRECT_H__1E95D33C_D4B1_11D2_AAF2_00A0C9312D57__INCLUDED_)
