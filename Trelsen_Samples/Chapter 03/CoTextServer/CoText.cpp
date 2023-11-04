// CoText.cpp: implementation of the CoText class.
//
//////////////////////////////////////////////////////////////////////

#include "CoText.h"
#include "iids.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// Because we are not in a Unicode build, we need to flip between string types.
//
STDMETHODIMP CoText::Speak(BSTR bstrMessage)
{
	//char buff[80];

	//// Transform the Unicode BSTR into an ANSI char* 
	//WideCharToMultiByte(CP_ACP, NULL, bstrMessage,
	//	-1, buff, 80, NULL, NULL);

	// Now put ANSI string (buff) into a message box.
	MessageBoxW(NULL, bstrMessage, L"The BSTR is...", MB_OK | MB_SETFOREGROUND);
	return S_OK;
}


// GiveMeABSTR() creates a copy of the internal underlying BSTR.
//
STDMETHODIMP CoText::GiveMeABSTR(BSTR* bstrMessage)
{
	*bstrMessage = SysAllocString(m_bstr);	// Client frees the memory.
	return S_OK;
}

// AddRef() is used to increase the internal reference count of the object.
//
STDMETHODIMP_(ULONG) CoText::AddRef()
{
	return ++m_refCount;		// Private unsigned long defined in CoHexagon.
}

// Release() is called to decrement the internal reference count on the object.
// If (and only if) the final reference to the object has be released, the coclass
// removes itself from memory.
//
STDMETHODIMP_(ULONG) CoText::Release()
{
	--m_refCount;

	// Did I loose my final connection?
	if(m_refCount == 0) {
		// Final Release!  Remove coclass from memory.
		delete this;
		return 0;
	}

	return m_refCount;
}


// QueryInterface() is implemented by each and every coclass, and provides
// a way to return an interface pointer to the client.
//
STDMETHODIMP CoText::QueryInterface(REFIID riid, void** ppv)
{
	// Which interface does the client want?
	if(riid == IID_IUnknown)		// Human readable version of GUID 
		*ppv = (IUnknown*)this;
	else if(riid == IID_IText)
		*ppv = (IText*)this; 		// Generalized  "*ppv = (IDraw*)theRect"

	if(*ppv)	// Did we have the IID?
	{
		((IUnknown*)(*ppv))->AddRef();		// Must call AddRef() if we hand out an interface.
		return S_OK;
	}

	return E_NOINTERFACE;	// Standard return if we don't support the IID.
}
