//
// Cmpnt.cpp
//
#include <iostream>
#include <objbase.h>
#include <atlbase.h>

#include "Iface.h"      // Interface declarations
#include "Registry.h"   // Registry helper functions

// Trace function
void trace(const char* msg) { std::cout << msg << std::endl ;}

///////////////////////////////////////////////////////////
//
// Global variables
//
static HMODULE g_hModule = NULL ;   // DLL module handle
static long g_cComponents = 0 ;     // Count of active components
static long g_cServerLocks = 0 ;    // Count of locks

// Friendly name of component
static constexpr const char g_szFriendlyName[] = "Inside COM, Chapter 7 Example" ;

// Version-independent ProgID
static constexpr const char g_szVerIndProgID[] = "InsideCOM.Chap07" ;

// ProgID
static constexpr const char g_szProgID[] = "InsideCOM.Chap07.1" ;


///////////////////////////////////////////////////////////
//
// Component 
//
class CA : public IX,
           public IY 
{
public:
	// IUnknown
	IFACEMETHOD( QueryInterface) (const IID& iid, void** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IX
	IFACEMETHOD(Fx)() override { trace("Fx"); return S_OK; }

	// Interface IY
	IFACEMETHOD(Fy)() override { trace("Fy"); return S_OK; }

	// Constructor
	CA() ;

	// Destructor
	~CA() ;

private:
	// Reference count
	long m_cRef ;
} ;


//
// Constructor
//
CA::CA() : m_cRef(1)
{ 
	InterlockedIncrement(&g_cComponents) ; 
}

//
// Destructor
//
CA::~CA() 
{ 
	InterlockedDecrement(&g_cComponents) ; 
	trace("Component:\t\tDestroy self.") ;
}

//
// IUnknown implementation
//
IFACEMETHODIMP CA::QueryInterface(const IID& iid, void** ppv)
{    
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this) ; 
		trace("Component:\t\tReturn pointer to IUnknown.");
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this) ;
		trace("Component:\t\tReturn pointer to IX.") ; 
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this) ; 
		trace("Component:\t\tReturn pointer to IY.") ; 
	}
	else
	{
		*ppv = NULL ;
		trace("Component:\t\tInterface not supported.");
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

IFACEMETHODIMP_(ULONG) CA::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

IFACEMETHODIMP_(ULONG) CA::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}


///////////////////////////////////////////////////////////
//
// Class factory
//
class CFactory : public IClassFactory
{
public:
	// IUnknown
	IFACEMETHOD( QueryInterface)(const IID& iid, void** ppv) override ;
	IFACEMETHOD_( ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IClassFactory
	IFACEMETHOD( CreateInstance)(IUnknown* pUnknownOuter,
	                                         const IID& iid,
	                                         void** ppv) override ;
	IFACEMETHOD( LockServer)(BOOL bLock) override;

	// Constructor
	CFactory() : m_cRef(1) {}

	// Destructor
	~CFactory() { trace("Class factory:\t\tDestroy self.") ;}

private:
	long m_cRef ;
} ;

//
// Class factory IUnknown implementation
//
IFACEMETHODIMP CFactory::QueryInterface(const IID& iid, void** ppv)
{    
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this) ; 
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

IFACEMETHODIMP_(ULONG ) CFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

IFACEMETHODIMP_(ULONG) CFactory::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}

//
// IClassFactory implementation
//
IFACEMETHODIMP CFactory::CreateInstance(IUnknown* pUnknownOuter,
                                           const IID& iid,
                                           void** ppv) 
{
	trace("Class factory:\t\tCreate component.") ;

	// Cannot aggregate.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION ;
	}
	try
	{
		// Create component.
		ATL::CComPtr <CA> pA;
		pA.Attach(new CA);

		// Get the requested interface.
		HRESULT hr = pA->QueryInterface(iid, ppv);

		// Release the IUnknown pointer.
		// (If QueryInterface failed, component will delete itself.)
		return hr;
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}
}

// LockServer
IFACEMETHODIMP CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks) ; 
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks) ;
	}
	return S_OK ;
}


///////////////////////////////////////////////////////////
//
// Exported functions
//

//
// Can DLL unload now?
//
STDAPI DllCanUnloadNow()
{
	if ((g_cComponents == 0) && (g_cServerLocks == 0))
	{
		return S_OK ;
	}
	else
	{
		return S_FALSE ;
	}
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
                         const IID& iid,
                         void** ppv)
{
	trace("DllGetClassObject:\tCreate class factory.") ;

	// Can we create this component?
	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE ;
	}

	// Create class factory.
	CFactory* pFactory = new CFactory ;  // Reference count set to 1
	                                     // in constructor
	if (pFactory == NULL)
	{
		return E_OUTOFMEMORY ;
	}

	// Get requested interface.
	HRESULT hr = pFactory->QueryInterface(iid, ppv) ;
	pFactory->Release() ;

	return hr ;
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, 
	                      CLSID_Component1,
	                      g_szFriendlyName,
	                      g_szVerIndProgID,
	                      g_szProgID) ;
}


//
// Server unregistration
//
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component1,
	                        g_szVerIndProgID,
	                        g_szProgID) ;
}

///////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD dwReason,
                      void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule ;
	}
	return TRUE ;
}
