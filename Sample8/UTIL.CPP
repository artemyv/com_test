//
// Util.cpp
//   - Common utilities for printing out messages
//
#include <objbase.h>
#include <iostream>

#include "Util.h"

namespace Util
{
//
// Print out a message with a label.
//
void Trace(char* szLabel, const char* szText, HRESULT hr)
{
	std::cout << szLabel
	     << ": \t"
	     << szText 
	     << '\n';

	if (FAILED(hr))
	{
		ErrorMessage(hr) ;
	}

}

void Trace(char* szLabel, void* pointer, const char* szText, int count)
{
	std::cout << szLabel << pointer
		<< ": \t"
		<< szText << count
		<< '\n';
}

//
// Print out the COM/OLE error string for an HRESULT.
//
void ErrorMessage(HRESULT hr)
{
	void* pMsgBuf ;
 
	::FormatMessageA( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &pMsgBuf,
		0,
		NULL) ;

	std::cout << "Error (" << std::hex << hr << "):  " 
		<< (LPTSTR)pMsgBuf << '\n';

	// Free the buffer.
	LocalFree(pMsgBuf) ;

}

} ; // End Namespace Util
