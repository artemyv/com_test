//
// Client1.cpp
// To compile, use: cl Client1.cpp Create.cpp GUIDs.cpp UUID.lib
//
#include <iostream>
#include <objbase.h>

#include "Iface.h"
#include "Create.h"

void trace(const char* msg) { std::cout << "Client 1:\t" << msg << std::endl ;}

//
// Client1
//
int main()
{
	HRESULT hr ;

	// Get the name of the component to use.
	char name[40] ;
	std::cout << "Enter the filename of a component to use [Cmpnt?.dll]: " ;
	std::cin  >> name ;
	std::cout << std::endl ;

	// Create component by calling the CreateInstance function in the DLL.
	trace("Get an IUnknown pointer.") ;
	IUnknown* pIUnknown = CallCreateInstance(name) ; 
	if (pIUnknown == NULL)
	{
		trace("CallCreateInstance Failed.") ;
		return 1 ;
	}

	trace("Get interface IX.") ;

	IX* pIX ; 
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX) ;

	if (SUCCEEDED(hr))
	{
		trace("Succeeded getting IX.") ;
		pIX->Fx() ;          // Use interface IX.
		pIX->Release() ;
	}
	else
	{
		trace("Could not get interface IX.") ;
	}

	trace("Release IUnknown interface.") ;
	pIUnknown->Release() ;

	return 0 ;
}


