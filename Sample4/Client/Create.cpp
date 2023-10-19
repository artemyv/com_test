//
// Create.cpp
//
#include <iostream>
#include "Create.h"
typedef IUnknown* (*CREATEFUNCPTR)();
IUnknown* CallCreateInstance(std::string name)
{
	if (name.empty()) name = "Component.dll";
	// Загрузить в процесс динамическую библиотеку
	HINSTANCE hComponent = ::LoadLibraryA(name.c_str());
	if (hComponent == nullptr)
	{
		std::cout << "CallCreateInstance:\tFailed to load dll\n";
		return NULL;
	}
	// Получить адрес функции CreateInstance
	auto CreateInstance
		= (CREATEFUNCPTR)::GetProcAddress(hComponent, "CreateInstance");
	if (CreateInstance == nullptr)
	{
		std::cout << "CallCreateInstance:\tfailed to get proc address of CreateInstance\n";
		return nullptr;
	}
	return CreateInstance();
}