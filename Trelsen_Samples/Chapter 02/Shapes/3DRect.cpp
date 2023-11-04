// 3DRect.cpp: implementation of the C3DRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3DRect.h"
#include <print>
#include <map>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DRect::C3DRect()
{
	std::println("Created a 3DRect");
}

C3DRect::~C3DRect()
{
	std::println("Destroyed a 3DRect");
}

// IDraw
void C3DRect::Draw()
{
	std::println("C3DRect::Draw");
}

// IShapeEdit
void C3DRect::Fill(FILLTYPE fType)
{
	static const std::map< FILLTYPE, const char*> FillString = {
		{FILLTYPE::HATCH, "Hatch"},
		{FILLTYPE::SOLID, "Solid"},
		{FILLTYPE::POLKADOT, "Polka dot"}
	};
	std::println("Filling a 3D Rect as: {}", FillString.at(fType));
}

void C3DRect::Inverse()
{
	std::println("Inverting a 3D Rect");
}

void C3DRect::Stretch(int factor)
{
	std::println("Streching a 3D Rect by a factor of: {}", factor);
}
