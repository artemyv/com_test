#pragma once
#include <atlbase.h>
#include "Apes_h.h"

class ApesLibModule : 
    public ATL::CAtlComModule,
    public ATL::CAtlDllModuleT<ApesLibModule>
{
public:
    using ATL::CAtlComModule::UnregisterServer;
    using ATL::CAtlComModule::RegisterServer;
    ApesLibModule() {}
    DECLARE_LIBID(LIBID_ApeLib);
private:
};

extern class ApesLibModule _AtlModule;
