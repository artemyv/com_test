///////////////////////////////////////////////////////////
//
// CUnknown.cpp 
//   - Implementation of IUnknown base class
//
#include "CUnknown.h"
#include "Util.h"

static inline void trace(char* msg)
	{Util::Trace("CUnknown", msg, S_OK) ;} 
static inline void trace(char* msg, HRESULT hr)
	{Util::Trace("CUnknown", msg, hr) ;}

static inline void trace(void* p, char* msg, int count)
{
	Util::Trace("CUnknown for ", p, msg, count);
}

///////////////////////////////////////////////////////////
//
// Count of active objects
//   - Use to determine if we can unload the DLL.
//
long CUnknown::s_cActiveComponents = 0 ;


///////////////////////////////////////////////////////////
//
// Constructor
//
CUnknown::CUnknown(IUnknown* pUnknownOuter)
: m_cRef(1)
{
	trace(this, "Init to ", 1);
	// Set m_pUnknownOuter pointer.
	if (pUnknownOuter == NULL)
	{
		trace("Not aggregating; delegate to nondelegating IUnknown.") ;
		m_pUnknownOuter = reinterpret_cast<IUnknown*>
		                     (static_cast<INondelegatingUnknown*>
		                     (this)) ;  // notice cast
	}
	else
	{
		trace("Aggregating; delegate to outer IUnknown.") ;
		m_pUnknownOuter = pUnknownOuter ;
	}

	// Increment count of active components.
	::InterlockedIncrement(&s_cActiveComponents) ;
}

//
// Destructor
//
CUnknown::~CUnknown()
{
	InterlockedDecrement(&s_cActiveComponents) ;
}

//
// FinalRelease - called by Release before it deletes the component
//
void CUnknown::FinalRelease()
{
	trace("Increment reference count for final release.") ;
	m_cRef = 1 ;
	trace(this, "forced to ", 1);
}

//
// Nondelegating IUnknown
//   - Override to handle custom interfaces.
//
HRESULT __stdcall 
	CUnknown::NondelegatingQueryInterface(const IID& iid, void** ppv)
{
	// CUnknown supports only IUnknown.
	if (iid == IID_IUnknown)
	{
		return FinishQI(reinterpret_cast<IUnknown*>
		                   (static_cast<INondelegatingUnknown*>(this)), ppv) ;
	}	
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
}

//
// AddRef
//
ULONG __stdcall CUnknown::NondelegatingAddRef()
{
	auto ret = ::InterlockedIncrement(&m_cRef) ;

	trace(this, "internal add to ", m_cRef);

	return ret;
}

//
// Release
//
ULONG __stdcall CUnknown::NondelegatingRelease()
{
	auto ret = ::InterlockedDecrement(&m_cRef) ;
	trace(this, "internal release to ", ret);
	if (ret == 0)
	{
		FinalRelease() ;
		delete this ;
		return 0 ;
	}
	return ret;
}

//
// FinishQI
//   - Helper function to simplify overriding
//     NondelegatingQueryInterface
//
HRESULT CUnknown::FinishQI(IUnknown* pI, void** ppv) 
{
	trace(this, "FinishQI from ", m_cRef);
	*ppv = pI ;
	pI->AddRef() ;
	return S_OK ;
}
