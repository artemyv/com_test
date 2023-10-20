///////////////////////////////////////////////////////////
//
// CFactory
//   - Base class for reusing a single class factory for
//     all components in a DLL
//
#include <objbase.h>

#include "Registry.h"
#include "CFactory.h"
#include "UTIL.H"

static inline void trace(char* msg)
{
	Util::Trace("CFactory", msg, S_OK);
}

///////////////////////////////////////////////////////////
//
// Static variables
//
LONG CFactory::s_cServerLocks = 0 ;    // Count of locks

HMODULE CFactory::s_hModule = NULL ;   // DLL module handle

///////////////////////////////////////////////////////////
//
// CFactory implementation
//
CFactory::CFactory(const CFactoryData* pFactoryData)
: m_cRef(1)
{
	trace("Constructing");
	m_pFactoryData = pFactoryData ;
}

//
// IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void** ppv)
{ 	
	trace("QueryInterface");
	IUnknown* pI ;
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		trace("\tIID_IUnknown or IID_IClassFactory");
		pI= this ;
	}
	else
	{
		trace("\tnot supported");
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	trace("\tgoing to add ref on result");
	pI->AddRef() ;
	*ppv = pI ;
	return S_OK ;
}

ULONG __stdcall CFactory::AddRef()
{
	trace("AddRef");
	return ::InterlockedIncrement(&m_cRef) ;
}

ULONG __stdcall CFactory::Release()
{
	trace("Release");
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
HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
                                           const IID& iid,
                                           void** ppv)
{
	trace("CreateInstance");
	// Aggregate only if the requested IID is IID_IUnknown.
	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		trace("\tCLASS_E_NOAGGREGATION");
		return CLASS_E_NOAGGREGATION ;
	}

	// Create the component.
	CUnknown* pNewComponent ;
	trace("\t going to m_pFactoryData->CreateInstance");
	HRESULT hr = m_pFactoryData->CreateInstance(pUnknownOuter,
	                                            &pNewComponent) ;
	if (FAILED(hr))
	{
		return hr ;
	}

	trace("\t going to pNewComponent->Init");
	// Initialize the component.
	hr = pNewComponent->Init() ;
	if (FAILED(hr))
	{
		// Initialization failed.  Release the component.
		trace("\t\t Failed. going to pNewComponent->NondelegatingRelease");
		pNewComponent->NondelegatingRelease() ;
		return hr ;
	}

	trace("\t going to pNewComponent->NondelegatingQueryInterface");
	// Get the requested interface.
	hr = pNewComponent->NondelegatingQueryInterface(iid, ppv) ;

	trace("\t going to pNewComponent->NondelegatingRelease");
	// Release the reference held by the class factory.
	pNewComponent->NondelegatingRelease() ;
	return hr ;
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock) 
	{
		::InterlockedIncrement(&s_cServerLocks) ;
	}
	else
	{
		::InterlockedDecrement(&s_cServerLocks) ;
	}
	return S_OK ;
}

///////////////////////////////////////////////////////////
//
// GetClassObject
//   - Create a class factory based on a CLSID.
//
HRESULT CFactory::GetClassObject(const CLSID& clsid,
                                 const IID& iid,
                                 void** ppv)
{
	if ((iid != IID_IUnknown) && (iid != IID_IClassFactory))
	{
		return E_NOINTERFACE ;
	}

	// Traverse the array of data looking for this class ID.
	for (int i = 0; i < g_cFactoryDataEntries; i++)
	{
		const CFactoryData* pData = &g_FactoryDataArray[i] ;
		if (pData->IsClassID(clsid))
		{
			// Found the ClassID in the array of components we can
			// create.  So create a class factory for this component.
			// Pass the CFactoryData structure to the class factory
			// so that it knows what kind of components to create.
			*ppv = (IUnknown*) new CFactory(pData) ;
			if (*ppv == NULL)
			{
				return E_OUTOFMEMORY ;
			}
			return NOERROR ;
		}
	}
	return CLASS_E_CLASSNOTAVAILABLE ;
}

//
// Register all components.
//
HRESULT CFactory::RegisterAll()
{
	for(int i = 0 ; i < g_cFactoryDataEntries ; i++)
	{
		RegisterServer(s_hModule,
		               *(g_FactoryDataArray[i].m_pCLSID),
		               g_FactoryDataArray[i].m_RegistryName,
		               g_FactoryDataArray[i].m_szVerIndProgID,
		               g_FactoryDataArray[i].m_szProgID) ;
	}
	return S_OK ;
}

HRESULT CFactory::UnregisterAll()
{
	for(int i = 0 ; i < g_cFactoryDataEntries ; i++)
	{
		UnregisterServer(*(g_FactoryDataArray[i].m_pCLSID),
		                 g_FactoryDataArray[i].m_szVerIndProgID,
		                 g_FactoryDataArray[i].m_szProgID) ;

	}
	return S_OK ;
}

//
// Determine if the component can be unloaded.
//
HRESULT CFactory::CanUnloadNow()
{
	if (CUnknown::ActiveComponents() || IsLocked())
	{
		return S_FALSE ;
	}
	else
	{
		return S_OK ;
	}
}

//////////////////////////////////////////////////////////
//
// Exported functions
//

STDAPI DllCanUnloadNow()
{
	return CFactory::CanUnloadNow() ;
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
                         const IID& iid,
                         void** ppv)
{
	return CFactory::GetClassObject(clsid, iid, ppv) ;
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return CFactory::RegisterAll() ;
}


STDAPI DllUnregisterServer()
{
	return CFactory::UnregisterAll() ;
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
		CFactory::s_hModule = (HMODULE)hModule ;
	}
	return TRUE ;
}
