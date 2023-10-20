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
	= "Inside COM, Chapter 8 Example 2, Component 1" ;

// Version-independent ProgID
const char g_szVerIndProgID[] = "InsideCOM.Chap08.Ex2.Cmpnt1" ;

// ProgID
const char g_szProgID[] = "InsideCOM.Chap08.Ex2.Cmpnt1.1" ;

///////////////////////////////////////////////////////////
//
// Component A
//
class CA : public IX
        // public IY @N
{
public:
	// IUnknown
	IFACEMETHOD( QueryInterface) (const IID& iid, void** ppv) override;
	IFACEMETHOD_(ULONG, AddRef)() override;
	IFACEMETHOD_(ULONG, Release)() override;

	// Interface IX
	IFACEMETHOD(Fx)() override { trace("Fx"); return S_OK; }

	/* @N Component1 aggregates instead of implementing interface IY.
	// Interface IY
	IFACEMETHOD( Fy)() override {return  m_pIY->Fy() ;} //@N
	*/
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
	
	// Pointer to the aggregated component's IY interface
	// (We do not have to retain an IY pointer. However, we
	// can use it in QueryInterface.)
	IY* m_pIY ;                 // @N
	
	// Pointer to inner component's IUnknown
	IUnknown* m_pUnknownInner ; // @N
} ;


//
// Constructor
//
CA::CA() 
: m_cRef(1), 
  m_pUnknownInner(NULL) //@N
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

	// Prevent recursive destruction on next AddRef/Release pair.
	m_cRef = 1 ;

	// Counter the pUnknownOuter->Release in the Init method.
	IUnknown* pUnknownOuter = this ;
	pUnknownOuter->AddRef() ;

	// Properly release the pointer; there might be per-interface
	// reference counts.
	if (m_pIY != NULL)
	{
		m_pIY->Release() ;
	}

	// Release contained component.
	if (m_pUnknownInner != NULL)   // @N
	{
		m_pUnknownInner->Release() ;
	}
}

// Initialize the component by creating the contained component.
STDMETHODIMP CA::Init()
{
	// Get the pointer to the outer unknown.
	// Since this component is not aggregated, the outer unknown
	// is the same as the this pointer. 
	IUnknown* pUnknownOuter = this ;
	
	trace("Create inner component.") ;
	HRESULT hr =
		::CoCreateInstance(CLSID_Component2, 		 
		                   pUnknownOuter, // Outer component's IUnknown @N
		                   CLSCTX_INPROC_SERVER,
		                   IID_IUnknown,  // IUnknown when aggregating  @N
		                   (void**)&m_pUnknownInner) ; 
	if (FAILED(hr))
	{
		trace("Could not create contained component.") ;
		return E_FAIL ;
	}
	
	// This call will increment the reference count on the outer component.
	trace("Get the IY interface from the inner component.") ;
	hr = m_pUnknownInner->QueryInterface(IID_IY, (void**)&m_pIY) ; //@N
	if (FAILED(hr))
	{
		trace("Inner component does not support interface IY.") ;
		m_pUnknownInner->Release() ;
		m_pUnknownInner = NULL ;
		m_pIY = NULL ; // Just to be safe
		return E_FAIL ;
	}
	
	// We need to release the reference count added to the
	// outer component in the above call.  So call Release
	// on the pointer you passed to CoCreateInstance.
	pUnknownOuter->Release() ; //@N
	return S_OK ;
}

//
// IUnknown implementation
//
HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{ 	
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this) ; 
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this) ;
	}
	else if (iid == IID_IY)
	{
		trace("Return inner component's IY interface.") ;
#if 1
		// You can query for the interface.
		return m_pUnknownInner->QueryInterface(iid,ppv) ; //@N
#else
		// Or you can return a cached pointer.
		*ppv = m_pIY ;   //@N
		// Fall through so it will get AddRef'ed
#endif
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

ULONG __stdcall CA::AddRef()
{
  	auto ret = ::InterlockedIncrement(&m_cRef) ;
	std::cout << "External counter: " << ret << '\n';
	return ret;
}

ULONG __stdcall CA::Release() 
{
	if (::InterlockedDecrement(&m_cRef) == 0)
	{
		trace("External counter: 0");
		delete this ;
		return 0 ;
	}
	std::cout << "External counter: " << m_cRef << '\n';
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
	IUnknown* pI ;
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		pI = static_cast<IClassFactory*>(this) ; 
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	pI->AddRef() ;
	*ppv = pI ;
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
	// Cannot aggregate
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION ;
	}
	
	try
	{
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
HRESULT __stdcall CFactory::LockServer(BOOL bLock) 
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
	// Can we create this component?
	if (clsid != CLSID_Component1)
	{
		return CLASS_E_CLASSNOTAVAILABLE ;
	}
	try
	{
		// Create class factory.
		ATL::CComPtr <CFactory> pFactory;
		pFactory.Attach(new CFactory);

		// Get requested interface.
		return pFactory->QueryInterface(iid, ppv);
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
