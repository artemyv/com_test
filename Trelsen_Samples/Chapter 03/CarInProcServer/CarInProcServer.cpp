/*  This file implements the minimal and complete 
 *	DLL exports for an in-process server.
 */

#include "cocarclassfactory.h"
#include "iid.h"

// Need these global data points to figure
// out when the DLL can get unloaded.
ULONG g_lockCount = 0;
ULONG g_objCount = 0;


// Called by SCM and CoFreeUnusedLibraries() to see
// if this DLL can be removed from memory.
STDAPI DllCanUnloadNow()
{
	if(g_lockCount == 0 && g_objCount == 0)
	{
		MessageBox(NULL, "Goodbye from DllCanUnloadNow!", "CarInProcServer", MB_OK | MB_SETFOREGROUND);
		return S_OK;		// Unload me.	
	}
	else
		return S_FALSE;		// Keep me alive.
}


// Called by SCM in responce to CoGetClassObject() or
// CoCreateInstance().  Creates a given class factory for
// the client based on the CLSID of the coclass.
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	MessageBox(NULL, "Hello from DllGetClassObject!", "CarInProcServer", MB_OK | MB_SETFOREGROUND);
	HRESULT hr;
	CoCarClassFactory *pCFact = NULL;
	
	// We only know how to make CoHexagon objects in this house.
	if(rclsid != CLSID_CoCar)
		return CLASS_E_CLASSNOTAVAILABLE;
 
	// They want a CoCarClassFactory
	pCFact = new CoCarClassFactory;	

	// Go get the interface from the CoCarClassFactory
	hr = pCFact -> QueryInterface(riid, ppv);

	if(FAILED(hr))
		delete pCFact;
	
	return hr;
}