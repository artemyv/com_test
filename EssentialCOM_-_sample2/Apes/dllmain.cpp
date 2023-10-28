// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "DllModule.h"
#include <atlbase.h>
#include "Apes_h.h"
#include <print>

class ApesLibModule _AtlModule;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return _AtlModule.DllMain(ul_reason_for_call, lpReserved);
}

//////////////////////////////////////////////////////////
//
// Exported functions
//

STDAPI DllCanUnloadNow()
{
    std::println("DllCanUnloadNow");
    return _AtlModule.DllCanUnloadNow();
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
    const IID& iid,
    void** ppv)
{
    std::println("DllGetClassObject");
    return _AtlModule.DllGetClassObject(clsid, iid, ppv);
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
    std::println("DllRegisterServer");
    return _AtlModule.DllRegisterServer(false);
}


STDAPI DllUnregisterServer()
{
    std::println("DllUnregisterServer");
    return _AtlModule.DllUnregisterServer(false);
}
