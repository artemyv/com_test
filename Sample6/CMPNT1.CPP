//
// Cmpnt1.cpp - Component 1
//
//   Interesting bits of code marked with @N.
//
#include <iostream>
#include <objbase.h>
#include <atlbase.h>

#include "Iface.h"
#include "Registry.h"

// Trace function
void trace(const char* msg) { std::cout << "Component 1:\t" << msg << std::endl ;}

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
	= "Inside COM, Chapter 8 Example 1, Component 1" ;

// Version-independent ProgID
const char g_szVerIndProgID[] = "InsideCOM.Chap08.Ex1.Cmpnt1" ;

// ProgID
const char g_szProgID[] = "InsideCOM.Chap08.Ex1.Cmpnt1.1" ;


///////////////////////////////////////////////////////////
//
// Component A
//
class CA : public IX,
           public IY //@N
{
public:
	// IUnknown
	IFACEMETHOD( QueryInterface) (const IID& iid, void** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IX
	IFACEMETHOD(Fx)() override { trace("Fx"); return S_OK; }

	// Interface IY
	IFACEMETHOD( Fy)() override {return  m_pIY->Fy() ;} //@N

	// Constructor
	CA() ;

	// Destructor
	~CA() ;

	// Initialization function called by the class factory
	// to create the contained component
	STDMETHOD (Init)() ;     //@N

private:
	// Reference count
	long m_cRef ;

	// Pointer to the contained component's IY interface
	ATL::CComPtr <IY> m_pIY ;
} ;


//
// Constructor
//
CA::CA()
: m_cRef(1),
  m_pIY() //@N
{
	::InterlockedIncrement(&g_cComponents) ;
}

//
// Destructor
//
CA::~CA() 
{ 
	::InterlockedDecrement(&g_cComponents) ; 
	trace("Destroy self.") ;
}

// Initialize the component by creating the contained component.
STDMETHODIMP CA::Init()
{
	trace("Create contained component.") ;
	HRESULT hr = ::CoCreateInstance(CLSID_Component2, 
	                                NULL, 
	                                CLSCTX_INPROC_SERVER,
	                                IID_IY,
	                                (void**)&m_pIY) ;
	if (FAILED(hr))
	{
		trace("Could not create contained component.") ;
		return E_FAIL ;
	}
	else
	{
		return S_OK ;
	}
}

//
// IUnknown implementation
//
IFACEMETHODIMP CA::QueryInterface(const IID& iid, void** ppv)
{ 	
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this) ;
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this) ;
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

IFACEMETHODIMP_(ULONG) CA::AddRef()
{
	return ::InterlockedIncrement(&m_cRef) ;
}

IFACEMETHODIMP_(ULONG) CA::Release()
{
	if (::InterlockedDecrement(&m_cRef) == 0)
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
		*ppv = static_cast<IClassFactory*>(this) ; 
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
	return ::InterlockedIncrement(&m_cRef) ;
}

IFACEMETHODIMP_(ULONG) CFactory::Release()
{
	if (::InterlockedDecrement(&m_cRef) == 0)
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
	try
	{
		// Cannot aggregate
		if (pUnknownOuter != NULL)
		{
			return CLASS_E_NOAGGREGATION;
		}
		// Create component.
		ATL::CComPtr <CA> pA;
		pA.Attach(new CA);
		// Initialize the component. @N
		HRESULT hr = pA->Init();
		if (FAILED(hr))
		{
			// Initialization failed. Delete component.
			return hr;
		}

		// Get the requested interface.
		hr = pA->QueryInterface(iid, ppv);
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
		if (clsid != CLSID_Component1)
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
	                      CLSID_Component1, 
	                      g_szFriendlyName,
	                      g_szVerIndProgID,
	                      g_szProgID) ;
}


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
