//
// Client.cpp - client implementation
//
#include <iostream>
#include <objbase.h>
#include <atlbase.h>

#include "Iface.h"

void trace(const char* msg) { std::cout << "Client: \t\t" << msg << std::endl ;}

//
// main function
//
int main()
{
	// Initialize COM Library
	CoInitialize(NULL) ;
	
	trace("Get interface IX from Component 1.") ;
	{
		ATL::CComPtr <IX> pIX;
		HRESULT hr = ::CoCreateInstance(CLSID_Component1,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IX,
			(void**)&pIX);
		if (SUCCEEDED(hr))
		{
			trace("Succeeded creating component.");
			pIX->Fx();

			trace("Get interface IY from IX.");
			{
				ATL::CComPtr <IY>  pIY;
				hr = pIX->QueryInterface(IID_IY, (void**)&pIY);
				if (SUCCEEDED(hr))
				{
					trace("Succeeded getting interface IY from IX.");
					pIY->Fy();

					trace("Get interface IX from IY.");
					{
						ATL::CComPtr <IX> pIX2;
						hr = pIY->QueryInterface(IID_IX, (void**)&pIX2);
						if (SUCCEEDED(hr))
						{
							trace("Succeeded getting interface IX from IY.");
						}
						else
						{
							trace("Error! Should have gotten interface IX.");
						}
					}
				}
				else
				{
					trace("Could not get interface IY.");
				}
			}
		}
		else
		{
			std::cout << "Could not create component: " << std::hex << hr << std::endl;
		}
	}
	
	// Uninitialize COM Library
	CoUninitialize() ;
	
	return 0 ;
}
