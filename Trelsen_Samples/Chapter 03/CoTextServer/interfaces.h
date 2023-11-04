// IText contains methods which work with the COM string of choice.
//
#define WIN32_LEAN_AND_MEAN
#pragma warning (push, 3)
#include <windows.h>
#include <basetyps.h>
#include <WTypes.h>
#include <unknwn.h>
#pragma warning (pop)

DECLARE_INTERFACE_(IText, IUnknown)
{
	virtual HRESULT STDMETHODCALLTYPE Speak (BSTR bstrMessage) 	PURE;
	virtual HRESULT STDMETHODCALLTYPE GiveMeABSTR(BSTR * bstr) 	PURE;
};



