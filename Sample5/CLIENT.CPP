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
	CoInitialize(NULL);

	trace("Call CoCreateInstance to create  component and get interface IX.");
	{
		ATL::CComPtr <IX> pIX;
		HRESULT hr = ::CoCreateInstance(CLSID_Component1,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IX,
			(void**)&pIX);
		if (SUCCEEDED(hr))
		{
			trace("Succeeded getting IX.");
			pIX->Fx();          // Use interface IX.

			trace("Ask for interface IY.");
			{
				ATL::CComPtr <IY>  pIY;
				hr = pIX->QueryInterface(IID_IY, (void**)&pIY);
				if (SUCCEEDED(hr))
				{
					trace("Succeeded getting IY.");
					pIY->Fy();       // Use interface IY.
					trace("Release IY interface.");
				}
				else
				{
					trace("Could not get interface IY.");
				}
			}

			trace("Ask for interface IZ.");
			{
				ATL::CComPtr <IZ> pIZ;
				hr = pIX->QueryInterface(IID_IZ, (void**)&pIZ);
				if (SUCCEEDED(hr))
				{
					trace("Succeeded in getting interface IZ.");
					pIZ->Fz();
					trace("Release IZ interface.");
				}
				else
				{
					trace("Could not get interface IZ.");
				}
			}
			trace("Release IX interface.");
		}
		else
		{
			std::cout << "Client: \t\tCould not create component. hr = "
				<< std::hex << hr << std::endl;
		}
	}

	// Uninitialize COM Library
	CoUninitialize();

	return 0;
}
