#ifndef __Util_h__
#define __Util_h__
//
// Util.h
//   - Shared utilities
//
namespace Util
{
	void Trace(char* szLabel, const char* szText, HRESULT hr) ;
	void Trace(char* szLabel, void* pointer, const char* szText, int count);

	void ErrorMessage(HRESULT hr) ;
} ;

#endif // __Util_h__
