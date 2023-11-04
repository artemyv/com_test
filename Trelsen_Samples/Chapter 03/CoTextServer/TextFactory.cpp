// TextFactory.cpp: implementation of the TextFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "TextFactory.h"
#include "cotext.h"

extern ULONG g_lockCount;
extern ULONG g_objCount;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextFactory::TextFactory()
{
	m_refCount = 0;
	g_objCount++;
}

TextFactory::~TextFactory()
{
	g_objCount--;
}

// Class objects, being COM objects, maintain a reference count.
//
ULONG STDMETHODCALLTYPE TextFactory::AddRef(void)
{
	return ++m_refCount;
}
ULONG STDMETHODCALLTYPE TextFactory::Release()
{
	if(--m_refCount == 0) {
		delete this;
		return 0;
	}
	return m_refCount;
}

// Every single 'generic' class object will have a QueryInterface() implementation
// which looks like the following.  Again note that there are no provisions for
// the interfaces supported by the related coclass (CoHexagon).
//
STDMETHODIMP TextFactory::QueryInterface(REFIID riid, void** pIFace)
{
	if(riid == IID_IUnknown)
		*pIFace = (IUnknown*)this;
	else if(riid == IID_IClassFactory)
		*pIFace = (IClassFactory*)this;
	else {
		*pIFace = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*pIFace))->AddRef();
	return S_OK;
}


// Looks a lot like the CarFactory(), does it not?
// 
STDMETHODIMP TextFactory::CreateInstance(LPUNKNOWN pUnk, REFIID riid, void** pIFace)
{
	// We do not support aggregation in this class object.
	// If LPUNKNOWN is not NULL, return this standard HRESULT.
	if(pUnk != NULL)
		return CLASS_E_NOAGGREGATION;

	auto pObj = new CoText;

	// Ask object for an interface.
	auto hr = pObj->QueryInterface(riid, pIFace);

	// Problem?  We must delete the memory we allocated!
	if(FAILED(hr))
		delete pObj;

	return hr;
}


// LockServer() simply increments or decrements the server level global lock counter.
//
STDMETHODIMP TextFactory::LockServer(BOOL fLock)
{
	if(fLock)
		++g_lockCount;
	else
		--g_lockCount;

	return S_OK;
}
