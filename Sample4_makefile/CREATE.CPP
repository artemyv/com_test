//
// Create.cpp 
//
#include <iostream>
#include <unknwn.h>    // Declare IUnknown.

#include "Create.h"

typedef IUnknown* (*CREATEFUNCPTR)() ;

IUnknown* CallCreateInstance(char* name)
{
	// Load dynamic link library into process.
	HINSTANCE hComponent = ::LoadLibrary(name) ;
	if (hComponent == NULL)
	{
		std::cout << "CallCreateInstance:\tError: Cannot load component.\n";
		return NULL ;
	}

	// Get address for CreateInstance function.
	CREATEFUNCPTR CreateInstance 
		= (CREATEFUNCPTR)::GetProcAddress(hComponent, "CreateInstance") ;
	if (CreateInstance == NULL)
	{
		std::cout  << "CallCreateInstance:\tError: Cannot find CreateInstance function.\n";
		return NULL ;
	}

	return CreateInstance() ;
}
