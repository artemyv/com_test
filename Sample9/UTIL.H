#ifndef __Util_h__
#define __Util_h__

//
// Util.h - Shared utilities
//
#include <ostream>

namespace Util
{
	void Trace(const char* szLabel,const  char* szText, HRESULT hr) ;

} ;


//
// Overloaded insertion operator for converting from
// Unicode (wchar_t) to non-Unicode.
//
std::ostream& operator<< ( std::ostream& os, const wchar_t* wsz ) ;

// Convert a CLSID into a char string.
void CLSIDtochar(const CLSID& clsid,
	char* szCLSID,
	int length);

// Size of a CLSID as a string
static const int CLSID_STRING_SIZE = 39;

#endif // __Util_h__