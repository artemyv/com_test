//
// Cmpnt2.cpp - Component 2
//   Note the changes in the class factory marked with @N.
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
	= "Inside COM, Chapter 8 Example 2, Component 2" ;

// Version-independent ProgID
const char g_szVerIndProgID[] = "InsideCOM.Chap08.Ex2.Cmpnt2" ;

// ProgID
const char g_szProgID[] = "InsideCOM.Chap08.Ex2.Cmpnt2.1" ;

///////////////////////////////////////////////////////////
//
// Nondelegating IUnknown interface  @N
//
DECLARE_INTERFACE( INondelegatingUnknown)
{
	BEGIN_INTERFACE
		STDMETHOD( NondelegatingQueryInterface)(const IID&, void**) PURE ;
		STDMETHOD_( ULONG, NondelegatingAddRef)() PURE;
		STDMETHOD_( ULONG , NondelegatingRelease)() PURE;
	END_INTERFACE
} ;

///////////////////////////////////////////////////////////
//
// Component 
//
class CB : public IY,
           public INondelegatingUnknown
{
public:
	// Delegating IUnknown
	IFACEMETHOD(QueryInterface)(const IID& iid, void** ppv) override
	{
		trace("Delegate QueryInterface.") ;
		return m_pUnknownOuter->QueryInterface(iid, ppv) ; 
	}

	IFACEMETHOD_(ULONG, AddRef)() override
	{
		trace("Delegate AddRef.") ;
		return m_pUnknownOuter->AddRef() ; 
	}

	virtual ULONG __stdcall Release() override
	{
		trace("Delegate Release.") ;
		return m_pUnknownOuter->Release() ; 
	}

	// Nondelegating IUnknown
	IFACEMETHOD(NondelegatingQueryInterface)(const IID& iid, void** ppv) override ;
	IFACEMETHOD_(ULONG, NondelegatingAddRef)() override;
	IFACEMETHOD_(ULONG, NondelegatingRelease)() override;

	// Interface IY
	IFACEMETHOD(Fy)() override { trace("Fy"); return S_OK; }

	// Constructor
	CB(IUnknown* m_pUnknownOuter) ;

	// Destructor
	~CB() ;

private:
	long m_cRef ;
	
	IUnknown* m_pUnknownOuter ;
} ;

//
// IUnknown implementation
//
IFACEMETHODIMP CB::NondelegatingQueryInterface(const IID& iid,
                                                  void** ppv)
{ 	
	if (iid == IID_IUnknown)
	{
		// !!! CAST IS VERY IMPORTANT !!!
		*ppv = static_cast<INondelegatingUnknown*>(this) ;  // @N
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

IFACEMETHODIMP_(ULONG) CB::NondelegatingAddRef()
{
	auto ret = ::InterlockedIncrement(&m_cRef) ;
	std::cout << "Internal counter: " << ret << "\n";
	return ret;
}

IFACEMETHODIMP_(ULONG) CB::NondelegatingRelease()
{
	if (::InterlockedDecrement(&m_cRef) == 0)
	{
		trace("Internal counter 0");
		delete this ;
		return 0 ;
	}
	std::cout << "Internal counter: " << m_cRef << "\n";
	return m_cRef ;
}

//
// Constructor
//
CB::CB(IUnknown* pUnknownOuter) 
: m_cRef(1)
{ 
	::InterlockedIncrement(&g_cComponents) ; 

	if (pUnknownOuter == NULL)
	{
		trace("Not aggregating; delegate to nondelegating IUnknown.") ;
		m_pUnknownOuter = reinterpret_cast<IUnknown*>
		                  (static_cast<INondelegatingUnknown*>
		                  (this)) ;
	}
	else
	{
		trace("Aggregating; delegate to outer IUnknown.") ;
		m_pUnknownOuter = pUnknownOuter ;
	}
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
	CFactory() : m_cRef(1) {
		trace("CFactory constructor");
	}

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
	trace("CFactory QueryInterface");
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this) ; 
	}
	else
	{
		trace("CFactory Wrong interface");
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
	// Aggregate only if the requested iid is IID_IUnknown.
	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown)) //@N
	{
		trace("CFactory CreateInstance - Wrong interface");
		return CLASS_E_NOAGGREGATION ;
	}
	
	// Create component.
	try {
		auto pB = new CB(pUnknownOuter);
		// Get the requested interface.
		HRESULT hr = pB->NondelegatingQueryInterface(iid, ppv); //@N
		pB->NondelegatingRelease();
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
	try {
		// Can we create this component?
		if (clsid != CLSID_Component2)
		{
			trace("Wrong clsid");
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
