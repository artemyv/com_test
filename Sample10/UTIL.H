#ifndef __Util_h__
#define __Util_h__

//
// Util.h - Shared utilities
//
#include <ostream>

namespace Util
{
	void Trace(const char* szLabel, const char* szText, HRESULT hr) ;

	void ErrorMessage(HRESULT hr) ;
} ;


//
// Overloaded insertion operator for converting from
// Unicode (wchar_t) to non-Unicode.
//
std::ostream& operator<< ( std::ostream& os, const wchar_t* wsz ) ;

#endif // __Util_h__