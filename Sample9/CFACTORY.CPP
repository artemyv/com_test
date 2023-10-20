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
#include <format>
static inline void trace(const char* msg)
{
	Util::Trace("CFactory", msg, S_OK);
}

///////////////////////////////////////////////////////////
//
// Static variables
//
LONG CFactory::s_cServerLocks = 0 ;    // Count of locks

HMODULE CFactory::s_hModule = NULL ;   // DLL module handle

#ifdef _OUTPROC_SERVER_
DWORD CFactory::s_dwThreadID = 0 ;
#endif

///////////////////////////////////////////////////////////
//
// CFactory implementation
//

CFactory::CFactory(const CFactoryData* pFactoryData)
: m_cRef(1)
{
	trace("Constructor");
	m_pFactoryData = pFactoryData ;
}

//
// IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void** ppv)
{ 	
	trace("CFactory::QueryInterface");
	IUnknown* pI ;
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		pI = this ; 
	}
	else
	{
	   *ppv = NULL;
		return E_NOINTERFACE;
	}
	pI->AddRef() ;
	*ppv = pI ;
	return S_OK;
}

ULONG __stdcall CFactory::AddRef() 
{ 
	trace("CFactory::AddRef");
	return ::InterlockedIncrement(&m_cRef) ;
}

ULONG __stdcall CFactory::Release() 
{
	trace("CFactory::Release");
	if (::InterlockedDecrement(&m_cRef) == 0)
	{
		delete this; 
		return 0 ;
	}   
	return m_cRef;
}

//
// IClassFactory implementation
//

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
                                           const IID& iid,
                                           void** ppv) 
{
	char szIID[CLSID_STRING_SIZE + 1] = " ";
	CLSIDtochar(iid, szIID + 1, sizeof(szIID) - 1);

	trace(std::format("CFactory::CreateInstance pUnknownOuter={}, iid={}", (void*)pUnknownOuter, szIID).c_str());

	// Aggregate only if the requested IID is IID_IUnknown.
	if ((pUnknownOuter != NULL) && (iid != IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION ;
	}

	// Create the component.
	CUnknown* pNewComponent ;
	HRESULT hr = m_pFactoryData->CreateInstance(pUnknownOuter,
	                                            &pNewComponent) ;
	if (FAILED(hr))
	{
		return hr ;
	}

	// Initialize the component.
	hr = pNewComponent->Init();
	if (FAILED(hr))
	{
		// Initialization failed.  Release the component.
		pNewComponent->NondelegatingRelease() ;
		return hr ;
	}
	
	// Get the requested interface.
	hr = pNewComponent->NondelegatingQueryInterface(iid, ppv) ;

	// Release the reference held by the class factory.
	pNewComponent->NondelegatingRelease() ;
	return hr ;   
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock) 
{
	trace("CFactory::LockServer");
	if (bLock)
	{
		::InterlockedIncrement(&s_cServerLocks) ; 
	}
	else
	{
		::InterlockedDecrement(&s_cServerLocks) ;
	}
	// If this is an out-of-proc server, check to see
	// whether we should shut down.
	CloseExe() ;  //@local

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
	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE + 1] = " ";
	CLSIDtochar(clsid, szCLSID + 1, sizeof(szCLSID) - 1);

	char szIID[CLSID_STRING_SIZE + 1] = " ";
	CLSIDtochar(iid, szIID + 1, sizeof(szIID) - 1);

	trace(std::format("CFactory::GetClassObject CLSID={} iid={}", szCLSID, szIID).c_str());

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
			// create. So create a class factory for this component.
			// Pass the CFactoryData structure to the class factory
			// so that it knows what kind of components to create.
			trace(std::format("Going to create factory for {}", pData->m_RegistryName).c_str());

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
// Determine if the component can be unloaded.
//
HRESULT CFactory::CanUnloadNow()
{
	trace("CFactory::CanUnloadNow");
	if (CUnknown::ActiveComponents() || IsLocked())
	{
		return S_FALSE ;
	}
	else
	{
		return S_OK ;
	}
}

//
// Register all components.
//
HRESULT CFactory::RegisterAll()
{
	trace("CFactory::RegisterAll");
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
	trace("CFactory::UnregisterAll");
	for(int i = 0 ; i < g_cFactoryDataEntries ; i++)
	{
		UnregisterServer(*(g_FactoryDataArray[i].m_pCLSID),
		                 g_FactoryDataArray[i].m_szVerIndProgID, 
		                 g_FactoryDataArray[i].m_szProgID) ;
	}
	return S_OK ;
}

#ifndef _OUTPROC_SERVER_
//////////////////////////////////////////////////////////
//
// Exported functions
//

STDAPI DllCanUnloadNow()
{
	trace("DllCanUnloadNow");
	return CFactory::CanUnloadNow() ;
}

//
// Get class factory
//
STDAPI DllGetClassObject(const CLSID& clsid,
                         const IID& iid,
                         void** ppv) 
{

	// Convert the CLSID into a char.
	char szCLSID[CLSID_STRING_SIZE+1] = " ";
	CLSIDtochar(clsid, szCLSID+1, sizeof(szCLSID)-1);

	char szIID[CLSID_STRING_SIZE + 1] = " ";
	CLSIDtochar(iid, szIID + 1, sizeof(szIID) - 1);

	trace(std::format("DllGetClassObject CLSID={} iid={}", szCLSID, szIID).c_str());

	return CFactory::GetClassObject(clsid, iid, ppv) ;
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	trace("DllRegisterServer");
	return CFactory::RegisterAll() ;
}


STDAPI DllUnregisterServer()
{
	trace("DllUnregisterServer");
	return CFactory::UnregisterAll() ;
}

///////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD dwReason, 
                      void* lpReserved )
{
	trace("DllMain");
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		trace("\tDLL_PROCESS_ATTACH");
		CFactory::s_hModule = (HMODULE) hModule ;
	}
	return TRUE ;
}

#else
//////////////////////////////////////////////////////////
//
// Out of process Server support
//

//
// Start factories
//
BOOL CFactory::StartFactories()
{
	CFactoryData* pStart = &g_FactoryDataArray[0] ;
	const CFactoryData* pEnd =
		&g_FactoryDataArray[g_cFactoryDataEntries - 1] ;

	for(CFactoryData* pData = pStart ; pData <= pEnd ; pData++)
	{
		// Initialize the class factory pointer and cookie.
		pData->m_pIClassFactory = NULL ;
		pData->m_dwRegister = NULL ;

		// Create the class factory for this component.
		IClassFactory* pIFactory = new CFactory(pData) ;

		// Register the class factory.
		DWORD dwRegister ;
		HRESULT hr = ::CoRegisterClassObject(
		                  *pData->m_pCLSID,
		                  static_cast<IUnknown*>(pIFactory),
		                  CLSCTX_LOCAL_SERVER,
		                  REGCLS_MULTIPLEUSE,
		                  // REGCLS_MULTI_SEPARATE, //@Multi
		                  &dwRegister) ;
		if (FAILED(hr))
		{
			pIFactory->Release() ;
			return FALSE ;
		}

		// Set the data.
		pData->m_pIClassFactory = pIFactory ;
		pData->m_dwRegister = dwRegister ;
	}
	return TRUE ;
}

//
// Stop factories
//
void CFactory::StopFactories()
{
	CFactoryData* pStart = &g_FactoryDataArray[0] ;
	const CFactoryData* pEnd =
		&g_FactoryDataArray[g_cFactoryDataEntries - 1] ;

	for (CFactoryData* pData = pStart ; pData <= pEnd ; pData++)
	{
		// Get the magic cookie and stop the factory from running.
		DWORD dwRegister = pData->m_dwRegister ;
		if (dwRegister != 0) 
		{
			::CoRevokeClassObject(dwRegister) ;
		}

		// Release the class factory.
		IClassFactory* pIFactory  = pData->m_pIClassFactory ;
		if (pIFactory != NULL) 
		{
			pIFactory->Release() ;
		}
	}
}

#endif //_OUTPROC_SERVER_
