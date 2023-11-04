// Templates.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shapetemplate.h"
#include "chexagon.h"
#include "csquare.h"

int main()
{
	CDrawTemplate<CHexagon> hex;
	hex.DrawTheShape();

	CDrawTemplate<CSquare> s;
	s.DrawTheShape();
	return 0;
}
