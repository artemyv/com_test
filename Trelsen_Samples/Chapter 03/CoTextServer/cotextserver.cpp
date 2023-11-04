#include "textfactory.h"
#include "interfaces.h"
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include "iids.h"

ULONG g_lockCount = 0;
ULONG g_objCount = 0;

// DllGetClassObject() is in charge of creating a class factory, and returning the 
// IClassFactory interface to the COM client.
//
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	HRESULT hr;
	TextFactory* pFact = NULL;

	// We only know how to make CoHexagon objects in this house.
	if(rclsid != CLSID_CoText)
		return CLASS_E_CLASSNOTAVAILABLE;

	// They want a CoHexagon, so create the hexagon factory.
	pFact = new TextFactory;

	// Go get the interface from the Hex Factory (IClassFactory(2) or IUnknown)
	hr = pFact->QueryInterface(riid, ppv);

	if(FAILED(hr))
		delete pFact;
	return hr;
}

STDAPI DllCanUnloadNow(void)
{
	if(g_lockCount == 0 && g_objCount == 0) {
		MessageBoxW(NULL, L"CoText dead", L"Message", MB_OK | MB_SETFOREGROUND);
		return S_OK;		// Unload me.	
	}
	else
		return S_FALSE;	// Keep me alive.
}
