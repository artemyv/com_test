//
// Cmpnt2.cpp - Component 2
//
#include <iostream>
#include <objbase.h>
#include <atlbase.h>

#include "Iface.h"
#include "Registry.h"

void trace(const char* msg) { std::cout << "Component 2:\t" << msg << std::endl ;}

///////////////////////////////////////////////////////////
//
// Global variables
//

// Static variables
static HMODULE g_hModule = NULL ;  // DLL module handle
static long g_cComponents = 0 ;    // Count of active components
static long g_cServerLocks = 0 ;   // Count of locks

// Friendly name of component
const char g_szFriendlyName[]
	= "Inside COM, Chapter 8 Example 1, Component 2" ;

// Version-independent ProgID
const char g_szVerIndProgID[] = "InsideCOM.Chap08.Ex1.Cmpnt2" ;

// ProgID
const char g_szProgID[] = "InsideCOM.Chap08.Ex1.Cmpnt2.1" ;

///////////////////////////////////////////////////////////
//
// Component B
//
class CB : public IY
{
public:
	// IUnknown
	IFACEMETHOD(QueryInterface) (const IID& iid, void** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IY
	IFACEMETHOD(Fy)() override { trace("Fy"); return S_OK; }

	// Constructor
	CB() ;

	// Destructor
	~CB() ;

private:
	// Reference count
	long m_cRef ;
} ;

//
// IUnknown implementation
//
IFACEMETHODIMP CB::QueryInterface(const IID& iid, void** ppv)
{ 	
	if (iid == IID_IUnknown)
	{
		*ppv = this ;
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this) ;
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

IFACEMETHODIMP_(ULONG) CB::AddRef()
{
	return ::InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CB::Release()
{
	if (::InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}


//
// Constructor
//
CB::CB() : m_cRef(1) 
{ 
	::InterlockedIncrement(&g_cComponents) ; 
}

//
// Destructor
//
CB::~CB() 
{ 
	::InterlockedDecrement(&g_cComponents) ; 
	trace("Destroy self.") ;
}


///////////////////////////////////////////////////////////
//
// Class factory
//
class CFactory : public IClassFactory
{
public:
	// IUnknown
	IFACEMETHOD(QueryInterface)(const IID& iid, void** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IClassFactory
	IFACEMETHOD(CreateInstance)(IUnknown* pUnknownOuter,
		const IID& iid,
		void** ppv) override;
	IFACEMETHOD(LockServer)(BOOL bLock) override;

	// Constructor
	CFactory() : m_cRef(1) {}

	// Destructor
	~CFactory() {}

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

IFACEMETHODIMP_(ULONG) CFactory::AddRef()
{
	return ::InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CFactory::Release()
{
	if (::InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//
// IClassFactory implementation
//
IFACEMETHODIMP CFactory::CreateInstance(IUnknown* pUnknownOuter,
                                           const IID& iid,
                                           void** ppv) 
{
	try
	{
		// Cannot aggregate
		if (pUnknownOuter != NULL)
		{
			return CLASS_E_NOAGGREGATION;
		}

		// Create component.
		ATL::CComPtr <CB> pB;
		pB.Attach(new CB);

		// Get the requested interface.
		HRESULT hr = pB->QueryInterface(iid, ppv);
		return hr;
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}
}

IFACEMETHODIMP CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		::InterlockedIncrement(&g_cServerLocks) ;
	}
	else
	{
		::InterlockedDecrement(&g_cServerLocks) ;
	}
	return S_OK ;
}


///////////////////////////////////////////////////////////
//
// Exported functions
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
// Get class factory.
//
STDAPI DllGetClassObject(const CLSID& clsid,
	const IID& iid,
	void** ppv)
{
	try {
		// Can we create this component?
		if (clsid != CLSID_Component2)
		{
			return CLASS_E_CLASSNOTAVAILABLE;
		}
		// Create class factory.
		ATL::CComPtr <CFactory> pFactory;
		pFactory.Attach(new CFactory);

		// Get requested interface.
		HRESULT hr = pFactory->QueryInterface(iid, ppv);
		return hr;
	}
	catch (...)
	{
		return E_OUTOFMEMORY;
	}
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule, 
	                      CLSID_Component2, 
	                      g_szFriendlyName,
	                      g_szVerIndProgID,
	                      g_szProgID) ;
}


STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component2,
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
