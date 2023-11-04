// 3DRectEx.cpp: implementation of the C3DRectEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3DRectEx.h"
#include <print>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DRectEx::C3DRectEx()
{
	std::println("Created a C3DRectEx");
}

C3DRectEx::~C3DRectEx()
{
	std::println("Destroyed a C3DRectEx");
}


void C3DRectEx::Draw()
{
	std::println("C3DRectEx::Draw");
}

void C3DRectEx::DrawToMemory()
{
	std::println("C3DRectEx::DrawToMemory");
}

void C3DRectEx::DrawToPrinter()
{
	std::println("C3DRectEx::DrawToPrinter");
}

void C3DRectEx::DrawToMetaFile()
{
	std::println("C3DRectEx::DrawToMetaFile");
}
