// IText contains methods which work with the COM string of choice.
//
#include <windows.h>

DECLARE_INTERFACE_(IText, IUnknown)
{
	STDMETHOD(Speak) (BSTR bstrMessage) 	PURE;	
	STDMETHOD(GiveMeABSTR) (BSTR* bstr) 	PURE;
};



